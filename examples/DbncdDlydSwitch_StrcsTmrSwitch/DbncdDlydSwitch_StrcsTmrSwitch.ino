#include <Arduino.h>
#include <switchHIL.h>

/*
  mpbToSwitch Library for
    Framework: Arduino
    Platform: ESP32

  DbncdDlydSwitch_StrcsTmrSwitch.ino
  Created by Gabriel D. Goldman, August, 2023.
  Updated by Gabriel D. Goldman, September, 2023.
  Released into the public domain in accordance with "GPL-3.0-or-later" license terms.

  Example file to demonstrate DbncdDlydSwitch and StrcsTmrSwitch classes, required hardware and connections:
  _ 1 push button between GND and ddmpbSwitchPin
  _ 1 push button between GND and htlmpbSwitchPin
  _ 1 led with it's corresponding resistor between GND and ddmpbLoadPin
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

const uint8_t ddmpbSwitchPin{GPIO_NUM_26};
const uint8_t ddmpbLoadPin{GPIO_NUM_19};

HntdTmLtchMPBttn htlBttn (htlmpbSwitchPin, 4000, 33, true, true, 20, 50);
StrcsTmrSwitch myStairSwitch (htlBttn, htlmpbLoadPin, htlmpbWnngPin, htlmpbPilotPin);

DbncdDlydMPBttn ddBttn (ddmpbSwitchPin, true, true, 0, 200);
DbncdDlydSwitch myDbncDlydSwitch(ddBttn, ddmpbLoadPin);

void setup() {
  // put your setup code here, to run once:
  //Change some of the HntdTmLtchMPBttn attributes to show how simple it is
  myStairSwitch.getUnderlMPB()->setKeepPilot(true);
  myStairSwitch.getUnderlMPB()->setActTime(8000);
  myStairSwitch.setBlnkRate(250);
  myStairSwitch.setBlnkWrnng(true);
}

void loop() {
  //Now unneeded as all runs as independent tasks! Delete the loop() task
  vTaskDelete(nullptr);
}
