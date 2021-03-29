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

typedef TGpio<PORT_L, PIN_0> Pin_Tacho_t; //PL0 (ICP4) - Tacho in [control]
typedef TGpio<PORT_B, PIN_1> Pin_Lock_t;  //PB1 - Lock out [control]
typedef TGpio<PORT_B, PIN_7> Pin_Led_t;   //PB7 - LED out [control]
typedef TGpio<PORT_B, PIN_4> Pin_Pwm_t;   //PB4 (OC2A) - PWM out [pwm]

typedef TGpio<PORT_E, PIN_0> Pin_RXD_t;   //PD0 (RXD) - RXD [wakeport]
typedef TGpio<PORT_E, PIN_1> Pin_TXD_t;   //PD1 (TXD) - TXD [wakeport]
typedef TGpio<PORT_D, PIN_2> Pin_PD2_t;   //PD2 - not used [control]
typedef TGpio<PORT_D, PIN_3> Pin_Spd1_t;  //PD3 - Speed 1 [control]
typedef TGpio<PORT_D, PIN_4> Pin_Spd2_t;  //PD4 - Speed 2 [control]
typedef TGpio<PORT_D, PIN_5> Pin_Sleep_t; //PD5 - Sleep [control]
typedef TGpio<PORT_D, PIN_6> Pin_PD6_t;   //PD6 - not used [control]
typedef TGpio<PORT_D, PIN_7> Pin_PD7_t;   //PD7 - not used [control]

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
