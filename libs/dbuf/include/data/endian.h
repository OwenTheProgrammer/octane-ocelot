#pragma once
#include <stdint.h>

#ifndef __cplusplus
typedef enum endian_t   endian_t;
#endif

/* Represents byte ordering or "endianness." */
enum endian_t
{
    ENDIAN_UNKNOWN  = 0,    /* The endianness is unknown, or invalid. */
    ENDIAN_LITTLE   = 1,    /* Little Endian or LE */
    ENDIAN_BIG      = 2,    /* Big Endian or BE*/
    ENDIAN_MAX              /* Max enumeration value */
};

/*! @brief Sets the endianness of the current value being processed (input).
 *  @param[in] endian The endian of the input data
 */
void endian_set_current(endian_t endian);

/*! @brief Sets the target or wanted endianness when a value has been processed (output).
 *  @param[in] endian The endian of the output data
 */
void endian_set_target(endian_t endian);

/*! @brief Obtains the endian ordering for values being processed.
 *  @return The `current` or "from" endian.
 */
endian_t endian_get_current();

/*! @brief Obtains the endian ordering for values already processed.
 *  @return The `target` or "to" endian.
 */
endian_t endian_get_target();

/*! @brief Evaluates if values in the current context need to switch endianness.
 *  @return `1` when the `current` and `target` endian does not match.
 */
int endian_should_convert();

/*! @brief Converts an endian enum value to its printable string variant.
 *  @param[in] endian The endian value to stringify
 *  @return The cstring representing the `endian` value.
 */
const char* endian_to_string(endian_t endian);

/*! @brief Converts the `target` endian value to its printable string variant.
 *  @return The cstring representing the `target` endian value.
 */ 
const char* endian_target_to_string();

/*! @brief Converts the `current` endian value to its printable string variant.
 *  @return The cstring representing the `current` endian value.
 */
const char* endian_current_to_string();

/*! @brief Ensures a 16-bit unsigned value matches the target endian.
 *  @param[in] value The value to evaluate.
 *  @return `value` byte-ordered according to the target endian.
 */
uint16_t endian_swap_u16(uint16_t value);

/*! @brief Ensures a 32-bit unsigned value matches the target endian.
 *  @param[in] value The value to evaluate.
 *  @return `value` byte-ordered according to the target endian.
 */
uint32_t endian_swap_u32(uint32_t value);

/*! @brief Ensures a dynamically strided unsigned value matches the target endian.
 *  @param[in] value The value to evaluate.
 *  @param[in] stride The width of `value` in bytes.
 *  @return `value` byte-ordered according to the target endian.
 *  @note `stride` expects values from 1 to 4 bytes wide.
 */
uint32_t endian_swap_uvar(uint32_t value, unsigned int stride);

/*! @brief Ensures a 16-bit unsigned value with `value_endian` byte-ordering, matches the target endian.
 *  @param[in] value The value to evaluate.
 *  @param[in] value_endian The endianness of `value`
 *  @return `value` byte-ordered according to the target endian.
 */
uint16_t endian_eval_u16(uint16_t value, endian_t value_endian);

/*! @brief Ensures a dynamically strided signed value matches the target endian.
 * 
 *  If the sign bit of `value` is set (`value` is negative), 
 *  a sign extension will occur to preserve two's complement.
 *  To mitigate improper signature handling, the result is stored as `unsigned`.
 * 
 *  @param[in] value The value to evaluate.
 *  @param[in] stride The width of `value` in bytes.
 *  @return `value` byte-ordered according to the target endian.
 *  @note `stride` expects values from 1 to 4 bytes wide.
 */
uint32_t endian_load_svar(uint32_t value, unsigned int stride);
