/***************************************************

  MIT license

  Permission is hereby granted, free of charge, to any person obtaining a copy of this 
  software and associated documentation files (the "Software"), to deal in the Software 
  without restriction, including without limitation the rights to use, copy, modify, merge,
  publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
  to whom the Software is furnished to do so, subject to the following conditions:

  - Any use or redistubuted of this software, in whole or in part must retain the MIT License
  - Contributers must be attributed in this header
  - MIT license, all text in this header will be included in any redistribution  

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
  PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE 
  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
  OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
  DEALINGS IN THE SOFTWARE.

Contributor:
- SensorPack V01.01: Keith Talbot
- Adafruit MQTT : Tony DiCola for Adafruit Industries.
- Adafruit HTU21D-F : Adafruit Industries

 ****************************************************/
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "...your ssid..."
#define WLAN_PASS       "...secret key..."


/************************* MQTT Setup *********************************/

#define AIO_SERVER      "XXX.XXX.XXX.XXX"       //MQTT Broker host name (mqtt.domain.com) or ip
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "...mqttuser..."
#define AIO_KEY         "...mqttpass..."
#define AIO_SENSOR      "id001"

//*This has beeen modified to work with a local MQTT server, AIO_SENSOR is not needed for Adafruit IO


/************************* i2c Setup *********************************/
#include <Wire.h>
#include "Adafruit_HTU21DF.h" // **for IO and temp/humid
float f_tempf = 0.0;
float f_temp = 0.0;
float f_hum = 0.0;
float temp_adjust = 0.0;                  // celsius  e.g -2 or 2
float hum_adjust = 0.0;                   // e.g. +20, -20

Adafruit_HTU21DF htu = Adafruit_HTU21DF(); // **for IO and temp/humid


/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiClientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Setup a feeds for publishing to changes.
Adafruit_MQTT_Publish tempf = Adafruit_MQTT_Publish(&mqtt, AIO_SENSOR "/feed/tempf");
Adafruit_MQTT_Publish humid = Adafruit_MQTT_Publish(&mqtt, AIO_SENSOR "/feed/humid");
Adafruit_MQTT_Publish pubonoffled = Adafruit_MQTT_Publish(&mqtt, AIO_SENSOR "/read/onoff");
Adafruit_MQTT_Publish pubpin12 = Adafruit_MQTT_Publish(&mqtt, AIO_SENSOR "/read/pin12");
Adafruit_MQTT_Publish pubpin13 = Adafruit_MQTT_Publish(&mqtt, AIO_SENSOR "/read/pin13");
Adafruit_MQTT_Publish pubpin14 = Adafruit_MQTT_Publish(&mqtt, AIO_SENSOR "/read/pin14");
Adafruit_MQTT_Publish pubpin15 = Adafruit_MQTT_Publish(&mqtt, AIO_SENSOR "/read/pin15");

// Setup a feed for subscribing to changes.
Adafruit_MQTT_Subscribe onoffled = Adafruit_MQTT_Subscribe(&mqtt, AIO_SENSOR "/read/onoff");
Adafruit_MQTT_Subscribe pin12 = Adafruit_MQTT_Subscribe(&mqtt, AIO_SENSOR "/read/pin12");
Adafruit_MQTT_Subscribe pin13 = Adafruit_MQTT_Subscribe(&mqtt, AIO_SENSOR "/read/pin13");
Adafruit_MQTT_Subscribe pin14 = Adafruit_MQTT_Subscribe(&mqtt, AIO_SENSOR "/read/pin14");
Adafruit_MQTT_Subscribe pin15 = Adafruit_MQTT_Subscribe(&mqtt, AIO_SENSOR "/read/pin15");

//*This has beeen modified to work with a local MQTT server, feed strings would need to be adjusted to work with AIO


