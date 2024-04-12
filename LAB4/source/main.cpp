/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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
#include "mbed.h"
#include <events/mbed_events.h>
#include "ble/BLE.h"
#include "ble/gap/Gap.h"
#include "ble/services/HeartRateService.h"
#include "pretty_printer.h"
#include "mbed-trace/mbed_trace.h"

#include "stm32l475e_iot01_magneto.h"
#include "SensorService.h"
#include "ButtonService.h"

using namespace std::literals::chrono_literals;

const static char DEVICE_NAME[] = "ABCDE";

static events::EventQueue event_queue(/* event count */ 16 * EVENTS_EVENT_SIZE);

class HeartrateDemo : ble::Gap::EventHandler {
public:
    HeartrateDemo(BLE &ble, events::EventQueue &event_queue) :
        _ble(ble),
        _event_queue(event_queue),
        //heartrate
        _heartrate_uuid(GattService::UUID_HEART_RATE_SERVICE),
        _heartrate_value(100),
        _heartrate_service(ble, _heartrate_value, HeartRateService::LOCATION_FINGER),
        //magnetometer
        //_magnetometer_uuid(MAGNETOMETER_SERVICE_UUID),
        //_magnetometer_service(ble),

        //button
        _led1(LED1, 1),
        _button(BLE_BUTTON_PIN_NAME, BLE_BUTTON_PIN_PULL),
        _button_service(NULL),
        _button_uuid(ButtonService::BUTTON_SERVICE_UUID),

        _adv_data_builder(_adv_buffer)
    {
        //_pDataXYZ[0]=0;
        //_pDataXYZ[1]=0;
        //_pDataXYZ[2]=0;
    }

    void start()
    {

        /* this allows us to receive events like onConnectionComplete() */
        _ble.gap().setEventHandler(this);

        _ble.init(this, &HeartrateDemo::on_init_complete);

        _event_queue.call_every(500, this, &HeartrateDemo::blink);

        _event_queue.dispatch_forever();
    }

private:
    /** Callback triggered when the ble initialization process has finished */
    void on_init_complete(BLE::InitializationCompleteCallbackContext *params)
    {
        if (params->error != BLE_ERROR_NONE) {
            printf("Ble initialization failed.");
            return;
        }

        print_mac_address();

        /* Setup primary service. */
        _button_service = new ButtonService(_ble, false /* initial value for button pressed */);

        _button.fall(Callback<void()>(this, &HeartrateDemo::button_pressed));
        _button.rise(Callback<void()>(this, &HeartrateDemo::button_released));

        /* heart rate value updated every second */
        _event_queue.call_every(
            1000ms,
            [this] {
                update_sensor_value();
                //update_magnetometer_value();
            }
        );

        start_advertising();
    }

    void start_advertising()
    {
        /* Create advertising parameters and payload */

        ble::AdvertisingParameters adv_parameters(
            ble::advertising_type_t::CONNECTABLE_UNDIRECTED,
            ble::adv_interval_t(ble::millisecond_t(100))
        );

        _adv_data_builder.setFlags();
        _adv_data_builder.setAppearance(ble::adv_data_appearance_t::GENERIC_HEART_RATE_SENSOR);

        UUID service_uuids[] = {_heartrate_uuid, _button_uuid};
        _adv_data_builder.setLocalServiceList(mbed::make_Span(service_uuids));
        //_adv_data_builder.setLocalServiceList({&_heartrate_uuid, 1});
        //_adv_data_builder.setLocalServiceList(mbed::make_Span(&_button_uuid, 1));
        _adv_data_builder.setName(DEVICE_NAME);


        /* Setup advertising */

        ble_error_t error = _ble.gap().setAdvertisingParameters(
            ble::LEGACY_ADVERTISING_HANDLE,
            adv_parameters
        );

        if (error) {
            printf("_ble.gap().setAdvertisingParameters() failed\r\n");
            return;
        }

        error = _ble.gap().setAdvertisingPayload(
            ble::LEGACY_ADVERTISING_HANDLE,
            _adv_data_builder.getAdvertisingData()
        );

        if (error) {
            printf("_ble.gap().setAdvertisingPayload() failed\r\n");
            return;
        }

        /* Start advertising */

        error = _ble.gap().startAdvertising(ble::LEGACY_ADVERTISING_HANDLE);

        if (error) {
            printf("_ble.gap().startAdvertising() failed\r\n");
            return;
        }

        printf("Heart rate and magnetometer sensor advertising, please connect\r\n");
    }

    void update_sensor_value()
    {
        /* you can read in the real value but here we just simulate a value */
        _heartrate_value++;

        /*  60 <= bpm value < 110 */
        if (_heartrate_value == 110) {
            _heartrate_value = 60;
        }

        _heartrate_service.updateHeartRate(_heartrate_value);
    }

    void button_pressed(void) {
        _event_queue.call(Callback<void(bool)>(_button_service, &ButtonService::updateButtonState), true);
    }

    void button_released(void) {
        _event_queue.call(Callback<void(bool)>(_button_service, &ButtonService::updateButtonState), false);
    }

    void blink(void) {
        _led1 = !_led1;
    }

    //void update_magnetometer_value() {
    //    BSP_MAGNETO_GetXYZ(_pDataXYZ);
    //    _magnetometer_service.updateMagnetometerData(_pDataXYZ[0], _pDataXYZ[1], _pDataXYZ[2]);
    //}

    /* these implement ble::Gap::EventHandler */
private:
    /* when we connect we stop advertising, restart advertising so others can connect */
    virtual void onConnectionComplete(const ble::ConnectionCompleteEvent &event)
    {
        if (event.getStatus() == ble_error_t::BLE_ERROR_NONE) {
            printf("Client connected, you may now subscribe to updates\r\n");
        }
    }

    /* when we connect we stop advertising, restart advertising so others can connect */
    virtual void onDisconnectionComplete(const ble::DisconnectionCompleteEvent &event)
    {
        printf("Client disconnected, restarting advertising\r\n");

        ble_error_t error = _ble.gap().startAdvertising(ble::LEGACY_ADVERTISING_HANDLE);

        if (error) {
            printf("_ble.gap().startAdvertising() failed\r\n");
            return;
        }
    }

private:
    BLE &_ble;
    events::EventQueue &_event_queue;

    UUID _heartrate_uuid;
    //UUID _magnetometer_uuid;

    uint16_t _heartrate_value;
    //int16_t _pDataXYZ[3];

    HeartRateService _heartrate_service;
    //MagnetometerService _magnetometer_service;

    DigitalOut  _led1;
    InterruptIn _button;
    ButtonService *_button_service;

    UUID _button_uuid;

    uint8_t _adv_buffer[ble::LEGACY_ADVERTISING_MAX_SIZE];
    ble::AdvertisingDataBuilder _adv_data_builder;
};

/* Schedule processing of events from the BLE middleware in the event queue. */
void schedule_ble_events(BLE::OnEventsToProcessCallbackContext *context)
{
    event_queue.call(Callback<void()>(&context->ble, &BLE::processEvents));
}


//main program
int main()
{
    mbed_trace_init();

    //sensor init
    //BSP_MAGNETO_Init();

    BLE &ble = BLE::Instance();
    ble.onEventsToProcess(schedule_ble_events);

    HeartrateDemo demo(ble, event_queue);
    demo.start();

    return 0;
}