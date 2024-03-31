#include <Arduino.h>

// --- port 26
// --- port 27
// --- Data 0x80
// --- Data 0x40
// --- Data 0x20
// --- Data 0x10
// --- Data 0x08
// --- Data 0x04
// --- Data 0x02
// --- Data 0x01
// --- port 33
// --- port 24
// --- port 25

#define LATCH 29
#define CP 30
#define SER 31
#define _OE 32


#ifdef __cplusplus
extern "C" {
#endif

void setupShiftRegister();
void shiftRegisterOE(bool b);
void DataInMode();
void DataOutMode();
void DataOut(uint32_t v);
uint32_t DataIn();
void SendByte(uint8_t v);
void SendAddress(uint32_t a, int16_t d);
uint32_t FixAddressForMemory(uint32_t i);

#ifdef __cplusplus
}
#endif
