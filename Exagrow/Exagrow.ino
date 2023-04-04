#include "DHT.h"

#define DHTTYPE DHT11

int sensor_pin = 14;
int dht_pin = 27;
int ir_pin = 25;
int led_pin = 20; //Assign one pin for LED, to glow.

DHT dht(dht_pin, DHTTYPE); //Initialise DHT object

int moist_val;

void setup() {
  Serial.begin(9600);
  Serial.println("Sensor Data\n");
  pinMode(sensor_pin, INPUT);
  pinMode(dht_pin, INPUT);
  pinMode(ir_pin, INPUT);
  pinMode(led_pin, OUTPUT);
  dht.begin();
}

void loop() {
  moist_val = map(analogRead(sensor_pin), 550, 0, 0, 100);
  Serial.print("Moisture : ");
  Serial.print(moist_val);
  Serial.print("%\n");
  delay(500);

  float humidity = dht.readHumidity();
  float temperature_C = dht.readTemperature();
  float temperature_F = dht.readTemperature(true);
  delay(500);
  if (isnan(humidity) || isnan(temperature_C) || isnan(temperature_F)) {
    Serial.println("Unable to read from DHT!");
    return;
  }
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%, Temperature(C): ");
  Serial.print(temperature_C);
  Serial.print("°C, Temperature(F): ");
  Serial.print("temperature_F");
  Serial.print("F\n");
  delay(500); //Prints values from DHT

  if(digitalRead(ir_pin) == 1){
    digitalWrite(led_pin, 1);
  }
  else if(digitalRead(ir_pin) == 0){
    digitalWrite(led_pin, 0);
  }
}
