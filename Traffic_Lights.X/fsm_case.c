/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.65.2
        Device            :  PIC16F1829
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"

volatile uint16_t g_ms; //global variable millisecond
volatile uint8_t g_s;   //global variable second

//define output Red, Green, Yellow
#define NS_GREEN    0x01    //north to south lights
#define NS_YELLOW   0x02
#define NS_RED      0x04

#define EW_GREEN    0x08    //east to west lights
#define EW_YELLOW   0x10
#define EW_RED      0x20

//define states
#define S0      0   // north and south directions go (green light)
#define S1		1   // north and south wait (yellow light)
#define S2  	2   // east and west go (green light)
#define S3		3   // EW - wait (yellow light)



//g_ms counts to 1000 milliseconds
//g_s count up seconds up to 255
void TMR0_Interrupt(void){
    g_ms++;
    if(g_ms >= 1000){
        g_ms = 0;
        g_s++;
    }    
}

//delay loop
//delay in seconds, 0-255 seconds
void delay_s(uint8_t delay){
    //reset global delay counter
    g_s = 0;
    //loop
    while(g_s < delay);
}


void main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    
    TMR0_SetInterruptHandler(TMR0_Interrupt);

    //init state
    uint8_t state=S0;
    uint8_t input;
        
    while (1){        
        switch(state){
            case S0: // north south green
                LATC = NS_GREEN | EW_RED;
                delay_s(5);
                if(input == 0x00 || input == 0x01)
                    state = S0;
                else if(input == 0x02 || input == 0x03)
                    state = S1;
                break;
            case S1: // north south in transition from green to red
                LATC = NS_YELLOW| EW_RED;
                delay_s(1);
                state = S2;
                break;
            case S2:// east west green
                LATC = NS_RED | EW_GREEN;
                delay_s(5);
                if(input == 0x02)
                    state = S2;
                else if(input == 0x01 || input == 0x03 || input == 0x00)
                    state = S3;
                break;
            case S3: // east west yellow
                LATC = NS_RED| EW_YELLOW;
                delay_s(1);
                state = S0;
                break;            
        }
        input = (uint8_t)(IO_RB5_PORT|(IO_RB6_PORT<<1u));        
    }
}
/**
 End of File
*/