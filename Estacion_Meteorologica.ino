#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// Configuración del LCD con I2C (Dirección del LCD, columnas, filas)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Definiciones para el sensor DHT11
#define DHTPIN 2  // Pin de datos del DHT11
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Definiciones para el LDR
#define LDR_PIN A0 // Pin analógico donde está conectado el LDR

// Definiciones para el ventilador y LED
#define FAN_PIN 5  // Pin PWM para el ventilador
#define LED_PIN 10   // Pin para el LED indicador

// Umbrales de temperatura y humedad
float tempThreshold = 25.0;  // Umbral de temperatura en °C
float humThreshold = 60.0;   // Umbral de humedad en %

bool systemOn = true;  // Estado del sistema

void setup() {

  // Inicializar comunicación serial
  Serial.begin(9600);

  // Inicializar el LCD
  lcd.init();
  lcd.backlight();
  
  // Inicializar el DHT11
  dht.begin();

  // Configurar pines
  pinMode(LDR_PIN, INPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  // Mensaje inicial en el LCD
  lcd.setCursor(0, 0);
  lcd.print("Estacion Meteo");
  lcd.setCursor(0, 1);
  lcd.print("Iniciando...");
  delay(2000);
}

void loop() {
  if (systemOn) {
    // Leer datos del DHT11
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // Verificar si la lectura es válida
    if (isnan(temperature) || isnan(humidity)) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error sensor DHT");
      Serial.println("Error: No se puede leer el sensor DHT11.");
    } else {
      // Leer el valor del LDR (luminosidad)
      int lightLevel = analogRead(LDR_PIN);

      // Convertir temperatura y humedad a cadenas con dos decimales
      char tempBuffer[7];  // Para almacenar "XX.XX"
      char humBuffer[7];   // Para almacenar "XX.XX"
      dtostrf(temperature, 5, 2, tempBuffer);  // Formato de 5 caracteres en total, 2 decimales
      dtostrf(humidity, 5, 2, humBuffer);      // Formato de 5 caracteres en total, 2 decimales

      // Mostrar temperatura y humedad en la primera línea con formato XX.XX
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("T:");
      lcd.print(tempBuffer);
      lcd.print("C H:");
      lcd.print(humBuffer);
      lcd.print("%");

      // Ajuste proporcional de la velocidad del ventilador
      int fanSpeed = 0;  // Velocidad del ventilador (0 - 255)

      // Comprobar si la temperatura o humedad superan los umbrales
      if (temperature > tempThreshold) {
        // Calcular la diferencia de temperatura y ajustar la velocidad proporcionalmente
        fanSpeed = map(temperature, tempThreshold, tempThreshold + 10, 0, 255); // Incrementa la velocidad si se excede el umbral
        if (fanSpeed > 255) fanSpeed = 255;  // Limitar a 255
      }

      if (humidity > humThreshold) {
        // Calcular la diferencia de humedad y ajustar la velocidad proporcionalmente
        int humFanSpeed = map(humidity, humThreshold, humThreshold + 20, 0, 255);  // Ajuste de velocidad para la humedad
        if (humFanSpeed > 255) humFanSpeed = 255;  // Limitar a 255

        // Elige el valor más alto entre la temperatura y la humedad
        if (humFanSpeed > fanSpeed) {
          fanSpeed = humFanSpeed;
        }
      }

      // Aplicar la velocidad al ventilador
      analogWrite(FAN_PIN, fanSpeed);

      // Mostrar luminosidad y velocidad del ventilador en la segunda línea
      lcd.setCursor(0, 1);
      lcd.print("Luz:");
      lcd.print(lightLevel);
      lcd.print(" Vel:");
      lcd.print(map(fanSpeed, 0, 255, 0, 100));  // Convertir a porcentaje (0 - 100%)
      lcd.print("%");

      // Enviar los datos por UART a la computadora
      Serial.print("T:");
      Serial.print(temperature);
      Serial.print(" H:");
      Serial.print(humidity);
      Serial.print(" L:");
      Serial.print(lightLevel);
      Serial.println("\r");
      //Serial.print(" FanSpeed: ");
      //Serial.println(fanSpeed);

      // Indicar que el sistema está encendido
      digitalWrite(LED_PIN, HIGH);
    }

    // Esperar 1 segundo antes de la siguiente lectura
    delay(1000);
  } else {
    // Si el sistema está apagado
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sistema apagado");

    // Apagar ventilador y LED
    analogWrite(FAN_PIN, 0);
    digitalWrite(LED_PIN, LOW);
  }

  // Comprobar si llegan comandos por UART
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');  // Leer hasta nueva línea

    // Limpiar cualquier salto de línea o retorno de carro extra
    command.trim();

    // Comandos para encender/apagar el sistema
    if (command == "SYSTEM ON") {
      systemOn = true;
      Serial.println("Sistema encendido");
    } else if (command == "SYSTEM OFF") {
      systemOn = false;
      Serial.println("Sistema apagado");
    }

    // Comandos para ajustar umbrales
    if (command.startsWith("SET TEMP")) {
      float newTempThreshold = command.substring(9).toFloat();
      if (newTempThreshold >= 0 && newTempThreshold <= 50) {  //solo lo ajusta si son valores con sentido
        tempThreshold = newTempThreshold;
        Serial.print("Nuevo umbral de temperatura: ");
        Serial.println(tempThreshold);
      }
    }

    if (command.startsWith("SET HUM")) {
      float newHumThreshold = command.substring(8).toFloat();
      if (newHumThreshold >= 0 && newHumThreshold <= 100) {   //solo lo ajusta si va de 0 a 100 porciento
        humThreshold = newHumThreshold;
        Serial.print("Nuevo umbral de humedad: ");
        Serial.println(humThreshold);
      }
    }
  }
}
