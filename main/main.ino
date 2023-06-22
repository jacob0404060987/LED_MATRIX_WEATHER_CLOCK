#include <my_matrix.h>
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
  //HTTPClient http;
  //HTTPClient* pt_http=&http;
//zmienne globalne kontrola pogody przed konwersja
 //https://openweathermap.org/weather-conditions
//zmienne globalne czasu przed konwersja
  myTime_t myTime;
  myTime_t* czas=&myTime;
  myWeather_t myWeather;
  myWeather_t* pogoda=&myWeather;
  myColors_t myColors;
  myColors_t* kolory=&myColors;
  myHttp_t myLinks;
  myHttp_t* linki=&myLinks;
  RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);
  RGBmatrixPanel* matryca=&matrix;


//timery
static TimerHandle_t auto_reload_timer = NULL;
//hw_timer_t *My_timer = NULL;
int counter,counter1;
//kolory

//kolory

void myTimerCallback(TimerHandle_t xTimer) {
 counter++;
 counter1++;
 
  }


//void IRAM_ATTR onTimer()
//{

//}
enum tryby{
zegar,
teskt,
papa
};

void setup() {
  
  //My_timer = timerBegin(0, 80, true);
  //timerAttachInterrupt(My_timer, &onTimer, true);
 // timerAlarmWrite(My_timer, 1000000, true);
 // timerAlarmEnable(My_timer);

   auto_reload_timer = xTimerCreate(
                      "Auto-reload timer",        // Name of timer
                      1000 / portTICK_PERIOD_MS,  // Period of timer (in ticks)
                      pdTRUE,                     // Auto-reload
                      (void *)1,                  // Timer ID
                      myTimerCallback); 

  Serial.begin(115200);
  WiFi.begin(ssid,passwd);
  while(WiFi.status()!=WL_CONNECTED)
  {
    delay(500);
    Serial.printf(".");
  }
  if(WiFi.status()==WL_CONNECTED)
    Serial.printf("Polaczono\n");
   matrix.begin();
  configTime(gmt_strefa, daylightsave, ntpServer);
  getTime(czas);
  linki->weather_link = weather_link;
  linki-> api_key= api_key;
  getWeather(pogoda,linki);
  xTimerStart(auto_reload_timer, portMAX_DELAY);
//pobranie danych z klineta http
}
void loop()
{ 
  if(counter>=100) 
 {
  getWeather(pogoda,linki);
  if(WiFi.status())
  {
  Serial.println("Pobrano pogode");
  }else
  {
    Serial.println("Siec ma downa");
  }
  counter=0;
 }
  if(counter1>=60)
 {
  if(pogoda->flag)
  {
  pogoda->flag=false;
  }else
  {
    pogoda->flag=true;
  }
  counter1=0;
  }  
   printClock(matryca,kolory,czas,pogoda);
}