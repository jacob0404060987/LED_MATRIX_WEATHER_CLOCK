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
#define KELVIN 273.15

typedef struct
{
    uint16_t year;
    uint16_t month;
    uint16_t day;
    uint16_t wday;
    uint16_t hours;
    uint16_t minutes;
    uint16_t seconds;
    String Sweekday;
    String Smonth;
}myTime_t;

typedef struct
{
    int temperature;
    int humidity;
    int pressure;
    String weatherStatus;
    bool flag;
}myWeather_t;

typedef struct
{
    uint16_t WHITE; 
    uint16_t YELLOW;
    uint16_t BLUE;
    uint16_t BLACK;
    uint16_t GREY;
}myColors_t;

typedef struct
{
    String weather_link;
    String api_key;
}myHttp_t;


/*
void dimmer(RGBmatrixPanel* matrix,myTime_t* myTime,myColors_t* myColors,uint16_t start, uint16_t end );
void printSunny(RGBmatrixPanel* matrix, myColors_t* myColor);
void printClouds(RGBmatrixPanel* matrix,myColors_t* myColor);
void printRain(RGBmatrixPanel* matrix,myColors_t* myColor );
void printThunder(RGBmatrixPanel* matrix, myColors_t* myColor);
void printWeather(RGBmatrixPanel* matrix, myColors_t* myColor,myWeather_t* myWeather);
void printZero(RGBmatrixPanel* matrix,myColors_t* myColors,int i, int j);
void printOne(RGBmatrixPanel* matrix,myColors_t* myColors,int i, int j);
void printTwo(RGBmatrixPanel* matrix,myColors_t* myColors,int i, int j);
void printThree(RGBmatrixPanel* matrix,myColors_t* myColors,int i, int j);
void printFour(RGBmatrixPanel* matrix,myColors_t* myColors,int i, int j);
void printFive(RGBmatrixPanel* matrix,myColors_t* myColors,int i, int j);
void printSix(RGBmatrixPanel* matrix,myColors_t* myColors,int i, int j);
void printSeven(RGBmatrixPanel* matrix,myColors_t* myColors,int i, int j);
void printEight(RGBmatrixPanel* matrix,myColors_t* myColors,int i, int j);
void printNine(RGBmatrixPanel* matrix,myColors_t* myColors,int i, int j);
void printDot(RGBmatrixPanel* matrix,myColors_t* myColors,int i, int j);
void printNumb(RGBmatrixPanel* matrix, myColors_t* myColors,int i, int j, int numb);
void printWeatherParams(RGBmatrixPanel* matrix, myColors_t* myColors, myWeather_t* myWeather);
void getStringDate(myTime_t* myWeather);
void printDate(RGBmatrixPanel* matrix, myTime_t* myTime,myColors_t* myColors);
 */
void getTime(myTime_t* myTime);
void getWeather(myWeather_t* myWeather, myHttp_t* myHttp);
 void printClock(RGBmatrixPanel* matrix, myColors_t* myColors, myTime_t* myTime,myWeather_t*);
 








