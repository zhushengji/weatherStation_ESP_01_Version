//////////配网/联网功能///////////
#include <ESP8266WiFi.h>
#include "./DNSServer.h" // Patched lib
#include <ESP8266WebServer.h>
extern "C"
{
#include <user_interface.h>
}
void showClients();
const byte DNS_PORT = 53;       // Capture DNS requests on port 53
IPAddress apIP(10, 10, 10, 1);  // Private network for server
DNSServer dnsServer;            // Create the DNS object
ESP8266WebServer webServer(80); // HTTP server

String responseHTML = "<!DOCTYPE html><html charset=utf-8><head > <title>气象站配网设置</title> <meta name='viewport' content='width=device-width,initial-scale=1'></head><body> <p>如果您想拥有更精准的天气数据可到“心知天气”申请免费密钥，详情可百度。如无要求，本页密钥可以不填</p><form accept-charset='UTF-8' action='' id='sign_in_form' method='get'> <div style='margin:0;padding:0;display:inline'><input name='utf8' type='hidden' value='&#x2713;' /></div> <div id='signin_form' class='formarea'> <h3>WiFi名称</h3> <input id='email_field' name='email' type='text'> <h3>WiFi密码</h3> <input id='password_field' name='password' type='password' class='jsAnalyticsExclude'><h3>天气密钥</h3> <input id='author_field' name='author' type='text'></p> <input class='btn btn-default' id='sign_in' name='commit' type='submit' value='提交' /> </div> </form></body></html>";
/*自动配网*/
bool autoConfig()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin();
  Serial.print("自动配网中，请稍候......");
  for (int i = 0; i < 50; i++)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("AutoConfig Success");
      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
      WiFi.printDiag(Serial);
      return true;
      // break;
    }
    else
    {
      Serial.print(".");
      delay(500);
    }
  }
  Serial.println("自动配网失败！");
  return false;
}
// 获取配网信息 连接或配置  服务
boolean settingMode;
boolean restoreConfig()
{
  Serial.print("wifi名：");
  Serial.println(WiFi.SSID());
  bool flag = autoConfig();
  if (!flag)
  {
    Serial.println("找不到配网信息");
    return false;
  }
  else
  {
    Serial.println("自动配置上次连接网络成功！");
    return true;
  }
}
// STA模式下 连接网络     工具
boolean checkConnection()
{
  int count = 0;
  Serial.println("正在等待连接");
  while (count < 30)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("成功连接！");
      return (true);
    }
    delay(500);
    Serial.print(".");
    count++;
  }
  Serial.println("连接超时.");
  return false;
}
bool wifi_init()
{
  delay(10);
  u8g2.clearBuffer();
  u8g2.drawXBMP(0, 0, 128, 64, autoPic);
  u8g2.sendBuffer();
  if (!restoreConfig())
  {
    if (!checkConnection())
    {
      while (1)
      {
        pinMode(LED_BUILTIN, OUTPUT);
        WiFi.mode(WIFI_AP);
        WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
        WiFi.softAP("STIA气象站配网WiFi");
        u8g2.clearBuffer();
        u8g2.drawXBMP(0, 0, 128, 64, Captive);
        u8g2.sendBuffer();
        // provided IP to all DNS request
        dnsServer.start(DNS_PORT, "*", apIP);
        // replay to all requests with same HTML
        webServer.onNotFound([]()
                             { webServer.send(200, "text/html", responseHTML); });
        webServer.begin();
        Serial.println("Hotspot up & running.");
        while (1)
        {
          dnsServer.processNextRequest();
          webServer.handleClient();
          if (webServer.args() != 0)
          {
            showClients();
            Serial.println("数据获取：");
            Serial.println(webServer.arg(1));
            Serial.println(webServer.arg(2));
            Serial.println(webServer.arg(3));
            if(webServer.arg(3).length()==17){
              Serial.print("有数据提交！开始存储！数据长度：");Serial.println(webServer.arg(3).length());
              reqUserKey = webServer.arg(3);
              for(int i=0;i<17;i++){
                EEPROM.write(i,reqUserKey[i]);
                EEPROM.commit();
              }
            }
            WiFi.persistent(true); // 保存WiFi信息
            WiFi.begin(webServer.arg(1).c_str(), webServer.arg(2).c_str());
            int i = 0;
            while (WiFi.status() != WL_CONNECTED)
            {
              delay(1000);
              Serial.print(++i);
              Serial.print(' ');
              if (i >= 50)
              {
                Serial.println("输入内容有误，设备重启");
                ESP.restart();
              }
            }
            ESP.restart();
            return true;
          }
        }
      }
      return true;
    }
    return true;
  }
  return true;
}

