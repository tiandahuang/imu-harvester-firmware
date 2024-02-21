/**
 * function calls for event triggers
 */

#include "app_callbacks.h"
#include "app_debug.h"
#include "app_ble_nus.h"

// BLE events

#ifndef POWER_PROFILING

// NUS connected -- start sampling IMU
CALLBACK_DEF(BLE_NUS_EVT_CONNECTED) {
    debug_log("NUS connected");
    ret_code_t err_code = 0;
    APP_ERROR_CHECK(err_code);
}

// NUS notifications enabled -- send data
CALLBACK_DEF(BLE_NUS_EVT_COMM_STARTED) {
    debug_log("NUS notifications enabled");
    ret_code_t err_code;
    err_code = ble_send("hello world", sizeof("hello world") - 1);
    if (err_code != NRF_ERROR_INVALID_STATE) {
        APP_ERROR_CHECK(err_code);
    }
}

#else   // POWER_PROFILING

// NUS connected -- do nothing here
CALLBACK_DEF(BLE_NUS_EVT_CONNECTED) {
    debug_log("NUS connected");
}

// NUS notifications enabled -- start sending on clock
CALLBACK_DEF(BLE_NUS_EVT_COMM_STARTED) {
    debug_log("NUS notifications enabled");
    // ret_code_t err_code;
    // err_code = ble_send("hello world", sizeof("hello world") - 1);
    // if (err_code != NRF_ERROR_INVALID_STATE) {
    //     APP_ERROR_CHECK(err_code);
    // }
}

#endif

