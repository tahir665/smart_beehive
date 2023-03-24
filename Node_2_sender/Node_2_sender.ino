#include "painlessMesh.h"
#include <SimpleDHT.h>
//#include "HX711.h"
#include <HX711_ADC.h>
#include <Wire.h>
#define cal1 -900.0 
#define data1  4
#define sck  0
// WiFi Credentials
#define   MESH_PREFIX     "beehive"
#define   MESH_PASSWORD   "beehive12345"
#define   MESH_PORT       5555
//Pin Declaration
#define Relay1 5
float a=567.76;
float b=675.76;
int pinDHT1 = 16;
SimpleDHT11 dht1(pinDHT1);
//HX711 scale1;
HX711_ADC LoadCell(4, 0);
//Variables
Scheduler userScheduler; 
painlessMesh  mesh;

byte H,T;
float weight;

void sendMessage() ; 
Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );
void sendMessage()
{
  // Serializing in JSON Format
  DynamicJsonDocument doc(1024);

 T =0; H=0; weight=0;
 dht1.read(&T, &H, NULL);
 String T2=String(T);
 String H2=String(H);
 //Serial.println(scale1.get_units());
 LoadCell.update();
 weight = LoadCell.getData();
 if (T>=30){
  digitalWrite(Relay1,HIGH);
 }
 if(T<=30){
  digitalWrite(Relay1,LOW);
 }
  
//  float h = dht1.readHumidity();
//  float t = dht1.readTemperature();
  doc["name"] = "Node2";
  doc["TEMP2"] = T2.toInt();
  doc["HUM2"] = H2.toInt();
  doc["weight2"]= weight;//weight;
  String msg ;
  serializeJson(doc, msg);
  mesh.sendBroadcast( msg );
  Serial.println("from node2");
  Serial.println(msg);
  taskSendMessage.setInterval((TASK_SECOND * 2));
}
// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  String json;
  DynamicJsonDocument doc(1024);
  json = msg.c_str();
  DeserializationError error = deserializeJson(doc, json);
  if (error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
  }
//  relay1_status = doc["Relay1"];
//  digitalWrite(Relay1, relay1_status);
}
void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}
void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}
void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}
void setup() {
  Serial.begin(115200);
  pinMode(Relay1, OUTPUT);

  LoadCell.begin(); // start connection to HX711
  LoadCell.start(2000); // load cells gets 2000ms of time to stabilize
  LoadCell.setCalFactor(-900);
  
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
}
void loop() {
  mesh.update();
}
