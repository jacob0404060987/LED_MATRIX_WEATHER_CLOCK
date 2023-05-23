//libraries
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "time.h"
#include <stdio.h>
#include <RGBmatrixPanel.h>
#include "esp32-hal-timer.h"
#include "driver/timer.h"
#include "soc/soc_caps.h"

#define CLK  15
#define OE   32
#define LAT 33 
#define A   12
#define B   4
#define C   5
#define D   2


//constants
#define KELVIN 273.15
//WiFI setup
const char* ssid = "TAJNA JASKINA BABCI JADZI";
const char* passwd = "Morys123";
//const char* ssid = "Jasminowa_6_FTTH_INTER";
//const char* passwd = "partur65";
//const char* ssid = "hamilton krul";
//const char* passwd = "qwerty1234";
//time setup
const char* ntpServer = "europe.pool.ntp.org";
const long gmt_strefa = 3600; //GMT+1
const int daylightsave = 3600;
//weather setup
const String weather_link = "http://api.openweathermap.org/data/2.5/weather?q=Krakow,pl&APPID=";
const String api_key = "0e11ccad42e110d8beb9583b9da92a4d";
HTTPClient http;
//zmienne globalne kontrola pogody przed konwersja
float temperatura;
int wilgotnosc,cisnienie;
String statusPogody; //https://openweathermap.org/weather-conditions
//zmienne globalne czasu przed konwersja
int hours,minutes,seconds,month,day,year,wday; 
String Sweekday,Smonth;
//timery
hw_timer_t *My_timer = NULL;
int counter;
//kolory

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
  if(*seconds!=timeinfo.tm_sec)
  {
  *year=timeinfo.tm_year;
  *month=timeinfo.tm_mon;
  *day=timeinfo.tm_mday;
  *wday=timeinfo.tm_wday;
  *hours=timeinfo.tm_hour;
  *minutes=timeinfo.tm_min;
  *seconds=timeinfo.tm_sec;
  
  }
}
RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);
//kolory
uint16_t WHITE=matrix.Color888(0xFF,0xFF,0xff);
uint16_t YELLOW=matrix.Color888(0xEE,0xFF,0x00);
uint16_t BLUE=matrix.Color888(0x0B,0x91,0xD9);
uint16_t BLACK=matrix.Color888(0x00,0x00,0x00);

/*
uint16_t WHITE=matrix.Color888(0xff*0.1,0xff*0.1,0xff*0.1);
uint16_t YELLOW=matrix.Color888(0xEE*0.1,0xFF*0.1,0x00*0.1);
uint16_t BLUE=matrix.Color888(0x0B*0.1,0x91*0.1,0xD9*0.1);
uint16_t BLACK=matrix.Color888(0x00*0.1,0x00*0.1,0x00*0.1);
*/
//funckja pogodowa z API (gotowa pod wyswietlanie)
void dimmer()
{
  if(hours>=22 || hours<7)
  {
WHITE=matrix.Color888(0xFF*0.07,0xFF*0.07,0xff*0.07);
YELLOW=WHITE;
BLUE=matrix.Color888(0x0B*0.07,0x91*0.07,0xD9*0.07);
BLACK=matrix.Color888(0x00*0.07,0x00*0.07,0x00*0.07);
  }
  else
  {
 WHITE=matrix.Color888(0xFF,0xFF,0xff);
 YELLOW=matrix.Color888(0xEE,0xFF,0x00);
 BLUE=matrix.Color888(0x0B,0x91,0xD9);
 BLACK=matrix.Color888(0x00,0x00,0x00);
  }
}

