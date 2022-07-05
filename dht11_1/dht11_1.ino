
// This Program is developed to aquire sensor data from DHT22 Sensor
// Created by Dr. Nandkishor Motiram Dhawale and Kiran Ashok Shinde
// Date of Creation Dec 2020
// Project Name :  Aquire sensor data from DHT11 Sensor
// Recreated and Implemented by :- Kiran Shinde and Rohan Kulkarni. For PVPIT Budhgaon, I/C - R&D Cell
// Under Guidence of Dr.N.M.Dhawale, I/c- R&D Cell, P.V.P.I.T Budhgaon, Sangli, India in Dec 2020 


#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin(); // initialize the sensor
}

void loop() {
  // wait a few seconds between measurements.
  delay(1000);

  // read humidity
  float humi  = dht.readHumidity();
  // read temperature as Celsius
  delay(10);
  float tempC = dht.readTemperature();
  delay(10);
 
  // read temperature as Fahrenheit
  float tempF = dht.readTemperature();
  delay(10);
 

  // check if any reads failed
  if (isnan(humi) || isnan(tempC) || isnan(tempF)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("Humidity: ");
    Serial.print(humi);
    Serial.print("%");

    Serial.print("  |  "); 

    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print("°C ~ ");
    Serial.print(tempF);
    Serial.println("°F");
  }
}
