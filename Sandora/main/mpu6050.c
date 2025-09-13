#include "mpu6050.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include <math.h>

static const char *TAG = "MPU6050";

// Helper: Write to a register
static esp_err_t mpu6050_write_byte(uint8_t reg, uint8_t data) 
{
    return i2c_master_write_to_device(MPU6050_I2C_PORT, MPU6050_I2C_ADDR, & (uint8_t){reg, data}, 2, 100 / portTICK_PERIOD_MS);
}

// Helper: Read multiple bytes
static esp_err_t mpu6050_read_bytes(uint8_t reg, uint8_t *data, size_t len) 
{
    return i2c_master_write_read_device(MPU6050_I2C_PORT, MPU6050_I2C_ADDR, &reg, 1, data, len, 100 / portTICK_PERIOD_MS);
}

esp_err_t mpu6050_init(void) 
{
    esp_err_t ret;

    // I2C config
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = MPU6050_SDA_GPIO,
        .scl_io_num = MPU6050_SCL_GPIO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 400000  // 400kHz
    };
    ret = i2c_param_config(MPU6050_I2C_PORT, &conf);
    if (ret != ESP_OK) return ret;

    ret = i2c_driver_install(MPU6050_I2C_PORT, conf.mode, 0, 0, 0);
    if (ret != ESP_OK) return ret;

    // Wake up MPU6050 (clear sleep bit)
    ret = mpu6050_write_byte(MPU6050_REG_PWR_MGMT_1, 0x00);
    if (ret != ESP_OK) return ret;

    ESP_LOGI(TAG, "MPU6050 initialized successfully");
    return ESP_OK;
}

esp_err_t mpu6050_get_orientation(mpu6050_orientation_t *orientation) 
{
    uint8_t data[14];
    int16_t acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z;

    esp_err_t ret = mpu6050_read_bytes(MPU6050_REG_ACCEL_XOUT_H, data, 14);
    if (ret != ESP_OK) return ret;

    // Accelerometer raw values
    acc_x = (int16_t)((data[0] << 8) | data[1]);
    acc_y = (int16_t)((data[2] << 8) | data[3]);
    acc_z = (int16_t)((data[4] << 8) | data[5]);

    // Gyroscope raw values (not used heavily for now)
    gyro_x = (int16_t)((data[8] << 8) | data[9]);
    gyro_y = (int16_t)((data[10] << 8) | data[11]);
    gyro_z = (int16_t)((data[12] << 8) | data[13]);

    // Convert to G values (assuming default ±2g)
    float ax = acc_x / 16384.0f;
    float ay = acc_y / 16384.0f;
    float az = acc_z / 16384.0f;

    // Calculate angles in degrees
    orientation->pitch = atan2f(ax, sqrtf(ay * ay + az * az)) * 180.0f / M_PI;
    orientation->roll  = atan2f(ay, sqrtf(ax * ax + az * az)) * 180.0f / M_PI;
    orientation->yaw   = atan2f(sqrtf(ax * ax + ay * ay), az) * 180.0f / M_PI;

    return ESP_OK;
}

void mpu6050_test(void) 
{
    mpu6050_orientation_t ori;
    while (1) 
    {
        if (mpu6050_get_orientation(&ori) == ESP_OK) 
        {
            ESP_LOGI(TAG, "Pitch: %.2f | Roll: %.2f | Yaw: %.2f", ori.pitch, ori.roll, ori.yaw);
        } 
        else 
        {
            ESP_LOGE(TAG, "Failed to read MPU6050");
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
