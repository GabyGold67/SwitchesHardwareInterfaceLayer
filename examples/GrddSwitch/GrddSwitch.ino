#include <Arduino.h>
#include <switchHIL.h>

/*
  SwitchHIL Library for
    Framework: Arduino
    Platform: ESP32

  GrddSwitch.ino
  Created by Gabriel D. Goldman, August, 2023.
  Updated by Gabriel D. Goldman, September, 2023.
  Released into the public domain in accordance with "GPL-3.0-or-later" license terms.

  Example file to demonstrate DbncdDlydSwitch and StrcsTmrSwitch classes, required hardware and connections:
  _ 1 push button between GND and grddSwitchGuardPin
  _ 1 push button between GND and grddSwitchPin
  _ 1 led with it's corresponding resistor between GND and guardRlsdNotifPin
  _ 1 led with it's corresponding resistor between GND and grddLoadPin
*/

// put Global declarations here: 
const uint8_t grddSwitchGuardPin{GPIO_NUM_26};
DbncdDlydMPBttn ddBttnGuard(grddSwitchGuardPin, true, true, 0, 50);

const uint8_t grddSwitchPin{GPIO_NUM_25};
DbncdDlydMPBttn ddBttnSwitch(grddSwitchPin, true, true, 0, 200);

const uint8_t guardRlsdNotifPin{GPIO_NUM_19};
const uint8_t grddLoadPin{GPIO_NUM_21};
GrddSwitch myGrddSwitch (ddBttnSwitch, ddBttnGuard, grddLoadPin, guardRlsdNotifPin);

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  //Now unneeded as all runs as independent tasks! Delete the loop() task
  vTaskDelete(nullptr);
}
