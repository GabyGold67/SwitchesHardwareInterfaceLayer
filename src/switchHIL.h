#ifndef _SWITCHHIL_H_
#define _SWITCHHIL_H_

#include <Arduino.h>
#include <mpbToSwitch.h>

#define MAX_SWITCHES 5
#define MIN_BLINK_RATE 100

//Function prototypes

//Global variables needed for this file (declaring them at the end of file produces "was not declared in this scope" errors)
static uint8_t totalSwitchesCount = 0; //Counter of all instantiated HILSwitches objects
static uint8_t _exePrty {1};   //Execution priority of the updating Task
static int app_cpu = xPortGetCoreID();
static BaseType_t rc;

//Classes definitions
class HILSwitches{  //Virtual class used as superclass of all the switches, one of the solutions to manage several switch subclasses through it

protected:
      uint8_t _loadPin{};
public:
  HILSwitches();
  virtual bool updOutputs() = 0;  //Makes it a virtual class and forces all subclasses to  implement this method, used to update outputs states
};

//=============================================================>

class StrcsTmrSwitch: public HILSwitches{
  static TaskHandle_t stSwtchTskHndl;  //TaskHandle to the updating task that keeps this class objects outputs updated
protected:
    HntdTmLtchMPBttn* _underlMPB;
    uint8_t _loadPin{};
    uint8_t _wrnngPin{};
    uint8_t _pltPin{};

    bool _actvWrnng{false};
    bool _actvPlt {false};
    bool _wrnngBlnks{false};
    unsigned long int _wrnngBlnkRate{250};
    unsigned long int _wrnngLstSwp{0};
    bool _blnkOutputOn{true};
public:
    StrcsTmrSwitch(HntdTmLtchMPBttn &lgcMPB, uint8_t loadPin,uint8_t wnngPin = 0, uint8_t pltPin = 0);
    bool updOutputs();
    bool setActvPilot(bool actvPilot);
    bool setActvWarning(bool actvWarning);
    const bool getActvPilot() const;
    const bool getActvWarning() const;
    HntdTmLtchMPBttn* getUnderlMPB();
    bool setBlnkWrnng(bool newBlnkSett);
    bool setBlnkRate(unsigned long int newBlnkRate);
    bool blinkWrnng();
    bool noBlinkWrnng();    
    static uint8_t stSwtchCount;
};

//=============================================================>>

class DbncdDlydSwitch: public HILSwitches{
  static TaskHandle_t ddSwtchTskHndl;  //TaskHandle to the updating task that keeps this class objects outputs updated
protected:
  DbncdDlydMPBttn* _underlMPB;
  uint8_t _loadPin{};
public:
  DbncdDlydSwitch(DbncdDlydMPBttn &lgcMPB, uint8_t loadPin);
  bool updOutputs();
  DbncdDlydMPBttn* getUnderlMPB();
  static uint8_t ddSwtchCount;
};

//=============================================================>

#endif