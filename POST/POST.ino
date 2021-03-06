/*
 *  HTTP over TLS (HTTPS) example sketch
 *
 *  This example demonstrates how to use
 *  WiFiClientSecure class to access HTTPS API.
 *  We fetch and display the status of
 *  esp8266/Arduino project continuous integration
 *  build.
 *
 *  Created by Ivan Grokhotkov, 2015.
 *  This example is in public domain.
 */

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid = "CITI-Terreo"; // nome da rede
const char* password = "1cbe991a14"; // senha

// servidor para envio
const char* host = "api.thingspeak.com";
const int httpsPort = 443;
String url;

WiFiClientSecure client;

int sensorValue;


void setup() {
  
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  
} 
    
  
void loop() {
  // Use WiFiClientSecure class to create TLS connection  
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  } 
  
  sensorValue = analogRead(A0); //pino analógico ligado ao sensor
  Serial.println(sensorValue);
  url = "/update.json?api_key=4QS8PYAXTT6TXQDC&field1="+String(sensorValue);
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");
  
  delay(16000); //delay de envio (mínimo 15s)

}
