# Vivarium-Controler

## Goals:
1. Simple solution for monitoring temperature and humidity in a Vivarium, Terrarium, or Palladium
1. Use automation to control (Home Assistant, Adafruit IO, etc)
	1. Lights
	1. Misting system
	1. Fans
  
## Completed:

### Sensor Pack
  Publish temperature and humidity to MQTT Brocker, monitor MQTT and enable 4 pins on the esp board 

## Todo:
### Misting controller
* Build a esp8266/relay to control main power for missting pump
* Use Solenoid valves(https://www.mistking.com/Solenoid-Valve-NC.html) to control which habatat is misted
* Error checking to limit over misting
* Modify "Sensor Pack" to monitor reservoir float valve
  
### Light controler
* Build esp8266/relay to control 24v T5 LED light strips (https://www.amazon.com/gp/product/B088CZNWHQ)
  
### Habitat display
* Use esp8266 and oled to display habitat data as is streams throgh the MQTT broker
  
