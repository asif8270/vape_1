/*
 * The library is not extensively tested and only
 * meant as a simple explanation and for inspiration.
 * NO WARRANTY of ANY KIND is provided.
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <nrfx_twi.h>
#include "twi.h"
#include "nrf_drv_mpu.h"

#define MPU_ADDRESS               0x68
#define MPU_AK89XX_MAGN_ADDRESS   0x0C

int nrf_drv_mpu_init(void)
{
    return 0; // there is no extra initialization required.
}

int nrf_drv_mpu_write_registers(uint8_t reg, uint8_t *p_data, uint32_t length)
{
    return twi_write(MPU_ADDRESS, reg, p_data, length);
}

int nrf_drv_mpu_write_single_register(uint8_t reg, uint8_t data)
{
    return twi_write(MPU_ADDRESS, reg, &data, 1U);
}

int nrf_drv_mpu_read_registers(uint8_t reg, uint8_t *p_data, uint32_t length)
{
    return twi_read(MPU_ADDRESS, reg, p_data, length);
}

int nrf_drv_mpu_read_magnetometer_registers(uint8_t reg, uint8_t *p_data, uint32_t length)
{
    return twi_read(MPU_AK89XX_MAGN_ADDRESS, reg, p_data, length);
}

int nrf_drv_mpu_write_magnetometer_register(uint8_t reg, uint8_t data)
{
    return twi_write(MPU_AK89XX_MAGN_ADDRESS, reg, &data, 1U);
}
