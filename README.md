# Santa's Fire Detector
### Final Project for Introduction to Embedded Systems
### Team Members: Shane Price, Kieran O'Connor, Nicholas Klein

## Description
Santa's Fire Detector is a warning system designed to alert users of a fire via wifi and to sound an alarm if someone is near.
Users are alerted using the MQTT DASH phone app by subscribing to the topic "Fire". They must be connected to the same wifi as
Santa's Fire Detector to recieve the notification. The name comes from the idea that the alarm would sound if Santa came down
the chimney while the fire is lit.

## Wifi communication with MQTT DASH
Users can download the MQTT DASH app for their phone and subscribe to the topic "Fire", this will send the message "Fire" to
their phone when a fire is detected.

## Circuit Design
### MSP-EXP430F5529LP Microcontroller
The microcontroller is the main processing unit of Santa's Fire Detector, using the input of the sensors to change the state of
the actuators in real time. The microcontroller detects input from sensors on a low signal.

### ESP-01 Serial Wifi Module
The ESP-01, using the ESP8266 microchip, is connected to the microcontroller via serial UART connection sends commuications from Santa's Fire Detector to the
user's phone app in the case of a fire.

### Arduino Flame Detector
The flame detector senses wavelengths of light from 760nm to 1100nm which covers the visible spectrum of flame, sending a signal
to the microprocessor when those wavelengths are present.

### Mini PIR Motion Detector
Detects motion within a 7 meter range, sending a signal to the microprocessor while objects remain in motion.

### Buzzer Alarm
In the event that there is a fire and motion near Santa's Fire Detector, the microcontroller sends a PWM signal with an 80% duty
cycle to the alarm which makes an audible alarm sound.

### Circuit Diagram
<img src="https://github.com/RU09342-F18/intro-to-embedded-final-project-russells-muscle/blob/master/FinalProjSchem.PNG" height="500" width="665">

## Parts List
• 1 MSP-EXP430F5529LP Microcontroller

• 1 ESP-01 Seral Wifi Module (ESP8266 microchip)
  
• 1 Arduino Flame Sensor

• 1 Mini PIR Motion Sensor

• 2 100Ω resistor
  
• 1 150Ω Resistor
  
• 1 10kΩ Resistor
  
• 2 2N7000G MOSFET

• 2 Buttons

• 1 Solderless Breadboard
