// // #include "hourglass.h"
// // #include "max7219_led.h"
// // #include "freertos/FreeRTOS.h"
// // #include "freertos/task.h"
// // #include "esp_log.h"



// // static uint32_t total_duration_ms = 6000; // default 10 min
// // static uint32_t elapsed_ms = 0;
// // static uint8_t sand_level = 0; // how much has fallen


// // static const char *TAG = "HourGlass";

// // void hourglass_init(uint32_t duration_ms) 
// // {

// //     ESP_LOGI(TAG, "Hourglass task INIT");
// //     total_duration_ms = duration_ms;
// //     hourglass_reset();
// // }

// // void hourglass_reset(void) 
// // {
// //     elapsed_ms = 0;
// //     sand_level = 0;
// //     max7219_clear();

// //     // Fill top half initially
// //     for (uint8_t y = 0; y < 4; y++) 
// //     {
// //         for (uint8_t x = 0; x < 16; x++) 
// //         {
// //             max7219_set_pixel(x, y, true);
// //         }
// //     }
// //     max7219_update();
// // }

// // // void hourglass_update(Orientation_t orientation) 
// // // {
// // //     if (orientation == ORIENT_UNKNOWN || orientation == ORIENT_FLAT) return;

// // //     ESP_LOGI(TAG, "Sand level: %d", sand_level);

// // //     uint32_t step_time = total_duration_ms / 64; // 64 pixels = total sand
// // //     elapsed_ms += 2000; // assume called every 2000ms

// // //     if (elapsed_ms >= step_time) 
// // //     {
// // //         // elapsed_ms = 0;
// // //         elapsed_ms -= step_time;

// // //         if (orientation == ORIENT_UPRIGHT && sand_level < 64) 
// // //         {
// // //             // Drop one grain from top to bottom
// // //             uint8_t top_y = 3 - (sand_level / 16);
// // //             uint8_t bottom_y = 4 + (sand_level / 16);
// // //             uint8_t x = sand_level % 16;

// // //             max7219_set_pixel(x, top_y, false);
// // //             max7219_set_pixel(x, bottom_y, true);
// // //             max7219_update();

// // //             sand_level++;
// // //             ESP_LOGI(TAG, "Sand level: %d (Down)", sand_level);
// // //         }
// // //         else if (orientation == ORIENT_FLIPPED && sand_level > 0) 
// // //         {
// // //             // Reverse flow
// // //             sand_level--;

// // //             uint8_t top_y = 3 - (sand_level / 16);
// // //             uint8_t bottom_y = 4 + (sand_level / 16);
// // //             uint8_t x = sand_level % 16;

// // //             max7219_set_pixel(x, top_y, true);
// // //             max7219_set_pixel(x, bottom_y, false);
// // //             max7219_update();
// // //             ESP_LOGI(TAG, "Sand level: %d (Up)", sand_level);
// // //         }
// // //     }
// // // }


// // void hourglass_update(Orientation_t orientation) 
// // {
// //     if (orientation == ORIENT_UNKNOWN || orientation == ORIENT_FLAT) return;

// //     uint32_t step_time = total_duration_ms / 64; // 64 pixels = total sand
// //     elapsed_ms += 2000; // must match vTaskDelay timing

// //     if (elapsed_ms >= step_time) 
// //     {
// //         elapsed_ms -= step_time;

// //         if (orientation == ORIENT_UPRIGHT && sand_level < 64) 
// //         {
// //             uint8_t top_y = 3 - (sand_level / 16);
// //             uint8_t bottom_y = 4 + (sand_level / 16);
// //             uint8_t x = sand_level % 16;

// //             max7219_set_pixel(x, top_y, false);
// //             max7219_set_pixel(x, bottom_y, true);
// //             max7219_update();

// //             sand_level++;
// //             ESP_LOGI(TAG, "Sand level: %d (Down)", sand_level);
// //         }
// //         else if (orientation == ORIENT_FLIPPED && sand_level > 0) 
// //         {
// //             sand_level--;

