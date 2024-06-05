#include <zephyr/kernel.h>
#include <zephyr/device.h>
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

    err = lis2dh12_init();
    if (err != 0)
    {
        printk("\rCould not lis2dh12_init\n");
        return 0;
    }
    else
    {
        printk("\rlis2dh12_init\n");
    }
    ret = lis2dh12_enable_fifo();
    if (ret != 0)
    {
        printk("Failed to enable FIFO on LIS2DH12\n");
        return 0;
    }
    while (1)
    {
        // Read accelerometer values
        ret = lis2dh12_read_accel(&accel_x, &accel_y, &accel_z);
        if (ret == 0)
        {
            printk("Accel X: %d, Y: %d, Z: %d\n", accel_x, accel_y, accel_z);
        }
        else
        {
            printk("Failed to read accelerometer values\n");
        }

        k_sleep(K_MSEC(500));
    }

    return 0;
}
