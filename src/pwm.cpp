//----------------------------------------------------------------------------

//Модуль формирования PWM-сигнала

//----------------------- Используемые ресурсы: ------------------------------

//Модуль используется для получения управляющего напряжения для схемы
//питания двигателя. Поскольку единственный 16-разрядный таймер занят,
//а разрешения 8-разрядного PWM не хватает, используется комбинация
//аппаратного 8-разрядного PWM (канал OCR2A) и программного 8-разрядного
//Delta-Sigma модулятора. В результате получается сигнал, имеющий разрешение
//16 бит (по постоянной составляющей), основная энергия спектра которого лежит
//на частоте аппаратного PWM (62.5 кГц) и выше, что упрощает фильтрацию.
//Недостатком является то, что программный Delta-Sigma модулятор заметно
//загружает процессор (примерно на 15%).

//----------------------------------------------------------------------------

#include "main.hpp"
#include "pwm.hpp"

const uint8_t sine_table[256] = {
0x00, 0x01, 0x03, 0x04, 0x06, 0x07, 0x09, 0x0a, 
0x0c, 0x0e, 0x0f, 0x11, 0x12, 0x14, 0x15, 0x17, 
0x18, 0x1a, 0x1b, 0x1d, 0x1f, 0x20, 0x22, 0x23, 
0x25, 0x26, 0x28, 0x29, 0x2b, 0x2c, 0x2e, 0x30, 
0x31, 0x33, 0x34, 0x36, 0x37, 0x39, 0x3a, 0x3c, 
0x3d, 0x3f, 0x40, 0x42, 0x43, 0x45, 0x46, 0x48, 
0x49, 0x4b, 0x4c, 0x4e, 0x4f, 0x51, 0x52, 0x54, 
0x55, 0x57, 0x58, 0x59, 0x5b, 0x5c, 0x5e, 0x5f, 
0x61, 0x62, 0x64, 0x65, 0x66, 0x68, 0x69, 0x6b, 
0x6c, 0x6e, 0x6f, 0x70, 0x72, 0x73, 0x74, 0x76, 
0x77, 0x79, 0x7a, 0x7b, 0x7d, 0x7e, 0x7f, 0x81, 
0x82, 0x83, 0x85, 0x86, 0x87, 0x89, 0x8a, 0x8b, 
0x8d, 0x8e, 0x8f, 0x90, 0x92, 0x93, 0x94, 0x96, 
0x97, 0x98, 0x99, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 
0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa7, 0xa8, 0xa9, 
0xaa, 0xab, 0xac, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 
0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb9, 0xba, 0xbb, 
0xbc, 0xbd, 0xbe, 0xbf, 0xc0, 0xc1, 0xc2, 0xc3, 
0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 
0xcc, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 
0xd3, 0xd4, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 
0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xdd, 0xde, 0xdf, 
0xe0, 0xe0, 0xe1, 0xe2, 0xe2, 0xe3, 0xe4, 0xe4, 
0xe5, 0xe6, 0xe6, 0xe7, 0xe8, 0xe8, 0xe9, 0xea, 
0xea, 0xeb, 0xeb, 0xec, 0xec, 0xed, 0xee, 0xee, 
0xef, 0xef, 0xf0, 0xf0, 0xf1, 0xf1, 0xf2, 0xf2, 
0xf3, 0xf3, 0xf3, 0xf4, 0xf4, 0xf5, 0xf5, 0xf6, 
0xf6, 0xf6, 0xf7, 0xf7, 0xf7, 0xf8, 0xf8, 0xf8, 
0xf9, 0xf9, 0xf9, 0xf9, 0xfa, 0xfa, 0xfa, 0xfb, 
0xfb, 0xfb, 0xfb, 0xfb, 0xfc, 0xfc, 0xfc, 0xfc, 
0xfc, 0xfc, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 
0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd
};


//----------------------------------------------------------------------------
//------------------------------ Класс TPwm: ---------------------------------
//----------------------------------------------------------------------------