// //             uint8_t top_y = 3 - (sand_level / 16);
// //             uint8_t bottom_y = 4 + (sand_level / 16);
// //             uint8_t x = sand_level % 16;

// //             max7219_set_pixel(x, top_y, true);
// //             max7219_set_pixel(x, bottom_y, false);
// //             max7219_update();

// //             ESP_LOGI(TAG, "Sand level: %d (Up)", sand_level);
// //         }
// //     }
// // }
























// #include "hourglass.h"
// #include "max7219_led.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_log.h"

// static uint32_t total_duration_ms = 60000; // default 60 seconds for testing
// static uint32_t elapsed_ms = 0;
// static uint8_t sand_level = 0; // how much has fallen (0-64)
// static bool is_flipped = false; // Track if device was flipped to reset animation

// static const char *TAG = "HourGlass";

// // Hourglass shape - defines which pixels should be part of the hourglass
// static bool is_hourglass_pixel(uint8_t x, uint8_t y) {
//     // Top part (triangle)
//     if (y < 4) {
//         return (x >= y && x < 16 - y);
//     }
//     // Bottom part (inverted triangle)
//     else {
//         return (x >= (15 - y) && x < (y + 1));
//     }
// }

// void hourglass_init(uint32_t duration_ms) 
// {
//     ESP_LOGI(TAG, "Hourglass initialized with duration: %lu ms", duration_ms);
//     total_duration_ms = duration_ms;
//     hourglass_reset();
// }

// void hourglass_reset(void) 
// {
//     elapsed_ms = 0;
//     sand_level = 0;
//     is_flipped = false;
//     max7219_clear_display();
    
//     // Draw hourglass shape with sand in top
//     for (uint8_t y = 0; y < 8; y++) {
//         for (uint8_t x = 0; x < 16; x++) {
//             if (is_hourglass_pixel(x, y)) {
//                 // Fill top half with sand
//                 if (y < 4) {
//                     max7219_set_pixel(x, y, true);
//                 } else {
//                     max7219_set_pixel(x, y, false);
//                 }
//             }
//         }
//     }
//     max7219_update();
//     ESP_LOGI(TAG, "Hourglass reset");
// }

// void hourglass_update(Orientation_t orientation) 
// {
//     // Check for device flip to reset animation
//     if (orientation == ORIENT_FLIPPED && !is_flipped) {
//         is_flipped = true;
//         ESP_LOGI(TAG, "Device flipped, resetting animation");
//         hourglass_reset();
//         return;
//     } else if (orientation == ORIENT_UPRIGHT) {
//         is_flipped = false;
//     }
    
//     // Only process sand movement when upright
//     if (orientation != ORIENT_UPRIGHT) {
//         return;
//     }
    
//     uint32_t step_time = total_duration_ms / 64; // 64 pixels = total sand
//     elapsed_ms += 2000; // must match vTaskDelay timing
    
//     if (elapsed_ms >= step_time && sand_level < 64) 
//     {
//         elapsed_ms -= step_time;
        
//         // Calculate which pixel to move
//         uint8_t pixel_index = sand_level;
//         uint8_t x, top_y, bottom_y;
        
//         // Map pixel index to actual position in hourglass
//         if (pixel_index < 16) {
//             // First row of top section
//             x = pixel_index;
//             top_y = 3;
//             bottom_y = 4;
//         } else if (pixel_index < 32) {
//             // Second row of top section
//             x = pixel_index - 16;
//             top_y = 2;
//             bottom_y = 5;
//         } else if (pixel_index < 48) {
//             // Third row of top section
//             x = pixel_index - 32;
//             top_y = 1;
//             bottom_y = 6;
//         } else {
//             // Fourth row of top section
//             x = pixel_index - 48;
//             top_y = 0;
//             bottom_y = 7;
//         }
        
//         // Only move if it's a valid hourglass pixel
//         if (is_hourglass_pixel(x, top_y) && is_hourglass_pixel(x, bottom_y)) {
//             // Move pixel from top to bottom
//             max7219_set_pixel(x, top_y, false);
//             max7219_set_pixel(x, bottom_y, true);
//             max7219_update();
            
