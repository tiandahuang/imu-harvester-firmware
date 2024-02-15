/**
 * wrapper for NRF LOG and other debug functionality
*/

#pragma once

#include "app_common.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

void debug_init(void);
bool debug_process(void);
void debug_flush(void);
#define debug_log(...)  \
        do { \
            NRF_LOG_INFO(__VA_ARGS__); \
        } while (0)
