//----------------------------------------------------------------------------

//Модуль общего управления

//----------------------- Используемые ресурсы: ------------------------------

//Класс TControl осуществляет общее управление.
//Анализируется состояние внешних сигналов управления:
//SLEEP - включение и выключение двигателя,
//SPD1, SPD2 - выбор пресета скорости.
//Если состояние внешних сигналов управления не меняется,
//они не мешают управлению с компьютера.

//----------------------------------------------------------------------------

#include "main.hpp"
#include "control.hpp"

TEeprom *Eeprom;

//----------------------------------------------------------------------------
//---------------------------- Класс TControl: -------------------------------
//----------------------------------------------------------------------------

//----------------------------- Конструктор: ---------------------------------

TControl::TControl(void)
{
  HardwareInit();
  Eeprom = new TEeprom();
  Dpll = new TDpll();
  PinMode = 0;
  SetMode(0);
  EERead();
}

//------------------------ Выполнение управления: ----------------------------

void TControl::Execute(void)
{
  Dpll->Execute();
  //сервисы:
  WatchdogService();
  ControlService();
}

//----------------------------------------------------------------------------
//--------------------------- Внутренние методы: -----------------------------
//----------------------------------------------------------------------------

//----------------------- Инициализация оборудования: ------------------------

inline void TControl::HardwareInit(void)
{
  //настройка пинов:
  Pin_Spd1.DirIn(PIN_PULL);
  Pin_Spd2.DirIn(PIN_PULL);
  Pin_Sleep.DirIn(PIN_PULL);
  Pin_Lock.DirOut();
  Pin_Led.DirOut();

  //неиспользуемые пины:
  Pin_PB4.DirIn(PIN_PULL);
  Pin_PB5.DirIn(PIN_PULL);
  Pin_PC0.DirIn(PIN_PULL);
  Pin_PC1.DirIn(PIN_PULL);
  Pin_PC2.DirIn(PIN_PULL);
  Pin_PC3.DirIn(PIN_PULL);
  Pin_PC4.DirIn(PIN_PULL);
  Pin_PC5.DirIn(PIN_PULL);

  //настройка watchdog-таймера:
  __watchdog_reset();
  WDTCSR |= (1 << WDCE) | (1 << WDE); //разрешение watchdog-таймера
  WDTCSR = (1 << WDE) | (1 << WDP2) | (1 << WDP0); //интервал 500 мс
}

//-------------------- Установка пресета скорости: ---------------------------

bool TControl::SetSpeedPreset(uint8_t p, uint16_t s)
{
  if(p < PRESETS && s <= MAX_V[p] && s >= MIN_V[p])
  {
    Preset = p;
    Presets[p][Dir] = s;
    return(1);
  }
  return(0);
}

//----------------------------------------------------------------------------
//------------------------- Реализация сервисов: -----------------------------
//----------------------------------------------------------------------------

//----------------------- Сервис Watchdog-таймера: ---------------------------

inline void TControl::WatchdogService(void)
{
  if(TSysTimer::Tick)
  {
    __watchdog_reset();
  }
}

//------------------ Сервис управления внешними сигналами: -------------------

inline void TControl::ControlService(void)
{
  if(TSysTimer::Tick)
  {
    uint8_t m = 0;
    //чтение сигналов управления:
    if(Pin_Sleep) m |= MRUN;
    //if(Pin_Dir)   m |= MDIR;
    if(Pin_Spd1)  m |= MPRS0;
    if(Pin_Spd2)  m |= MPRS1;
    //если сигналы изменились, то изменение режима:
    if(PinMode != m)
    {
      PinMode = m;
      SetMode(m);
    }
    //чтение состояния захвата:
    if(Dpll->Locked())
    {
      Mode = Mode | MLOCK;
      Pin_Lock = 1;
      Pin_Led = 1;
    }
    else
    {
      Mode = Mode & ~MLOCK;
      Pin_Lock = 0;
      Pin_Led = 0;
    }
  }
}

//----------------------------------------------------------------------------
//------------------------- Интерфейсные методы: -----------------------------
//----------------------------------------------------------------------------

//--------------------------- Установка режима: ------------------------------

//m.0 = 0 - stop, = 1 - run
//m.1 = 0 - forward, = 1 - reverse
//m.2 - PRE.0
//m.3 - PRE.1

void TControl::SetMode(uint8_t m)
{
  Mode = (Mode & MLOCK) | m;
  Run = m & MRUN;
  Dir = (m & MDIR) >> 1;
  Preset = (m & (MPRS0 | MPRS1)) >> 2;
  Dpll->SetSpeed(Presets[Preset][Dir]);
  Dpll->SetDir(Run? ((Dir == DIRF)? DIRF : DIRR) : DIRS );
}

//------------------------- Установка скорости: ------------------------------

bool TControl::SetSpeed(uint16_t s)
{
  if(SetSpeedPreset(Preset, s))
  {
    Dpll->SetSpeed(Presets[Preset][Dir]);
    return(1);
  }
  return(0);
}

//-------------------------- Чтение состояния: -------------------------------

uint8_t TControl::GetState(void)
{
  return(Mode);
}

//--------------------- Чтение параметров из EEPROM: -------------------------

void TControl::EERead(void)
{
  for(uint8_t i = 0; i < PRESETS; i++)
  {
    Presets[i][DIRF] = Eeprom->Rd16(EE_V0F + i * 2, NOM_V[i]);
    Presets[i][DIRR] = Eeprom->Rd16(EE_V0R + i * 2, NOM_V[i]);
  }
  Dpll->EERead();
}

//------------------------ Save Params to EEPROM: ----------------------------

void TControl::EESave(void)
{
  for(uint8_t i = 0; i < PRESETS; i++)
  {
    Eeprom->Wr16(EE_V0F + i * 2, Presets[i][DIRF]);
    Eeprom->Wr16(EE_V0R + i * 2, Presets[i][DIRR]);
  }
  Dpll->EESave();
  Eeprom->Validate();
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------