#include <Arduino.h>

// https://arduino.me/a/image-to-bitmap-array 图片取模工具地址
#include <ArduinoJson.h>
#include <ESP8266_Seniverse.h>
#include <Forecast.h>
#include <LifeInfo.h>
#include <WeatherNow.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <U8x8lib.h>
#include "images.h"
#include <EEPROM.h>
#define UPKEY  2
#define DOWNKEY 0
// #define UPKEY D3
// #define DOWNKEY D4
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=1*/ 1, /* data=3*/ 3, /* reset=*/U8X8_PIN_NONE);

const char *ssid = "";
const char *password = "";
String reqUserKey = "SxzwEXUQLKZoS1-nv";
const char *reqLocation = "shanghai";
const char *reqUnit = "c";

bool secondFlash = true;
String date;
String tim = "";
String col;
int l, h, tmp;
String dayname = "today"; // 高低温
WeatherNow weatherNow;
Forecast forecast;

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
const long utcOffsetInSeconds = 3600;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
// 时间结构体
struct DateTime
{
  int year;
  byte month;
  int day;
  byte hour;
  byte minute;
  byte second;
  String dayofweek;
} timedata;
// 天气信息结构体
struct weatherIonfo
{
  String dayText;
  int highTemp;
  int lowTemp;
  int nowTemp;
  float rainPer;
};
// 当日天气，明天天气，后天天气
weatherIonfo weatherData0, weatherData1, weatherData2;
#include "getData.h"
#include "netWork.h"
int initHour = 0;
int rotation = 0;
void setup()
{
  Serial.begin(115200);
  EEPROM.begin(256);
  if(isalpha(char(EEPROM.read(0)))&&isalpha(char(EEPROM.read(1)))){
    String s;
    for(int i=0;i<17;i++){
      s+=char(EEPROM.read(i));
    }
    reqUserKey=s;
    Serial.print("读取的密钥：");Serial.println(reqUserKey);
  }
  pinMode(UPKEY, INPUT_PULLUP);
  pinMode(DOWNKEY, INPUT_PULLUP);
  u8g2.begin();
  u8g2.enableUTF8Print();
  u8g2.setDisplayRotation(U8G2_R0);
  u8g2.clearBuffer();
// return;
  u8g2.drawXBMP(0, 0, 128, 64, logo);
  u8g2.sendBuffer();
  delay(2000);
  // 联网功能
  if (wifi_init())
  {
    Serial.println("联网成功！");
  } 
  timeClient.begin();
  timeClient.setTimeOffset(28800);
  getCurTime();
  initHour = timedata.hour;

  weatherNow.config(reqUserKey, reqLocation, reqUnit);
  forecast.config(reqUserKey, reqLocation, reqUnit);
  updateweather();

  drawTimeweather();

  int flag = 0;
  int tCount = 0;
  while (1)
  {
    ESP.wdtFeed();
    if (digitalRead(UPKEY) == LOW)
    {
      delay(800);
      if (digitalRead(UPKEY) == LOW)
      {
        rotation++;
        rotation % 2 == 0 ? u8g2.setDisplayRotation(U8G2_R0) : u8g2.setDisplayRotation(U8G2_R2);
        drawTimeweather();
        delay(500);
        continue;
      }
      flag--;
      delay(800);
      if (flag < 0)
        flag = 3;
      if (flag % 3 == 1)
      {
        drawTem();
      }
      else if (flag % 3 == 2)
      {
        getFutrueWeather();
      }
      else
      {
        getCurTime();
        drawTimeweather();
      }
      tCount = 0;

      getCurTime();
    }
    if (digitalRead(DOWNKEY) == LOW)
    {
      flag++;
      delay(800);
      if (flag > 3)
        flag = 0;
      if (flag % 3 == 1)
      {
        drawTem();
      }
      else if (flag % 3 == 2)
      {
        getFutrueWeather();
      }
      else
      {
        getCurTime();
        drawTimeweather();
      }
      tCount = 0;

      getCurTime();
    }
    delay(10);
    tCount++;
    if (tCount == 6000)
    {
      flag = 0;
      // Serial.println("正常更新");
      getCurTime();
      // 整点更新天气
      if (timedata.hour - initHour == 1 || timedata.hour - initHour == -23)
      {
        initHour = timedata.hour;
        updateweather();
      }
      drawTimeweather();
      tCount = 0;
    }
  }
}

void loop()
{
}