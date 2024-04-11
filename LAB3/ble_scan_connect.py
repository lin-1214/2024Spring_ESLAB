from bluepy.btle import Peripheral, UUID
from bluepy.btle import Scanner, DefaultDelegate

NOTIFIABLE = False
INDICATIBLE = True

class ScanDelegate(DefaultDelegate):
    def __init__(self):
        DefaultDelegate.__init__(self)

    def handleDiscovery(self, dev, isNewDev, isNewData):
        if isNewDev:
            print("Discovered device", dev.addr)
        elif isNewData:
            print("Received new data from", dev.addr)


class serviceDelegate(DefaultDelegate):
    def __init__(self):
        DefaultDelegate.__init__(self)

    def handleNotification(self, ch, data):
        print("Received notification: %s" % data)


scanner = Scanner().withDelegate(ScanDelegate())
devices = scanner.scan(10.0)
n = 0
addr = []
for dev in devices:
    print("%d: Device %s (%s), RSSI=%d dB" %
          (n, dev.addr, dev.addrType, dev.rssi))
    addr.append(dev.addr)
    n += 1
    for (adtype, desc, value) in dev.getScanData():
        print("  %s = %s" % (desc, value))

number = input('Enter your device number: ')
number = int(number)
print('Device', number)
print(list(devices)[number].addr)

print("Connecting...")
dev = Peripheral(list(devices)[number].addr, 'random')
dev.setDelegate(myDelegate())

for service in dev.services:
    print(str(svc))

try:
    service = dev.getServiceByUUID(UUID("0000fff0-0000-1000-8000-00805f9b34fb"))
    for ch in service.getCharacteristics():
        print(str(ch))

    ch = dev.getCharacteristics(uuid=UUID("0000fff2-0000-1000-8000-00805f9b34fb"))[0]

    print(ch.valHandle)
    cccd = ch.valHandle + 1

    if NOTIFIABLE:
        dev.writeCharacteristic(cccd, bytes([1, 0]))
        print("Enable notifications!")
    if INDICATIBLE:
        dev.writeCharacteristic(cccd, bytes([2, 0]))
        print("Enable indications!")

    while True:
        if dev.waitForNotifications(1.0):
            continue
        print("Waiting...")

finally:
    dev.disconnect()