#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "hzy";//连接的路由器的名字
const char* password = "88888888";//连接的路由器的密码
const char* mqtt_server = "172.18.93.1";//服务器的地址 iot.eclipse.org是开源服务器

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;//存放时间的变量 
char msg[100];//存放要发的数据

void setup_wifi() {//自动连WIFI接入网络
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {//用于接收服务器接收的数据

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);//串口打印出接收到的数据
  }
  Serial.println();//换行

}

void reconnect() {//等待，直到连接上服务器
  while (!client.connected()) {//如果没有连接上
    int randnum = random(0, 999); 
    if (client.connect("mqtt"+randnum)) {//接入时的用户名，尽量取一个很不常用的用户名
      client.subscribe("testin");//接收外来的数据时的intopic
    } else {
      Serial.print("failed, rc=");//连接失败
      Serial.print(client.state());//重新连接
      Serial.println(" try again in 5 seconds");//延时5秒后重新连接
      delay(5000);
    }
  }
}

void setup() {//初始化程序，只运行一遍
  Serial.begin(115200);//设置串口波特率（与烧写用波特率不是一个概念）
  setup_wifi();//自动连WIFI接入网络
  client.setServer(mqtt_server, 1883);//1883为端口号
  client.setCallback(callback); //用于接收服务器接收的数据
}



void loop() {//主循环
  reconnect();//确保连上服务器，否则一直等待。
  client.loop();//MUC接收数据的主循环函数。
  long now = millis();//记录当前时间
  if (now - lastMsg > 1000) {//每隔1秒发一次信号
    client.publish("testout", msg);//发送数据，其中testout是发出去的outtopic
    lastMsg = now;//刷新上一次发送数据的时间
  }
}

