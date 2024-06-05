/**
 * @file      twi.c
 * @author    Usman Mehmood (usmanmehmood55@gmail.com)
 * 
 * @brief     This is a very rudimentary implementation of a TWI or i2c library
 *            using the nrfx library. The read-write functions can be modified
 *            and expanded into more functions such as single-register read/write,
 *            non-stop write, device scanning etc.
 * 
 * @version   0.1
 * @date      2023-08-14
 * @copyright 2023, Usman Mehmood
 */

#include <zephyr/kernel.h>
#include <zephyr/irq.h>
#include <nrfx_twi.h>
#include <zephyr/logging/log.h>
#include "twi.h"
#include "utils.h"

LOG_MODULE_REGISTER(twi_component, LOG_LEVEL_INF);

#define TWI_INSTANCE_ID   0             ///< 0 -> NRFX_TWI0, which has been configured in prj.conf
#define NO_FLAGS          (uint32_t)0U  ///< 0 -> default settings for nrfx_twi_xfer
#define NO_CONTEXT        NULL          ///< NULL -> no context passed to nrfx_twi_init 
#define NO_HANDLER        NULL          ///< NULL -> no event handler passed to nrfx_twi_init 

/**
 * @brief Used to ensure thread safety of the TWI peripheral, and its functioning
 * as a singleton.
 */
static struct k_mutex twi_mutex = { 0 };

/**
 * @brief This instance is being set/populated by the NRFX_TWI_INSTANCE macro
 * which gives all the relevant parameters to a nrfx_twi_t variable. Any use
 * of this instance must be guarded by the twi_mutex lock.
 */
static const nrfx_twi_t twi_instance = NRFX_TWI_INSTANCE(TWI_INSTANCE_ID);

/**
 * @brief Initializes the TWI peripheral based on given SCL and SDA pins, and 
 * prints an error message if the initialization fails.
 * 
 * @param[in] scl_pin SCL pin number
 * @param[in] sda_pin SDA pin number
 * 
 * @return 0 on success
 * @return -ENOTTY on error.
 */
int twi_init(const uint32_t scl_pin, const uint32_t sda_pin)
{
    int err = -ENOTTY;
    nrfx_err_t nrfx_err = NRFX_ERROR_NOT_SUPPORTED;

    /**
     * @brief NRFX_TWI_DEFAULT_CONFIG is used to fill the default values for the bus
     * priority etc, but this nrfx_twi_config_t struct can be manually filled
     * as well if any other settings are to be used
     */
    const nrfx_twi_config_t config = NRFX_TWI_DEFAULT_CONFIG(scl_pin, sda_pin);

    nrfx_err = nrfx_twi_init(&twi_instance, &config, NO_HANDLER, NO_CONTEXT);
    if (nrfx_err == NRFX_SUCCESS)
    {
        err = k_mutex_init(&twi_mutex);
        if (err != 0)
        {
            LOG_ERR("Unable to initialize a mutex for the TWI component, err: %d", err);
        }
    }
    else
    {
        LOG_ERR("twi_init:nrfx_twi_init failed with error: %s", nrfx_err_string(nrfx_err));
    }

    return err;
}

/**
 * @brief Enables the TWI peripheral. The peripheral must be initialized beforehand
 * using \ref twi_init
 * 
 * @return 0        Mutex locked.
 * @return -EBUSY   Returned without waiting.
 * @return -EINVAL  The mutex is not locked
 */
int twi_enable(void)
{
    int err;

    err = k_mutex_lock(&twi_mutex, K_FOREVER);
    if (err == 0)
    {
        nrfx_twi_enable(&twi_instance);
        err = k_mutex_unlock(&twi_mutex);
        if (err != 0)
        {
            LOG_ERR("twi_enable:k_mutex_unlock for unlocking twi_mutex failed with error: %d", err);
        }
    }
    else
    {
        LOG_ERR("twi_enable:k_mutex_lock for locking twi_mutex failed with error: %d", err);
    }

    return err;
}

/**
 * @brief Disables the TWI peripheral. The peripheral must be initialized beforehand
 * using \ref twi_init
 * 
 * @return 0        Mutex locked.
 * @return -EBUSY   Returned without waiting.
 * @return -EINVAL  The mutex is not locked
 */
