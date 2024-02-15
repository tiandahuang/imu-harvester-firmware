/**
 * wrapper for NRF LOG and other debug functionality
*/

#include "app_debug.h"
#include "app_error.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

void debug_init(void) {
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

bool debug_process(void) {
    return NRF_LOG_PROCESS();
}

void debug_flush(void) {
    NRF_LOG_FLUSH();
}
