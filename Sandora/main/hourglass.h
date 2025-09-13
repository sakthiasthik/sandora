#ifndef HOURGLASS_H
#define HOURGLASS_H

#include <stdint.h>
#include "mpu6050.h"

// Initialize with total duration in ms (e.g. 10 min → 600000)
void hourglass_init(uint32_t duration_ms);

// Reset the hourglass (refill top)
void hourglass_reset(void);

// Update animation based on orientation
void hourglass_update(Orientation_t orientation);

#endif // HOURGLASS_H
