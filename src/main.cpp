#include <ezButton.h>

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

#include <LiquidCrystal_I2C.h>

#define VRX_PIN 36 // ESP32 pin GPIO36 (ADC0) connected to VRX pin
#define VRY_PIN 39 // ESP32 pin GPIO39 (ADC0) connected to VRY pin
#define SW_PIN 17  // ESP32 pin GPIO17 connected to SW  pin

ezButton button(SW_PIN);
Adafruit_BME680 bme; // I2C

#define SEALEVELPRESSURE_HPA (1013.25)

int valueX = 0; // to store the X-axis value
int valueY = 0; // to store the Y-axis value
int bValue = 0; // To store value of the button

void setup()
{
  Serial.begin(115200);
  button.setDebounceTime(50); // set debounce time to 50 milliseconds

  while (!Serial)
    ;
  Serial.println(F("BME680 async test"));

  if (!bme.begin())
  {
    Serial.println(F("Could not find a valid BME680 sensor, check wiring!"));
    while (1)
      ;
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
}

void loop()
{
  button.loop(); // MUST call the loop() function first

  // read X and Y analog values
  valueX = analogRead(VRX_PIN);
  valueY = analogRead(VRY_PIN);

  // Read the button value
  bValue = button.getState();

  if (button.isPressed())
  {
    Serial.println("The button is pressed");
    // TODO do something here
  }

  if (button.isReleased())
  {
    Serial.println("The button is released");
    // TODO do something here
  }

  // print data to Serial Monitor on Arduino IDE
  // Serial.print("x = ");
  // Serial.print(valueX);
  // Serial.print(", y = ");
  // Serial.print(valueY);
  // Serial.print(" : button = ");
  // Serial.println(bValue);

  // Tell BME680 to begin measurement.
  unsigned long endTime = bme.beginReading();
  if (endTime == 0)
  {
    Serial.println(F("Failed to begin reading :("));
    return;
  }
  Serial.print(F("Reading started at "));
  Serial.print(millis());
  Serial.print(F(" and will finish at "));
  Serial.println(endTime);

  Serial.println(F("You can do other work during BME680 measurement."));
  delay(2000); // This represents parallel work.
  // There's no need to delay() until millis() >= endTime: bme.endReading()
  // takes care of that. It's okay for parallel work to take longer than
  // BME680's measurement time.

  // Obtain measurement results from BME680. Note that this operation isn't
  // instantaneous even if milli() >= endTime due to I2C/SPI latency.
  if (!bme.endReading())
  {
    Serial.println(F("Failed to complete reading :("));
    return;
  }
  Serial.print(F("Reading completed at "));
  Serial.println(millis());

  Serial.print(F("Temperature = "));
  Serial.print(bme.temperature);
  Serial.println(F(" *C"));

  Serial.print(F("Pressure = "));
  Serial.print(bme.pressure / 100.0);
  Serial.println(F(" hPa"));

  Serial.print(F("Humidity = "));
  Serial.print(bme.humidity);
  Serial.println(F(" %"));

  Serial.print(F("Gas = "));
  Serial.print(bme.gas_resistance / 1000.0);
  Serial.println(F(" KOhms"));

  Serial.print(F("Approx. Altitude = "));
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(F(" m"));

  Serial.println();
  delay(2000);
}
