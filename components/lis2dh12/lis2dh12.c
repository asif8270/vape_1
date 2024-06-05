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

int lis2dh12_init()
{
    uint8_t ctrl_reg = 0x57U;
    int err;
    err = twi_write(LIS2DH12_I2C_ADDR, LIS2DH12_CTRL_REG1, &ctrl_reg, sizeof(ctrl_reg));
    if (err != 0)
    {
        printk("\rFailed to twi_write");
        return 0;
    }
    
    return 0;
}