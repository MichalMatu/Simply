#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <LiquidCrystal_I2C.h>

// Function prototypes
void displayReadings();
void handleJoystick();

// Define BME680 sensor
Adafruit_BME680 bme;

// Define LCD
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;
  // set pin 39 to input mode
  pinMode(39, INPUT);

  // Initialize BME680 sensor
  if (!bme.begin())
  {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1)
      ;
  }

  // Initialize LCD
  lcd.init();
  lcd.backlight();
}

int16_t temperature = 0;
int16_t humidity = 0;
int16_t pressure = 0;
int16_t gasResistance = 0;

unsigned long lastSensorReadingTime = 0;
const unsigned long sensorReadingInterval = 2000; // 2 seconds

void sensorReadings()
{
  if (millis() - lastSensorReadingTime >= sensorReadingInterval)
  {
    temperature = bme.readTemperature();
    humidity = bme.readHumidity();
    pressure = bme.readPressure() / 100;
    gasResistance = bme.readGas();
    lastSensorReadingTime = millis();
  }
}

void loop()
{
  // Display sensor readings
  displayReadings();
  sensorReadings();
  // Read joystick input and navigate menu
  handleJoystick();
}

void displayReadings()
{

  lcd.setCursor(0, 0);
  lcd.print("Temperature: ");
  lcd.print(temperature);

  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(humidity);

  lcd.setCursor(0, 2);
  lcd.print("Pressure: ");
  lcd.print(pressure);

  lcd.setCursor(0, 3);
  lcd.print("API: ");
  lcd.print(gasResistance);
}

void handleJoystick()
{
  int joystickValue = analogRead(39); // Replace 39 with the actual pin number you are using
  Serial.println(joystickValue);

  // Adjust the threshold based on your joystick characteristics
  if (joystickValue < 800)
  {
    // Scroll down
    // Implement scrolling logic if you have more than 4 lines of data
  }
  else if (joystickValue > 3000)
  {
    // Scroll up
    // Implement scrolling logic if you have more than 4 lines of data
  }
}
