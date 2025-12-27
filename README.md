# Pico Grove Multichannel Gas Sensor V2.0 Driver

![Work in Progress](https://img.shields.io/badge/status-work%20in%20progress-yellow)

A C library for the **Seeed Studio Grove Multichannel Gas Sensor V2.0**, designed for the Raspberry Pi Pico (RP2040, RP2350) using the official Pico SDK.

This is a port of the original [Seeed Arduino Library](https://github.com/Seeed-Studio/Seeed_Arduino_MultiGas), refactored to be lightweight, CMake-friendly, and easy to integrate into existing Pico projects.

## Supported Hardware
* **MCU:** Raspberry Pi Pico, Pico W, Pico 2, or any RP2040/RP2350-based board.
* **Sensor:** [Grove - Multichannel Gas Sensor V2.0](https://wiki.seeedstudio.com/Grove-Multichannel-Gas-Sensor-V2/)
    * **NO2** (Nitrogen Dioxide)
    * **C2H5OH** (Ethanol)
    * **VOC** (Volatile Organic Compounds)
    * **CO** (Carbon Monoxide)

## Features
* **CMake Integration:** Built natively for the Pico SDK build system; easily importable via `FetchContent`.
* **Lightweight:** Pure C implementation with no external dependencies other than `pico_stdlib` and `hardware_i2c`.
* **Raw Data Access:** Provides direct access to the raw ADC values from the sensor's internal STM32 MCU.

## Wiring Guide
The sensor uses I2C. Connect it to any valid I2C pair on the Pico.

| Sensor Pin | Pico Pin (Example) | Description          |
| :---       | :---               | :---                 |
| GND        | GND                | Ground               |
| VCC        | 3V3(OUT)           | 3.3V Power           |
| SDA        | GP4 (I2C0 SDA)     | I2C Data             |
| SCL        | GP5 (I2C0 SCL)     | I2C Clock            |

## Installation
This library is designed to be added to your project using CMake's `FetchContent`.
1. Open your project's `CMakeLists.txt`.
2. Add the following block before you define your executable:
    ```cmake
    include(FetchContent)

    FetchContent_Declare(
        grove_multigas_v2
        GIT_REPOSITORY https://github.com/IoT-gamer/pico-grove-multigas-v2.git
        GIT_TAG        main # or specify a release tag
    )

    FetchContent_MakeAvailable(grove_multigas_v2)
    ```

3. Link the library to your target executable:
    ```cmake
    add_executable(my_project main.c)

    target_link_libraries(my_project 
        pico_stdlib 
        hardware_i2c
        grove_multigas_v2
    )
    ```

## Quick Start Example
Here is a simple `main.c` example to read all four gas channels.
```c
#include <stdio.h>
#include "pico/stdlib.h"
#include "grove_multigas_v2.h"

// I2C Configuration
#define I2C_PORT        i2c0
#define I2C_SDA_PIN     4
#define I2C_SCL_PIN     5

int main() {
    stdio_init_all();
    sleep_ms(2000); // Wait for serial monitor
    
    printf("Initializing Multichannel Gas Sensor V2.0...\n");

    // 1. Initialize I2C Hardware
    i2c_init(I2C_PORT, 100 * 1000); // 100 kHz
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    
    // Enable pull-ups (optional if module has them, but good practice)
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    // 2. Initialize Sensor Driver
    // Default address is 0x08
    if (!multigas_v2_init(I2C_PORT, MULTIGAS_V2_DEFAULT_ADDR)) {
        printf("FAILED: Could not communicate with sensor.\n");
        while(1) tight_loop_contents();
    }

    printf("Sensor Initialized.\n");

    while (true) {
        // 3. Read Data
        uint32_t val_no2    = multigas_v2_read_no2();
        uint32_t val_c2h5oh = multigas_v2_read_c2h5oh();
        uint32_t val_voc    = multigas_v2_read_voc();
        uint32_t val_co     = multigas_v2_read_co();

        // 4. Print Results
        printf("NO2: %lu | Ethanol: %lu | VOC: %lu | CO: %lu\n", 
               val_no2, val_c2h5oh, val_voc, val_co);

        sleep_ms(1000);
    }
}
```
## API Reference
### Initialization
```c
bool multigas_v2_init(i2c_inst_t *i2c, uint8_t addr);
```
Initializes the driver struct. Returns `true` if the sensor acknowledges the I2C address.
### Reading Data
```c
uint32_t multigas_v2_read_no2(void);
uint32_t multigas_v2_read_c2h5oh(void);
uint32_t multigas_v2_read_voc(void);
uint32_t multigas_v2_read_co(void);
```
### Utilities
```c
float multigas_v2_calc_vol(uint32_t raw_val);
```
Converts the raw ADC value to a voltage (0.0V - 3.3V) based on 1024-step resolution.

## Troubleshooting
* **Reading 0 values:** Ensure the sensor is powered. The MiCS-6814 heating element consumes significant current; ensure your Pico's 3.3V rail can supply it.

* **Initialization Fails:** If using long wires, try lowering the I2C frequency to 50kHz.

* **Sensor Warm-up:** These gas sensors require a "burn-in" period. Values may drift significantly for the first 10-30 minutes of operation.

## License
This project is licensed under the MIT License - see the `LICENSE` file for details.

Based on the original work by Wayen Weng for [Seeed Studio](https://github.com/Seeed-Studio/Seeed_Arduino_MultiGas).