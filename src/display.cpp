
//----------------------------------------------------------------------------

//Модуль поддержки LED-дисплея

//----------------------- Используемые ресурсы: ------------------------------

//Используется дисплей на 7-сегментных LED-индикаторах.
//Индикация - статическая, индикаторы подключены
//к выходам внешнего сдвигового регистра (цепочка из 6-ти 74HC595).
//Загрузка регистра осуществляется с помощью аппаратного порта SPI.
//Для доступа к регистру реализован класс TSreg.
//Задействованы порты: SCLK - PB5 (SPI SCK), SDATA - PB3 (SPI MOSI),
//LOAD - PC0, OE - PC1.

//Реализован вывод времени в формате "-9.59.59", метража в формате
//"-9999.9", скорости в формате "999.999" и диаметра ролика в формате
//"d 99.99". Диаметр представлен с точностю до тысячной мм, но при выводе
//тысячные доли мм не отображаются. Кроме того, в режиме отображения диаметра
//поддерживается вывод текстовых сообщений "d CAL" и "d Err", которые
//используются в режиме калибровки. Любой разряд на дисплее можно включить
//в режим мигания.

//----------------------------------------------------------------------------

#include "main.hpp"
#include "display.hpp"
#include "count.hpp"

//----------------------------- Константы: -----------------------------------

//расположение сегментов на индикаторе:
//
//    -- A --
//   |       |
//   F       B
//   |       |
//    -- G --
//   |       |
//   E       C
//   |       |
//    -- D --  (H)

//сегменты индикаторов в той последовательности, как
//они подключены к выходам регистров на печатной плате:
enum Segs_t
{ SEG_A, SEG_B, SEG_C, SEG_H, SEG_D, SEG_E, SEG_F, SEG_G };

//маски сегментов:
enum SegMask_t
{
  A = (1 << SEG_A),
  B = (1 << SEG_B),
  C = (1 << SEG_C),
  D = (1 << SEG_D),
  E = (1 << SEG_E),
  F = (1 << SEG_F),
  G = (1 << SEG_G),
  H = (1 << SEG_H)
};

//----------------------------------------------------------------------------
//----------------------------- Класс TSreg: ---------------------------------
//----------------------------------------------------------------------------

//---------------------------- Инициализация: --------------------------------

void TSreg::Init(bool on)
{
  Pin_SS.DirOut(PIN_LOW);
  if(on)
  {
    Pin_OE.DirOut(PIN_HIGH);
    Pin_LOAD.DirOut(PIN_HIGH);
    Pin_SDATA.DirOut(PIN_LOW);
    Pin_SCLK.DirOut(PIN_LOW);
    //настройка SPI:
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << CPOL);
  }
  else
  {
    //выключение SPI:
    SPCR = 0;
    Pin_OE.DirIn(PIN_FLOAT);
    Pin_LOAD.DirIn(PIN_FLOAT);
    Pin_SDATA.DirIn(PIN_FLOAT);
    Pin_SCLK.DirIn(PIN_FLOAT);
  }
}

//----------------------- Запись байта через SPI: ----------------------------

void TSreg::SpiWr(uint8_t d)
{
  SPDR = ~d;
  while(!(SPSR & (1 << SPIF)));
}

//-------------------------- Загрузка регистров: -----------------------------

void TSreg::Load(uint8_t *Value)
{
  Pin_LOAD = 0;
  for(uint8_t n = 0; n < REGS; n++)
    SpiWr(Value[n]);
  Pin_LOAD = 1;
  Pin_OE = 0;
}

//----------------------------------------------------------------------------
//---------------------------- Класс TDisplay --------------------------------
//----------------------------------------------------------------------------

//------------ Преобразование кода символа в 7-сегментный код: ---------------

