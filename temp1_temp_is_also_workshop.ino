#include "DHTesp.h"
#include <ESP8266WiFi.h>
 
String apiKey = "BDSXA56NHQYRNS5Q";    
const char *ssid =  "WiFi";     
const char *pass =  "Password";
const char* server = "api.thingspeak.com";
//int pirPin = 12;           //pir sensor pin GPIO
//int buzzer_pin=04;       //buzzer pin GPIO
int state = LOW;

WiFiClient client;  
DHTesp dht;

void setup()
{
  //pinMode(buzzer_pin,OUTPUT);
  //pinMode(pirPin, INPUT);
  Serial.begin(115200);
  Serial.println("connecting to");
  Serial.println(ssid);                            //wifi connection config
  WiFi.begin(ssid,pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected");                  //connected
  Serial.println();
  String thisBoard= ARDUINO_BOARD;
  Serial.println(thisBoard);
  dht.setup(02, DHTesp::DHT11);
}

void loop()
{
  //pir and buzzer ends
  delay(dht.getMinimumSamplingPeriod());  //temperature and humidity begins
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  Serial.println(dht.getStatusString());
  Serial.println(humidity);
  Serial.println(temperature);
  delay(2000);

  if (client.connect(server,80))      //thingspeak upload
  {
    String postStr= apiKey;
    postStr+="&field1=";
    postStr+=String(temperature);
    postStr+="&field2=";
    postStr+=String(humidity);
    postStr+="\r\n\r\n";
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);             //thingspeak done
  }
  client.stop();
  Serial.println("waiting");
  delay(1000);
  
  }
  
