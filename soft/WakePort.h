#ifndef WakePortH
#define WakePortH

#include "WakeCom.h"
#include "commands.hpp"
#include <stdint.h>

#define DEVICE_NAME "Capstan" //��� ����������
#define BAUD_RATE 19200 //�������� ������, ���

//-------------------------------- State: ------------------------------------

enum
{
  S_RUN  = 0x01, //���������/���������� ���������
  S_REV  = 0x02, //����������� ��������
  S_PRS0 = 0x04, //����� �������, ��� 0
  S_PRS1 = 0x08, //����� �������, ��� 1
  S_SEP  = 0x10, //����� ����������� �������� (separate/joint)
  S_AUX  = 0x20, //�������������� �������
  S_LOCK = 0x80  //������ �������
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

//------------------ ����� ����������������� ����� WAKE: ---------------------

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
