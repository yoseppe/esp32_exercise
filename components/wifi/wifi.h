#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"

#include "esp_mac.h"

#include "lwip/err.h"
#include "lwip/sys.h"

esp_err_t _wifi_init_softap(void);
static void _event_handler(void *p_arg, esp_event_base_t event_base, int32_t event_id, void *p_event_data);