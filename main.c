/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdio.h>
#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include <pic16f688.h>

//PORTA defines

//PORTC defines


void main(void)
{
    ANSEL = 0x00;
    CMCON0 = 0x07;
    
    TRISA = 0x07; //RA0,1,2 nputs. others outputs
    PORTA = 0xF8; //PORTA outputs set to mcu logic high
    
    TRISC = 0x00; //all outputs
    PORTC = 0xFF; // PORTC outputs set to mcu logic high
    
    //check VHZ input on reset

}

