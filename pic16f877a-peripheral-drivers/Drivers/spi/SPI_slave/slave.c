#include <xc.h>
#include <stdint.h>
#include "config.h"

// Function Prototypes
void SPI_Slave_Init(void);
void __interrupt() ISR(void);

// Global variable to store received SPI data
volatile uint8_t Data;

void main(void)
{
    SPI_Slave_Init();  
    TRISB = 0x00;  // Set PORTB as output for displaying received data

    while(1)
    {
        PORTB = Data;  // Show received data on PORTB (LEDs, Debug, etc.)
    }
}

void SPI_Slave_Init(void)
{
    // SPI Mode: Slave mode, SS enabled (SSPM3:SSPM0 = 0100 -> 0x04)
    SSPCON = 0x24;   // SSPEN = 1 (bit 5), SSPM = 0x04
    SSPSTAT = 0x00;  // CKP = 0, SMP = 0, CKE = 0 (SPI Mode 1)

    // Pin Directions
    TRISC5 = 0;  // SDO -> output
    TRISC4 = 1;  // SDI -> input
    TRISC3 = 1;  // SCK -> input
    TRISA5 = 1;  // SS  -> input

    // Disable analog on SS pin (AN4/RA5)
    ADCON1 = 0x06;   // All digital (PCFG3:0 = 0110)

    // Enable SPI Interrupt
    PIR1bits.SSPIF = 0;
    PIE1bits.SSPIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
}

void __interrupt() ISR(void)
{
    if (PIR1bits.SSPIF)
    {
        Data = SSPBUF;              // Read received byte
        PIR1bits.SSPIF = 0;         // Clear interrupt flag
    }
}
