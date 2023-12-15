#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <LiquidCrystal_I2C.h>
#define UP 4
#define DOWN 15
#define LEFT 14
#define RIGHT 5
#define ENTER 12
#define BACK 13
#define BACKSPACE 8
#define CLEAR 46
// **************************************************************
float temperature = 0;
float humidity = 0;
float pressure = 0;
float gasResistance = 0;
int currentItem = 0;
unsigned long lastSensorReadingTime = 0;
const unsigned long sensorReadingInterval = 2000; // 2 seconds delay
unsigned long lastButtonPressTime = 0;
const unsigned long buttonPressDelay = 200; // 1 second delay
// **************************************************************
// Define BME680 sensor
Adafruit_BME680 bme;
// Define LCD
LiquidCrystal_I2C lcd(0x27, 20, 4);
// **************************************************************
void buttons()
{
  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(LEFT, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);
  pinMode(ENTER, INPUT_PULLUP);
  pinMode(BACK, INPUT_PULLUP);
}
void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ; // wait for serial port to connect. Needed for native USB port only

  buttons();

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
void buttonsListener()
{
  unsigned long currentTime = millis();

  // Check if at least 1 second has passed since the last button press
  if (currentTime - lastButtonPressTime >= buttonPressDelay)
  {
    if (digitalRead(UP) == LOW)
    {
      Serial.println("UP");
      lastButtonPressTime = currentTime; // Update the last button press time
    }
    else if (digitalRead(DOWN) == LOW)
    {
      Serial.println("DOWN");
      lastButtonPressTime = currentTime; // Update the last button press time
    }
    else if (digitalRead(LEFT) == LOW)
    {
      Serial.println("LEFT");
      lastButtonPressTime = currentTime; // Update the last button press time
    }
    else if (digitalRead(RIGHT) == LOW)
    {
      Serial.println("RIGHT");
      lastButtonPressTime = currentTime; // Update the last button press time
    }
    else if (digitalRead(ENTER) == LOW)
    {
      Serial.println("ENTER");
      lastButtonPressTime = currentTime; // Update the last button press time
    }
    else if (digitalRead(BACK) == LOW)
    {
      Serial.println("BACK");
      lastButtonPressTime = currentTime; // Update the last button press time
    }
  }
}

void loop()
{
  sensorReadings();
  buttonsListener();

  lcd.setCursor(0, 1);
  lcd.print(temperature);
}