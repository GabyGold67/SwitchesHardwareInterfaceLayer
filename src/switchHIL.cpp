#include <switchHIL.h>

//Static members for HILSwitches CLASS <<================
//static uint8_t totalSwitchesCount = 0; //Counter of all instantiated HILSwitches objects
uint8_t HILSwitches::totalSwitchesCount = 0; //Counter of all instantiated HILSwitches objects
TaskHandle_t HILSwitches::HILSwtchsTskHndl = nullptr;
HILSwitches* HILSwtchsInstances[MAX_SWITCHES_TOTAL];

//Static members for HILSwitches SUBCLASSES
uint8_t StrcsTmrSwitch::stSwtchCount = 0;
TaskHandle_t StrcsTmrSwitch::stSwtchTskHndl = nullptr;
StrcsTmrSwitch* stSwtchsInstances[MAX_SWITCHES];

uint8_t DbncdDlydSwitch::ddSwtchCount = 0;
TaskHandle_t DbncdDlydSwitch::ddSwtchTskHndl = nullptr;
DbncdDlydSwitch* ddSwtchsInstances[MAX_SWITCHES];

uint8_t HntdTmVdblScrtySwitch::htvsSwtchCount = 0;
TaskHandle_t HntdTmVdblScrtySwitch::htvsSwtchTskHndl = nullptr;
HntdTmVdblScrtySwitch* htvsSwtchsInstances[MAX_SWITCHES];

void stSwtchsTskCallback(void* argp){
  //StrcsTmrSwitch *mySwtch = (StrcsTmrSwitch*)argp; //Parameter casting: the parameter passed by the task is casted to its real type

  for (;;){
    for(int i{0}; i < StrcsTmrSwitch::getSwitchesCount(); i++){
       stSwtchsInstances[i]->updOutputs();
    }
    vTaskDelay(10/portTICK_PERIOD_MS);  //There's no need to refresh the outputs so frequently, and failing to let lower priorities tasks includes the idle(), so the Watchdog will reset the mcu
  }
}

void ddSwtchsTskCallback(void* argp){
  //DbncdDlydSwitch *mySwtch = (DbncdDlydSwitch*)argp;  //Parameter casting: the parameter passed by the task is casted to its real type

  for (;;){
    for(int i{0}; i < DbncdDlydSwitch::getSwitchesCount(); i++){
       ddSwtchsInstances[i]->updOutputs();
    }
    vTaskDelay(10/portTICK_PERIOD_MS);  //There's no need to refresh the outputs so frequently, and failing to let lower priorities tasks includes the idle(), so the Watchdog will reset the mcu

  }
}

void htvsSwtchsTskCallback(void *argp){
  //TmVdblMPBttn *mySwtch = (TmVdblMPBttn*)argp;  //Parameter casting: the parameter passed by the task is casted to its real type
  for (;;){
    for(int i{0}; i < HntdTmVdblScrtySwitch::getSwitchesCount(); i++){
       htvsSwtchsInstances[i]->updOutputs();
    }
    vTaskDelay(10/portTICK_PERIOD_MS);  //There's no need to refresh the outputs so frequently, and failing to let lower priorities tasks includes the idle(), so the Watchdog will reset the mcu

  }

}

void HILSwtchsTskCallback(void *argp){
  //TmVdblMPBttn *mySwtch = (TmVdblMPBttn*)argp;  //Parameter casting: the parameter passed by the task is casted to its real type
  for (;;){
    for(int i{0}; i < HILSwitches::getSwitchesCount(); i++){
       HILSwtchsInstances[i]->updOutputs();
    }
    vTaskDelay(10/portTICK_PERIOD_MS);  //There's no need to refresh the outputs so frequently, and failing to let lower priorities tasks includes the idle(), so the Watchdog will reset the mcu

  }
    
}
//=========================================================================> Class methods delimiter 

