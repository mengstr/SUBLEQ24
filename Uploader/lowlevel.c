#include "lowlevel.h"

void setupShiftRegister() {
  pinMode(CP, OUTPUT);
  digitalWrite(CP, 0);
  pinMode(_OE, OUTPUT);
  digitalWrite(_OE, 1);
  pinMode(LATCH, OUTPUT);
  digitalWrite(LATCH, 0);
  pinMode(SER, OUTPUT);
  digitalWrite(SERIAL, 0);
}

void shiftRegisterOE(bool b) {
  digitalWrite(_OE, !b);
}

void DataInMode() {
  // delayMicroseconds(10);
  for (int i = 0; i < 24; i++) pinMode(i, INPUT);
  // delayMicroseconds(10);
}

void DataOutMode() {
  // delayMicroseconds(10);
  for (int i = 0; i < 24; i++) pinMode(i, OUTPUT);
  // delayMicroseconds(10);
}

void DataOut(uint32_t v) {
  digitalWrite(0, 0 != (v & (1 << 1)));
  digitalWrite(1, 0 != (v & (1 << 0)));
  digitalWrite(2, 0 != (v & (1 << 3)));
  digitalWrite(3, 0 != (v & (1 << 2)));
  digitalWrite(4, 0 != (v & (1 << 5)));
  digitalWrite(5, 0 != (v & (1 << 4)));
  digitalWrite(6, 0 != (v & (1 << 7)));
  digitalWrite(7, 0 != (v & (1 << 6)));
  digitalWrite(8, 0 != (v & (1 << 9)));
  digitalWrite(9, 0 != (v & (1 << 8)));
  digitalWrite(10, 0 != (v & (1 << 11)));
  digitalWrite(11, 0 != (v & (1 << 10)));
  digitalWrite(12, 0 != (v & (1 << 13)));
  digitalWrite(13, 0 != (v & (1 << 12)));
  digitalWrite(14, 0 != (v & (1 << 15)));
  digitalWrite(15, 0 != (v & (1 << 14)));
  digitalWrite(16, 0 != (v & (1 << 17)));
  digitalWrite(17, 0 != (v & (1 << 16)));
  digitalWrite(18, 0 != (v & (1 << 19)));
  digitalWrite(19, 0 != (v & (1 << 18)));
  digitalWrite(20, 0 != (v & (1 << 21)));
  digitalWrite(21, 0 != (v & (1 << 20)));
  digitalWrite(22, 0 != (v & (1 << 23)));
  digitalWrite(23, 0 != (v & (1 << 22)));
  delayMicroseconds(10);
}

uint32_t DataIn() {
  uint32_t v = 0;
  // delayMicroseconds(10);
  if (digitalRead(0)) v|=(1<<1);
  if (digitalRead(1)) v|=(1<<0);
  if (digitalRead(2)) v|=(1<<3);
  if (digitalRead(3)) v|=(1<<2);
  if (digitalRead(4)) v|=(1<<5);
  if (digitalRead(5)) v|=(1<<4);
  if (digitalRead(6)) v|=(1<<7);
  if (digitalRead(7)) v|=(1<<6);
  if (digitalRead(8)) v|=(1<<9);
  if (digitalRead(9)) v|=(1<<8);
  if (digitalRead(10)) v|=(1<<11);
  if (digitalRead(11)) v|=(1<<10);
  if (digitalRead(12)) v|=(1<<13);
  if (digitalRead(13)) v|=(1<<12);
  if (digitalRead(14)) v|=(1<<15);
  if (digitalRead(15)) v|=(1<<14);
  if (digitalRead(16)) v|=(1<<17);
  if (digitalRead(17)) v|=(1<<16);
  if (digitalRead(18)) v|=(1<<19);
  if (digitalRead(19)) v|=(1<<18);
  if (digitalRead(20)) v|=(1<<21);
  if (digitalRead(21)) v|=(1<<20);
  if (digitalRead(22)) v|=(1<<23);
  if (digitalRead(23)) v|=(1<<22);
  return v;
}


