//----------------------------------------------------------------------------

//Модуль DPLL

//------------------------- Используемые ресурсы: ----------------------------

//Стабилизация частоты вращения двигателя осуществляется с помощью цифровой
//петли DPLL.

//Двигатель содержит таходатчик, который формирует на выходе синусоидальный
//сигнал. Датчик представляет собой магнитную головку с подмагничивающим
//магнитом, возле зазора которой вращается ферромагнитная шестерня, имеющая
//174 зуба. Сигнал датчика поступает на триггер Шмитта, который формирует
//на выходе прямоугольные импульсы. На каждый оборот вала двигателя приходится
//174 импульса. Для скорости ленты 19.05 см/с номинальная частота равна 1348 Гц.

//Сигнал таходатчика сравнивается с опорным сигналом, который формирует Timer1.
//Сравнение производится с помощью частотно-фазового детектора, реализованного
//программно в обработчиках прерывания по событию захвата (Capture) и событию
//совпадения (Compare), которое означает переполнение таймера.

//Сигнал таходатчика поступает на вход захвата ICP. Захваченное значение
//представляет собой текущую фазу сигнала. Эта фаза сравнивается с опорной (0°),
//в результате формируется значение ошибки фазы. Ошибка фазы может принимать
//значения от -180° до +180°.

//Когда система не находится в режиме захвата, частота таходатчика может сильно
//отличаться от опорной частоты. В таком режиме величина ошибки фазы теряет
//смысл. Поэтому ее значение поддерживается равным -180°, когда частота выше
//номинальной, и +180°, когда частота ниже.

//В обработчике прерывания Capture производится сохранение текущего значения
//таймера, а также увеличение на 1 состояния фазового дететора vPfdState.
//В обработчике прерывания Compare состояние фазового дететора vPfdState
//уменьшаетя на 1. Затем оно сравнивается с нулем.

//Если входная частота равна опорной, на каждое событие Capture приходится
//одно событие Compare. При этом значения vPfdState чередуются: 1, 0, 1, 0 и т.д.
//В прерывании Compare после декремента vPfdState = 0, это означает номинальную
//частоту. Выходное значение PFD в этом случае равно захваченному значению фазы.

//Capture: __|_____|_____|_____|_____|_____|_____|_____|_____|____
//Compare: _____|_____|_____|_____|_____|_____|_____|_____|_____|_
//State:     1  0  1  0  1  0  1  0  1  0  1  0  1  0  1  0  1  0
//FREQ:        OK    OK    OK    OK    OK    OK    OK    OK    OK

//Если входная частота становится выше опорной, между двумя событиями Compare
//происходят два (или больше) события Capture. При этом vPfdState становится
//больше 1. В прерывании Compare после декремента vPfdState > 0. Это
//означает повышенную частоту. Выходное значение PFD в этом случае равно
//минимальному значению фазы (опережение). При выходе из обработчика Compare
//значение vPfdState ограничивается величиной 1.

//Если входная частота восстанавливается и на одно событие Compare снова
//приходится одно событие Capture, работа детектора не восстанавливается.
//Для восстановления необходим один цикл, когда между событиями Compare
//не будет событий Capture.

//Capture: __|____|_|___|||____|___________|_____|_____|_____|____
//Compare: _____|_____|_____|_____|_____|_____|_____|_____|_____|_
//State:     1  0 1 2 1 234 1  2  1     0  1  0  1  0  1  0  1  0
//FREQ:        OK     H     H     H    OK    OK    OK    OK    OK

//Если входная частота становится ниже опорной, между двумя событиями Compare
//может вообще не быть событий Capture. При этом события Compare происходят два
//(или больше) раза подряд, после декремента значение vPfdState < 0. Это
//означает пониженную частоту. Выходное значение PFD в этом случае равно
//максимальному значению фазы (отставание). При выходе из обработчика Compare
//значение vPfdState ограничивается величиной -1.

//Если входная частота восстанавливается и на одно событие Compare снова
//приходится одно событие Capture, работа детектора не восстанавливается.
//Для восстановления необходим один цикл, когда между событиями Compare
//будут два события Capture.

//Capture: __|___________|_____|____|_|____|_____|_____|_____|____
//Compare: _____|_____|_____|_____|_____|_____|_____|_____|_____|_
//State:     1  0    -1  0 -1  0 -1 0 1 0  1  0  1  0  1  0  1  0
//FREQ:        OK     L     L     L    OK    OK    OK    OK    OK

//С выхода частотно-фазового детектора код ошибки фазы поступает на петлевой
//фильтр, реализованный в виде PID-регулятора. Выходной код регулятора подается
//на модуль PWM, который формирует сигнал управления скоростью двигателя.

//Для контроля работы двигателя производится измерение частоты его вращения.
//В начале и в конце интервала измерения (T_FREQ) производится захват кода
//таймера, количества его переполнений и количества входных импульсов.
//По двум соседним захватам вычисляется период, который потом переводится в
//частоту.

