/**
 * Copyright (c) 2014 - 2021, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "app_common.h"
#include "nrf.h"
#include "nrf_pwr_mgmt.h"
#include "nrfx_gpiote.h"

#include "app_timer.h"
#include "app_scheduler.h"

#include "app_ble_nus.h"
#include "app_spi.h"
#include "app_accelerometer.h"

#define APP_SCHED_EVENT_SIZE    APP_TIMER_SCHED_EVENT_DATA_SIZE
#define APP_SCHED_QUEUE_SIZE    10

/**@brief Function for assert macro callback.
 *
 * @details This function will be called in case of an assert in the SoftDevice.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyse
 *          how your product is supposed to react in case of Assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in] line_num    Line number of the failing ASSERT call.
 * @param[in] p_file_name File name of the failing ASSERT call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t *p_file_name) {
    app_error_handler(42, line_num, p_file_name);
}

/**@brief Function for handling the idle state (main loop).
 *
 * @details If there is no pending log operation, then sleep until next the next event occurs.
 */
static void idle_state_handle(void) {
    if (debug_process() == false) {
        nrf_pwr_mgmt_run();
    }
}

#if POWER_PROFILING_ENABLED
static void start_sampling_accel(void *p_event_data, uint16_t event_size) {
    debug_log("accelerometer set to wake");
    accelerometer_wake();
    // debug_log("started advertising");
    // advertising_start();
}

APP_TIMER_DEF(m_repeated_timer_id);
void timer_handler(void *p_context) {
    app_sched_event_put(NULL, 0, start_sampling_accel);
}
#endif

/**@brief Application main function.
 */
int main(void) {

    // Initialize.
    nrf_pwr_mgmt_init();
    debug_init();
    nrfx_gpiote_init();
    app_spi_init();
    APP_SCHED_INIT(APP_SCHED_EVENT_SIZE, APP_SCHED_QUEUE_SIZE);

    accelerometer_init();

    ble_all_services_init();
    app_timer_init();
    

    #if POWER_PROFILING_ENABLED
    
    ret_code_t err_code;
    err_code = app_timer_create(&m_repeated_timer_id,
                                APP_TIMER_MODE_REPEATED,
                                timer_handler);
    err_code = app_timer_start(m_repeated_timer_id,
                               APP_TIMER_TICKS(2000),
                               NULL);
    #endif
    
    // Start execution.
    debug_log("Debug logging over RTT started.");
    advertising_start();

    // Enter main loop.
    for (;;) {
        app_sched_execute();
        idle_state_handle();
    }
}

/**
 * @}
 */
