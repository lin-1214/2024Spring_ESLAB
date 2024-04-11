# Intro
This project implement a simple ble bbutton service with IOT01A & RPi 

# Setup
## 1. file structure
There are 7 files in the repo which is mbed_app.json, main.cpp, ButtonService.h, LEDService.h, GattService.h, pretty_printer.h, ble_scan_connect.py 

## 2. replace the code in the right directory
* Mbed-os
    * Replace mbed_app.json and source/main.cpp
    * Add pretty_printer.h to source/
    * Add GattService.h to mbed-os/connectivity/FEATURE_BLE/include/ble/gatt/
    * Add ButtonService.h & LEDService.h to mbed-os/connectivity/FEATURE_BLE/include/ble/services/
* RPi
    * Place ble_scan_connect.py into RPi

## 3. build and run the application on mbed-os
Now the mbed-os is advertising. We can also check whether the LED is blinking on STM32L4.

## 4. run ble_scan_connect.py
Run `ble_scan_connect.py` and find the device name of mbed-os, which is set in main.cpp.

## 5. Connection complete & press the button
After connecting the device, we can press the button on STM32L4, and as we check the terminal in RPi, we can see the sequence of number being sent to RPi.

