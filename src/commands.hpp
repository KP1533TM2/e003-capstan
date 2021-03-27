//----------------------------------------------------------------------------

#ifndef COMMANDS_HPP
#define COMMANDS_HPP

//----------------------------------------------------------------------------
//------------------------ Коды специальных команд: --------------------------
//----------------------------------------------------------------------------

enum WakeScmd_t
{
  CMD_DUMMY = CMD_GETADDR,

//------------------------- Управление двигателем: ---------------------------

  CMD_SET_MODE,
  //TX: word M RX:
  //M - mode
  //M.0 = 0 - stop, = 1 - run
  //M.1 = 0 - forward, = 1 - reverse
  //M.2 - PRE0: 0 - 4, 1 - 9, 0 - 19, 1 - 38
  //M.3 - PRE1: 0      0      1       1
  //M.4 = 0 - joint mode, = 1 - separate mode

//--------------------------- Установка скорости: ----------------------------

  CMD_SET_SPEED,
  //TX: word V RX:
  //V = 2500..64000 - speed, x 0.1 Hz

//-------------------- Непосредственное управление PWM: ----------------------

  CMD_SET_PWM,
  //TX: word P RX:
  //P = 0..65535 - PWM code

//-------------------------- Чтение состояния: -------------------------------

  CMD_GET_STATE,
  //TX: RX: byte S, word V, word R, word E, word P
  //S - current motor state
  //S.0 = 0 - stop, = 1 - run
  //S.1 = 0 - forward, = 1 - reverse
  //S.2 - SPD1: 0 - 4, 1 - 9, 0 - 19, 1 - 38
  //S.3 - SPD2: 0      0      1       1
  //S.4 = 0 - joint mode, = 1 - separate mode
  //S.7 = 1 - DPLL locked
  //V = 0..65535 - measured speed, x 0.1 Hz
  //R = 2500..64000 - reference speed, x 0.1 Hz
  //E = ±32767 - current phase error, x 62.5 ns
  //P = 0..65535 - current PWM code

//------------------------- Установка параметров: ----------------------------

  CMD_SET_PARAMS,
  //TX: byte En, byte Kp, byte Ki, byte Kd,
  //En = 0 - DPLL disabled, = 1 - DPLL enabled
  //Kp = 0..255 - пропорциональный коэффициент PID
  //Ki = 0..255 - интегральный коэффициент PID
  //Kd = 0..255 - дифференциальный коэффициент PID

//-------------------------- Чтение параметров: ------------------------------

  CMD_GET_PARAMS,
  //TX: RX: byte En, byte Kp, byte Ki, byte Kd,
  //En = 0 - DPLL disabled, = 1 - DPLL enabled
  //Kp = 0..255 - пропорциональный коэффициент PID
  //Ki = 0..255 - интегральный коэффициент PID
  //Kd = 0..255 - дифференциальный коэффициент PID

//-------------------- Сохранение параметров в EEPROM: -----------------------

  CMD_EE_SAVE,
  //TX: RX:

//------------------- Загрузка параметров по умолчанию: ----------------------

  CMD_DEFAULT,
  //TX: RX:

//----------------------------------------------------------------------------

  CMD_LAST_COMMAND
};

//----------------------------------------------------------------------------

#endif