//----------------------------- Конструктор: ---------------------------------

TPwm::TPwm(void)
{
  Pin_Pwm.DirOut(PIN_LOW);
  Pin_Zcd.DirIn();
  Pin_Debug.DirOut(PIN_LOW);
  Set(0);
  OCR2B = Pwm_iHi;
  //fast PWM, clear OC2A on compare match:
  TCCR2A = (1 << COM2B1) | (1 << COM2B0) | (1 << WGM21) | (1 << WGM20);
  TCCR2B = (1 << CS20);   //CK/1
  TIFR2 = (1 << TOV2);    //pending interrupts clear
  TIMSK2 |= (1 << TOIE2); //overflow interrupt enable
}

//--------------------------- Прерывание таймера: ----------------------------

uint8_t TPwm::Pwm_Lo;
uint8_t TPwm::Pwm_iHi;
uint8_t TPwm::Pwm_iHi_plus_1;
uint16_t TPwm::Raw_V;

#pragma vector = TIMER2_OVF_vect
__interrupt void SigmaDelta(void)
{
  TPwm::Pin_Debug = 1;
  const uint32_t Phase_Inc = 6871948; // approx. 100Hz test
  const uint32_t Phase_Comp =  84618830; // approx. 100Hz test
  static uint32_t Mod_Zcd_Low = 0;
  static uint32_t Mod_Phase_Offset = 0;
//  static uint32_t Mod_ZCD_
  static uint32_t Mod_Phase = 0;
  static uint8_t Prev_Zcd = 0;
  static uint8_t Zcd = 0;
  static uint16_t Raw_V_buf = 0;
//  Mod_Phase += 6871948; // ~100Hz test
  
  Prev_Zcd = Zcd;
  Zcd = TPwm::Pin_Zcd?0xff:0x00;
  
  if(!Zcd) {
      // если уровень с детектора перехода низкий, накапливаем фазу
      // в отдельный аккумулятор, начиная со спада
      Mod_Zcd_Low += Phase_Inc;
      Mod_Phase += Phase_Inc;
  } else {
      // если уровень поднялся, то получаем сдвиг фазы напополам,
      // с которого начинаем счет ЦВСа
      if(!Prev_Zcd) {
          Mod_Phase = Mod_Zcd_Low>>1;
          Mod_Phase += Phase_Comp;
          Mod_Zcd_Low = 0;
 //         Raw_V_buf = TPwm::Raw_V;
      } else {
          Mod_Phase += Phase_Inc;
      }
  }
  
  Raw_V_buf = TPwm::Raw_V;
  
//  if(Pin_Zcd = 0
  
  uint8_t index = (Mod_Phase>>23)&0xFF; // take 8 bits before the last one
//  uint8_t sine = 
//  OCR2B = index;
//  OCR2B=TPwm::Pwm_iHi;

  uint16_t output = (Raw_V_buf>>8)*sine_table[index^(((Mod_Phase>>31)&1)?0xFF:0x00)];
  OCR2B=~(output>>8);

//  static uint8_t Sigma = 0;
//  Sigma = Sigma + TPwm::Pwm_Lo;
//  OCR2B = (SREG & 1)? TPwm::Pwm_iHi_plus_1 : TPwm::Pwm_iHi;
  TPwm::Pin_Debug = 0;
}

//------------------------------ Задание PWM: --------------------------------

void TPwm::Set(uint16_t v)
{
  if(v > PWM_MAX) v = PWM_MAX;
  Raw_V = v;
  Pwm_Lo = LO(v) & LO(PWM_MASK);
  Pwm_iHi = ~HI(v);
  Pwm_iHi_plus_1 = ~(HI(v) + 1);
}

//------------------------------- Чтение PWM: --------------------------------

uint16_t TPwm::Get(void)
{
  return(WORD(~Pwm_iHi, Pwm_Lo));
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
