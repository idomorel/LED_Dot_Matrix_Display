#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <SPI.h>
#include <DMD2.h>
//#include <TimerOne.h>
#include "SystemFont5x7.h"
#include "Arial_black_16.h"
#include "Droid_Sans_24.h"


#define ROW 3
#define COLUMN 2
// #define FONT Droid_Sans_24
#define FONT Arial_Black_16


IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

const char* WifiName="DisplayMatrix";
const char* WifiPass="12345678";
char MESSAGE[255]; //Store the message

bool flag = false;
//bool textChange = false;
long start = 0;
int num = 0;
String webPage,notice;
int posX, pixelLength;

ESP8266WebServer server(80);

//SPIDMD led_module(ROW, COLUMN);
SPIDMD led_module(ROW , COLUMN, 5, 16, 2, 12); 
DMD_TextBox box(led_module, 0, 0);


const char htmlPage[]PROGMEM=R"=====(
<!DOCTYPE html>
<html>
<body>
<h3>Electronic Notice Board </h3>
<FORM METHOD="POST"action="/postForm">
<input type="text" name="myText" value="Write your Notice...">
<input type="submit" value="Post Notice">
</form>
</body>
</html>
)=====";

void handlePostForm()
{
 Serial.println("Handler Reached");
 //webPage=htmlPage;
 Serial.println("Handler Reached 1");
 notice=server.arg("myText");
 int msg_len = notice.length() + 1; 
 MESSAGE[msg_len];
 notice.toCharArray(MESSAGE, msg_len);
 pixelLength = notice.length()*14;
 flag = false;
  Serial.println("Read the Text");
 Serial.println("Text Received, contents:");
 Serial.println(notice);
 server.send(200,"text/html",htmlPage);
}

// void scan_module()
// {
//   led_module.scanDisplayBySPI();
// }

void setup()
{
  led_module.begin();
  //Timer1.initialize(2000);
  //Timer1.attachInterrupt(scan_module);
  led_module.clearScreen();
    
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.print("Connecting");
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(WifiName, WifiPass);
    
  delay(50);
  //WiFi.begin(WifiName,WifiPass);
  server.on("/postForm",handlePostForm);
  server.on("/",handle_OnConnect);
  //while(WiFi.status()!= WL_CONNECTED)
  //{
  //delay(500);
  //Serial.print(".");
  //} 
  Serial.println("");
  Serial.println("Wi-Fi Connected");
  Serial.println("IP Address is:");
  Serial.println("192.168.1.1");
  //Serial.println(WiFi.localIP());
  Serial.println(notice);
  server.begin();
  Serial.println("HTTP Server Started");
  notice = "Try";
  //pixelLength = notice.length()*14;
  pixelLength = 0;
  for(int i = 0; i < notice.length(); i++){
    int asciiVal = (int)notice[i];
    pixelLength += (int)Arial_Black_16[asciiVal];
  }
  Serial.println(Serial.println(WiFi.localIP()));
  //Arial_Black_16[38]
  // put your setup code here, to run once:
}

void handle_OnConnect()
{
  Serial.println("Client Connected");
  server.send(200, "text/html", htmlPage); 
}

// void Scroll(){
//   const char *next = MESSAGE;
//   while(*next) {
//     Serial.print(*next);
//     box.print(*next);
//     delay(200);
//     next++;
//   }
// }

