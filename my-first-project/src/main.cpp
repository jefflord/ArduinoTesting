#include <Arduino.h>
#include <ArduinoJson.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "sensor_reader.h"

#define LED_OFF LOW
#define LED_ON HIGH
#define RELAY_OFF LOW
#define RELAY_ON HIGH
#define ONE_WIRE_BUS D2 // DS18B20 data pin
const int ledPin = LED_BUILTIN; // Built-in LED pin

int counter = 0;

SensorReader sensor;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup()
{

  Serial.begin(230400);

  while (!Serial)
    continue;

  pinMode(ledPin, OUTPUT);
  sensors.begin();
}

void printTemperature(int num, String label, float temperatureF, String fLabel, float temperatureC, String cLabel)
{
  Serial.print(label); // Print the Fahrenheit label (e.g., "Outdoor Temp (F): ")
  Serial.print(num);   // Print Fahrenheit with 1 decimal place
  Serial.print(", ");
  Serial.print(fLabel);          // Print the Fahrenheit label (e.g., "Outdoor Temp (F): ")
  Serial.print(temperatureF, 1); // Print Fahrenheit with 1 decimal place
  Serial.print(", ");
  Serial.print(cLabel);          // Print the Celsius label (e.g., "Indoor Temp (C): ")
  Serial.print(temperatureC, 1); // Print Celsius with 1 decimal place
  Serial.println();              // Move to the next line
}

String localFormatNumber(int num)
{
  String result = String(num);
  int len = result.length();
  int commaCount = (len - 1) / 3; // Calculate how many commas are needed

  if (commaCount > 0)
  {                              // Only proceed if commas are needed
    String formattedResult = ""; // Create a new string to store the formatted result
    int index = len - 1;         // Start at the end of the original string

    for (int i = 0; i < len; i++)
    {
      formattedResult = result[index] + formattedResult; // Add characters in reverse
      if ((i + 1) % 3 == 0 && i != len - 1)
      {
        formattedResult = "," + formattedResult; // Add a comma every 3 digits
      }
      index--;
    }

    return formattedResult;
  }
  else
  {
    return result; // No commas needed, return the original string
  }
}

void doTemp(int counter)
{

  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  printTemperature(counter, "Counter:", temperatureF, "(F):", temperatureC, "(C):");
}

void loop()
{
  counter += 1;
  delay(1000);

  if ((counter % 2) == 0)
  {
    digitalWrite(ledPin, LED_OFF);
  }
  else
  {
    digitalWrite(ledPin, LED_ON);
  }

  doTemp(counter);

  return;
  counter += 10000000 * 4;
  Serial.println();
  // put your main code here, to run repeatedly:

  if (!true)
  {

    JsonDocument doc;
    // Add values in the document
    doc["sensor"] = "gps!!!";

    doc["counter"] = counter;
    doc["buffer"] = localFormatNumber(counter);

    Serial.print("buffer:");
    Serial.println(localFormatNumber(counter));

    // Add an array
    JsonArray data = doc["data"].to<JsonArray>();
    data.add(48.756080);
    data.add(2.302038);

    // Generate the minified JSON and send it to the Serial port
    serializeJson(doc, Serial);
    // The above line prints:
    // {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}

    // Start a new line
    Serial.println();

    // Generate the prettified JSON and send it to the Serial port
    serializeJsonPretty(doc, Serial);
  }
  else
  {

    JsonDocument doc = SensorReader::getDoc(counter);
    serializeJson(doc, Serial);
    Serial.println();
    serializeJsonPretty(doc, Serial);
  }
  // Serial.println("");
  // Serial.println("---");
  // Serial.println(ARDUINO);
  // Serial.println(__FILE__);
  // Serial.println(__DATE__);
  // Serial.println(__TIME__);
  // Serial.println(__LINE__);
  // Serial.println("---");
}
