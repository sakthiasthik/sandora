// #include "max7219_led.h"
// #include "driver/spi_master.h"
// #include "driver/gpio.h"
// #include <string.h>
// #include "esp_log.h"

// static spi_device_handle_t spi_handle;

// static const char *TAG = "MAX7219";

// // Display buffer (16x8 for 2 chained modules)
// static uint8_t display_buffer[MAX7219_TOTAL_HEIGHT][MAX7219_TOTAL_WIDTH / 8];

// // ======= LOW LEVEL: SEND COMMAND =======
// static void max7219_send_cmd(uint8_t device, uint8_t reg, uint8_t data) 
// {
//     uint8_t tx_data[MAX7219_CHAINED_DEVICES * 2];
//     memset(tx_data, 0, sizeof(tx_data));

//     // Fill with NO-OPs
//     for (int i = 0; i < MAX7219_CHAINED_DEVICES; i++) 
//     {
//         tx_data[i * 2]     = 0x00; 
//         tx_data[i * 2 + 1] = 0x00;
//     }

//     // Place actual command for target device
//     tx_data[device * 2]     = reg;
//     tx_data[device * 2 + 1] = data;

//     spi_transaction_t t = {
//         .length = MAX7219_CHAINED_DEVICES * 16,  // bits
//         .tx_buffer = tx_data,
//     };
//     spi_device_transmit(spi_handle, &t);
// }

// // Broadcast command to all devices
// static void max7219_send_cmd_all(uint8_t reg, uint8_t data) 
// {
//     uint8_t tx_data[MAX7219_CHAINED_DEVICES * 2];
//     for (int i = 0; i < MAX7219_CHAINED_DEVICES; i++) 
//     {
//         tx_data[i * 2]     = reg;
//         tx_data[i * 2 + 1] = data;
//     }
//     spi_transaction_t t = {
//         .length = MAX7219_CHAINED_DEVICES * 16,
//         .tx_buffer = tx_data,
//     };
//     spi_device_transmit(spi_handle, &t);
// }

// // ======= PUBLIC API =======
// void max7219_init(void) 
// {
//     spi_bus_config_t buscfg = {
//         .mosi_io_num = MAX7219_MOSI_PIN,
//         .miso_io_num = -1,
//         .sclk_io_num = MAX7219_CLK_PIN,
//         .quadwp_io_num = -1,
//         .quadhd_io_num = -1
//     };

//     spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_DISABLED);

//     spi_device_interface_config_t devcfg = {
//         .clock_speed_hz = 1 * 1000 * 1000, // 1 MHz
//         .mode = 0,
//         .spics_io_num = MAX7219_CS_PIN,
//         .queue_size = 1,
//     };

//     spi_bus_add_device(SPI2_HOST, &devcfg, &spi_handle);

//     // Init sequence for all devices
//     max7219_send_cmd_all(0x0C, 0x01); // Shutdown register: normal operation
//     max7219_send_cmd_all(0x0F, 0x00); // Display test: off
//     max7219_send_cmd_all(0x0B, 0x07); // Scan limit: all digits
//     max7219_send_cmd_all(0x09, 0x00); // Decode mode: none
//     max7219_send_cmd_all(0x0A, 0x08); // Intensity: medium
//     max7219_clear();
//     ESP_LOGI(TAG, "Hourglass task started");
// }

// void max7219_clear(void) 
// {
//     memset(display_buffer, 0, sizeof(display_buffer));
//     for (int row = 0; row < 8; row++) 
//     {
//         uint8_t tx_data[MAX7219_CHAINED_DEVICES * 2];
//         for (int dev = 0; dev < MAX7219_CHAINED_DEVICES; dev++) 
//         {
//             tx_data[dev * 2]     = row + 1;
//             tx_data[dev * 2 + 1] = 0x00;
//         }
//         spi_transaction_t t = {
//             .length = MAX7219_CHAINED_DEVICES * 16,
//             .tx_buffer = tx_data,
//         };
//         spi_device_transmit(spi_handle, &t);
//     }
// }

// void max7219_set_pixel(uint8_t x, uint8_t y, bool on) 
// {
//     if (x >= MAX7219_TOTAL_WIDTH || y >= MAX7219_TOTAL_HEIGHT) return;
//     uint8_t byte_index = x / 8;
//     uint8_t bit_index = x % 8;

//     if (on)
//         display_buffer[y][byte_index] |= (1 << bit_index);
//     else
//         display_buffer[y][byte_index] &= ~(1 << bit_index);
// }

// void max7219_update(void) 
// {
//     for (int row = 0; row < MAX7219_MATRIX_HEIGHT; row++) 
//     {
//         uint8_t tx_data[MAX7219_CHAINED_DEVICES * 2];
//         for (int dev = 0; dev < MAX7219_CHAINED_DEVICES; dev++) 
//         {
//             tx_data[dev * 2]     = row + 1;
//             tx_data[dev * 2 + 1] = display_buffer[row][dev];
//         }
//         spi_transaction_t t = {
//             .length = MAX7219_CHAINED_DEVICES * 16,
//             .tx_buffer = tx_data,
//         };
//         spi_device_transmit(spi_handle, &t);
//     }
// }

