#include "mpu6050.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "MPU6050";

// Register addresses
#define MPU6050_PWR_MGMT_1      0x6B
#define MPU6050_ACCEL_XOUT_H    0x3B

static esp_err_t mpu6050_write_byte(uint8_t reg, uint8_t data)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    esp_err_t ret;

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (MPU6050_I2C_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg, true);
    i2c_master_write_byte(cmd, data, true);
    i2c_master_stop(cmd);

    ret = i2c_master_cmd_begin(MPU6050_I2C_PORT, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);
    return ret;
}

static esp_err_t mpu6050_read_bytes(uint8_t reg, uint8_t *data, size_t len)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    esp_err_t ret;

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (MPU6050_I2C_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (MPU6050_I2C_ADDR << 1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd, data, len, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);

    ret = i2c_master_cmd_begin(MPU6050_I2C_PORT, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);
    return ret;
}

esp_err_t mpu6050_init(void)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = MPU6050_SDA_GPIO,
        .scl_io_num = MPU6050_SCL_GPIO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 400000,
    };
    ESP_ERROR_CHECK(i2c_param_config(MPU6050_I2C_PORT, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(MPU6050_I2C_PORT, I2C_MODE_MASTER, 0, 0, 0));

    // Wake up MPU6050 from sleep
    esp_err_t ret = mpu6050_write_byte(MPU6050_PWR_MGMT_1, 0x00);
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "MPU6050 initialized");
    } else {
        ESP_LOGE(TAG, "Failed to initialize MPU6050");
    }
    return ret;
}

esp_err_t mpu6050_read_sensor(mpu6050_data_t *sensor_data)
{
    uint8_t raw[14];
    esp_err_t ret = mpu6050_read_bytes(MPU6050_ACCEL_XOUT_H, raw, 14);
    if (ret != ESP_OK) return ret;

    // sensor_data->accel_x = (raw[0] << 8) | raw[1];
    // sensor_data->accel_y = (raw[2] << 8) | raw[3];
    // sensor_data->accel_z = (raw[4] << 8) | raw[5];
    // sensor_data->temperature_raw = (raw[6] << 8) | raw[7];
    // sensor_data->gyro_x = (raw[8] << 8) | raw[9];
    // sensor_data->gyro_y = (raw[10] << 8) | raw[11];
    // sensor_data->gyro_z = (raw[12] << 8) | raw[13];

    int16_t ax = (raw[0] << 8) | raw[1];
    int16_t ay = (raw[2] << 8) | raw[3];
    int16_t az = (raw[4] << 8) | raw[5];
    int16_t temp_raw = (raw[6] << 8) | raw[7];
    int16_t gx = (raw[8] << 8) | raw[9];
    int16_t gy = (raw[10] << 8) | raw[11];
    int16_t gz = (raw[12] << 8) | raw[13];

    // Convert to real-world units
    sensor_data->accel_x = ax / 16384.0f;
    sensor_data->accel_y = ay / 16384.0f;
    sensor_data->accel_z = az / 16384.0f;

    sensor_data->temperature = (temp_raw / 340.0f) + 36.53f;

    sensor_data->gyro_x = gx / 131.0f;
    sensor_data->gyro_y = gy / 131.0f;
    sensor_data->gyro_z = gz / 131.0f;

    return ESP_OK;
}
