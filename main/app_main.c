//#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
//#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_mac.h"

#include "lwip/err.h"
#include "lwip/sys.h"

//---- moji includeovi ----
#include "wifi.h"

static void _event_handler(void *p_arg, esp_event_base_t event_base, int32_t event_id, void *p_event_data);

static esp_err_t _nvs_init(void);

static const char *TAG = "APP_MAIN";

void app_main(void)
{
    // Initialize NVS
    esp_err_t err = ESP_OK;

    /* Initialize NVS partition */
    err = _nvs_init();

    if(ESP_OK == err)
    {
        ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");
        err = _wifi_init_softap();
    }

    if(ESP_OK != err)
    {
        ESP_LOGE(TAG, "Init failed");
    }

    vTaskSuspend(NULL);
}

static esp_err_t _nvs_init(void)
{
    esp_err_t ret = nvs_flash_init();
    if((ESP_ERR_NVS_NO_FREE_PAGES == ret) || (ESP_ERR_NVS_NEW_VERSION_FOUND == ret))
    {
        /* NVS partition was truncated
         * and needs to be erased */
        ret = nvs_flash_erase();

        /* Retry nvs_flash_init */
        ret |= nvs_flash_init();
    }
    return ret;
}