// // ====== HOURGLASS HELPERS ======
// void hourglass_reset_led(void) 
// {
//     max7219_clear();
// }

// void hourglass_fill_level(uint8_t top_pixels) 
// {
//     max7219_clear();

//     // Fill top section
//     for (uint8_t y = 0; y < 8 && top_pixels > 0; y++) 
//     {
//         for (uint8_t x = 0; x < 8 && top_pixels > 0; x++) 
//         {
//             max7219_set_pixel(x, y, true);
//             top_pixels--;
//         }
//     }
//     max7219_update();
// }


// // ====== TEST FUNCTION ======
// void max7219_test_pattern(void) {
//     // Step 1: Clear display
//     hourglass_reset_led();

//     // Step 2: Draw diagonal line
//     for (uint8_t i = 0; i < 8; i++) {
//         max7219_set_pixel(i, i, true);           // First matrix
//         max7219_set_pixel(i + 8, 7 - i, true);   // Second matrix (mirrored)
//     }
//     max7219_update();
//     vTaskDelay(pdMS_TO_TICKS(1000));

//     // Step 3: Fill left half
//     hourglass_reset_led();
//     for (uint8_t y = 0; y < 8; y++) {
//         for (uint8_t x = 0; x < 4; x++) {
//             max7219_set_pixel(x, y, true);
//             max7219_set_pixel(x + 8, y, true);
//         }
//     }
//     max7219_update();
//     vTaskDelay(pdMS_TO_TICKS(1000));

//     // Step 4: Fill entire top matrix
//     hourglass_reset_led();
//     for (uint8_t y = 0; y < 8; y++) {
//         for (uint8_t x = 0; x < 8; x++) {
//             max7219_set_pixel(x, y, true);
//         }
//     }
//     max7219_update();
//     vTaskDelay(pdMS_TO_TICKS(1000));

//     // Step 5: Simple falling pixels animation
//     hourglass_reset_led();
//     for (uint8_t y = 0; y < 8; y++) {
//         hourglass_reset_led();
//         max7219_set_pixel(3, y, true);
//         max7219_set_pixel(12, y, true);
//         max7219_update();
//         vTaskDelay(pdMS_TO_TICKS(300));
//     }

//     // Step 6: Reset display
//     hourglass_reset_led();
// }
















#include "max7219_led.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include <string.h>
#include "esp_log.h"
static spi_device_handle_t spi_handle;
static const char *TAG = "MAX7219";
// Display buffer (16x8 for 2 chained modules)
static uint8_t display_buffer[MAX7219_TOTAL_HEIGHT][MAX7219_TOTAL_WIDTH / 8];
// ======= LOW LEVEL: SEND COMMAND =======
static void max7219_send_cmd(uint8_t device, uint8_t reg, uint8_t data) 
{
    uint8_t tx_data[MAX7219_CHAINED_DEVICES * 2];
    memset(tx_data, 0, sizeof(tx_data));
    // Fill with NO-OPs
    for (int i = 0; i < MAX7219_CHAINED_DEVICES; i++) 
    {
        tx_data[i * 2]     = 0x00; 
        tx_data[i * 2 + 1] = 0x00;
    }
    // Place actual command for target device
    tx_data[device * 2]     = reg;
    tx_data[device * 2 + 1] = data;
    spi_transaction_t t = {
        .length = MAX7219_CHAINED_DEVICES * 16,  // bits
        .tx_buffer = tx_data,
    };
    spi_device_transmit(spi_handle, &t);
}
// Broadcast command to all devices
static void max7219_send_cmd_all(uint8_t reg, uint8_t data) 
{
    uint8_t tx_data[MAX7219_CHAINED_DEVICES * 2];
    for (int i = 0; i < MAX7219_CHAINED_DEVICES; i++) 
    {
        tx_data[i * 2]     = reg;
        tx_data[i * 2 + 1] = data;
    }
    spi_transaction_t t = {
        .length = MAX7219_CHAINED_DEVICES * 16,
        .tx_buffer = tx_data,
    };
    spi_device_transmit(spi_handle, &t);
}
// ======= PUBLIC API =======
void max7219_init(void) 
{
    spi_bus_config_t buscfg = {
        .mosi_io_num = MAX7219_MOSI_PIN,
        .miso_io_num = -1,
        .sclk_io_num = MAX7219_CLK_PIN,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1
    };
    spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_DISABLED);
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 1 * 1000 * 1000, // 1 MHz
        .mode = 0,
        .spics_io_num = MAX7219_CS_PIN,
        .queue_size = 1,
    };
    spi_bus_add_device(SPI2_HOST, &devcfg, &spi_handle);
    // Init sequence for all devices
    max7219_send_cmd_all(0x0C, 0x01); // Shutdown register: normal operation
    max7219_send_cmd_all(0x0F, 0x00); // Display test: off
    max7219_send_cmd_all(0x0B, 0x07); // Scan limit: all digits
    max7219_send_cmd_all(0x09, 0x00); // Decode mode: none
    max7219_send_cmd_all(0x0A, 0x08); // Intensity: medium
    max7219_clear();
    ESP_LOGI(TAG, "MAX7219 initialized successfully");
}
void max7219_clear(void) 
{
    memset(display_buffer, 0, sizeof(display_buffer));
    for (int row = 0; row < 8; row++) 
    {
        uint8_t tx_data[MAX7219_CHAINED_DEVICES * 2];
        for (int dev = 0; dev < MAX7219_CHAINED_DEVICES; dev++) 
        {
            tx_data[dev * 2]     = row + 1;
            tx_data[dev * 2 + 1] = 0x00;
        }
        spi_transaction_t t = {
            .length = MAX7219_CHAINED_DEVICES * 16,
            .tx_buffer = tx_data,
        };
        spi_device_transmit(spi_handle, &t);
    }
}
void max7219_set_pixel(uint8_t x, uint8_t y, bool on) 
{
    if (x >= MAX7219_TOTAL_WIDTH || y >= MAX7219_TOTAL_HEIGHT) return;
    uint8_t byte_index = x / 8;
    uint8_t bit_index = x % 8;
    if (on)
        display_buffer[y][byte_index] |= (1 << bit_index);
    else
        display_buffer[y][byte_index] &= ~(1 << bit_index);
}
// void max7219_update(void) 
// {
//     for (int row = 0; row < MAX7219_MATRIX_HEIGHT; row++) 
//     {
//         uint8_t tx_data[MAX7219_CHAINED_DEVICES * 2];
//         for (int dev = 0; dev < MAX7219_CHAINED_DEVICES; dev++) 
//         {
//             tx_data[dev * 2]     = row + 1;
//             tx_data[dev * 2 + 1] = display_buffer[row][dev];
//         }
//         spi_transaction_t t = {
//             .length = MAX7219_CHAINED_DEVICES * 16,
//             .tx_buffer = tx_data,
//         };
//         spi_device_transmit(spi_handle, &t);
//     }
// }

