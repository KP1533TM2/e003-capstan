#pragma hdrstop

#include "WakePort.h"

#pragma package(smart_init)

//--------------------------- Конструктор: ----------------------------------

__fastcall TWakePort::TWakePort()
{
  SetBaudRate(BAUD_RATE);
}

//---------------------------- Деструктор: ----------------------------------

__fastcall TWakePort::~TWakePort()
{
//
}

//---------------------------------------------------------------------------
//------------------------- Специальные команды: ----------------------------
//---------------------------------------------------------------------------

//---------------------------- CMD_SET_MODE: --------------------------------

void __fastcall TWakePort::SetMode(int mode)
{
  _ENTER_COMMAND(CmdCrSec)
  NewCmd(CMD_SET_MODE, "SetMode");
  AddByte(mode);
  SendCmd();
  _LEAVE_COMMAND(CmdCrSec)
}

//---------------------------- CMD_SET_SPEED: -------------------------------

void __fastcall TWakePort::SetSpeed(int speed)
{
  _ENTER_COMMAND(CmdCrSec)
  NewCmd(CMD_SET_SPEED, "SetSpeed");
  AddWord(speed);
  SendCmd();
  _LEAVE_COMMAND(CmdCrSec)
}

//----------------------------- CMD_SET_PWM: --------------------------------

void __fastcall TWakePort::SetPWM(int pwm)
{
  _ENTER_COMMAND(CmdCrSec)
  NewCmd(CMD_SET_PWM, "SetPWM");
  AddWord(pwm);
  SendCmd();
  _LEAVE_COMMAND(CmdCrSec)
}

//--------------------------- CMD_GET_STATE: --------------------------------

void __fastcall TWakePort::GetState(state_t &s)
{
  _ENTER_COMMAND(CmdCrSec)
  NewCmd(CMD_GET_STATE, "GetState");
  SendCmd();
  s.s = GetByte();
  s.v = GetWord();
  s.r = GetWord();
  s.e = GetWord();
  s.p = GetWord();
  _LEAVE_COMMAND(CmdCrSec)
}

//--------------------------- CMD_SET_PARAMS: -------------------------------

void __fastcall TWakePort::SetParams(params_t &p)
{
  _ENTER_COMMAND(CmdCrSec)
  NewCmd(CMD_SET_PARAMS, "SetParams");
  AddByte(p.en);
  AddByte(p.kp);
  AddByte(p.ki);
  AddByte(p.kd);
  SendCmd();
  _LEAVE_COMMAND(CmdCrSec)
}

//--------------------------- CMD_GET_PARAMS: -------------------------------

void __fastcall TWakePort::GetParams(params_t &p)
{
  _ENTER_COMMAND(CmdCrSec)
  NewCmd(CMD_GET_PARAMS, "GetParams");
  SendCmd();
  p.en = GetByte();
  p.kp = GetByte();
  p.ki = GetByte();
  p.kd = GetByte();
  _LEAVE_COMMAND(CmdCrSec)
}

//------------------------------ CMD_EE_SAVE: -------------------------------

void __fastcall TWakePort::EESave(void)
{
  _ENTER_COMMAND(CmdCrSec)
  NewCmd(CMD_EE_SAVE, "EESave");
  SetTo(LONG_RX_TIMEOUT, DEFAULT_TX_TIMEOUT);
  SendCmd();
  SetTo(DEFAULT_RX_TIMEOUT, DEFAULT_TX_TIMEOUT);
  _LEAVE_COMMAND(CmdCrSec)
}

//------------------------------ CMD_DEFAULT: -------------------------------

void __fastcall TWakePort::Default(void)
{
  _ENTER_COMMAND(CmdCrSec)
  NewCmd(CMD_DEFAULT, "Default");
  SendCmd();
  _LEAVE_COMMAND(CmdCrSec)
}

//---------------------------------------------------------------------------