void getWeather( float *temperature, int *pressure, int *humidity, String* weatherStatus)
{
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
void printSunny()
{
  matrix.fillRect(48, 21, 15, 11,BLACK);
  matrix.fillCircle(56, 26, 5, matrix.Color888(17,18,0));
}
void printClouds()
{
  matrix.fillRect(48, 21, 15, 11,BLACK);
  matrix.fillRect(52, 24, 9,7,WHITE);
  for(int n=0; n<6;n++)
  {
    matrix.drawPixel(51, 25+n, WHITE);
  }
  for(int n=0; n<4;n++)
  {
    matrix.drawPixel(50, 26+n, WHITE);
  }
    for(int n=0; n<2;n++)
  {
    matrix.drawPixel(49, 27+n, WHITE);
  }
      for(int n=0; n<6;n++)
  {
    matrix.drawPixel(61, 24+n, WHITE);
  }
  for(int n=0; n<3;n++)
  {
    matrix.drawPixel(62, 25+n, WHITE);
  }
  for(int n=0; n<6;n++)
  {
    matrix.drawPixel(54+n, 23, WHITE);
  }
  for(int n=0; n<4;n++)
  {
    matrix.drawPixel(55+n, 22, WHITE);
  }
}
void printDrizzle()
{
  matrix.fillRect(48, 21, 15, 11, BLACK);
  matrix.fillRect(51, 24, 9, 3, WHITE);
  for(int n=0; n<8;n++)
  {
    matrix.drawPixel(52+n, 23, WHITE);
  }
  for(int n=0; n<5;n++)
  {
    matrix.drawPixel(53+n, 22, WHITE);
  }
  for(int n=0; n<2;n++)
  {
    matrix.drawPixel(54+n, 21, WHITE);
  }
    for(int n=0; n<2;n++)
  {
    matrix.drawPixel(60, 24+n, WHITE);
  }
  matrix.drawPixel(50, 25, WHITE);
    matrix.drawPixel(51, 27, BLUE);
  matrix.drawPixel(52, 29, BLUE);
  matrix.drawPixel(54, 28, BLUE);
  matrix.drawPixel(54, 30, BLUE);
  matrix.drawPixel(56, 27, BLUE);
    matrix.drawPixel(56, 29, BLUE);
  matrix.drawPixel(58, 30, BLUE);
  matrix.drawPixel(59, 28, BLUE);
  
}
void printRain()
{
  matrix.fillRect(48, 21, 15, 11, BLACK);
  matrix.fillRect(51, 24, 9, 3, matrix.Color888(0x87,0x82,0x7d));
  for(int n=0; n<8;n++)
  {
    matrix.drawPixel(52+n, 23, matrix.Color888(0x87,0x82,0x7d));
  }
  for(int n=0; n<5;n++)
  {
    matrix.drawPixel(53+n, 22, matrix.Color888(0x87,0x82,0x7d));
  }
  for(int n=0; n<2;n++)
  {
    matrix.drawPixel(54+n, 21, matrix.Color888(0x87,0x82,0x7d));
  }
    for(int n=0; n<2;n++)
  {
    matrix.drawPixel(60, 24+n, matrix.Color888(0x87,0x82,0x7d));
  }
  matrix.drawPixel(50, 25, matrix.Color888(0x87,0x82,0x7d));
  for(int n=0;n<4;n++)
  {
    matrix.drawPixel(52+2*n, 27, BLUE);
  }
    for(int n=0;n<5;n++)
  {
    matrix.drawPixel(51+2*n, 28, BLUE);
  }
    for(int n=0;n<4;n++)
  {
    matrix.drawPixel(52+2*n, 29, BLUE);
  }
    for(int n=0;n<4;n++)
  {
    matrix.drawPixel(53+2*n, 30, BLUE);
  }
}
void printThunder()
{
  matrix.fillRect(48, 21, 15, 11, BLACK);
  matrix.fillRect(51, 24, 9, 3, matrix.Color888(0x87,0x82,0x7d));
  for(int n=0; n<8;n++)
  {
    matrix.drawPixel(52+n, 23, matrix.Color888(0x87,0x82,0x7d));
  }
  for(int n=0; n<5;n++)
  {
    matrix.drawPixel(53+n, 22, matrix.Color888(0x87,0x82,0x7d));
  }
  for(int n=0; n<2;n++)
  {
    matrix.drawPixel(54+n, 21, matrix.Color888(0x87,0x82,0x7d));
  }
    for(int n=0; n<2;n++)
  {
    matrix.drawPixel(60, 24+n, matrix.Color888(0x87,0x82,0x7d));
  }
  matrix.drawPixel(50, 25, matrix.Color888(0x87,0x82,0x7d));
  for(int n=0;n<2;n++)
  {
    matrix.drawPixel(52+5*n, 26, YELLOW);
    matrix.drawPixel(53+5*n, 26, YELLOW);
    matrix.drawPixel(52+5*n, 27, YELLOW);
    matrix.drawPixel(51+5*n, 28, YELLOW);
    matrix.drawPixel(51+5*n, 29, YELLOW);
    matrix.drawPixel(50+5*n, 30, YELLOW);

  }
}
void printWeather(String weather)
{
  if(weather=="Clear")
  {
    printSunny();
  }else if(weather=="Drizzle")
  {
    printDrizzle();
  }else if(weather=="Rain")
  {
    printRain();
  }else if(weather=="Thunderstorm")
  {
    printThunder();
  }else if(weather=="Clouds")
  {
    printClouds();
  };
}
void IRAM_ATTR onTimer(){
 counter++;
}
void printZero(int i, int j)
{
  matrix.fillRect(i,j,6,2,WHITE);
  matrix.fillRect(i,j+8,6,2,WHITE);
  matrix.fillRect(i,j+2,2,6,WHITE);
  matrix.fillRect(i+4,j+2,2,6,WHITE);
}
void printOne(int i, int j)
{
  matrix.fillRect(i+2,j,2,8,WHITE);
  matrix.fillRect(i,j+8,6,2,WHITE);
  matrix.drawPixel(i,j+2,WHITE);
  matrix.drawPixel(i+1,j+2,WHITE);
  matrix.drawPixel(i+1,j+1,WHITE);
  
}
void printTwo(int i, int j)
{
  matrix.fillRect(i,j,6,2,WHITE);
  matrix.fillRect(i+4,j+2,2,2,WHITE);
  matrix.fillRect(i,j+4,6,2,WHITE);
  matrix.fillRect(i,j+6,2,2,WHITE);
  matrix.fillRect(i,j+8,6,2,WHITE);

}
void printThree(int i, int j)
{
  matrix.fillRect(i,j,6,2,WHITE);
  matrix.fillRect(i,j+8,6,2,WHITE);
  matrix.fillRect(i+4,j+2,2,6,WHITE);
  matrix.fillRect(i,j,6,2,WHITE);
  matrix.fillRect(i+2,j+4,2,2,WHITE);
}
void printFour(int i, int j)
{
  matrix.fillRect(i,j,2,6,WHITE);
  matrix.fillRect(i+4,j,2,10,WHITE);
  matrix.fillRect(i+2,j+4,2,2,WHITE);
}
void printFive(int i, int j)
{
  matrix.fillRect(i,j,6,2,WHITE);
  matrix.fillRect(i,j+2,2,2,WHITE);
  matrix.fillRect(i,j+4,6,2,WHITE);
  matrix.fillRect(i+4,j+6,2,2,WHITE);
  matrix.fillRect(i,j+8,6,2,WHITE);

}
void printSix(int i, int j)
{
  matrix.fillRect(i,j,2,10,WHITE);
  matrix.fillRect(i+2,j,4,2,WHITE);
  matrix.fillRect(i+2,j+4,4,2,WHITE);
  matrix.fillRect(i+2,j+8,4,2,WHITE);
  matrix.fillRect(i+4,j+6,2,2,WHITE);
}
void printSeven(int i, int j)
{
  matrix.fillRect(i,j,6,2,WHITE);
  matrix.fillRect(i+4,j+2,2,8,WHITE);
}
void printEight(int i, int j)
{
  matrix.fillRect(i,j,6,2,WHITE);
  matrix.fillRect(i,j+8,6,2,WHITE);
  matrix.fillRect(i,j+2,2,6,WHITE);
  matrix.fillRect(i+4,j+2,2,6,WHITE);
  matrix.fillRect(i+2,j+4,2,2,WHITE);
}
void printNine(int i, int j)
{
  matrix.fillRect(i,j,6,2,WHITE);
  matrix.fillRect(i,j+8,6,2,WHITE);
  matrix.fillRect(i,j+2,2,6,WHITE);
  matrix.fillRect(i+4,j+2,2,6,WHITE);
  matrix.fillRect(i+2,j+4,2,2,WHITE);
  matrix.fillRect(i,j+6,2,2,matrix.Color888(0,0,0));
}
void printDot(int i, int j)
{
  matrix.fillRect(i,j,2,2,WHITE);
}
void printNumb(int i, int j, int numb)
{
  switch(numb)
      {
        case 0:
        printZero(i,j);
        break;
        case 1:
        printOne(i,j);
        break;
        case 2:
        printTwo(i,j);
        break;
        case 3:
        printThree(i,j);
        break;
        case 4:
        printFour(i,j);
        break;
        case 5:
        printFive(i,j);
        break;
        case 6:
        printSix(i,j);
        break;
        case 7:
        printSeven(i,j);
        break;
        case 8:
        printEight(i,j);
        break;
        case 9:
        printNine(i,j);
        break;
}
}
void printTime(int s, int m, int h)
{
  static int temps=0;
  printDot(15,23);
  printDot(15,27);
   printDot(32,23);
  printDot(32,27);
  if(temps!=s)
  {
    matrix.fillRect(1,21,47,10,matrix.Color888(0,0,0));
    if(s<10)
    {
      printZero(35,21);
      printNumb(42,21,s);
    }else if(s>=10 && s<20)
    {
       printOne(35,21);
      printNumb(42,21,s-10);
    }else if(s>=20 && s<30)
    {
       printTwo(35,21);
      printNumb(42,21,s-20);
    }else if(s>=30 && s<40)
    {
       printThree(35,21);
      printNumb(42,21,s-30);
    }else if(s>=40 && s<50)
    {
       printFour(35,21);
      printNumb(42,21,s-40);
    }else if(s>=50 && s<60)
    {
       printFive(35,21);
      printNumb(42,21,s-50);
    };

if(m<10)
    {
      printZero(18,21);
      printNumb(25,21,m);
    }else if(m>=10 && m<20)
    {
       printOne(18,21);
      printNumb(25,21,m-10);
    }else if(m>=20 && m<30)
    {
       printTwo(18,21);
      printNumb(25,21,m-20);
    }else if(m>=30 && m<40)
    {
       printThree(18,21);
      printNumb(25,21,m-30);
    }else if(m>=40 && m<50)
    {
       printFour(18,21);
      printNumb(25,21,m-40);
    }else if(m>=50 && m<60)
    {
       printFive(18,21);
      printNumb(25,21,m-50);
    };
if(h<10)

    {
      printZero(1,21);
      printNumb(8,21,h);
    }else if(h>=10 && h<20)
    {
       printOne(1,21);
      printNumb(8,21,h-10);
    }else if(h>=20 && h<24)
    {
       printTwo(1,21);
      printNumb(8,21,h-20);
    }
    temps=s;
    }

  }
  
void printWeatherParams()
{
  matrix.setCursor(8, 1);
  matrix.setTextColor(WHITE);
  matrix.setCursor(1, 1);
  matrix.print('T');
  matrix.setCursor(4, 1);
  matrix.print(':');
  matrix.setCursor(8, 1);
  matrix.println(int(temperatura-KELVIN));
  matrix.setCursor(20, 1);
  matrix.print('C');
  matrix.setCursor(32, 1);
  matrix.print('P');
  matrix.setCursor(36, 1);
  matrix.print(':');
  matrix.setCursor(40, 1);
  matrix.println((int)cisnienie);
}

void getStringDate()
{
  switch(wday)
  {
    case 0:
    Sweekday="SUN";
    break;
    case 1:
    Sweekday="MON";
    break;
    case 2:
    Sweekday="TUE";
    break;
    case 3:
    Sweekday="WED";
    break;
    case 4:
    Sweekday="THU";
    break;
    case 5:
    Sweekday="FRI";
    break;
    case 6:
    Sweekday="SAT";
    break;
  }
  switch(month)
  {
    case 0:
    Smonth="JAN";
    break;
    case 1:
    Smonth="FEB";
    break;
    case 2:
    Smonth="MAR";
    break;
    case 3:
    Smonth="APR";
    break;
    case 4:
    Smonth="MAY";
    break;
    case 5:
    Smonth="JUN";
    break;
    case 6:
    Smonth="JUL";
    break;
    case 7:
    Smonth="AUG";
    break;
    case 8:
    Smonth="SEP";
    break;
    case 9:
    Smonth="OCT";
    break;
    case 10:
    Smonth="NOV";
    break;
    case 11:
    Smonth="DEC";
    break;
  }
}
  // Here were copy the strings we're interested in using to your struct data
  // It's not mandatory to make a copy, you could just use the pointers
  // Since, they are pointing inside the "content" buffer, so you need to make
  // sure it's still in memory when you read the string
  

void setup() {
  My_timer = timerBegin(0, 80, true);
  timerAttachInterrupt(My_timer, &onTimer, true);
   timerAlarmWrite(My_timer, 1000000, true);
   timerAlarmEnable(My_timer);
//  Serial.begin(115200);
  WiFi.begin(ssid,passwd);
  while(WiFi.status()!=WL_CONNECTED)
  {
    delay(500);
   // Serial.printf(".");
  }
  if(WiFi.status()==WL_CONNECTED)
   // Serial.printf("Polaczono\n");
  
  configTime(gmt_strefa, daylightsave, ntpServer);
//pobranie danych z klineta http
  getWeather(&temperatura, &cisnienie, &wilgotnosc,&statusPogody);
  getTime(&year, &month, &day, &wday, &hours, &minutes, &seconds);
  matrix.begin();
  printWeather(statusPogody);
}

void loop()
{ 
  //getWeather(&temperatura, &cisnienie, &wilgotnosc,&statusPogody);
  getTime(&year, &month, &day, &wday, &hours, &minutes, &seconds);
  /*Serial.println("Cisnienie wynosi=");
  Serial.println(cisnienie);
  Serial.println("\nTemperatura wynosi=");
  Serial.println(temperatura);
  Serial.println("\nwilgotnosc wynosi=");
  Serial.println(wilgotnosc);
  Serial.println("\nStatus pogody to:");
  Serial.println(statusPogody);
  
  Serial.println("****\n");
  Serial.println(hours);
  Serial.println(":");
  Serial.println(minutes);
  Serial.println(":");
  Serial.println(seconds);
  Serial.println(":");
*/
/*matrix.setTextSize(1);     // size 1 == 8 pixels high
  matrix.setTextWrap(false);
matrix.setCursor(35, 0);
matrix.print(seconds);
matrix.setCursor(20, 0);
matrix.print(minutes);
matrix.setCursor(8, 0);
matrix.print(hours);
//matrix.setCursor(30, 15);
//matrix.print(temperatura-KELVIN);
printSeven(1,21);
printEight(8,21);
printTwo(18,21);
printThree(25,21);
printNine(35,21);
printSix(42,21);
delay(1000);
matrix.fillScreen(matrix.Color888(0, 0, 0));
*/
printTime(seconds,minutes,hours);
//printWeather(statusPogody);//trzeba to wsadzic do przerwania
printWeatherParams();
matrix.setCursor(3, 10);
matrix.println(wday);
dimmer();
if(counter==1500)
 {
   printTime(seconds,minutes,hours);
   printWeather(statusPogody);
getWeather(&temperatura, &cisnienie, &wilgotnosc,&statusPogody);
printWeather(statusPogody);
 counter=0;
 }

}