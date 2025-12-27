#include <stdio.h>
#include "pico/stdlib.h"
#include "grove_multigas_v2.h"

#define I2C_PORT i2c0
#define SDA_PIN 4
#define SCL_PIN 5

int main() {
    stdio_init_all();

    // Initialize I2C
    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    // Initialize Sensor
    if (!multigas_v2_init(I2C_PORT, MULTIGAS_V2_DEFAULT_ADDR)) {
        printf("Sensor not found!\n");
        while(1);
    }

    while (1) {
        uint32_t val_no2 = multigas_v2_read_no2();
        uint32_t val_co  = multigas_v2_read_co();

        printf("NO2 Raw: %lu, CO Raw: %lu\n", val_no2, val_co);
        sleep_ms(1000);
    }
}