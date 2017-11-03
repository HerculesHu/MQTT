#include <ArduinoJson.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>


const char* ssid = "hzy";
const char* password = "88888888";
const char* mqtt_server = "172.18.93.1";//服务器的地址 iot.eclipse.org是开源服务器


//全局变量区域上界







//全局变量区域下界

WiFiClient espClient;
PubSubClient client(espClient);

int OTA=0;
int OTAS=0;
long lastMsg = 0;//存放时间的变量 
char msg[200];//存放要发的数据
String load;


void setup_wifi() {//自动连WIFI接入网络
  delay(10);
    WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}



void reconnect() {//等待，直到连接上服务器
  while (!client.connected()) {//如果没有连接上
    int randnum = random(0, 999); 
    if (client.connect("OTADEMO"+randnum)) {//接入时的用户名，尽量取一个很不常用的用户名
      client.subscribe("testin");
    } else {
      Serial.print("failed, rc=");//连接失败
      Serial.print(client.state());//重新连接
      Serial.println(" try again in 5 seconds");//延时5秒后重新连接
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {//用于接收服务器接收的数据
  load="";
  for (int i = 0; i < length; i++) {
      load +=(char)payload[i];//串口打印出接收到的数据
  }
   decodeJson();
}

void  decodeJson() {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(load);
   OTA = root["OTA"];
   OTAS =OTA;
   //接收数据json处理区上界

   //添加其他自己的JSON收听处理方式就像这样  int Activity=root["ACT"];

   //接收数据json处理区下界
}

void OTAsetup(){
   if(OTAS){
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  OTAS=0;
   }
}


void setup() {
  //setup代码区域上界



  //填写自己的逻辑代码
  Serial.begin(9600);


  //setup代码区域下界

   setup_wifi();//自动连WIFI接入网络
  client.setServer(mqtt_server, 1883);//1883为端口号
  client.setCallback(callback); //用于接收服务器接收的数据
}

void loop() {
        if(OTA){
          OTAsetup();
        ArduinoOTA.handle();
       }
       else{
        reconnect();//确保连上服务器，否则一直等待。
        client.loop();//MUC接收数据的主循环函数。
        //loop代码上界


        //自己的逻辑代码



        //loop代码下界   
         long now = millis();//记录当前时间
        if (now - lastMsg > 1000) {//每隔100毫秒秒发一次数据
           encodeJson();
           client.publish("testout",msg);
          lastMsg = now;//刷新上一次发送数据的时间
        }
       }
}

void encodeJson(){
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root1 = jsonBuffer.createObject();
  //发送数据区上界


  //添加其他要发送的JSON包就像这样下面这句代码
  root1["back"] = "OTA";


  //发送数据区下界
  root1.printTo(msg);
  }