//             sand_level++;
//             ESP_LOGI(TAG, "Moved pixel (%d,%d) to (%d,%d). Sand level: %d/64", 
//                      x, top_y, x, bottom_y, sand_level);
//         }
//     }
// }










#include "hourglass.h"
#include "max7219_led.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static uint32_t total_duration_ms = 60000; // default 60 seconds for testing
static uint32_t elapsed_ms = 0;
static uint8_t sand_level = 0; // how much has fallen (0-64)
static bool is_flipped = false; // Track if device was flipped to reset animation

static const char *TAG = "HourGlass";

// Hourglass shape - defines which pixels should be part of the hourglass
static bool is_hourglass_pixel(uint8_t x, uint8_t y) {
    // Top matrix (first 8 columns)
    if (x < 8) {
        // Top part (triangle) - rows 0-3
        if (y < 4) {
            // Centered triangle: widest at row 3, narrowest at row 0
            uint8_t center = 3; // Center of the 8-column matrix
            uint8_t width = y * 2 + 1; // Width increases with row
            uint8_t left = center - (width / 2);
            uint8_t right = center + (width / 2);
            return (x >= left && x <= right);
        }
        // Bottom part (inverted triangle) - rows 4-7
        else {
            uint8_t center = 3;
            uint8_t row_from_bottom = 7 - y;
            uint8_t width = row_from_bottom * 2 + 1;
            uint8_t left = center - (width / 2);
            uint8_t right = center + (width / 2);
            return (x >= left && x <= right);
        }
    }
    // Bottom matrix (columns 8-15)
    else {
        // Adjust x to be 0-7 for the second matrix
        uint8_t adj_x = x - 8;
        
        // Top part (triangle) - rows 0-3
        if (y < 4) {
            uint8_t center = 3;
            uint8_t width = y * 2 + 1;
            uint8_t left = center - (width / 2);
            uint8_t right = center + (width / 2);
            return (adj_x >= left && adj_x <= right);
        }
        // Bottom part (inverted triangle) - rows 4-7
        else {
            uint8_t center = 3;
            uint8_t row_from_bottom = 7 - y;
            uint8_t width = row_from_bottom * 2 + 1;
            uint8_t left = center - (width / 2);
            uint8_t right = center + (width / 2);
            return (adj_x >= left && adj_x <= right);
        }
    }
}

void hourglass_init(uint32_t duration_ms) 
{
    ESP_LOGI(TAG, "Hourglass initialized with duration: %lu ms", duration_ms);
    total_duration_ms = duration_ms;
    hourglass_reset();
}

void hourglass_reset(void) 
{
    elapsed_ms = 0;
    sand_level = 0;
    is_flipped = false;
    max7219_clear_display();
    
    // Draw hourglass shape with sand in top
    for (uint8_t y = 0; y < 8; y++) {
        for (uint8_t x = 0; x < 16; x++) {
            if (is_hourglass_pixel(x, y)) {
                // Fill top half with sand
                if (y < 4) {
                    max7219_set_pixel(x, y, true);
                } else {
                    max7219_set_pixel(x, y, false);
                }
            }
        }
    }
    max7219_update();
    ESP_LOGI(TAG, "Hourglass reset");
}

