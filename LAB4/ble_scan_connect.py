from bluepy.btle import Peripheral, UUID
from bluepy.btle import Scanner, DefaultDelegate

class ScanDelegate(DefaultDelegate):
    def __init__(self):
        DefaultDelegate.__init__(self)
    def handleDiscovery(self, dev, isNewDev, isNewData):
        if isNewDev:
            print("Discovered device", dev.addr)
        elif isNewData:
            print("Received new data from", dev.addr)
               
class ServiceDelegate(DefaultDelegate):
    def __init__(self):
        DefaultDelegate.__init__(self)
    def handleNotification(self, ch, notificationData):
        print("Notification received: %s"% notificationData)

def blink(led, ch):
    if not led:
        ch.write(b'1')
    else:
        ch.write(b'0')
    
    return not led

scanner = Scanner().withDelegate(ScanDelegate())
devices = scanner.scan(10.0)
n=0
addr = []
for dev in devices:
    print("%d: Device %s (%s), RSSI=%d dB" % (n, dev.addr, dev.addrType, dev.rssi))
    addr.append(dev.addr)
    n += 1

    for (adtype, desc, value) in dev.getScanData():
        print("  %s = %s" % (desc, value))
        
number = input('Enter your device number: ')
number = int(number)
print('Device', number)
print(addr[number])

print("Connecting...")
dev = Peripheral(list(devices)[number].addr, 'random')

dev.setDelegate(ServiceDelegate())
print("Services connected: ")
for service in dev.services:
    print(str(service))

# Btn service
BtnService= dev.getServiceByUUID(UUID(0xAAAA))
for ch in BtnService.getCharacteristics():
    print(str(ch))
    
ch_Btn = dev.getCharacteristics(uuid=UUID(0xBBBB))[0]
print("Button service...")
print(ch_Btn.read())

# LED service
LEDService= dev.getServiceByUUID(UUID(0xCCCC))
for ch in LEDService.getCharacteristics():
    print(str(ch))
    
ch_LED = dev.getCharacteristics(uuid=UUID(0xDDDD))[0]
print("LED service...")
print(ch_LED.read())



while True:
    led=blink(False, ch_LED)
    if dev.waitForNotifications(1.0):
        continue
    print("Waiting for notification...")

dev.disconnect()