/*****************************************************************************/
/********************************* Setup *************************************/
/*****************************************************************************/

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);  //enable ESP8266 built in led
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);  
  pinMode(14, OUTPUT);  
  pinMode(15, OUTPUT);  
  
  digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH 
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  digitalWrite(14, LOW);
  digitalWrite(15, LOW);
  
  Serial.begin(115200);
  delay(1500);

  Serial.println(F("Starting Sensor Pack"));

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
  delay(1500);

  // Setup callbacks
  onoffled.setCallback(onoffcallback);
  pin12.setCallback(pin12callback);
  pin13.setCallback(pin13callback);
  pin14.setCallback(pin14callback);
  pin15.setCallback(pin15callback);

  // Setup MQTT subscription
  mqtt.subscribe(&onoffled);
  mqtt.subscribe(&pin12);
  mqtt.subscribe(&pin13);
  mqtt.subscribe(&pin14);
  mqtt.subscribe(&pin15);

  // Setup temperature and humidity sensor
  if (!htu.begin()) {   // **for IO and temp/humid
    Serial.println("Couldn't find sensor!");
    while (1);
  }

  MQTT_connect();  // Connect to the MQTT Broker

  // Publish pin states
  pubonoffled.publish(0);
  pubpin12.publish(0);
  pubpin13.publish(0);
  pubpin14.publish(0);
  pubpin15.publish(0);
  
}

/*****************************************************************************/
/******************************* void loop ***********************************/
/*****************************************************************************/

void loop() {

  MQTT_connect();                               //Make sure we are connected 
  mqttPublish();                                //Post sensor data to the MQTT Broker 
  mqtt.processPackets(10000);                   //Listen for MQTT Subscriptions(time)
  if(! mqtt.ping()) {mqtt.disconnect();}        // ping MQTT Broker to keep alive

}


/*****************************************************************************/
/***************************** MQTT Broker ***********************************/
/*****************************************************************************/

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

/*****************************************************************************/
/***************************** MQTT Publish **********************************/
/*****************************************************************************/

void mqttPublish() {

// get temperature and humidity
  float newTemp = htu.readTemperature();             //get temp
    newTemp = newTemp + temp_adjust;                 //if needed we can adjust the temp(calabrate)
    f_tempf = ((newTemp * 1.8) + 32);                //Convert celsius to fahrenheit
    delay(100);
    
  float newHum = htu.readHumidity();                 //get humidity
    newHum = newHum + hum_adjust;                    //calabrate

  Serial.print(F("\nSending Temperature "));
  Serial.print(f_tempf);                             //or (newTemp)
  Serial.print("...");
                                                    //Publish temp to MQTT 
  if (! tempf.publish(f_tempf)) {                   //or (newTemp)
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  Serial.print(F("Sending Humidity "));
  Serial.print(newHum);
  Serial.print("...");

  if (! humid.publish(newHum)) {                    //Publish humidity to MQTT   
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }
}

/*****************************************************************************/
/****************************** Call Backs ***********************************/
/*****************************************************************************/

void onoffcallback(char *data, uint16_t len) {
  Serial.print("Hey we're in a onoff callback, the value is: ");
  Serial.println(data);
String str_data = data;
      if (str_data  == "1"){
        digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
        }
      if (str_data  == "0"){
        digitalWrite(BUILTIN_LED, HIGH);   // Turn LED off 
        }
}


void pin12callback(char *data, uint16_t len) {
  Serial.print("PIN12 callback, the value is: ");
  Serial.println(data);
String str_data = data;
      if (str_data  == "1"){
        digitalWrite(12, HIGH);
        }
      if (str_data  == "0"){
        digitalWrite(12, LOW);
        }
}


void pin13callback(char *data, uint16_t len) {
  Serial.print("PIN13 callback, the value is: ");
  Serial.println(data);
String str_data = data;
      if (str_data  == "1"){
        digitalWrite(13, HIGH);
        }
      if (str_data  == "0"){
        digitalWrite(13, LOW);
        }
}


void pin14callback(char *data, uint16_t len) {
  Serial.print("PIN14 callback, the value is: ");
  Serial.println(data);
String str_data = data;
      if (str_data  == "1"){
        digitalWrite(14, HIGH);
        }
      if (str_data  == "0"){
        digitalWrite(14, LOW);
        }
}


void pin15callback(char *data, uint16_t len) {
  Serial.print("PIN15 callback, the value is: ");
  Serial.println(data);
String str_data = data;
      if (str_data  == "1"){
        digitalWrite(15, HIGH);
        }
      if (str_data  == "0"){
        digitalWrite(15, LOW);
        }
}
