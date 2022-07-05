
// This Program is developed to aquire sensor data from DHT22 Sensor
// To trasmitt the aquired data using the cellular module Sim 900
// over wireess serial communication link to the cloud
// Created by Dr. Nandkishor Motiram Dhawale and Kiran Ashok Shinde
// Date of Creation Sep 2020
// Project Name :  aquire sensor data from DHT22 Sensor
// Recreated and Implemented by :- Kiran Shinde and Rohan Kulkarni. For PVPIT Budhgaon, I/C - R&D Cell
// Under Guidence of Dr.N.M.Dhawale, I/c- R&D Cell, P.V.P.I.T Budhgaon, Sangli, India in Sep 2020 


#include <SoftwareSerial.h>
#include <TinyGPS.h> 
#include <String.h>
#include <DHT.h> 
#define DHTPIN A0
DHT dht(DHTPIN, DHT11);
SoftwareSerial gpsSerial(3,4);//rx,tx for GPS
SoftwareSerial gprsSerial(9,10); //rx,tx for sim900a
TinyGPS gps; // create gps object
float flat, flon;
unsigned long age;

void setup()
{
  dht.begin();
  delay(100);
  Serial.begin(9600);    // the GPRS baud rate
  delay(100); 
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // Start each software serial port
  gpsSerial.begin(9600); // connect gps sensor
  delay(100);
  gprsSerial.begin(9600);  // connect to GPRS baud rate 
  delay(100);
  
}
 
void loop()
{
  float h = dht.readHumidity(); float t = dht.readTemperature(); delay(100);   
  Serial.print("Temperature = "); Serial.print(t); Serial.println(" °C");
  Serial.print("Humidity = "); Serial.print(h); Serial.println(" %"); 
  delay(100);   
  
  gpsSerial.listen();  // connect to GPRS baud rate 
  delay(100);  
  //Serial.println("Start GPS ");
  bool newdata = false;
  unsigned long start = millis();
  // Every 1 seconds we print an update
  while (millis() - start < 2000) 
  {
    if (gpsSerial.available())
    //Serial.println("GPS available "); 
    {
      char c = gpsSerial.read();
      if (gps.encode(c)) 
      {
        //Serial.println("GPS encode");
        newdata = true;
        break;  // uncomment to print new data immediately!
      }
    }
  }
  
  if (newdata) 
  {
    gps.f_get_position(&flat, &flon, &age);
    Serial.print(flat,6); Serial.print(", "); Serial.println(flon,6);
    newdata = false;
    delay(1000);
    gpsSerial.end();  // disconnect to GPS 
    delay(1000); 
  }
  
  gprsSerial.listen(); // connect gprs sensor
  delay(100);
  
  if (gprsSerial.available())
    Serial.write(gprsSerial.read());
 
  gprsSerial.println("AT");
  delay(1000);
 
  gprsSerial.println("AT+CPIN?");
  delay(1000);
 
  gprsSerial.println("AT+CREG?");
  delay(1000);
 
  gprsSerial.println("AT+CGATT?");
  delay(1000);
 
  gprsSerial.println("AT+CIPSHUT");
  delay(1000);
 
  gprsSerial.println("AT+CIPSTATUS");
  delay(2000);
 
  gprsSerial.println("AT+CIPMUX=0");
  delay(2000);
  ShowSerialData();
 
  gprsSerial.println("AT+CSTT=\"internet\"");//start task and setting the APN,
  delay(1000);
  ShowSerialData();
 
  gprsSerial.println("AT+CIICR");//bring up wireless connection
  delay(3000);
  ShowSerialData();
 
  gprsSerial.println("AT+CIFSR");//get local IP adress
  delay(2000);
  ShowSerialData();
 
  gprsSerial.println("AT+CIPSPRT=0");
  delay(3000);
  ShowSerialData();
  
  gprsSerial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection
  delay(6000);
  ShowSerialData();
 
  gprsSerial.println("AT+CIPSEND");//begin send data to remote server
  delay(4000);
  ShowSerialData();
  
  String str="GET https://api.thingspeak.com/update?api_key=9SM1FF97XUGCCOI0&field1="+String(t)+"&field2="+String(h) +"&field3="+String(flat, 6) +"&field4="+String(flon, 6);
  Serial.println(str);
  gprsSerial.println(str);//begin send data to remote server
  delay(4000);
  ShowSerialData();
 
  gprsSerial.println((char)26);//sending
  delay(5000);//waitting for reply, important! the time is base on the condition of internet 
  gprsSerial.println();
  ShowSerialData();
 
  gprsSerial.println("AT+CIPSHUT");//close the connection
  delay(100);
  ShowSerialData();
  
  gprsSerial.end();  // disconnect to GPRS
  delay(1000);

 delay(780000);  
} 

void ShowSerialData()
{
  while(gprsSerial.available()!=0)
  Serial.write(gprsSerial.read());
  delay(5000); 
}
