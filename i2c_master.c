
#include "i2c_master.h"

#include <avr/io.h>
#include <util/delay.h>
#include <compat/twi.h>
#include <stdio.h>

#define I2C_START       0
#define I2C_STOP        1
#define I2C_DATA_NACK   2
#define I2C_DATA_ACK    3

// Based on Ronald Willem Besinga i2c tutorial (http://www.ermicro.com/blog/?p=744)

void I2C_Init(uint32_t clock)
{
     // Initialize TWI clock: 100 kHz clock, TWPS = 0 => prescaler = 1
     TWSR = 0;                         /* no prescaler */
     TWBR = ((F_CPU/clock)-16)/2;  /* must be > 10 for stable operation */
}

unsigned char I2C_Transmit(uint8_t type) {
    switch(type) {
        case I2C_START:     // Send start condition
            TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
            break;
        case I2C_DATA_NACK: // Send data
            TWCR = (1 << TWINT) | (1 << TWEN);
            break;
        case I2C_DATA_ACK:  // Send data with acknowledge.
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
            break;
        case I2C_STOP:      // Send stop condition
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
            return 0;
    }

    // Wait for TWINT flag set in TWCR Register
    while (!(TWCR & (1 << TWINT)));

    // Return TWI Status Register, mask the prescaler bits (TWPS1,TWPS0)
    return (TWSR & 0xF8);
}

uint8_t I2C_Start(uint8_t dev_id, uint8_t dev_addr, uint8_t addr){
    uint8_t s;

    // First step, send start
    s = I2C_Transmit(I2C_START);

    if (s == TW_MT_ARB_LOST || (s != TW_START && s != TW_REP_START)){
        printf("Error(Start): %x\r\n",s);
        goto failure;
    }

    // Second step, set device id and device address
    TWDR = (dev_id & 0xF0) | ((dev_addr << 1) & 0x0E) | TW_WRITE;

    s = I2C_Transmit(I2C_DATA_ACK);

    if (s == TW_MT_ARB_LOST || s != TW_MT_SLA_ACK || s == TW_MT_SLA_NACK){
        printf("Error(Device Id/Address): %x\r\n",s);
        goto failure;
    }

    // Third step, set memory address
    TWDR = addr;

    s = I2C_Transmit(I2C_DATA_ACK);

    if (s != TW_MT_DATA_ACK){
        printf("Error(EEPROM Address): %x\r\n",s);
        goto failure;
    }

    return 0;

    failure:return 1;
}

uint8_t I2C_Random_Read(uint8_t dev_id,
    uint8_t dev_addr,
    uint8_t addr, uint8_t *data)
{
    uint8_t ret = 0;

    // First step, setup i2c and addresses (dummy write)
    if(I2C_Start(dev_id,dev_addr,addr))
        goto exit;

    // Second step, start again
    uint8_t s = I2C_Transmit(I2C_START);

    if (s == TW_MT_ARB_LOST || (s != TW_START && s != TW_REP_START)){
        printf("Error(Start again): %x\r\n",s);
        goto exit;
    }

    // Third step, set device id and device address
    TWDR = (dev_id & 0xF0) | ((dev_addr << 1) & 0x0E) | TW_READ;

    s = I2C_Transmit(I2C_DATA_ACK);

    if (s == TW_MR_ARB_LOST || s != TW_MR_SLA_ACK || s == TW_MR_SLA_NACK){
        printf("Error(Device address 2): %x\r\n",s);
        goto exit;
    }

    // Forth step, read data
    s = I2C_Transmit(I2C_DATA_NACK);

    if (s != TW_MR_DATA_NACK){
        printf("Error(Data reception NACK): %x\r\n",s);
        goto exit;
    }

    *data = TWDR;

    ret = 1;

exit:    I2C_Transmit(I2C_STOP);
    return ret;
}

uint8_t I2C_Sequencial_Read(uint8_t dev_id,
    uint8_t dev_addr,
    uint8_t addr, uint8_t * data, uint8_t size)
{
    uint8_t ret = 0;

    // First step, setup i2c and addresses (dummy write)
    if(I2C_Start(dev_id,dev_addr,addr))
        goto exit;

    // Second step, start again
    uint8_t s = I2C_Transmit(I2C_START);

    if (s == TW_MT_ARB_LOST || (s != TW_START && s != TW_REP_START)){
        printf("Error(Start again): %x\r\n",s);
        goto exit;
    }

    // Third step, set device id and device address
    TWDR = (dev_id & 0xF0) | ((dev_addr << 1) & 0x0E) | TW_READ;

    s = I2C_Transmit(I2C_DATA_ACK);

    if (s == TW_MR_ARB_LOST || s != TW_MR_SLA_ACK || s == TW_MR_SLA_NACK){
        printf("Error(Device address 2): %x\r\n",s);
        goto exit;
    }

    for(uint8_t i = 0;i < size;i++){
        if(i == size - 1){
            // Fifth step, read and finish with NACK
            s = I2C_Transmit(I2C_DATA_NACK);
            if (s != TW_MR_DATA_NACK){
                printf("Error(Data reception NACK): %x\r\n",s);
                break;
            }
        }else{
            // Forth step, read data with ACK
            s = I2C_Transmit(I2C_DATA_ACK);
            if (s != TW_MR_DATA_ACK){
                printf("Error(Data reception ACK): %x\r\n",s);
                break;
            }
        }

        data[i] = TWDR;
        ret += 1;
    }

exit:    I2C_Transmit(I2C_STOP);
    return ret;
}

uint8_t I2C_Byte_Write(uint8_t dev_id,
    uint8_t dev_addr,
    uint8_t addr, uint8_t data)
{
    uint8_t ret = 0;

    // First step, setup i2c and addresses
    if(I2C_Start(dev_id,dev_addr,addr))
        goto exit;

    // Second step, write byte into memory
    TWDR = data;

    uint8_t s = I2C_Transmit(I2C_DATA_ACK);

    if (s != TW_MT_DATA_ACK){
        printf("Error(Data dispatch): %x\r\n",s);
        goto exit;
    }

    ret = 1;

exit:    I2C_Transmit(I2C_STOP);
    return ret;
}

uint8_t I2C_Page_Write(uint8_t dev_id,
    uint8_t dev_addr,
    uint8_t addr, uint8_t * data, uint8_t size)
{
    uint8_t ret = 0;

    // First step, setup i2c and addresses
    if(I2C_Start(dev_id,dev_addr,addr))
        goto exit;

    // Second step, write byte into memory
    for(uint8_t i = 0;i < size;i++){
        TWDR = data[i];

        uint8_t s = I2C_Transmit(I2C_DATA_ACK);

        if (s != TW_MT_DATA_ACK){
            printf("Error(Data dispatch): %x\r\n",s);
            break;
        }

        ret ++;
    }

exit:    I2C_Transmit(I2C_STOP);
    return ret;
}
