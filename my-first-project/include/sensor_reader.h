#ifndef SENSOR_READER_H
#define SENSOR_READER_H
#include <ArduinoJson.h>

// Class declaration (if applicable)
class SensorReader
{
public:
  void initialize();
  float getTemperature();
  void printSensorData();
  static JsonDocument getDoc(int counter);
  static String formatNumber(int num);
  // ... other methods
private:
  // ... private variables
};

#endif