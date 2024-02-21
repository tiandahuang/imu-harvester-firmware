/**
 * function calls for event triggers
 */

#pragma once

#include "app_common.h"
#include "app_debug.h"

// callback function declaration macro
#define WEAK_CALLBACK_DEF(name) \
        __WEAK void callback_##name(void) { debug_log("event: " #name); }
#define CALLBACK_DEF(name) \
        void callback_##name(void)
#define CALLBACK_FUNC(name) \
        callback_##name

typedef void (* callback_t)(void);

