#include "delay.h"
// #include "hal_gpio.h"
#include "led.h"
#include "printf.h"
#include "stm32f10x.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Set up the system clock for internal 8MHz HSI with PLL at 64MHz
void setclocks(void) {
  // Enable HSI
  RCC->CR |= RCC_CR_HSION;
  // Wait until HSI is ready
  while ((RCC->CR & RCC_CR_HSIRDY) == 0)
    ;

  // Set flash latency to 2
  FLASH->ACR &= ~FLASH_ACR_LATENCY;
  FLASH->ACR |= FLASH_ACR_LATENCY_2;

  // Set PLL multiplication factor to 8
  RCC->CFGR &= ~RCC_CFGR_PLLMULL;
  RCC->CFGR |= RCC_CFGR_PLLMULL8;

  // Enable PLL
  RCC->CR |= RCC_CR_PLLON;
  // Wait until PLL is ready
  while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    ;

  // Set system clock to PLL output
  RCC->CFGR &= ~RCC_CFGR_SW;
  RCC->CFGR |= RCC_CFGR_SW_PLL;

  // Wait until system clock is ready
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)
    ;

  // Set APB1 prescaler to 2 (when HCLK is 64MHz)
  RCC->CFGR &= ~RCC_CFGR_PPRE1;     // Clear prescaler bits
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV2; // Set prescaler to 2

  // APB2 does not need prescaling for 64MHz
  RCC->CFGR &= ~RCC_CFGR_PPRE2; // Clear prescaler bits
}

void enable_all_ports_clock() {
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; // Enable GPIOA clock
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; // Enable GPIOB clock
  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; // Enable GPIOC clock
  RCC->APB2ENR |= RCC_APB2ENR_IOPDEN; // Enable GPIOD clock
  RCC->APB2ENR |= RCC_APB2ENR_IOPEEN; // Enable GPIOE clock
  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; // Enable AFIO clock
}


#define GPIO_PIN(name, port, pin)                                              \
  static inline void GPIO_##name##_pp(void) {                                  \
    if (pin > 7) {                                                             \
      GPIO##port->CRH &= ~(0xF << ((pin - 8) * 4));                                                     \
      GPIO##port->CRH |= (GPIO_CRH_MODE8_0 << ((pin - 8) * 4));                \
    } else {                                                                   \
      GPIO##port->CRL &= ~(0xF << (pin * 4));                                                     \
      GPIO##port->CRL |= (GPIO_CRL_MODE0_0 << (pin * 4));                      \
    }                                                                          \
    (void)GPIO_##name##_pp;                                                    \
  }                                                                            \
                                                                               \
  static inline void GPIO_##name##_set(void) {                                 \
    GPIO##port->BSRR = GPIO_BSRR_BS0 << pin;                                   \
    (void)GPIO_##name##_set;                                                   \
  }                                                                            \
                                                                               \
  static inline void GPIO_##name##_clr(void) {                                 \
    GPIO##port->BRR = GPIO_BRR_BR0 << pin;                                     \
    (void)GPIO_##name##_clr;                                                   \
  }                                                                            \
  static inline bool GPIO_##name##_read(void) {                              \
    return (GPIO##port->IDR >> pin) & 1;                                       \
  }                                                                            \
                                                                               \
/*  Floating: To configure a GPIO pin as an input with floating, you need to 
              set the corresponding CNFy bits to '01'.

  GPIOA->CRL &= ~(GPIO_CRL_CNF1 | GPIO_CRL_MODE1); // Reset CNF and MODE bits
  GPIOA->CRL |= GPIO_CRL_CNF1_0; // Set CNF bits to '01' for floating input
                                                                            */ \
  static inline void GPIO_##name##_inFloat(void) {                             \
    if (pin > 7) {                                                             \
      GPIO##port->CRH &= ~(0xF << ((pin - 8) * 4));                                                     \
    } else {                                                                   \
      GPIO##port->CRL &= ~(0xF << (pin * 4));                                                     \
    }                                                                          \
    (void)GPIO_##name##_inFloat;                                               \
  }                                                                            \
                                                                               \
/*  Pull-up: To configure a GPIO pin as an input with pull-up, you need to 
            set the corresponding CNFy bits to '10' and 
            set the corresponding ODRy bit to '1'.

  GPIOA->CRL &= ~(GPIO_CRL_CNF1 | GPIO_CRL_MODE1); // Reset CNF and MODE bits
  GPIOA->CRL |= GPIO_CRL_CNF1_1; // Set CNF bits to '10' for pull-up/pull-down
  GPIOA->ODR |= GPIO_ODR_ODR1; // Set ODR bit to '1' for pull-up
                                                                            */ \
  static inline void GPIO_##name##_inPU(void) {                                \
    if (pin > 7) {                                                             \
      GPIO##port->CRH &= ~(0xF << ((pin - 8) * 4));                                                     \
      GPIO##port->CRH |=  ((GPIO_CRH_CNF8_1)<< ((pin - 8) * 4)); \
    } else {                                                                   \
      GPIO##port->CRL &= ~(0xF << (pin * 4));                                                     \
      GPIO##port->CRL |=  ((GPIO_CRL_CNF0_1) << (pin * 4));     \
    }                                                                          \
    GPIO##port->BSRR = 1 << pin;                                   \
    (void)GPIO_##name##_inPU;                                                  \
  }                                                                            \
                                                                               \
