/**
 * @file utils.c
 * @author Usman Mehmood (u.mehmood@ssp.de.com)
 * @brief 
 * @version 0.1
 * @date 01-02-2024
 * 
 * @copyright 2024, SSP - Safety System Products
 * 
 */

#include "utils.h"
#include <zephyr/sys/printk.h>

void print_buffer(const void * p_data, const uint16_t length)
{
    printk("\r\n + ======= len: %03u ======= +\n", length);
    for (uint16_t i = 0; i < length; i++)
    {
        uint8_t data = ((uint8_t *)p_data)[i]; // cppcheck-suppress cstyleCast
        printk("\r | data[%03u] = 0x%02X, %03u, %c |\n", i, data, data, data);
    }
    printk("\r + ======================== +\n");
}

/**
 * @brief Helper function to provide string representation of the nrfx error
 * 
 * @param[in] err error code
 * 
 * @return string of the error code
 */
const char * nrfx_err_string(const nrfx_err_t err)
{
    const char * result;

    switch (err) 
    {
        case NRFX_SUCCESS:                   result = "Operation performed successfully.";                  break;
        case NRFX_ERROR_INTERNAL:            result = "Internal error.";                                    break;
        case NRFX_ERROR_NO_MEM:              result = "No memory for operation.";                           break;
        case NRFX_ERROR_NOT_SUPPORTED:       result = "Not supported.";                                     break;
        case NRFX_ERROR_INVALID_PARAM:       result = "Invalid parameter.";                                 break;
        case NRFX_ERROR_INVALID_STATE:       result = "Invalid state, operation disallowed in this state."; break;
        case NRFX_ERROR_INVALID_LENGTH:      result = "Invalid length.";                                    break;
        case NRFX_ERROR_TIMEOUT:             result = "Operation timed out.";                               break;
        case NRFX_ERROR_FORBIDDEN:           result = "Operation is forbidden.";                            break;
        case NRFX_ERROR_NULL:                result = "Null pointer.";                                      break;
        case NRFX_ERROR_INVALID_ADDR:        result = "Bad memory address.";                                break;
        case NRFX_ERROR_BUSY:                result = "Busy.";                                              break;
        case NRFX_ERROR_ALREADY_INITIALIZED: result = "Module already initialized.";                        break;
        case NRFX_ERROR_DRV_TWI_ERR_OVERRUN: result = "TWI error: Overrun.";                                break;
        case NRFX_ERROR_DRV_TWI_ERR_ANACK:   result = "TWI error: Address not acknowledged.";               break;
        case NRFX_ERROR_DRV_TWI_ERR_DNACK:   result = "TWI error: Data not acknowledged.";                  break;
        default:                             result = "Unknown error.";                                     break;
    }

    return result;
}