uint8_t TDisplay::Seven(uint8_t d)
{
  //таблица знакогенератора:
  //TODO: для некоторых цифр есть альтернативные
  //формы написания на 7-сегментном индикаторе.
  //Это цифры 6, 7, 9. Применять ли эти формы?
  //На данный момент применяется только для цифры 7.
  static const uint8_t Font[] =
  {                            //code - symbol
    A | B | C | D | E | F    , //  0  -  "0"
    B | C                    , //  1  -  "1"
    A | B | D | E | G        , //  2  -  "2"
    A | B | C | D | G        , //  3  -  "3"
    B | C | F | G            , //  4  -  "4"
    A | C | D | F | G        , //  5  -  "5"
    A | C | D | E | F | G    , //  6  -  "6"
//    C | D | E | F | G        , //  6  -  "6 alt"
//    A | B | C                , //  7  -  "7"
    A | B | C | F            , //  7  -  "7 alt"
    A | B | C | D | E | F | G, //  8  -  "8"
    A | B | C | D | F | G    , //  9  -  "9"
//    A | B | C | F | G        , //  9  -  "9 alt"
    A | B | C | E | F | G    , // 10  -  "A"
    A | D | E | F            , // 11  -  "С"
    B | C | D | E | G        , // 12  -  "d"
    A | D | E | F | G        , // 13  -  "E"
    D | E | F                , // 14  -  "L"
    E | G                    , // 15  -  "r"
    G                        , // 16  -  "-"
    0                          // 17  -  " "
  };

  //запоминание и удаление точки:
  bool p = d & POINT;
  d &= ~POINT;
  //перекодировка букв и специальных символов:
  if(d > 10)
  {
    switch(d)
    {
    case 'A': d = 10; break; // "A"
    case 'C': d = 11; break; // "C"
    case 'd': d = 12; break; // "d"
    case 'E': d = 13; break; // "E"
    case 'L': d = 14; break; // "L"
    case 'r': d = 15; break; // "r"
    case '-': d = 16; break; // "-"
    case ' ': d = 17; break; // " "
    default:  d = 16;        // "-"
    }
  }
  //7-сегментное кодирование:
  char c = Font[d];
  //восстановление точки:
  if(p) c |= H;
  return(c);
}

//---------------------- Вывод цифры на индикатор: ---------------------------

void TDisplay::Digit(uint8_t d)
{
  if(d == HOME) Pos = 0;
    else if(d == '.') Segs[Pos - 1] |= H;
      else Segs[Pos++] = Seven(d);
}

//----------------- Заполнение индикатора заданным символом: -----------------

void TDisplay::Digits(uint8_t d)
{
  Digit(HOME);
  for(uint8_t i = 0; i < DIGS; i++)
    Digit(d);
}

//-------------------------- Вывод строки из Flash: --------------------------

void TDisplay::String(const uint8_t *s)
{
  while(*s) Digit(*s++);
}

//-------------------- Преобразование числа в BCD-формат: --------------------

//Преобразование двоичного числа в двоично-десятичное:
//v - входное двоичное число (32 бита, без знака)
//buff - выходной массив (6 цифр)

void TDisplay::Bcd(uint32_t v, uint8_t *buff)
{
  for(uint8_t p = 0; p < DIGS; p++)
    buff[p] = 0;                      //очистка выходного буфера
  for(uint8_t i = 0; i < 32; i++)     //цикл по количеству входных бит
  {
    uint8_t c = (v >> 31) & 1;        //сохранение переноса
    v = v << 1;                       //сдвиг входного числа
    for(uint8_t p = 0; p < DIGS; p++) //цикл по количеству цифр
    {
      uint8_t s = buff[p];            //чтение цифры
      s = (s << 1) | c; c = 0;        //сдвиг с учетом переноса
      if(s > 9) { s += 0x06; c = 1; } //коррекция цифры
      s &= 0x0F;                      //выделение ниббла
      buff[p] = s;                    //сохранение цифры
    }
  }
}

//----------------------------------------------------------------------------
//---------------------------- Public методы: --------------------------------
//----------------------------------------------------------------------------

//----------------------------- Конструктор: ---------------------------------

