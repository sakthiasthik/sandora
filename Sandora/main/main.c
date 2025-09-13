#include "mpu6050.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "max7219_led.h"

static const char *TAG = "MAIN";


void app_main(void) 
{
    // if (mpu6050_init() == ESP_OK) 
    // {
    //     mpu6050_test();  // Runs infinite loop printing processed orientation
    //     vTaskDelay(pdMS_TO_TICKS(500));
    // }      
    max7219_init();
    while (1) 
    {
        max7219_test_pattern();
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
