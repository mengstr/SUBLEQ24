#include "lowlevel.h"
// #include "ram.h"
// #include "flash.h"

// SUBLEQ a, b, c
// Mem[b] := Mem[b] - Mem[a]
// if (Mem[b] ≤ 0) goto c

// 0x000000 RAM bank 0
// 0x100000 RAM bank 0
// 0x200000 
// 0x300000
// 0x400000 RAM bank 1
// 0x500000 RAM bank 1
// 0x600000 FLASH0
// 0x680000 (FLASH1)
// 0x700000 (FlASH2)
// 0x780000 (FLASH3)
// 0x800000 
// 0x900000
// 0xA00000 
// 0xB00000
// 0xC00000 
// 0xD00000
// 0xE00000 
// 0xF00000

#define RAMOFFSET 0x000000
#define RAMLENGTH (1024 * 2048)

#define FLASHLENGTH (1024 * 256)
#define FLASHOFFSET 0x600000

#define DISABLE_SUBLEQ 33
#define nRD 25
#define nWR 24

void setup() {
  Serial.begin(9600);  // USB is always 12 Mbit/sec
  delay(1000);
  Serial.println("\n\nExternal Loader v0.1 - Enter ? for help\n");
  DataInMode();
  setupShiftRegister();
  shiftRegisterOE(false);

  pinMode(27, INPUT_DISABLE); // Tredje uppifrån
  pinMode(26, INPUT_DISABLE); // Andra uppifrån
  pinMode(DISABLE_SUBLEQ, OUTPUT); // DISABLE SUBLEQ - Tredje nedifrån
  pinMode(nWR, INPUT_DISABLE); // ~WR - Andra nedifrån 
  pinMode(nRD, INPUT_DISABLE); // ~RD - Första nedifrån

}

uint8_t cb=0;

char *getstringfromserial() {
  static char buf[100];
  buf[0]=0;
  for (;;) {
    if (Serial.available()) {
      char c=Serial.read();
      if (c==13) return buf;
      if (c>=32) {
        // strncat(buf,&c,1);
        int l=strlen(buf);
        buf[l]=c;
        buf[l+1]=0;
      }
    }
  }
}



void Write(int a, int v) {
  SendAddress(a,0);
  DataOut(v);
  digitalWrite(nWR,LOW);
  digitalWrite(nWR,HIGH);
}

int Read(int a) {
    SendAddress(a,0);
    digitalWrite(nRD,LOW);
    int v=DataIn();
    digitalWrite(nRD,HIGH);
    return v;
}


