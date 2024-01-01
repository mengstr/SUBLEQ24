#define CP 28
#define _OE 27
#define LATCH 26
#define SERIAL 25

void setup() {
  // Serial.begin(9600);
  delay(1000);
  Serial.println("\n\nStarting");
  DataInMode();

  pinMode(CP, OUTPUT);
  digitalWrite(CP, 0);
  pinMode(_OE, OUTPUT);
  digitalWrite(_OE, 1);
  pinMode(LATCH, OUTPUT);
  digitalWrite(LATCH, 0);
  pinMode(SERIAL, OUTPUT);
  digitalWrite(SERIAL, 0);

  digitalWrite(_OE, 0);
}


void DataInMode() {
  for (int i = 0; i < 24; i++) pinMode(i, INPUT);
}

void DataOutMode() {
  for (int i = 0; i < 24; i++) pinMode(i, OUTPUT);
}

void DataOut(uint32_t v) {
  for (int i = 0; i < 24; i++) {
    digitalWrite(i, v & 1);
    v = v / 2;
  }
}

uint32_t DataIn() {
  uint32_t v = 0;
  for (int i = 23; i >= 0; i--) {
    v = v * 2;
    if (digitalRead(i)) v = v + 1;
  }
  return v;
}


void SendByte(uint8_t v) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(SERIAL, v & 1);
    digitalWrite(CP, 1);
    v = v / 2;
    digitalWrite(CP, 0);
  }
}


void SendAddress(uint32_t a, int16_t d) {
  SendByte(d);
  SendByte((a >> 0) & 0xff);
  SendByte((a >> 8) & 0xff);
  SendByte((a >> 16) & 0xff);
  digitalWrite(LATCH, 1);
  digitalWrite(LATCH, 0);
}

#define _RAM_WE 4
#define _RAM_OE 8
#define _MEM_CE 16
#define _FLASH_WE 32
#define _FLASH_OE 64

//-------------------------------------

#define SEED 1234

#define FLASHSIZE 1024*256
#define FLASHSTEP 4000
#define RAMSIZE 1024*2048
#define RAMSTEP 50000


void WriteFlash(uint32_t a, uint32_t d) {
  SendAddress(0x600000+a, _FLASH_WE |  _FLASH_OE | _RAM_WE | _RAM_OE); 
  DataOutMode();
  DataOut(d);
  SendAddress(0x600000+a,  0        |  _FLASH_OE | _RAM_WE | _RAM_OE); 
  SendAddress(0x600000+a, _FLASH_WE |  _FLASH_OE | _RAM_WE | _RAM_OE); 
  DataInMode();
  delayMicroseconds(30);
}

void WriteRAM(uint32_t a, uint32_t d) {
  SendAddress(a, _FLASH_WE |  _FLASH_OE | _RAM_WE | _RAM_OE); 
  DataOutMode();
  DataOut(d);
  SendAddress(a, _FLASH_WE  | _FLASH_OE | 0       | _RAM_OE); 
  SendAddress(a, _FLASH_WE |  _FLASH_OE | _RAM_WE | _RAM_OE); 
  DataInMode();
}



