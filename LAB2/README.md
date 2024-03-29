# Intro
This project use mbed-OS and python 3.12. We use `B-L475E-IOT01` to collect temperature, pressure, humidity, magneto, gyro, accelero sensor values, transmitting these data using json format through wifi to server.

# Setup
## 1. start new example
start a new example of "mbed-os-example-sockets" using mbed-OS.
## 2. add proper library
copy folder "BSP_B-L475E-IOT01" under "DISCO_L475VG_IOT01-Sensors-BSP" in lecture slide "STM32 IoT Node Onboard Sensors"
## 3. modfiy file
override `source/main.cpp` and `mbed_app.json` of example "mbed-os-example-sockets" with the one in this folder
# 4. modfiy Internet related parameters
### In `mbed_app.json`
Modify ssid, password to your wifi. Modify hostname's value to host's ip.
### In `source/main.cpp`
Modify `static constexpr size_t REMOTE_PORT = 4000` to host's port
### In `server.py`
Modify `HOST` to host's ip.
## 5. enable printf’s floating format support
Modify targets.json under "Mbed Programs/DISCO_L475VG_IOT01-Sensors-BSP/mbed-os/targets"
change "printf_lib": "minimal-printf" -> "printf_lib": "std"
## 6. run server on the host
run `python3 server.py`
This will receive and plot data using `matplotlib`.
## 7. write file to `B-L475E-IOT01`
it will first attempt to connect the wifi you provide and start sending sensors' data.
