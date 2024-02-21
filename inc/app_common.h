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
#define APP_ADV_INTERVAL        64          // advertising interval (in units of 0.625 ms)
#define APP_ADV_DURATION        18000       // advertising duration (in units of 10 milliseconds)