int twi_disable(void)
{
    int err;

    err = k_mutex_lock(&twi_mutex, K_FOREVER);
    if (err == 0)
    {
        nrfx_twi_disable(&twi_instance);
        err = k_mutex_unlock(&twi_mutex);
        if (err != 0)
        {
            LOG_ERR("twi_enable:k_mutex_unlock for unlocking twi_mutex failed with error: %d", err);
        }
    }
    else
    {
        LOG_ERR("twi_enable:k_mutex_lock for locking twi_mutex failed with error: %d", err);
    }

    return err;
}

/**
 * @brief Writes data to a register of a device on the TWI line, and
 * prints an error message if the write fails.
 *
 * @param[in] device_address Address of the device
 * @param[in] reg_address    Address of the register to write to
 * @param[in] p_data         Pointer to the data buffer that has to be written
 * @param[in] length         Length of the data buffer that has to be written
 *
 * @return 0 on success,
 * @return -ENOTTY on error.
 */
int twi_write(const uint8_t device_address, uint8_t reg_address, uint8_t * p_data, const uint16_t length)
{
    int err;
    nrfx_err_t nrfx_err = NRFX_SUCCESS;

    err = k_mutex_lock(&twi_mutex, K_FOREVER);
    if (err == 0)
    {
        // Writing the device address and the target register address on the TWI line with a NO_STOP flag
        nrfx_twi_xfer_desc_t xfer_desc_reg_address = NRFX_TWI_XFER_DESC_TX(device_address, &reg_address, sizeof(reg_address));
        nrfx_err = nrfx_twi_xfer(&twi_instance, &xfer_desc_reg_address, NRFX_TWI_FLAG_TX_NO_STOP);
        if (nrfx_err == NRFX_SUCCESS)
        {
            // Writing the data on the target register
            nrfx_twi_xfer_desc_t xfer_desc_data = NRFX_TWI_XFER_DESC_TX(device_address, p_data, length);
            nrfx_err = nrfx_twi_xfer(&twi_instance, &xfer_desc_data, NO_FLAGS);
            if (nrfx_err == NRFX_SUCCESS)
            {
                err = 0;
            }
            else
            {
                LOG_ERR("twi_write:nrfx_twi_xfer for writing reg_address failed with error: %s", nrfx_err_string(nrfx_err));
            }
        }
        else
        {
            LOG_ERR("twi_write:nrfx_twi_xfer for writing data failed with error: %s", nrfx_err_string(nrfx_err));
        }

        err = k_mutex_unlock(&twi_mutex);
        if (err != 0)
        {
            LOG_ERR("twi_write:k_mutex_unlock for unlocking twi_mutex failed with error: %d", err);
        }
    }
    else
    {
        LOG_ERR("twi_write:k_mutex_lock for locking twi_mutex failed with error: %d", err);
    }

    return (nrfx_err != NRFX_SUCCESS) ? -ENOTTY : err;
}

/**
 * @brief Reads data from a register of a device on the TWI line, and 
 * prints an error message if the read fails.
 * 
 * @param[in]  device_address Address of the device
 * @param[in]  reg_address    Address of the register to read from 
 * @param[out] p_data         Pointer to a buffer that will store the data
 * @param[in]  length         Length of the data that has to be read
 * 
 * @return 0 on success,
 * @return -ENOTTY on error.
 */
