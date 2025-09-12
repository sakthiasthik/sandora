#include "max7219_led.h"
#include "driver/spi_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// SPI device handle
static spi_device_handle_t spi;

#define MAX7219_REG_DIGIT0      0x01
#define MAX7219_REG_DIGIT7      0x08
#define MAX7219_REG_DECODEMODE  0x09
#define MAX7219_REG_INTENSITY   0x0A
#define MAX7219_REG_SCANLIMIT   0x0B
#define MAX7219_REG_SHUTDOWN    0x0C
#define MAX7219_REG_DISPLAYTEST 0x0F

// Send one command to MAX7219
static void max7219_send(uint8_t reg, uint8_t data)
{
    uint8_t tx_data[2] = { reg, data };
    spi_transaction_t t = {
        .length = 16,
        .tx_buffer = tx_data
    };
    spi_device_transmit(spi, &t);
}

void max7219_init()
{
    spi_bus_config_t buscfg = {
        .mosi_io_num = MAX7219_MOSI_PIN,
        .miso_io_num = -1,
        .sclk_io_num = MAX7219_CLK_PIN,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };
    spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 10000000,
        .mode = 0,
        .spics_io_num = MAX7219_CS_PIN,
        .queue_size = 1,
    };
    spi_bus_add_device(SPI2_HOST, &devcfg, &spi);

    max7219_send(MAX7219_REG_SCANLIMIT, 0x07);
    max7219_send(MAX7219_REG_DECODEMODE, 0x00);
    max7219_send(MAX7219_REG_SHUTDOWN, 0x01);
    max7219_send(MAX7219_REG_DISPLAYTEST, 0x00);
    max7219_send(MAX7219_REG_INTENSITY, 0x08);

    // Clear display
    for (int i = 1; i <= 8; i++) {
        max7219_send(i, 0x00);
    }
}

void max7219_draw(const uint8_t *pattern)
{
    for (int row = 0; row < 8; row++) {
        max7219_send(row + 1, pattern[row]);
    }
}

void max7219_heartbeat_animation()
{
    const uint8_t HEART_FRAME_0[8] = {
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000
    };

    const uint8_t HEART_FRAME_1[8] = {
        0b00000000,
        0b00000000,
        0b00100100,
        0b00100100,
        0b00011000,
        0b00000000,
        0b00000000,
        0b00000000
    };

    const uint8_t HEART_FRAME_2[8] = {
        0b00000000,
        0b00100100,
        0b01111110,
        0b01111110,
        0b00111100,
        0b00011000,
        0b00000000,
        0b00000000
    };

    const uint8_t HEART_FRAME_3[8] = {
        0b00100100,
        0b01111110,
        0b11111111,
        0b11111111,
        0b01111110,
        0b00111100,
        0b00011000,
        0b00000000
    };

    const uint8_t *frames[] = {
        HEART_FRAME_0,
        HEART_FRAME_1,
        HEART_FRAME_2,
        HEART_FRAME_3,
        HEART_FRAME_2,
        HEART_FRAME_1,
        HEART_FRAME_0
    };

    const int frame_count = sizeof(frames) / sizeof(frames[0]);

    for (int i = 0; i < frame_count; i++) {
        max7219_draw(frames[i]);
        vTaskDelay(pdMS_TO_TICKS(120));
    }
}
