#include <xc.h>


void CAN_Init(void) {
    // Set CAN to config mode
    CANCON = 0x80;         
    while((CANSTAT & 0xE0) != 0x80);

    // Set Baud rate: 125kbps @20MHz
    BRGCON1 = 0x03;  
    BRGCON2 = 0xBA;  
    BRGCON3 = 0x07;  

    // Normal mode
    CANCON = 0x00;         
    while((CANSTAT & 0xE0) != 0x00);
}

void CAN_Send(unsigned long id, unsigned char *data, unsigned char len) {
    TXB0SIDH = (id >> 3) & 0xFF;
    TXB0SIDL = (id << 5) & 0xE0;
    TXB0DLC = len;
    for (char i=0; i<len; i++) {
        *(&TXB0D0 + i) = data[i];
    }
    TXB0CON = 0x08;   // Request to send
    while(TXB0CONbits.TXREQ); // Wait until sent
}

void main(void) {
    unsigned char msg[8] = {1,2,3,4,5,6,7,8};
    CAN_Init();

    while(1) {
        CAN_Send(0x100, msg, 8);
        for(long d=0; d<30000; d++);  // delay
    }
}
