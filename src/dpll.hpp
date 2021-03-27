//----------------------------------------------------------------------------

//Модуль DPLL, заголовочный файл

//----------------------------------------------------------------------------

#ifndef DPLL_HPP
#define DPLL_HPP

//----------------------------------------------------------------------------

#include "pwm.hpp"

//----------------------------------------------------------------------------

typedef struct
{
  uint8_t p;
  uint8_t i;
  uint8_t d;
} kpid_t;

//----------------------------------------------------------------------------
//------------------------------ Класс TDpll: --------------------------------
//----------------------------------------------------------------------------

#pragma vector = TIMER1_CAPT_vect
extern "C" __interrupt void Capture(void);
#pragma vector = TIMER1_COMPA_vect
extern "C" __interrupt void Compare(void);

class TDpll
{
  friend __interrupt void Capture(void);
  friend __interrupt void Compare(void);
  friend class TPort;
private:
  TPwm *Pwm;
  Pin_Tacho_t Pin_Tacho;
  static uint16_t vCapV;    //захваченное значение
  static uint8_t  vCapN;    //количество входных импульсов
  static uint8_t  vOvfN;    //количество переполнений таймера
  static uint8_t  vOvfC;    //счетчик переполнений таймера
  static uint16_t HPeriod;  //середина периода опорной частоты
  void AtomicReadCapture(void); //атомарное чтение данных захвата
  uint16_t preCapV, CapV;   //захваченные значение для измерения частоты
  uint8_t  preCapN, CapN;   //номера захватов для измерения частоты
  uint8_t  preOvfN, OvfN;   //количество переполнений для измерения частоты
  static int8_t  vPfdState; //состояние фазового детектора
  void AtomicReadCompare(void); //атомарное чтение данных фазового детектора
  static uint16_t vPhase;   //выходной код фазового детектора
  uint16_t Phase;           //текущее значение фазы
  static bool vPfdUpd;      //флаг обновления значения фазы
  TSoftTimer *LockTimer;    //таймер детектора захвата
  static const uint16_t LOCK_TM = 100; //время определения захвата
  static const uint16_t PH_ERR = 120; //допустимое отклонение фазы, градусов
  static const uint8_t PH_MIN = UINT8_MAX / 2 - UINT8_MAX * PH_ERR / 360;
  static const uint8_t PH_MAX = UINT8_MAX / 2 + UINT8_MAX * PH_ERR / 360;
  bool Lock;                //флаг захвата частоты
  uint16_t Speed;           //заданная скорость, x 0.1 Hz
  uint16_t Tacho;           //частота тахогенератора, x 0.1 Hz
  uint16_t Period;          //период опорной частоты
  uint8_t Dir;              //текущее направление вращения
  kpid_t K;                 //коэффициенты PID
  uint16_t Xp;              //PID inp(n-1)
  uint16_t Xpp;             //PID inp(n-2)
  int32_t Yp;               //PID out(n-1) * 512
  uint16_t Pid(uint16_t inp); //PID-регулятор
  static const uint32_t MAX_PFD = UINT16_MAX;     //максимальный код фазы
  static const uint32_t NOM_PFD = UINT16_MAX / 2; //номинальный код фазы
  static const uint32_t MIN_PFD = 0;              //минимальный код фазы
  TSoftTimer *MeterTimer;   //таймер измерения частоты
  static const uint16_t METER_TM = 20; //интервал измерения частоты
  static const uint8_t NOM_KP = 20; //значение Kp по умолчанию
  static const uint8_t NOM_KI = 20; //значение Ki по умолчанию
  static const uint8_t NOM_KD = 50; //значение Kd по умолчанию
public:
  TDpll(void);
  void Execute(void);
  bool Enable;              //разрешение PID
  void SetPID(kpid_t k);    //установка коэффициентов PID
  kpid_t GetPID(void);      //чтение коэффициентов PID
  void SetSpeed(uint16_t speed); //установка скорости
  uint16_t GetSpeed(void);  //чтение заданной скорости
  uint16_t GetTacho(void);  //чтение текущей скорости
  uint16_t GetPhase(void);  //чтение текущей фазы
  void SetDir(uint8_t dir); //задание направления вращения
  bool Locked(void);        //чтение захвата частоты
  void EERead(void);        //чтение параметров из EEPROM
  void EESave(void);        //сохранение параметров в EEPROM
};

//----------------------------------------------------------------------------

#endif
