# **Switches Hardware Interface Layer** Library (switchHIL)
## An Arduino-ESP32 library that builds physical electric switches outputs using the mpbToSwitch library for logic simulated mechanisms.  

The general working working path is:
_ Instantiate the required mpbToSwitch class with all the required attributes for it's use, this will be the **underlying momentary push button**.  
_ Configure the rest of the mpbToSwitch object attributes needed by using the class methods provided. Don't use the `.begin()` method as the switch will start the underlying mpbToSwitch object when needed.
_ Use the mpbToSwitch object as argument of the HILSwitches object as per needed.  

### The library implements the following switches outputs: ###  
* **Debounced Delayed Switch**  
* **Time Voidable Switch**
* **Staircase Timer Switch**
* **Hinted Time Voidable Security Switch**
* **Guarded Switch**


### **Debounced Delayed Switch**
The **DbncdDlydSwitch** class uses the logic implemented in the **DbncdDlydMPBttn** class of the mpbToSwitch library. The resulting switch is a simple switch that rises it's output pin while the mpb is being pressed, after the debouncing process and the setted delay time is completed.  

### **Time Voidable Switch**
The **TmVdblSwitch** class uses the logic implemented in the **TmVdblMPBttn** class of the mpbToSwitch library. The resulting switch is a simple switch that rises it's output pin while the mpb is being pressed, after the debouncing process and the setted delay time is completed, and until the configured void time is reached, then the output pin es lowered and the mpb has to be released to reset the void condition.    

**Staircase Timer Switch**
The **StrcsTmrSwitch** class uses the logic implemented in the **HntdTmLtchMPBttn** class of the mpbToSwitch library.  

* **Hinted Time Voidable Security Switch**
The **HntdTmVdblScrtySwitch** class uses the logic implemented in the **TmVdblMPBttn** class of the mpbToSwitch library.  

* **Guarded Switch**
The **GrddSwitch** class uses the logic implemented in the **DbncdDlydMPBttn** class of the mpbToSwitch library.  
