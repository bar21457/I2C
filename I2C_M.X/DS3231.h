/* 
 * File:   DS3231.h
 * Author: byron
 *
 * Created on February 10, 2023, 12:13 PM
 */

#ifndef DS3231_H
#define	DS3231_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <pic16f887.h>

typedef enum 
{
    REG_SEG = 0x00,
    REG_MIN = 0x01,
    REG_HOR = 0x02,
    REG_DIA = 0x03,
    REG_MES = 0x04,
    REG_A = 0x05,
}REG;

uint8_t leerRTC (REG);

#endif	/* DS3231_H */

