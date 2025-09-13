// #include "max7219_led.h"
// #include "driver/spi_master.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"

// // SPI device handle
// static spi_device_handle_t spi;

// #define MAX7219_REG_DIGIT0      0x01
// #define MAX7219_REG_DIGIT7      0x08
// #define MAX7219_REG_DECODEMODE  0x09
// #define MAX7219_REG_INTENSITY   0x0A
// #define MAX7219_REG_SCANLIMIT   0x0B
// #define MAX7219_REG_SHUTDOWN    0x0C
// #define MAX7219_REG_DISPLAYTEST 0x0F

// // Send one command to MAX7219
// static void max7219_send(uint8_t reg, uint8_t data)
// {
//     uint8_t tx_data[2] = { reg, data };
//     spi_transaction_t t = {
//         .length = 16,
//         .tx_buffer = tx_data
//     };
//     spi_device_transmit(spi, &t);
// }

// void max7219_init()
// {
//     spi_bus_config_t buscfg = {
//         .mosi_io_num = MAX7219_MOSI_PIN,
//         .miso_io_num = -1,
//         .sclk_io_num = MAX7219_CLK_PIN,
//         .quadwp_io_num = -1,
//         .quadhd_io_num = -1,
//     };
//     spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);

//     spi_device_interface_config_t devcfg = {
//         .clock_speed_hz = 10000000,
//         .mode = 0,
//         .spics_io_num = MAX7219_CS_PIN,
//         .queue_size = 1,
//     };
//     spi_bus_add_device(SPI2_HOST, &devcfg, &spi);

//     max7219_send(MAX7219_REG_SCANLIMIT, 0x07);
//     max7219_send(MAX7219_REG_DECODEMODE, 0x00);
//     max7219_send(MAX7219_REG_SHUTDOWN, 0x01);
//     max7219_send(MAX7219_REG_DISPLAYTEST, 0x00);
//     max7219_send(MAX7219_REG_INTENSITY, 0x08);

//     // Clear display
//     for (int i = 1; i <= 8; i++) {
//         max7219_send(i, 0x00);
//     }
// }

// void max7219_draw(const uint8_t *pattern)
// {
//     for (int row = 0; row < 8; row++) {
//         max7219_send(row + 1, pattern[row]);
//     }
// }

// void max7219_heartbeat_animation()
// {
//     const uint8_t HEART_FRAME_0[8] = {
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000,
//         0b00000000
//     };

//     const uint8_t HEART_FRAME_1[8] = {
//         0b00000000,
//         0b00000000,
//         0b00100100,
//         0b00100100,
//         0b00011000,
//         0b00000000,
//         0b00000000,
//         0b00000000
//     };

//     const uint8_t HEART_FRAME_2[8] = {
//         0b00000000,
//         0b00100100,
//         0b01111110,
//         0b01111110,
//         0b00111100,
//         0b00011000,
//         0b00000000,
//         0b00000000
//     };

//     const uint8_t HEART_FRAME_3[8] = {
//         0b00100100,
//         0b01111110,
//         0b11111111,
//         0b11111111,
//         0b01111110,
//         0b00111100,
//         0b00011000,
//         0b00000000
//     };

//     const uint8_t *frames[] = {
//         HEART_FRAME_0,
//         HEART_FRAME_1,
//         HEART_FRAME_2,
//         HEART_FRAME_3,
//         HEART_FRAME_2,
//         HEART_FRAME_1,
//         HEART_FRAME_0
//     };

//     const int frame_count = sizeof(frames) / sizeof(frames[0]);

//     for (int i = 0; i < frame_count; i++) {
//         max7219_draw(frames[i]);
//         vTaskDelay(pdMS_TO_TICKS(120));
//     }
// }












#include "max7219_led.h"
#include "driver/spi_master.h"
#include "esp_log.h"

static spi_device_handle_t spi_handle;

// Helper function to send a byte of data to the MAX7219
static esp_err_t max7219_send_byte(uint8_t reg, uint8_t data)
{
    spi_transaction_t t = {
        .length = 8, // 8 bits
        .tx_buffer = &data,
        .rx_buffer = NULL,
        .user = (void*) reg
    };

    esp_err_t ret = spi_device_transmit(spi_handle, &t);
    if (ret != ESP_OK) {
        ESP_LOGE("MAX7219", "Failed to transmit data: reg=0x%02x, data=0x%02x", reg, data);
    }
    return ret;
}

