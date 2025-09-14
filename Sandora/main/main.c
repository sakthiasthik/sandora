// // // // #include "mpu6050.h"
// // // // #include "freertos/FreeRTOS.h"
// // // // #include "freertos/task.h"

// // // // #include "max7219_led.h"

// // // // static const char *TAG = "MAIN";


// // // // void app_main(void) 
// // // // {
// // // //     // if (mpu6050_init() == ESP_OK) 
// // // //     // {
// // // //     //     mpu6050_test();  // Runs infinite loop printing processed orientation
// // // //     //     vTaskDelay(pdMS_TO_TICKS(500));
// // // //     // }      
// // // //     max7219_init();
// // // //     while (1) 
// // // //     {
// // // //         max7219_test_pattern();
// // // //         vTaskDelay(pdMS_TO_TICKS(2000));
// // // //     }
// // // // }

















// // // #include "freertos/FreeRTOS.h"
// // // #include "freertos/task.h"
// // // #include "mpu6050.h"
// // // #include "max7219_led.h"
// // // #include "hourglass.h"
// // // #include "esp_log.h"

// // // static const char *TAG = "MAIN";

// // // void app_main(void) 
// // // {
// // //     mpu6050_init();
// // //     max7219_init();
// // //     hourglass_init(6000); // 10 min

// // //     while (1) 
// // //     {
// // //         ESP_LOGI(TAG, "In While Loop");
// // //         Orientation_t o = mpu6050_get_orientation_state();
// // //         ESP_LOGI(TAG, "Orientation: %d", o); // ← log orientation
// // //         hourglass_update(o);
// // //         vTaskDelay(pdMS_TO_TICKS(2000));
// // //     }
// // // }
















// // #include "freertos/FreeRTOS.h"
// // #include "freertos/task.h"
// // #include "mpu6050.h"
// // #include "max7219_led.h"
// // #include "hourglass.h"
// // #include "esp_log.h"

// // static const char *TAG = "MAIN";

// // void app_main(void) 
// // {
// //     // Initialize hardware
// //     ESP_LOGI(TAG, "Initializing hardware...");
// //     esp_err_t mpu_ret = mpu6050_init();
// //     if (mpu_ret != ESP_OK) {
// //         ESP_LOGE(TAG, "Failed to initialize MPU6050");
// //         return;
// //     }
    
// //     max7219_init();
    
// //     // Show test pattern first
// //     ESP_LOGI(TAG, "Displaying test pattern");
// //     max7219_test_pattern();
// //     vTaskDelay(pdMS_TO_TICKS(3000));
    
// //     // Initialize hourglass with 60 second duration for testing
// //     hourglass_init(60000);
    
// //     ESP_LOGI(TAG, "Starting hourglass demo");
    
// //     while (1) 
// //     {
// //         Orientation_t o = mpu6050_get_orientation_state();
// //         ESP_LOGI(TAG, "Orientation: %d (%s)", o, orientation_to_string(o));
        
// //         hourglass_update(o);
// //         vTaskDelay(pdMS_TO_TICKS(2000));
// //     }
// // }




















// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "mpu6050.h"
// #include "max7219_led.h"
// #include "hourglass.h"
// #include "esp_log.h"

// static const char *TAG = "MAIN";

// void app_main(void) 
// {
//     // Initialize hardware
//     ESP_LOGI(TAG, "Initializing hardware...");
//     esp_err_t mpu_ret = mpu6050_init();
//     if (mpu_ret != ESP_OK) {
//         ESP_LOGE(TAG, "Failed to initialize MPU6050");
//         return;
//     }
    
//     max7219_init();
    
//     // Show test pattern first
//     ESP_LOGI(TAG, "Displaying test pattern");
//     // max7219_test_pattern();
//     // vTaskDelay(pdMS_TO_TICKS(3000));
    
//     // // Initialize hourglass with 60 second duration for testing
//     // hourglass_init(60000);
    
//     // ESP_LOGI(TAG, "Starting hourglass test animation");
    
//     // // Run the test animation in a loop
//     // while (1) 
//     // {
//     //     hourglass_test_animation();
//     //     vTaskDelay(pdMS_TO_TICKS(5000)); // Wait 5 seconds before restarting
//     // }

//     // // hourglass_turn_on_led();
//         hourglass_init();
//     hourglass_start();
// }

















#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mpu6050.h"
#include "max7219_led.h"
#include "hourglass.h"
#include "esp_log.h"

static const char *TAG = "MAIN";

void app_main(void) 
{
    ESP_LOGI(TAG, "Initializing hardware...");

    // Seed RNG once for randomness
    srand(time(NULL));

    esp_err_t mpu_ret = mpu6050_init();
    if (mpu_ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize MPU6050");
        return;
    }

    max7219_init();

    ESP_LOGI(TAG, "Starting hourglass animation...");
    hourglass_init();
    hourglass_start();

    // Nothing else to do in app_main
    while(1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
