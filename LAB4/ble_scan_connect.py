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

def blink(led, ch):
    if led==False:
        ch.write(b'1')
        led = True
    else:
        ch.write(b'0')
        led = False
    return led


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
dev.setDelegate(serviceDelegate())
for service in dev.services:
    print(str(service))

try:
    BtnService= dev.getServiceByUUID(UUID(0xa000))
    ch_Btn = dev.getCharacteristics(uuid=UUID(0xa001))[0]

    LEDService= dev.getServiceByUUID(UUID(0xa002))
    ch_LED = dev.getCharacteristics(uuid=UUID(0xa003))[0]
 
    print(ch_Btn.valHandle)
    cccd = ch_Btn.valHandle + 1
    
    if NOTIFIABLE:
        dev.writeCharacteristic(cccd, bytes([1, 0])) 
        print("Enable notifications")
    if INDICATIBLE:
        dev.writeCharacteristic(cccd, bytes([2, 0]))
        print("Enable indications")
    
    led = False
    while True:
        led=blink(led, ch_LED)
        if dev.waitForNotifications(1.0):
            # handleNotification() was called
            continue
        print("Waiting...")

finally:
    dev.disconnect()