#ifndef MPU6050_H
#define MPU6050_H

#include "esp_err.h"

#define MPU6050_I2C_ADDR        0x68  // Default address
#define MPU6050_SDA_GPIO        8
#define MPU6050_SCL_GPIO        9
#define MPU6050_I2C_PORT        I2C_NUM_0

typedef struct {
    float accel_x;
    float accel_y;
    float accel_z;
    float gyro_x;
    float gyro_y;
    float gyro_z;
    float temperature;
} mpu6050_data_t;

esp_err_t mpu6050_init(void);
esp_err_t mpu6050_read_sensor(mpu6050_data_t *data);

#endif // MPU6050_H
