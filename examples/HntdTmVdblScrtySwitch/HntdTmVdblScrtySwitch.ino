#include <Arduino.h>
#include <switchHIL.h>

/*
  mpbToSwitch Library for
    Framework: Arduino
    Platform: ESP32

  HntdTmVdblScrtySwitch.ino
  Created by Gabriel D. Goldman, August, 2023.
  Updated by Gabriel D. Goldman, September, 2023.
  Released into the public domain in accordance with "GPL-3.0-or-later" license terms.

  Example file to demonstrate DbncdDlydSwitch and StrcsTmrSwitch classes, required hardware and connections:
  _ 1 push button between GND and htvsSwtchPin
  _ 1 push button between GND and ddSwtchDsblPin
  _ 1 led with it's corresponding resistor between GND and htvsLoadPin
  _ 1 led with it's corresponding resistor between GND and htvsVddPin
  _ 1 led with it's corresponding resistor between GND and htvsDsbldPin
*/

// put function declarations here:

// put Global declarations here: 
const uint8_t htvsSwtchPin{GPIO_NUM_25};
const uint8_t htvsLoadPin{GPIO_NUM_21};
const uint8_t htvsVddPin{GPIO_NUM_15};
const uint8_t htvsDsbldPin{GPIO_NUM_18};

const uint8_t ddSwtchDsblPin{GPIO_NUM_26};
const uint8_t dmpbLoadPin{GPIO_NUM_19};

TmVdblMPBttn tvBttn (htvsSwtchPin, 4000, true, true, 20, 50, true);
HntdTmVdblScrtySwitch mySecuritySwitch (tvBttn, htvsLoadPin, htvsVddPin, htvsDsbldPin);

DbncdDlydMPBttn ddBttn (ddSwtchDsblPin, true, true, 0, 50);
DbncdDlydSwitch myDbncDlydSwitch(ddBttn, dmpbLoadPin);

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  //Now unneeded as all runs as independent tasks! Delete the loop() task
  vTaskDelete(nullptr);
}

