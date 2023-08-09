String months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
String weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

/**
 * 实时天气图标绘制
*/
void tubiaoBig()
{
  col = weatherData0.dayText;
  if (col.equalsIgnoreCase("Sunny") || col.equalsIgnoreCase("Clear"))
  { // 晴
    u8g2.drawXBMP(82, 3, 46, 46, col_100);
  }
  else if (col.equalsIgnoreCase("Overcast") || col.equalsIgnoreCase("Mostly cloudy")||col.equalsIgnoreCase("Cloudy"))
  {
    
    u8g2.drawXBMP(82, 3, 46, 46, col_102); // 云
  }
  else if (col.equalsIgnoreCase("Partly Cloudy") || col.equalsIgnoreCase("Partly cloudy"))
  {
    u8g2.drawXBMP(82, 3, 46, 46, col_103); // 晴间多云
  }
  else if (col.equalsIgnoreCase("Thundershower"))
  {
    u8g2.drawXBMP(82, 3, 46, 46, col_302); // 雷阵雨
  }
  else if (col.equalsIgnoreCase("Ice rain"))
  {
    u8g2.drawXBMP(82, 3, 46, 46, col_304); // 冰雹
  }
  else if (col.equalsIgnoreCase("Storm") || col.equalsIgnoreCase("Heavy Storm") || col.equalsIgnoreCase("Severe storm")||col.equalsIgnoreCase("Shower")||col.equalsIgnoreCase("Light rain") || col.equalsIgnoreCase("Moderate rain") || col.equalsIgnoreCase("Heavy rain"))
  {
    u8g2.drawXBMP(82, 3, 46, 46, col_301); // 雨
  }
  else if (col.equalsIgnoreCase("Snow flurry")||col.equalsIgnoreCase("Light snow") || col.equalsIgnoreCase("Moderate Snow") || col.equalsIgnoreCase("Heavy snow") || col.equalsIgnoreCase("Snowstorm"))
  {
    u8g2.drawXBMP(82, 3, 46, 46, col_304); // 雪
  }
  else if (col.equalsIgnoreCase("Foggy") || col.equalsIgnoreCase("Haze")||col.equalsIgnoreCase("Sand") || col.equalsIgnoreCase("Duststorm") || col.equalsIgnoreCase("Sandstorm") || col.equalsIgnoreCase("Dust"))
  {
    u8g2.drawXBMP(82, 3, 46, 46, col_500); // 沙尘暴
  }
  else
  {
    u8g2.drawXBMP(82, 3, 46, 46, col_208); // 未知
  }
}
/**
 * 判断当前年份是否为闰年
*/
bool isLeapYear(int year){
  return (year%100!=0&&year%4==0)||(year%400 == 0);
}
int daysNum(int y,int mon){
  bool f = isLeapYear(y);
  if(mon==1||mon==3||mon==5||mon==7||mon==8||mon==10||mon==12) return 31;
  else if(mon==4||mon==6||mon==1||mon==9||mon==11) return 30;
  else if(mon==2&&f) return 29;
  else return 28;
}
/**
 * 天气预报数据绘制
*/
void tubiaoSmall(int p)
{
  String currentDate;
  String text;
  int f;
  int tx,ty;
  int totalDays = daysNum(timedata.year,timedata.month);
  switch (p)
  {
  case 0:
    tx=0;
    ty=0;
    currentDate = String(timedata.month) + "-" + String(timedata.day);
    text = weatherData0.dayText;
    f = weatherData0.rainPer*100;
    break;
  case 1:
    tx=49;
    ty=0;
    if(timedata.day==totalDays)currentDate = String(timedata.month+1) + "-1" ;
    else currentDate = String(timedata.month) + "-" + String(timedata.day+1);
    text = weatherData1.dayText;
    f = weatherData1.rainPer*100;
    break;
  case 2:
    tx=98;
    ty=0;
    if(timedata.day>=totalDays)currentDate = String(timedata.month+1) + "-2";
    else currentDate = String(timedata.month) + "-" + String(timedata.day+2);
    text = weatherData2.dayText;
    f = weatherData2.rainPer*100;
    break;
  default:
    return;
  }
  if (col.equalsIgnoreCase("Sunny") || col.equalsIgnoreCase("Clear"))
  { // 晴
    u8g2.drawXBMP(tx, ty, 30, 30, qing);
  }
  else if (col.equalsIgnoreCase("Overcast") || col.equalsIgnoreCase("Mostly cloudy")||col.equalsIgnoreCase("Cloudy"))
  {
    u8g2.drawXBMP(tx, ty, 30, 30, yin); // 云
  }
  else if (col.equalsIgnoreCase("Partly Cloudy") || col.equalsIgnoreCase("Partly cloudy"))
  {
    u8g2.drawXBMP(tx, ty, 30, 30, yun); // 晴间多云
  }
  else if (col.equalsIgnoreCase("Thundershower"))
  {
    u8g2.drawXBMP(tx, ty, 30, 30, lei); // 雷阵雨
  }
  else if (col.equalsIgnoreCase("Ice rain"))
  {
    u8g2.drawXBMP(tx, ty, 30, 30, col_304); // 冰雹
  }
  else if (col.equalsIgnoreCase("Shower")||col.equalsIgnoreCase("Storm") || col.equalsIgnoreCase("Heavy Storm") || col.equalsIgnoreCase("Severe storm")||col.equalsIgnoreCase("Light rain") || col.equalsIgnoreCase("Moderate rain") || col.equalsIgnoreCase("Heavy rain"))
  {
    u8g2.drawXBMP(tx, ty, 30, 30, yu); // 雨
  }
  else if (col.equalsIgnoreCase("Light snow") || col.equalsIgnoreCase("Moderate Snow") || col.equalsIgnoreCase("Heavy snow") || col.equalsIgnoreCase("Snowstorm"))
  {
    u8g2.drawXBMP(tx, ty, 30, 30, xue); // 雪
  }
  else 
  {
    u8g2.drawXBMP(tx, ty, 30, 30, wu); // 沙尘暴
  }
  u8g2.setFont(u8g2_font_spleen6x12_mf);
  u8g2.setCursor(tx+4, ty+40);
  u8g2.print(currentDate); 
  u8g2.setCursor(tx, ty+52);
  u8g2.print(String(weatherData0.lowTemp)+"\'-"+String(weatherData0.highTemp)+"\'"); 
  u8g2.setCursor(tx+5, ty+62);
  if(f>100) f/=10;
  u8g2.print(f);u8g2.print("%");
}

