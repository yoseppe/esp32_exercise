//--------------------------------- INCLUDES ----------------------------------
#include "wifi.h"

//---------------------------------- MACROS -----------------------------------
#define EXAMPLE_ESP_WIFI_SSID    ("ESP32_sipjo")
#define EXAMPLE_ESP_WIFI_PASS    ("12345678")
#define EXAMPLE_ESP_WIFI_CHANNEL (1U)
#define EXAMPLE_MAX_STA_CONN     (4U)

esp_err_t _wifi_init_softap(void);

//------------------------- STATIC DATA & CONSTANTS ---------------------------
static const char *TAG = "WIFI";

//---------------------------- PRIVATE FUNCTIONS ------------------------------

esp_err_t _wifi_init_softap(void)
{
    esp_err_t err = ESP_OK;

    if(ESP_OK == err)
    {
        /* Initialize TCP/IP */
        ESP_LOGI(TAG, " Initialize TCP/IP");
        err = esp_netif_init();
    }

    if(ESP_OK == err)
    {
        /* Initialize the event loop */
        ESP_LOGI(TAG, " Initialize the event loop");
        err = esp_event_loop_create_default();
    }

    if(ESP_OK == err)
    {
        /* Create default wifi AP */
        ESP_LOGI(TAG, " Create default wifi AP");
        esp_netif_create_default_wifi_ap();
    }

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    if(ESP_OK == err)
    {
        ESP_LOGI(TAG, " ESP Wifi init");
        err = esp_wifi_init(&cfg);
    }

    if(ESP_OK == err)
    {
        ESP_LOGI(TAG, " Set up event handler");
        err = esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &_event_handler, NULL, NULL);
    }

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .ssid_len = strlen(EXAMPLE_ESP_WIFI_SSID),
            .channel = EXAMPLE_ESP_WIFI_CHANNEL,
            .password = EXAMPLE_ESP_WIFI_PASS,
            .max_connection = EXAMPLE_MAX_STA_CONN,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK,
          },
    };

    if(0 == strlen(EXAMPLE_ESP_WIFI_PASS))
    {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    if(ESP_OK == err)
    {
        /* Set wifi mode: AP */
        ESP_LOGI(TAG, " Set wifi mode: AP");
        err = esp_wifi_set_mode(WIFI_MODE_AP);
    }

    if(ESP_OK == err)
    {
        /* Set wifi config: AP */
        ESP_LOGI(TAG, " Set wifi config for AP");
        err = esp_wifi_set_config(WIFI_IF_AP, &wifi_config);
    }

    if(ESP_OK == err)
    {
        /* Start wifi in AP mode */
        ESP_LOGI(TAG, " Start wifi in AP mode");
        err = esp_wifi_start();
        ESP_LOGI(TAG,
                 "wifi_init_softap finished. SSID:%s password:%s channel:%d",
                 EXAMPLE_ESP_WIFI_SSID,
                 EXAMPLE_ESP_WIFI_PASS,
                 EXAMPLE_ESP_WIFI_CHANNEL);
    }

    return err;
}

//---------------------------- INTERRUPT HANDLERS -----------------------------

static void _event_handler(void *p_arg, esp_event_base_t event_base, int32_t event_id, void *p_event_data)
{
    if(WIFI_EVENT_AP_STACONNECTED == event_id)
    {
        wifi_event_ap_staconnected_t *p_event = (wifi_event_ap_staconnected_t *)p_event_data;
        ESP_LOGI(TAG, "station " MACSTR " JOIN, AID=%d", MAC2STR(p_event->mac), p_event->aid);
    }
    else if(WIFI_EVENT_AP_STADISCONNECTED == event_id)
    {
        wifi_event_ap_stadisconnected_t *p_event = (wifi_event_ap_stadisconnected_t *)p_event_data;
        ESP_LOGI(TAG, "station " MACSTR " LEAVE, AID=%d", MAC2STR(p_event->mac), p_event->aid);
    }
    else
    {
        // Ignore.
    }
}