// Function to initialize the MAX7219 and SPI interface
esp_err_t max7219_init()
{
    ESP_LOGI("MAX7219", "Initializing MAX7219...");

    spi_bus_config_t bus_config = {
        .mosi_io_num = MAX7219_MOSI_PIN,
        .sclk_io_num = MAX7219_CLK_PIN,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1
    };

    spi_device_interface_config_t dev_config = {
        .clock_speed_hz = 1000000, // 1 MHz
        .mode = 0, // SPI Mode 0
        .spics_io_num = MAX7219_CS_PIN,
        .queue_size = 7
    };

    esp_err_t ret = spi_bus_initialize(SPI2_HOST, &bus_config, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) {
        ESP_LOGE("MAX7219", "Failed to initialize SPI bus");
        return ret;
    }

    ret = spi_bus_add_device(SPI2_HOST, &dev_config, &spi_handle);
    if (ret != ESP_OK) {
        ESP_LOGE("MAX7219", "Failed to add SPI device");
        return ret;
    }

    // Initialize MAX7219
    max7219_send(MAX7219_REG_SHUTDOWN, 0x01); // Turn on the display
    max7219_send(MAX7219_REG_DECODEMODE, 0x00); // No BCD decode
    max7219_send(MAX7219_REG_SCANLIMIT, 0x07); // Scan all 8 digits
    max7219_send(MAX7219_REG_INTENSITY, 0x0F); // Maximum brightness
    max7219_send(MAX7219_REG_DISPLAYTEST, 0x00); // No display test
    max7219_clear();

    ESP_LOGI("MAX7219", "MAX7219 initialized successfully");
    return ESP_OK;
}

// Function to send data to a specific register of the MAX7219
esp_err_t max7219_send(uint8_t reg, uint8_t data)
{
    return max7219_send_byte(reg, data);
}

// Function to clear the display
void max7219_clear()
{
    ESP_LOGI("MAX7219", "Clearing the display...");
    for (int i = 0; i < 8; i++) {
        max7219_send(i + 1, 0x00); // Clear each digit
    }
}

// Function to update the hourglass animation on the display
void max7219_update_hourglass(int seconds)
{
    ESP_LOGD("MAX7219", "Updating hourglass for %d seconds", seconds);

    if (seconds > 0) {
        int led_index = 8 - (seconds / 4); // Dividing by 4 to slow down the effect
        if (led_index < 1) led_index = 1; // Ensure at least one LED is lit
        max7219_send(led_index, 0xFF); // Light up the current LED
        ESP_LOGD("MAX7219", "Lighting LED index %d", led_index);
    } else {
        max7219_clear(); // Clear the display when the timer is done
        ESP_LOGI("MAX7219", "Hourglass finished");
    }
}

// Function to test by lighting up all LEDs in the display
void max7219_light_all()
{
    ESP_LOGI("MAX7219", "Lighting all LEDs on the display...");

    // Turn on all LEDs (0xFF will light up all 8 LEDs in each digit)
    for (int i = 0; i < 8; i++) {
        max7219_send(i + 1, 0xFF);  // Light up each digit
    }
}

void max7219_update_hourglass_1(int elapsed_time, int total_time) 
{
    int num_sand_top = (elapsed_time * 4) / total_time;   // Top half (4 rows)
    int num_sand_bottom = (elapsed_time * 4) / total_time; // Bottom half (4 rows)

    // Clear all rows first
    for (int i = 0; i < 8; i++) {
        max7219_send(i + 1, 0x00); // Turn off all LEDs in each row
    }

    // Top half of the hourglass: gradually fill from top to bottom
    for (int i = 0; i < num_sand_top; i++) {
        max7219_send(i + 1, 0xFF);  // Light up all LEDs in the row
    }

    // Bottom half of the hourglass: gradually fill from bottom to top
    for (int i = 7; i >= 8 - num_sand_bottom; i--) {
        max7219_send(i + 1, 0xFF);  // Light up all LEDs in the row
    }

    ESP_LOGI("MAX7219", "Top Sand: %d, Bottom Sand: %d", num_sand_top, num_sand_bottom);
}
