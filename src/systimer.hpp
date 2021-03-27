//----------------------------------------------------------------------------

//Модуль системного таймера, заголовочный файл

//----------------------------------------------------------------------------

#ifndef SYSTIMER_HPP
#define SYSTIMER_HPP

//----------------------------- Константы: -----------------------------------

uint64_t const F_CLK = 16000000; //тактовая частота, Гц
uint16_t const T_SYS = 1000;     //системный интервал, мкс

//----------------------------------------------------------------------------
//--------------------------- Класс TSysTimer: -------------------------------
//----------------------------------------------------------------------------

#pragma vector = TIMER0_COMPA_vect
extern "C" __interrupt void SysTick_Handler(void);

class TSysTimer
{
private:
  friend __interrupt void SysTick_Handler(void);
  static uint8_t const DIV_SYS = 64;
  static uint8_t const TC0_VAL = (F_CLK / DIV_SYS * T_SYS / 100000 + 5) / 10;
  static volatile bool vTick;
  static volatile uint16_t vCounter;
  static uint16_t Start_ms;
  static uint16_t Interval_ms;
  static uint16_t GetCount(void);
protected:
public:
  static void Init(void);
  static bool Tick;
  static uint16_t Counter;
  static void Sync(void);
  static void Delay_ms(uint16_t d);
  static void TimeoutStart_ms(uint16_t t);
  static bool TimeoutOver_ms(void);
};

//----------------------------------------------------------------------------
//-------------------------- Класс TSoftTimer: -------------------------------
//----------------------------------------------------------------------------

class TSoftTimer : public TSysTimer
{
private:
  uint16_t Interval;
  uint16_t StartCount;
  bool fEvent;
  bool fRun;
protected:
public:
  TSoftTimer(uint16_t = 0);
  bool Autoreload;
  bool Oneshot;
  void Start(uint16_t t);
  void Start();
  void Stop();
  void SetInterval_ms(uint16_t t);
  void SetInterval_sec(uint16_t t);
  void Force(void);
  bool Over(void);
};

//----------------------------------------------------------------------------

#endif
