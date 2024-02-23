/**
 * function calls for event triggers
 */

#include "app_callbacks.h"
#include "app_debug.h"
#include "app_ble_nus.h"
#include "app_accelerometer.h"
#include "nrf_pwr_mgmt.h"

// global buffers for sharing data

uint8_t accelerometer_data_buf[ACCELEROMETER_N_SAMPLES * 6] = { 0 };
uint16_t accelerometer_num_data = 0;

// BLE events

static volatile bool send_pend = false;
static volatile bool send_ready = false;

#if POWER_PROFILING_ENABLED == 0
#pragma message "power profiling disabled -- running in one shot sampling mode"

// NUS connected -- start sampling IMU
CALLBACK_DEF_APP_SCHED(BLE_GAP_EVT_CONNECTED) {
    debug_log("NUS connected. Waking accelerometer.");
    accelerometer_wake();
}

// NUS notifications enabled -- send data
CALLBACK_DEF_APP_SCHED(BLE_NUS_EVT_COMM_STARTED) {
    debug_log("NUS notifications enabled");
    if (send_pend) {    // notifications enabled after watermark interrupt
        debug_log("notifications enabled, sending pending data");
        send_pend = false;
        ble_send(accelerometer_data_buf, accelerometer_num_data);
    }
    send_ready = true;
}

// Accelerometer watermark interrupt raised
CALLBACK_DEF_APP_SCHED(ACCELEROMETER_DATA_READY) {
    accelerometer_num_data = accelerometer_fetch_data();
    accelerometer_copy_data(accelerometer_data_buf, accelerometer_num_data);
    debug_log("ACCELEROMETER_DATA_READY: %d", accelerometer_num_data);
    accelerometer_sleep();

    if (send_ready) {   // notifications enabled before watermark interrupt
        ble_send(accelerometer_data_buf, accelerometer_num_data);
    }
    else {              // notifications not yet enabled -- pend a send
        debug_log("notifications not yet enabled, pending send");
        send_pend = true;
    }
}

// NUS disconnected -- reset
CALLBACK_DEF_APP_SCHED(BLE_GAP_EVT_DISCONNECTED) {
    debug_log("NUS disconnected. Resetting.");
    // send_pend = false;
    // send_ready = false;
    accelerometer_sleep();
    // nrf_pwr_mgmt_shutdown(NRF_PWR_MGMT_SHUTDOWN_GOTO_SYSOFF);
}

#else   // POWER_PROFILING_ENABLED == 1
#pragma message "power profiling enabled -- running in looped sampling mode"

// NUS connected -- do nothing here
CALLBACK_DEF_APP_SCHED(BLE_NUS_EVT_CONNECTED) {
    debug_log("NUS connected");
}

// NUS notifications enabled
CALLBACK_DEF_APP_SCHED(BLE_NUS_EVT_COMM_STARTED) {
    debug_log("NUS notifications enabled");
    send_ready = true;
}

// NUS disconnected -- reset
CALLBACK_DEF_APP_SCHED(BLE_GAP_EVT_DISCONNECTED) {
    debug_log("NUS disconnected. Resetting.");
    send_ready = false;
}

// Accelerometer watermark interrupt raised
CALLBACK_DEF_APP_SCHED(ACCELEROMETER_DATA_READY) {
    accelerometer_num_data = accelerometer_fetch_data();
    accelerometer_copy_data(accelerometer_data_buf, accelerometer_num_data);
    debug_log("ACCELEROMETER_DATA_READY: %d", accelerometer_num_data);
    accelerometer_sleep();

    if (send_ready) {
        ble_send(accelerometer_data_buf, accelerometer_num_data);
    }
}

#endif

