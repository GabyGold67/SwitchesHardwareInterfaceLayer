#include <switchHIL.h>

//Static members for HILSwitches CLASS <<================
uint8_t HILSwitches::totalSwitchesCount = 0; //Counter of all instantiated HILSwitches objects
TaskHandle_t HILSwitches::HILSwtchsTskHndl = nullptr;
HILSwitches** HILSwtchsInstPtrs{nullptr};   // pointer to a array of pointers that'll be created dinamically if needed at runtime when HILSwitches is first instantiated

//Static members for HILSwitches SUBCLASSES
uint8_t DbncdDlydSwitch::ddSwtchCount = 0;
uint8_t StrcsTmrSwitch::stSwtchCount = 0;
uint8_t HntdTmVdblScrtySwitch::htvsSwtchCount = 0;
uint8_t GrddSwitch::gSwtchCount = 0;

void HILSwtchsTskCallback(void *argp){
  //TmVdblMPBttn *mySwtch = (TmVdblMPBttn*)argp;  //Parameter casting: the parameter passed by the task is casted to its real type
  for (;;){
    for(int i{0}; i < HILSwitches::getSwitchesCount(); i++){
       HILSwtchsInstPtrs[i]->updOutputs();
    }
    ulTaskNotifyTake(   pdTRUE,          /* Clear the notification value before exiting. */
                        portMAX_DELAY ); /* Block indefinitely. */
  }    
}

//=========================================================================> Class methods delimiter 

HILSwitches::HILSwitches(){ //This default constructor of this base class will be called first each time the constructor of the subclasses are invoked
    if (!HILSwtchsTskHndl){  //The task has not been created yet, create it through a function that all switch classes must use

        //Set the task to keep the outputs updated and set the function name to the updater function
        rc = xTaskCreatePinnedToCore(
            HILSwtchsTskCallback,  //function to be called
            "UpdHILSwtchsOutputs",  //Name of the task
            1716,   //Stack size (in bytes in ESP32, words in FreeRTOS), the minimum value is in the config file, for this is 768 bytes
            &HILSwtchsInstPtrs,  //Pointer to the parameters for the function to work with
            // &ddSwtchsInstPtrs,  //Pointer to the parameters for the function to work with
            _exePrty,      //Priority level given to the task
            &HILSwtchsTskHndl, //Task handle
            app_cpu //Run in the App Core if it's a dual core mcu (ESP-FreeRTOS specific)
        );
        assert(rc == pdPASS);
        assert(HILSwtchsTskHndl);
    }
    if (HILSwtchsInstPtrs == nullptr){ //First instantantiation of the class, an array will be created
        HILSwtchsInstPtrs = new HILSwitches* [MAX_TOTAL_SWITCHES];
    }
}

HILSwitches::~HILSwitches(){
    // Decrement switches total count
    // Check for switches instantiated (from the switches count and/or traversing the pointers aray)
    // If counter == 0 and/or pointers array empty
        // delete [] HILSwtchsInstPtrs
        // HILSwtchsInstPtrs = nullptr
}

uint8_t HILSwitches::getSwitchesCount(){
  
  return totalSwitchesCount;
}

//=========================================================================> Class methods delimiter

DbncdDlydSwitch::DbncdDlydSwitch(DbncdDlydMPBttn &lgcMPB, uint8_t loadPin)
:_underlMPB{&lgcMPB}, _loadPin{loadPin}
{
    //Set the output pins to the required states
    digitalWrite(_loadPin, LOW);   //Ensure the pin signal is down before setting as output for safety. Usually unneded as all pins are initiated openC, 
    pinMode(_loadPin, OUTPUT);

    //if the ddSwtchTskHndl is still not created the HILSwitches constructor will take care

    //Add a pointer to the switch instantiated to the array of pointers to all HILSwitches subclasses created
    if(totalSwitchesCount < MAX_TOTAL_SWITCHES){
        HILSwtchsInstPtrs[totalSwitchesCount] = this;
        ++totalSwitchesCount;
        ++ddSwtchCount;
    }

    //This implementation of the Switch uses the xTaskToNotify
    _underlMPB->setTaskToNotify(HILSwtchsTskHndl);    //Notify the Underlying DbncdDlydMPButton the taskhandle it'll have to notify for updating the switch outputs

    _underlMPB->begin(); //Set the logical underlying mpBttn to start updating it's inputs readings & output states
}

