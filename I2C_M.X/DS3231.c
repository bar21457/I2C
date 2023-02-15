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
    I2C_Master_Write(0xD0);             // Vamos a escribirle al slave
    I2C_Master_Write(0x00);             // Leemos el reg 0x00
    I2C_Master_RepeatedStart();
    I2C_Master_Write(0xD1);             // Vamos a leer el slave
    SEG = I2C_Master_Read(0);           // Leemos el reg 0x00
    I2C_Master_Stop();
    __delay_ms(200);
    
    SEG = (SEG >> 4) * 10 + (SEG & 0x0F);
    
    return SEG;
}

uint8_t leerMIN(void) {
    
    uint8_t MIN;
    
    I2C_Master_Start();
    I2C_Master_Write(0xD0);             // Vamos a escribirle al slave
    I2C_Master_Write(0x01);             // Leemos el reg 0x01
    I2C_Master_RepeatedStart();
    I2C_Master_Write(0xD1);             // Vamos a leer el slave
    MIN = I2C_Master_Read(0);           // Leemos el reg 0x01
    I2C_Master_Stop();
    __delay_ms(200);
    
    MIN = (MIN >> 4) * 10 + (MIN & 0x0F);
    
    return MIN;
}

uint8_t leerHOR(void) {
    
    uint8_t HOR;
    
    I2C_Master_Start();
    I2C_Master_Write(0xD0);             // Vamos a escribirle al slave
    I2C_Master_Write(0x02);             // Leemos el reg 0x02
    I2C_Master_RepeatedStart();
    I2C_Master_Write(0xD1);             // Vamos a leer el slave
    HOR = I2C_Master_Read(0);           // Leemos el reg 0x02
    I2C_Master_Stop();
    __delay_ms(200);
    
    HOR = (HOR >> 4) * 10 + (HOR & 0x0F);
    
    return HOR;
}