#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ArduinoHttpClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#ifndef STASSID
#define STASSID "DarkNet"
#define STAPSK "7pu77ies77"
#endif

#define github_host "httpbin.org"
#define github_port 80
#define ONE_WIRE_BUS D2 // DS18B20 data pin

#define LED_OFF HIGH
#define LED_ON LOW

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

const int sensorPin = D1;       // MC-38 sensor pin
const int ledPin = LED_BUILTIN; // Built-in LED pin

const char *ssid = STASSID;
const char *password = STAPSK;
int loopCounter = 0;

bool doTemp = true;
// X509List cert(cert_DigiCert_Global_Root_CA);

void setup()
{

  Serial.begin(9600);

  Serial.println("\r\n\t### STARTING!!! ###\r\n");

  pinMode(sensorPin, INPUT_PULLUP); // Enable internal pull-up resistor
  pinMode(ledPin, OUTPUT);

  sensors.begin();
  return;

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Set time via NTP, as required for x.509 validation
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2)
  {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));

  // Serial.printf("Using certificate: %s\n", cert_DigiCert_Global_Root_CA);
  // client.setTrustAnchors(&cert);

  testHttp();

  // testRawHttp();
}

void testRawHttp()
{

  WiFiClient client;
  Serial.print("Connecting to ");
  Serial.println(github_host);

  if (!client.connect(github_host, github_port))
  {
    Serial.println("Connection failed");
    return;
  }

  String url = "/ip";
  Serial.print("Requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + github_host + "\r\n" + "User-Agent: BuildFailureDetectorESP8266\r\n" + "Connection: close\r\n\r\n");

  Serial.println("Request sent");

  bool headersComplete = false;

  while (client.connected())
  {
    while (client.available())
    {
      String line = client.readStringUntil('\n');
      if (!headersComplete)
      {
        if (line == "\r")
        {
          // line is empty, so wl
          Serial.println("All Headers received");
          headersComplete = true;
        }
        else
        {
          Serial.println(line);
        }
      }
      else
      {
        Serial.println(line);
      }
    }
  }
  Serial.println("Done");
}

void testHttp()
{

  // char serverAddress[] = "192.168.0.3"; // server address
  // int port = 8080;
  WiFiClient wifi;
  HttpClient client = HttpClient(wifi, github_host, github_port);

  Serial.println("making GET request");
  client.get("/ip");
  client.sendHeader("User-Agent", "BuildFailureDetectorESP8266");

  // read the status code and body of the response

  int statusCode = client.responseStatusCode();
  int responseBufferLen = 2048;
  while (client.headerAvailable())
  {
    String headerName = client.readHeaderName();
    String headerValue = client.readHeaderValue();

    if (headerName.equals("Content-Length"))
    {
      responseBufferLen = headerValue.toInt() * 2;
    }

    Serial.print(headerName);
    Serial.print(" -> ");
    Serial.println(headerValue);
  }

  String response = client.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);

  Serial.print("responseBufferLen --> ");
  Serial.println(responseBufferLen);

  DynamicJsonDocument doc(responseBufferLen);
  deserializeJson(doc, response);

  const char *origin = doc["origin"];
  Serial.println(origin);

  Serial.println("Wait five seconds");
}

void testJson()
{

  const char *jsonString = "{\"name\":\"John\",\"age\":30,\"city\":\"New York\"}";
  DynamicJsonDocument doc(2048);
  deserializeJson(doc, jsonString);

  const char *name = doc["name"];
  int age = doc["age"];
  const char *city = doc["city"];

  Serial.println("testJson Start");
  Serial.println(name);
  Serial.println(age);
  Serial.println(city);
  Serial.println("testJson Done");
}

int counter = 0;
void loop()
{

  if (doTemp)
  {
    // if ((counter++ % 2) == 0)
    // {
    //   digitalWrite(ledPin, LED_OFF);
    //   Serial.print("LED_OFF -> ");
    // }
    // else
    // {
    //   digitalWrite(ledPin, LED_ON);
    //   Serial.print("LED_ON -> ");
    // }

    sensors.requestTemperatures();
    float temperatureC = sensors.getTempCByIndex(0);
    float temperatureF = sensors.getTempFByIndex(0);

    printTemperature(temperatureF, "(F):", temperatureC, "(C):");

    // Serial.print("F-temp:");
    // Serial.print(temperatureF);
    // //Serial.print("F ");
    // Serial.print(",");
    // Serial.print("C-temp:");
    // Serial.print(temperature);
    // Serial.println();
    // Serial.println("C ");

    if (temperatureF < 50.0 || temperatureF > 100.0)
    {
      digitalWrite(ledPin, LED_ON);
      Serial.print("LED_ON -> ");
    }
    else
    {
      digitalWrite(ledPin, LED_OFF);
      Serial.print("LED_OFF -> ");
    }

    delay(1000); // Read temperature every 1s

    return;
  }

  if (false)
  {
    int sensorState = digitalRead(sensorPin);
    if (sensorState == LOW)
    {                             // Switch is open
      digitalWrite(ledPin, HIGH); // Turn on the LED
    }
    else
    {
      digitalWrite(ledPin, LOW); // Turn off the LED
    }

    delay(100); // wait for 0.1 seconds before checking again
  }

  if ((counter++ % 2) == 0)
  {
    Serial.println("!!SET HIGH!!");
    digitalWrite(ledPin, HIGH);
    // digitalWrite(0, LOW);
    // digitalWrite(1, LOW);
    // digitalWrite(16, LOW);

    delay(10000);
  }
  else
  {
    Serial.println("!!START FLASH!!");
    for (int x = 0; x < 4; x++)
    {
      digitalWrite(ledPin, LOW);
      delay(100);
      digitalWrite(ledPin, HIGH);
      delay(100);
    }

    // digitalWrite(0, HIGH);
    // digitalWrite(1, HIGH);
    // digitalWrite(16, HIGH);
  }

  if (true)
  {
    if (false && loopCounter < 5)
    {
      Serial.print("loopCounter: ");
      Serial.println(loopCounter++);

      testHttp();
    }

    // delay(2000);
  }
}

void printTemperature(float temperatureF, String fLabel, float temperatureC, String cLabel)
{
  Serial.print(fLabel);          // Print the Fahrenheit label (e.g., "Outdoor Temp (F): ")
  Serial.print(temperatureF, 1); // Print Fahrenheit with 1 decimal place
  Serial.print(", ");
  Serial.print(cLabel);          // Print the Celsius label (e.g., "Indoor Temp (C): ")
  Serial.print(temperatureC, 1); // Print Celsius with 1 decimal place
  Serial.println();              // Move to the next line
}
