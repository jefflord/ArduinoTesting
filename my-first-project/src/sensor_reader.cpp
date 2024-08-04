#include "sensor_reader.h"

// ... implementation of SensorReader class methods ...
void SensorReader::initialize()
{
    
    Serial.println("sensor_reader initialize");
    

}

float SensorReader::getTemperature()
{
    // ... code to read temperature ...
    return 34.3;
}

// ... implementation of other functions ...
void SensorReader::printSensorData()
{
    // ... code to print sensor data ...
}

JsonDocument SensorReader::getDoc(int counter)
{

    JsonDocument doc;

    // Add values in the document
    doc["sensor"] = "gps sensor.";

    doc["counter"] = counter;
    doc["buffer"] = SensorReader::formatNumber(counter);

    Serial.print("buffer:");
    Serial.println(SensorReader::formatNumber(counter));

    // Add an array
    JsonArray data = doc["data"].to<JsonArray>();
    data.add(48.756080);
    data.add(2.302038);


    return doc;
}


String SensorReader::formatNumber(int num)
{
    String result = String(num);
    int len = result.length();
    int commaCount = (len - 1) / 3; // Calculate how many commas are needed

    if (commaCount > 0)
    {                                // Only proceed if commas are needed
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
