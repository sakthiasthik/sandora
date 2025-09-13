// // // // #include <stdio.h>
// // // // #include "freertos/FreeRTOS.h"
// // // // #include "freertos/task.h"
// // // // #include "driver/gpio.h"

// // // // // Define the GPIO pin for the LED
// // // // #define LED_GPIO GPIO_NUM_2

// // // // void app_main(void)
// // // // {
// // // //     // Print start-up message
// // // //     printf("Starting LED blink application on ESP32-C3...\n");

// // // //     // Reset and configure the GPIO pin
// // // //     gpio_reset_pin(LED_GPIO);
// // // //     gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
// // // //     printf("Configured GPIO %d as output.\n", LED_GPIO);

// // // //     int led_state = 0;

// // // //     while (1)
// // // //     {
// // // //         // Toggle LED state
// // // //         led_state = !led_state;

// // // //         // Set GPIO level
// // // //         gpio_set_level(LED_GPIO, led_state);

// // // //         // Print debug message
// // // //         printf("LED is now %s\n", led_state ? "ON" : "OFF");

// // // //         // Delay 500 milliseconds
// // // //         vTaskDelay(pdMS_TO_TICKS(500));
// // // //     }
// // // // }















// // #include <stdio.h>
// // #include "driver/spi_master.h"
// // #include "driver/gpio.h"
// // #include "freertos/FreeRTOS.h"
// // #include "freertos/task.h"

// // #define SPI_MOSI_PIN 6
// // #define SPI_CLK_PIN 4
// // #define SPI_CS_PIN 7

// // static spi_device_handle_t spi;

// // // MAX7219 Register Addresses
// // #define MAX7219_REG_NOOP        0x00
// // #define MAX7219_REG_DIGIT0      0x01
// // #define MAX7219_REG_DIGIT1      0x02
// // #define MAX7219_REG_DIGIT2      0x03
// // #define MAX7219_REG_DIGIT3      0x04
// // #define MAX7219_REG_DIGIT4      0x05
// // #define MAX7219_REG_DIGIT5      0x06
// // #define MAX7219_REG_DIGIT6      0x07
// // #define MAX7219_REG_DIGIT7      0x08
// // #define MAX7219_REG_DECODEMODE  0x09
// // #define MAX7219_REG_INTENSITY   0x0A
// // #define MAX7219_REG_SCANLIMIT   0x0B
// // #define MAX7219_REG_SHUTDOWN    0x0C
// // #define MAX7219_REG_DISPLAYTEST 0x0F

// // // Send two bytes to MAX7219 (register and data)
// // void max7219_send(uint8_t reg, uint8_t data)
// // {
// //     uint8_t tx_data[2] = { reg, data };
// //     spi_transaction_t t = {
// //         .length = 16, // 16 bits
// //         .tx_buffer = tx_data
// //     };
// //     spi_device_transmit(spi, &t);
// // }

// // void max7219_draw(const uint8_t *pattern)
// // {
// //     for (int row = 0; row < 8; row++) {
// //         max7219_send(row + 1, pattern[row]);
// //     }
// // }

// // // Initialize MAX7219
// // void max7219_init()
// // {
// //     max7219_send(MAX7219_REG_SCANLIMIT, 0x07);    // Display all 8 digits
// //     max7219_send(MAX7219_REG_DECODEMODE, 0x00);    // No decode for 8x8 LED
// //     max7219_send(MAX7219_REG_SHUTDOWN, 0x01);      // Exit shutdown mode
// //     max7219_send(MAX7219_REG_DISPLAYTEST, 0x00);   // Disable display test
// //     max7219_send(MAX7219_REG_INTENSITY, 0x08);     // Medium brightness
// //     // Clear display
// //     for (int i = 1; i <= 8; i++) {
// //         max7219_send(i, 0x00);
// //     }
// // }

// // // Example: draw a smiley face (8 bytes)
// // const uint8_t smiley[8] = {
// //     0b00111100,
// //     0b01000010,
// //     0b10100101,
// //     0b10000001,
// //     0b10100101,
// //     0b10011001,
// //     0b01000010,
// //     0b00111100
// // };

// // void draw_heart()
// // {
// //     const uint8_t heart_pattern[8] = {
// //         0b00100100,
// //         0b01111110,
// //         0b11111111,
// //         0b11111111,
// //         0b01111110,
// //         0b00111100,
// //         0b00011000,
// //         0b00000000
// //     };

// //     for (int i = 0; i < 8; i++) {
// //         max7219_send(i + 1, heart_pattern[i]);
// //     }
// // }

