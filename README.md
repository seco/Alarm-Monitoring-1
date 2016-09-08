### Security System Alarm Monitoring with ESP8266 and MQTT

This is a hardware and software interface to monitor a GE NetworX home security system using a ESP8266 ESP-07 WiFi Module and a Raspberry Pi. The system sends a text and/or email in the event of any one of four alarm events.

The monitoring system requires Mosquitto MQTT and Node-Red software installed on a Raspberry Pi.

The ESP-07 software is written in two versions: Arduino IDE code and NodeMCU Lua code.

Alarm events are handled by publishing MQTT messages to Node-Red on a Raspberry Pi. Node-Red then sends texts and/or emails, depending on the event. In addition, Node-Red can send MQTT On/Off messages to the ESP-07 to blink the green LED, providing visual indication the system is active.

##### Arduino IDE Code Notes:
The Arduino IDE can be used to program the ESP8266, using the provided code.

To upload code for the ESP-07, select the board "Generic ESP8266 Module" and leave all board settings as default. Select the proper port for your USB/Serial adapter. Press and hold the "Flash" pushbutton, then press and release the "Reset" pushbutton,  then release the "Flash" button. Finally, click the Upload button.

##### Lua Code Notes:
After installing the NodeMCU firmware, the ESP-07 needs to be set to connect to your wireless router. Once set, it remembers the connection. Setting can be done with a short Lua script or the LuaLoader program. Using a terminal program connected to the ESP-07, set the module to STATION mode and set the Access Point SSID and Password with the following Lua commands:
```
wifi.sta.getip()
wifi.setmode(wifi.STATION)
wifi.sta.config("SSID","password")
print(wifi.sta.getip())
```
Or, using LuaLoader, fill-in your SSID and password, then press "Set AP". Next, check that you are connected with "Get IP".

##### NX8V2 Alarm System Notes:
The NX8V2 system parameters (locations) related to the ESP8266 monitoring are:
```
45 AUXILIARY OUTPUT 1-4 PARTITION SELECTION
	SEG# 1	12345678	AUX 1 Partitions
	SEG# 2	12345678	AUX 2 Partitions
	SEG# 3	12345678	AUX 3 Partitions
	SEG# 4	12345678	AUX 4 Partitions
	
46 AUXILIARY OUTPUT 1 - 4 SPECIAL TIMING  
	SEG# 1	--------	Output continues timing upon code entry
	SEG# 2	--------	Output continues timing upon code entry
	SEG# 3	--------	Output continues timing upon code entry
	SEG# 4	--------	Output continues timing upon code entry
	
47 AUXILIARY OUTPUT 1, EVENT & TIME
	SEG# 1	0	Burglary Alarm
	SEG# 2	1	Event timing set to 1 second
	
48 AUXILIARY OUTPUT 2, EVENT & TIME
	SEG# 1	1	Fire Alarm
	SEG# 2	1	Event timing set to 1 second
	
49 AUXILIARY OUTPUT 3, EVENT & TIME
	SEG# 1	21	Armed State
	SEG# 2	1	Event timing set to 1 second
	
50 AUXILIARY OUTPUT 4, EVENT & TIME
	SEG# 1	22	Disarmed State
	SEG# 2	1	Event timing set to 1 second
```
