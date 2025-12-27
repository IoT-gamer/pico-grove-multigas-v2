#include "grove_multigas_v2.h"
#include <stdio.h>

static i2c_inst_t *_sensor_i2c = NULL;
static uint8_t _sensor_addr = MULTIGAS_V2_DEFAULT_ADDR;

bool multigas_v2_init(i2c_inst_t *i2c, uint8_t addr) {
    _sensor_i2c = i2c;
    _sensor_addr = addr;

    // Simple check: attempt to read a dummy byte or just check if device ACKs
    uint8_t dummy_cmd = 0x00;
    int ret = i2c_write_blocking(_sensor_i2c, _sensor_addr, &dummy_cmd, 1, false);
    return ret >= 0;
}

static uint32_t _read_sensor_metric(uint8_t cmd) {
    if (!_sensor_i2c) return 0;

    // 1. Send Command Byte
    int ret = i2c_write_blocking(_sensor_i2c, _sensor_addr, &cmd, 1, false);
    if (ret < 0) return 0; // Error

    // 2. Wait for conversion (Arduino library uses delay)
    sleep_ms(2); 

    // 3. Read 4 bytes (uint32_t)
    uint8_t buffer[4];
    ret = i2c_read_blocking(_sensor_i2c, _sensor_addr, buffer, 4, false);
    if (ret < 0) return 0;

    // 4. Combine bytes (Little Endian usually, check datasheet)
    // Assuming standard Seeed pattern: Byte0 = LSB
    uint32_t value = buffer[0] | (buffer[1] << 8) | (buffer[2] << 16) | (buffer[3] << 24);
    return value;
}

uint32_t multigas_v2_read_no2(void) {
    return _read_sensor_metric(CMD_READ_NO2);
}

uint32_t multigas_v2_read_c2h5oh(void) {
    return _read_sensor_metric(CMD_READ_C2H5OH);
}

uint32_t multigas_v2_read_voc(void) {
    return _read_sensor_metric(CMD_READ_VOC);
}

uint32_t multigas_v2_read_co(void) {
    return _read_sensor_metric(CMD_READ_CO);
}

float multigas_v2_calc_vol(uint32_t raw_val) {
    // Standard calculation from Seeed library: Raw * 3.3V / Resolution (1023 or similar?)
    // The Arduino library often returns 'val' directly. 
    // If the sensor returns raw ADC (0-1023), voltage = val * 3.3 / 1023.0;
    // However, some versions of the firmware return pre-calculated values.
    // Adjust logic based on observation.
    return (float)raw_val * 3.3f / 1023.0f; // Example conversion
}