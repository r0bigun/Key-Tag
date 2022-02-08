#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti wifiMulti;

const char* ssid0 = "Your WiFi SSID"; // Change this to your WiFi-home name
const char* pass0 = "Your WiFi password"; // Change this to your WiFi-home password

const char* ssid1 = "Your WiFi SSID"; // Change this to your WiFi-school name
const char* pass1 = "Your WiFi password"; // Change this to your WiFi-school password

const char* host = "api.callmebot.com";

int analogInPin  = 35;    // Analog input pin
int sensorValue;          // Analog Output of Sensor
int bat_percentage;

void battery(){
  sensorValue = analogRead(analogInPin);
  float voltage = (((sensorValue * 3.3) / 3810) * 2); //multiply by two as voltage divider network is 10K & 10K Resistor
 
  bat_percentage = mapfloat(voltage, 3.0, 4.2, 0, 100); //3.0V as Battery Cut off Voltage & 4.2V as Maximum Voltage
 
  if (bat_percentage >= 100)
  {
    bat_percentage = 100;
  }
  if (bat_percentage <= 0)
  {
    bat_percentage = 1;
  }
  Serial.print("\t Battery Percentage = ");
  Serial.println(bat_percentage);
}
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}





void setup()
{
    Serial.begin(115200);
    delay(10);
    WiFi.disconnect(true);
    
    battery();
    wifiMulti.addAP(ssid0, pass0);
    wifiMulti.addAP(ssid1, pass1);
   
   if (wifiMulti.run() == WL_CONNECTED) {
      delay(5000);
      Serial.println("WiFi connected for sleeping");
      ESP.deepSleep(3e8); //5m sleep, 5m = 3e+8 microseconds
      
   }
    
}


void loop(){


if (WiFi.status() != WL_CONNECTED) { 
  Serial.println("Loop again");
  delay(5000);
  process();
  
  
}
  
  
}

void Home(){
    Serial.print("Connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
        Serial.println("Connection failed1");
        return;
    }

    
    String url = "/signal/send.php?phone=+4550238043&apikey=796879&text=Your+child+has+arrived+at+home+%0D+Battery+%20" + String(bat_percentage) + "%25";
	// phone=+45 your phone number, apikey = your apikey, text = write your desired message by adding "+" after each word.

    Serial.print("Requesting URL: ");
    Serial.println(url);

    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");


  Serial.println("Request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  Serial.println("Closing connection");
  client.stop();
  ESP.deepSleep(3e7);// 30s sleep, 30s = 3e+7 
}

void School(){
    Serial.print("Connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
        Serial.println("Connection failed2");
        return;
    }

    
    String url1 = "/signal/send.php?phone=+4550238043&apikey=796879&text=Your+child+has+arrived+at+school+%0D+Battery+%20" + String(bat_percentage) + "%25";
	// phone=+45 your phone number, apikey = your apikey, text = write your desired message by adding "+" after each word.

    Serial.print("Requesting URL: ");
    Serial.println(url1);

    // This will send the request to the server
    client.print(String("GET ") + url1 + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");

  Serial.println("Request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  Serial.println("Closing connection");
  client.stop();
  ESP.deepSleep(3e7);// 30s sleep, 30s = 3e+7 
}

void process() {
    wifiMulti.addAP(ssid0, pass0);
    wifiMulti.addAP(ssid1, pass1);

    // We start by connecting to a WiFi network

    Serial.println("WiFi connecting");
    Serial.println(".");
    delay(500);
    if (wifiMulti.run() == WL_CONNECTED && WiFi.SSID() == ssid0){
    Home();
    }   
    else if (WiFi.SSID() == ssid1) {
      delay(500);
      School();
    }
}
