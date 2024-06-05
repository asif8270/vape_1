/**
 * @file      twi.h
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

#ifndef TWI_H_
#define TWI_H_

#include <stdint.h>

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
int twi_init(const uint32_t scl_pin, const uint32_t sda_pin);

/**
 * @brief Enables the TWI peripheral. The peripheral must be initialized beforehand
 * using \ref twi_init
 * 
 * @return 0        Mutex locked.
 * @return -EBUSY   Returned without waiting.
 * @return -EINVAL  The mutex is not locked
 */
int twi_enable(void);

/**
 * @brief Disables the TWI peripheral. The peripheral must be initialized beforehand
 * using \ref twi_init
 * 
 * @return 0        Mutex locked.
 * @return -EBUSY   Returned without waiting.
 * @return -EINVAL  The mutex is not locked
 */
int twi_disable(void);

/**
 * @brief Writes data to a register of a device on the TWI line, and
 * prints an error message if the write fails.
 *
 * @param[in] device_address Address of the device
 * @param[in] reg_address    Address of the register to write to
 * @param[in] p_data         Pointer to the data buffer that has to be written
 * @param[in] length         Length of the data buffer that has to be written
 *
 * @return 0 on success
 * @return -ENOTTY on error.
 */
int twi_write(uint8_t device_address, uint8_t reg_address, uint8_t * p_data, uint16_t length);

/**
 * @brief Reads data from a register of a device on the TWI line, and 
 * prints an error message if the read fails.
 * 
 * @param[in]  device_address Address of the device
 * @param[in]  reg_address    Address of the register to read from 
 * @param[out] p_data         Pointer to a buffer that will store the data
 * @param[in]  length         Length of the data that has to be read
 * 
 * @return 0 on success
 * @return -ENOTTY on error.
 */
int twi_read(const uint8_t device_address, uint8_t reg_address, uint8_t * p_data, const uint16_t length);

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
int twi_read_single(const uint8_t device_address, uint8_t * p_data, const uint16_t length);

int twi_write_single(const uint8_t device_address, uint8_t reg_address, uint8_t reg_value);

#endif // TWI_H_