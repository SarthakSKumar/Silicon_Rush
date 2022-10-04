
#include <FirebaseESP32.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#define WIFI_SSID "*********************"      // your wifi SSID
#define WIFI_PASSWORD "*********************"  // your wifi PASSWORD

#define LED 12
#define FIREBASE_HOST "*********************"  // change here
#define FIREBASE_AUTH "*********************"  // your private key
FirebaseData firebaseData;

void setup() {
    pinMode(LED, OUTPUT);
    Serial.begin(115200);
    // connect to wifi.
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("connecting");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println();
    Serial.print("connected: ");
    Serial.println(WiFi.localIP());
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);
}
void loop() {
    if (Firebase.getString(firebaseData, "/Motor1Status")) {
        String ledstatus = firebaseData.stringData();
        if (ledstatus.toInt() == 1) {
            digitalWrite(LED, LOW);
            Serial.println("on");
        } else {
            digitalWrite(LED, HIGH);
            Serial.println("off");
        }
    } else {
        Serial.print("Error in getInt, ");
        Serial.println(firebaseData.errorReason());
    }
}
