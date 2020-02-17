#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

//============WiFi名称和密码================//
const char* ssid     = "MI";
const char* password = "asdfghjkl";
//==========目标服务器网址和端口==============//
const char* host = "api.seniverse.com";
const uint16_t port = 80 ;
//===============地区设置===================//
String City = "宁波";//城市
String My_Key = "key";
//=================变量=====================//
int Hour,Minute,Second,Year,Month,Day;
bool DatFlag = true;
unsigned long getTime = 0;
String inputTime = "";
String inputString = "";  //接收到的数据
//请求URL
String url = "/v3/weather/daily.json?key=" + My_Key +"&location=" + City + "&language=zh-Hans&unit=c&start=0&days=3";
//请求数据
String urlDat = "key=" + My_Key +"&location=" + City + "&language=zh-Hans&unit=c&start=0&days=3";
WiFiClient client;
void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.printf("即将连接WiFi：%s\n",ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) //等待连接
  {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("\nWiFi连接成功\nIP: ");
  Serial.println(WiFi.localIP());
}
void loop() 
{
    GET_Weather();
    DateHandle();
}

void GET_Weather()
{
    if((millis() - getTime > 10000) or DatFlag)//10s
    {
        getTime = millis();
        //Serial.print("connecting to ");
        if (!client.connect(host,port)) {
          Serial.println("服务器连接失败");
          return;
        }    
        //Serial.print("Requesting URL: ");
        // 发送请求报文
        client.print(String("GET ") + url + " HTTP/1.1\r\n" +  //请求行  请求方法 ＋ 请求地址 + 协议版本
                     "Host: " + host + "\r\n" +                //请求头部
                     "Connection: close\r\n" +                //处理完成后断开连接
                     "\r\n" +                                 //空行
                     urlDat);                                 //请求数据            
        delay(100);  
        while(client.available()){          //接收数据
          String line = client.readStringUntil('\r');
            inputString += line;
        }
        //Serial.println(inputString);
        client.stop();      //断开与服务器连接以节约资源
        DatFlag = true;
       // Serial.println(inputString);
    }
}

