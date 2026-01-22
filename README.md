
## 🧩 Implemented Peripherals

#✅ UART (USART)
- Baud rate configuration
- TX/RX enable and status flag handling
- Polling-based data transmission
- Master–slave communication demos

#✅ SPI
- Master and slave mode configuration
- Clock polarity and phase setup
- Full-duplex data transfer using SSPBUF

#✅ I2C
- START, STOP, ACK/NACK sequencing
- Master mode configuration
- External EEPROM interfacing
  
#✅ Timers & Interrupts
- Timer0 / Timer1 configuration
- Interrupt flag handling
- Time-based task execution

#✅ Memory
- Internal EEPROM read/write sequencing
- External EEPROM using I2C

#✅ CAN (Basic Node Communication)
- Node-based communication structure
- Message transfer validation

## Driver Development Approach

- Peripheral registers configured explicitly using datasheet references
- No use of HAL or library abstractions
- Clear distinction between:
  - **Driver layer** (register configuration & data transfer)
  - **Application layer** (display, logic, formatting)

Each peripheral folder contains:
- Initialization logic
- Data transfer routines
- Minimal demo application for validation

---

## Validation & Debugging

- Peripheral behavior verified using:
  - Status flags
  - Polling loops
  - LCD-based output
- Timing and sequencing validated against datasheet specifications

---

##  Key Learning Outcomes

- Register-level peripheral programming
- MCU datasheet interpretation
- Communication protocol fundamentals
- Debugging peripheral behavior without abstractions
- Structuring reusable embedded drivers

---

##  Scope for Improvement

- Interrupt-driven UART / SPI / I2C
- Error handling and timeouts
- Driver API abstraction
- Porting drivers to other PIC or ARM MCUs

*This repository is intended to demonstrate strong embedded fundamentals
and low-level driver development skills rather than production-ready firmware.*
