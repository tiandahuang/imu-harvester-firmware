/**
 * application specific accelerometer driver for BMA400
 */

#pragma once

#include "app_common.h"

int accelerometer_init(void);
void accelerometer_wake(void);
void accelerometer_sleep(void);
void accelerometer_get_data(uint8_t **data_ptr, uint16_t *data_len);
