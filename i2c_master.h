/**
* AVRKIT
* https://github.com/nczeroshift/avrkit/
*/

#ifndef _AVRKIT_CLI_I2C_MASTER_H_
#define _AVRKIT_CLI_I2C_MASTER_H_

#include <stdint.h>
#include "settings.h"

void I2C_Init(uint32_t clock, uint64_t f_cpu);

uint8_t I2C_Random_Read(uint8_t dev_id,
    uint8_t dev_addr,
    uint8_t addr, uint8_t *data);

uint8_t I2C_Sequencial_Read(uint8_t dev_id,
    uint8_t dev_addr,
    uint8_t addr, uint8_t * data, uint8_t size);

uint8_t I2C_Page_Write(uint8_t dev_id,
    uint8_t dev_addr,
    uint8_t addr, uint8_t * data, uint8_t size);

uint8_t I2C_Byte_Write(uint8_t dev_id,
    uint8_t dev_addr,
    uint8_t addr, uint8_t data);

#endif
