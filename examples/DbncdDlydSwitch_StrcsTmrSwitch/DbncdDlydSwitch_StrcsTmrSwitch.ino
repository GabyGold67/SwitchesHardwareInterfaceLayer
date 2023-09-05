#include <Arduino.h>
#include <switchHIL.h>

/*
  mpbToSwitch Library for
    Framework: Arduino
    Platform: ESP32

  DbncdDlydSwitch_StrcsTmrSwitch.ino
  Created by Gabriel D. Goldman, August, 2023.
  Updated by Gabriel D. Goldman, August 28, 2023.
  Released into the public domain in accordance with "GPL-3.0-or-later" license terms.

  Example file to demonstrate DbncdMPBttn and TmLtchMPBttn classes, required hardware and connections:
  _ 1 push button between GND and dbncdSwitchPin
  _ 1 push button between GND and tlmpbSwitchPin
  _ 1 led with it's corresponding resistor between GND and dbncdLoad
  _ 1 led with it's corresponding resistor between GND and tlmpLoadPin
  _ 1 led with it's corresponding resistor between GND and tlmpbPilotPin
*/

// put function declarations here:

// put Global declarations here: 
const uint8_t htlmpbSwitchPin{GPIO_NUM_25};
const uint8_t htlmpbLoadPin{GPIO_NUM_21};
const uint8_t htlmpbWnngPin{GPIO_NUM_15};
const uint8_t htlmpbPilotPin{GPIO_NUM_18};

const uint8_t dmpbSwitchPin{GPIO_NUM_26};
const uint8_t dmpbLoadPin{GPIO_NUM_19};

HntdTmLtchMPBttn htlBttn (htlmpbSwitchPin, 4000, 33, true, true, 20, 50);
StrcsTmrSwitch myStairSwitch (htlBttn, htlmpbLoadPin, htlmpbWnngPin, htlmpbPilotPin);

DbncdDlydMPBttn ddBttn (dmpbSwitchPin, true, true, 0, 200);
DbncdDlydSwitch myDbncDlydSwitch(ddBttn, dmpbLoadPin);

void setup() {
  // put your setup code here, to run once:
  myStairSwitch.getUnderlMPB()->setKeepPilot(true);
  myStairSwitch.getUnderlMPB()->setActTime(6000);
  myStairSwitch.setBlnkRate(300);
  myStairSwitch.setBlnkWrnng(true);
}

void loop() {
  //Now unneeded as all runs as independent tasks! Delete the loop() task
  vTaskDelete(nullptr);
}
