#ifndef __BLE_MAGNETOMETER_SERVICE_H__
#define __BLE_MAGNETOMETER_SERVICE_H__

#include "ble/BLE.h"


/**
 * @class MagnetometerService
 * @brief BLE Magnetometer Service. This service provides 3D magnetometer data
 */
const UUID MAGNETOMETER_SERVICE_UUID("735882c4-5eeb-48b0-a833-156a115be57c");
const UUID MAGNETOMETER_X_CHAR_UUID("5da74ef8-a228-4c00-b91e-f87ef3f6ffe4");
const UUID MAGNETOMETER_Y_CHAR_UUID("c75c842a-791c-4dc1-b15e-5ed394defae2");
const UUID MAGNETOMETER_Z_CHAR_UUID("439d09a2-f383-44df-a86d-5f2a0ada9d05");

const static char* MAGNETOMETER_SERVICE_ADDED = "Magnetometer service added\r\n";
const static char* MAGNETOMETER_SERVICE_ERROR = "Error adding magnetometer service\r\n";

class MagnetometerService {
public:
    typedef int16_t MagnetometerDataType_t; // Using float for magnetometer data

    /**
     * @brief   MagnetometerService constructor.
     * @param   ble Reference to BLE device.
     * @param   temperature_en Enable this characteristic.
     */
    MagnetometerService(BLE& _ble) :
        ble(_ble),
        xCharacteristic(MAGNETOMETER_X_CHAR_UUID, (uint8_t *)&x, sizeof(x), sizeof(x),
                        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY),
        yCharacteristic(MAGNETOMETER_Y_CHAR_UUID, (uint8_t *)&y, sizeof(y), sizeof(y),
                        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY),
        zCharacteristic(MAGNETOMETER_Z_CHAR_UUID, (uint8_t *)&z, sizeof(z), sizeof(z),
                        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY)
    {
        static bool serviceAdded = false;
        if (!serviceAdded) {
            GattCharacteristic *charTable[] = { &xCharacteristic, &yCharacteristic, &zCharacteristic };
            GattService magnetometerService(MAGNETOMETER_SERVICE_UUID, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));
            
            ble_error_t addServiceResult = ble.gattServer().addService(magnetometerService);

            if (addServiceResult == BLE_ERROR_NONE) {
                // If the service has been successfully added, print a debug message
                printf(MAGNETOMETER_SERVICE_ADDED);
                serviceAdded = true; // Set the flag to true to avoid adding the service again
            } else {
                // If there was an error adding the service, print an error message with the error code
                printf(MAGNETOMETER_SERVICE_ERROR);
                printf("Error code: %u\r\n", addServiceResult);
            }
        }
    }

    /**
     * @brief   Update magnetometer data characteristic.
     * @param   newX New X-axis magnetometer data.
     * @param   newY New Y-axis magnetometer data.
     * @param   newZ New Z-axis magnetometer data.
     */
    void updateMagnetometerData(MagnetometerDataType_t newX, MagnetometerDataType_t newY, MagnetometerDataType_t newZ)
    {
        x = (MagnetometerDataType_t) (newX);
        y = (MagnetometerDataType_t) (newY);
        z = (MagnetometerDataType_t) (newZ);

        ble.gattServer().write(xCharacteristic.getValueHandle(), (uint8_t *)&x, sizeof(x), true);
        ble.gattServer().write(yCharacteristic.getValueHandle(), (uint8_t *)&y, sizeof(y), true);
        ble.gattServer().write(zCharacteristic.getValueHandle(), (uint8_t *)&z, sizeof(z), true);
    }

private:
    BLE& ble;

    MagnetometerDataType_t x, y, z;

    GattCharacteristic xCharacteristic;
    GattCharacteristic yCharacteristic;
    GattCharacteristic zCharacteristic;
    
};

#endif /* #ifndef __BLE_MAGNETOMETER_SERVICE_H__ */ 