/* Pull-down: To configure a GPIO pin as an input with pull-down, you need 
              to set the corresponding CNFy bits to '10' and set 
              the corresponding ODRy bit to '0'.

GPIOA->CRL &= ~(GPIO_CRL_CNF1 | GPIO_CRL_MODE1); // Reset CNF and MODE bits
GPIOA->CRL |= GPIO_CRL_CNF1_1; // Set CNF bits to '10' for pull-up/pull-down
GPIOA->ODR &= ~GPIO_ODR_ODR1; // Set ODR bit to '0' for pull-down
                                                                            */ \
  static inline void GPIO_##name##_inPD(void) {                                \
    if (pin > 7) {                                                             \
      GPIO##port->CRH &= ~(0xF << ((pin - 8) * 4));                                                     \
      GPIO##port->CRH |= ((GPIO_CRH_CNF8_1)<< ((pin - 8) * 4));                 \
    } else {                                                                    \
      GPIO##port->CRL &= ~(0xF << (pin * 4));                                                     \
      GPIO##port->CRL |=  ((GPIO_CRL_CNF0_1) << (pin * 4));                       \
    }                                                                           \
    GPIO##port->BSRR = 1 << (16+pin);                                           \
    (void)GPIO_##name##_inPD;                                                   \
  }                                                                            


GPIO_PIN(PCe, D, 0)
GPIO_PIN(PCck, D, 1)
GPIO_PIN(nPCld, A, 0)
GPIO_PIN(Wr, A, 1)
GPIO_PIN(Rd, A, 2)
GPIO_PIN(nMARe, A, 3)
GPIO_PIN(MARck, A, 4)
GPIO_PIN(AvalCk, A, 5)
GPIO_PIN(Ack, A, 6)
GPIO_PIN(Bck, B, 9)
GPIO_PIN(BvalCk, B, 8)
GPIO_PIN(nResultE, B, 7)
// GPIO_PIN(LEQ, B, 6)
// GPIO_PIN(Aneg, B, 5)
// GPIO_PIN(Bneg, B, 4)
// GPIO_PIN(IOread, B, 3)
// GPIO_PIN(IOwrite, A, 15)
// GPIO_PIN(IOwriteP, B, 12)
// GPIO_PIN(IoReadP, A, 7)
// GPIO_PIN(MopEnd, B, 0)
// GPIO_PIN(Spare1, B, 1)
// GPIO_PIN(Spare2, B, 2)
// GPIO_PIN(Spare3, B, 10)
// GPIO_PIN(Spare4, B, 11)

GPIO_PIN(DISABLE, B, 13)
GPIO_PIN(LED, C, 13)

void setup_usart1(void) {
  // Enable USART1 clock
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

  // Configure USART1 pins
  GPIOA->CRH &=
      ~(GPIO_CRH_CNF9 | GPIO_CRH_MODE9); // Clear configuration for TX pin (PA9)
  GPIOA->CRH |=
      GPIO_CRH_CNF9_1 |
      GPIO_CRH_MODE9_0; // Set TX pin as alternate function output push-pull

  GPIOA->CRH &= ~(GPIO_CRH_CNF10 |
                  GPIO_CRH_MODE10); // Clear configuration for RX pin (PA10)
  GPIOA->CRH |= GPIO_CRH_CNF10_0;   // Set RX pin as floating input

  // Configure USART1
  USART1->BRR =64000000 /(2 * 9600); 
  USART1->CR1 = USART_CR1_TE | USART_CR1_RE |
                USART_CR1_UE; // Enable transmitter, receiver, and USART


  // Enable USART1 interrupt (if needed)
  // NVIC_EnableIRQ(USART1_IRQn);
}

// // Handle received serial data
// void USART1_IRQHandler(void) {
//   if (USART1->SR & USART_SR_RXNE) {
//     uint8_t data = USART1->DR;
//     GPIO_LED_clr();
//     delay(100);
//     GPIO_LED_set();
//     // Acknowledge the interrupt by writing a value to the SR register
//     USART1->SR &= ~USART_SR_RXNE;
//   }
// }


void printchar (void* p, char c) {
    while ((USART1->SR & USART_SR_TXE) == 0);
    USART1->DR =c; 
}


void setupTimer2() {
//   // Enable TIM2 clock
// RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

// // Set TIM2 prescaler to divide the system clock to 1MHz
// TIM2->PSC = 64000000 / 1000000 - 1;

// // Set TIM2 auto-reload register to generate an update event every 100ms
// TIM2->ARR = 100000 - 1;

// // Enable TIM2 update interrupt
// TIM2->DIER |= TIM_DIER_UIE;

// // Enable TIM2
// TIM2->CR1 |= TIM_CR1_CEN;

// // Enable TIM2 interrupt in NVIC
// NVIC_EnableIRQ(TIM2_IRQn);
}