int twi_read(const uint8_t device_address, uint8_t reg_address, uint8_t * p_data, const uint16_t length)
{
    int err;
    nrfx_err_t nrfx_err = NRFX_SUCCESS;
    
    err = k_mutex_lock(&twi_mutex, K_FOREVER);
    if (err == 0)
    {
        // Writing the device address and the target register address on the TWI line with a NO_STOP flag
        nrfx_twi_xfer_desc_t xfer_desc_reg_address = NRFX_TWI_XFER_DESC_TX(device_address, &reg_address, sizeof(reg_address));
        nrfx_err = nrfx_twi_xfer(&twi_instance, &xfer_desc_reg_address, NRFX_TWI_FLAG_TX_NO_STOP);
        if (nrfx_err == NRFX_SUCCESS)
        {
            // Reading the data on the target register
            nrfx_twi_xfer_desc_t xfer_desc_data = NRFX_TWI_XFER_DESC_RX(device_address, p_data, length);
            nrfx_err = nrfx_twi_xfer(&twi_instance, &xfer_desc_data, NO_FLAGS);
            if (nrfx_err == NRFX_SUCCESS)
            {
                err = 0;
            }
            else
            {
                LOG_ERR("twi_read:nrfx_twi_xfer for writing reg_address failed with error: %s", nrfx_err_string(nrfx_err));
            }
        }
        else
        {
            LOG_ERR("twi_read:nrfx_twi_xfer for reading data failed with error: %s", nrfx_err_string(nrfx_err));
        }

        err = k_mutex_unlock(&twi_mutex);
        if (err != 0)
        {
            LOG_ERR("twi_read:k_mutex_unlock for unlocking twi_mutex failed with error: %d", err);
        }
    }
    else
    {
        LOG_ERR("twi_read:k_mutex_lock for locking twi_mutex failed with error: %d", err);
    }

    return (nrfx_err != NRFX_SUCCESS) ? -ENOTTY : err;
}

/**
 * @brief Reads data from device on the TWI line, and 
 * prints an error message if the read fails.
 * 
 * @param[in]  device_address Address of the device
 * @param[out] p_data         Pointer to a buffer that will store the data
 * @param[in]  length         Length of the data that has to be read
 * 
 * @return 0 on success
 * @return -ENOTTY on error.
 */
int twi_read_single(const uint8_t device_address, uint8_t * p_data, const uint16_t length)
{
    int err;
    nrfx_err_t nrfx_err = NRFX_SUCCESS;
    
    err = k_mutex_lock(&twi_mutex, K_FOREVER);
    if (err != 0)
    {
       LOG_ERR("twi_just_read:k_mutex_lock for locking twi_mutex failed with error: %d", err);
       return err;
    }

    // Sending a read request to the device
    nrfx_twi_xfer_desc_t xfer_desc_data = NRFX_TWI_XFER_DESC_RX(device_address, p_data, length);
    nrfx_err = nrfx_twi_xfer(&twi_instance, &xfer_desc_data, NO_FLAGS);
    if (nrfx_err != NRFX_SUCCESS)
    {
        LOG_ERR("twi_just_read:nrfx_twi_xfer for writing reg_address failed with error: %s", nrfx_err_string(nrfx_err));
    }

    err = k_mutex_unlock(&twi_mutex);
    if (err != 0)
    {
        LOG_ERR("twi_just_read:k_mutex_unlock for unlocking twi_mutex failed with error: %d", err);
    }

    return (nrfx_err != NRFX_SUCCESS) ? -ENOTTY : err;
}

int twi_write_single(const uint8_t device_address, uint8_t reg_address, uint8_t reg_value)
{
    int err;
    nrfx_err_t nrfx_err = NRFX_SUCCESS;
    uint8_t packet[2] = { reg_address, reg_value };

    err = k_mutex_lock(&twi_mutex, K_FOREVER);
    if (err == 0)
    {
        nrfx_twi_xfer_desc_t xfer_desc_full = NRFX_TWI_XFER_DESC_TX(device_address, packet, sizeof(packet));
        nrfx_err = nrfx_twi_xfer(&twi_instance, &xfer_desc_full, NO_FLAGS);
        if (nrfx_err == NRFX_SUCCESS)
        {
            err = 0;
        }
        else
        {
            LOG_ERR("twi_write:nrfx_twi_xfer for writing data failed with error: %s", nrfx_err_string(nrfx_err));
        }

        err = k_mutex_unlock(&twi_mutex);
        if (err != 0)
        {
            LOG_ERR("twi_write:k_mutex_unlock for unlocking twi_mutex failed with error: %d", err);
        }
    }
    else
    {
        LOG_ERR("twi_write:k_mutex_lock for locking twi_mutex failed with error: %d", err);
    }

    return (nrfx_err != NRFX_SUCCESS) ? -ENOTTY : err;
}