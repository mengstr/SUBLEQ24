// #include <Arduino.h>
// #include "lowlevel.h"

// #define nWEF 1
// #define nOEF 2

// // FLASH Chip-Erase
// //          -------- ~ ----------- ~ ----------- ~ ----------- ~ -----------
// // 0x5555    0  0  0 ~  0  1  0  1 ~  0  1  0  1 ~  0  1  0  1 ~  0  1  0  1
// //          -------- ~ ----------- ~ ----------- ~ ----------- ~ -----------
// // On Chip  18 17 16 ~ 15 14 13 12 ~ 11 10  9  8 ~  7  6  5  4 ~  3  2  1  0
// // Bus.      2  3  1 ~  0  4  6 18 ~ 12 15 10  8  ~ 5  7  9 11 ~ 13 14 16 17  = 011 0110 0100 1100 1000 = 0x364c8
// //          -------- ~ ----------- ~ ----------- ~ ----------- ~ -----------
// //          -------- ~ ----------- ~ ----------- ~ ----------- ~ -----------
// //          -------- ~ ----------- ~ ----------- ~ ----------- ~ -----------
// // 0x2AAA    0  0  0 ~  0  0  1  0 ~ 1  0  1   0 ~  1  0  1  0 ~  1  0  1  0
// //          -------- ~ ----------- ~ ----------- ~ ----------- ~ -----------
// // On Chip  18 17 16 ~ 15 14 13 12 ~ 11 10  9  8 ~  7  6  5  4 ~  3  2  1  0
// // Bus.      2  3  1 ~  0  4  6 18 ~ 12 15 10  8 ~  5  7  9 11 ~ 13 14 16 17  = 001 0011 0110 0110 0000 = 0x13660
// //          -------- ~ ----------- ~ ----------- ~ ----------- ~ -----------
// //
// //      (original)   0x5555:0xAA   0x2AAA:0x55   0x5555:0x80.  0x5555:0xAA   0x2AAA:0x55   0x5555:0x10
// //

// #define FLASHSTART 0x600000
// #define FLASHSIZE 1024 * 256
// #define FLASHSTEP 25

// #ifdef __cplusplus
// extern "C" {
// #endif

// void WriteFlash(uint32_t a, uint32_t d) {
//   a=FixAddressForMemory(a);
//   SendAddress(a, 0xFF);
//   DataOutMode();
//   DataOut(d);
//   SendAddress(a, 0xFF & (~nWEF));
//   SendAddress(a, 0xFF);
//   DataInMode();
// }

// uint32_t ReadFlash(uint32_t a) {
//   a=FixAddressForMemory(a);
//   DataInMode();
//   SendAddress(a, 0xFF & (~nOEF));
//   uint32_t d = DataIn();
//   SendAddress(a, 0xFF);
//   return d;
// }


// #ifdef __cplusplus
// }
// #endif