TDisplay::TDisplay(void)
{
  BlinkTimer = new TSoftTimer();
  BlinkEn = 0;
  BlinkOn = 0;
  Sreg.Init(1);
  Clear();
}

//-------------------------- Выполнение мигания: -----------------------------

void TDisplay::Execute(void)
{
  if(BlinkEn && TSysTimer::Tick)
  {
    if(BlinkTimer->Over())
    {
      BlinkTimer->Start(BLINK_TM / 2);
      BlinkOn = !BlinkOn;
      Sreg.Load(BlinkOn? Segs : Darks);
    }
  }
}

//-------------------------- Включение мигания: ------------------------------

void TDisplay::Blink(uint8_t p)
{
  if(p)
  {
    for(int8_t i = 0; i < DIGS; i++)
      Darks[i] = (p & (1 << i))? 0 : Segs[i];
  }
  BlinkEn = p;
  BlinkOn = 1;
  BlinkTimer->Start(BLINK_TM / 2);
  Sreg.Load(Segs);
}

//--------------------------- Очистка дисплея: -------------------------------

void TDisplay::Clear(void)
{
  Digits(' ');
  BlinkEn = 0;
  Sreg.Load(Segs);
}

//----------------------------- Тест дисплея: --------------------------------

void TDisplay::Test(void)
{
  Digits(8 + POINT);
  BlinkEn = 0;
  Sreg.Load(Segs);
}

//--------------------- Включение/выключение дисплея: ------------------------

void TDisplay::OnOff(bool on)
{
  Sreg.Init(on);
  if(on) Sreg.Load(Segs);
}

//------ Вывод времени в формате -H.MM.SS или HH.MM.SS или H.MM.SS.D: --------

void TDisplay::Time(int32_t v, bool lap)
{
  Digit(HOME);
  if(USE_UNSIGN)
  {
    if(USE_SEC10)
    {
      //время без знака с десятыми секунды:
      uint8_t sec10 = v % 10; v = v / 10;
      //преобразование sec -> hrs.min.sec:
      uint8_t hrs = v / SEC_PER_HRS;
      uint16_t m = v % SEC_PER_HRS;
      uint8_t min = m / SEC_PER_MIN;
      uint8_t sec = m % SEC_PER_MIN;
      //вывод на индикатор:
      Digit(hrs);
      Digit('.');
      Digit(min / 10);
      Digit(min % 10);
      Digit('.');
      Digit(sec / 10);
      Digit(sec % 10);
      Digit('.');
      Digit(sec10);
    }
    else
    {
      //время без знака без десятых секунды:
      //преобразование sec -> hrs.min.sec:
      uint8_t hrs = v / SEC_PER_HRS;
      uint16_t m = v % SEC_PER_HRS;
      uint8_t min = m / SEC_PER_MIN;
      uint8_t sec = m % SEC_PER_MIN;
      //вывод на индикатор:
      Digit(hrs / 10);
      Digit(hrs % 10);
      Digit('.');
      Digit(min / 10);
      Digit(min % 10);
      Digit('.');
      Digit(sec / 10);
      Digit(sec % 10);
    }
  }
  else
  {
    //время со знаком без десятых секунды:
    //запоминание знака:
    bool minus = v < 0;
    if(minus) v = -v;
    //преобразование sec -> hrs.min.sec:
    uint8_t hrs = v / SEC_PER_HRS;
    uint16_t m = v % SEC_PER_HRS;
    uint8_t min = m / SEC_PER_MIN;
    uint8_t sec = m % SEC_PER_MIN;
    //вывод на индикатор:
    Digit(minus? '-' : ' ');
    Digit(hrs);
    Digit('.');
    Digit(min / 10);
    Digit(min % 10);
    Digit('.');
    Digit(sec / 10);
    Digit(sec % 10);
  }
  if(lap) Digit('.');
  BlinkEn = 0;
  Sreg.Load(Segs);
}

//--------- Вывод метража в формате -MMMM.M или MMMMM.M или MMMM.M: ----------