void loop() {
  
  char *buf=getstringfromserial();

  if (buf[0]=='h') {
    Serial.println("Stopping SubLEQ");
    DataOutMode();
    DataOut(0xBEEFED);
                      // DataInMode();
    shiftRegisterOE(true);
    SendAddress(0xFADEDD,0x55);
    pinMode(nWR, INPUT); // ~WR - Andra nedifrån 
    pinMode(nRD, INPUT); // ~RD - Första nedifrån
    digitalWrite(DISABLE_SUBLEQ,HIGH);
    digitalWrite(nWR,HIGH);
    pinMode(nWR, OUTPUT); // ~WR - Andra nedifrån 
    digitalWrite(nWR,HIGH);
    digitalWrite(nRD,HIGH);
    pinMode(nRD, OUTPUT); // ~RD - Första nedifrån
    digitalWrite(nRD,HIGH);
  }


  if (buf[0]=='c') {
    DataInMode();
    shiftRegisterOE(false);
    pinMode(nWR, INPUT); // ~WR - Andra nedifrån 
    pinMode(nRD, INPUT); // ~RD - Första nedifrån
    digitalWrite(DISABLE_SUBLEQ,LOW);
    digitalWrite(33,LOW);
    pinMode(nWR, INPUT_DISABLE); // ~WR - Andra nedifrån 
    pinMode(nRD, INPUT_DISABLE); // ~RD - Första nedifrån
    Serial.println("Starting SubLEQ");
  }

  if (buf[0]=='r') {
    DataInMode();
    for (int a=0; a<128; a++) {
      int v=Read(a);
      if (a % 8==0) Serial.printf("\r\n%06x: ",a);
      Serial.printf("%06x ",v);
    }
    Serial.println();
  }

 if (buf[0]=='R') {
    DataInMode();
    for (int a=FLASHOFFSET+0; a<FLASHOFFSET+128; a++) {
      int v=Read(a);
      if (a % 8==0) Serial.printf("\r\n%06x: ",a);
      Serial.printf("%06x ",v);
    }
    Serial.println();
  }

  if (buf[0]=='l') {
    Serial.println("Loading test into RAM 0x000000");
    DataOutMode();
    for (int a=0; a<1024; a++) Write(a,0x555555);
    Write(0,6);
    Write(1,7);
    Write(2,3);
    Write(3,8);
    Write(4,8);
    Write(5,0xFFFFFF);
    Write(6,1);
    Write(7,0x123456);
    Write(8,0xaaaaaa);
  }

  if (buf[0]=='t') {
    unsigned long seed=millis();
    int ramteststep=128;
    int ramsize=RAMLENGTH;

    Serial.println("Testing RAM (writing)");
    randomSeed(seed);
    DataOutMode();
    for (int a=0; a<ramsize; a++) {
      Write(RAMOFFSET+a,random(1<<24));
      // a+=random(ramteststep);
    }

    Serial.println("Testing RAM (verifying)");
    randomSeed(seed);
    DataInMode();
    int ok=0;
    int fail=0;
    for (int a=0; a<ramsize; a++) {
      int v=Read(RAMOFFSET+a);
      int expectedV=random(1<<24);
      if (v==expectedV) ok++; else fail ++;
      if (v!=expectedV) Serial.printf("Wanted %06x got %06x\r\n",expectedV,v);
      // a+=random(ramteststep);
    }
    Serial.printf("%d correct, %d failed RAM locations\r\n",ok,fail);
  }


  if (buf[0]=='T') {
    unsigned long seed=millis();
    int flashteststep=128;
    int flashsize=FLASHLENGTH;

    Serial.println("Testing FLASH (erasing)");
    DataOutMode();
    Write(FLASHOFFSET+0x005555, 0xAAAAAA); delay(250);
    Write(FLASHOFFSET+0x002AAA, 0x555555); delay(250);
    Write(FLASHOFFSET+0x005555, 0x808080); delay(250);
    Write(FLASHOFFSET+0x005555, 0xAAAAAA); delay(250);
    Write(FLASHOFFSET+0x002AAA, 0x555555); delay(250);
    Write(FLASHOFFSET+0x005555, 0x101010); delay(250);
    delay(500);

    Serial.println("Testing FLASH (writing)");
    randomSeed(seed);
    DataOutMode();
    for (int a=0,cnt=0; a<flashsize; a++,cnt++) {
      Write(FLASHOFFSET+0x005555,0xAAAAAA);
      Write(FLASHOFFSET+0x002AAA,0x555555);
      Write(FLASHOFFSET+0x005555,0xA0A0A0);
      int v=random(1<<24);
      Write(FLASHOFFSET+a,v);
      // if (cnt%64) delay(50);
      a+=random(flashteststep);
    }

    Serial.println("Testing FLASH (verifying)");
    randomSeed(seed);
    DataInMode();
    int ok=0;
    int fail=0;
    for (int a=0; a<flashsize; a++) {
      int v=Read(FLASHOFFSET+a);
      int expectedV=random(1<<24);
      if (v==expectedV) ok++; else fail ++;
      if (v!=expectedV) Serial.printf("Wanted %06x got %06x\r\n",expectedV,v);
      a+=random(flashteststep);
    }
    Serial.printf("%d correct, %d failed FLASH locations\r\n",ok,fail);
   }

}

  
  // cb&=~CB_GPIOb;  SendAddress(-1, cb);
  // cb|=CB_nGPIOwp;  SendAddress(-1, cb);
  // cb&=~CB_SUBCkA;  SendAddress(-1, cb);
  // cb&=~CB_SUBCkB;  SendAddress(-1, cb);
  // cb|=CB_nSUBoe;  SendAddress(-1, cb);


  // for (;;) {
  //   DataOut(0); SendAddress(0,0); delay(500);
  //   for (int v=0; v<24; v++) {
  //     DataOut(1<<v); SendAddress(1<<v,v&1); delay(500);
  //   }
  // }






  // SendAddress(0xFFFFFF,0); delay(100);
  // DataOutMode();


  // for (;;) {
  //   uint32_t a=1234567;
  //   uint32_t b=1234568;

  // DataOut(a); delay(500);
  // cb|=CB_SUBCkA;  SendAddress(-1, cb);
  // cb&=~CB_SUBCkA;  SendAddress(-1, cb);

  // // DataOut(b^0xFFFFFF); delay(500);
  // DataOut(b); delay(500);
  // cb|=CB_SUBCkB;  SendAddress(-1, cb);
  // cb&=~CB_SUBCkB;  SendAddress(-1, cb);

  // DataInMode();
  // cb&=~CB_nSUBoe;  SendAddress(-1, cb);
  // int32_t res=DataIn();
  // cb|=CB_nSUBoe;  SendAddress(-1, cb);
  // DataOutMode();
  // if (res>0x7FFFFF) {res=res-0xFFFFFF-1; }

  // Serial.printf("%ld-%ld=%ld\n",a,b,res);

  // }



// uint32_t e=0x55AA0F;

