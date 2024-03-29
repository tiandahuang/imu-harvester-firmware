/**
 * application specific accelerometer driver for BMA400
 */

#pragma once

#include "app_common.h"

#define ACCELEROMETER_N_SAMPLES   18  // for some reason this results in 16 valid samples

int accelerometer_init(void);
void accelerometer_wake(void);
void accelerometer_sleep(void);
uint16_t accelerometer_fetch_data(void);
void accelerometer_copy_data(uint8_t *data_ptr, uint16_t data_len);
