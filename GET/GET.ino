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

// servidor para receber dados
const char* host = "api.thingspeak.com";
const int httpsPort = 443;

WiFiClientSecure client;

String url = "/channels/219279/feeds/last.txt"; 
String line;
int pos;
int ledPin = D0; //pino digital ligado ao LED

void setup() {

  pinMode(ledPin, OUTPUT);
  //pinMode(BUILTIN_LED, OUTPUT);
  
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
  
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {
    line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  
  Serial.println("==========");
  line = client.readStringUntil('\r');  
  line = client.readStringUntil('\r');
  Serial.println("closing connection");  
  
  pos = line.indexOf("field1"); // pegar field1
  line = line.substring(pos, line.length());
  Serial.println(line);

  if (line.indexOf("ON") != -1) {
      digitalWrite(ledPin, HIGH);
      //digitalWrite(BUILTIN_LED, HIGH);
      Serial.println("ON!");
  }
  else {
      digitalWrite(ledPin, LOW);
      //digitalWrite(BUILTIN_LED, LOW);
      Serial.println("OFF!");
  }

  delay(16000);
  
}
