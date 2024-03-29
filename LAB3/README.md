# Intro
This project implement a simple usage of bluetooth on RPi by the python module bluepy.

# Setup
## 1. open BLE tool and start advertising
Download BLE tool on your android system and start advertising. 

## 2. start scanning the bluetooth device on RPi
Run `ble_scan_connect.py`. User can see the bluetooth devices which are able to connect.

## 3. find the target device
On the list of bluetooth devices, find out the local device which is running the BLE tool.

## 4. check the receive message on RPi terminal
Once connected, RPi can receive the encoded message sent by BLE tool.

## 5. handle the CCCD by RPi
By using the function `writeCharacteristic()`, we can modify the CCCD of specific uuid.

