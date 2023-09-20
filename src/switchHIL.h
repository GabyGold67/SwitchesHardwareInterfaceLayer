#ifndef _SWITCHHIL_H_
#define _SWITCHHIL_H_

#include <Arduino.h>
#include <mpbToSwitch.h>

#define MAX_SWITCHES_PER_CLASS 5
#define MAX_SWITCHES_TOTAL 20
#define MIN_BLINK_RATE 100

//Function prototypes

//Global variables
static const uint8_t _exePrty = 1;   //Execution priority of the updating Task
static const int app_cpu = xPortGetCoreID();
static BaseType_t rc;

//Classes definitions
class HILSwitches{  //Virtual superclass (Base Class) for all the switches subclases
protected:
  uint8_t _loadPin{};
  static uint8_t totalSwitchesCount;
public:
  HILSwitches();
  virtual ~HILSwitches();
  virtual bool updOutputs() = 0;  //Makes this an abstract class and forces all subclasses to implement this method to become non abstracts (used to update outputs states)
  static uint8_t getSwitchesCount();
  static TaskHandle_t HILSwtchsTskHndl;
};

//=============================================================>

class DbncdDlydSwitch: public HILSwitches{
protected:
  DbncdDlydMPBttn* _underlMPB;
  uint8_t _loadPin{};
  static uint8_t ddSwtchCount;
public:
  DbncdDlydSwitch(DbncdDlydMPBttn &lgcMPB, uint8_t loadPin);
  virtual bool updOutputs();
  DbncdDlydMPBttn* getUnderlMPB();
  static uint8_t getSwitchesCount();
};

//=============================================================>
class TmVdblSwitch: public HILSwitches{
  static TaskHandle_t tvSwtchTskHndl;  //TaskHandle to the updating task that keeps this class objects outputs updated
protected:
  TmVdblMPBttn* _underlMPB;
  uint8_t _loadPin{};
  static uint8_t tvSwtchCount;
public:
  TmVdblSwitch(TmVdblMPBttn &lgcMPB, uint8_t loadPin);
  virtual bool updOutputs();
  TmVdblMPBttn* getUnderlMPB();
  static uint8_t getSwitchesCount();

};

//=============================================================>

class StrcsTmrSwitch: public HILSwitches{
  //static TaskHandle_t stSwtchTskHndl;  //TaskHandle to the updating task that keeps this class objects outputs updated
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
  static uint8_t stSwtchCount;
public:
  StrcsTmrSwitch(HntdTmLtchMPBttn &lgcMPB, uint8_t loadPin,uint8_t wnngPin = 0, uint8_t pltPin = 0);
  virtual bool updOutputs();
  bool setActvPilot(bool actvPilot);
  bool setActvWarning(bool actvWarning);
  const bool getActvPilot() const;
  const bool getActvWarning() const;
  HntdTmLtchMPBttn* getUnderlMPB();
  bool setBlnkWrnng(bool newBlnkSett);
  bool setBlnkRate(unsigned long int newBlnkRate);
  bool blinkWrnng();
  bool noBlinkWrnng();    
  static uint8_t getSwitchesCount();

};

//=============================================================>

class HntdTmVdblScrtySwitch: public HILSwitches{
  static TaskHandle_t htvsSwtchTskHndl;
protected:
  TmVdblMPBttn* _underlMPB;
  uint8_t _loadPin{};
  uint8_t _voidedPin{};
  uint8_t _disabledPin{};
  
  const unsigned long int _minVoidTime{1000};
  bool _enabled{true};
  bool _onIfDisabled{false};
  static uint8_t htvsSwtchCount;
public:
  HntdTmVdblScrtySwitch(TmVdblMPBttn &lgcMPB, uint8_t loadPin, uint8_t voidedPin = 0, uint8_t disabledPin = 0);
  virtual bool updOutputs();
  bool setEnabled(bool newEnable);
  bool updIsEnabled(const bool &enabledValue);
  bool updIsOn(const bool &onValue);
  bool updIsVoided(const bool &voidValue);
  bool setOnIfDisabled(bool newIsOn);
  bool enable();
  bool disable();
  static uint8_t getSwitchesCount();
};

//=============================================================>

class GuardedSwitch: public HILSwitches{
  static TaskHandle_t grddSwtchTskHndl;
protected:
  DbncdDlydMPBttn* _underlGuard;
  DbncdDlydMPBttn* _underlMPB;
  uint8_t _loadPin{};
  uint8_t _guardPin{};
  static uint8_t gSwtchCount;
public:
  GuardedSwitch(DbncdDlydMPBttn* underlMPB, DbncdDlydMPBttn* underlGuard, uint8_t loadPin);
  virtual bool updOutputs();
  DbncdDlydMPBttn* getUnderlMPB();
  DbncdDlydMPBttn* getUnderlGuard();
  static uint8_t getSwitchesCount();
};

#endif