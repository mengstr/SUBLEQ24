#include "lowlevel.h"

#define LCD_E 0x10
#define LCD_RS 0x20
#define nGPIOwp 25
#define GPIOb 24
#define CB_nGPIOwp 0x01  // controlbyte
#define CB_GPIOb 0x02    // controlbyte
#define CB_SUBCkA 0x04
#define CB_SUBCkB 0x08
#define CB_nSUBoe 0x10

uint8_t mycb=0;


void LCDSendToPort(uint8_t v) {
  // Step 1 - Set LCD port address and latch
  DataOut(0xFFFFFF-6); 

  mycb|=CB_GPIOb;  SendAddress(-1, mycb);
  mycb&=~CB_GPIOb; SendAddress(-1, mycb);


  // Step 2 - Output (inverted) data to port and latch
  DataOut(~v); 
  mycb&=~CB_nGPIOwp; SendAddress(-1, mycb);
  mycb|=CB_nGPIOwp;  SendAddress(-1, mycb);
}

void LCDSendCmd(uint8_t v) {
  LCDSendToPort((v>>4));
  LCDSendToPort((v>>4) | LCD_E);
  LCDSendToPort((v>>4));

  LCDSendToPort((v&0x0F));
  LCDSendToPort((v&0x0F) | LCD_E);
  LCDSendToPort((v&0x0F));
}

void LCDSend(uint8_t v) {
  LCDSendToPort(v>>4 | LCD_RS);
  LCDSendToPort(v>>4 | LCD_RS | LCD_E);
  LCDSendToPort(v>>4 | LCD_RS);

  LCDSendToPort((v&0x0F) | LCD_RS);
  LCDSendToPort((v&0x0F) | LCD_RS | LCD_E);
  LCDSendToPort((v&0x0F) | LCD_RS);
  delayMicroseconds(10);
}


void InitLCD() {
  delay(100);
  LCDSendCmd(0x03); delay(100);       // Initialize Lcd in 4-bit mode
  LCDSendCmd(0x33); delay(100);
  LCDSendCmd(0x32); delay(100);

  LCDSendCmd(0x28); delay(100);       // Setup LCD parameters
  LCDSendCmd(0x0C); delay(100);
  LCDSendCmd(0x06); delay(100);
  LCDSendCmd(0x01); delay(100);
}

  // for (;;) {
  //   for (uint8_t ch='A'; ch<='Z'; ch++) { 
  //     LCDSendCmd(0x80); delayMicroseconds(1);
  //     for (uint8_t i=0; i<20;i++) LCDSend(ch); 
  //     LCDSendCmd(0x80+0x40); delayMicroseconds(1);
  //     for (uint8_t i=0; i<20;i++) LCDSend(ch); 
  //     LCDSendCmd(0x80+0x14); delayMicroseconds(1);
  //     for (uint8_t i=0; i<20;i++) LCDSend(ch); 
  //     LCDSendCmd(0x80+0x54); delayMicroseconds(1);
  //     for (uint8_t i=0; i<20;i++) LCDSend(ch); 
  //     delay(50);
  //   }
  // }



