# Intro
This project implement a simple ble button service with IOT01A & RPi 

# Setup
## 1. file structure
There are 6 files in the repo which is mbed_app.json, main.cpp, ButtonService.h, SensorService.h, pretty_printer.h, ble_scan_connect.py 

## 2. replace the code in the right directory
* Mbed-os
    * Import the GattService example program.
    * Replace mbed_app.json
    * Replace the source file
* RPi
    * Place ble_scan_connect.py into RPi

## 3. build and run the application on mbed-os
Now the mbed-os is advertising. We can now connect to the device.

## 4. run ble_scan_connect.py
Run `ble_scan_connect.py` and find the device name of mbed-os, which is set in main.cpp.

## 5. Connection complete & press the button
After connecting the device, we can press the button on STM32L4, and as we check the terminal in RPi, we can see the sequence of message being sent to RPi.

