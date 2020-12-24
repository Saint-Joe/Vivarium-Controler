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
* Create a Sensor Pack for monitiorin temperature and humidity 
	* Publish MQTT Brocker, monitor MQTT and enable 4 pins on the esp board 
	* Use MQTT to control 4 pins
#### Sensor Pack Hardware & Supplies
* [NodeMcu Mini D1](https://www.amazon.com/gp/product/B081PX9YFV/)
* [HTU21D Temperature Humidity Sensor Breakout Module](https://www.amazon.com/dp/B07VRNW2X6/)
* [30 awg Silicone Electrical Wire](https://www.amazon.com/gp/product/B07G7R9BVJ/)
* [Braided Sleeving](https://www.amazon.com/gp/product/B071JH14WZ/)
* [LulzBot PolyLite PLA](https://www.amazon.com/gp/product/B01IAVQI2E/) for the cases

## ToDo:
### Misting controller
* Build a esp8266/relay to control main power for missting pump
* Use [Solenoid valves](https://www.mistking.com/Solenoid-Valve-NC.html) to control which habatat is misted
* Error checking to limit over misting
* Modify "Sensor Pack" to monitor reservoir float valve
  
### Light Controller
* Build esp8266/relay to control 24v T5 LED light strips
	* [24v T5 LED light strips](https://www.amazon.com/gp/product/B088CZNWHQ)
	* [LED Driver 200 Watts 24V](https://www.amazon.com/gp/product/B07MZV7148/ref=ox_sc_saved_title_4?smid=A2S7ZIGVI25YZ9&psc=1)
	* [Relay](https://www.amazon.com/ELEGOO-Channel-Optocoupler-Arduino-Raspberry/dp/B01HCFJC0Y/)

### Habitat display
* Use esp8266 and oled to display habitat data as is streams throgh the MQTT broker

### Sensor Pack
* Error checking code, need some code to deal with bad data from the sensor (large data swings)
