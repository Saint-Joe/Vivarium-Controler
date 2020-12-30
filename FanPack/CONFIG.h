/************************* Application Info **********************************/

#define APP_NAME        "PWM FAN Controller"
#define CODE_VER        "00.01"

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "...ssid..."
#define WLAN_PASS       "...pass..."

/***************************** MQTT Setup ************************************/

#define MQTT_SERVER      "192.168.1.1"          //MQTT Broker host name (mqtt.domain.com) or ip
#define MQTT_SERVERPORT  1883                   // use 8883 for SSL
#define MQTT_USERNAME    "...mqttuser..."
#define MQTT_PASSWORD    "...mqttpass..."
#define UNIQUE_ID        "...fanTEST..."              //user defined id, useful if you have many sensor packs
