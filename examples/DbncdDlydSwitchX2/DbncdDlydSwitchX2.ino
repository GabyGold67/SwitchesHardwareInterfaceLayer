#include <Arduino.h>
#include <switchHIL.h>

/*
  SwitchHIL Library for
    Framework: Arduino
    Platform: ESP32

  DbncdDlydSwitchX2.ino
  Created by Gabriel D. Goldman, August, 2023.
  Updated by Gabriel D. Goldman, August 28, 2023.
  Released into the public domain in accordance with "GPL-3.0-or-later" license terms.

  Example file to demonstrate DbncdDlydSwitch and StrcsTmrSwitch classes, required hardware and connections:
  _ 1 push button between GND and ddmpbSwitchPin00
  _ 1 push button between GND and ddmpbSwitchPin01
  _ 1 led with it's corresponding resistor between GND and ddmpbLoadPin00
  _ 1 led with it's corresponding resistor between GND and ddmpbLoadPin01
*/

// put Global declarations here: 
const uint8_t ddmpbSwitchPin00{GPIO_NUM_26};
DbncdDlydMPBttn ddBttn00 (ddmpbSwitchPin00, true, true, 0, 50);

const uint8_t ddmpbLoadPin00{GPIO_NUM_19};
DbncdDlydSwitch myDbncDlydSwitch00(ddBttn00, ddmpbLoadPin00);

const uint8_t ddmpbSwitchPin01{GPIO_NUM_25};
DbncdDlydMPBttn ddBttn01 (ddmpbSwitchPin01, true, true, 0, 250);

const uint8_t ddmpbLoadPin01{GPIO_NUM_21};
DbncdDlydSwitch myDbncDlydSwitch01(ddBttn01, ddmpbLoadPin01);

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  //Now unneeded as all runs as independent tasks! Delete the loop() task
  vTaskDelete(nullptr);
}
