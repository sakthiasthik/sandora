#include "hourglass.h"
#include "max7219_led.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static uint32_t total_duration_ms = 600000; // default 10 min
static uint32_t elapsed_ms = 0;
static uint8_t sand_level = 0; // how much has fallen

void hourglass_init(uint32_t duration_ms) 
{
    total_duration_ms = duration_ms;
    hourglass_reset();
}

void hourglass_reset(void) 
{
    elapsed_ms = 0;
    sand_level = 0;
    max7219_clear();

    // Fill top half initially
    for (uint8_t y = 0; y < 4; y++) 
    {
        for (uint8_t x = 0; x < 16; x++) 
        {
            max7219_set_pixel(x, y, true);
        }
    }
    max7219_update();
}

void hourglass_update(Orientation_t orientation) 
{
    if (orientation == ORIENT_UNKNOWN || orientation == ORIENT_FLAT) return;

    uint32_t step_time = total_duration_ms / 64; // 64 pixels = total sand
    elapsed_ms += 200; // assume called every 200ms

    if (elapsed_ms >= step_time) 
    {
        elapsed_ms = 0;

        if (orientation == ORIENT_UPRIGHT && sand_level < 64) 
        {
            // Drop one grain from top to bottom
            uint8_t top_y = 3 - (sand_level / 16);
            uint8_t bottom_y = 4 + (sand_level / 16);
            uint8_t x = sand_level % 16;

            max7219_set_pixel(x, top_y, false);
            max7219_set_pixel(x, bottom_y, true);
            max7219_update();

            sand_level++;
        }
        else if (orientation == ORIENT_FLIPPED && sand_level > 0) 
        {
            // Reverse flow
            sand_level--;

            uint8_t top_y = 3 - (sand_level / 16);
            uint8_t bottom_y = 4 + (sand_level / 16);
            uint8_t x = sand_level % 16;

            max7219_set_pixel(x, top_y, true);
            max7219_set_pixel(x, bottom_y, false);
            max7219_update();
        }
    }
}
