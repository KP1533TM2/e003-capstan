//----------------------------------------------------------------------------

//Главный модуль, заголовочный файл

//----------------------------------------------------------------------------

#ifndef MAIN_HPP
#define MAIN_HPP

//----------------------------------------------------------------------------

#include <ioavr.h>
#include <intrinsics.h>
#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include <stdbool.h>
#include "gpio.hpp"
#include "systimer.hpp"

//-------------------------- Номер версии firmware: --------------------------

#define VERSION "1.00"

//-------------- Использование выводов процессора [module]: ------------------

typedef TGpio<PORT_C, PIN_1> Pin_Led_t;   //PC1 - LED out [control]
typedef TGpio<PORT_D, PIN_2> Pin_Relay_t; //PD2 - Reverse relay
typedef TGpio<PORT_C, PIN_3> Pin_Fref_t;  //PC3 - Fref out (oscilloscope) (normally PC3)
typedef TGpio<PORT_C, PIN_4> Pin_Debug_t; //PC4 - Oscilloscope debug out
typedef TGpio<PORT_B, PIN_1> Pin_Dir_t;   //PB1 - Reverse [control]
typedef TGpio<PORT_B, PIN_2> Pin_Spd1_t;  //PB2 - Speed 1 [control], 9/19

typedef TGpio<PORT_B, PIN_0> Pin_Tacho_t; //PB0 (ICP0) - Tacho in [control]
typedef TGpio<PORT_D, PIN_3> Pin_Pwm_t;   //PB3 (OC2B) - PWM out [pwm]
typedef TGpio<PORT_C, PIN_2> Pin_Zcd_t;   //PC2 - Zero Crossing Detector

typedef TGpio<PORT_D, PIN_0> Pin_RXD_t;   //PD0 (RXD) - RXD [wakeport]
typedef TGpio<PORT_D, PIN_1> Pin_TXD_t;   //PD1 (TXD) - TXD [wakeport]

typedef TGpio<PORT_D, PIN_4> Pin_Lock_t;  //PD4 - Lock out [control] NOT USED
typedef TGpio<PORT_D, PIN_5> Pin_Sleep_t; //PD5 - Sleep [control] NOT USED
//typedef TGpio<PORT_D, PIN_6> Pin_Spd2_t;  //PD6 - Speed 2 [control], FWD/REV

//-------------- Использование аппаратных блоков процессора: -----------------

//Timer0: system timer [systimer]
//Timer1: reference divider [pll]
//Timer2: PWM generation [pwm]
//USART: computer port [wakeport]
//Comparator : NOT USED
//ADC: NOT USED
//SPI: NOT USED
//TWI: NOT USED

//----------------------- Использование прерываний: --------------------------

//TIMER0_COMPA: system tick [systimer]
//TIMER1_CAPT: speed sensor [pll]
//TIMER1_COMPA: phase estimation [pll]
//TIMER2_COMPA: delta-sigma modulator [pwm]

//---------------------------- Макроопределения: -----------------------------

#define LO(x)    ((uint8_t)((x) & 0xFF))
#define HI(x)    ((uint8_t)(((x) >> 8) & 0xFF))
#define BYTE1(x) (LO(x))
#define BYTE2(x) (HI(x))
#define BYTE3(x) ((uint8_t)(((uint32_t)(x) >> 16) & 0xFF))
#define BYTE4(x) ((uint8_t)(((uint32_t)(x) >> 24) & 0xFF))

#define WORD(b1,b0)        (((uint16_t)(b1) << 8) | (b0))
#define DWORD(b3,b2,b1,b0) (((uint32_t)WORD(b3,b2) << 16) | WORD(b1,b0))

#define ABS(x) ((x < 0)? -x : x)

#ifndef NULL
  #define NULL _NULL
#endif

//----------------------------------------------------------------------------

#endif
