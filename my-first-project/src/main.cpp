#include <Arduino.h>
#include <ArduinoJson.h>

// put function declarations here:
int myFunction(int, int);
int counter = 0;
void setup()
{
  //Serial.begin(9600);
  Serial.begin(230400);
  
  while (!Serial)
    continue;
  // put your setup code here, to run once:
}

String formatNumber(int num) {
  String result = String(num);
  int len = result.length();
  int commaCount = (len - 1) / 3; // Calculate how many commas are needed
  
  if (commaCount > 0) { // Only proceed if commas are needed
    String formattedResult = "";  // Create a new string to store the formatted result
    int index = len - 1;  // Start at the end of the original string

    for (int i = 0; i < len; i++) { 
      formattedResult = result[index] + formattedResult; // Add characters in reverse
      if ((i + 1) % 3 == 0 && i != len - 1) { 
        formattedResult = "," + formattedResult; // Add a comma every 3 digits
      }
      index--;
    }

    return formattedResult; 
  } else {
    return result; // No commas needed, return the original string
  }
}

void loop()
{
  delay(1000);
  counter += 10000000 * 4;
  Serial.print(counter++);
  Serial.println();
  // put your main code here, to run repeatedly:

  JsonDocument doc;

  // Add values in the document
  doc["sensor"] = "gps";

  doc["counter"] = counter;
  doc["buffer"] = formatNumber(counter);

  Serial.print("buffer:");
  Serial.println(formatNumber(counter));

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

  // Serial.println("");
  // Serial.println("---");
  // Serial.println(ARDUINO);
  // Serial.println(__FILE__);
  // Serial.println(__DATE__);
  // Serial.println(__TIME__);
  // Serial.println(__LINE__);
  // Serial.println("---");
}

// put function definitions here:
int myFunction(int x, int y)
{
  return x + y;
}