/**
 * 实时时间数据拼装
*/
void updatetime()
{
  tim = "";
  tim += timedata.hour;
  tim += ":";
  if (timedata.minute < 10)
  {
    tim += "0";
  }
  tim += timedata.minute;
}
/**
 * 获取当前时间
*/
void getCurTime()
{
  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime((time_t *)&epochTime);
  int currentHour = timeClient.getHours();
  timedata.hour = currentHour;
  int currentMinute = timeClient.getMinutes();
  timedata.minute = currentMinute;
  int currentSecond = timeClient.getSeconds();
  timedata.second = currentSecond;
  String weekDay = weekDays[timeClient.getDay()];
  timedata.dayofweek = weekDay;
  int currentMonth = ptm->tm_mon + 1;
  timedata.month = currentMonth;
  int monthDay = ptm->tm_mday;
  timedata.day = monthDay;
  String currentMonthName = months[currentMonth - 1];
  int currentYear = ptm->tm_year + 1900;
  timedata.year = currentYear;
}
/**
 * 实时天气与未来天气获取并封装
*/
void updateweather()
{
  weatherNow.update();
  forecast.update();
  weatherData0.dayText = forecast.getDayText(0);
  weatherData1.dayText = forecast.getDayText(1);
  weatherData2.dayText = forecast.getDayText(2);
  weatherData0.highTemp = forecast.getHigh(0);
  weatherData1.highTemp = forecast.getHigh(1);
  weatherData2.highTemp = forecast.getHigh(2);
  weatherData0.lowTemp = forecast.getLow(0);
  weatherData1.lowTemp = forecast.getLow(1);
  weatherData2.lowTemp = forecast.getLow(2);
  weatherData0.rainPer = forecast.getRain(0);
  weatherData1.rainPer = forecast.getRain(1);
  weatherData2.rainPer = forecast.getRain(2);
  tmp = weatherNow.getDegree();
}
/**
 * 绘制当前实时室外气温功能
*/
void drawTem(){
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_logisoso24_tf);
  u8g2.setCursor(71, 48);
  u8g2.print(tmp);
  u8g2.drawXBMP(100, 23, 25, 25, col_ssd); 
  u8g2.drawXBMP(13, 9, 46, 46, wen);
  u8g2.sendBuffer();
}
/**
 * 天气预报界面绘制
*/
void getFutrueWeather(){
  u8g2.clearBuffer();
  col = weatherData0.dayText;
  tubiaoSmall(0);
  col = weatherData1.dayText;
  tubiaoSmall(1);
  col = weatherData2.dayText;
  tubiaoSmall(2);
  u8g2.sendBuffer();
}
/**
 * 绘制主界面
*/
void drawTimeweather(){
  updatetime();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_profont29_mr);
  u8g2.setCursor(0, 33);
  u8g2.print(tim); 
  u8g2.setFont(u8g2_font_spleen6x12_mf);
  u8g2.setCursor(8, 50);
  String currentDate = String(timedata.year) + "-" + String(timedata.month) + "-" + String(timedata.day);
  u8g2.print(currentDate); 
  u8g2.setCursor(13, 60);
  u8g2.print(timedata.dayofweek); 
  tubiaoBig();
  // u8g2.drawXBMP(82, 3, 46, 46, col_100);
  u8g2.setFont(u8g2_font_profont15_tf);
  u8g2.setCursor(85, 60);
  u8g2.print(weatherData0.dayText); 
  u8g2.sendBuffer();
}