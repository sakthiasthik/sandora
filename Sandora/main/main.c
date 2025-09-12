// // #include <stdio.h>

// // void app_main(void)
// // {

// // }




// #include <stdio.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "driver/gpio.h"

// // Define the GPIO pin where LED is connected
// #define LED_GPIO GPIO_NUM_2

// void app_main(void)
// {
//     // Configure the GPIO pin
//     gpio_reset_pin(LED_GPIO);
//     gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);

//     while (1)
//     {
//         // Turn the LED on
//         gpio_set_level(LED_GPIO, 1);
//         vTaskDelay(pdMS_TO_TICKS(500));  // Delay 500 ms

//         // Turn the LED off
//         gpio_set_level(LED_GPIO, 0);
//         vTaskDelay(pdMS_TO_TICKS(500));  // Delay 500 ms
//     }
// }















#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

// Define the GPIO pin for the LED
#define LED_GPIO GPIO_NUM_2

void app_main(void)
{
    // Print start-up message
    printf("Starting LED blink application on ESP32-C3...\n");

    // Reset and configure the GPIO pin
    gpio_reset_pin(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
    printf("Configured GPIO %d as output.\n", LED_GPIO);

    int led_state = 0;

    while (1)
    {
        // Toggle LED state
        led_state = !led_state;

        // Set GPIO level
        gpio_set_level(LED_GPIO, led_state);

        // Print debug message
        printf("LED is now %s\n", led_state ? "ON" : "OFF");

        // Delay 500 milliseconds
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
