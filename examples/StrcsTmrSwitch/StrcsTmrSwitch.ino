#include <Arduino.h>
#include <switchHIL.h>

/*
  SwitchHIL Library for
    Framework: Arduino
    Platform: ESP32

  StrcsTmrSwitch.ino
  Created by Gabriel D. Goldman, August, 2023.
  Updated by Gabriel D. Goldman, September, 2023.
  Released into the public domain in accordance with "GPL-3.0-or-later" license terms.

  Example file to demonstrate DbncdDlydSwitch and StrcsTmrSwitch classes, required hardware and connections:
  _ 1 push button between GND and htlmpbSwitchPin
  _ 1 led with it's corresponding resistor between GND and htlmpbLoadPin
  _ 1 led with it's corresponding resistor between GND and htlmpbWnngPin
  _ 1 led with it's corresponding resistor between GND and htlmpbPilotPin
*/

// put function declarations here:

// put Global declarations here: 
const uint8_t htlmpbSwitchPin{GPIO_NUM_25};
const uint8_t htlmpbLoadPin{GPIO_NUM_21};
const uint8_t htlmpbWnngPin{GPIO_NUM_17};
const uint8_t htlmpbPilotPin{GPIO_NUM_18};

HntdTmLtchMPBttn htlBttn (htlmpbSwitchPin, 4000, 33, true, true, 20, 50);
StrcsTmrSwitch myStairSwitch (htlBttn, htlmpbLoadPin, htlmpbWnngPin, htlmpbPilotPin);

void setup() {
  //Change some of the HntdTmLtchMPBttn attributes to show how simple it is
  myStairSwitch.getUnderlMPB()->setKeepPilot(true);
  myStairSwitch.getUnderlMPB()->setActTime(6000);
  myStairSwitch.setBlnkRate(150);
  myStairSwitch.setBlnkWrnng(true);
}

void loop() {
  //Now unneeded as all runs as independent tasks! Delete the loop() task
  vTaskDelete(nullptr);
}
