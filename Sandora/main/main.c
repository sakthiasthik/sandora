// #include "mpu6050.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"

// #include "max7219_led.h"

// static const char *TAG = "MAIN";


// void app_main(void) 
// {
//     // if (mpu6050_init() == ESP_OK) 
//     // {
//     //     mpu6050_test();  // Runs infinite loop printing processed orientation
//     //     vTaskDelay(pdMS_TO_TICKS(500));
//     // }      
//     max7219_init();
//     while (1) 
//     {
//         max7219_test_pattern();
//         vTaskDelay(pdMS_TO_TICKS(2000));
//     }
// }

















#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mpu6050.h"
#include "max7219_led.h"
#include "hourglass.h"

void app_main(void) 
{
    mpu6050_init();
    max7219_init();
    hourglass_init(600000); // 10 min

    while (1) 
    {
        Orientation_t o = mpu6050_get_orientation_state();
        hourglass_update(o);
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}
