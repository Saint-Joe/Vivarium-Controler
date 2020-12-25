# Vivarium Controller HW Build

## Prerequisites 

*	MQTT Broker, local or public internet. MQTT is a lightweight messaging protocol that facilitates data transfer or commands to and from IoT devices.
  *	Consider running Home Assistant w/ Mosquitto broker 
*	Files from https://github.com/Saint-Joe/Vivarium-Controller
  *	3d Printer to print cases
  *	Sensor Pack software 
*	Basic understanding of electronics and open source software (aka patience)
*	Soldering Iron, 3D Printer, etc..


## Hardware Assembly
1. Gather the parts and supplies 
	1. 3d printed cases
	1. Circuit boards
	1. Wire and braided sleeve
	1. Soldering Iron and Solder
![get started](/images/parts.jpg)
1. Cut 14” of the Braided Sleeving or length as you desire
	1. Heat ends to prevent unraveling
	1. Thread 4 wires through sleeving, leave on the spool for now.
1. Put wire and sleeving through the hole on the HTU21D case
1. Strip 1/8 (3mm) from the end of the wire not attached to the spool
1. Solder the wires to the HTU21D. Wires should run out the back; this will prevent them from covering the sensor. Document the wire colors once this is done. 
1. From the wire spool, pull your wires back through the sleeving, make sure your sleeving stays in the case 
1. Use hot-glue to secure the HTU21D in the case; gluing the sleeving down to reduce strain on the wires. 
1. Use plastic cement to attach the HTU21D case lid.
1. With no tension on the wires in the braided sleeve, cut the wires just under 1 inch(20mm) from the end of the sleeve(we can pull the sleeve back to solder.
1. Run wires and sleeve into the Mini D1 case. The vents on the HTU21D case should be up once this step is complete. 
1. Solder the wires on to the Mini D1 (D1 is Clock and D2 is Data).
1. Install and test software. I use MQTT Explorer to see data; we are looking for the 5 pins and the temperature and humidity data
1. Hot-glue the Mini D1 into its case and then put the top on.
	1. Make sure that the sleeve is oriented so that the two cases will align back to back.
	1. Add some hot-glue for strain relief
1. Use some VHB tape to attach the sensor in your habitat and the controller to the back or side. 
1. Use a usb charger or usb battery pack to power your device.