void loop() {
 uint32_t cntOk=0;
 uint32_t cntErr=0;

  // for (;;) {
  //   SendAddress(0x000000 + (1<<0)+ (1<<2)+ (1<<4)+ (1<<6)+ (1<<8)+ (1<<10)+ (1<<12)+ (1<<14)+ (1<<16)+ (1<<18)+ (1<<20)+ (1<<22)  , _FLASH_WE |  _FLASH_OE | _RAM_WE | _RAM_OE);  delay(500);
  //   SendAddress(0x000000 + (1<<1)+ (1<<3)+ (1<<5)+ (1<<7)+ (1<<9)+ (1<<11)+ (1<<13)+ (1<<15)+ (1<<17)+ (1<<19)+ (1<<21)+ (1<<23)  , _FLASH_WE |  _FLASH_OE | _RAM_WE | _RAM_OE);  delay(500);
  // }


  DataOutMode();

  // for(;;) {
  //   for (int i=0; i<24; i++) {
  //     digitalWrite(i,1);
  //     SendAddress(1<<i, _RAM_WE | _RAM_OE | _FLASH_WE | _FLASH_OE | _MEM_CE);
  //     delay(222);
  //     digitalWrite(i,0);
  //   }
  // }


// 0x5555 -> 10 0010 1001 1010 1010 = 0x229AA
// 0x2AAA -> 00 0001 0110 0101 0101 = 0x01655


Serial.println("Writing random data to RAM");
randomSeed(SEED);
for (uint32_t a=0; a<RAMSIZE; a++) {
  WriteRAM(a,random(1<<24));
  a+=random(RAMSTEP);
  delay(50);
}

 Serial.println("Verifying RAM");
 randomSeed(SEED);
 DataInMode();
  cntOk=0;
  cntErr=0;
for (uint32_t i = 0; i < RAMSIZE; i++) {
    uint32_t a = i;
    SendAddress(a, _FLASH_WE | _FLASH_OE |  _RAM_WE | 0);
    uint32_t v = DataIn();
    if (v==random(1<<24)) cntOk++; else cntErr++;
    i+=random(RAMSTEP);
}
Serial.printf("RAM: OK=%ld Err=%ld\n",cntOk,cntErr);


// FLASH Chip-Erase  
//      (original)   0x5555:0xAA   0x2AAA:0x55   0x5555:0x80.  0x5555:0xAA   0x2AAA:0x55   0x5555:0x10
//      (scrambled) 0x229AA:0xAA  0x01655:0x55  0x229AA:0x80  0x229AA:0xAA  0x01655:0x55  0x229AA:0x10
  Serial.println("Erasing FLASH");
  DataOutMode();
  WriteFlash(0x229AA,0xAAAAAA);
  WriteFlash(0x01655,0x555555);
  WriteFlash(0x229AA,0x808080);
  WriteFlash(0x229AA,0xAAAAAA);
  WriteFlash(0x01655,0x555555);
  WriteFlash(0x229AA,0x101010);
  delay(250);



Serial.println("Writing random data to FLASH");
randomSeed(SEED);
for (uint32_t a=0; a<FLASHSIZE; a++) {
// FLASH Byte-Program
//      (original)   0x5555:0xAA    0x2AAA:0x55    0x5555:0xA0   Address:Data 
//      (scrambled) 0x229AA:0xAA   0x01655:0x55   0x229AA:0xA0   Address:Data
  WriteFlash(0x229AA,0xAAAAAA);
  WriteFlash(0x01655,0x555555);
  WriteFlash(0x229AA,0xA0A0A0);
  WriteFlash(a,random(1<<24));
  delay(50);
  a+=random(FLASHSTEP);
}

 Serial.println("Verifying FLASH");
 randomSeed(SEED);
 DataInMode();
cntOk=0;
cntErr=0;
for (uint32_t i = 0; i < FLASHSIZE; i++) {
    uint32_t a = 0x600000+i;
    SendAddress(a, _FLASH_WE |  _RAM_WE | _RAM_OE);
    uint32_t v = DataIn();
    if (v==random(1<<24)) cntOk++; else cntErr++;
    i+=random(FLASHSTEP);
}
Serial.printf("FLASH: OK=%ld Err=%ld\n",cntOk,cntErr);
 
  //
  // *** DUMP FLASH
  //
  // DataInMode();
  // for (uint32_t i = 0; i < LOCATIONS; i++) {
  //   uint32_t a = 0x600000+i;
  //   SendAddress(a, _FLASH_WE |  _RAM_WE | _RAM_OE);
  //   uint32_t v = DataIn();
  //   if ((i % 16)==0) Serial.printf("%08x: ",a);
  //   Serial.printf("%06x ",v);
  //   if ((i % 16)==15) Serial.println();
  // }

  Serial.println("Done");
  uint32_t a=0;
  uint32_t d=0;
  uint8_t redleds=random(256)|_MEM_CE;
  DataOutMode();
  for(;;) {
    if (random(100)<10) redleds=random(256)|_MEM_CE;
      SendAddress(a, redleds); 
      delay(10);
      DataOut(d);
      if (random(100)<20) {
        a|=1<<random(2^24);
        d|=1<<random(2^24);
      } else {
        a&=~(1<<random(2^24));
        d&=~(1<<random(2^24));
      }
  }
}
