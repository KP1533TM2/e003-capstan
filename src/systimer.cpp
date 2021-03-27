//----------------------------------------------------------------------------

//Модуль системного таймера

//----------------------------------------------------------------------------

#include "main.hpp"
#include "systimer.hpp"

//----------------------- Используемые ресурсы: ------------------------------

//Системный таймер реализован на основе прерываний таймера 0 с периодом 1 мс.
//Системный тик с таким периодом раздается всем процессам. Кроме того,
//в этой сетке работают 16-разрядные програмные таймеры (TSoftTimer),
//которые могут создаваться по мере необходимости.

//----------------------------------------------------------------------------
//--------------------------- Класс TSysTimer --------------------------------
//----------------------------------------------------------------------------

//-------------------- Инициализация системного таймера: ---------------------

void TSysTimer::Init(void)
{
  TCCR0A = (1 << WGM01);           //CTC
  TCCR0B = (1 << CS01) | (1 << CS00); //CK/64
  OCR0A = TC0_VAL - 1;             //загрузка периода
  TIFR0 = (1 << OCF0A);            //сброс отложенных прерываний
  TIMSK0 = (1 << OCIE0A);          //разрешение прерывания по совпадению
  vTick = 1;
  vCounter = 0;
  Counter = 0;
  __enable_interrupt();            //разрешение прерываний
}

//--------------------- Прерывание системного таймера: -----------------------

volatile bool TSysTimer::vTick;
volatile uint16_t TSysTimer::vCounter;

#pragma vector = TIMER0_COMPA_vect
__nested __interrupt void SysTick_Handler(void)
{
  TSysTimer::vCounter++;
  TSysTimer::vTick = 1;
}

//-------------------- Синхронизация системных тиков: ------------------------

bool TSysTimer::Tick;
uint16_t TSysTimer::Counter;

__monitor void TSysTimer::Sync(void)
{
  Counter = vCounter;
  Tick = vTick;
  vTick = 0;
}

//---------------------- Атомарное чтение счетчика: --------------------------

__monitor uint16_t TSysTimer::GetCount(void)
{
  return(vCounter);
}

//-------------- Функция задержки миллисекундного диапазона: -----------------

void TSysTimer::Delay_ms(uint16_t d)
{
  uint16_t DelayStart = GetCount();
  do { Sync(); __watchdog_reset(); }
    while(Counter - DelayStart < d);
}

//------------------ Таймаут миллисекундного диапазона: ----------------------

uint16_t TSysTimer::Start_ms;
uint16_t TSysTimer::Interval_ms;

void TSysTimer::TimeoutStart_ms(uint16_t t)
{
  Start_ms = GetCount();
  Interval_ms = t;
}

bool TSysTimer::TimeoutOver_ms(void)
{
  return(GetCount() - Start_ms >= Interval_ms);
}

//----------------------------------------------------------------------------
//-------------------------- Класс TSoftTimer: -------------------------------
//----------------------------------------------------------------------------

//---------------------------- Конструктор: ----------------------------------

TSoftTimer::TSoftTimer(uint16_t t)
{
  Interval = t;
  Autoreload = 0;
  Oneshot = 0;
  fRun = 0;
  fEvent = 0;
  StartCount = TSysTimer::Counter;
}

//-------------------------------- Старт: ------------------------------------

void TSoftTimer::Start(uint16_t t)
{
  Interval = t;
  fRun = 1;
  fEvent = 0;
  StartCount = TSysTimer::Counter;
}

void TSoftTimer::Start()
{
  fRun = 1;
  fEvent = 0;
  StartCount = TSysTimer::Counter;
}

//--------------------------------- Стоп: ------------------------------------

void TSoftTimer::Stop()
{
  fRun = 0;
}

//------------------------- Установка интервала: -----------------------------

void TSoftTimer::SetInterval_ms(uint16_t t)
{
  Interval = t;
}

void TSoftTimer::SetInterval_sec(uint16_t t)
{
  Interval = t * 1000;
}

//---------------------- Принудительное переполнение: ------------------------

void TSoftTimer::Force(void)
{
  fRun = 1;
  StartCount = TSysTimer::Counter - Interval;
}

//------------------------- Чтение переполнения: -----------------------------

bool TSoftTimer::Over(void)
{
  bool fevent = TSysTimer::Counter - StartCount >= Interval;
  if(fevent)
  {
    if(Oneshot && fEvent) fevent = 0;
    fEvent = 1;
    if(Autoreload) StartCount = TSysTimer::Counter;
  }
  if(Oneshot || Autoreload) return(fevent && fRun);
  return(fEvent && fRun);
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
