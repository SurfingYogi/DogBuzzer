# DogBuzzer
Remotely activated Ultrasonic buzzer for barking dogs using Arduino Uno WiFi Rev2

The Arduino serves a web page that allows you to turn on a ultrasonic buzzer for 1-5 seconds, or you can push a button to turn on the buzzer.
The sound is at 25KHz, which is a frequency that dogs can hear, but humans cannot hear.

## Hardware Requirements

Required hardware
- Arduino Uno WiFi Rev2
- Custom circuit board containing the following:
  - Low Side Driver IC: TC4420CPA
  - Ultrasonic Transducer (25KHz): CUSA-T80-12-2600-TH
  - Momentary push button
- 12V (1 Amp) power supply with barrel connector to power Arduino
- WiFi network with a known SSID and Password
- A barking dog

You need to make your own circuit board to plug onto the top of the Arduino.

The code uses the following IO pins on the Arduino
- Buzzer Pin: D7
- Momentary Button pin: A2

## Setup
1. Rename "secrets.example.h" to "secrets.h" and change the SSID and Password to match your wifi network.
2. Change the Buzzer pin and button pin to match your hardware in the Arduino code. 
3. Build and install the code on your Arduino Uno WiFi Rev2.
4. Use the Arduino IDE Serial Monitor to get the Arduino's IP address (i.e. "192.168.1.100"), when it powers On
5. Open a web browser on a device on the wifi network and goto "http://192.168.1.100"
  <img width="546" height="1000" alt="image" src="https://github.com/user-attachments/assets/104f374b-d4c9-4529-a426-77654d274fcd" />

6. Select a time and click on the button, and the Arduino built-in LED should light and the buzzer will sound.
7. Alternatively, the LED/buzzer will sound when the button is pressed.

## Warning
Be nice to dogs. Humans can't hear sound at 25KHz but dogs can and they have really sensitive hearing. You won't be able to hear any sound coming from the transducer, but its there. 
