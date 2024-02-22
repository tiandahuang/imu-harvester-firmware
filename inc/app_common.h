/**
 * common includes and configurations
*/

#pragma once

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include "nordic_common.h"
#include "app_util.h"
#include "app_timer.h"


// config

// GPIO config
#define GPIO_V_STORE_DIV_IN     4           // capacitor voltage
#define GPIO_DIV_EN             5           // enable capacitor voltage divider

#define SPI_SCK                 9
#define SPI_MISO                10
#define SPI_MOSI                12

#define IMU_CS                  6
#define IMU_INT1                14
#define IMU_INT2                15

// test config
#define POWER_PROFILING                     // enable power profiling -- runs IMU sample and BLE send on loop

// BLE config
#define DEVICE_NAME             "test"      // device name in BLE advertising
#define ENABLE_DEVICE_NAME      1           // enable device name in advertising
#define APP_ADV_INTERVAL        3200        // advertising interval (in units of 0.625 ms)
#define APP_ADV_DURATION        1000        // advertising duration (in units of 10 milliseconds)
#define MIN_CONN_INTERVAL MSEC_TO_UNITS(8, UNIT_1_25_MS)    /**< Minimum acceptable connection interval (20 ms), Connection interval uses 1.25 ms units. */
#define MAX_CONN_INTERVAL MSEC_TO_UNITS(10, UNIT_1_25_MS)    /**< Maximum acceptable connection interval (75 ms), Connection interval uses 1.25 ms units. */
#define SLAVE_LATENCY 150                                      /**< Slave latency. */
#define CONN_SUP_TIMEOUT MSEC_TO_UNITS(4000, UNIT_10_MS)     /**< Connection supervisory timeout (4 seconds), Supervision Timeout uses 10 ms units. */
#define FIRST_CONN_PARAMS_UPDATE_DELAY APP_TIMER_TICKS(50) /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY APP_TIMER_TICKS(30000) /**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT 3                       /**< Number of attempts before giving up the connection parameter negotiation. */

