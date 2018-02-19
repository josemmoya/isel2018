#include <string.h>
#include <stdio.h>
#include "esp_common.h"
#include "freertos/task.h"
#include "gpio.h"


/******************************************************************************
 * FunctionName : user_rf_cal_sector_set
 * Description  : SDK just reversed 4 sectors, used for rf init data and paramters.
 *                We add this function to force users to set rf cal sector, since
 *                we don't know which sector is free in user's application.
 *                sector map for last several sectors : ABCCC
 *                A : rf cal
 *                B : rf init data
 *                C : sdk parameters
 * Parameters   : none
 * Returns      : rf cal sector
*******************************************************************************/
uint32 user_rf_cal_sector_set(void)
{
    flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;
    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}

static void
baliza (void* ignore)
{
    /* de esta forma no hay derivas acumuladas */
    portTickType period = 500 /portTICK_RATE_MS;
    portTickType last = xTaskGetTickCount();
    int value = 0;

    while (1) {
        value = !value;
        GPIO_OUTPUT_SET (2, value);
        vTaskDelayUntil (&last, period);
    }
}

void user_init(void)
{
    xTaskHandle task;
    xTaskCreate (baliza, "startup", 2048, NULL, 1, &task);
}
