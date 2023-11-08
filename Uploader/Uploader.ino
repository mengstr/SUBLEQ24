#define CP 28
#define _OE 27
#define LATCH 26
#define SERIAL 25

void setup() {
  Serial.begin(9600);
  delay(500);
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB
  }
  Serial.println("\n\nStaring");
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

#define LOCATIONS 1024*256

void WriteFlash(uint32_t a, uint8_t d) {
  SendAddress(0x600000+a, _FLASH_WE |  _FLASH_OE | _RAM_WE | _RAM_OE); 
  DataOutMode();
  DataOut(d);
  SendAddress(0x600000+a,              _FLASH_OE | _RAM_WE | _RAM_OE); 
  SendAddress(0x600000+a, _FLASH_WE |  _FLASH_OE | _RAM_WE | _RAM_OE); 
  DataInMode();
}


void loop() {

  // for (int i=0; i<24; i++) {
  //   digitalWrite(i,1);
  //   SendAddress(1<<i, _RAM_WE | _RAM_OE | _FLASH_WE | _FLASH_OE);
  //   delay(250);
  //   digitalWrite(i,0);
  // }

  // delay(1000);
  // for (uint32_t a=0; a<0x200000; a+=16)       SendAddress(a, _RAM_WE | _FLASH_WE  | _FLASH_OE);
  // for (uint32_t a=0x600000; a<0x640000; a+=2) SendAddress(a, _RAM_WE | _FLASH_WE  | _FLASH_OE);

    // uint32_t a = 0x600000+5;

// 0x5555 -> 10 0010 1001 1010 1010 = 0x229AA
// 0x2AAA -> 00 0001 0110 0101 0101 = 0x01655

//Byte-Program
//(original)   0x5555:0xAA    0x2AAA:0x55    0x5555:0xA0   Address:Data 
//(scrambled) 0x229AA:0xAA   0x01655:0x55   0x229AA:0xA0   Address:Data

//Chip-Erase  
//(original)   0x5555:0xAA   0x2AAA:0x55   0x5555:0x80.  0x5555:0xAA   0x2AAA:0x55   0x5555:0x10
//(scrambled) 0x229AA:0xAA  0x01655:0x55  0x229AA:0x80  0x229AA:0xAA  0x01655:0x55  0x229AA:0x10


  WriteFlash(0x229AA,0xAA);
  WriteFlash(0x01655,0x55);
  WriteFlash(0x229AA,0x80);
  WriteFlash(0x229AA,0xAA);
  WriteFlash(0x01655,0x55);
  WriteFlash(0x229AA,0x10);
  delay(100);

  randomSeed(1);

for (uint32_t a=0; a<LOCATIONS; a++) {
  WriteFlash(0x229AA,0xAA);
  WriteFlash(0x01655,0x55);
  WriteFlash(0x229AA,0xA0);
  if (a%1024<32) WriteFlash(a,(a>>10)&0xFF);
  else WriteFlash(a,random(256*256*256));
}

  DataInMode();
  for (uint32_t i = 0; i < LOCATIONS; i++) {
    uint32_t a = 0x600000+i;
    SendAddress(a, _FLASH_WE |  _RAM_WE | _RAM_OE);
    uint32_t v = DataIn();
    Serial.printf("%02x ",v);
    if ((i % 32)==31) Serial.println();
  }

  for(;;) {
        SendAddress(0x600000, _FLASH_WE |  _FLASH_OE | _RAM_WE | _RAM_OE); delay(333);
        SendAddress(0x600001, _FLASH_WE |              _RAM_WE | _RAM_OE); delay(333);
        // SendAddress(0x000000, _FLASH_WE |  _FLASH_OE | _RAM_WE | _RAM_OE); delay(333);
        // SendAddress(0x000001, _FLASH_WE |  _FLASH_OE | _RAM_WE           ); delay(333);
  }
}