void hourglass_update(Orientation_t orientation) 
{
    // Check for device flip to reset animation
    if (orientation == ORIENT_FACE_DOWN && !is_flipped) {
        is_flipped = true;
        ESP_LOGI(TAG, "Device flipped to FACE_DOWN, resetting animation");
        hourglass_reset();
        return;
    } 
    // Reset flip state when device returns to normal position
    else if (orientation == ORIENT_FLAT || orientation == ORIENT_UNKNOWN) {
        is_flipped = false;
    }
    
    // Only process sand movement when in normal position (FLAT or FACE_UP)
    if (orientation != ORIENT_FLAT && orientation != ORIENT_UNKNOWN) {
        return;
    }
    
    uint32_t step_time = total_duration_ms / 64; // 64 pixels = total sand
    elapsed_ms += 2000; // must match vTaskDelay timing
    
    if (elapsed_ms >= step_time && sand_level < 64) 
    {
        elapsed_ms -= step_time;
        
        // Calculate which pixel to move
        uint8_t pixel_index = sand_level;
        uint8_t x, top_y, bottom_y;
        
        // Map pixel index to actual position in hourglass
        if (pixel_index < 16) {
            // First row of top section
            x = pixel_index;
            top_y = 3;
            bottom_y = 4;
        } else if (pixel_index < 32) {
            // Second row of top section
            x = pixel_index - 16;
            top_y = 2;
            bottom_y = 5;
        } else if (pixel_index < 48) {
            // Third row of top section
            x = pixel_index - 32;
            top_y = 1;
            bottom_y = 6;
        } else {
            // Fourth row of top section
            x = pixel_index - 48;
            top_y = 0;
            bottom_y = 7;
        }
        
        // Only move if it's a valid hourglass pixel
        if (is_hourglass_pixel(x, top_y) && is_hourglass_pixel(x, bottom_y)) {
            // Move pixel from top to bottom
            max7219_set_pixel(x, top_y, false);
            max7219_set_pixel(x, bottom_y, true);
            max7219_update();
            
            sand_level++;
            ESP_LOGI(TAG, "Moved pixel (%d,%d) to (%d,%d). Sand level: %d/64", 
                     x, top_y, x, bottom_y, sand_level);
        }
    }
}

// // Test function for sand falling animation
// // Test function for sand falling animation
// void hourglass_test_animation(void) 
// {
//     ESP_LOGI(TAG, "Starting hourglass test animation");
    
//     // Reset the display
//     max7219_clear_display();
    
//     // Define the hourglass shape for the top module (module 1, x=8-15)
//     // Top module should have an inverted triangle shape (wider at the bottom)
//     for (uint8_t y = 0; y < 8; y++) {
//         for (uint8_t x = 8; x < 16; x++) {
//             // Calculate the distance from the center of the module (x=11.5)
//             float center_x = 11.5f;
//             float distance = fabsf(x - center_x);
            
//             // The width of the triangle increases as we go down (y increases)
//             float max_distance = y * 0.8f; // Adjust the multiplier to control the shape
            
//             if (distance <= max_distance) {
//                 max7219_set_pixel(x, y, true);
//             }
//         }
//     }
    
//     // Define the hourglass shape for the bottom module (module 0, x=0-7)
//     // Bottom module should have a triangle shape (wider at the bottom)
//     for (uint8_t y = 0; y < 8; y++) {
//         for (uint8_t x = 0; x < 8; x++) {
//             // Calculate the distance from the center of the module (x=3.5)
//             float center_x = 3.5f;
//             float distance = fabsf(x - center_x);
            
//             // The width of the triangle increases as we go down (y increases)
//             float max_distance = (7 - y) * 0.8f; // Inverted compared to top module
            
//             if (distance <= max_distance) {
//                 // Initially, don't turn on the bottom module (it starts empty)
//                 max7219_set_pixel(x, y, false);
//             }
//         }
//     }
    
//     // Update the display to show the initial state
//     max7219_update();
//     vTaskDelay(pdMS_TO_TICKS(1000));
    
//     // Create a list of all sand pixels in the top module
//     uint8_t sand_pixels[64][2]; // [x, y] coordinates
//     uint8_t pixel_count = 0;
    
//     // Collect all sand pixels in the top module (module 1)
//     for (uint8_t y = 7; y >= 0; y--) {  // Start from bottom row
//         for (uint8_t x = 15; x >= 8; x--) {  // Start from rightmost column
//             // Check if this pixel is part of the hourglass shape
//             float center_x = 11.5f;
//             float distance = fabsf(x - center_x);
//             float max_distance = y * 0.8f;
            
//             if (distance <= max_distance && pixel_count < 64) {
//                 sand_pixels[pixel_count][0] = x;
//                 sand_pixels[pixel_count][1] = y;
//                 pixel_count++;
//             }
//         }
//     }
    
//     ESP_LOGI(TAG, "Found %d sand pixels in top module", pixel_count);
    
