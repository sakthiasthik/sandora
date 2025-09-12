#ifndef MAX7219_LED_H
#define MAX7219_LED_H

#include <stdint.h>

// SPI pins (can be overridden)
#define MAX7219_MOSI_PIN 6
#define MAX7219_CLK_PIN 4
#define MAX7219_CS_PIN 7

// Public functions
void max7219_init(void);
void max7219_draw(const uint8_t *pattern);
void max7219_heartbeat_animation(void);

#endif // MAX7219_LED_H
