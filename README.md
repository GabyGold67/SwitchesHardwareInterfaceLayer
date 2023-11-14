# **Switches Hardware Interface Layer** Library (switchHIL)
## An Arduino-ESP32 RTOS library that builds physical electric switches outputs using the mpbToSwitch library for logic simulated mechanisms.  

The general implementation working path is:  
_ Instantiate the required mpbToSwitch class with all the required attributes for it's use, this will be the **underlying momentary push button**.  
_ Configure the rest of the mpbToSwitch object attributes needed by using the class methods provided. There's no need to use the `.begin()` method as the switch will start the underlying mpbToSwitch object when needed.  
_ Use the mpbToSwitch object/s created as argument/s for the HILSwitches classes instantiation as needed.  

---  

### The library implements the following switches outputs: ###  
* **Debounced Delayed Switch**  
* **Time Voidable Switch**
* **Staircase Timer Switch**
* **Hinted Time Voidable Security Switch**
* **Guarded Switch**


### **Debounced Delayed Switch**
The **DbncdDlydSwitch** class uses the logic implemented in the **DbncdDlydMPBttn** class of the mpbToSwitch library. The resulting switch is a simple switch that rises it's output pin while the mpb is being pressed (after the debouncing process and the setted delay time is completed). The output pin is lowered as soon as the mpb is released.  

### **Time Voidable Switch**
The **TmVdblSwitch** class uses the logic implemented in the **TmVdblMPBttn** class of the mpbToSwitch library. The resulting switch is a simple switch that rises it's output pin while the mpb is being pressed (after the debouncing process and the setted delay time is completed) and until the mpb is released OR a configured void time is reached, then the output pin is lowered. In the case of the switch being voided by the time counter, the mpb has to be released to reset the void condition.  

### **Staircase Timer Switch**
The **StrcsTmrSwitch** class uses the logic implemented in the **HntdTmLtchMPBttn** class of the mpbToSwitch library. The resulting switch is a replacement of the staircase timer switch, that raises it's output pin since the mpb is pressed (after the debouncing process and the setted delay time is completed), and until a preset time is completed. The switch can be modified to let a new press before the timeout to reset the timer count by changing the attributes. Two other pins might be set: the first to give a warning that the timer is running out of time, and a second one to control a pilot signal that the main pin is low. The timing for the warning signal to kick in is a setteable attribute given as a percentage of the total time given to the swuitch to keep the main output pin on. In a staircase application the main pin is the staircase lighting, the hint is a in-button on beside button light or sound emmiter that signals the running out of time condition, and a hint might be a low power led giving some dim lighting to avoid accidents. This implementation gives the option to use the same or different pins for the warning and the hint outputs. Other usual uses of this kind of switch includes lighting an accesory room from outside, leaving the warning inside that room and the hint besides the mpb to show the lighting condition inside the room.  

### **Hinted Time Voidable Security Switch**
The **HntdTmVdblScrtySwitch** class uses the logic implemented in the **TmVdblMPBttn** class of the mpbToSwitch library. This kind of switch replaces and extends the functionality of the industrial safety suitches used to activate or trigger dangerous machinery, having to keep them pressed to ensure physical (specially limbs) security. These kind of suitches, when correctly implemented, must be reliable and give the tools to avoid tampering. In some indsutrial machines they have to be configurable for different uses of the machines, including the voiding times and the possibility of bypassing some of them for certain machine uses (not all hands and feet might be needed to be pressing all the switches for all activities, for example). All this is achievable in this implementation through attribute settings. The switch also provides outputs to display the switch status, including the not pressed, pressed ok, pressed voided and bypassed situations.  

### **Guarded Switch**
The **GrddSwitch** class uses the logic implemented in the **DbncdDlydMPBttn** class of the mpbToSwitch library, **two** are needed for this switch. The first DbncdDlydMPBttn -the guard button- when pressed, enables de second DbncdDlydMPBttn -the guarded button-, that will activate the switch output when pressed. Failing to press the guard button, or releasing it, will disable the guarded button ability to switch the output. As the first mpb can be replaced by different validators (electric keylock, NFC devices or tags, fingerprints scanners, etc), this results in a protected **Debounce Delayed Switch**.  