void showClients()
{
  struct station_info *stat_info;
  stat_info = wifi_softap_get_station_info();
  uint8_t client_count = wifi_softap_get_station_num();
  String str = "Number of clients = ";
  str += String(client_count);
  str += "\r\nList of clients : \r\n";
  int i = 1;
  while (stat_info != NULL)
  {
    str += "Station #";
    str += String(i);
    str += " : ";
    str += String(stat_info->bssid[0], HEX);
    str += ":";
    str += String(stat_info->bssid[1], HEX);
    str += ":";
    str += String(stat_info->bssid[2], HEX);
    str += ":";
    str += String(stat_info->bssid[3], HEX);
    str += ":";
    str += String(stat_info->bssid[4], HEX);
    str += ":";
    str += String(stat_info->bssid[5], HEX);
    str += "\r\n";
    i++;
    stat_info = STAILQ_NEXT(stat_info, next);
    //    stat_info = stat_info->next;
  }
  Serial.println(str);
}
///////////////////////OTA功能///////////////////////////
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <ESP8266HTTPUpdateServer.h>
ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;
void ota()
{
  
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("联网成功！开启ota");
    u8g2.clearBuffer();
    u8g2.drawXBMP(0, 0, 128, 64, otapic);
    u8g2.setCursor(30,55);
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.setFontDirection(0);
    u8g2.print(WiFi.localIP());
    u8g2.sendBuffer();
    ArduinoOTA.setHostname("STIA气象站OTA");
    ArduinoOTA.begin();
    Serial.println("Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    // Initialize OTA
    ArduinoOTA.setHostname("STIA_OTA");
    ArduinoOTA.begin();

    // Initialize Web server and OTA update server
    server.on("/", HTTP_GET, []()
              { server.send(200, "text/html", "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>"); });

    // HTTP POST handler for firmware upload
    server.on(
        "/update", HTTP_POST, []()
        {
          server.send(200, "text/plain", "Update started. Please wait...");
          // Note: The firmware update will happen automatically after the response is sent.
        },
        []()
        {
          // Upload handler, called when a new file is received
          HTTPUpload &upload = server.upload();
          if (upload.status == UPLOAD_FILE_START)
          {
            Serial.printf("Update: %s\n", upload.filename.c_str());
            Serial.printf("size: %d\n", upload.contentLength);
            if (!Update.begin(upload.contentLength, 0, -1, -1))
            {
              Update.printError(Serial);
            }
          }
          else if (upload.status == UPLOAD_FILE_WRITE)
          {
            if (Update.write(upload.buf, upload.currentSize) != upload.currentSize)
            {
              Update.printError(Serial);
            }
          }
          else if (upload.status == UPLOAD_FILE_END)
          {
            if (Update.end(true))
            {
              Serial.printf("Update Success: %uB\n", upload.totalSize);
              ESP.restart();
            }
            else
            {
              Update.printError(Serial);
            }
          }
        });

    server.begin();
    Serial.println("Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    while (1)
    {
      ArduinoOTA.handle();
      server.handleClient();
      ESP.wdtFeed();
    }
  }
}