/**
 * @file utils.h
 * @author Usman Mehmood (u.mehmood@ssp.de.com)
 * @brief 
 * @version 0.1
 * @date 01-02-2024
 * 
 * @copyright 2024, SSP - Safety System Products
 * 
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdint.h>
#include <nrfx.h>

#define LENGTH_OF(array) (sizeof(array) / sizeof(array[0]))

/**
 * @brief Helper function to provide string representation of the nrfx error
 * 
 * @param[in] err error code
 * 
 * @return string of the error code
 */
const char * nrfx_err_string(const nrfx_err_t err);

void print_buffer(const void * p_data, const uint16_t length);

#endif // UTILS_H_