#define BLYNK_PRINT Serial
 
#include "EmonLib.h"   //https://github.com/openenergymonitor/EmonLib
#include <WiFi.h>
#include <WiFiClient.h>
 
EnergyMonitor emon;
#define vCalibration 100.0
#define currCalibration 0.30

float kWh;
unsigned long lastmillis = millis();

#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

//Provide the token generation process info.
#include <addons/TokenHelper.h>

//Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "*********************"
#define WIFI_PASSWORD "*********************"

//For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "*********************"

/* 3. Define the RTDB URL */
#define DATABASE_URL "*********************"//<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app


//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;


#define deviceID "ESP101"

#define LED 26
 
void setup() {
  Serial.begin(115200);

  pinMode(LED,OUTPUT);
  
  emon.voltage(35, vCalibration, 1.7); // Voltage: input pin, calibration, phase_shift
  emon.current(34, currCalibration); // Current: input pin, calibration.

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

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  config.database_url = DATABASE_URL;



  //////////////////////////////////////////////////////////////////////////////////////////////
  //Please make sure the device free Heap is not lower than 80 k for ESP32 and 10 k for ESP8266,
  //otherwise the SSL connection will fail.
  //////////////////////////////////////////////////////////////////////////////////////////////

  Firebase.begin(DATABASE_URL, API_KEY);

  //Comment or pass false value when WiFi reconnection will control by your code or third party library
 // Firebase.reconnectWiFi(true);

  Firebase.setDoubleDigits(5);
}
 
void loop() {
    float volt,curr,power,units;
    emon.calcVI(20, 100);
    volt = emon.Vrms;
    Serial.print("Vrms: ");
    Serial.print(emon.Vrms, 2);
    Serial.print("V");
    curr = emon.Irms;
    Serial.print("\tIrms: ");
    Serial.print(emon.Irms, 4);
    Serial.print("A");
    power = emon.apparentPower;
    Serial.print("\tPower: ");
    Serial.print(emon.apparentPower, 4);
    Serial.print("W");
    Serial.print("\tkWh: ");
    
    
    kWh = kWh + emon.apparentPower*(millis()-lastmillis)/3600000000.0;
    units = kWh;
    Serial.print(kWh, 4);
    Serial.println("kWh");

    if (Firebase.ready()) 
  {
    
    //Firebase.setInt(fbdo, main, 5);
    Firebase.setInt(fbdo, "/ESP101/voltage", volt);
    Firebase.setInt(fbdo, "/ESP101/current", curr);
    Firebase.setInt(fbdo, "/ESP101/power", power);
    Firebase.setInt(fbdo, "/ESP101/units", units);
    }
    if(Firebase.getString(fbdo, "/Led1Status"))
  {
    String ledstatus = fbdo.stringData();
    if(ledstatus.toInt() == 1){
      digitalWrite(LED,HIGH);
      Serial.println("on");
    }
    else {
      digitalWrite(LED,LOW);
      Serial.println("off");
    }
    }else{
      Serial.print("Error in getInt, ");
      Serial.println(fbdo.errorReason());
    } 
    
    delay(500);
}