bool DbncdDlydSwitch::updOutputs(){
    if(_underlMPB->getIsOn()){
        if(digitalRead(_loadPin) != HIGH)
            digitalWrite(_loadPin, HIGH);
    }
    else{
        if(digitalRead(_loadPin) != LOW)
            digitalWrite(_loadPin, LOW);
    }

    return true;
}

DbncdDlydMPBttn* DbncdDlydSwitch::getUnderlMPB(){
    
    return _underlMPB;
}

uint8_t DbncdDlydSwitch::getSwitchesCount(){
    
    return ddSwtchCount;
}

//=========================================================================> Class methods delimiter 

StrcsTmrSwitch::StrcsTmrSwitch(HntdTmLtchMPBttn &lgcMPB, uint8_t loadPin, uint8_t wnngPin, uint8_t pltPin)
:_underlMPB{&lgcMPB}, _loadPin {loadPin}, _wrnngPin{wnngPin}, _pltPin{pltPin}
{     
    //Set the output pins to the required states
    digitalWrite(_loadPin, LOW);   //Ensure the pin signal is down before setting as output for safety. Usually unneded as all pins are initiated input openC, 
    pinMode(_loadPin, OUTPUT);

    if (_wrnngPin > 0){
        digitalWrite(_wrnngPin, LOW);   //Ensure the pin signal is down before setting as output for safety. Usually unneded as all pins are initiated input openC, 
        pinMode(_wrnngPin, OUTPUT);
    }

    if (_pltPin > 0){
        digitalWrite(_pltPin, LOW);   //Ensure the pin signal is down before setting as output for safety. Usually unneded as all pins are initiated openC, 
        pinMode(_pltPin, OUTPUT);
    }

    //Add a pointer to the switch instantiated in the array of pointers of switches whose outputs must be updated
    if(totalSwitchesCount < MAX_TOTAL_SWITCHES){
        HILSwtchsInstPtrs[totalSwitchesCount] = this;
        ++totalSwitchesCount;
        ++stSwtchCount;
    }            

    //This implementation of the Switch uses the xTaskToNotify
    _underlMPB->setTaskToNotify(HILSwtchsTskHndl);    //Notify the Underlying DbncdDlydMPButton the taskhandle it'll have to notify for updating the switch outputs

    _underlMPB->begin(); //Set the logical underlying mpBttn to start updating it's inputs readings & output states
}

