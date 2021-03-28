//----------------------------------------------------------------------------

//Модуль порта с протоколом Wake, заголовочный файл

//----------------------------------------------------------------------------

#ifndef WAKEPORT_HPP
#define WAKEPORT_HPP

#include "wake.hpp"

//----------------------------------------------------------------------------
//--------------------------- Класс TWakePort --------------------------------
//----------------------------------------------------------------------------

#pragma vector = USART0_RX_vect
extern "C" __interrupt void Rx_Int(void); //прерывание после приема байта
#pragma vector = USART0_TX_vect
extern "C" __interrupt void Tx_Int(void); //прерывание после передачи байта

class TWakePort : public TWake
{
private:
  Pin_RXD_t Pin_RXD;
  Pin_TXD_t Pin_TXD;
  static TWakePort *Wp;
  friend __interrupt void Rx_Int(void);
  friend __interrupt void Tx_Int(void);
protected:
public:
  TWakePort(const uint16_t br, const uint8_t frame);
  void StartTx(uint8_t cmd);
};

//----------------------------------------------------------------------------

#endif
