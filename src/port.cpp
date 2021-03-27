//----------------------------------------------------------------------------

//Модуль поддержки порта

//----------------------------------------------------------------------------

//Модуль реализует набор команд, который используется для управления
//устройством с компьютера, а также для задания и считывания параметров.
//Протокол обмена - Wake, реализован в классе TWake, от которого наследован
//класс TWakePort, привязывающий протокол к аппаратному порту USART.

//----------------------------------------------------------------------------

#include "main.hpp"
#include "port.hpp"
#include "control.hpp"

//----------------------------------------------------------------------------
//------------------------------ Класс TPort ---------------------------------
//----------------------------------------------------------------------------

//----------------------------- Конструктор: ---------------------------------

TPort::TPort(void)
{
  const uint16_t br = (F_CLK / BAUD_RATE / 8 + 1) / 2;
  WakePort = new TWakePort(br, FRAME_SIZE);
}

//-------------------------- Выполнение команд: ------------------------------

void TPort::Execute(void)
{
  char Command = WakePort->GetCmd(); //чтение кода принятой команды
  if(Command != CMD_NOP)             //если есть команда, выполнение
  {
    switch(Command)
    {

    //стандартные команды:
    case CMD_ERR: //обработка ошибки
      {
        WakePort->AddByte(ERR_TX);
        break;
      }
    case CMD_ECHO: //эхо
      {
        char cnt = WakePort->GetRxCount();
        for(char i = 0; i < cnt; i++)
          WakePort->AddByte(WakePort->GetByte());
        break;
      }
    case CMD_INFO: //чтение иформации об устройстве
      {
        uint8_t i = 0;
        do WakePort->AddByte(DevName[i]);
          while(DevName[++i]);
        break;
      }

    //специальные команды:
    case CMD_SET_MODE: //установка режима
      {
        Control->SetMode(WakePort->GetByte());
        WakePort->AddByte(ERR_NO);
        break;
      }
    case CMD_SET_SPEED: //установка скорости
      {
        if(Control->SetSpeed(WakePort->GetWord()))
          WakePort->AddByte(ERR_NO);
            else WakePort->AddByte(ERR_PA);
        break;
      }
    case CMD_SET_PWM: //непосредственное управление PWM
      {
        Control->Dpll->Pwm->Set(WakePort->GetWord());
        WakePort->AddByte(ERR_NO);
        break;
      }
    case CMD_GET_STATE: //чтение состояния
      {
        WakePort->AddByte(ERR_NO);
        WakePort->AddByte(Control->GetState());
        WakePort->AddWord(Control->Dpll->GetTacho());
        WakePort->AddWord(Control->Dpll->GetSpeed());
        WakePort->AddWord(Control->Dpll->GetPhase());
        WakePort->AddWord(Control->Dpll->Pwm->Get());
        break;
      }
    case CMD_SET_PARAMS: //установка параметров
      {
        Control->Dpll->Enable = WakePort->GetByte();
        kpid_t K;
        K.p = WakePort->GetByte();
        K.i = WakePort->GetByte();
        K.d = WakePort->GetByte();
        Control->Dpll->SetPID(K);
        WakePort->AddByte(ERR_NO);
        break;
      }
    case CMD_GET_PARAMS: //чтение параметров
      {
        WakePort->AddByte(ERR_NO);
        WakePort->AddByte(Control->Dpll->Enable);
        kpid_t k = Control->Dpll->GetPID();
        WakePort->AddByte(k.p);
        WakePort->AddByte(k.i);
        WakePort->AddByte(k.d);
        break;
      }
    case CMD_EE_SAVE: //сохранение параметров в EEPROM
      {
        Control->EESave();
        WakePort->AddByte(ERR_NO);
        break;
      }
    case CMD_DEFAULT: //загрузка параметров по умолчанию
      {
        Eeprom->Invalidate();
        Control->EERead();
        WakePort->AddByte(ERR_NO);
        break;
      }
    //неизвестная команда:
    default:
      {
        WakePort->AddByte(ERR_PA);
      }
    }
    WakePort->StartTx(Command);
  }
}

//----------------------------------------------------------------------------