bool StrcsTmrSwitch::updOutputs(){
    if(_underlMPB->getIsOn()){
        if(digitalRead(_loadPin) != HIGH)
            digitalWrite(_loadPin, HIGH);
    }
    else{
        if(digitalRead(_loadPin) != LOW)
            digitalWrite(_loadPin, LOW);
    }
    
    if(_wrnngPin > 0){
        if(_underlMPB->getWrnngOn()){   //The underlying mpb has the Warning flag activated
            if (_wrnngBlnks){   //If the warning output is configured to blink            
                if( !xTimerIsTimerActive(_stsBlnkTmrHndl)){
                    // the blinking timer is not active, activate it to provide blinking service.                    
                    if(!_blnkOutputOn)  //**************************************************>> _blnkOutputOn is a shared resourse //Set protective MuTex
                        _blnkOutputOn = true;   //the blinking always starts setting the warning on and then turning it off if there's enough time!
                    xTimerStart(_stsBlnkTmrHndl, portMAX_DELAY);
                }
            }
            else{   //The warning output is configured to stay still (no blinking)
                if(xTimerIsTimerActive(_stsBlnkTmrHndl)){   
                    xTimerStop(_stsBlnkTmrHndl, portMAX_DELAY); //The blinking timer is running, so we'll stop it
                    
                    if(!_blnkOutputOn)  //**************************************************>> _blnkOutputOn is a shared resourse, //Set protective MuTex
                        _blnkOutputOn = true;   //the blinking always ends setting the warning on for the next blinking loop.
                }
            }                        
            // set the pin high/low deppending on _blnkOutputOn state
            if(!_wrnngBlnks || (_wrnngBlnks && _blnkOutputOn)){ //The output pin must be HIGH
                if(digitalRead(_wrnngPin) != HIGH)
                    digitalWrite(_wrnngPin, HIGH);
            }
            else{
                if(digitalRead(_wrnngPin) != LOW)
                    digitalWrite(_wrnngPin, LOW);
            }
        }
        else{   //The underlying mpb has the Warning flag deactivated
            if(digitalRead(_wrnngPin) != LOW)
                digitalWrite(_wrnngPin, LOW);
        }
    }

    if(_pltPin > 0){
        if(_underlMPB->getPilotOn()){
            if(digitalRead(_pltPin) != HIGH)
                digitalWrite(_pltPin, HIGH);
        }
        else{
            if(digitalRead(_pltPin) != LOW)
                digitalWrite(_pltPin, LOW);
        }
    }
    
    return true;
}

HntdTmLtchMPBttn* StrcsTmrSwitch::getUnderlMPB(){

    return _underlMPB;
}

bool StrcsTmrSwitch::setBlnkWrnng(bool newBlnkSett){    
    if(_wrnngPin > 0){  //There's a pin asigned for some warning hardware
        if(newBlnkSett != _wrnngBlnks){ //There's a real change in the _brnngBlnks value
            _wrnngBlnks = newBlnkSett;
            if(_wrnngBlnks == true){  //Create the warning timer to the current blinkRate
                if (!_stsBlnkTmrHndl){        
                    _stsBlnkTmrHndl = xTimerCreate(
                        "WarningBlink",  //Timer name
                        pdMS_TO_TICKS(_wrnngBlnkRate),  //Timer period in ticks
                        pdTRUE,     //Autoreload true
                        this,       //TimerID: data passed to the callback funtion to work
                        StrcsTmrSwitch::toggleBlnkOutputOn);
                    assert (_stsBlnkTmrHndl);
                } 
            }
            else{       //If there is a timer handle: delete the timer and null the handle

                xTimerDelete(_stsBlnkTmrHndl, portMAX_DELAY);
                _stsBlnkTmrHndl = nullptr;
            }
        }
    }

    return _wrnngBlnks;
}

bool StrcsTmrSwitch::setBlnkRate(unsigned long int newBlnkRate){
    bool result {false};
    
    if(newBlnkRate >= MIN_BLINK_RATE)
        _wrnngBlnkRate = newBlnkRate;
        result = true;
        if (_stsBlnkTmrHndl){        
            //If the timer is already running modify the Timer period
            if(!xTimerChangePeriod( _stsBlnkTmrHndl, _wrnngBlnkRate / portTICK_PERIOD_MS, 100 ) == pdPASS)
                result = false;
        }

    return result;
}

void StrcsTmrSwitch::toggleBlnkOutputOn(TimerHandle_t blnkTmrArg){
    StrcsTmrSwitch *swtchObj = (StrcsTmrSwitch*)pvTimerGetTimerID(blnkTmrArg);
    swtchObj->setBlnkOutputOn(!(swtchObj->getBlnkOutputOn()));  

    return;
  }

const bool StrcsTmrSwitch::getBlnkOutputOn() const{

    return _blnkOutputOn;   
}

bool StrcsTmrSwitch::setBlnkOutputOn(const bool &newBlnkOutputOn){
    if(_blnkOutputOn != newBlnkOutputOn){
        _blnkOutputOn = newBlnkOutputOn;    //Set protective MuTex
    }

    return _blnkOutputOn;
}

