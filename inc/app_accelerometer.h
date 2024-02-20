
#pragma once

#include <stdint.h>
#include "bma400.h"
#include "bma400_defs.h"

#define N_FRAMES_FULL                       1024
#define FIFO_SIZE_FULL                      (N_FRAMES_FULL + BMA400_FIFO_BYTES_OVERREAD)
#define ACCELEROMETER_NUM_SAMPLES           64
#define ACCELEROMETER_BUFFER_SIZE           (ACCELEROMETER_NUM_SAMPLES * 3)
extern int16_t accelerometer_buffer[ACCELEROMETER_BUFFER_SIZE];

int accelerometer_init(void);
void accelerometer_power_on(void);
void accelerometer_power_off(void);
void accelerometer_read(void * p_event_data, uint16_t event_size);
