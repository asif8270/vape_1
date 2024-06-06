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
#include <zephyr/logging/log.h>

#include "lis2dh12.h"
#include "twi.h"

#define LIS2DH12_WHO_AM_I_VAL 0x33
LOG_MODULE_REGISTER(lis2dh12, LOG_LEVEL_DBG);

int lis2dh12_check_who_am_i(void) {
    uint8_t who_am_i;
    int ret = twi_read(LIS2DH12_I2C_ADDR, LIS2DH12_WHO_AM_I, &who_am_i, 1);
    if (ret != 0) {
        LOG_ERR("Failed to read WHO_AM_I register: %d", ret);
        return ret;
    }

    if (who_am_i != LIS2DH12_WHO_AM_I_VAL) {
        LOG_ERR("WHO_AM_I value mismatch: expected 0x%33, got 0x%02x", LIS2DH12_WHO_AM_I_VAL, who_am_i);
        return -EIO;
    }

    LOG_INF("WHO_AM_I register matched: 0x%02x", who_am_i);
    return 0;
}

bool lis2dh12_is_connected(void) {
    uint8_t dummy = 0;
    int ret = twi_read(LIS2DH12_I2C_ADDR, 0x00, &dummy, 1); // Try to read a byte to check if the device is present
    if (ret == 0) {
        // If the device acknowledged, check the WHO_AM_I register
        ret = lis2dh12_check_who_am_i();
        if (ret == 0) {
            return true;
        }
    }
    return false;
}

int lis2dh12_init(void) {
    uint8_t data;

    // Set data rate to 100 Hz and enable X, Y, Z axes
    data = 0x57; // ODR = 100 Hz, LPen = 0, Zen = 1, Yen = 1, Xen = 1
    int ret = twi_write(LIS2DH12_I2C_ADDR, LIS2DH12_CTRL_REG1, &data, 1);
    if (ret != 0) {
        LOG_ERR("Failed to write to CTRL_REG1: %d", ret);
        return ret;
    }

    // Set full-scale range to ±2g, high-resolution mode off
    data = 0x01; // BDU = 0, BLE = 0, FS = ±2g, HR = 0
    ret = twi_write(LIS2DH12_I2C_ADDR, LIS2DH12_CTRL_REG4, &data, 1);
    if (ret != 0) {
        LOG_ERR("Failed to write to CTRL_REG4: %d", ret);
        return ret;
    }

    // Enable FIFO mode
    data = 0x80; // FM = FIFO mode
    ret = twi_write(LIS2DH12_I2C_ADDR, LIS2DH12_FIFO_CTRL_REG, &data, 1);
    if (ret != 0) {
        LOG_ERR("Failed to write to FIFO_CTRL_REG: %d", ret);
        return ret;
    }

    // Enable FIFO in control register 5
    data = 0x40; // FIFO_EN = 1
    ret = twi_write(LIS2DH12_I2C_ADDR, LIS2DH12_CTRL_REG5, &data, 1);
    if (ret != 0) {
        LOG_ERR("Failed to write to CTRL_REG5: %d", ret);
        return ret;
    }

    return 0;
}

int lis2dh12_read_accel(lis2dh12_accel_data_t* data) {
    uint8_t raw_data[6];
    int ret = twi_read(LIS2DH12_I2C_ADDR, LIS2DH12_OUT_X_L | 0x80, raw_data, 6); // 0x80 for auto-increment
    if (ret != 0) {
        LOG_ERR("Failed to read accelerometer data: %d", ret);
        return ret;
    }

    data->x = (int16_t)((raw_data[1] << 8) | raw_data[0]);
    data->y = (int16_t)((raw_data[3] << 8) | raw_data[2]);
    data->z = (int16_t)((raw_data[5] << 8) | raw_data[4]);

    return 0;
}
