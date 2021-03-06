//----------------------------------------------------------------------------

//Модуль формирования PWM-сигнала, заголовочный файл

//----------------------------------------------------------------------------

#ifndef PWM_HPP
#define PWM_HPP

//----------------------------------------------------------------------------
//------------------------------ Класс TPwm: ---------------------------------
//----------------------------------------------------------------------------

#pragma vector = TIMER2_OVF_vect
extern "C" __interrupt void SigmaDelta(void);

class TPwm
{
private:
  friend __interrupt void SigmaDelta(void);
  static Pin_Pwm_t Pin_Pwm;
  static Pin_Zcd_t Pin_Zcd;
  static Pin_Debug_t Pin_Debug;
  static uint16_t const PWM_MAX  = 0xFF00;
  static uint16_t const PWM_MASK = 0xFFFC; //limit to 14 bit
  static uint8_t Pwm_Lo;
  static uint8_t Pwm_iHi;
  static uint8_t Pwm_iHi_plus_1;
  static uint16_t Raw_V;
public:
  TPwm(void);
  void Set(uint16_t v); //задание PWM
  uint16_t Get(void);   //чтение PWM
};

//----------------------------------------------------------------------------

#endif
