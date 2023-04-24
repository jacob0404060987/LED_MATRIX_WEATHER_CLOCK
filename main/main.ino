#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include "time.h"
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

struct WeatherData {
  char temp[8];
  char humidity[8];
};

void printTime()
{
  struct tm timeinfo;
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
