/**
  Descriptive File Name
	
  Company:
    Microchip Technology Inc.

  File Name:
    file_template_source.c

  Summary:
    Brief Description of the file (will placed in a table if using Doc-o-Matic)

  Description:
    This section is for a description of the file.  It should be in complete
    sentences describing the purpose of this file.

 */

/*

�  [2015] Microchip Technology Inc. and its subsidiaries.  You may use this software 
and any derivatives exclusively with Microchip products. 
  
THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER EXPRESS, 
IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF 
NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE, OR ITS 
INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE 
IN ANY APPLICATION. 

IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL 
OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED 
TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY 
OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S 
TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED 
THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE TERMS. 

*/

/* 
 * File:   UART.c
 * Author: C13069
 *
 * Created on June 3, 2014, 12:22 AM
 */
#include <xc.h>
#include "uart.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * 
 */
void uart_init(void)
{
    //PORT pins setup
    TRISCbits.TRISC7 = 1; // RC7 is input
    TRISCbits.TRISC6 = 0; // RC6 is output
    
    //UART registers setup
    BAUDCON = 0x00;  // setup for 8-bit async.

    
    SPBRG = 68;   // 32MHz/(4(N+1) = 115200baud
    BAUDCONbits.BRG16 = 1;
    TXSTA = 0x26;

    CREN = 1;
    SPEN = 1;   
}

void putch(char data)
{
    if(data == '\n')
    {
        while( ! TXIF) // check buffer
            continue; // wait till ready
        TXREG = '\r';
    }
    while( ! TXIF) // check buffer
        continue; // wait till ready
    TXREG = data; // send data
}