// for (;;) {
//   for (uint32_t a=0; a<64; a++) WriteRAM(a,e);
//   cntOk=0;
//   cntErr=0;
//   for (uint32_t a = 0; a < 64; a++) {
//     uint32_t v = ReadRAM(a);
//     if (v==e) cntOk++; else 
//     {
//       cntErr++;
//       Serial.printf("At %06x got %06x expected %06x diff=%06x\n",a,v,e,v^e);
//     }
//   }
//   Serial.printf("RAM: %06x OK=%ld Err=%ld\n",e,cntOk,cntErr);
//   delay(1000);
// }





// #if 0
//   DataInMode();
//   int cnt=0;
//   for (;;) {
//   SendAddress(0x000000,0xFF); delay(50);
//   SendAddress(1<<cnt,0xFF); delay(50);
//      if (Serial.available() > 0) {
//         char ch = Serial.read();
//         if (ch=='s') {
//           cnt++;
//           if (cnt>23) cnt=0;
//         }
//         if (ch=='a') {
//           cnt--;
//           if (cnt<0) cnt=23;
//         }
//         Serial.printf("Bit %d\n",cnt);
//      }
//   }
// #endif

// #if 1
// Serial.println("Writing random data to RAM");
// randomSeed(seed);
// for (uint32_t a=0; a<RAMSIZE; a+=RAMSTEP) {
//   WriteRAM(a,random(1<<24));
//   if (RAMRANDOMSTEP>0) a+=random(RAMRANDOMSTEP);
// }

//  Serial.println("Verifying RAM");
//  randomSeed(seed);
//  DataInMode();
// cntOk=0;
// cntErr=0;
// for (uint32_t a = 0; a < RAMSIZE; a+=RAMSTEP) {
//     uint32_t v = ReadRAM(a);
//     uint32_t expect=random(1<<24);
//     if (v==expect) cntOk++; else {
//       cntErr++;
//       Serial.printf("At %06x got %06x expected %06x diff=%06x\n",a,v,expect,v^expect);
//     }
//   if (RAMRANDOMSTEP>0) a+=random(RAMRANDOMSTEP);
// }
// Serial.printf("RAM: OK=%ld Err=%ld\n",cntOk,cntErr);
// #endif

// #if 0
// for (int i=0; i<2; i++) {
//   Serial.println("Erasing FLASH");
//   DataOutMode();
//   WriteFlash(0x5555, 0xAAAAAA);
//   WriteFlash(0x2AAA, 0x555555);
//   WriteFlash(0x5555, 0x808080);
//   WriteFlash(0x5555, 0xAAAAAA);
//   WriteFlash(0x2AAA, 0x555555);
//   WriteFlash(0x5555, 0x101010);
//   delay(500);
// }
// #endif

// #if 0
// Serial.println("Writing random data to FLASH");
// randomSeed(seed);
// for (uint32_t a=0; a<FLASHSIZE; a++) {
// //      (original)   0x5555:0xAA    0x2AAA:0x55    0x5555:0xA0   Address:Data 
//   WriteFlash(FLASHOFFSET+0x5555,0xAAAAAA);
//   WriteFlash(FLASHOFFSET+0x2AAA,0x555555);
//   WriteFlash(FLASHOFFSET+0x5555,0xA0A0A0);
//   WriteFlash(FLASHOFFSET+a,random(1<<24));
//   delay(0);
//   a+=random(FLASHSTEP);
// }

//  Serial.println("Verifying FLASH");
//  randomSeed(seed);
//  DataInMode();
// cntOk=0;
// cntErr=0;
// for (uint32_t a = 0; a < FLASHSIZE; a++) {
//     uint32_t v=ReadFlash(FLASHOFFSET+a);
//     uint32_t expect=random(1<<24);
//     if (v==expect) cntOk++; else {
//       Serial.printf("address %06x wanted %06x got %06x\n",FLASHOFFSET+a,expect,v);
//       cntErr++;
//     }
//     a+=random(FLASHSTEP);
// }
// Serial.printf("FLASH: OK=%ld Err=%ld\n",cntOk,cntErr);
// #endif

// #if 0
//   //  *** DUMP FLASH
//   DataInMode();
//   for (uint32_t a = 0; a < 4096; a++) {
//     uint32_t v = ReadFlash(FLASHOFFSET+a);
//     if ((i % 16) == 0) Serial.printf("%08x: ", a);
//     Serial.printf("%06x ", v);
//     if ((i % 16) == 15) Serial.println();
//   }
// #endif

// #if 0
//   Serial.println("Done");
//   uint32_t a=0;
//   uint32_t d=0;
//   uint8_t redleds=random(256)|_MEM_CE;
//   DataOutMode();
//   for(;;) {
//     if (random(100)<10) redleds=random(256)|_MEM_CE;
//       SendAddress(a, redleds); 
//       delay(10);
//       DataOut(d);
//       if (random(100)<20) {
//         a|=1<<random(2^24);
//         d|=1<<random(2^24);
//       } else {
//         a&=~(1<<random(2^24));
//         d&=~(1<<random(2^24));
//       }
//   }
// #endif

//   delay(1000);
//   seed++;
// }
