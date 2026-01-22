#include <xc.h>
#include "config.h"
#include "I2C_EEPROM.h"
#define _XTAL_FREQ 16000000

void main(void) {

    I2C_Master_Init(100000);  // Initialize I2C Master at 100kHz

    // === Write Operations ===
    EEPROM_Write(0x0023, 'A');  // Write 'A' (0x41) to address 0x0023
    EEPROM_Write(0x0028, 'B');  // Write 'B' (0x42) to address 0x0028
    EEPROM_Write(0x0036, 'C');  // Write 'C' (0x43) to address 0x0036
    __delay_ms(10);             // EEPROM write delay (tWR)

    // === Read Operations ===
    TRISD = 0x00;               // Set PORTD as output

    PORTD = EEPROM_Read(0x0023);  // Read 'A' from 0x0023 -> PORTD
    __delay_ms(1000);            // Delay to observe LED effect

    PORTD = EEPROM_Read(0x0028);  // Read 'B' from 0x0028 -> PORTD
    __delay_ms(1000);            // Delay to observe LED effect

    PORTD = EEPROM_Read(0x0036);  // Read 'C' from 0x0036 -> PORTD

    while(1)
    {
        // Infinite loop
    }

    return;
}
