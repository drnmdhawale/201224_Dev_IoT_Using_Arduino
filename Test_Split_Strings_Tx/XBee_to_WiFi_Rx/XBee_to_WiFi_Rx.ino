// This Program is developed to aquire sensor data from DHT22 Sensor and trasnmit to the cloud
// over wireless serial communication link
// Created by Dr. Nandkishor Motiram Dhawale and Kiran Ashok Shinde
// Date of Creation Dec 2020
// Project Name : to transmit data on the cloud via esp8266
// Recreated and Implemented by :- Kiran Shinde and Rohan Kulkarni. For PVPIT Budhgaon, I/C - R&D Cell
// Under Guidence of Dr.N.M.Dhawale, I/c- R&D Cell, P.V.P.I.T Budhgaon, Sangli, India in Dec 2020 

/***************************************************
  Adafruit MQTT Library ESP8266 Example

  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino

  Works great with Adafruit's Huzzah ESP board & Feather
  ----> https://www.adafruit.com/product/2471
  ----> https://www.adafruit.com/products/2821

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Tony DiCola for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "DHT.h"

/************************* WiFi Access Point *********************************/

//#define WLAN_SSID       "iBall-Baton"
//#define WLAN_PASS       "1234@5678"
#define WLAN_SSID       "ROOM_263"
#define WLAN_PASS       "ROOM_263"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "Adafruit_project"
#define AIO_KEY         "aio_HmsJ08harhe7yr2Kq0FwFea59YZI"

/************* DHT11 Setup ********************************************/
//#define DHTPIN D6
//#define DHTTYPE DHT11
//DHT dht(DHTPIN, DHTTYPE);
String xyz; //main captured String
String ID; //data String
String Humidity; //data String
String Temperature; //data String

int ind1; // , locations
int ind2;
int ind3;

 
 /************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
//Store the MQTT server, username, and password in flash memory.
//This is required for using the Adafruit MQTT library.
const char MQTT_server[] PROGMEM   = AIO_SERVER;
const char MQTT_USERNAME[] PROGMEM = AIO_USERNAME;
const char MQTT_PASSWORD[] PROGMEM = AIO_KEY;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish photocell = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/photocell");

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temp");
Adafruit_MQTT_Publish humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/hum");

// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/onoff");
Adafruit_MQTT_Subscribe slider = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/slider");
/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

void setup() {
  Serial.begin(115200);
  //dht.begin();
  delay(100);

  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&onoffbutton);
   mqtt.subscribe(&slider);
}

uint32_t x=0;

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &onoffbutton) {
      Serial.print(F("Got: "));
      Serial.println((char *)onoffbutton.lastread);
    }

     if (subscription == &slider) {
      Serial.print(F("slider: "));
      Serial.println((char *)slider.lastread);
    }  
  }

  // Now we can publish stuff!
  Serial.print(F("\nSending photocell val "));
  Serial.print(x);
  Serial.print("...");
  if (! photocell.publish(x++)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  */

  delay(5);
  if (Serial.available())  {

      xyz=""; //clears variable for new input
      ID="";
      Humidity="";
      Temperature="";
      
      xyz = Serial.readString(); delay(5);
      Serial.print("captured String is : "); delay(1); 
      Serial.println(xyz); delay(1); //prints string to serial port out
      
     
      ind1 = xyz.indexOf(',');  //finds location of first ,
      ID = xyz.substring(0, ind1);   //captures first data String
      ind2 = xyz.indexOf(',', ind1+1 );   //finds location of second ,
      Humidity = xyz.substring(ind1+1, ind2+1);   //captures second data String
      ind3 = xyz.indexOf(',', ind2+1 );
      Temperature = xyz.substring(ind2+1, ind3+1);
    
      Serial.print("ID = "); delay(1);
      Serial.println(ID); delay(1);
      Serial.print("Humidity = "); delay(1);
      Serial.println(Humidity); delay(1);
      Serial.print("Temperature = "); delay(1);
      Serial.println(Temperature); delay(1);
 }

  humidity.publish(Humidity.toFloat()); delay(1);
  temperature.publish(Temperature.toFloat()); delay(1);
  //routerid1.publish(ID.toInt());
  delay(25000);
  //delay(900000);
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
