#include <xc.h>

void UART_Init(void) {
    TXSTAbits.BRGH = 1;
    SPBRG = 129;        // 9600 baud @20MHz
    RCSTAbits.SPEN = 1;
    TXSTAbits.TXEN = 1;
}

void UART_SendChar(char c) {
    while(!TXSTAbits.TRMT);
    TXREG = c;
}

void UART_SendString(const char *s) {
    while(*s) UART_SendChar(*s++);
}

void CAN_Init(void) {
    CANCON = 0x80;         
    while((CANSTAT & 0xE0) != 0x80);

    BRGCON1 = 0x03;  
    BRGCON2 = 0xBA;  
    BRGCON3 = 0x07;  

    CANCON = 0x00;         
    while((CANSTAT & 0xE0) != 0x00);
}

void main(void) {
    UART_Init();
    CAN_Init();

    UART_SendString("CAN Receiver Ready\r\n");

    while(1) {
        if(RXB0CONbits.RXFUL) {
            unsigned char len = RXB0DLC & 0x0F;
            UART_SendString("ID:0x100 Data: ");
            for(char i=0; i<len; i++) {
                char val = *(&RXB0D0 + i);
                UART_SendChar('0' + (val/10));
                UART_SendChar('0' + (val%10));
                UART_SendChar(' ');
            }
            UART_SendString("\r\n");
            RXB0CONbits.RXFUL = 0;  // Clear buffer
        }
    }
}
