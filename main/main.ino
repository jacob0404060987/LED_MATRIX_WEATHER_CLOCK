//libraries
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "time.h"
#include <stdio.h>
#include <RGBmatrixPanel.h>
//constants
#define KELVIN 273.15
#define CLK 8
#define OE 9
#define LAT 10
#define A A0
#define B A1
#define C A2
#define D A3
//WiFI setup
//const char* ssid = "TAJNA JASKINA BABCI JADZI";
//const char* passwd = "Morys123";
const char* ssid = "Jasminowa_6_FTTH_INTER";
const char* passwd = "partur65";
//const char* ssid = "hamilton krul";
//const char* passwd = "qwerty1234";
//time setup
const char* ntpServer = "europe.pool.ntp.org";
const long gmt_strefa = 3600; //GMT+1
const int daylightsave = 3600;
//weather setup
const String weather_link = "http://api.openweathermap.org/data/2.5/weather?q=Krakow,pl&APPID=";
const String api_key = "0e11ccad42e110d8beb9583b9da92a4d";
//zmienne globalne kontrola pogody przed konwersja
float temperatura;
int wilgotnosc,cisnienie;
String statusPogody; //https://openweathermap.org/weather-conditions
//zmienne globalne czasu przed konwersja
int hours,minutes,seconds,month,day,year,wday; 
//RGBmatrixPanel matrix()


void getTime(int * year, int * month, int * day, int * wday, int * hours, int * minutes, int * seconds)
{
  struct tm timeinfo;
  //struct manual https://cplusplus.com/reference/ctime/tm/
  if(!getLocalTime(&timeinfo))
  {
    Serial.printf("Time getting failed\n");
  }else
  {
    //Serial.println(&timeinfo,"%A, %B %d %Y %H:%M:%S\n");
  }
  *year=timeinfo.tm_year;
  *month=timeinfo.tm_mon;
  *day=timeinfo.tm_mday;
  *wday=timeinfo.tm_wday;
  *hours=timeinfo.tm_hour;
  *minutes=timeinfo.tm_min;
  *seconds=timeinfo.tm_sec;
}
//funckja pogodowa z API (gotowa pod wyswietlanie)
void getWeather( float *temperature, int *pressure, int *humidity, String* weatherStatus)
{
    HTTPClient http;
  http.begin(weather_link + api_key);
  int httpCode = http.GET();
  String JSONinput=http.getString();
StaticJsonDocument<1024> doc;

DeserializationError error = deserializeJson(doc, JSONinput);

if (error) {
  Serial.print("deserializeJson() failed: ");
  Serial.println(error.c_str());
  return;
}
float coord_lon = doc["coord"]["lon"]; // 19.9167
float coord_lat = doc["coord"]["lat"]; // 50.0833

JsonObject weather_0 = doc["weather"][0];
int weather_0_id = weather_0["id"]; // 802
const char* weather_0_main = weather_0["main"]; // "Clouds"
const char* weather_0_description = weather_0["description"]; // "scattered clouds"
const char* weather_0_icon = weather_0["icon"]; // "03n"

const char* base = doc["base"]; // "stations"

JsonObject main = doc["main"];
float main_temp = main["temp"]; // 285.99
float main_feels_like = main["feels_like"]; // 285.21
float main_temp_min = main["temp_min"]; // 285.26
float main_temp_max = main["temp_max"]; // 286.78
int main_pressure = main["pressure"]; // 1004
int main_humidity = main["humidity"]; // 72

int visibility = doc["visibility"]; // 10000

float wind_speed = doc["wind"]["speed"]; // 3.6
int wind_deg = doc["wind"]["deg"]; // 250

int clouds_all = doc["clouds"]["all"]; // 40

long dt = doc["dt"]; // 1682361409

JsonObject sys = doc["sys"];
int sys_type = sys["type"]; // 2
long sys_id = sys["id"]; // 2009211
const char* sys_country = sys["country"]; // "PL"
long sys_sunrise = sys["sunrise"]; // 1682307007
long sys_sunset = sys["sunset"]; // 1682358398

int timezone = doc["timezone"]; // 7200
long id = doc["id"]; // 3094802
const char* name = doc["name"]; // "Krakow"
int cod = doc["cod"]; // 200
//przypisanie wartosci do zmiennych globalnych
*temperature=main_temp;
*pressure=main_pressure;
*humidity=main_humidity;
*weatherStatus=(String)weather_0_main;
}


  // Here were copy the strings we're interested in using to your struct data
  // It's not mandatory to make a copy, you could just use the pointers
  // Since, they are pointing inside the "content" buffer, so you need to make
  // sure it's still in memory when you read the string
  

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

void loop()
{ 
  getWeather(&temperatura, &cisnienie, &wilgotnosc,&statusPogody);
  getTime(&year, &month, &day, &wday, &hours, &minutes, &seconds);
  /*
  Serial.println("Cisnienie wynosi=");
  Serial.println(cisnienie);
  Serial.println("\nTemperatura wynosi=");
  Serial.println(temperatura);
  Serial.println("\nwilgotnosc wynosi=");
  Serial.println(wilgotnosc);
  Serial.println("\nStatus pogody to:");
  Serial.println(statusPogody);
  */
  Serial.println("****\n");
  Serial.println(hours);
  Serial.println(":");
   Serial.println(minutes);
  Serial.println(":");
   Serial.println(seconds);
  Serial.println(":");

  delay(10000);
}



