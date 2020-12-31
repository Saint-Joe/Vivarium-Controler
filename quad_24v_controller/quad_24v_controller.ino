/*
 * Hardware
 *  esp8266 board
 *  2n2222 transitor 
 *  12v 4 wire fan
 *  12v power supply
 *  
 * Setup 
 *  Fan positive and ground connected to 12v power supply
 *  Ground from power supply conneted to esp ground
 *  Pin D8 (GPIO15) connected to the fans PWM
 * 
 * Adjust fanSpead between 1 and 100
 * 
 */
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "CONFIG.h"

WiFiClient client;


/*****************************MQTT  Feeds ************************************/

Adafruit_MQTT_Client mqtt(&client,MQTT_SERVER, MQTT_SERVERPORT, MQTT_USERNAME, MQTT_PASSWORD);

// Setup a feeds for publishing to the MQTT Server
Adafruit_MQTT_Publish statusmessage = Adafruit_MQTT_Publish(&mqtt, UNIQUE_ID "/feed/ello");

// Setup subscriptions to pull from the MQTT Server
Adafruit_MQTT_Subscribe QuadLevelOne = Adafruit_MQTT_Subscribe(&mqtt, UNIQUE_ID "/feed/levelone");
Adafruit_MQTT_Subscribe QuadLevelTwo = Adafruit_MQTT_Subscribe(&mqtt, UNIQUE_ID "/feed/leveltwo");
Adafruit_MQTT_Subscribe QuadLevelThree = Adafruit_MQTT_Subscribe(&mqtt, UNIQUE_ID "/feed/levelthree");
Adafruit_MQTT_Subscribe QuadLevelFour = Adafruit_MQTT_Subscribe(&mqtt, UNIQUE_ID "/feed/levelfour");
Adafruit_MQTT_Subscribe onoffled = Adafruit_MQTT_Subscribe(&mqtt, UNIQUE_ID "/read/onoff");


#define BASEone 0                                       //PWM
#define BASEtwo 3
#define BASEthree 13
#define BASEfour 16

int i;                                               // Counter


void setup() {
  delay(2000);
  Serial.begin(115200);
  delay(2000);
    Serial.println("Hello World!");  

  connectWifi();                                       

  pinMode(BASEone, OUTPUT);
  pinMode(BASEtwo, OUTPUT);
  pinMode(BASEthree, OUTPUT);
  pinMode(BASEfour, OUTPUT);
  analogWriteRange(100);                               // range 1 - 100 for the fan
  analogWriteFreq(500); 

  //setup mqtt callbacks
  QuadLevelOne.setCallback(oneCallback);
  QuadLevelTwo.setCallback(twoCallback);
  QuadLevelThree.setCallback(threeCallback);
  QuadLevelFour.setCallback(fourCallback);
  onoffled.setCallback(onoffcallback);
  // Setup MQTT subscription
  mqtt.subscribe(&QuadLevelOne);
  mqtt.subscribe(&QuadLevelTwo);
  mqtt.subscribe(&QuadLevelThree);
  mqtt.subscribe(&QuadLevelFour);
  mqtt.subscribe(&onoffled);


  MQTT_connect();  // Connect to the MQTT Broker

  statusmessage.publish("Alive!");

  
}

void loop() {
  
  MQTT_connect();                               //Make sure we are connected 
  mqtt.processPackets(10000);                   //Listen for MQTT Subscriptions(time)
  if(! mqtt.ping()) {mqtt.disconnect();}        // ping MQTT Broker to keep alive

}



/*****************************************************************************/
/****************************** Call Backs ***********************************/
/*****************************************************************************/



void onoffcallback(char *data, uint16_t len) {
  Serial.print("Hey we're in a onoff callback, the value is: ");
  Serial.println(data);
}

void oneCallback(char *data, uint16_t len) {
  Serial.print("oneCallback, the value is: ");
  Serial.println(data);
  int onePWMrate = atoi(data);
  analogWrite(BASEone, onePWMrate);
}

void twoCallback(char *data, uint16_t len) {
  Serial.print("twoCallback, the value is: ");
  Serial.println(data);
    int twoPWMrate = atoi(data);
    analogWrite(BASEtwo, twoPWMrate);
}

void threeCallback(char *data, uint16_t len) {
  Serial.print("threeCallback, the value is: ");
  Serial.println(data);
    int threePWMrate = atoi(data);
    analogWrite(BASEthree, threePWMrate);
}

void fourCallback(char *data, uint16_t len) {
  Serial.print("fourCallback, the value is: ");
  Serial.println(data);
    int fourPWMrate = atoi(data);
    analogWrite(BASEfour, fourPWMrate);
}

/*****************************************************************************/
/**************************** WiFi Service ***********************************/
/*****************************************************************************/


void connectWifi(){

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



