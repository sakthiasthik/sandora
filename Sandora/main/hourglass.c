#include "hourglass.h"
#include "max7219_led.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"



static uint32_t total_duration_ms = 6000; // default 10 min
static uint32_t elapsed_ms = 0;
static uint8_t sand_level = 0; // how much has fallen


static const char *TAG = "HourGlass";

void hourglass_init(uint32_t duration_ms) 
{

    ESP_LOGI(TAG, "Hourglass task INIT");
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

// void hourglass_update(Orientation_t orientation) 
// {
//     if (orientation == ORIENT_UNKNOWN || orientation == ORIENT_FLAT) return;

//     ESP_LOGI(TAG, "Sand level: %d", sand_level);

//     uint32_t step_time = total_duration_ms / 64; // 64 pixels = total sand
//     elapsed_ms += 2000; // assume called every 2000ms

//     if (elapsed_ms >= step_time) 
//     {
//         // elapsed_ms = 0;
//         elapsed_ms -= step_time;

//         if (orientation == ORIENT_UPRIGHT && sand_level < 64) 
//         {
//             // Drop one grain from top to bottom
//             uint8_t top_y = 3 - (sand_level / 16);
//             uint8_t bottom_y = 4 + (sand_level / 16);
//             uint8_t x = sand_level % 16;

//             max7219_set_pixel(x, top_y, false);
//             max7219_set_pixel(x, bottom_y, true);
//             max7219_update();

//             sand_level++;
//             ESP_LOGI(TAG, "Sand level: %d (Down)", sand_level);
//         }
//         else if (orientation == ORIENT_FLIPPED && sand_level > 0) 
//         {
//             // Reverse flow
//             sand_level--;

//             uint8_t top_y = 3 - (sand_level / 16);
//             uint8_t bottom_y = 4 + (sand_level / 16);
//             uint8_t x = sand_level % 16;

//             max7219_set_pixel(x, top_y, true);
//             max7219_set_pixel(x, bottom_y, false);
//             max7219_update();
//             ESP_LOGI(TAG, "Sand level: %d (Up)", sand_level);
//         }
//     }
// }


void hourglass_update(Orientation_t orientation) 
{
    if (orientation == ORIENT_UNKNOWN || orientation == ORIENT_FLAT) return;

    uint32_t step_time = total_duration_ms / 64; // 64 pixels = total sand
    elapsed_ms += 2000; // must match vTaskDelay timing

    if (elapsed_ms >= step_time) 
    {
        elapsed_ms -= step_time;

        if (orientation == ORIENT_UPRIGHT && sand_level < 64) 
        {
            uint8_t top_y = 3 - (sand_level / 16);
            uint8_t bottom_y = 4 + (sand_level / 16);
            uint8_t x = sand_level % 16;

            max7219_set_pixel(x, top_y, false);
            max7219_set_pixel(x, bottom_y, true);
            max7219_update();

            sand_level++;
            ESP_LOGI(TAG, "Sand level: %d (Down)", sand_level);
        }
        else if (orientation == ORIENT_FLIPPED && sand_level > 0) 
        {
            sand_level--;

            uint8_t top_y = 3 - (sand_level / 16);
            uint8_t bottom_y = 4 + (sand_level / 16);
            uint8_t x = sand_level % 16;

            max7219_set_pixel(x, top_y, true);
            max7219_set_pixel(x, bottom_y, false);
            max7219_update();

            ESP_LOGI(TAG, "Sand level: %d (Up)", sand_level);
        }
    }
}
