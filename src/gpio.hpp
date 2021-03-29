//----------------------------------------------------------------------------

//Реализация шаблонного класса TGpio

//----------------------------------------------------------------------------

#ifndef GPIO_HPP
#define	GPIO_HPP

//----------------------------- Классы портов: -------------------------------

#define DEFINE_PORT_CLASS(port_reg, ddr_reg, pin_reg, port_class_name) \
  class port_class_name\
    { public:\
	static uint8_t port()       { return(port_reg); }\
	static void port(uint8_t v) { port_reg = v; }\
	static uint8_t dir()        { return(ddr_reg); }\
	static void dir(uint8_t v)  { ddr_reg = v; }\
	static uint8_t pin()        { return(pin_reg); }\
	static void pin(uint8_t v)  { pin_reg = v; }\
    };

#ifdef PORTA0
DEFINE_PORT_CLASS(PORTA, DDRA, PINA, PORT_A)
#endif
#ifdef PORTB0
DEFINE_PORT_CLASS(PORTB, DDRB, PINB, PORT_B)
#endif
#ifdef PORTC0
DEFINE_PORT_CLASS(PORTC, DDRC, PINC, PORT_C)
#endif
#ifdef PORTD0
DEFINE_PORT_CLASS(PORTD, DDRD, PIND, PORT_D)
#endif
#ifdef PORTE0
DEFINE_PORT_CLASS(PORTE, DDRE, PINE, PORT_E)
#endif
#ifdef PORTF0
DEFINE_PORT_CLASS(PORTF, DDRF, PINF, PORT_F)
#endif
#ifdef PORTG0
DEFINE_PORT_CLASS(PORTG, DDRG, PING, PORT_G)
#endif
#ifdef PORTH0
DEFINE_PORT_CLASS(PORTH, DDRH, PINH, PORT_H)
#endif
#ifdef PORTL0
DEFINE_PORT_CLASS(PORTL, DDRL, PINL, PORT_L)
#endif

//----------------------------- Номера пинов: --------------------------------

enum PinNumber_t
{
  PIN_0,
  PIN_1,
  PIN_2,
  PIN_3,
  PIN_4,
  PIN_5,
  PIN_6,
  PIN_7
};

//------------------ Подтяжки/начальные состояния пинов: ---------------------

enum PinMode_t
{
  PIN_FLOAT = 0, //подтяжка выключена
  PIN_PULL  = 1, //подтяжка включена
  PIN_LOW   = 0, //начальное состояние LOW
  PIN_HIGH  = 1  //начальное состояние HIGH
};

//----------------------------------------------------------------------------
//------------------------ Шаблонный класс TGpio: ----------------------------
//----------------------------------------------------------------------------

template<class Port, PinNumber_t Pin>
class TGpio
{
public:
  TGpio() {};
  static void DirOut();
  static void DirOut(PinMode_t Mode);
  static void DirIn();
  static void DirIn(PinMode_t Mode);
  static void Pullup(PinMode_t Mode);
  static void Set();
  static void Clr();
  static bool Get();
  static bool GetLatch();
  void operator = (bool Value);
  operator bool();
};

//------------------------- Реализация методов: ------------------------------

template<class Port, PinNumber_t Pin>
inline void TGpio<Port, Pin>::DirOut()
{
  Port::dir(Port::dir() | (1 << Pin));
}

template<class Port, PinNumber_t Pin>
inline void TGpio<Port, Pin>::DirOut(PinMode_t Mode)
{
  (Mode)? Set() : Clr();
  Port::dir(Port::dir() | (1 << Pin));
}

template<class Port, PinNumber_t Pin>
inline void TGpio<Port, Pin>::DirIn()
{
  Port::dir(Port::dir() & ~(1 << Pin));
}

template<class Port, PinNumber_t Pin>
inline void TGpio<Port, Pin>::DirIn(PinMode_t Mode)
{
  (Mode)? Set() : Clr();
  Port::dir(Port::dir() & ~(1 << Pin));
}

template<class Port, PinNumber_t Pin>
inline void TGpio<Port, Pin>::Pullup(PinMode_t Mode)
{
  (Mode)? Set() : Clr();
}

template<class Port, PinNumber_t Pin>
inline void TGpio<Port, Pin>::Set()
{
  Port::port(Port::port() | (1 << Pin));
}

template<class Port, PinNumber_t Pin>
inline void TGpio<Port, Pin>::Clr()
{
  Port::port(Port::port() & ~(1 << Pin));
}

template<class Port, PinNumber_t Pin>
inline bool TGpio<Port, Pin>::Get()
{
  return(Port::pin() & (1 << Pin));
}

template<class Port, PinNumber_t Pin>
inline bool TGpio<Port, Pin>::GetLatch()
{
  return(Port::port() & (1 << Pin));
}

template<class Port, PinNumber_t Pin>
inline void TGpio<Port, Pin>::operator = (bool Value)
{
  Value? Set() : Clr();
}

template<class Port, PinNumber_t Pin>
inline TGpio<Port, Pin>::operator bool()
{
  return(Get());
}

//----------------------------------------------------------------------------

#endif
