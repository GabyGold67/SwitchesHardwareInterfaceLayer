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

// put Global declarations here: 
const uint8_t htvsSwtchPin{GPIO_NUM_25};
const uint8_t htvsLoadPin{GPIO_NUM_21};
const uint8_t htvsVddPin{GPIO_NUM_17};
const uint8_t htvsDsbldPin{GPIO_NUM_18};

const uint8_t ddSwtchDsblPin{GPIO_NUM_26};
const uint8_t dmpbLoadPin{GPIO_NUM_19};

TmVdblMPBttn tvBttn (htvsSwtchPin, 3000, true, true, 20, 50, false);
HntdTmVdblScrtySwitch mySecuritySwitch (tvBttn, htvsLoadPin, htvsVddPin, htvsDsbldPin);

DbncdDlydMPBttn ddBttn (ddSwtchDsblPin, true, true, 0, 50);
DbncdDlydSwitch myDbncDlydSwitch(ddBttn, dmpbLoadPin);

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  if(myDbncDlydSwitch.getUnderlMPB()->getIsOn()){
    if(mySecuritySwitch.getEnabled())
      mySecuritySwitch.disable();
  }
  else{
    if(!mySecuritySwitch.getEnabled())
      mySecuritySwitch.enable();
  }
  vTaskDelay(25/portTICK_PERIOD_MS);
}
