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

const char* ssid = "Link_One_3DC6F8";
const char* password = "LePapitoDonaEli2014";

//const char* host = "api.github.com";
const char* host = "api.thingspeak.com";
const int httpsPort = 443;

int ledPin = D4;


// Use web browser to view and copy
// SHA1 fingerprint of the certificate
//const char* fingerprint = "CF 05 98 89 CA FF 8E D8 5E 5C E0 C2 E4 F7 E6 C3 C7 50 DD 5C";

void setup() {

  //pinMode(ledPin, OUTPUT);
  pinMode(BUILTIN_LED, OUTPUT);
  
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

  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  //if (client.verify(fingerprint, host)) {
    //Serial.println("certificate matches");
  //} else {
    //Serial.println("certificate doesn't match");
  //}

  //String url = "/repos/esp8266/Arduino/commits/master/status";
  String url = "/channels/219279/feeds.json";
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  
  Serial.println("==========");
  String line = client.readStringUntil('\r');
  Serial.print(line);
  line = client.readStringUntil('\r');
  Serial.print(line);
  Serial.println("");
  
  //if (line.startsWith("{\"state\":\"success\"")) {
    //Serial.println("esp8266/Arduino CI successfull!");
  //} else {
    //Serial.println("esp8266/Arduino CI has failed");
  //}
  //Serial.println("reply was:");
  //Serial.println("==========");
  //Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");
  int pos;
  while (line.indexOf("field1") != -1) {
    pos = line.indexOf("field1");
    line = line.substring(pos+1, line.length());
    Serial.println(line);
  }
  if (line.indexOf("ON") != -1) {
      //digitalWrite(ledPin, LOW);
      digitalWrite(BUILTIN_LED, HIGH);
      Serial.println("ON!");
  }
  else {
      //digitalWrite(ledPin, HIGH);
      digitalWrite(BUILTIN_LED, LOW);
      Serial.println("OFF!");
  }
}

void loop() {
}
