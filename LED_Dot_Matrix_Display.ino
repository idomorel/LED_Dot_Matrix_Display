#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <SPI.h>
#include <DMD2.h>
//#include <TimerOne.h>
#include "SystemFont5x7.h"
#include "Arial_black_16.h"


#define ROW 1
#define COLUMN 1
#define FONT Arial_Black_16



IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

const char* WifiName="*****";
const char* WifiPass="******";
String webPage,notice;

ESP8266WebServer server(80);

DMD2 led_module(ROW, COLUMN);


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
 webPage=htmlPage;
 notice=server.arg("myText");
 Serial.println("Text Received, contents:");
 Serial.println(notice);
 server.send(200,"text/html",webPage);
}

void scan_module()
{
  led_module.scanDisplayBySPI();
}


   void setup()
   {
  //Timer1.initialize(2000);
  //Timer1.attachInterrupt(scan_module);
  led_module.clearScreen( true );

  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.print("Connecting");
  WiFi.softAP(WifiName, WifiPass);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  //WiFi.begin(WifiName,WifiPass);
  server.on("/postForm",handlePostForm);
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
  // put your setup code here, to run once:
}


void loop() {
  server.handleClient();
  led_module.selectFont(FONT);
  led_module.drawMarquee("Welcome to Circuit Digest", 25, (32 * ROW), 0);
  long start = millis();
  long timming = start;
  boolean flag = false;
  while (!flag)
  {
    if ((timming + 20) < millis())
    {
      flag = led_module.stepMarquee(-1, 0);
      timming = millis();
    }
  }
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
