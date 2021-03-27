//----------------------------------------------------------------------------

//Модуль поддержки LED-дисплея, заголовочный файл

//----------------------------------------------------------------------------

#ifndef DISPLAY_HPP
#define DISPLAY_HPP

//----------------------------------------------------------------------------
//----------------------------- Класс TSreg: ---------------------------------
//----------------------------------------------------------------------------

class TSreg
{
private:
  Pin_SCLK_t Pin_SCLK;
  Pin_SDATA_t Pin_SDATA;
  Pin_SS_t Pin_SS;
  Pin_LOAD_t Pin_LOAD;
  Pin_OE_t Pin_OE;
  void SpiWr(uint8_t d);
  static uint8_t const REGS = 6;        //количество регистров
  uint8_t Pos;
public:
  TSreg(void) {};
  void Init(bool on);                   //инициализация SPI
  void Load(uint8_t *Value);            //загрузка регистра
};

//----------------------------------------------------------------------------
//---------------------------- Класс TDisplay: -------------------------------
//----------------------------------------------------------------------------

class TDisplay
{
private:
  enum { DIG1, DIG2, DIG3, DIG4, DIG5, DIG6, DIGS };
  static uint8_t const POINT = 0x80;   //код децимальной точки
  static uint8_t const HOME = '\r';    //код возврата на начало строки
  uint8_t Digs[DIGS];                  //буфер символов
  uint8_t Segs[DIGS];                  //буфер сегментов
  uint8_t Darks[DIGS];                 //буфер сегментов для режима гашения
  uint8_t Seven(uint8_t d);            //перекодировка в 7-сегментный код
  void Digit(uint8_t d);               //вывод цифры на индикатор
  void Digits(uint8_t d);              //заполнение индикатора одной цифрой
  void String(const uint8_t *s);       //вывод строки из Flash
  void Bcd(uint32_t v, uint8_t *buff); //преобразование числа в BCD-формат
  uint8_t Pos;                         //текущая позиция
  bool BlinkEn;                        //флаг разрешения мигания
  bool BlinkOn;                        //флаг активной фазы мигания
  TSreg Sreg;
  TSoftTimer *BlinkTimer;
  static uint16_t const BLINK_TM = 500; //период мигания, мс
public:
  TDisplay(void);
  void Execute(void);                  //выполнение мигания
  void Blink(uint8_t p);               //включение мигания выбранных разрядов
  static uint8_t const BLINK = 0x3C;   //маска разрядов мигания при калибровке
  void Clear(void);                    //очистка дисплея
  void Test(void);                     //тест дисплея (включение всех сегментов)
  void OnOff(bool on);                 //включение/выключение дисплея
  void Time(int32_t v, bool lap = 0);  //вывод времени
  static bool const LAP = 1;           //код включения режима LapTime
  void Metrage(int32_t v);             //вывод метража
  void Speed(uint32_t v);              //вывод скорости
  void Diameter(uint32_t v, uint8_t s = 0); //вывод диаметра ролика и сообщений
  static uint16_t const D_CAL = 1;     //код сообщения "CAL"
  static uint16_t const D_ERR = 2;     //код сообщения "Err"
  void Version(void);                  //вывод номера версии
};

//----------------------------------------------------------------------------

extern TDisplay *Display;

//----------------------------------------------------------------------------

#endif
