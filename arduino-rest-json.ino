#include <Arduino.h>

#include <Ethernet.h>
#include <SPI.h>
#include "RestClient.h"
#include <ArduinoJson.h>

#define delayTime 300     // Time in seconds beetwen sendings
#define IP "192.168.0.10"  // Server IP
#define PORT 5000         // Server Port

const int pinRandom1 = A0;
const int pinRandom2 = A5;


RestClient client = RestClient(IP, PORT);

void setup() {
  
  pinMode(pinRandom1, INPUT);
  pinMode(pinRandom2, INPUT);
  
  Serial.begin(9600);
  // Connect via DHCP
  Serial.println("connect to network");
  client.dhcp();
  
  /*
  // Can still fall back to manual config:
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  //the IP address for the shield:
  byte ip[] = { 192, 168, 1, 101 };
  Ethernet.begin(mac,ip);
  */  
  Serial.println("Setup!");
}

void loop() {
  String response= "";
  char json[256];

  // JSON_ARRAY_SIZE(n)
  // n*JSON_OBJECT_SIZE(2)
  // SIENDO n = CANTIDAD DE OBJETOS DENTRO DEL ARRAY
  const size_t bufferSize = JSON_ARRAY_SIZE(2) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(2);
  DynamicJsonBuffer jsonBuffer(bufferSize);

  JsonObject& root = jsonBuffer.createObject();

  JsonArray& sensores = root.createNestedArray("sensores");

  JsonObject& sensores_0 = sensores.createNestedObject();
  sensores_0["id"] = 1;
  sensores_0["valor"] = analogRead(pinRandom1);

  JsonObject& sensores_1 = sensores.createNestedObject();
  sensores_1["id"] = 2;
  sensores_1["valor"] = analogRead(pinRandom2);
  
  
  root.printTo(json, sizeof(json));
  root.printTo(Serial);
  Serial.println();
  
  client.setContentType("application/json");
  
  int statusCode = client.post("/api/test", json, &response);
  //int statusCode = client.get("/api/test", &response);
    
  Serial.print("Status :");
  Serial.println(statusCode);
  
  Serial.print("Response :");
  Serial.println(response);

}
/*
void get(){
  int statusCode = client.get("/api/test", &response);

  Serial.print("Status :");
  Serial.println(statusCode);
  
  Serial.print("Response :");
  Serial.println(response);
}
*/