// // void heartbeat_animation()
// // {
// //     const uint8_t heart_pattern_small[8] = {
// //         0b00000000,
// //         0b00100100,
// //         0b01111110,
// //         0b01111110,
// //         0b00111100,
// //         0b00011000,
// //         0b00000000,
// //         0b00000000
// //     };

// //     const uint8_t heart_pattern_big[8] = {
// //         0b00100100,
// //         0b01111110,
// //         0b11111111,
// //         0b11111111,
// //         0b01111110,
// //         0b00111100,
// //         0b00011000,
// //         0b00000000
// //     };

// //     for (int i = 0; i < 3; i++) {
// //         // Small heart (beat in)
// //         for (int row = 0; row < 8; row++) {
// //             max7219_send(row + 1, heart_pattern_small[row]);
// //         }
// //         vTaskDelay(pdMS_TO_TICKS(150));

// //         // Big heart (beat out)
// //         for (int row = 0; row < 8; row++) {
// //             max7219_send(row + 1, heart_pattern_big[row]);
// //         }
// //         vTaskDelay(pdMS_TO_TICKS(150));
// //     }
// // }

// // const uint8_t HEART_FRAME_0[8] = {
// //     0b00000000,
// //     0b00000000,
// //     0b00000000,
// //     0b00000000,
// //     0b00000000,
// //     0b00000000,
// //     0b00000000,
// //     0b00000000
// // };

// // const uint8_t HEART_FRAME_1[8] = {
// //     0b00000000,
// //     0b00000000,
// //     0b00100100,
// //     0b00100100,
// //     0b00011000,
// //     0b00000000,
// //     0b00000000,
// //     0b00000000
// // };

// // const uint8_t HEART_FRAME_2[8] = {
// //     0b00000000,
// //     0b00100100,
// //     0b01111110,
// //     0b01111110,
// //     0b00111100,
// //     0b00011000,
// //     0b00000000,
// //     0b00000000
// // };

// // const uint8_t HEART_FRAME_3[8] = {
// //     0b00100100,
// //     0b01111110,
// //     0b11111111,
// //     0b11111111,
// //     0b01111110,
// //     0b00111100,
// //     0b00011000,
// //     0b00000000
// // };

// // void heartbeat_animation_full()
// // {
// //     const uint8_t *frames[] = {
// //         HEART_FRAME_0,
// //         HEART_FRAME_1,
// //         HEART_FRAME_2,
// //         HEART_FRAME_3,
// //         HEART_FRAME_2,
// //         HEART_FRAME_1,
// //         HEART_FRAME_0
// //     };

// //     const int frame_count = sizeof(frames) / sizeof(frames[0]);

// //     for (int i = 0; i < frame_count; i++) {
// //         max7219_draw(frames[i]);
// //         vTaskDelay(pdMS_TO_TICKS(120));
// //     }
// // }




// // // void max7219_draw(const uint8_t *data)
// // // {
// // //     for (int i = 0; i < 8; i++) {
// // //         max7219_send(i + 1, data[i]);
// // //     }
// // // }



// // void app_main()
// // {
// //     // SPI bus configuration
// //     spi_bus_config_t buscfg = {
// //         .mosi_io_num = SPI_MOSI_PIN,
// //         .miso_io_num = -1,
// //         .sclk_io_num = SPI_CLK_PIN,
// //         .quadwp_io_num = -1,
// //         .quadhd_io_num = -1,
// //     };
// //     spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);

// //     // SPI device interface configuration
// //     spi_device_interface_config_t devcfg = {
// //         .clock_speed_hz = 10000000,           // 10 MHz
// //         .mode = 0,                             // SPI mode 0
// //         .spics_io_num = SPI_CS_PIN,           // CS pin
// //         .queue_size = 1,
// //     };

// //     // Attach MAX7219 to the SPI bus
// //     spi_bus_add_device(SPI2_HOST, &devcfg, &spi);

// //     // Initialize MAX7219
// //     max7219_init();

// //     // // while (1) {
// //     // //     max7219_draw(smiley);  // Draw pattern
// //     // //     vTaskDelay(pdMS_TO_TICKS(1000));

// //     // //     // Clear display
// //     // //     for (int i = 1; i <= 8; i++) {
// //     // //         max7219_send(i, 0x00);
// //     // //     }
// //     // //     vTaskDelay(pdMS_TO_TICKS(1000));
// //     // // }

