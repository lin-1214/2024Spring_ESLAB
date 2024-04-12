from bluepy.btle import Peripheral, UUID
from bluepy.btle import Scanner, DefaultDelegate
import sys
import select
import time

HEART_RATE_SERVICE_UUID = UUID("180D")
HEART_RATE_MEASUREMENT_CHAR_UUID = UUID("2A37")
BUTTON_SERVICE_UUID = UUID("A000")
BUTTON_STATE_CHARACTERISTIC_UUID = UUID("A001")

class MyDelegate(DefaultDelegate):
    def __init__(self, heart_rate_handle, button_handle):
        DefaultDelegate.__init__(self)
        self.heart_rate_handle = heart_rate_handle
        self.button_handle = button_handle

    def handleNotification(self, cHandle, data):
        if cHandle == self.heart_rate_handle:
            # 解析心率值
            bpm = int.from_bytes(data, byteorder='little')
            print(f"Heart Rate Notification: {bpm} bpm")
        elif cHandle == self.button_handle:
            # 解析按钮状态
            state = "Pressed" if int.from_bytes(data, byteorder='little') == 1 else "Released"
            print(f"Button Notification: {state}")

def enable_notifications(peripheral, char_uuid):
    ch = peripheral.getCharacteristics(uuid=char_uuid)[0]
    print(f"Found characteristic {char_uuid}")

    cccd = ch.getDescriptors(forUUID=UUID(0x2902))[0]
    print(f"Found CCCD descriptor for characteristic {char_uuid}")

    notification_enable_value = bytes([0x01, 0x00])
    cccd.write(notification_enable_value, withResponse=True)
    print(f"Successfully wrote to CCCD to enable notifications for {char_uuid}")
    return ch.getHandle()

scanner = Scanner()
devices = scanner.scan(10.0)
n = 0
addr = []
for dev in devices:
    print(f"{n}: Device {dev.addr} ({dev.addrType}), RSSI={dev.rssi} dB")
    addr.append(dev.addr)
    n += 1
    for (adtype, desc, value) in dev.getScanData():
        print(f" {desc} = {value}")
number = input('Enter your device number: ')
print('Device', number)
num = int(number)
print(addr[num])

print("Connecting...")
dev = Peripheral(addr[num], 'random')


hr_handle = enable_notifications(dev, HEART_RATE_MEASUREMENT_CHAR_UUID)
btn_handle = enable_notifications(dev, BUTTON_STATE_CHARACTERISTIC_UUID)

dev.setDelegate(MyDelegate(hr_handle, btn_handle))

try:
    print("Waiting for notifications...")
    while True:
        dev.waitForNotifications(1.0)
except Exception as e:
    print(f"An error occurred: {e}")
finally:
    print("Disconnecting...")
    dev.disconnect()