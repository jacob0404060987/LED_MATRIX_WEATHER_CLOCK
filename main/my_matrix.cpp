#include "my_matrix.h"

void getTime(myTime_t* myTime)
{
    struct tm timeinfo;
    getLocalTime(&timeinfo);
    if((myTime->seconds)!=timeinfo.tm_sec)
    {
        myTime->seconds=timeinfo.tm_sec;
    }
    if((myTime->minutes)!=timeinfo.tm_min)
    {
        myTime->minutes=timeinfo.tm_min;
    }
    if((myTime->hours)!=timeinfo.tm_hour)
    {
        myTime->hours=timeinfo.tm_hour;
    }
    if((myTime->day)!=timeinfo.tm_mday)
    {
        myTime->year=timeinfo.tm_year;
        myTime->month=timeinfo.tm_mon;
        myTime->day=timeinfo.tm_mday;
        myTime->wday=timeinfo.tm_wday;
    }
}
/*static void colors(RGBmatrixPanel* matrix, myColors_t* myColors)
{
    (myColors->WHITE)=matrix->Color888(0xFF,0xFF,0xff);
    (myColors->YELLOW)=matrix->Color888(0xEE,0xFF,0x00);
    (myColors->BLUE)=matrix->Color888(0x0B,0x91,0xD9);
    (myColors->BLACK)=matrix->Color888(0x00,0x00,0x00);
    (myColors->GREY) = matrix->Color888(0x87,0x82,0x7d);
}*/
static void dimmer(RGBmatrixPanel* matrix,myTime_t* myTime,myColors_t* myColors,int start, int end )
{

    int h;
    h=(myTime->hours);
    if(h>=22 || h<7)
  {
    (myColors->WHITE)=matrix->Color888(0xFF*0.07,0xFF*0.07,0xff*0.07);
    (myColors->YELLOW)=matrix->Color888(0xFF*0.07,0xFF*0.07,0xff*0.07);
    (myColors->BLUE)=matrix->Color888(0x0B*0.08,0x91*0.08,0xD9*0.08);
    (myColors->BLACK)=matrix->Color888(0x00*0.07,0x00*0.07,0x00*0.07);
    (myColors->GREY)=matrix->Color888(0x87*0.7,0x82*0.7,0x7d*0.7);
  }
  else
  {
  (myColors->WHITE)=matrix->Color888(0xFF,0xFF,0xff);
  (myColors->YELLOW)=matrix->Color888(0xEE,0xFF,0x00);
  (myColors->BLUE)=matrix->Color888(0x0B,0x91,0xD9);
 (myColors->BLACK)=matrix->Color888(0x00,0x00,0x00);
 (myColors->GREY) = matrix->Color888(0x87,0x82,0x7d);
}
matrix->setTextColor(myColors->WHITE);
}
void getWeather(myWeather_t* myWeather, myHttp_t* myHttp)
{
  HTTPClient http;
    http.begin((myHttp->weather_link)+(myHttp->api_key));
    int httpCode = http.GET();
    String JSONinput = http.getString();
    StaticJsonDocument<1024> doc;

DeserializationError error = deserializeJson(doc, JSONinput);

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
myWeather->temperature=(int)(main_temp-KELVIN);
myWeather->pressure=main_pressure;
myWeather->humidity=main_humidity;
myWeather->weatherStatus=(String)weather_0_main;
}
static void printSunny(RGBmatrixPanel* matrix, myColors_t* myColor)
{
    matrix->fillRect(48, 21, 15, 11,myColor->BLACK);
    matrix->fillCircle(56, 26, 5,myColor->YELLOW);
}
static void printClouds(RGBmatrixPanel* matrix,myColors_t* myColor)
{
    matrix->fillRect(48, 21, 15, 11,myColor->BLACK);
  matrix->fillRect(52, 24, 9,7,myColor->WHITE);
  for(int n=0; n<6;n++)
  {
    matrix->drawPixel(51, 25+n,myColor-> WHITE);
  }
  for(int n=0; n<4;n++)
  {
    matrix->drawPixel(50, 26+n,myColor-> WHITE);
  }
    for(int n=0; n<2;n++)
  {
    matrix->drawPixel(49, 27+n,myColor-> WHITE);
  }
      for(int n=0; n<6;n++)
  {
    matrix->drawPixel(61, 24+n,myColor-> WHITE);
  }
  for(int n=0; n<3;n++)
  {
    matrix->drawPixel(62, 25+n,myColor-> WHITE);
  }
  for(int n=0; n<6;n++)
  {
    matrix->drawPixel(54+n, 23,myColor-> WHITE);
  }
  for(int n=0; n<4;n++)
  {
    matrix->drawPixel(55+n, 22,myColor-> WHITE);
  }
}
static void printRain(RGBmatrixPanel* matrix,myColors_t* myColor )
{
    matrix->fillRect(48, 21, 15, 11,myColor->BLACK);
  matrix->fillRect(51, 24, 9, 3,myColor-> GREY);
  for(int n=0; n<8;n++)
  {
    matrix->drawPixel(52+n, 23, myColor->GREY);
  }
  for(int n=0; n<5;n++)
  {
    matrix->drawPixel(53+n, 22,myColor-> GREY);
  }
  for(int n=0; n<2;n++)
  {
    matrix->drawPixel(54+n, 21,myColor-> GREY);
  }
    for(int n=0; n<2;n++)
  {
    matrix->drawPixel(60, 24+n,myColor-> GREY);
  }
  matrix->drawPixel(50, 25,myColor-> GREY);
  for(int n=0;n<4;n++)
  {
    matrix->drawPixel(52+2*n, 27,myColor-> BLUE);
  }
    for(int n=0;n<5;n++)
  {
    matrix->drawPixel(51+2*n, 28,myColor-> BLUE);
  }
    for(int n=0;n<4;n++)
  {
    matrix->drawPixel(52+2*n, 29,myColor-> BLUE);
  }
    for(int n=0;n<4;n++)
  {
    matrix->drawPixel(53+2*n, 30,myColor-> BLUE);
  }
}
static void printThunder(RGBmatrixPanel* matrix, myColors_t* myColor)
{
    matrix->fillRect(48, 21, 15, 11,myColor-> BLACK);
  matrix->fillRect(51, 24, 9, 3,myColor-> GREY);
  for(int n=0; n<8;n++)
  {
    matrix->drawPixel(52+n, 23,myColor-> GREY);
  }
  for(int n=0; n<5;n++)
  {
    matrix->drawPixel(53+n, 22,myColor-> GREY);
  }
  for(int n=0; n<2;n++)
  {
    matrix->drawPixel(54+n, 21,myColor-> GREY);
  }
    for(int n=0; n<2;n++)
  {
    matrix->drawPixel(60, 24+n,myColor-> GREY);
  }
  matrix->drawPixel(50, 25,myColor-> GREY);
  for(int n=0;n<2;n++)
  {
    matrix->drawPixel(52+5*n, 26,myColor-> YELLOW);
    matrix->drawPixel(53+5*n, 26,myColor-> YELLOW);
    matrix->drawPixel(52+5*n, 27,myColor-> YELLOW);
    matrix->drawPixel(51+5*n, 28,myColor-> YELLOW);
    matrix->drawPixel(51+5*n, 29,myColor-> YELLOW);
    matrix->drawPixel(50+5*n, 30,myColor-> YELLOW);

  }
}
static void printWeather(RGBmatrixPanel* matrix, myColors_t* myColor,myWeather_t* myWeather)
{
    if(myWeather->weatherStatus=="Clear")
  {
    printSunny(matrix,myColor);
  }else if(myWeather->weatherStatus=="Rain")
  {
    printRain(matrix,myColor);
  }else if(myWeather->weatherStatus=="Thunderstorm")
  {
    printThunder(matrix,myColor);
  }else if(myWeather->weatherStatus=="Clouds")
  {
    printClouds(matrix,myColor);
  }
}
static void printZero(RGBmatrixPanel* matrix,myColors_t* myColors,int i, int j)
{
  matrix->fillRect(i,j,6,2,myColors->WHITE);
  matrix->fillRect(i,j+8,6,2,myColors->WHITE);
  matrix->fillRect(i,j+2,2,6,myColors->WHITE);
  matrix->fillRect(i+4,j+2,2,6,myColors->WHITE);
}
static void printOne(RGBmatrixPanel* matrix,myColors_t* myColors,int i, int j)
{
    matrix->fillRect(i+2,j,2,8,myColors->WHITE);
  matrix->fillRect(i,j+8,6,2,myColors->WHITE);
  matrix->drawPixel(i,j+2,myColors->WHITE);
  matrix->drawPixel(i+1,j+2,myColors->WHITE);
  matrix->drawPixel(i+1,j+1,myColors->WHITE);
}
static void printTwo(RGBmatrixPanel* matrix,myColors_t* myColors,int i, int j)
{
    matrix->fillRect(i,j,6,2,myColors->WHITE);
  matrix->fillRect(i+4,j+2,2,2,myColors->WHITE);
  matrix->fillRect(i,j+4,6,2,myColors->WHITE);
  matrix->fillRect(i,j+6,2,2,myColors->WHITE);
  matrix->fillRect(i,j+8,6,2,myColors->WHITE);

}
static void printThree(RGBmatrixPanel* matrix,myColors_t* myColors,int i, int j)
{
  matrix->fillRect(i,j,6,2,myColors->WHITE);
  matrix->fillRect(i,j+8,6,2,myColors->WHITE);
  matrix->fillRect(i+4,j+2,2,6,myColors->WHITE);
  matrix->fillRect(i,j,6,2,myColors->WHITE);
  matrix->fillRect(i+2,j+4,2,2,myColors->WHITE);
}
static void printFour(RGBmatrixPanel* matrix,myColors_t* myColors,int i, int j)
{
  matrix->fillRect(i,j,2,6,myColors->WHITE);
  matrix->fillRect(i+4,j,2,10,myColors->WHITE);
  matrix->fillRect(i+2,j+4,2,2,myColors->WHITE);
}
static void printFive(RGBmatrixPanel* matrix,myColors_t* myColors,int i, int j)
{
  matrix->fillRect(i,j,6,2,myColors->WHITE);
  matrix->fillRect(i,j+2,2,2,myColors->WHITE);
  matrix->fillRect(i,j+4,6,2,myColors->WHITE);
  matrix->fillRect(i+4,j+6,2,2,myColors->WHITE);
  matrix->fillRect(i,j+8,6,2,myColors->WHITE);

}
static void printSix(RGBmatrixPanel* matrix,myColors_t* myColors,int i, int j)
{
  matrix->fillRect(i,j,2,10,myColors->WHITE);
  matrix->fillRect(i+2,j,4,2,myColors->WHITE);
  matrix->fillRect(i+2,j+4,4,2,myColors->WHITE);
  matrix->fillRect(i+2,j+8,4,2,myColors->WHITE);
  matrix->fillRect(i+4,j+6,2,2,myColors->WHITE);
}
static void printSeven(RGBmatrixPanel* matrix,myColors_t* myColors,int i, int j)
{
  matrix->fillRect(i,j,6,2,myColors->WHITE);
  matrix->fillRect(i+4,j+2,2,8,myColors->WHITE);
}
static void printEight(RGBmatrixPanel* matrix,myColors_t* myColors,int i, int j)
{
  matrix->fillRect(i,j,6,2,myColors->WHITE);
  matrix->fillRect(i,j+8,6,2,myColors->WHITE);
  matrix->fillRect(i,j+2,2,6,myColors->WHITE);
  matrix->fillRect(i+4,j+2,2,6,myColors->WHITE);
  matrix->fillRect(i+2,j+4,2,2,myColors->WHITE);
}
static void printNine(RGBmatrixPanel* matrix,myColors_t* myColors,int i, int j)
{
  matrix->fillRect(i,j,6,2,myColors->WHITE);
  matrix->fillRect(i,j+8,6,2,myColors->WHITE);
  matrix->fillRect(i,j+2,2,6,myColors->WHITE);
  matrix->fillRect(i+4,j+2,2,6,myColors->WHITE);
  matrix->fillRect(i+2,j+4,2,2,myColors->WHITE);
  matrix->fillRect(i,j+6,2,2,myColors->BLACK);
}
static void printDot(RGBmatrixPanel* matrix,myColors_t* myColors,int i, int j)
{
  matrix->fillRect(i,j,2,2,myColors->WHITE);
}
static void printNumb(RGBmatrixPanel* matrix, myColors_t* myColors,int i, int j, int numb)
  {
  switch(numb)
      {
        case 0:
        printZero(matrix,myColors,i,j);
        break;
        case 1:
        printOne(matrix,myColors,i,j);
        break;
        case 2:
        printTwo(matrix,myColors,i,j);
        break;
        case 3:
        printThree(matrix,myColors,i,j);
        break;
        case 4:
        printFour(matrix,myColors,i,j);
        break;
        case 5:
        printFive(matrix,myColors,i,j);
        break;
        case 6:
        printSix(matrix,myColors,i,j);
        break;
        case 7:
        printSeven(matrix,myColors,i,j);
        break;
        case 8:
        printEight(matrix,myColors,i,j);
        break;
        case 9:
        printNine(matrix,myColors,i,j);
        break;
}
}
static void printWeatherParams(RGBmatrixPanel* matrix, myColors_t* myColors, myWeather_t* myWeather)
{ 
  matrix->fillRect(0, 0, 64, 10,myColors->BLACK);
  matrix->setCursor(1, 1);
  matrix->print('T');
  matrix->setCursor(4, 1);
  matrix->print(':');
  matrix->setCursor(8, 1);
  matrix->println(myWeather->temperature);
  matrix->setCursor(20, 1);
  matrix->print('C');
  if(myWeather->flag)
  {
  matrix->setCursor(32, 1);
  matrix->print('P');
  matrix->setCursor(36, 1);
  matrix->print(':');
  matrix->setCursor(40, 1);
  matrix->println(myWeather->pressure);
  }else
  {
  matrix->setCursor(32, 1);
  matrix->print('H');
  matrix->setCursor(36, 1);
  matrix->print(':');
  matrix->setCursor(40, 1);
  matrix->println(myWeather->humidity);
  if(myWeather->humidity!=100)
  {
  matrix->setCursor(53, 1);
  }else
  {
     matrix->setCursor(58, 1);
  }
    matrix->print('%');
  }
}
static void getStringDate(myTime_t* myWeather)
{
  switch(myWeather->wday)
  {
    case 0:
    myWeather->Sweekday="SUN";
    break;
    case 1:
    myWeather->Sweekday="MON";
    break;
    case 2:
    myWeather->Sweekday="TUE";
    break;
    case 3:
    myWeather->Sweekday="WED";
    break;
    case 4:
    myWeather->Sweekday="THU";
    break;
    case 5:
    myWeather->Sweekday="FRI";
    break;
    case 6:
    myWeather->Sweekday="SAT";
    break;
  }
  switch(myWeather->month)
  {
    case 0:
    myWeather->Smonth="JAN";
    break;
    case 1:
    myWeather->Smonth="FEB";
    break;
    case 2:
    myWeather->Smonth="MAR";
    break;
    case 3:
    myWeather->Smonth="APR";
    break;
    case 4:
    myWeather->Smonth="MAY";
    break;
    case 5:
    myWeather->Smonth="JUN";
    break;
    case 6:
    myWeather->Smonth="JUL";
    break;
    case 7:
    myWeather->Smonth="AUG";
    break;
    case 8:
    myWeather->Smonth="SEP";
    break;
    case 9:
    myWeather->Smonth="OCT";
    break;
    case 10:
    myWeather->Smonth="NOV";
    break;
    case 11:
    myWeather->Smonth="DEC";
    break;
  }
}
static void printDate(RGBmatrixPanel* matrix, myTime_t* myTime,myColors_t* myColors)
{
  matrix->fillRect(0, 11, 64, 8,myColors->BLACK);
  matrix->setCursor(7, 11);
  matrix->println(myTime->Smonth);
  matrix->setCursor(27,11);
  matrix->println(myTime->day);
  matrix->setCursor(41, 11);
  matrix->println(myTime->Sweekday);
}
void printClock(RGBmatrixPanel* matrix, myColors_t* myColors, myTime_t* myTime,myWeather_t* myWeather)
{
  getTime(myTime);
  static int temps=0;
  static int tempm=0;
  static int temph=0;
  int s=myTime->seconds;
  int m=myTime->minutes;
  int h=myTime->hours;
  printDot(matrix,myColors,15,23);
  printDot(matrix,myColors,15,27);
  printDot(matrix,myColors,32,23);
  printDot(matrix,myColors,32,27);
  if(temph!=h)
{
  dimmer(matrix,myTime,myColors,22,7);
  getStringDate(myTime);
  matrix->fillRect(1,21,14,10,myColors->BLACK);
  if(h<10)

    {
      printZero(matrix,myColors,1,21);
      printNumb(matrix,myColors,8,21,h);
    }else if(h>=10 && h<20)
    {
       printOne(matrix,myColors,1,21);
      printNumb(matrix,myColors,8,21,h-10);
    }else if(h>=20 && h<24)
    {
       printTwo(matrix,myColors,1,21);
      printNumb(matrix,myColors,8,21,h-20);
    }
    temph=h;
}
if(tempm!=m)
{
    dimmer(matrix,myTime,myColors,22,7);
    printWeather(matrix,myColors,myWeather);
    printWeatherParams(matrix,myColors,myWeather);
  matrix->fillRect(18,21,14,10,myColors->BLACK);
if(m<10)
    {
     
      printZero(matrix,myColors,18,21);
      printNumb(matrix,myColors,25,21,m);
    }else if(m>=10 && m<20)
    {
       printOne(matrix,myColors,18,21);
      printNumb(matrix,myColors,25,21,m-10);
    }else if(m>=20 && m<30)
    {
       printTwo(matrix,myColors,18,21);
      printNumb(matrix,myColors,25,21,m-20);
    }else if(m>=30 && m<40)
    {
       printThree(matrix,myColors,18,21);
      printNumb(matrix,myColors,25,21,m-30);
    }else if(m>=40 && m<50)
    {
       printFour(matrix,myColors,18,21);
      printNumb(matrix,myColors,25,21,m-40);
    }else if(m>=50 && m<60)
    {
       printFive(matrix,myColors,18,21);
      printNumb(matrix,myColors,25,21,m-50);
    }
  tempm=m;
}
  if(temps!=s)
  {
    printDate(matrix,myTime,myColors);
    matrix->fillRect(35,21,14,10,myColors->BLACK);
    if(s<10)
    {
      printZero(matrix,myColors,35,21);
      printNumb(matrix,myColors,42,21,s);
    }else if(s>=10 && s<20)
    {
       printOne(matrix,myColors,35,21);
      printNumb(matrix,myColors,42,21,s-10);
    }else if(s>=20 && s<30)
    {
       printTwo(matrix,myColors,35,21);
      printNumb(matrix,myColors,42,21,s-20);
    }else if(s>=30 && s<40)
    {
       printThree(matrix,myColors,35,21);
      printNumb(matrix,myColors,42,21,s-30);
    }else if(s>=40 && s<50)
    {
       printFour(matrix,myColors,35,21);
      printNumb(matrix,myColors,42,21,s-40);
    }else if(s>=50 && s<60)
    {
       printFive(matrix,myColors,35,21);
      printNumb(matrix,myColors,42,21,s-50);
    }
    temps=s;
  }



  }



