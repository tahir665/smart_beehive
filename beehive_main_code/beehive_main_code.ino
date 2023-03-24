#include "FirebaseESP8266.h"  // Install Firebase ESP8266 library
#include <ESP8266WiFi.h>
#define WIFI_SSID "speed"
#define WIFI_PASSWORD "18678a20"
#define FIREBASE_HOST "https://beehive-96465-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "GjNAeZKgUPAbwaoNRZ7E4S7FoUJlYNsbbVsqC1wF"

#include <SimpleDHT.h>
#include "HX711.h"

#define cal1 370 
#define data1  4
#define sck  0

FirebaseData fbdo;
FirebaseJson json;

HX711 scale1;

int pinDHT1 = 16;

byte T1,H1;
float weight=0.0000;
SimpleDHT11 dht1(pinDHT1);


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
  
  scale1.begin(data1, sck);
  scale1.set_scale(cal1); 
  scale1.tare(); 
  pinMode(5,OUTPUT);
}

void loop() {
 T1 =0; H1=0;
 dht1.read(&T1, &H1, NULL);
Serial.print("1st "); Serial.print((int)T1); Serial.print(" *C, "); 
  Serial.print((int)H1); Serial.println(" H");
  Serial.print("1st : ");
  weight=scale1.get_units();
  Serial.print(weight); 
  Serial.println(" g");
  Serial.println(".............."); 
  if (T1>=30)
  {
    digitalWrite(5,HIGH);
  }
   if (T1<30)
  {
    digitalWrite(5,LOW);
  }
  String H_1 = String(H1);
  String T_1 = String(T1);
  String W_1 = String(weight);
  Firebase.setInt(fbdo, "/Node1/H1", H_1.toInt());
  Firebase.setInt(fbdo, "/Node1/T1", T_1.toInt());
  Firebase.setString(fbdo, "/Node1/W1", W_1);
  
  delay(1500);
}
