#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include <OneButton.h>

// Configuración Wi-Fi
const char* ssid = "Leyton";
const char* password = "Surirancho2734";

// Configuración MQTT
const char* mqtt_server = "192.168.100.171";
const char* topic = "sensor/data";

// Configuración del DHT11
#define DHTPIN 15
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Configuración del relé
#define RELAY_PIN 5

// Configuración del LED integrado
const int ledPin = 2;

// Configuración LCD
#define LCD_I2C_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2
LiquidCrystal_PCF8574 lcd(LCD_I2C_ADDRESS);

// Configuración del botón
#define BUTTON_PIN 23
OneButton button(BUTTON_PIN, true);  // Configuración del botón (true: lógica invertida)

// WiFi y MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// Variables de estado
bool relayState = false;
bool temperatureControlEnabled = true;
bool autoDisplayActive = false;  // Para controlar el mensaje temporal en la pantalla
unsigned long autoDisplayTime = 0;

// Función para actualizar el estado del relé y el LED
void updateRelayAndLED() {
  digitalWrite(RELAY_PIN, relayState ? HIGH : LOW);
  digitalWrite(ledPin, relayState ? HIGH : LOW);
  updateLCD();
  logToSerial();
}

// Función para actualizar la pantalla LCD
void updateLCD() {
  if (autoDisplayActive) {
    return;  // No actualizar la pantalla si el mensaje de "Auto" está activo
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(dht.readTemperature());
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Hum: ");
  lcd.print(dht.readHumidity());
  lcd.print("% R:");
  lcd.print(relayState ? "ON" : "OFF");
}

// Función para mostrar el estado del modo automático
void displayAutoState() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Modo Automatico");
  lcd.setCursor(0, 1);
  lcd.print(temperatureControlEnabled ? "Activado" : "Desactivado");

  autoDisplayActive = true;
  autoDisplayTime = millis();  // Guardar el tiempo de inicio del mensaje
}

// Función para registrar información en el monitor serial
void logToSerial() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.print(" °C, Humedad: ");
  Serial.print(humidity);
  Serial.print(" %, Relé: ");
  Serial.println(relayState ? "ON" : "OFF");

  Serial.print("Modo Automático: ");
  Serial.println(temperatureControlEnabled ? "Activado" : "Desactivado");
}

// Función para manejar pulsación corta
void handleClick() {
  if (temperatureControlEnabled) {
    // Si el modo automático está activado, no permitir el cambio de estado del relé
    Serial.println("Pulsación corta: Modo automático activado. No se puede cambiar el estado del relé.");
    return;
  }

  relayState = !relayState;  // Alternar estado del relé
  updateRelayAndLED();
  Serial.println("Pulsación corta: Cambio en estado del relé.");
}

// Función para manejar presión larga
void handleLongPress() {
  temperatureControlEnabled = !temperatureControlEnabled;  // Alternar modo automático
  displayAutoState();  // Mostrar el mensaje temporal
  Serial.println(temperatureControlEnabled ? "Modo automático activado." : "Modo automático desactivado.");
  updateRelayAndLED();
}

// Función para conectarse a Wi-Fi
void setup_wifi() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Conectando WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    lcd.setCursor(0, 1);
    lcd.print(".");
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi conectado!");
  delay(2000);
  lcd.clear();
}

// Función para reconectar al broker MQTT
void reconnect() {
  while (!client.connected()) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Conectando MQTT...");
    if (client.connect("ESP32Client")) {
      lcd.setCursor(0, 1);
      lcd.print("MQTT conectado!");
      delay(2000);
    } else {
      lcd.setCursor(0, 1);
      lcd.print("Fallo MQTT!");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  // Configurar el pin del LED
  pinMode(ledPin, OUTPUT);

  // Configurar pantalla LCD
  lcd.begin(LCD_COLUMNS, LCD_ROWS);
  lcd.setBacklight(255);

  // Configurar Wi-Fi y MQTT
  setup_wifi();
  client.setServer(mqtt_server, 1883);

  // Configurar DHT11 y relé
  dht.begin();
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  // Configurar el botón
  button.attachClick(handleClick);           // Pulsación corta
  button.attachLongPressStart(handleLongPress);  // Presión larga
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Leer datos del sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Verificar lectura válida
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Error leyendo el sensor DHT11.");
    return;
  }

  // Control del relé basado en temperatura si el modo automático está activado
  if (temperatureControlEnabled) {
    relayState = (temperature >= 20.0);
    updateRelayAndLED();
  }

  // Crear mensaje JSON
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["temperature"] = temperature;
  jsonDoc["humidity"] = humidity;
  jsonDoc["relay"] = relayState ? "ON" : "OFF";
  jsonDoc["automatic"] = temperatureControlEnabled ? "Enabled" : "Disabled";

  char buffer[256];
  serializeJson(jsonDoc, buffer);
  client.publish(topic, buffer);

  // Mostrar en el monitor serial
  logToSerial();

  // Procesar eventos del botón
  button.tick();

  // Restaurar pantalla principal después de 4 segundos si el mensaje de "Auto" está activo
  if (autoDisplayActive && millis() - autoDisplayTime >= 4000) {
    autoDisplayActive = false;
    updateLCD();  // Volver a la pantalla principal después de 4 segundos
  }

  delay(100);  // Reducir la frecuencia del loop
}
