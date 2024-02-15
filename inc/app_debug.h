/**
 * wrapper for NRF LOG and other debug functionality
*/

#pragma once

#include "app_common.h"
#include "nrf_log.h"

void debug_init(void);
bool debug_process(void);
void debug_flush(void);
#define debug_log(...)  NRF_LOG_RAW_INFO(__VA_ARGS__)