//----------------------------------------------------------------------------

#include "main.hpp"
#include "dpll.hpp"
#include "control.hpp"

//----------------------------------------------------------------------------
//------------------------------ Класс TPll: ---------------------------------
//----------------------------------------------------------------------------

//----------------------------- Конструктор: ---------------------------------

TDpll::TDpll(void)
{
  Pin_Tacho.DirIn(PIN_PULL);
  Pin_Relay.DirOut();
  Pin_Fref.DirOut();

  Pwm = new TPwm();
  //timer 1 mode: CTC, TOP = OCR1A, CK/1:
  TCCR1B = (1 << WGM12) | (1 << CS10);
  //IC1 and OC1A interrupts enable:
  TIFR1 = (1 << ICF1) | (1 << OCF1A);
  TIMSK1 |= (1 << ICIE1) | (1 << OCIE1A);
  SetSpeed(NOM_V[PRE2]);

  vCapV = 0; preCapV = 0;
  vCapN = 0; preCapN = 0;
  vOvfN = 0; preOvfN = 0;
  Tacho = 0;
  vPfdUpd = 0;
  vPfdState = 0;
  Lock = 0;

  SetDir(DIRS);
  Enable = 1;
  Yp = 0;
  Xp = 0;
  Xpp = 0;

  MeterTimer = new TSoftTimer(METER_TM);
  MeterTimer->Autoreload = 1;
  MeterTimer->Start();
  LockTimer = new TSoftTimer(LOCK_TM);
  LockTimer->Start();
}

//------------------------- Прерывание по захвату: ---------------------------

uint16_t TDpll::vCapV;
uint8_t  TDpll::vCapN;
uint8_t  TDpll::vOvfN;
uint8_t  TDpll::vOvfC;
uint16_t TDpll::HPeriod;
int8_t   TDpll::vPfdState;

#pragma vector = TIMER1_CAPT_vect
__interrupt void Capture(void)
{
  //сохранение захваченного значения фазы:
  TDpll::vCapV = ICR1;
  //обновление состояния фазового детектора:
  TDpll::vPfdState++;
  //счет событий захвата таймера:
  TDpll::vCapN++;
  //сохранение количества переполнений таймера:
  TDpll::vOvfN = TDpll::vOvfC;
  //коррекция, если переполнение было до захвата таймера:
  if((HI(TDpll::vCapV) < HI(TDpll::HPeriod)) && (TIFR1 & (1 << OCF1A)))
    TDpll::vOvfN++;
}

//------------------------ Прерывание по сравнению: --------------------------

uint16_t TDpll::vPhase;
bool TDpll::vPfdUpd;

#pragma vector = TIMER1_COMPA_vect
__interrupt void Compare(void)
{
  //счет переполнений таймера:
  TDpll::Pin_Fref = (TDpll::vOvfC++)&1;
  //вычисление ошибки фазы:
  TDpll::vPfdState--;
  if(TDpll::vPfdState == 0) TDpll::vPhase = TDpll::vCapV;
    else if(TDpll::vPfdState < 0) { TDpll::vPhase = OCR1A; TDpll::vPfdState = -1; }
      else { TDpll::vPhase = 0; TDpll::vPfdState = 1; }
  TDpll::vPfdUpd = 1;
}

//------------------ Атомарное чтение захваченных значений: ------------------

__monitor void TDpll::AtomicReadCapture(void)
{
  CapV = vCapV;
  CapN = vCapN;
  OvfN = vOvfN;
}

//--------------- Атомарное чтение выхода фазового детектора: ----------------

__monitor void TDpll::AtomicReadCompare(void)
{
  Phase = vPhase;
}

//------------------------ Выполнение регулирования: -------------------------

#define MOV_AVG_SIZE 8 
#define MOV_AVG_SIZE_MASK 0x07
#define MOV_AVG_SIZE_SHIFT 3

