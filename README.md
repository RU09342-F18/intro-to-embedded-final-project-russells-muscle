# Santa's Fire Detector
### Final Project for Introduction to Embedded Systems
### Team Members: Nicholas Klein, Kieran O'Connor, Shane Price

## Description
Santa's Fire Detector is a warning system designed to alert users of a fire via wifi and to sound an alarm if someone is near. Users are alerted using the MQTT DASH phone app by subscribing to the topic "Fire". They must be connected to the same wifi as Santa's Fire Detector to recieve the notification. The name comes from the idea that the alarm would sound if Santa came down the chimney while the fire is lit.

## Wifi communication with MQTT DASH
Users can download the MQTT DASH app for their phone and subscribe to the topic "Fire", this will send the message "Fire" to their phone when a fire is detected.

## Software
### main.c
This is the file for the MSP microcontroller. Using Code Composer Studio, this file is flashed to the board which will then process the inputs and outputs of Santa's Fire Detector. The code itself is broken into several functions.
• CUART() is the setup function for the serial UART connection that sends data to the ESP chip which in turn sends a message to the user's phone.
• CPINS() sets up the GPIO pins to the sensors as inputs and outputs. CPWM() configures the PWM that is sent to the buzzer alarm during a fire.
• ToggleRed(), ToggleGreen(), OFFRed(), and OFFGreen() perform their respective power functions to the named LED on the microcontroller. • CTopic() subscribes to the topic "Fire" which allows it to send messages to that topic.
• sendNotice() sends the message "Fire" over wifi to all subscribed users.

Only one interrupt is used, an overflow interrupt for Timer A, which polls constantly checking if there is motion, a fire, or both. In the event of motion, the LEDs flash. In the event of a fire, the message "Fire" is sent to all users. When both are true, both the LEDs and message are activated as well as the PWM signal sent to the buzzer which makes an audible alarm.

### esp8266code.ino
This is an arduino file, authored by Russell Trafford, which is flashed to the ESP-01 wifi module. This allows the ESP to communicate over the Rowan_IoT network

## Circuit Design
### MSP-EXP430F5529LP Microcontroller
The microcontroller is the main processing unit of Santa's Fire Detector, using the input of the sensors to change the state of the actuators in real time. The microcontroller detects input from sensors on a low signal.

### ESP-01 Serial Wifi Module
The ESP-01, using the ESP8266 microchip, is connected to the microcontroller via serial UART connection sends commuications from Santa's Fire Detector to the user's phone app in the case of a fire. The TX (P4.5) and RX (P4.4) pins of the microcontroller connect to  Pin 2 and Pin 7 on the microcontroller respectively. Pin 1 is tied to ground and Pins 4, 5, and 8 are tied to a 3.3V VCC. The other pins  remain Unconnected. When flashing the ESP, Pins 4 and 5 must be pulled to GND.

### Arduino Flame Detector
The flame detector senses wavelengths of light from 760nm to 1100nm which covers the visible spectrum of flame, sending a signal to the microprocessor when those wavelengths are present. Pin 2 is connected to GND, Pin 3 is connected to 3.3V VCC, and Pin 4 connects to the gate of an N-MOS which allows Pin 8.2 on the microcontroller to be pulled low. Pin 8.2 on the Microcontroller is connected to the drain of the N-MOS and the source is connected to GND.

### Mini PIR Motion Detector
Detects motion within a 7 meter range, sending a signal to the microprocessor while objects remain in motion. Pin 1 is connected to 3.3V VCC, Pin 3 is connected to GND, and Pin 2 connects to Pin 3.7 on the Microcontroller.

### Buzzer Alarm
In the event that there is a fire and motion near Santa's Fire Detector, the microcontroller sends a PWM signal with an 80% duty cycle to the alarm which makes an audible alarm sound. Pin 1 connects to 3.3V VCC, Pin 2 connects to the Drain of an N-MOS. The Source connects to GND, and the gate connects to a 150Ω resistor which in turn connects to a 10kΩ resistor and Pin 2.0 on the microcontoller. The 10kΩ connects to GND on its other side. This voltage splitter keeps the gate pulled low when Pin 2.0 is not pushing a high signal.

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
