// #include <Arduino.h>
// #include "lowlevel.h"

// #define nOER 8
// #define nWER 4

// #define RAMSIZE 0x200000  //1024*2048
// #define RAMSTEP 512
// #define RAMRANDOMSTEP 0


// #ifdef __cplusplus
// extern "C" {
// #endif

// void WriteRAM(uint32_t a, uint32_t d) {
//   a=FixAddressForMemory(a);
//   SendAddress(a, 0xFF);
//   DataOutMode();
//   DataOut(d);
//   SendAddress(a, 0xFF & (~nWER));
//   SendAddress(a, 0xFF);
//   DataInMode();
// }

// uint32_t ReadRAM(uint32_t a) {
//   a=FixAddressForMemory(a);
//   DataInMode();
//   SendAddress(a, 0xFF & (~nOER));
//   uint32_t d = DataIn();
//   SendAddress(a, 0xFF);
//   return d;
// }

// #ifdef __cplusplus
// }
// #endif
