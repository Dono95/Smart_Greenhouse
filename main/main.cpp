#include <stdio.h>

#include "Drivers/Sensors/SHT4x.hpp"

#include <esp_log.h>
#include <nvs_flash.h>

#include <esp_bt.h>
#include <esp_gap_bt_api.h>
#include "esp_gatt_common_api.h"
#include <esp_bt_main.h>
#include <esp_bt_device.h>

#include "esp_system.h"
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"

static const char *MAIN_TAG = "Main";

extern "C" void app_main(void)
{
    printf("Test\n");

    while (true)
    {
    }
}
