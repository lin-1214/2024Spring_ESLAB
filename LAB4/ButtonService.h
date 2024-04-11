/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __BLE_BUTTON_SERVICE_H__
#define __BLE_BUTTON_SERVICE_H__


class ButtonService {
public:
    const static uint16_t UUID_BUTTON_SERVICE = 0xA000;
    const static uint16_t UUID_BUTTON_STATE_CHARACTERISTIC = 0xA001;
    int counter;
    ButtonService(BLE &_ble, bool buttonPressedInitial) :
        ble(_ble), buttonState(UUID_BUTTON_STATE_CHARACTERISTIC, &buttonPressedInitial, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY)
    {
        GattCharacteristic *charTable[] = {&buttonState};
        GattService         buttonService(ButtonService::UUID_BUTTON_SERVICE, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));
        ble.gattServer().addService(buttonService);
        counter = 0;
    }

    void updateButtonState(bool newState) {
        char message[] = {'h', 'e', 'l', 'l', 'o', 't', 'h', 'e', 'r', 'e'};
        for(int i=0; i<10; ++i) {
            message[i] = int(message[i]);
        }
        ble.gattServer().write(buttonState.getValueHandle(), (uint8_t *)(message+(counter%10)), sizeof(bool));
        ++counter;
    }

private:
    BLE                              &ble;
    ReadOnlyGattCharacteristic<bool>  buttonState;
};

#endif /* #ifndef __BLE_BUTTON_SERVICE_H__ */
