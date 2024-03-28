# 1
copy folder "BSP_B-L475E-IOT01" under lib "DISCO_L475VG_IOT01-Sensors-BSP" in lecture slide "STM32 IoT Node Onboard Sensors"
# 2
modify main.cpp, mbed_app.json of example "mbed-os-example-sockets"

# 3 
modify ip, port

# 4 enable printf’s floating format support
Modify targets.json under "Mbed Programs/DISCO_L475VG_IOT01-Sensors-BSP/mbed-os/targets"
change "printf_lib": "minimal-printf" -> "printf_lib": "std"