void TIM2_IRQHandler(void) {
  // static int count = 0;
  // if (TIM2->SR & TIM_SR_UIF) {
  //   count++;
  //   if (count%2 == 0) {
  //     GPIO_LED_clr();
  //   } else {
  //     GPIO_LED_set();
  //   }

  //   // Clear update interrupt flag
  //   TIM2->SR &= ~TIM_SR_UIF;
  // }
}



void setup() {
  setclocks();
  enable_all_ports_clock();
  // Remap PD0 and PD1 (from XTAL) and PB3, PB4, PA15 (from JTAG) to GPIO
  AFIO->MAPR = AFIO_MAPR_PD01_REMAP | AFIO_MAPR_SWJ_CFG_JTAGDISABLE;
  setup_usart1();
  init_printf(NULL,printchar);
}

void DeactivateSignals() {
  GPIO_DISABLE_inPD();
  GPIO_LED_pp();
  GPIO_LED_set(); // Turn off LED
  GPIO_Wr_inPU();
  GPIO_Rd_inPU();
  GPIO_Ack_inPD();
  GPIO_Bck_inPD();
  GPIO_AvalCk_inPD();
  GPIO_BvalCk_inPD();
  GPIO_PCck_inPD();
  GPIO_MARck_inPD();
  GPIO_nResultE_inPU();
  GPIO_nMARe_inPU();
  GPIO_PCe_inPU();
  GPIO_nPCld_inPU();
  printf("Control signals floated\n\r");
} 

void IdleAllSignals() {
  GPIO_DISABLE_inPD();
  GPIO_LED_pp();
  GPIO_LED_clr(); // Turn on LED
  GPIO_Wr_pp(); GPIO_Wr_set();
  GPIO_Rd_pp(); GPIO_Rd_set();
  GPIO_Ack_pp(); GPIO_Ack_clr();
  GPIO_Bck_pp(); GPIO_Bck_clr();
  GPIO_AvalCk_pp(); GPIO_AvalCk_clr();
  GPIO_BvalCk_pp(); GPIO_AvalCk_clr();
  GPIO_PCck_pp(); GPIO_PCck_clr();
  GPIO_MARck_pp(); GPIO_MARck_clr();
  GPIO_nResultE_pp(); GPIO_nResultE_set();
  GPIO_nMARe_pp(); GPIO_nMARe_set();
  GPIO_PCe_pp();  GPIO_PCe_set();
  GPIO_nPCld_pp(); GPIO_nPCld_set();
  printf("Control signals set to default idle state\n\r");
} 

 void TestForDeactivation() {
  if (!GPIO_DISABLE_read()) return;
  DeactivateSignals();
  while (GPIO_DISABLE_read());
  IdleAllSignals();
 }


int main(void) {
  setup();
  printf("\n\rMOP Emulator\n\r");
  DeactivateSignals();
  IdleAllSignals();
  
  for (;;) {
    TestForDeactivation();

  GPIO_nMARe_set();
  GPIO_nResultE_set();
  GPIO_nPCld_set();
  GPIO_Ack_set();
  GPIO_Bck_set();
  GPIO_AvalCk_set();
  GPIO_BvalCk_set();

  delay(1000);
  GPIO_PCck_set();
  GPIO_PCck_clr();
  GPIO_Ack_clr();
  GPIO_Bck_clr();
  GPIO_AvalCk_clr();
  GPIO_BvalCk_clr();
  delay(1000);

// Check for incoming character at USART1 and read it.
    if (USART1->SR & USART_SR_RXNE) {
      uint8_t data = USART1->DR;
      if (data=='=') {
        IdleAllSignals();
      }

      if (data=='.') {
        GPIO_nPCld_clr();
        GPIO_PCck_set();
        GPIO_PCck_clr();
        GPIO_nPCld_set();

      }


      if (data=='0') {
        printf("State 0 - ~PCe ~Rd\n\r");
        GPIO_PCe_clr();
        GPIO_Rd_clr();
      }
      if (data=='1') {
        printf("State 1 -\n\r");
        GPIO_PCe_set();
        GPIO_Rd_clr();
        GPIO_nMARe_clr();
        GPIO_Ack_set();
        GPIO_PCck_set();
        GPIO_MARck_set();
      }
      if (data=='2') {
        printf("State 2 -\n\r");
        GPIO_PCe_clr();
        GPIO_Rd_clr();
        GPIO_nMARe_set();
        GPIO_Ack_clr();
        GPIO_PCck_clr();
        GPIO_MARck_clr();
        GPIO_AvalCk_set();
        GPIO_nMARe_clr();
      }

    }


    // delay(100);
      // GPIO_PCck_set();
    // delay(100);
      // GPIO_PCck_clr();
      // GPIO_PCe_clr();
      // GPIO_Rd_clr();
      // GPIO_nPCld_set();

    // GPIO_LED_set(); delay(1000);
    // USART1->DR = '@'; while ((USART1->SR & USART_SR_TXE) == 0);
    // delay(1000);
  }
}