void TDisplay::Metrage(int32_t v)
{
  //Лидирующие нули не подавляются.
  Digit(HOME);
  //метраж без знака:
  if(USE_UNSIGN)
  {
    if(USE_SEC10)
    {
      //метраж без знака с десятыми секунды:
      //вывод на индикатор:
      Bcd(v, Digs);
      Digit(' ');
      Digit(Digs[4]);
      Digit(Digs[3]);
      Digit(Digs[2]);
      Digit(Digs[1]);
      Digit('.');
      Digit(Digs[0]);
    }
    else
    {
      //метраж без знака без десятых секунды:
      //вывод на индикатор:
      Bcd(v, Digs);
      Digit(Digs[5]);
      Digit(Digs[4]);
      Digit(Digs[3]);
      Digit(Digs[2]);
      Digit(Digs[1]);
      Digit('.');
      Digit(Digs[0]);
    }
  }
  //метраж со знаком без десятых секунды:
  else
  {
    //запоминание знака:
    bool minus = v < 0;
    if(minus) v = -v;
    //вывод на индикатор:
    Bcd(v, Digs);
    Digit(minus? '-' : ' ');
    Digit(Digs[4]);
    Digit(Digs[3]);
    Digit(Digs[2]);
    Digit(Digs[1]);
    Digit('.');
    Digit(Digs[0]);
  }
  BlinkEn = 0;
  Sreg.Load(Segs);
}

//------------------- Вывод скорости в формате SSS.SSS: ----------------------

void TDisplay::Speed(uint32_t v)
{
  Digit(HOME);
  //2 цифры скорости после запятой:
  if(SPEED_LOW_RES)
  {
    if(v > MAX_SPEED / 10)
    {
      //вывод переполнения:
      String("   -.--");
    }
    else
    {
      //вывод значения на индикатор:
      Bcd(v, Digs);
      Digit(' ');
      if(!Digs[4]) Digs[4] = ' ';
      Digit(Digs[4]);
      if(!Digs[3] && Digs[4] == ' ') Digs[3] = ' ';
      Digit(Digs[3]);
      Digit(Digs[2]);
      Digit('.');
      Digit(Digs[1]);
      Digit(Digs[0]);
    }
  }
  //3 цифры скорости после запятой:
  else
  {
    if(v > MAX_SPEED)
    {
      //вывод переполнения:
      String("  -.---");
    }
    else
    {
      //вывод значения на индикатор:
      Bcd(v, Digs);
      if(!Digs[5]) Digs[5] = ' ';
      Digit(Digs[5]);
      if(!Digs[4] && Digs[5] == ' ') Digs[4] = ' ';
      Digit(Digs[4]);
      Digit(Digs[3]);
      Digit('.');
      Digit(Digs[2]);
      Digit(Digs[1]);
      Digit(Digs[0]);
    }
  }
  BlinkEn = 0;
  Sreg.Load(Segs);
}

//--------------- Вывод диаметра ролика в формате d DD.DD: -------------------

void TDisplay::Diameter(uint32_t v, uint8_t s)
{
  Digit(HOME);
  String("d ");
  if(s == D_CAL)
  {
    //вывод признака калибровки:
    String("CAL ");
  }
  else if(s == D_ERR)
  {
    //вывод сообщения об ошибке:
    String("Err ");
  }
  //вывод значения на индикатор:
  Bcd(v, Digs);
  Digit(Digs[4]);
  Digit(Digs[3]);
  Digit('.');
  Digit(Digs[2]);
  Digit(Digs[1]);
  BlinkEn = 0;
  Sreg.Load(Segs);
}

//----------------- Вывод номера версии в формате V.VV: ----------------------

void TDisplay::Version(void)
{
  Digit(HOME);
  Bcd(VERSION, Digs);
  String("   ");
  Digit(Digs[2]);
  Digit('.');
  Digit(Digs[1]);
  Digit(Digs[0]);
  BlinkEn = 0;
  Sreg.Load(Segs);
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
