//----------------------------------------------------------------------------

//Карта EEPROM

//----------------------------------------------------------------------------

#ifndef EEMAP_HPP
#define EEMAP_HPP

//----------------------------- Карта EEPROM: --------------------------------

enum EE_Map_t
{
  EE_SIGN, EE_SIGN_H,

  EE_KP,
  EE_KI,
  EE_KD,

  EE_V0F, EE_V0F_H,
  EE_V1F, EE_V1F_H,
  EE_V2F, EE_V2F_H,
  EE_V3F, EE_V3F_H,

  EE_V0R, EE_V0R_H,
  EE_V1R, EE_V1R_H,
  EE_V2R, EE_V2R_H,
  EE_V3R, EE_V3R_H,

  EE_PARAMS //EEPROM size
};

//----------------------------------------------------------------------------

#endif
