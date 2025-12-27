#ifndef GROVE_MULTIGAS_V2_H
#define GROVE_MULTIGAS_V2_H

#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

// Default I2C Address for V2.0 Sensor
#define MULTIGAS_V2_DEFAULT_ADDR 0x08

// Sensor Command Codes
// NO2=0x01, C2H5OH=0x02, VOC=0x03, CO=0x04
#define CMD_READ_NO2      0x01 // GM_102B
#define CMD_READ_C2H5OH   0x02 // GM_302B
#define CMD_READ_VOC      0x03 // GM_502B
#define CMD_READ_CO       0x04 // GM_702B
#define CMD_CHANGE_ADDR   0x55 // Example command to change I2C addr

/**
 * @brief Initialize the gas sensor driver.
 * @param i2c The I2C instance (i2c0 or i2c1).
 * @param addr The I2C address (default 0x08).
 * @return true if sensor acknowledged, false otherwise.
 */
bool multigas_v2_init(i2c_inst_t *i2c, uint8_t addr);

/**
 * @brief Read raw value from NO2 sensor (GM102B).
 * @return Raw ADC value (uint32_t) or 0 on error.
 */
uint32_t multigas_v2_read_no2(void);

/**
 * @brief Read raw value from Ethanol sensor (GM302B).
 * @return Raw ADC value.
 */
uint32_t multigas_v2_read_c2h5oh(void);

/**
 * @brief Read raw value from VOC sensor (GM502B).
 * @return Raw ADC value.
 */
uint32_t multigas_v2_read_voc(void);

/**
 * @brief Read raw value from CO sensor (GM702B).
 * @return Raw ADC value.
 */
uint32_t multigas_v2_read_co(void);

/**
 * @brief Convert raw reading to voltage (V).
 * The sensor returns a value relative to reference voltage (3.3V typically).
 */
float multigas_v2_calc_vol(uint32_t raw_val);

#ifdef __cplusplus
}
#endif

#endif // GROVE_MULTIGAS_V2_H