// //     //     while (1) {
// //     //     heartbeat_animation();
// //     //     vTaskDelay(pdMS_TO_TICKS(1000));
// //     // }


// //     while (1) {
// //         heartbeat_animation_full();
// //         vTaskDelay(pdMS_TO_TICKS(600)); // Pause between heartbeats
// //     }

// // }
















// #include <stdio.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "max7219_led.h"
// #include "esp_log.h"
// #include "mpu6050.h"


// static const char *TAG = "APP";

// void app_main()
// {
//     max7219_init();
//     while (1) 
//     {
//         max7219_heartbeat_animation();
//         vTaskDelay(pdMS_TO_TICKS(600));
//     }

//         ESP_ERROR_CHECK(mpu6050_init());

//     mpu6050_data_t data;

//     // while (1) {
//     //             max7219_heartbeat_animation();
//     //     vTaskDelay(pdMS_TO_TICKS(600));
//     //     if (mpu6050_read_sensor(&data) == ESP_OK) 
//     //     {
//     //         // ESP_LOGI(TAG,
//     //         //          "Accel [X: %d Y: %d Z: %d] | Gyro [X: %d Y: %d Z: %d] | Temp Raw: %d",
//     //         //          data.accel_x, data.accel_y, data.accel_z,
//     //         //          data.gyro_x, data.gyro_y, data.gyro_z,
//     //         //          data.temperature_raw);

//     //         ESP_LOGI(TAG,
//     //                 "Accel [X: %.2f g Y: %.2f g Z: %.2f g] | "
//     //                 "Gyro [X: %.2f °/s Y: %.2f °/s Z: %.2f °/s] | "
//     //                 "Temp: %.2f °C",
//     //                 data.accel_x, data.accel_y, data.accel_z,
//     //                 data.gyro_x, data.gyro_y, data.gyro_z,
//     //                 data.temperature);

//     //     } else 
//     //     {
//     //         ESP_LOGE(TAG, "Failed to read sensor data");
//     //     }

//     //     vTaskDelay(pdMS_TO_TICKS(2000));
//     // }

// }








#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "max7219_led.h"

// Global variable to control the duration of the hourglass (in seconds)
// int hourglass_time = 30;  // 30 seconds, you can modify this value dynamically

#define HOURGLASS_TIME 60  // 10 seconds for the full animation cycle


// // void app_main()
// // {
// //     ESP_LOGI("MAIN", "Starting the program...");

// //     // Initialize MAX7219
// //     esp_err_t ret = max7219_init();
// //     if (ret != ESP_OK) {
// //         ESP_LOGE("MAIN", "Failed to initialize MAX7219");
// //         return;
// //     }

// //     // Hourglass simulation loop
// //     for (int i = hourglass_time; i >= 0; i--) {
// //         ESP_LOGI("MAIN", "Updating hourglass with %d seconds remaining", i);
// //         max7219_update_hourglass(i); // Update the display based on remaining time
// //         vTaskDelay(pdMS_TO_TICKS(1000)); // Wait for 1 second
// //     }

// //     // After timer runs out, clear the display
// //     max7219_clear();
// //     ESP_LOGI("MAIN", "Hourglass simulation finished");
// // }

// void app_main()
// {
//     ESP_LOGI("MAIN", "Starting the program...");

//     // Initialize MAX7219
//     esp_err_t ret = max7219_init();
//     if (ret != ESP_OK) {
//         ESP_LOGE("MAIN", "Failed to initialize MAX7219");
//         return;
//     }

//     // Test lighting all LEDs
//     max7219_light_all();

//     // Pause to keep the LEDs lit up for a while
//     vTaskDelay(pdMS_TO_TICKS(5000)); // Wait 5 seconds
// }

void app_main() {
    ESP_LOGI("MAIN", "Starting the program...");

    // Initialize MAX7219
    esp_err_t ret = max7219_init();
    if (ret != ESP_OK) {
        ESP_LOGE("MAIN", "Failed to initialize MAX7219");
        return;
    }

    // Test lighting all LEDs first
    max7219_light_all();

    // Wait for 3 seconds before starting the hourglass animation
    vTaskDelay(pdMS_TO_TICKS(3000));

    // Start the hourglass animation
    for (int t = 0; t <= HOURGLASS_TIME; t++) 
    {
        max7219_update_hourglass_1(t, HOURGLASS_TIME);  // Update hourglass every second
        vTaskDelay(pdMS_TO_TICKS(1000));  // Wait 1 second before the next update
    }

    ESP_LOGI("MAIN", "Hourglass animation complete!");
}

