/**
 * @file      lis2dh12.h
 * @author    Asif Mahmood (asifmehmood.8270@gmail.com)
 * @brief     Library implementation for ST LIS2DH12 Accelerometer
 *            and Gyrometer.
 * @version   0.1
 * @date      2024-06-05
 * 
 * @copyright Asif, 2024
 * 
 */

#ifndef LIS2DH12_H_
#define LIS2DH12_H_

#include <stdbool.h>
#include <stdint.h>

// Register addresses
#define LIS2DH12_I2C_ADDR         0x18
#define LIS2DH12_STATUS_REG_AUX   0x07
#define LIS2DH12_OUT_TEMP_L       0x0C
#define LIS2DH12_OUT_TEMP_H       0x0D
#define LIS2DH12_WHO_AM_I         0x0F
#define LIS2DH12_CTRL_REG0        0x1E
#define LIS2DH12_TEMP_CFG_REG     0x1F
#define LIS2DH12_CTRL_REG1        0x20
#define LIS2DH12_CTRL_REG2        0x21
#define LIS2DH12_CTRL_REG3        0x22
#define LIS2DH12_CTRL_REG4        0x23
#define LIS2DH12_CTRL_REG5        0x24
#define LIS2DH12_CTRL_REG6        0x25
#define LIS2DH12_REFERENCE        0x26
#define LIS2DH12_STATUS_REG       0x27
#define LIS2DH12_OUT_X_L          0x28
#define LIS2DH12_OUT_X_H          0x29
#define LIS2DH12_OUT_Y_L          0x2A
#define LIS2DH12_OUT_Y_H          0x2B
#define LIS2DH12_OUT_Z_L          0x2C
#define LIS2DH12_OUT_Z_H          0x2D
#define LIS2DH12_FIFO_CTRL_REG    0x2E
#define LIS2DH12_FIFO_SRC_REG     0x2F
#define LIS2DH12_INT1_CFG         0x30
#define LIS2DH12_INT1_SRC         0x31
#define LIS2DH12_INT1_THS         0x32
#define LIS2DH12_INT1_DURATION    0x33
#define LIS2DH12_INT2_CFG         0x34
#define LIS2DH12_INT2_SRC         0x35
#define LIS2DH12_INT2_THS         0x36
#define LIS2DH12_INT2_DURATION    0x37
#define LIS2DH12_CLICK_CFG        0x38
#define LIS2DH12_CLICK_SRC        0x39
#define LIS2DH12_CLICK_THS        0x3A
#define LIS2DH12_TIME_LIMIT       0x3B
#define LIS2DH12_TIME_LATENCY     0x3C
#define LIS2DH12_TIME_WINDOW      0x3D
#define LIS2DH12_ACT_THS          0x3E
#define LIS2DH12_ACT_DUR          0x3F

/**@brief Enum defining Accelerometer's Full Scale range posibillities in Gs. */
enum accel_range
{
    AFS_2G = 0, // 2 G
    AFS_4G,     // 4 G
    AFS_8G,     // 8 G
    AFS_16G     // 16 G
};

/**@brief Enum defining Gyroscopes? Full Scale range posibillities in Degrees Pr Second. */
enum gyro_range
{
    GFS_250DPS = 0, // 250 deg/s
    GFS_500DPS,     // 500 deg/s
    GFS_1000DPS,    // 1000 deg/s
    GFS_2000DPS     // 2000 deg/s
};

/**@brief Structure to hold acceleromter values.
 * Sequence of z, y, and x is important to correspond with
 * the sequence of which z, y, and x data are read from the sensor.
 * All values are unsigned 16 bit integers
 */
typedef struct
{
    int16_t z;
    int16_t y;
    int16_t x;
} accel_values_t;

/**@brief Structure to hold gyroscope values.
 * Sequence of z, y, and x is important to correspond with
 * the sequence of which z, y, and x data are read from the sensor.
 * All values are unsigned 16 bit integers
 */
typedef struct
{
    int16_t z;
    int16_t y;
    int16_t x;
} gyro_values_t;

/**
 * @brief  LIS2DH12 initialization function
 * 
 * @return int 
 */
int lis2dh12_init(void);

int lis2dh12_register_write();

#endif //LIS2DH12_H_