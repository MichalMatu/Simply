#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

#include <ItemSubMenu.h>
#include <LcdMenu.h>

#define LCD_ROWS 4
#define LCD_COLS 20

#define UP 4
#define DOWN 15
#define LEFT 4
#define RIGHT 5
#define ENTER 53
#define BACK 55
#define BACKSPACE 8
#define CLEAR 46

extern MenuItem *settingsMenu[];

// Define the main menu
MAIN_MENU(
    ITEM_BASIC("Start service"),
    ITEM_BASIC("Connect to WiFi"),
    ITEM_SUBMENU("Settings", settingsMenu),
    ITEM_BASIC("Blink SOS"),
    ITEM_BASIC("Blink random"));
/**
 * Create submenu and precise its parent
 */
SUB_MENU(settingsMenu, mainMenu,
         ITEM_BASIC("Backlight"),
         ITEM_BASIC("Contrast"));

LcdMenu menu(LCD_ROWS, LCD_COLS);

// Define BME680 sensor
Adafruit_BME680 bme;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;
  // set pin modes, up, down, left, right and set to have internal pullups
  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(LEFT, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);

  // Initialize BME680 sensor
  if (!bme.begin())
  {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1)
      ;
  }

  menu.setupLcdWithMenu(0x27, mainMenu);
}

float temperature = 0;
float humidity = 0;
float pressure = 0;
float gasResistance = 0;

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
  sensorReadings();

  if (digitalRead(UP) == LOW)
    menu.up();
  else if (digitalRead(DOWN) == LOW)
    menu.down();
  else if (digitalRead(LEFT) == LOW)
    menu.left();
  else if (digitalRead(RIGHT) == LOW)
    menu.right();
  else if (digitalRead(ENTER) == LOW)
    menu.enter();
  else if (digitalRead(BACK) == LOW)
    menu.back();
}
