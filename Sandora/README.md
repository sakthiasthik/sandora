Pinouts for 8x8 dot led rgb matrix MAX7219 Dot Matrix Module

for c3 
GPIO4 --> sck 
gpio6 --> din
gpio7 --> cs 
// SPI pins (can be overridden)
#define MAX7219_MOSI_PIN 6
#define MAX7219_CLK_PIN 4
#define MAX7219_CS_PIN 7



#define MPU6050_I2C_ADDR        0x68  // Default address
#define MPU6050_SDA_GPIO        8
#define MPU6050_SCL_GPIO        9