void SendByte(uint8_t v) {
  for (int i = 0; i < 8; i++) {
  // delayMicroseconds(10);
    digitalWrite(SER, v & 1);
  // delayMicroseconds(10);
    digitalWrite(CP, 1);
  // delayMicroseconds(10);
    v = v / 2;
    digitalWrite(CP, 0);
  // delayMicroseconds(10);
  }
  // delayMicroseconds(10);
}


void SendAddress(uint32_t a, int16_t d) {
  static uint32_t lastA;

  if (a==-1) a=lastA;
  lastA=a;

  uint8_t byteHi = 0;
  uint8_t byteMid = 0;
  uint8_t byteLow = 0;

  if (a & (1 << 0)) byteLow |= 64;
  if (a & (1 << 1)) byteLow |= 128;
  if (a & (1 << 2)) byteLow |= 16;
  if (a & (1 << 3)) byteLow |= 32;
  if (a & (1 << 4)) byteLow |= 4;
  if (a & (1 << 5)) byteLow |= 8;
  if (a & (1 << 6)) byteLow |= 1;
  if (a & (1 << 7)) byteLow |= 2;
  if (a & (1 << 8)) byteMid |= 64;
  if (a & (1 << 9)) byteMid |= 128;
  if (a & (1 << 10)) byteMid |= 4;
  if (a & (1 << 11)) byteMid |= 1;
  if (a & (1 << 12)) byteMid |= 2;
  if (a & (1 << 13)) byteMid |= 16;
  if (a & (1 << 14)) byteMid |= 8;
  if (a & (1 << 15)) byteMid |= 32;
  if (a & (1 << 16)) byteHi |= 128;
  if (a & (1 << 17)) byteHi |= 8;
  if (a & (1 << 18)) byteHi |= 4;
  if (a & (1 << 19)) byteHi |= 64;
  if (a & (1 << 20)) byteHi |= 16;
  if (a & (1 << 21)) byteHi |= 32;
  if (a & (1 << 22)) byteHi |= 2;
  if (a & (1 << 23)) byteHi |= 1;

  SendByte(d);
  // delayMicroseconds(10);
  SendByte(byteHi);
  // delayMicroseconds(10);
  SendByte(byteMid);
  // delayMicroseconds(10);
  SendByte(byteLow);
  // delayMicroseconds(10);
  digitalWrite(LATCH, 1);
  // delayMicroseconds(10);
  digitalWrite(LATCH, 0);
  // delayMicroseconds(10);
}


// uint32_t FixAddressForMemory(uint32_t i) {
//   return i;
//   uint32_t o = 0;
//   if (0 != (i & (1 << 0))) o |= 1 << 17;
//   if (0 != (i & (1 << 1))) o |= 1 << 16;
//   if (0 != (i & (1 << 2))) o |= 1 << 14;
//   if (0 != (i & (1 << 3))) o |= 1 << 13;
//   if (0 != (i & (1 << 4))) o |= 1 << 11;
//   if (0 != (i & (1 << 5))) o |= 1 << 9;
//   if (0 != (i & (1 << 6))) o |= 1 << 7;
//   if (0 != (i & (1 << 7))) o |= 1 << 5;
//   if (0 != (i & (1 << 8))) o |= 1 << 8;
//   if (0 != (i & (1 << 9))) o |= 1 << 10;
//   if (0 != (i & (1 << 10))) o |= 1 << 15;
//   if (0 != (i & (1 << 11))) o |= 1 << 12;
//   if (0 != (i & (1 << 12))) o |= 1 << 18;
//   if (0 != (i & (1 << 13))) o |= 1 << 6;
//   if (0 != (i & (1 << 14))) o |= 1 << 4;
//   if (0 != (i & (1 << 15))) o |= 1 << 0;
//   if (0 != (i & (1 << 16))) o |= 1 << 1;
//   if (0 != (i & (1 << 17))) o |= 1 << 3;
//   if (0 != (i & (1 << 18))) o |= 1 << 2;
//   if (0 != (i & (1 << 19))) o |= 1 << 19;
//   if (0 != (i & (1 << 20))) o |= 1 << 20;
//   if (0 != (i & (1 << 21))) o |= 1 << 21;
//   if (0 != (i & (1 << 22))) o |= 1 << 22;
//   if (0 != (i & (1 << 23))) o |= 1 << 23;
//   return o;
// }