bool StrcsTmrSwitch::blinkWrnng(){
    
    return setBlnkWrnng(true);
}

bool StrcsTmrSwitch::noBlinkWrnng(){
    
    return setBlnkWrnng(false);
}

uint8_t StrcsTmrSwitch::getSwitchesCount(){
    
    return stSwtchCount;
}

//=========================================================================> Class methods delimiter

HntdTmVdblScrtySwitch::HntdTmVdblScrtySwitch(TmVdblMPBttn &lgcMPB, uint8_t loadPin, uint8_t voidedPin, uint8_t disabledPin)
:_underlMPB{&lgcMPB}, _loadPin{loadPin}, _voidedPin{voidedPin}, _disabledPin{disabledPin}
{
    //Set the output pins to the required states
    digitalWrite(_loadPin, LOW);   //Ensure the pin signal is down before setting as output for safety. Usually unneded as all pins are initiated Input OpenC
    pinMode(_loadPin, OUTPUT);

    if (_voidedPin > 0){
        digitalWrite(_voidedPin, LOW);   //Ensure the pin signal is down before setting as output for safety. Usually unneded as all pins are initiated Input OpenC
        pinMode(_voidedPin, OUTPUT);
    }

    if (_disabledPin > 0){
        digitalWrite(_disabledPin, LOW);   //Ensure the pin signal is down before setting as output for safety. Usually unneded as all pins are initiated Input OpenC
        pinMode(_disabledPin, OUTPUT);
    }
    
    //Add a pointer to the switch instantiated to the array of pointers of switches which outputs must be updated
    if(totalSwitchesCount < MAX_TOTAL_SWITCHES){
        HILSwtchsInstPtrs[totalSwitchesCount] = this;
        ++totalSwitchesCount;
        ++htvsSwtchCount;
    }

    //This implementation of the Switch uses the xTaskToNotify
    _underlMPB->setTaskToNotify(HILSwtchsTskHndl);    //Notify the Underlying DbncdDlydMPButton the taskhandle it'll have to notify is updating the switch outputs

    _underlMPB->begin(); //Set the logical underlying mpBttn to start updating it's inputs readings & output states

}

bool HntdTmVdblScrtySwitch::updOutputs(){
    
    if(!_underlMPB->getIsEnabled()){ // The switch is DISABLED, handle the outputs accordingly
        // Set disabledPin ON
        updIsEnabled(false);
        //If the switch is disabled there should be no voided indication
        updIsVoided(false);
        // if the switch should stay on when disabled
        updIsOn(_underlMPB->getIsOnDisabled());
        // if(_onIfDisabled){      
        //     updIsOn(true);  // LoadPin is forced ON
        // }
        // else{
        //     updIsOn(false); // LoadPin is forced OFF
        // }
    }
    else{
        //The switch is ENABLED, update outputs accordingly
        //Update disabled pin
        updIsEnabled(true);   // Set disabledPin OFF
        //Update voided status/pins
        updIsVoided(_underlMPB->getIsVoided());
        //Update On status/pins
        updIsOn(_underlMPB->getIsOn());
    }
    
    return true;
}

bool HntdTmVdblScrtySwitch::setEnabled(bool newEnabled){
    if (_underlMPB->getIsEnabled() != newEnabled){
        _underlMPB->setIsEnabled(newEnabled);
    }

    return newEnabled;
}

const bool HntdTmVdblScrtySwitch::getEnabled() const{

    return _underlMPB->getIsEnabled();
}

uint8_t HntdTmVdblScrtySwitch::getSwitchesCount(){
    
    return htvsSwtchCount;
}

bool HntdTmVdblScrtySwitch::updIsEnabled(const bool &enabledValue){
    if(_disabledPin > 0){
        if(enabledValue != (!digitalRead(_disabledPin)))
            digitalWrite(_disabledPin, !enabledValue);
    }

    return enabledValue;

}

