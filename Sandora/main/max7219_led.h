#ifndef MAX7219_LED_H
#define MAX7219_LED_H

#include <stdbool.h>
#include <stdint.h>
#include "driver/spi_master.h"

// ======= CONFIGURATION =======
#define MAX7219_CHAINED_DEVICES   2   // Number of cascaded MAX7219 modules (2 for hourglass top+bottom)
#define MAX7219_MATRIX_WIDTH      8
#define MAX7219_MATRIX_HEIGHT     8
#define MAX7219_TOTAL_WIDTH       (MAX7219_CHAINED_DEVICES * MAX7219_MATRIX_WIDTH)
#define MAX7219_TOTAL_HEIGHT      MAX7219_MATRIX_HEIGHT

// SPI pins (already provided by you)
#define MAX7219_MOSI_PIN 6
#define MAX7219_CLK_PIN  4
#define MAX7219_CS_PIN   7

// ======= PUBLIC FUNCTIONS =======
void max7219_init(void);
void max7219_clear(void);
void max7219_set_pixel(uint8_t x, uint8_t y, bool on);
void max7219_update(void);

// Extra helpers for hourglass project
void hourglass_reset(void);
void hourglass_fill_level(uint8_t top_pixels);  // e.g. set how many pixels in "top sand"

// Test helper
void max7219_test_pattern(void);

#endif // MAX7219_LED_H
