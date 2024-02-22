
#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#pragma message "Linked App Config"

#ifndef VSCODE_EDITING
#define NRF_LOG_ENABLED 1
#else   // in vscode
#define NRF_LOG_ENABLED 0
#endif

#endif	// APP_CONFIG_H