void DateHandle()
{
    if(DatFlag)
    {
        DatFlag = false;
        int t = inputString.indexOf("Date:");//找时间
        int m = inputString.lastIndexOf("GMT");
        inputTime = inputString.substring(t, m + 1);
        int miao = inputTime.lastIndexOf(":");
        Hour = (inputTime.substring(miao-5, miao-3)).toInt();
        Minute = (inputTime.substring(miao-2, miao)).toInt();
        Second = (inputTime.substring(miao+1, miao+3)).toInt();
        /*Serial.println(Hour);
        Serial.println(Minute);
        Serial.println(Second);*/
        //Serial.println(inputTime);
        int jsonBeginAt = inputString.indexOf("{");
        int jsonEndAt = inputString.lastIndexOf("}");
        if (jsonBeginAt != -1 && jsonEndAt != -1) 
        {
          //净化json数据
          inputString = inputString.substring(jsonBeginAt, jsonEndAt + 1);//取得一个完整的JSON字符串
          processMessage();//对数据进行处理
          //Serial.println(inputString);
          //aJson.deleteItem(msg);//清空
          inputString="";
        }
    }  
}
void processMessage()
{
    const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(3) + JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(6) + 3*JSON_OBJECT_SIZE(13) + 760;
    DynamicJsonDocument doc(capacity);
    
    deserializeJson(doc, inputString);
    
    JsonObject results_0 = doc["results"][0];
    
    JsonObject results_0_location = results_0["location"];
    const char* results_0_location_name = results_0_location["name"]; // "宁波"
    
    JsonArray results_0_daily = results_0["daily"];
    
    JsonObject results_0_daily_0 = results_0_daily[0];
    const char* results_0_daily_0_date = results_0_daily_0["date"]; // "2019-12-21"==========================今天日期
    const char* results_0_daily_0_text_day = results_0_daily_0["text_day"]; // "中雨"=========================白天天气
    const char* results_0_daily_0_code_day = results_0_daily_0["code_day"]; // "14"==========================图标编号
    const char* results_0_daily_0_text_night = results_0_daily_0["text_night"]; // "中雨"=====================晚上天气
    const char* results_0_daily_0_code_night = results_0_daily_0["code_night"]; // "14"=======================图标编号
    const char* results_0_daily_0_high = results_0_daily_0["high"]; // "11"===================================最高气温
    const char* results_0_daily_0_low = results_0_daily_0["low"]; // "9"======================================最低气温
    const char* results_0_daily_0_wind_direction = results_0_daily_0["wind_direction"]; // "东北"==============风向
    const char* results_0_daily_0_wind_direction_degree = results_0_daily_0["wind_direction_degree"]; // "45"==度数
    const char* results_0_daily_0_wind_speed = results_0_daily_0["wind_speed"]; // "25.20"=====================速度
    const char* results_0_daily_0_wind_scale = results_0_daily_0["wind_scale"]; // "4"=========================风标
    const char* results_0_daily_0_humidity = results_0_daily_0["humidity"]; // "94"============================湿度
    
    JsonObject results_0_daily_1 = results_0_daily[1];
    const char* results_0_daily_1_date = results_0_daily_1["date"]; // "2019-12-22"
    const char* results_0_daily_1_text_day = results_0_daily_1["text_day"]; // "小雨"
    const char* results_0_daily_1_code_day = results_0_daily_1["code_day"]; // "13"
    const char* results_0_daily_1_text_night = results_0_daily_1["text_night"]; // "阴"
    const char* results_0_daily_1_code_night = results_0_daily_1["code_night"]; // "9"
    const char* results_0_daily_1_high = results_0_daily_1["high"]; // "12"
    const char* results_0_daily_1_low = results_0_daily_1["low"]; // "8"
    const char* results_0_daily_1_precip = results_0_daily_1["precip"]; // ""
    const char* results_0_daily_1_wind_direction = results_0_daily_1["wind_direction"]; // "西"
    const char* results_0_daily_1_wind_direction_degree = results_0_daily_1["wind_direction_degree"]; // "270"
    const char* results_0_daily_1_wind_speed = results_0_daily_1["wind_speed"]; // "25.20"
    const char* results_0_daily_1_wind_scale = results_0_daily_1["wind_scale"]; // "4"
    const char* results_0_daily_1_humidity = results_0_daily_1["humidity"]; // "91"
    
    JsonObject results_0_daily_2 = results_0_daily[2];
    const char* results_0_daily_2_date = results_0_daily_2["date"]; // "2019-12-23"
    const char* results_0_daily_2_text_day = results_0_daily_2["text_day"]; // "小雨"
    const char* results_0_daily_2_code_day = results_0_daily_2["code_day"]; // "13"
    const char* results_0_daily_2_text_night = results_0_daily_2["text_night"]; // "小雨"
    const char* results_0_daily_2_code_night = results_0_daily_2["code_night"]; // "13"
    const char* results_0_daily_2_high = results_0_daily_2["high"]; // "12"
    const char* results_0_daily_2_low = results_0_daily_2["low"]; // "11"
    const char* results_0_daily_2_precip = results_0_daily_2["precip"]; // ""
    const char* results_0_daily_2_wind_direction = results_0_daily_2["wind_direction"]; // "东北"
    const char* results_0_daily_2_wind_direction_degree = results_0_daily_2["wind_direction_degree"]; // "59"
    const char* results_0_daily_2_wind_speed = results_0_daily_2["wind_speed"]; // "34.20"
    const char* results_0_daily_2_wind_scale = results_0_daily_2["wind_scale"]; // "5"
    const char* results_0_daily_2_humidity = results_0_daily_2["humidity"]; // "86"
    
    const char* results_0_last_update = results_0["last_update"]; // "2019-12-21T17:23:52+08:00"
    String riqi = results_0_last_update;
    int nian = riqi.lastIndexOf("T");
    Year = (riqi.substring(nian-10, nian-6)).toInt();
    Month = (riqi.substring(nian-5, nian-3)).toInt();
    Day = (riqi.substring(nian-2, nian)).toInt();
    
    Serial.printf("北京时间：%d时%d分%d秒\n",Hour+8,Minute,Second);
    Serial.printf("        %d年%d月%d日\n",Year,Month,Day);
    Serial.printf("当前定位：%s\n",results_0_location_name);
    Serial.printf("今天：\n天气：%s\n最高温度：%s℃\n最低温度：%s℃\n",results_0_daily_0_text_day,results_0_daily_0_high,results_0_daily_0_low);
    Serial.printf("明天：\n天气：%s\n最高温度：%s℃\n最低温度：%s℃\n",results_0_daily_1_text_day,results_0_daily_1_high,results_0_daily_1_low);
    Serial.printf("后天：\n天气：%s\n最高温度：%s℃\n最低温度：%s℃\n",results_0_daily_2_text_day,results_0_daily_2_high,results_0_daily_2_low);
}
