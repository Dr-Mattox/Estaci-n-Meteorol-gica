# Estación-Meteorológica
Proyecto de la materia de Microcontroladores

Este proyecto es una estación meteorológica interactiva que mide la temperatura, la humedad y la luz ambiental, y controla un ventilador en función de los umbrales establecidos para la temperatura y la humedad. Los datos se muestran en un display LCD y pueden enviarse a una computadora a través de UART.

## Funcionalidades
- Lectura de temperatura y humedad mediante un sensor **DHT11**.
- Lectura de luminosidad con un **LDR** (sensor de luz).
- Control de un ventilador utilizando **PWM** en función de los umbrales establecidos para la temperatura y la humedad.
- El sistema se comunica bidireccionalmente con la computadora vía **UART** para ajustar los umbrales y controlar el sistema.
- Visualización de los datos en un **LCD 16x2** con interfaz I2C.
  
## Componentes
- **Arduino Nano**
- **Sensor de temperatura y humedad DHT11**
- **Sensor de luminosidad (LDR)**
- **Display LCD 16x2 con I2C**
- **Ventilador de 5V**
- **Transistor 2N2222 para control del ventilador**
- **LED indicador**
- **Resistencias**: 220Ω (para el LED), 10kΩ (para el LDR)
  
## Conexiones

### 1. **Display LCD 16x2 (I2C)**
- **VCC** → Pin **5V** del Arduino Nano.
- **GND** → Pin **GND** del Arduino Nano.
- **SDA** → Pin **A4** del Arduino Nano.
- **SCL** → Pin **A5** del Arduino Nano.

### 2. **Sensor DHT11 (Temperatura y Humedad)**
- **VCC** → Pin **5V** del Arduino Nano.
- **GND** → Pin **GND** del Arduino Nano.
- **Data** → Pin **2** del Arduino Nano.

### 3. **Módulo LDR (Sensor de Luz Integrado)**
- **VCC** → Pin **5V** del Arduino Nano.
- **GND** → Pin **GND** del Arduino Nano.
- **A0** → Pin **A0** del Arduino Nano.

### 4. **Ventilador (5V)**
- **VCC del ventilador** → Colector del **transistor 2N2222**.
- **GND del ventilador** → Pin **GND** del Arduino Nano.
- **Base del transistor** → A través de una resistencia de **1kΩ** al Pin **5** del Arduino Nano.
- **Emisor del transistor** → **GND** del Arduino Nano.

### 5. **LED Indicador**
- **Anodo del LED** → Pin **10** del Arduino Nano (con resistencia de **220Ω**).
- **Cátodo del LED** → **GND** del Arduino Nano.

## Comandos UART
Puedes controlar y ajustar el sistema utilizando el **Monitor Serie** del IDE de Arduino o cualquier software de terminal.

### 1. Encender o apagar el sistema:
   - **Encender el sistema**: `SYSTEM ON`
   - **Apagar el sistema**: `SYSTEM OFF`

### 2. Ajustar el umbral de temperatura:
   - **SET TEMP XX.XX**  
   (Donde `XX.XX` es el nuevo umbral de temperatura en grados Celsius)

   Ejemplo:  
SET TEMP 28.00

### 3. Ajustar el umbral de humedad:
- **SET HUM XX.XX**  
(Donde `XX.XX` es el nuevo umbral de humedad en porcentaje)

Ejemplo:  
SET HUM 65.00

## Visualización en el LCD
- **Primera línea**: Muestra la temperatura en °C y la humedad en %.
- **Segunda línea**: Muestra el nivel de luz y la velocidad del ventilador en porcentaje.
