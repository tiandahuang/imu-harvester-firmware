/**
 * BLE NUS functions
 */

#pragma once

#include "sdk_errors.h"

void ble_all_services_init(void);
void advertising_start(void);
ret_code_t ble_send(uint8_t *data, uint16_t length);
