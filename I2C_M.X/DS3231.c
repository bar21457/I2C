/*
 * File:   DS3231.c
 * Author: byron
 *
 * Created on February 10, 2023, 12:12 PM
 */


#include <xc.h>
#include "DS3231.h"
#include "configI2C.h"

uint8_t leerSEG(void) {
    
    uint8_t SEG;
    
    I2C_Master_Start();
    I2C_Master_Write(0xD0);
    I2C_Master_Write(0x00);
    I2C_Master_RepeatedStart();
    I2C_Master_Write(0xD1);
    SEG = I2C_Master_Read(0);
    I2C_Master_Stop();
    __delay_ms(200);
    
    SEG = (SEG >> 4) * 10 + (SEG & 0x0F);
    
    return SEG;
}