void loop() {
  num++;
  //Serial.println("Loop Started");
  server.handleClient();
  led_module.selectFont(FONT);
  //led_module.drawMarquee("Sheduza!", 25, (32 * ROW), 0);
  
  if(flag == false){
    box.clear();
    start = millis();
    //led_module.clearScreen();
    //led_module.drawString(posX, 4, notice);
    //led_module.drawString(posX, 0, notice);
    //box.print("        ");
    box.print(notice);
    //oldNotice = notice;
    flag = true;
    posX = 0;
  }
  
  if(millis()-100 > start){
    start = millis();
    //led_module.clearScreen();
    if(posX == (pixelLength + 96)* -1){
       posX = 0;
       flag = false;
    }
    //posX--;
    //led_module.drawString(posX, 4, notice);
    //led_module.drawString(posX, 0, notice);
    //led_module.marqueeScrollX(-1);
    //box.print(notice);
    box.scrollX(-1);
    posX--;
  }

  //Serial.print("Loop number:  ");
  //Serial.println(num);
  Serial.println(notice);
  
  //delay(50);
  // long start = millis();
  // // long timming = start;
  // boolean flag = false;
  // // while(!flag)
  // // {
  //   if ((start + 20) < millis())
  //   {
  //     led_module.marqueeScrollX(1);
  //     flag = true;
  //     stepped = true;
  //     start = millis();
  //   }
  // }
  // put your main code here, to run repeatedly:

}























// #include <ESP8266WiFi.h>
// #include <ESP8266WebServer.h>

// /* Put your SSID & Password */
// const char* ssid = "NodeMCU";  // Enter SSID here
// const char* password = "12345678";  //Enter Password here

// /* Put IP Address details */
// IPAddress local_ip(192,168,1,1);
// IPAddress gateway(192,168,1,1);
// IPAddress subnet(255,255,255,0);

// ESP8266WebServer server(80);

// uint8_t LED1pin = D7;
// bool LED1status = LOW;

// uint8_t LED2pin = D6;
// bool LED2status = LOW;

// void setup() {
//   Serial.begin(115200);
//   pinMode(LED1pin, OUTPUT);
//   pinMode(LED2pin, OUTPUT);

//   WiFi.softAP(ssid, password);
//   WiFi.softAPConfig(local_ip, gateway, subnet);
//   delay(100);
  
//   server.on("/", handle_OnConnect);
//   server.on("/led1on", handle_led1on);
//   server.on("/led1off", handle_led1off);
//   server.on("/led2on", handle_led2on);
//   server.on("/led2off", handle_led2off);
//   server.onNotFound(handle_NotFound);
  
//   server.begin();
//   Serial.println("HTTP server started");
// }
// void loop() {
//   server.handleClient();
//   if(LED1status)
//   {digitalWrite(LED1pin, HIGH);}
//   else
//   {digitalWrite(LED1pin, LOW);}
  
//   if(LED2status)
//   {digitalWrite(LED2pin, HIGH);}
//   else
//   {digitalWrite(LED2pin, LOW);}
// }

// void handle_OnConnect() {
//   LED1status = LOW;
//   LED2status = LOW;
//   Serial.println("GPIO7 Status: OFF | GPIO6 Status: OFF");
//   server.send(200, "text/html", SendHTML(LED1status,LED2status)); 
// }

// void handle_led1on() {
//   LED1status = HIGH;
//   Serial.println("GPIO7 Status: ON");
//   server.send(200, "text/html", SendHTML(true,LED2status)); 
// }

// void handle_led1off() {
//   LED1status = LOW;
//   Serial.println("GPIO7 Status: OFF");
//   server.send(200, "text/html", SendHTML(false,LED2status)); 
// }

// void handle_led2on() {
//   LED2status = HIGH;
//   Serial.println("GPIO6 Status: ON");
//   server.send(200, "text/html", SendHTML(LED1status,true)); 
// }

// void handle_led2off() {
//   LED2status = LOW;
//   Serial.println("GPIO6 Status: OFF");
//   server.send(200, "text/html", SendHTML(LED1status,false)); 
// }

// void handle_NotFound(){
//   server.send(404, "text/plain", "Not found");
// }

// String SendHTML(uint8_t led1stat,uint8_t led2stat){
//   String ptr = "<!DOCTYPE html> <html>\n";
//   ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
//   ptr +="<title>LED Control</title>\n";
//   ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
//   ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
//   ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
//   ptr +=".button-on {background-color: #1abc9c;}\n";
//   ptr +=".button-on:active {background-color: #16a085;}\n";
//   ptr +=".button-off {background-color: #34495e;}\n";
//   ptr +=".button-off:active {background-color: #2c3e50;}\n";
//   ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
//   ptr +="</style>\n";
//   ptr +="</head>\n";
