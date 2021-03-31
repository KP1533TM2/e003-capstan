#ifndef WakePortH
#define WakePortH

#include "WakeCom.h"
#include "commands.hpp"
#include <stdint.h>

#define DEVICE_NAME "Capstan" //имя устройства
#define BAUD_RATE 19200 //скорость обмена, бод

//-------------------------------- State: ------------------------------------

enum
{
  S_RUN  = 0x01, //включение/выключение двигателя
  S_REV  = 0x02, //направление вращения
  S_PRS0 = 0x04, //номер пресета, бит 0
  S_PRS1 = 0x08, //номер пресета, бит 1
  S_SEP  = 0x10, //режим регулировки скорости (separate/joint)
  S_AUX  = 0x20, //дополнительный джампер
  S_LOCK = 0x80  //захват частоты
};

typedef struct
{
  int s;
  int v;
  int r;
  int e;
  int p;
} state_t;

typedef struct
{
  bool en;
  int kp;
  int ki;
  int kd;
} params_t;

//------------------ Класс последовательного порта WAKE: ---------------------

class TWakePort : public TWakeCom
{
private:
protected:

public:
  __fastcall TWakePort();
  __fastcall ~TWakePort();
  void __fastcall SetMode(int mode);
  void __fastcall SetSpeed(int speed);
  void __fastcall SetPWM(int pwm);
  void __fastcall GetState(state_t &s);
  void __fastcall SetParams(params_t &p);
  void __fastcall GetParams(params_t &p);
  void __fastcall EESave(void);
  void __fastcall Default(void);
};

//---------------------------------------------------------------------------

#endif
