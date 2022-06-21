#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
void setup() {
 Serial.begin(9600);
 dht.begin();
}
void loop() {
 // Wait for 10 minutes between measurements.
 delay(100);
 float h = dht.readHumidity();
 // Read temperature as Celsius
 float t = dht.readTemperature();
 // Check if any reads failed and exit early (to try again).
 if (isnan(h) || isnan(t) ) {
 Serial.println("Failed to read from DHT sensor!");
 return;
 }
 //Serial.print("Humidity: ");
//Serial.println(h);
// Serial.print(",");
//// Serial.print(" %\t");

 //Serial.print("Temperature: ");
 Serial.println(t);
//Serial.print(" *C ");
 delay(1000);
}
