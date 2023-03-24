TaskHandle_t getdata;
#include "painlessMesh.h"
//#include "namedMesh.h"
#include <FirebaseESP32.h>
const char* ssid_wifi = "speed";
const char* password_wifi = "18678a20";
String recieveddata = "";
#define FIREBASE_HOST "https://beehive-8de67-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "zX05YN4tSclTxf3p59J9osldt5oIcieSDNHGQ4pR"
FirebaseData firebaseData;
FirebaseData firebaseData1;
int messageswaiting;
// WiFi Credentials
#define   MESH_PREFIX     "beehive"
#define   MESH_PASSWORD   "beehive12345"
#define   MESH_PORT       5555
#define HOSTNAME "FireBase_Bridge"
String nodeName = "123";
bool incoming_data0 = false;
bool incoming_data1 = false;
bool incoming_data2 = false;

String path = "/devices/";
String address_2 = "Node2";
int Temp2 = 0;
int Hum2 = 0;
float weight2 = 0;

String address_3 = "Node3";
int Temp3 = 0;
int Hum3 = 0;
float weight3 = 0;
//Pin Declaration

Scheduler userScheduler; 
painlessMesh  mesh;
void sendMessage() ; 
Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );
void sendMessage() 
{
// Reading Status of Pushbutton
//  if (digitalRead(Button1) == HIGH)
//    button1_status = !button1_status;
//  if (digitalRead(Button2) == HIGH)
//    button2_status = !button2_status;
// Serializing in JSON Format 
  DynamicJsonDocument doc(1024);
//  doc["Relay1"] = button1_status;
//  doc["Relay2"] = button2_status;
  String msg ;
  serializeJson(doc, msg);
    mesh.sendBroadcast( msg );
  Serial.println(msg);
  taskSendMessage.setInterval((TASK_SECOND * 1));
}
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
  String temp_address_2 = doc["name"];
  if (temp_address_2 == address_2)
  {
    Temp2 = doc["TEMP2"];
    Hum2 = doc["HUM2"];
    weight2 = doc["weight2"];
    incoming_data0 = true;
   Serial.println("From node2");
   Serial.println("Temperature:");
   Serial.print(Temp2);
   Serial.println("Humidity:");
   Serial.print(Hum2);
   Serial.println("Weight:");
   Serial.print(weight2);
  }

  if (temp_address_2 == address_3)
  {
    Temp3 = doc["TEMP3"];
    Hum3 = doc["HUM3"];
    weight3 = doc["weight3"];
     incoming_data1 = true;
  Serial.println("From node3");
   Serial.println("Temperature:");
   Serial.print(Temp3);
    Serial.println("Humidity:");
   Serial.print(Hum3);
     Serial.println("Weight:");
   Serial.print(weight3);
}
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

void Processing(void *parameter) //Function to calculate  values
{
  mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION );  // set before init() so that you can see startup messages
  mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT, WIFI_AP_STA);
  mesh.onReceive(&receivedCallback);
  //mesh.setName(nodeName);
  mesh.stationManual(ssid_wifi, password_wifi);
  while ((WiFi.status() == WL_DISCONNECTED)) {
    Serial.print(". -");
    delay(100);
  }
 // mesh.setHostname(HOSTNAME);
  mesh.setRoot(true);
  mesh.setContainsRoot(true);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  while (true) {
    mesh.update();
    vTaskDelay(10);
  }
}

void setup() {
  Serial.begin(115200);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  if (!Firebase.beginStream(firebaseData1, path + "/" ))
  {
    Serial.println("Could not begin stream on node 1");
    Serial.println("REASON: " + firebaseData1.errorReason());
    Serial.println();
  }

  xTaskCreatePinnedToCore(
    Processing, /* Function to implement the task */
    "getdata",    /* Name of the task */
    8000,        /* Stack size in words */
    NULL,        /* Task input parameter */
    1,           /* Priority of the task */
    &getdata,     /* Task handle. */
    0);          /* Core where the task should run */
}
void loop() {
  if ((WiFi.status() != WL_DISCONNECTED)) {
    String address;
    int temp ;
    int hum ;
    float weight ;

    if (incoming_data0 == true) {
      Serial.println("Sent0");
      address = address_2;
      temp = Temp2;
      hum = Hum2;
      weight=weight2;
      
      Firebase.setInt(firebaseData,"/Mesh/H2", hum);
      Firebase.setInt(firebaseData,"/Mesh/T2", temp);
      Firebase.setFloat(firebaseData,"/Mesh/W2", weight);
      incoming_data0 = false;
    }
    if (incoming_data1 == true) {
      Serial.println("Sent1");
      address = address_3;
      temp = Temp3;
      hum = Hum3;
      weight = weight3;
      

      Firebase.setInt(firebaseData,"/Mesh/H3", hum);
      Firebase.setInt(firebaseData,"/Mesh/T3", temp);
      Firebase.setFloat(firebaseData,"/Mesh/W3", weight);
      incoming_data1 = false;
    }
  }

}
