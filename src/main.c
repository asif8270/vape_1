#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <stdio.h>
#include <string.h>

#include "twi.h"
#include "lis2dh12.h"

int main(void)
{
    int err;
    int ret;
    int16_t accel_x, accel_y, accel_z;

    err = twi_init(8, 7);
    if (err != 0)
    {
        printk("\rcould not initialize i2c\n");
        return 0;
    }

    err = twi_enable();
    if (err != 0)
    {
        printk("\rcould not enable i2c\n");
        return 0;
    }

    if (lis2dh12_is_connected())
    {
        printk("LIS2DH12 is connected and WHO_AM_I matched.\n");

        // Initialize the sensor
        ret = lis2dh12_init();
        if (ret != 0)
        {
            printk("Failed to initialize LIS2DH12\n");
            return;
        }

        // Read accelerometer data
        lis2dh12_accel_data_t accel_data;
        while (1)
        {
            ret = lis2dh12_read_accel(&accel_data);
            if (ret == 0)
            {
                printk("Accel X: %d, Y: %d, Z: %d\n", accel_data.x, accel_data.y, accel_data.z);
            }
            else
            {
                printk("Failed to read accelerometer data\n");
            }
            k_sleep(K_MSEC(100)); // Sleep for 1 second
        }
    }
    else
    {
        printk("LIS2DH12 is not connected or WHO_AM_I mismatch.\n");
    }

    return 0;
}
