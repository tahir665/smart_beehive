#include "FirebaseESP32.h"  // Install Firebase ESP8266 library
#include <WiFi.h>
#define WIFI_SSID "speed"
#define WIFI_PASSWORD "18678a20"
#define FIREBASE_HOST "https://beehive-96465-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "GjNAeZKgUPAbwaoNRZ7E4S7FoUJlYNsbbVsqC1wF"


FirebaseData fbdo;
FirebaseData getdata1;
FirebaseData firebaseData;
FirebaseJson json;


int d1=0;
float d2=0.0000;
float d3=0.0000;


void setup() {
  Serial.begin(9600);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  
}

void loop() {
  Firebase.getInt(getdata1,"/Node2/H2");
   d1=getdata1.intData();
   Serial.println("humidity");
   Serial.println(d1);
   Firebase.setInt(firebaseData, "/Mesh/H2", d1);
   
  Firebase.getInt(getdata1,"/Node2/T2");
   d1=getdata1.intData();
   Serial.println("Temprature");
   Serial.println(d1);
   Firebase.setInt(firebaseData, "/Mesh/T2", d1);

  Firebase.getFloat(getdata1,"/Node2/W2");
   d2=getdata1.intData();
   Serial.println("weight");
   Serial.println(d2);
   Firebase.setFloat(firebaseData, "/Mesh/W2", d2);


  Firebase.getInt(getdata1,"/Node2/H3");
   d1=getdata1.intData();
   Serial.println("humidity 2");
   Serial.println(d1);
   Firebase.setInt(firebaseData, "/Mesh/H3", d1);
   
  Firebase.getInt(getdata1,"/Node2/T3");
   d1=getdata1.intData();
   Serial.println("Temprature 2");
   Serial.println(d1);
   Firebase.setInt(firebaseData, "/Mesh/T3", d1);

  Firebase.getFloat(getdata1,"/Node2/W3");
   d3=getdata1.intData();
   Serial.println("weight 2");
   Serial.println(d3);
  Firebase.setFloat(firebaseData, "/Mesh/W3", d3);
}