void TDpll::Execute(void)
{
  // Переменные для скользящего усреднения
  static uint16_t Phase_Arr[MOV_AVG_SIZE] = {0};
  static uint32_t Phase_Avg = 0;
  static uint8_t Phase_Ptr = 0;

  //Измерение частоты сигнала таходатчика:
  if(MeterTimer->Over())
  {
    AtomicReadCapture(); //атомарное чтение захваченных значений
    uint8_t dOvfN = OvfN - preOvfN; //количество переполнений за интервал
    uint8_t dCapN = CapN - preCapN; //количество входных импульсов
    if(dCapN) //если входные импульсы были
    {
      //вычисление перода сигнала тахогенератора в тиках F_CLK:
      uint32_t per = (uint32_t)Period * dOvfN + CapV - preCapV;
      //вычисление частоты тахогенератора в десятых Гц:
      Tacho = (F_CLK * 10) * dCapN / per;
    }
    else //если входных импульсов не было
    {
      Tacho = 0; //нулевая частота
    }
    //сохранение значений для следующего измерения:
    preCapV = CapV;
    preCapN = CapN;
    preOvfN = OvfN;
  }
  //Реализация DPLL:
  if(vPfdUpd) //если значение фазы обновилось
  {
    vPfdUpd = 0; //сброс флага обновления
    AtomicReadCompare(); //атомарное чтение фазы
    //приведение кода фазы к диапазону 0..MAX_PFD:

    Phase = ~((uint16_t)((uint32_t)Phase * MAX_PFD / (Period - 1)));
    Phase_Avg -= Phase_Arr[Phase_Ptr];
    Phase_Avg += Phase;
    Phase_Arr[Phase_Ptr++] = Phase;
    Phase_Ptr &= MOV_AVG_SIZE_MASK;

    Phase = Phase_Avg>>MOV_AVG_SIZE_SHIFT;

    //регулировка скорости двигателя:
    if(Enable) //если разрешено автоматическое регулирование
    {
      if(Dir != DIRS)         //если не режим STOP
        Pwm->Set(Pid(Phase)); //вычисление PID и загрузка PWM
          else Pwm->Set(0);   //режим STOP, выключение двигателя
    }
    uint8_t ph = HI(Phase);
    if(ph < PH_MIN || ph > PH_MAX)
    {
      LockTimer->Start();
      Lock = 0;
    }
    else if(LockTimer->Over())
    {
      Lock = 1;
    }
  }
}

//-------------------- PID-регулятор скорости двигателя: ---------------------

uint16_t TDpll::Pid(uint16_t inp)
{
  //PID-регулятор реализован в дифференциальной форме:
  //Y(n) = Y(n-1) - Kp*[X(n)-X(n-1)] + Ki*Err(n) - Kd*[X(n)-2*X(n-1)+X(n-2)]
  int32_t Y = Yp;
  //учет пропорциональной составляющей:
  int32_t div1 = (int32_t)inp - Xp;
  Y = Y - (int32_t)K.p * div1 * 32;
  //учет интегральной составляющей:
  int32_t err = NOM_PFD - inp;
  Y = Y + (int32_t)K.i * err * HI(Period) / 256;

  if((Y>0)&&(Y<MAX_PFD*512)) // при ограничении D отключается
  {
      //учет дифференциальной составляющей:
      int32_t div2 = (int32_t)inp - 2 * (int32_t)Xp + (int32_t)Xpp;
      Y = Y - (int32_t)K.d * div2 * 32768 / HI(Period);
  }

  //ограничение выходного значения:
  if(Y < 0) Y = 0;
  if(Y > (MAX_PFD * 512)) Y = (MAX_PFD * 512);
  //обновление величин:
  Yp = Y;
  Xpp = Xp;
  Xp = inp;
  return(Y / 512);
}

//----------------------------------------------------------------------------
//------------------------- Интерфейсные методы: -----------------------------
//----------------------------------------------------------------------------

//--------------------- Установка коэффициентов PID: -------------------------

void TDpll::SetPID(kpid_t k)
{
  K = k;
}

//----------------------- Чтение коэффициентов PID: --------------------------

kpid_t TDpll::GetPID(void)
{
  return(K);
}

//------------------------- Установка скорости: ------------------------------

void TDpll::SetSpeed(uint16_t speed)
{
  if(speed > MAX_V[PRE3]) speed = MAX_V[PRE3];
  if(speed < MIN_V[PRE0]) speed = MIN_V[PRE0];
  Period = F_CLK * 10 / speed;
  Speed = speed;
  HPeriod = Period / 2;
  OCR1A = Period - 1;
}

//-------------------------- Чтение скорости: --------------------------------

uint16_t TDpll::GetSpeed(void)
{
  return(Speed);
}

//--------------------- Чтение частоты таходатчика: --------------------------

uint16_t TDpll::GetTacho(void)
{
  return(Tacho);
}

//------------------------- Чтение ошибки фазы: ------------------------------

uint16_t TDpll::GetPhase(void)
{
  return(Phase);
}

//----------------------- Задание направления вращения: ----------------------

void TDpll::SetDir(uint8_t dir)
{
  Dir = dir;
  if(dir == DIRF) {
    Pin_Relay = 0;
  } else if (dir == DIRR) {
    Pin_Relay = 1;
  }
  //Motor->SetDir(dir);
}

//------------------------- Чтение захвата частоты: --------------------------

bool TDpll::Locked(void)
{
  return(Lock);
}

//--------------------- Чтение параметров из EEPROM: -------------------------

void TDpll::EERead(void)
{
  K.p = Eeprom->Rd8(EE_KP, NOM_KP);
  K.i = Eeprom->Rd8(EE_KI, NOM_KI);
  K.d = Eeprom->Rd8(EE_KD, NOM_KD);
}

//------------------- Сохранение параметров в EEPROM: ------------------------

void TDpll::EESave(void)
{
  Eeprom->Wr8(EE_KP, K.p);
  Eeprom->Wr8(EE_KI, K.i);
  Eeprom->Wr8(EE_KD, K.d);
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
