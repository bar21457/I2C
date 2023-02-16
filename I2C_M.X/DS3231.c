/*
 * File:   DS3231.c
 * Author: byron
 *
 * Created on February 10, 2023, 12:12 PM
 */


#include <xc.h>
#include "DS3231.h"
#include "configI2C.h"

uint8_t leerRTC(REG R) {
    
    uint8_t DATO;
    
    I2C_Master_Start();
    I2C_Master_Write(0xD0);             // Vamos a escribirle al slave
    I2C_Master_Write(R);             // Leemos el reg 0x00
    I2C_Master_RepeatedStart();
    I2C_Master_Write(0xD1);             // Vamos a leer el slave
    DATO = I2C_Master_Read(0);           // Leemos el reg 0x00
    I2C_Master_Stop();
    __delay_ms(200);
    
    DATO = (DATO >> 4) * 10 + (DATO & 0x0F);
    
    return DATO;
}