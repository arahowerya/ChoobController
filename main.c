/* 
 * File:   main.c
 * Author: the lads
 *
 * Created on July 19, 2018, 8:43 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <xc.h>

#include "mcc_generated_files/mcc.h"

/*
 * 
 */

uint8_t volatile readBuffer = 0;

static struct Frame
{
    uint8_t out_buf[10][10]; //7seg data
    
    uint8_t in_buffer[200];
    
}Frame;

void loadFrame(void)
{
    for(int i = 0; i < 10; i++)
    {
        memcpy(Frame.out_buf[i],&(Frame.in_buffer[i*10]),10);
    }
}

void frame_init()
{
    memset(Frame.out_buf, 0, sizeof(Frame.out_buf));
    
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            Frame.in_buffer[i*10+j] = '0' + j;
        }
    }
    loadFrame();
}

void send_data()
{
    loadFrame();
    for(int row = 0; row < 8; row++)
    {
        while(!EUSART2_is_tx_ready())
        {
            NOP();
        }

//        TXSTA2bits.TX9D = 1;
        EUSART2_Write(8 - row);
//        TXSTA2bits.TX9D = 0;
        for(int col = 0; col < 10; col ++)
        {      
            while(!EUSART2_is_tx_ready())
            {
                NOP();
            }
            EUSART2_Write(Frame.out_buf[row][col]);
        }
        while(!EUSART2_is_tx_ready())
        {
            NOP();
        }
//        __delay_ms(2);
    }
}
uint8_t bytesSend = 0;
int main(int argc, char** argv)
{
    SYSTEM_Initialize();
    __delay_ms(2000);
    frame_init();
    
    LATBbits.LATB5 = 0;
    
    /* Enable global and peripheral interrupts */
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    
    while(1)
    {
//        send_data();
//        __delay_ms(80);
//        if(f++ >8)f =0;
//        Frame.in_buffer
//        send_data();
//        __delay_ms(80);
        
        if(readBuffer){
            readBuffer = 0;
            // read USART buffer
            readRxBuffer(Frame.in_buffer);
            send_data();
        }
        
    }
    return (EXIT_SUCCESS);
}



