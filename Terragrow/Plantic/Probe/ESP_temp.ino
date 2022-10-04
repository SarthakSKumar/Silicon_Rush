#include <FirebaseESP32.h>
#include <WiFi.h>
#include <stdlib.h>

#include "DHTesp.h"

#define DHTpin 15
DHTesp dht;

char buff[10];
#define FIREBASE_HOST "*******************"
#define FIREBASE_AUTH "*******************"
#define WIFI_SSID "*******************"
#define WIFI_PASSWORD "*******************"
#define LED 2

// Define FirebaseESP32 data object
FirebaseData firebaseData;
FirebaseJson json;

int x;
String mytemp;
String myhum;

FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

#define deviceID "ESP102"

void setup() {
    pinMode(LED, OUTPUT);
    Serial.begin(115200);
    delay(2000);
    dht.setup(DHTpin, DHTesp::DHT11);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);

    // Set database read timeout to 1 minute (max 15 minutes)
    Firebase.setReadTimeout(firebaseData, 1000 * 60);
    // tiny, small, medium, large and unlimited.
    // Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
    Firebase.setwriteSizeLimit(firebaseData, "tiny");

    /*
    This option allows get and delete functions (PUT and DELETE HTTP requests) works for device connected behind the
    Firewall that allows only GET and POST requests.

    Firebase.enableClassicRequest(firebaseData, true);
    */

    // String path = "/data";
    Serial.println("------------------------------------");
    Serial.println("Connected...");
    digitalWrite(LED, HIGH);
}

void loop() {
    digitalWrite(LED, HIGH);
    x = random(80, 90);
    delay(dht.getMinimumSamplingPeriod());
    if (Firebase.ready()) {
        // Firebase.setInt(fbdo, main, 5);
        Firebase.setInt(fbdo, "/ESP102/soilMoisture", x);
    }
    float humidity = dht.getHumidity();
    float temperature = dht.getTemperature();
    myhum = dtostrf(humidity, 3, 2, buff);
    mytemp = dtostrf(temperature, 3, 2, buff);
    Serial.println(myhum);
    Serial.println(mytemp);

    delay(100);
    json.set("/hum", myhum);
    json.set("/temp", mytemp);
    Firebase.updateNode(firebaseData, "/ESP102", json);
}
