// This Program is developed to aquire sensor data from DHT22 Sensor
// To trasmitt the aquired data using the XBEE RF module (router 2) 
// over serial communication link
// Created by Dr. Nandkishor Motiram Dhawale and Kiran Ashok Shinde
// Date of Creation Dec 2020
// Project Name :  Aquire sensor data from DHT22 Sensor
// Recreated and Implemented by :- Kiran Shinde and Rohan Kulkarni. For PVPIT Budhgaon, I/C - R&D Cell
// Under Guidence of Dr.N.M.Dhawale, I/c- R&D Cell, P.V.P.I.T Budhgaon, Sangli, India in Dec 2020 


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
  Serial.print("$R1"); Serial.print(',');
  Serial.print(h); Serial.print(',');
  Serial.println(t);
  delay(1900); // delay in between reads for stability
//Serial.println(t);
}
