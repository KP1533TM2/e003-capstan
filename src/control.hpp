//----------------------------------------------------------------------------

//Модуль общего управления, заголовочный файл

//----------------------------------------------------------------------------

#ifndef CONTROL_HPP
#define CONTROL_HPP

//----------------------------------------------------------------------------

#include "dpll.hpp"
#include "eeprom.hpp"

//------------------------------ Константы: ----------------------------------

enum { PRE0, PRE1, PRE2, PRE3, PRESETS }; //номера пресетов скорости
enum { DIRF, DIRR, DIRS }; //коды направления вращения
enum
{
  MRUN  = 0x01, //включение/выключение двигателя
  MDIR  = 0x02, //направление вращения
  MPRS0 = 0x04, //номер пресета, бит 0
  MPRS1 = 0x08, //номер пресета, бит 1
  MSEP  = 0x10, //режим регулировки скорости (не используется)
  MAUX  = 0x20, //дополнительный джампер (не используется)
  MLOCK = 0x80  //захват частоты
};
//коды максимальной скорости, x 0.1 Гц:
static const uint16_t MAX_V[PRESETS] = { 64000, 64000, 64000, 64000 };
//коды номинальной скорости, x 0.1 Гц:
static const uint16_t NOM_V[PRESETS] = {  2529,  5058,  8093, 10117 };
//коды минимальной скорости, x 0.1 Гц:
static const uint16_t MIN_V[PRESETS] = {  2500,  2500,  2500,  2500 };

//----------------------------------------------------------------------------
//---------------------------- Класс TControl: -------------------------------
//----------------------------------------------------------------------------

class TControl
{
  friend class TPort;
private:
  Pin_Spd1_t Pin_Spd1;
  Pin_Spd2_t Pin_Spd2;
  Pin_Sleep_t Pin_Sleep;
  Pin_Lock_t Pin_Lock;
  Pin_Led_t Pin_Led;

  //неиспользуемые пины:
  Pin_PB4_t Pin_PB4;
  Pin_PB5_t Pin_PB5;
  Pin_PC0_t Pin_PC0;
  Pin_PC1_t Pin_PC1;
  Pin_PC2_t Pin_PC2;
  Pin_PC3_t Pin_PC3;
  Pin_PC4_t Pin_PC4;
  Pin_PC5_t Pin_PC5;

  TDpll *Dpll;
  uint8_t Preset;            //номер текущего пресета скорости
  bool Run;                  //состояние (вкл./выкл.)
  uint8_t Dir;               //текущее направление вращения
  uint16_t Presets[PRESETS][DIRS]; //массив скоростей
  uint8_t PinMode;           //состояние внешних сигналов управления
  uint8_t Mode;              //копия кода режима
  void HardwareInit(void);
  bool SetSpeedPreset(uint8_t p, uint16_t s);
  void WatchdogService(void);
  void ControlService(void);
public:
  TControl(void);
  void Execute(void);
  void SetMode(uint8_t m);   //установка режима
  bool SetSpeed(uint16_t s); //установка скорости
  uint8_t GetState(void);    //чтение состояния
  void EERead(void);         //чтение параметров из EEPROM
  void EESave(void);         //сохранение параметров в EEPROM
};

//----------------------------------------------------------------------------

extern TControl *Control;

//----------------------------------------------------------------------------

#endif