bool HntdTmVdblScrtySwitch::updIsOn(const bool &onValue){
    if(_loadPin > 0){
        if(onValue != digitalRead(_loadPin))
            digitalWrite(_loadPin, onValue);
    }

    return onValue;
}

bool HntdTmVdblScrtySwitch::updIsVoided(const bool &voidValue){
    if(_voidedPin > 0){
        if(voidValue != digitalRead(_voidedPin) )
            digitalWrite(_voidedPin, voidValue);
    }
    
    return voidValue;
}

bool HntdTmVdblScrtySwitch::enable(){
    
    return setEnabled(true);
}

bool HntdTmVdblScrtySwitch::disable(){
    
    return setEnabled(false);
}

//=========================================================================> Class methods delimiter

GrddSwitch::GrddSwitch(DbncdDlydMPBttn &underlMPB, DbncdDlydMPBttn &underlGuard, uint8_t loadPin, uint8_t guardRlsdPin)
:_underlMPB{&underlMPB}, _underlGuard{&underlGuard}, _loadPin{loadPin}, _guardRlsdPin{guardRlsdPin}
{
    //Set the output pins to the required states
    digitalWrite(_loadPin, LOW);   //Ensure the pin signal is down before setting as output for safety. Usually unneded as all pins are initiated Input OpenC
    pinMode(_loadPin, OUTPUT);

    if(_guardRlsdPin > 0){
        digitalWrite(_guardRlsdPin, LOW);   //Ensure the pin signal is down before setting as output for safety. Usually unneded as all pins are initiated Input OpenC
        pinMode(_guardRlsdPin, OUTPUT);
    }
    //Add a pointer to the switch instantiated to the array of pointers of switches which outputs must be updated
    if(totalSwitchesCount < MAX_TOTAL_SWITCHES){
        HILSwtchsInstPtrs[totalSwitchesCount] = this;
        ++totalSwitchesCount;
        ++gSwtchCount;
    }
    //This implementation of the Switch uses the xTaskToNotify

    _underlGuard->setTaskToNotify(HILSwtchsTskHndl);    //Notify the Underlying DbncdDlydMPButton the taskhandle it'll have to notify is updating the switch outputs
    _underlMPB->setTaskToNotify(HILSwtchsTskHndl);    //Notify the Underlying DbncdDlydMPButton the taskhandle it'll have to notify is updating the switch outputs
    _underlGuard->begin(); //Set the logical underlying mpBttn to start updating it's inputs readings & output states
    _underlMPB->begin(); //Set the logical underlying mpBttn to start updating it's inputs readings & output states
        
}

bool GrddSwitch::updOutputs(){
    if(_underlGuard->getIsOn()){
        if(_guardRlsdPin > 0){
            if(digitalRead(_guardRlsdPin) != HIGH)
                digitalWrite(_guardRlsdPin, HIGH);   //Ensure the pin signal is down before setting as output for safety. Usually unneded as all pins are initiated Input OpenC
        }
        if(_underlMPB->getIsOn()){
            if(digitalRead(_loadPin) != HIGH)
                digitalWrite(_loadPin, HIGH);
        }
        else{
            if(digitalRead(_loadPin) != LOW)
                digitalWrite(_loadPin, LOW);
        }
    }
    else{
        if(_guardRlsdPin > 0){
            if(digitalRead(_guardRlsdPin) != LOW)
                digitalWrite(_guardRlsdPin, LOW);   //Ensure the pin signal is down before setting as output for safety. Usually unneded as all pins are initiated Input OpenC
        }
        if(digitalRead(_loadPin) != LOW)
            digitalWrite(_loadPin, LOW);
    }

    return true;
}

DbncdDlydMPBttn *GrddSwitch::getUnderlMPB(){

    return _underlMPB;
}

uint8_t GrddSwitch::getSwitchesCount(){
    
    return gSwtchCount;
}

DbncdDlydMPBttn *GrddSwitch::getUnderlGuard(){
    
    return _underlGuard;
}
