# **Switches Hardware Interface Layer** Library (switchHIL)
## An Arduino-ESP32 library that builds physical electric switches outputs using the mpbToSwitch library for logic simulated mechanisms.  

### The library implements -at this moment- the following switches interfaces layers: ###  
* **Debounced Delayed Switch**  
*  **Staircase Timer Switch**




**Debounced Delayed Switch**
The DbncdDlydSwitch class uses the logic implemented in the DbncdDlydMPBttn class of the mpbToSwitch library.

**Staircase Timer Switch**
The StrcsTmrSwitch class uses the logic implemented in the HntdTmLtchMPBttn class of the mpbToSwitch library.