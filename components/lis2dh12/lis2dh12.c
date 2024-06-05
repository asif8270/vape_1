/**
 * @file   lis2dh12.c
 * @author Asif Mahmood (asifmehmood.8270@gmail.com)
 * @brief  ??
 * @version 0.1
 * @date 2024-06-05
 *
 * @copyright Asif, 2024
 *
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>

#include "lis2dh12.h"
#include "twi.h"

#define LIS2DH12_WHO_AM_I_VAL 0x33

int lis2dh12_check_who_am_i(void)
{
    uint8_t who_am_i;
    int ret = twi_read(LIS2DH12_I2C_ADDR, LIS2DH12_WHO_AM_I, &who_am_i, 1);
    if (ret != 0)
    {
        LOG_ERR("Failed to read WHO_AM_I register: %d", ret);
        return ret;
    }

    if (who_am_i != LIS2DH12_WHO_AM_I_VAL)
    {
        LOG_ERR("WHO_AM_I value mismatch: expected 0x%02x, got 0x%02x", LIS2DH12_WHO_AM_I_VAL, who_am_i);
        return -EIO;
    }

    LOG_INF("WHO_AM_I register matched: 0x%02x", who_am_i);
    return 0;
}

int lis2dh12_init()
{
    uint8_t ctrl_reg_1 = 0x57U;
    uint8_t ctrl_reg_4 = 0x00U;
    int err;
    err = twi_write(LIS2DH12_I2C_ADDR, LIS2DH12_CTRL_REG1, &ctrl_reg_1, sizeof(ctrl_reg_1));
    if (err != 0)
    {
        printk("\rFailed to twi_write\n");
        return 0;
    }

    err = twi_write(LIS2DH12_I2C_ADDR, LIS2DH12_CTRL_REG4, &ctrl_reg_4, sizeof(ctrl_reg_4));
    if (err != 0)
    {
        printk("\rFailed to twi_write\n");
        return 0;
    }

    return 0;
}

// int lis2dh12_read_accel(accel_values_t *accel_values)
// {
//     int err;
//     uint8_t raw_values_x_h[6];
//     uint8_t raw_values_x_l[6];
//     err = twi_read(LIS2DH12_I2C_ADDR, LIS2DH12_OUT_X_L, &raw_values_x_l, sizeof(raw_values_x_l));
//     if (err != 0)
//     {
//         printk("\rCould not read accel_values\n");
//         return 0;
//     }
//     err = twi_read(LIS2DH12_I2C_ADDR, LIS2DH12_OUT_X_H, &raw_values_x_h, sizeof(raw_values_x_h));
//     if (err != 0)
//     {
//         printk("\rCould not read accel_values\n");
//         return 0;
//     }

//     return 0;
// }

int lis2dh12_enable_fifo(void)
{
    int ret;
    uint8_t data;

    // Configure FIFO_CTRL_REG: Enable FIFO mode
    data = 0x80; // FM = FIFO mode
    ret = twi_write(LIS2DH12_I2C_ADDR, LIS2DH12_FIFO_CTRL_REG, &data, 1);
    if (ret != 0)
        return ret;

    // Configure CTRL_REG5: Enable FIFO
    data = 0x40; // FIFO_EN = 1
    ret = twi_write(LIS2DH12_I2C_ADDR, LIS2DH12_CTRL_REG5, &data, 1);
    if (ret != 0)
        return ret;

    return 0;
}

int lis2dh12_read_accel(int16_t *accel_x, int16_t *accel_y, int16_t *accel_z)
{
    uint8_t data[6];
    int ret;

    // Read OUT_X_L to OUT_Z_H
    ret = twi_read(LIS2DH12_I2C_ADDR, LIS2DH12_OUT_X_L, data, 6);
    if (ret != 0)
        return ret;

    // Combine low and high bytes
    *accel_x = (int16_t)((data[1] << 8) | data[0]);
    *accel_y = (int16_t)((data[3] << 8) | data[2]);
    *accel_z = (int16_t)((data[5] << 8) | data[4]);

    return 0;
}