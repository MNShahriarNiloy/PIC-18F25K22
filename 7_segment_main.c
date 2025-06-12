#pragma config LVP = OFF  
#pragma config PLLCFG = OFF     
#pragma config PRICLKEN = ON    
#pragma config FCMEN = OFF    
#pragma config IESO = OFF     
#pragma config WDTEN = OFF      
#pragma config BOREN = OFF      
#pragma config FOSC = INTIO67
#define _XTAL_FREQ 8000000UL

#include <xc.h>
#include <stdio.h>

#define MCP23S08_WRITE 0x40  // A1:A0 = 00, R/W = 0
#define IODIR 0x00
#define OLAT  0x0A
#define CS LATCbits.LATC6
#define CS_TRIS TRISCbits.TRISC6

void DisplayDigit(uint8_t digit);
void MCP23S08_Init(void);
void MCP23S08_Write(uint8_t reg, uint8_t data);
void SPI_Write(uint8_t data);
void SPI_Init(void);

uint8_t current_position = 0;
uint16_t moisture_value = 0; 
volatile uint8_t timeout_occurred = 0;
const uint8_t segmentMap[10] = {
    0b00111111 << 1, // 0
    0b00000110 << 1, // 1
    0b01011011 << 1, // 2
    0b01001111 << 1, // 3
    0b01100110 << 1, // 4
    0b01101101 << 1, // 5
    0b01111101 << 1, // 6
    0b00000111 << 1, // 7
    0b01111111 << 1, // 8
    0b01101111 << 1  // 9
};

void main(void){
    OSCCONbits.IRCF = 0b110;   
    OSCCONbits.SCS = 0b10;  
    while (!OSCCONbits.HFIOFS);
    
    ANSELC = 0x00;
    SPI_Init();
    MCP23S08_Init();
    for (uint8_t i = 0; i < 10; i++) {
                DisplayDigit(i);
                __delay_ms(100);
        
            }
}

void SPI_Init(void) {
    TRISC3 = 0; 
    TRISC4 = 1;
    TRISC5 = 0;
    CS_TRIS = 0;
    CS = 1;

    SSPSTAT = 0x40;   
    SSPCON1 = 0x20;      
}

void SPI_Write(uint8_t data) {
    SSPBUF = data;
    while (!SSPSTATbits.BF); 
}

void MCP23S08_Write(uint8_t reg, uint8_t data) {
    CS = 0;
    SPI_Write(MCP23S08_WRITE);
    SPI_Write(reg);
    SPI_Write(data);
    CS = 1;
}

void MCP23S08_Init(void) {
    MCP23S08_Write(IODIR, 0x01);
}

void DisplayDigit(uint8_t digit) {
    if (digit > 9) digit = 0;
    MCP23S08_Write(OLAT, segmentMap[digit]);
}