void max7219_update(void) 
{
    for (int row = 0; row < MAX7219_MATRIX_HEIGHT; row++) 
    {
        uint8_t tx_data[MAX7219_CHAINED_DEVICES * 2];
        for (int dev = 0; dev < MAX7219_CHAINED_DEVICES; dev++) 
        {
            tx_data[dev * 2]     = row + 1;
            tx_data[dev * 2 + 1] = display_buffer[row][dev];  // dev is index of byte; this is correct only if display_buffer is [row][byte]
        }
        spi_transaction_t t = {
            .length = MAX7219_CHAINED_DEVICES * 16,
            .tx_buffer = tx_data,
        };
        spi_device_transmit(spi_handle, &t);
    }
}


// ====== HOURGLASS HELPERS ======
void max7219_clear_display(void)  // Renamed from hourglass_reset_led
{
    max7219_clear();
}
void hourglass_fill_level(uint8_t top_pixels) 
{
    max7219_clear();
    // Fill top section
    for (uint8_t y = 0; y < 8 && top_pixels > 0; y++) 
    {
        for (uint8_t x = 0; x < 8 && top_pixels > 0; x++) 
        {
            max7219_set_pixel(x, y, true);
            top_pixels--;
        }
    }
    max7219_update();
}
// ====== TEST FUNCTION ======
void max7219_test_pattern(void) {
    // Step 1: Clear display
    max7219_clear_display();
    // Step 2: Draw diagonal line
    for (uint8_t i = 0; i < 8; i++) {
        max7219_set_pixel(i, i, true);           // First matrix
        max7219_set_pixel(i + 8, 7 - i, true);   // Second matrix (mirrored)
    }
    max7219_update();
    vTaskDelay(pdMS_TO_TICKS(1000));
    // Step 3: Fill left half
    max7219_clear_display();
    for (uint8_t y = 0; y < 8; y++) {
        for (uint8_t x = 0; x < 4; x++) {
            max7219_set_pixel(x, y, true);
            max7219_set_pixel(x + 8, y, true);
        }
    }
    max7219_update();
    vTaskDelay(pdMS_TO_TICKS(1000));
    // Step 4: Fill entire top matrix
    max7219_clear_display();
    for (uint8_t y = 0; y < 8; y++) {
        for (uint8_t x = 0; x < 8; x++) {
            max7219_set_pixel(x, y, true);
        }
    }
    max7219_update();
    vTaskDelay(pdMS_TO_TICKS(1000));
    // Step 5: Simple falling pixels animation
    max7219_clear_display();
    for (uint8_t y = 0; y < 8; y++) {
        max7219_clear_display();
        max7219_set_pixel(3, y, true);
        max7219_set_pixel(12, y, true);
        max7219_update();
        vTaskDelay(pdMS_TO_TICKS(300));
    }
    // Step 6: Reset display
    max7219_clear_display();
}