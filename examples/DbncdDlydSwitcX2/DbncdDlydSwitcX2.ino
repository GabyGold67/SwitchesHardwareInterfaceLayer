#include <Arduino.h>
#include <switchHIL.h>

/*
  mpbToSwitch Library for
    Framework: Arduino
    Platform: ESP32

  DbncdDlydSwitchX2.ino
  Created by Gabriel D. Goldman, August, 2023.
  Updated by Gabriel D. Goldman, September, 2023.
  Released into the public domain in accordance with "GPL-3.0-or-later" license terms.

  Example file to demonstrate DbncdDlydSwitch class, required hardware and connections:
  _ 1 push button between GND and ddmpbSwitch0Pin
  _ 1 push button between GND and ddmpbSwitch1Pin
  _ 1 led with it's corresponding resistor between GND and ddmpb0LoadPin
  _ 1 led with it's corresponding resistor between GND and ddmpb1LoadPin
*/

// put function declarations here:

// put Global declarations here: 
const uint8_t ddmpbSwitch1Pin{GPIO_NUM_25};
const uint8_t ddmpb1LoadPin{GPIO_NUM_21};

const uint8_t ddmpbSwitch0Pin{GPIO_NUM_26};
const uint8_t ddmpb0LoadPin{GPIO_NUM_19};

DbncdDlydMPBttn ddBttn0 (ddmpbSwitch0Pin, true, true, 0, 200);
DbncdDlydSwitch myDbncDlyd0Switch(ddBttn0, ddmpb0LoadPin);

DbncdDlydMPBttn ddBttn1 (ddmpbSwitch1Pin, true, true, 0, 50);
DbncdDlydSwitch myDbncDlydSwitch1(ddBttn1, ddmpb1LoadPin);

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  //Now unneeded as all runs as independent tasks! Delete the loop() task
  vTaskDelete(nullptr);

}