//     // Animate sand falling from top module to bottom module
//     for (uint8_t i = 0; i < pixel_count; i++) {
//         // Get the current sand pixel in the top module
//         uint8_t top_x = sand_pixels[i][0];
//         uint8_t top_y = sand_pixels[i][1];
        
//         // Calculate the corresponding position in the bottom module
//         // Map from top module (8-15) to bottom module (0-7)
//         // And from top_y to a mirrored position in the bottom module
//         uint8_t bottom_x = 15 - top_x;  // This maps 15->0, 14->1, ..., 8->7
//         uint8_t bottom_y = 7 - top_y;   // This maps 7->0, 6->1, ..., 0->7
        
//         // Turn off the pixel in the top module
//         max7219_set_pixel(top_x, top_y, false);
        
//         // Turn on the pixel in the bottom module
//         max7219_set_pixel(bottom_x, bottom_y, true);
        
//         // Update the display
//         max7219_update();
        
//         ESP_LOGI(TAG, "Moved pixel from (%d,%d) to (%d,%d)", top_x, top_y, bottom_x, bottom_y);
        
//         // Delay for animation effect
//         vTaskDelay(pdMS_TO_TICKS(200));
//     }
    
//     ESP_LOGI(TAG, "Test animation complete");
    
//     // Wait a moment and reset
//     vTaskDelay(pdMS_TO_TICKS(3000));
//     max7219_clear_display();
// }

void hourglass_test_animation(void)
{
    ESP_LOGI(TAG, "Starting new hourglass sand fall animation");

    // Clear display
    max7219_clear_display();

    // Loop over all 8 sand grains
    for (uint8_t i = 0; i < 8; i++) 
    {
        uint8_t top_x = 15 - i;  // Module 1 (top): x from 15 to 8
        uint8_t top_y = 7;       // Bottom row of top module

        uint8_t bottom_x = i;    // Module 0 (bottom): x from 0 to 7
        uint8_t bottom_y = 0;    // Top row of bottom module

        // Light up the grain in the top module
        max7219_set_pixel(top_x, top_y, true);
        max7219_update();
        vTaskDelay(pdMS_TO_TICKS(200));

        // Turn off grain from top module
        max7219_set_pixel(top_x, top_y, false);

        // Show falling animation through intermediate rows (optional)
        for (int mid_y = 6; mid_y >= 1; mid_y--) {
            max7219_set_pixel(top_x, mid_y, true);
            max7219_update();
            vTaskDelay(pdMS_TO_TICKS(100));
            max7219_set_pixel(top_x, mid_y, false);
        }

        // Turn on grain in the bottom module
        max7219_set_pixel(bottom_x, bottom_y, true);
        max7219_update();

        ESP_LOGI(TAG, "Moved sand grain from (%d,%d) to (%d,%d)", top_x, top_y, bottom_x, bottom_y);

        vTaskDelay(pdMS_TO_TICKS(300));
    }

    ESP_LOGI(TAG, "Test animation complete");

    // Wait before resetting
    vTaskDelay(pdMS_TO_TICKS(2000));
    hourglass_reset();
}



// void hourglass_turn_on_led(void) 
// {
//     // Turn on the LED at position (x=3, y=0) in the first module (row 1)
//     max7219_set_pixel(3, 0, true);  // First module, row 1, 3rd LED (x=3, y=0)
    
//     // Turn on the LED at position (x=4, y=1) in the second module (row 2)
//     max7219_set_pixel(4 + 8, 1, true);  // Second module, row 2, 4th LED (x=4 + 8, y=1)

//     // Update the display after turning on the LEDs
//     max7219_update();
// }

void hourglass_turn_on_led(void) 
{
    // Turn on the LED at position (x=3, y=0) in the first module (row 1)
    max7219_set_pixel(2, 0, true);  // First module, row 1, 3rd LED (x=3, y=0)
    
    // Turn on the LED at position (x=4, y=1) in the second module (row 2)
    max7219_set_pixel(3 + 8, 1, true);  // Second module, row 2, 4th LED (x=4 + 8, y=1)

    // Update the display after turning on the LEDs
    max7219_update();
}
