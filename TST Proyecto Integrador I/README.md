# Proyecto Integrador I

## Solución IoT Basada en MQTT con Visualización en Grafana

Este proyecto implementa una solución IoT utilizando el ESP32, un sensor DHT11 para medir temperatura y humedad, un módulo relé para el control de dispositivos, una pantalla LCD 1602 con interfaz I2C para visualización local, y un pulsador para alternar entre modos de operación. El sistema envía los datos recolectados a un servidor MQTT local, donde se procesan y almacenan utilizando Telegraf e InfluxDB. Los datos se visualizan en tiempo real mediante dashboards en Grafana.

## Contenido del Repositorio

- **Carpeta `src`**: Contiene el código fuente del programa desarrollado para el ESP32.
- **Carpeta `docs`**: Incluye la documentación del proyecto, como el informe del Trabajo Práctico y otros documentos relevantes.
- **Carpeta `images`**: Contiene capturas de pantalla del sistema en funcionamiento y otros recursos visuales.
- **Archivo `README.md`**: Documentación general del proyecto.

## Requisitos de Hardware

- **ESP32-S WROOM32**: Microcontrolador principal.
- **Sensor DHT11**: Para medir temperatura y humedad.
- **Módulo Relé**: Controla dispositivos externos.
- **Pantalla LCD 1602 con interfaz I2C**: Muestra datos locales.
- **Pulsador**: Permite cambiar entre modos de operación.
- **Fuente de alimentación**: Para alimentar el ESP32 y otros componentes.
- **Esquemático de conexiones**:
  - **DHT11**:
    - VCC a 3.3V del ESP32.
    - GND a GND del ESP32.
    - Data a GPIO15.
  - **Módulo Relé**:
    - Señal a GPIO5 del ESP32.
    - VCC a 5V.
    - GND a GND del ESP32.
  - **Pantalla LCD 1602 I2C**:
    - SDA a GPIO21.
    - SCL a GPIO22.
    - VCC a 5V.
    - GND a GND.
  - **Pulsador**:
    - Un terminal a GPIO23.
    - Otro terminal a GND (con lógica invertida).

## Requisitos de Software

- **Arduino IDE**: Para programar el ESP32.
- **Librerías Necesarias**:
  - `LiquidCrystal_PCF8574` para la pantalla LCD I2C.
  - `DHT` para el sensor DHT11.
  - `PubSubClient` para MQTT.
  - `ArduinoJson` para manejo de JSON.
  - `OneButton` para manejar el pulsador.
- **Servicios Necesarios**:
  - MQTT Mosquitto
  - Telegraf
  - InfluxDB
  - Grafana

## Uso del Sistema IoT

1. **Configuración del Hardware**:
   - Conecta el sensor DHT11, el módulo relé, la pantalla LCD 1602 y el pulsador al ESP32 según el esquema de conexiones.
   - Alimenta el ESP32 y verifica que el dispositivo esté funcionando.

2. **Configuración del Software**:
   - Sube el código al ESP32 utilizando el Arduino IDE.
   - Configura el servidor MQTT, Telegraf, InfluxDB y Grafana según las instrucciones del proyecto.

3. **Operación del Sistema**:
   - Conecta el ESP32 a una red Wi-Fi.
   - El dispositivo comenzará a leer los datos de temperatura y humedad del sensor DHT11.
   - Los datos se enviarán al servidor MQTT y se visualizarán en los dashboards de Grafana.
   - Utiliza el pulsador para alternar entre modos manual y automático.

## Modo de Uso del Pulsador

El pulsador permite interactuar con el sistema de dos maneras:

- **Pulsación corta**:
  - Si el sistema está en modo manual, cambia el estado del relé (encendido/apagado).
  - Si el sistema está en modo automático, esta acción no tiene efecto y se mostrará un mensaje en el monitor serial indicando que el cambio no es permitido.

- **Pulsación larga**:
  - Cambia entre el modo automático y el modo manual.
  - Al cambiar de modo, se muestra un mensaje temporal en la pantalla LCD indicando el estado actual del sistema ("Modo Automático Activado/Desactivado").

## Características del Sistema

- **Modo Automático**:
  - Controla el módulo relé basado en la temperatura (e.g., activa el relé si la temperatura supera los 20 °C).
- **Modo Manual**:
  - Permite alternar manualmente el estado del relé mediante un pulsador.
- **Visualización Local**:
  - Una pantalla LCD 1602 muestra los datos de temperatura, humedad, estado del relé y el modo de operación actual.
- **Visualización Remota**:
  - Dashboards en Grafana muestran los datos en tiempo real y permiten análisis histórico.

## Capturas de Pantalla

![Dashboards - Grafana](https://github.com/user-attachments/assets/c1d3c001-d30f-4c28-9fc0-6af4e2cef20f)

![ESP32 lcd](https://github.com/user-attachments/assets/a190ddbf-5847-4a5b-93c0-ea9ed5da7ee7)

![DATOS VISUALIZADOS](https://github.com/user-attachments/assets/5123853b-5bae-4871-a6e1-def2b55571d6)

Capturas de pantalla del sistema en funcionamiento.

## Enlace al Video del Proyecto

Puedes ver el funcionamiento del sistema en el siguiente enlace:
[[Enlace al video](https://drive.google.com/file/d/1ZTxHqVcLMl7sM4bdJk6Gz_pVQZ7wZuS2/view?usp=drivesdk)](#).

## Contribuciones

Las contribuciones son bienvenidas. Si deseas mejorar este proyecto:
- Realiza un *fork* del repositorio.
- Envía tus cambios mediante un *pull request*.
- Abre un *issue* para discutir ideas o problemas.

## Licencia

Este proyecto está licenciado bajo la [MIT License](LICENSE).

