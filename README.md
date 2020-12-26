# Vivarium Controller
Use common inexpensive open-source hardware and software to control and the environment in a Vivarium, Terrarium, or Palladium.
* esp8266 and for sensors and controllers
* MQTT messaging protocol for communication
* RaspberryPi running Home Assistant w/ MQTT Broker and Node-Red for automation
* Adafruit IO for anywhere access to environment data

## Goals:
1. Simple solution for monitoring temperature and humidity in a Vivarium, Terrarium, or Palladium
1. Use automation to control (Home Assistant, Adafruit IO, etc)
	1. Lights
	1. Misting system
	1. Fans
  
## Completed:
### Sensor Pack
* Create a Sensor Pack for monitoring temperature and humidity 
	* Publish MQTT Broker, monitor MQTT and enable 4 pins on the esp board 
	* Use MQTT to control 4 pins
	* basic logic to discard one bad reading from the HTU21D sensor
#### Sensor Pack Hardware & Supplies
* [NodeMcu Mini D1](https://www.amazon.com/gp/product/B081PX9YFV/)
* [HTU21D Temperature Humidity Sensor Breakout Module](https://www.amazon.com/dp/B07VRNW2X6/)
* [30 awg Silicone Electrical Wire](https://www.amazon.com/gp/product/B07G7R9BVJ/)
* [Braided Sleeving](https://www.amazon.com/gp/product/B071JH14WZ/)
* [LulzBot PolyLite PLA](https://www.amazon.com/gp/product/B01IAVQI2E/) for the cases

## Wish List/ToDo:
### Misting Controller
* Build a esp8266/relay to control main power for misting pump
* Use [Solenoid valves](https://www.mistking.com/Solenoid-Valve-NC.html) to control which habitat is misted
* Error checking to limit over misting
* Modify "Sensor Pack" to monitor reservoir float valve
* limit "on" time (x sec in y hours), this will prevent bad data from drowning the habitat
  
### Light Controller
* Build esp8266/relay to control 24v T5 LED light strips
	* [24v T5 LED light strips](https://www.amazon.com/gp/product/B088CZNWHQ)
	* [LED Driver 200 Watts 24V](https://www.amazon.com/gp/product/B07MZV7148)
	* [Relay](https://www.amazon.com/dp/B01HCFJC0Y/)

### Habitat Display
* Use esp8266 and oled to display habitat data as is streams through the MQTT broker

### Web Server
Create a simple web interface for the Sensor Pack that would alow you to make changes to the sensors; these changes would be saved to the eeprom and loaded on reboot
* Change WiFi SSID and Secret 
* Change MQTT Server, user, or password
* Change the Sensor Pack's device ID
* Modify the Temperature and Humidity offset values

### Fan Pack
Use an esp8266 and a 5v fan to control humidity in the habitat
* 3d printed fan holder that will sit in one of my 60mm vents
* Logic to monitor humitity and enable fan to maintain desired humitity levels
	* will need to work with the mising controler
* Maybe a [pwm](https://github.com/faeibson/ESPPWMFanControl) fan? 
	* requires a 12v power supply; but that could be regulated to control the 3.3v controler
	

	
