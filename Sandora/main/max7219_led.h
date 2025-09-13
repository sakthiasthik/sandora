// #ifndef MAX7219_LED_H
// #define MAX7219_LED_H

// #include <stdint.h>

// // SPI pins (can be overridden)
// #define MAX7219_MOSI_PIN 6
// #define MAX7219_CLK_PIN 4
// #define MAX7219_CS_PIN 7

// // Public functions
// void max7219_init(void);
// void max7219_draw(const uint8_t *pattern);
// void max7219_heartbeat_animation(void);

// #endif // MAX7219_LED_H






#ifndef MAX7219_LED_H
#define MAX7219_LED_H

#include "driver/spi_master.h"

// SPI pins (can be overridden)
#define MAX7219_MOSI_PIN 6
#define MAX7219_CLK_PIN 4
#define MAX7219_CS_PIN 7

// MAX7219 register addresses
#define MAX7219_REG_NOOP 0x00
#define MAX7219_REG_DIGIT0 0x01
#define MAX7219_REG_DIGIT1 0x02
#define MAX7219_REG_DIGIT2 0x03
#define MAX7219_REG_DIGIT3 0x04
#define MAX7219_REG_DIGIT4 0x05
#define MAX7219_REG_DIGIT5 0x06
#define MAX7219_REG_DIGIT6 0x07
#define MAX7219_REG_DIGIT7 0x08
#define MAX7219_REG_DECODEMODE 0x09
#define MAX7219_REG_INTENSITY 0x0A
#define MAX7219_REG_SCANLIMIT 0x0B
#define MAX7219_REG_SHUTDOWN 0x0C
#define MAX7219_REG_DISPLAYTEST 0x0F

// Function to initialize the MAX7219
esp_err_t max7219_init();

// Function to send a data byte to a specific register
esp_err_t max7219_send(uint8_t reg, uint8_t data);

// Function to clear the display
void max7219_clear();

// Function to update the hourglass animation on the display
void max7219_update_hourglass(int seconds);

void max7219_light_all();

void max7219_update_hourglass_1(int elapsed_time, int total_time);

#endif // MAX7219_LED_H
