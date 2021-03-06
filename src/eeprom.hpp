//----------------------------------------------------------------------------

//Модуль поддержки EEPROM, заголовочный файл

//----------------------------------------------------------------------------

#ifndef EEPROM_HPP
#define EEPROM_HPP

//----------------------------------------------------------------------------

#include "eemap.hpp"

//----------------------------------------------------------------------------
//----------------------------- Класс TEeprom: -------------------------------
//----------------------------------------------------------------------------

class TEeprom
{
private:
  static __no_init __eeprom uint8_t EEData[EE_PARAMS];
  static uint16_t const EE_SIGNATURE = 0xBEDA;   //сигнатура EEPROM
  bool fValid;
protected:
public:
  TEeprom(void);
  uint8_t Rd8(uint8_t addr, uint8_t def = 0);    //чтение 8 бит
  void Wr8(uint8_t addr, uint8_t val);           //запись 8 бит
  uint16_t Rd16(uint8_t addr, uint16_t def = 0); //чтение 16 бит
  void Wr16(uint8_t addr, uint16_t val);         //запись 16 бит
  void Validate(void);                           //запись сигнатуры
  void Invalidate(void);                         //сброс флага валидности
};

//----------------------------------------------------------------------------

extern TEeprom *Eeprom;

//----------------------------------------------------------------------------

#endif
