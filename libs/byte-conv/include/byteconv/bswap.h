#pragma once
#include <stdint.h>

/*! @brief Performs a byteswap on a 16 bit unsigned value.
 *  @param[in] src The value to be swapped
 * 	@return The byte swapped value
 */
uint16_t bswap_u16(uint16_t src);

/*! @brief Performs a byteswap on a 32 bit unsigned value.
 *  @param[in] src The value to be swapped
 * 	@return The byte swapped value
 */
uint32_t bswap_u32(uint32_t src);

/*! @brief Performs a byteswap on a variable stride unsigned value.
 *	@param[in] src The value to be swapped
 * 	@param[in] stride The width of the `src` in bytes from 1-4
 * 	@return The byte swapped value
 * 	@note No pre-masking is needed, as the value is left shifted to the LSB.
 */
uint32_t bswap_uvar(uint32_t src, unsigned int stride);
