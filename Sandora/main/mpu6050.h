// // #ifndef MPU6050_H
// // #define MPU6050_H

// // #include "esp_err.h"
// // #include "freertos/FreeRTOS.h"
// // #include "freertos/task.h"


// // // I2C configuration
// // #define MPU6050_I2C_ADDR        0x68
// // #define MPU6050_SDA_GPIO        8
// // #define MPU6050_SCL_GPIO        9
// // #define MPU6050_I2C_PORT        I2C_NUM_0

// // // MPU6050 registers
// // #define MPU6050_REG_PWR_MGMT_1  0x6B
// // #define MPU6050_REG_ACCEL_XOUT_H 0x3B
// // #define MPU6050_REG_GYRO_XOUT_H  0x43

// // typedef struct {
// //     float pitch;   // Forward / backward tilt
// //     float roll;    // Left / right tilt
// //     float yaw;     // Rotation around vertical axis
// // } mpu6050_orientation_t;

// // /**
// //  * @brief Initialize MPU6050 sensor
// //  */
// // esp_err_t mpu6050_init(void);

// // /**
// //  * @brief Read processed orientation (pitch, roll, yaw) from MPU6050
// //  */
// // esp_err_t mpu6050_get_orientation(mpu6050_orientation_t *orientation);

// // /**
// //  * @brief Simple test function (reads and prints orientation values)
// //  */
// // void mpu6050_test(void);

// // #endif // MPU6050_H



















// #ifndef MPU6050_H
// #define MPU6050_H

// #include "esp_err.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"

// // I2C configuration
// #define MPU6050_I2C_ADDR        0x68
// #define MPU6050_SDA_GPIO        8
// #define MPU6050_SCL_GPIO        9
// #define MPU6050_I2C_PORT        I2C_NUM_0

// // MPU6050 registers
// #define MPU6050_REG_PWR_MGMT_1   0x6B
// #define MPU6050_REG_ACCEL_XOUT_H 0x3B
// #define MPU6050_REG_GYRO_XOUT_H  0x43

// typedef struct {
//     float pitch;   // Forward / backward tilt
//     float roll;    // Left / right tilt
//     float yaw;     // Rotation around vertical axis
// } mpu6050_orientation_t;

// // // Orientation types
// // typedef enum {
// //     ORIENT_UPRIGHT,
// //     ORIENT_FLIPPED,
// //     ORIENT_FLAT,
// //     ORIENT_UNKNOWN
// // } Orientation_t;

// typedef enum {
//     ORIENT_UPRIGHT,
//     ORIENT_FLIPPED,
//     ORIENT_FLAT,
//     ORIENT_FACE_DOWN,  // NEW
//     ORIENT_UNKNOWN
// } Orientation_t;


// /**
//  * @brief Initialize MPU6050 sensor
//  */
// esp_err_t mpu6050_init(void);

// /**
//  * @brief Read filtered pitch, roll, and yaw using complementary filter
//  */
// esp_err_t mpu6050_get_orientation(mpu6050_orientation_t *orientation);

// /**
//  * @brief Determine basic orientation state from accelerometer (e.g., UPRIGHT)
//  */
// Orientation_t MPU6050_GetOrientation(void);

// /**
//  * @brief Convert orientation enum to string
//  */
// const char* orientation_to_string(Orientation_t orientation);

// /**
//  * @brief Simple test task: logs pitch, roll, yaw and orientation state
//  */
// void mpu6050_test(void);

// Orientation_t mpu6050_get_orientation_state(void);

// #endif // MPU6050_H





















#ifndef MPU6050_H
#define MPU6050_H
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// I2C configuration
#define MPU6050_I2C_ADDR        0x68
#define MPU6050_SDA_GPIO        8
#define MPU6050_SCL_GPIO        9
#define MPU6050_I2C_PORT        I2C_NUM_0
// MPU6050 registers
#define MPU6050_REG_PWR_MGMT_1   0x6B
#define MPU6050_REG_ACCEL_XOUT_H 0x3B
#define MPU6050_REG_GYRO_XOUT_H  0x43
typedef struct {
    float pitch;   // Forward / backward tilt
    float roll;    // Left / right tilt
    float yaw;     // Rotation around vertical axis
} mpu6050_orientation_t;
typedef enum {
    ORIENT_UPRIGHT,
    ORIENT_FLIPPED,
    ORIENT_FLAT,
    ORIENT_FACE_DOWN,
    ORIENT_UNKNOWN
} Orientation_t;
/**
 * @brief Initialize MPU6050 sensor
 */
esp_err_t mpu6050_init(void);
/**
 * @brief Read filtered pitch, roll, and yaw using complementary filter
 */
esp_err_t mpu6050_get_orientation(mpu6050_orientation_t *orientation);
/**
 * @brief Determine basic orientation state from accelerometer (e.g., UPRIGHT)
 */
Orientation_t MPU6050_GetOrientation(void);
/**
 * @brief Convert orientation enum to string
 */
const char* orientation_to_string(Orientation_t orientation);
/**
 * @brief Simple test task: logs pitch, roll, yaw and orientation state
 */
void mpu6050_test(void);
Orientation_t mpu6050_get_orientation_state(void);
#endif // MPU6050_H