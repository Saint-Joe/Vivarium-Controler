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
Adafruit_MQTT_Subscribe mqttfanSpeed = Adafruit_MQTT_Subscribe(&mqtt, UNIQUE_ID "/feed/fanspeed");
Adafruit_MQTT_Subscribe onoffled = Adafruit_MQTT_Subscribe(&mqtt, UNIQUE_ID "/read/onoff");

int fanPin = 16;                                     // GPIO PINs for controlling the fan
int offPin = 14;
int i;                                               // Counter
int minFan = 15;                                     // min fan speed

void setup() {
  delay(2000);
  Serial.begin(115200);
  delay(2000);
  Serial.println("Hello World!");  

  connectWifi();                                       

  pinMode(fanPin, OUTPUT);                             // sets the pin as outputs:
  analogWriteRange(100);                               // range 1 - 100 for the fan
  analogWriteFreq(20000);                            
  pinMode(offPin, OUTPUT);

  //setup mqtt callbacks
  mqttfanSpeed.setCallback(speedCallback);
  onoffled.setCallback(onoffcallback);
  // Setup MQTT subscription
  mqtt.subscribe(&mqttfanSpeed);
  mqtt.subscribe(&onoffled);

  MQTT_connect();  // Connect to the MQTT Broker

  statusmessage.publish("Alive!");                   //mqtt hello
  controlFan(0);                                     //start with fan off  
}

void controlFan (int fanSpeed) {

  if (fanSpeed == 0){                                 // turn fan off if dutycycle is <
      
      analogWrite(fanPin, 0);
      delay(5000);
      digitalWrite(offPin, LOW);
                  Serial.println("Fan OFF");
      
  } else {
    
      if (fanSpeed < minFan){fanSpeed = minFan;}
      digitalWrite(offPin, HIGH);                   //enable fan
      delay(100);
      if (fanSpeed < 50){                           //spin up fan w/full power
        analogWrite(fanPin, 100);                   
        delay(1000);                                //wait befor going to the called speed
      }
      
      analogWrite(fanPin, fanSpeed);                   // set the fan speed
      
                           // turn fan on
                  Serial.print("Fan Speed: \t\t");
                  Serial.print(fanSpeed);
                  Serial.println("%");  
  }
}

void loop() {
  
  MQTT_connect();                               //Make sure we are connected 
  mqtt.processPackets(10000);                   //Listen for MQTT Subscriptions(time)
  if(! mqtt.ping()) {mqtt.disconnect();}        // ping MQTT Broker to keep alive
}



/*****************************************************************************/
/****************************** Call Backs ***********************************/
/*****************************************************************************/

void speedCallback(char *data, uint16_t len) {
  Serial.print("Message received from MQTT: ");
  Serial.println(data);
  String str_data = data;
  if (str_data == "0"){
      controlFan(0);
      Serial.println("Full Stop");
  } else {
  int speedInt = atoi(data);                            // convert char to int; returns 0 on error
    if (speedInt == 0){
        Serial.println("Bad Data");statusmessage.publish("Bad Data");
      } else {
        controlFan(speedInt);
      }
  }
}

void onoffcallback(char *data, uint16_t len) {
  Serial.print("Hey we're in a onoff callback, the value is: ");
  Serial.println(data);
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
