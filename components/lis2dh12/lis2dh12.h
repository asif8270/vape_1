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