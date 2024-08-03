#include <Arduino.h>
#include <ArduinoJson.h>

// put function declarations here:
int myFunction(int, int);
int counter = 0;
void setup()
{
  Serial.begin(9600);
  while (!Serial)
    continue;
  // put your setup code here, to run once:
}

void loop()
{
  delay(1000);
  Serial.print(counter++);
  Serial.println();
  // put your main code here, to run repeatedly:

  JsonDocument doc;

  // Add values in the document
  doc["sensor"] = "gps";
  doc["time"] = 1351824120 + counter;

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

  Serial.println("");
  Serial.println("---");
  Serial.println(ARDUINO);
  Serial.println(__FILE__);
  Serial.println(__DATE__);
  Serial.println(__TIME__);
  Serial.println(__LINE__);
  Serial.println("---");
}

// put function definitions here:
int myFunction(int x, int y)
{
  return x + y;
}