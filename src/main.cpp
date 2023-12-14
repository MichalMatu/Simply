#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

#include <ItemSubMenu.h>
#include <LcdMenu.h>

#define LCD_ROWS 4
#define LCD_COLS 20

// Configure keyboard keys (ASCII)
#define UP 56       // NUMPAD 8
#define DOWN 50     // NUMPAD 2
#define LEFT 52     // NUMPAD 4
#define RIGHT 54    // NUMPAD 6
#define ENTER 53    // NUMPAD 5
#define BACK 55     // NUMPAD 7
#define BACKSPACE 8 // BACKSPACE
#define CLEAR 46    // NUMPAD .

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
  // set pin 39 to input mode
  pinMode(39, INPUT);

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
  if (!Serial.available())
    return;
  char command = Serial.read();

  if (command == UP)
    menu.up();
  else if (command == DOWN)
    menu.down();
  else if (command == LEFT)
    menu.left();
  else if (command == RIGHT)
    menu.right();
  else if (command == ENTER)
    menu.enter();
  else if (command == BACK)
    menu.back();
}