HILSwitches::HILSwitches(){ //This default constructor of this superclass will be called first each time the constructor of the subclasses are invoked
    //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<  WIP START
    //Under development, trying to build a general HILSwitches subclasses array of pointers!
        /*
        if (!HILSwtchsTskHndl){  //The task has not been created yet, create it through a function that all switch classes must use

            //Set the task to keep the outputs updated and set the function name to the updater function
            rc = xTaskCreatePinnedToCore(
            HILSwtchsTskCallback,  //function to be called
            "UpdHILSwtchOutputs",  //Name of the task
            1716,   //Stack size (in bytes in ESP32, words in FreeRTOS), the minimum value is in the config file, for this is 768 bytes
            &HILSwtchsInstances,  //Pointer to the parameters for the function to work with
            _exePrty,      //Priority level given to the task
            &HILSwtchsTskHndl, //Task handle
            app_cpu //Run in the App Core if it's a dual core mcu (ESP-FreeRTOS specific)
            );
            assert(rc == pdPASS);
            assert(HILSwtchsTskHndl);
        }
        */
    //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<  WIP END


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

    if (!ddSwtchTskHndl){  //The task has not been created yet, create it through a function that all switch classes must use
        //int app_cpu = xPortGetCoreID();

        //Set the task to keep the outputs updated and set the function name to the updater function
        rc = xTaskCreatePinnedToCore(
        ddSwtchsTskCallback,  //function to be called
        "UpdDdSwtchOutputs",  //Name of the task
        1716,   //Stack size (in bytes in ESP32, words in FreeRTOS), the minimum value is in the config file, for this is 768 bytes
        &ddSwtchsInstances,  //Pointer to the parameters for the function to work with
        _exePrty,      //Priority level given to the task
        &ddSwtchTskHndl, //Task handle
        app_cpu //Run in the App Core if it's a dual core mcu (ESP-FreeRTOS specific)
        );
        assert(rc == pdPASS);
        assert(ddSwtchTskHndl);
    }
    //Add a pointer to the switch instantiated to the array of pointers to Debounced Delayed Switches whose outputs must be updated
    if(ddSwtchCount < MAX_SWITCHES){
        ddSwtchsInstances[ddSwtchCount] = this;
        ++ddSwtchCount;
    }

    //Add a pointer to the switch instantiated to the array of pointers to all HILSwitches subclasses created
    if(totalSwitchesCount < MAX_SWITCHES_TOTAL){
        HILSwtchsInstances[totalSwitchesCount] = this;
        ++totalSwitchesCount;
    }

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
    digitalWrite(_loadPin, LOW);   //Ensure the pin signal is down before setting as output for safety. Usually unneded as all pins are initiated openC, 
    pinMode(_loadPin, OUTPUT);

    if (_wrnngPin > 0){
        digitalWrite(_wrnngPin, LOW);   //Ensure the pin signal is down before setting as output for safety. Usually unneded as all pins are initiated openC, 
        pinMode(_wrnngPin, OUTPUT);
        _actvWrnng = true;
    }

    if (_pltPin > 0){
        digitalWrite(_pltPin, LOW);   //Ensure the pin signal is down before setting as output for safety. Usually unneded as all pins are initiated openC, 
        pinMode(_pltPin, OUTPUT);
        _actvPlt = true;
    }

    if (!stSwtchTskHndl){  //The task has not been created yet, create it through a function that all switch classes must use
        //int app_cpu = xPortGetCoreID();

        //Set the task to keep the outputs updated and set the function name to the updater function
        rc = xTaskCreatePinnedToCore(
        stSwtchsTskCallback,  //function to be called
        "UpdStSwtchOutputs",  //Name of the task
        1716,   //Stack size (in bytes in ESP32, words in FreeRTOS), the minimum value is in the config file, for this is 768 bytes, this gives a size of 1.5 KB
        &stSwtchsInstances,  //Pointer to the parameters for the function to work with
        _exePrty,      //Priority level given to the task
        &stSwtchTskHndl, //Task handle
        app_cpu //Run in the App Core if it's a dual core mcu (ESP-FreeRTOS specific)
        );
        assert(rc == pdPASS);
        assert(stSwtchTskHndl);
    }
    
    //Add a pointer to the switch instantiated in the array of pointers of switches whose outputs must be updated
    if(stSwtchCount < MAX_SWITCHES){
        stSwtchsInstances[stSwtchCount] = this;
        ++stSwtchCount;
    }            

    //Add a pointer to the switch instantiated to the array of HIL pointers of switches whose outputs must be updated
    if(totalSwitchesCount < MAX_SWITCHES_TOTAL){
        HILSwtchsInstances[totalSwitchesCount] = this;
        ++totalSwitchesCount;
    }

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
        if(_underlMPB->getWrnngOn()){
            if (_wrnngBlnks){   //If the warning output it's configured to blink
                if (_wrnngLstSwp == 0){ //If warning it's just been activated
                    _blnkOutputOn = true;   //Set (ensure) the warning output starts in ON condition
                    _wrnngLstSwp = xTaskGetTickCount() / portTICK_RATE_MS;  //Register blinking activity start on the stopwatch
                }
                else{   // not just activated
                    if (((xTaskGetTickCount() / portTICK_RATE_MS) - _wrnngLstSwp) >= _wrnngBlnkRate){   //Verify stopwatch time to change condition?
                        _blnkOutputOn = !_blnkOutputOn;     //Yes, swap _blnkOutputOn value
                        _wrnngLstSwp = xTaskGetTickCount() / portTICK_RATE_MS;  //Register time of last swap
                    }
                }
            }

            if((_wrnngBlnks == false) || (_wrnngBlnks && _blnkOutputOn)){
                if(digitalRead(_wrnngPin) != HIGH)
                    digitalWrite(_wrnngPin, HIGH);
                else if(digitalRead(_wrnngPin) != LOW)
                    digitalWrite(_wrnngPin, LOW);
            }
                        
        }
        else{
            if(digitalRead(_wrnngPin) != LOW)
                digitalWrite(_wrnngPin, LOW);
            if(_wrnngLstSwp > 0){   //Safe method to check if blinking was used, as the _wrnngBlnks flag might be changed while a blinking was taking place
                //Ensure blinking stopwatch reset for next use
                _wrnngLstSwp = 0;
                //Ensure the blinking output on is true for next use
                _blnkOutputOn = true;
            }
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

bool StrcsTmrSwitch::setActvPilot(bool actvPilot){
    bool result {true};

    if(_pltPin > 0){
        if(actvPilot != _actvPlt)
            _actvPlt = actvPilot;
    }
    else
        result = false;
    
    return result;
}

bool StrcsTmrSwitch::setActvWarning(bool actvWarning){
    bool result {true};

    if(_wrnngPin > 0){
        if(actvWarning != _actvWrnng)
            _actvPlt = actvWarning;
    }    
    else
        result = false;
    
    return result;
}

const bool StrcsTmrSwitch::getActvPilot() const{

    return _actvPlt;
}

const bool StrcsTmrSwitch::getActvWarning() const{

    return _actvWrnng;
}

HntdTmLtchMPBttn* StrcsTmrSwitch::getUnderlMPB(){
    return _underlMPB;
}

bool StrcsTmrSwitch::setBlnkWrnng(bool newBlnkSett){
    if(newBlnkSett != _wrnngBlnks)
        _wrnngBlnks = newBlnkSett;

    return _wrnngBlnks;
}

bool StrcsTmrSwitch::setBlnkRate(unsigned long int newBlnkRate){
    bool result {false};
    
    if(newBlnkRate >= MIN_BLINK_RATE)
        _wrnngBlnkRate = newBlnkRate;
    else
        result = false;

    return result;
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
    digitalWrite(_loadPin, LOW);   //Ensure the pin signal is down before setting as output for safety. Usually unneded as all pins are initiated openC, 
    pinMode(_loadPin, OUTPUT);

    if (_voidedPin > 0){
        digitalWrite(_voidedPin, LOW);   //Ensure the pin signal is down before setting as output for safety. Usually unneded as all pins are initiated openC, 
        pinMode(_voidedPin, OUTPUT);
    }

    if (_disabledPin > 0){
        digitalWrite(_disabledPin, LOW);   //Ensure the pin signal is down before setting as output for safety. Usually unneded as all pins are initiated openC, 
        pinMode(_disabledPin, OUTPUT);
    }
    if (!htvsSwtchTskHndl){  //The task has not been created yet, create it through a function that all switch classes must use

        //Set the task to keep the outputs updated and set the function name to the updater function
        rc = xTaskCreatePinnedToCore(
        htvsSwtchsTskCallback,  //function to be called
        "UpdHtvsSwtchOutputs",  //Name of the task
        1716,   //Stack size (in bytes in ESP32, words in FreeRTOS), the minimum value is in the config file, for this is 768 bytes, this gives a size of 1.5 KB
        &htvsSwtchsInstances,  //Pointer to the parameters for the function to work with
        _exePrty,      //Priority level given to the task
        &htvsSwtchTskHndl, //Task handle
        app_cpu //Run in the App Core if it's a dual core mcu (ESP-FreeRTOS specific)
        );
        assert(rc == pdPASS);
        assert(htvsSwtchTskHndl);
    }
    
    //Add a pointer to the switch instantiated to the array of pointers of switches whose outputs must be updated
    if(htvsSwtchCount < MAX_SWITCHES){
        htvsSwtchsInstances[htvsSwtchCount] = this;
        ++htvsSwtchCount;
    }            

    //Add a pointer to the switch instantiated to the array of HIL pointers of switches whose outputs must be updated
    if(totalSwitchesCount < MAX_SWITCHES_TOTAL){
        HILSwtchsInstances[totalSwitchesCount] = this;
        ++totalSwitchesCount;
    }
    
    _underlMPB->begin(); //Set the logical underlying mpBttn to start updating it's inputs readings & output states

}

bool HntdTmVdblScrtySwitch::updOutputs(){
    
    if(!_enabled){ // The switch is DISABLED, handle the outputs accordingly
        // Set disabledPin ON
        updIsEnabled(false);
        //If the switch is disabled there should be no voided indication
        updIsVoided(false);
        // if the switch should stay on when disabled
        updIsOn(_onIfDisabled);
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
    if (_enabled != newEnabled){
        _enabled = newEnabled;
    }

    return _enabled;
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

bool HntdTmVdblScrtySwitch::setOnIfDisabled(bool newIsOn){
    if (_onIfDisabled != newIsOn)
        _onIfDisabled = newIsOn;

    return _onIfDisabled;
}

bool HntdTmVdblScrtySwitch::enable(){
    
    return setEnabled(true);
}

bool HntdTmVdblScrtySwitch::disable(){
    
    return setEnabled(false);
}

//=========================================================================> Class methods delimiter

GuardedSwitch::GuardedSwitch(DbncdDlydMPBttn* underlMPB, DbncdDlydMPBttn* underlGuard, uint8_t loadPin)
{
}

bool GuardedSwitch::updOutputs(){

    return false;
}

DbncdDlydMPBttn *GuardedSwitch::getUnderlMPB(){

    return nullptr;
}

uint8_t GuardedSwitch::getSwitchesCount(){
    
    return gSwtchCount;
}

DbncdDlydMPBttn *GuardedSwitch::getUnderlGuard(){
    
    return nullptr;
}
