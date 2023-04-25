//libraries
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include "time.h"
//constants
//WiFI setup
const char* ssid = "TAJNA JASKINA BABCI JADZI";
const char* passwd = "Morys123";
//time setup
const char* ntpServer = "europe.pool.ntp.org";
const long gmt_strefa = 3600; //GMT+1
const int daylightsave = 3600;
//weather setup
const String weather_link = "http://api.openweathermap.org/data/2.5/weather?q=Krakow,pl&APPID=";
const String api_key = "0e11ccad42e110d8beb9583b9da92a4d";
void printTime()
{
  struct tm timeinfo;
  //struct manual https://cplusplus.com/reference/ctime/tm/
  if(!getLocalTime(&timeinfo))
  {
    Serial.printf("Time getting failed\n");
  }else
  {
    Serial.println(&timeinfo,"%A, %B %d %Y %H:%M:%S\n");
  }
}

bool readReponseContent(struct clientData* clientData) {
  // Compute optimal size of the JSON buffer according to what we need to parse.
  // See https://bblanchon.github.io/ArduinoJson/assistant/
  const size_t bufferSize = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 
      2*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + 
      JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(12) + 390;
  DynamicJsonBuffer jsonBuffer(bufferSize);

  JsonObject& root = jsonBuffer.parseObject(client);
}
  if (!root.success()) {
    Serial.println("JSON parsing failed!");
    return false;
  }

  // Here were copy the strings we're interested in using to your struct data
      strcpy(clientData->temp, root["main"]["temp"]);
  strcpy(clientData->humidity, root["main"]["humidity"]);
  // It's not mandatory to make a copy, you could just use the pointers
  // Since, they are pointing inside the "content" buffer, so you need to make
  // sure it's still in memory when you read the string

  return true;
}
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid,passwd);
  while(WiFi.status()!=WL_CONNECTED)
  {
    delay(500);
    Serial.printf(".");
  }
  if(WiFi.status()==WL_CONNECTED)
    Serial.printf("Polaczono\n");
  
  configTime(gmt_strefa, daylightsave, ntpServer);
//pobranie danych z klineta http
    HTTPClient http;
  http.begin(weather_link + api_key);
  int httpCode = http.GET();
  String payload=http.getString();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  //printTime();

  Serial.println(payload);
 JSONVar my_json = JSON.parse(payload);
 JSONVar keys = my_json.keys();
 Serial.println("Dana z Dzejsona");
   Serial.printf("\n");

 delay(30000);
}


/*Json from weather API
{
  "coord": {
    "lon": 19.9167,
    "lat": 50.0833
  },
  "weather": [
    {
      "id": 802,
      "main": "Clouds",
      "description": "scattered clouds",
      "icon": "03n"
    }
  ],
  "base": "stations",
  "main": {
    "temp": 285.99,
    "feels_like": 285.21,
    "temp_min": 285.26,
    "temp_max": 286.78,
    "pressure": 1004,
    "humidity": 72
  },
  "visibility": 10000,
  "wind": {
    "speed": 3.6,
    "deg": 250
  },
  "clouds": {
    "all": 40
  },
  "dt": 1682361409,
  "sys": {
    "type": 2,
    "id": 2009211,
    "country": "PL",
    "sunrise": 1682307007,
    "sunset": 1682358398
  },
  "timezone": 7200,
  "id": 3094802,
  "name": "Krakow",
  "cod": 200
}
JSON filter:
{
  "coord": false,
  "weather": [
    {
      "id": false,
      "main": true,
      "description": false,
      "icon": false
    }
  ],
  "base": "stations",
  "main": {
    "temp": true,
    "feels_like": false,
    "temp_min": false,
    "temp_max": false,
    "pressure": true,
    "humidity": true
  },
  "visibility": false,
  "wind": false,
  "clouds": false,
  "dt": false,
  "sys": false,
  "timezone": false,
  "id": false,
  "name": false,
  "cod": false
}
*/


//filterd JSON parse code
/*
// String payload;

StaticJsonDocument<112> filter;
filter["coord"] = false;

JsonObject filter_weather_0 = filter["weather"].createNestedObject();
filter_weather_0["id"] = false;
filter_weather_0["main"] = true;
filter_weather_0["description"] = false;
filter_weather_0["icon"] = false;
filter["base"] = "stations";

JsonObject filter_main = filter.createNestedObject("main");
filter_main["temp"] = true;
filter_main["feels_like"] = false;
filter_main["temp_min"] = false;
filter_main["temp_max"] = false;
filter_main["pressure"] = true;
filter_main["humidity"] = true;
filter["visibility"] = false;
filter["wind"] = false;
filter["clouds"] = false;
filter["dt"] = false;
filter["sys"] = false;
filter["timezone"] = false;
filter["id"] = false;
filter["name"] = false;
filter["cod"] = false;

StaticJsonDocument<192> doc;

DeserializationError error = deserializeJson(doc, payload, DeserializationOption::Filter(filter));

if (error) {
  Serial.print("deserializeJson() failed: ");
  Serial.println(error.c_str());
  return;
}

const char* weather_0_main = doc["weather"][0]["main"]; // "Clouds"

JsonObject main = doc["main"];
float main_temp = main["temp"]; // 285.99
int main_pressure = main["pressure"]; // 1004
int main_humidity = main["humidity"]; // 72

*/
