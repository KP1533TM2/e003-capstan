//----------------------------------------------------------------------------

//Модуль релизации протокола Wake, заголовочный файл

//----------------------------------------------------------------------------

#ifndef WAKE_HPP
#define WAKE_HPP

//----------------------------- Константы: -----------------------------------

//Коды ошибок:
enum WakeErr_t
{
  ERR_NO, //no error
  ERR_TX, //Rx/Tx error
  ERR_BU, //device busy error
  ERR_RE, //device not ready error
  ERR_PA, //parameters value error
  ERR_NR, //no replay
  ERR_NC  //no carrier
};

//Коды стандартных команд:
enum WakeCmd_t
{
  CMD_NOP,     //нет операции
  CMD_ERR,     //ошибка приема пакета
  CMD_ECHO,    //эхо
  CMD_INFO,    //чтение информации об устройстве
  CMD_SETADDR, //установка адреса
  CMD_GETADDR  //чтение адреса
};

//------------------------ Класс протокола WAKE: -----------------------------

class TWake
{
private:
  enum WPnt_t
  {
    PTR_ADD,       //смещение в буфере для адреса
    PTR_CMD,       //смещение в буфере для кода команды
    PTR_LNG,       //смещение в буфере для длины пакета
    PTR_DAT        //смещение в буфере для данных
  };
  enum WStuff_t
  {
    FEND  = 0xC0,  //Frame END
    FESC  = 0xDB,  //Frame ESCape
    TFEND = 0xDC,  //Transposed Frame END
    TFESC = 0xDD   //Transposed Frame ESCape
  };
  enum WCrc_t
  {
    CRC_INIT = 0xDE, //начальное значение контрольной суммы
    CRC_FEND = 0x82  //начальное значение с учетом FEND
  };
  enum WState_t
  {
    WST_IDLE,      //состояние ожидания
    WST_ADD,       //прием адреса
    WST_CMD,       //прием команды
    WST_LNG,       //прием длины пакета
    WST_DATA,      //прием/передача данных
    WST_CRC,       //прием/передача CRC
    WST_DONE       //состояние готовности
  };

  uint8_t Addr;    //адрес устройства
  uint8_t RxState; //состояние процесса приема
  bool RxStuff;    //признак стаффинга при приеме
  uint8_t *RxPtr;  //указатель буфера приема
  uint8_t *RxEnd;  //значение указателя конца буфера приема
  uint8_t RxCount; //количество принятых байт
  uint8_t *RxData; //буфер приема

  uint8_t TxState; //состояние процесса передачи
  bool TxStuff;    //признак стаффинга при передаче
  uint8_t *TxPtr;  //указатель буфера передачи
  uint8_t *TxEnd;  //значение указателя конца буфера передачи
  uint8_t TxCount; //количество передаваемых байт
  uint8_t *TxData; //буфер передачи

  uint8_t Frame;
  void Do_Crc8(uint8_t b, uint8_t *crc); //вычисление контрольной суммы
protected:
  inline void Rx(uint8_t data);  //прием байта
  inline bool Tx(uint8_t &data); //передача байта
public:
  TWake(const uint8_t frame);
  uint8_t GetCmd(void);      //возвращает текущий код команды
  uint8_t GetRxCount(void);  //возвращает количество принятых байт
  void SetRxPtr(uint8_t p);  //устанавливает указатель буфера приема
  uint8_t GetRxPtr(void);    //читает указатель буфера приема
  uint8_t GetByte(void);     //читает байт из буфера приема
  int16_t GetWord(void);     //читает слово из буфера приема
  int32_t GetDWord(void);    //читает двойное слово из буфера приема
  void GetData(uint8_t *d, uint8_t count); //читает данные из буфера приема

  void SetTxPtr(uint8_t p);  //устанавливает указатель буфера передачи
  uint8_t GetTxPtr(void);    //читает указатель буфера передачи
  void AddByte(uint8_t b);   //помещает байт в буфер передачи
  void AddWord(int16_t w);   //помещает слово в буфер передачи
  void AddDWord(int32_t dw); //помещает двойное слово в буфер передачи
  void AddData(uint8_t *d, uint8_t count);  //помещает данные в буфер передачи
  void TxStart(uint8_t cmd, uint8_t &data); //начало передачи пакета
  bool AskTxEnd(void);       //определение конца передачи пакета
};

//----------------------------------------------------------------------------
//---------------------------- Inline-функции: -------------------------------
//----------------------------------------------------------------------------

//----------------------------- Прием байта: ---------------------------------

#pragma inline = forced
inline void TWake::Rx(uint8_t data)
{
  if(RxState != WST_DONE)            //если прием разрешен
  {
    if(data == FEND)                 //обнаружен FEND (из любого состояния)
    {
      RxState = WST_ADD;             //переход к приему адреса
      RxPtr = RxData;                //указатель на начало буфера
      RxStuff = 0; return;           //нет дестаффинга
    }
    if(data == FESC)                 //принят FESC,
    { RxStuff = 1; return; }         //начало дестафинга
    if(RxStuff)                      //если идет дестафинг,
    {
      if(data == TFESC)              //если принят TFESC,
        data = FESC;                 //замена его на FESC
      else if(data == TFEND)         //если принят TFEND,
        data = FEND;                 //замена его на FEND
        else { RxState = WST_IDLE; return; } //иначе ошибка стаффинга
      RxStuff = 0;                   //дестаффинг закончен
    }
    switch(RxState)
    {
    case WST_ADD:                    //прием адреса
        RxState = WST_CMD;           //далее - прием команды
        if(data & 0x80)              //если принят адрес,
        {
          data &= ~0x80;             //восстановление значения адреса
          if(data != Addr)           //адрес не совпал,
          { RxState = WST_IDLE; return; } //переход к поиску FEND
          break;                     //переход к сохранению адреса
        }
        else *RxPtr++ = 0;           //сохранение нулевого адреса
    case WST_CMD:                    //прием кода команды
        RxState = WST_LNG;           //далее - прием длины пакета
        break;                       //переход к сохранению команды
    case WST_LNG:                    //идет прием длины пакета
        RxState = WST_DATA;          //далее - прием данных
        if(data > Frame) data = Frame;   //ограничение длины пакета
        RxEnd = RxData + PTR_DAT + data; //указатель на конец данных
        break;
    case WST_DATA:                   //идет прием данных
        if(RxPtr == RxEnd)           //если все данные и CRC приняты,
          RxState = WST_DONE;        //прием окончен
        break;
    default: return;
    }
    *RxPtr++ = data;                 //сохранение данных в буфере
  }
}

//---------------------------- Передача байта: -------------------------------

#pragma inline = forced
inline bool TWake::Tx(uint8_t &data)
{
  if(TxState == WST_DATA)            //если идет передача данных,
  {
    data = *TxPtr++;                 //то чтение байта из буфера
    if(data == FEND || data == FESC) //попытка передать FEND или FESC,
      if(!TxStuff)                   //нужен стаффинг
      {
        data = FESC;                 //передача FESC
        TxStuff = 1;                 //начало стаффинга
        TxPtr--;                     //возврат к тому же байту
      }
      else
      {
        if(data == FEND) data = TFEND; //передача TFEND
          else data = TFESC;         //или TFESC
        TxStuff = 0;                 //конец стаффинга
      }
    if(TxPtr > TxEnd)                //если конец буфера достигнут,
      TxState = WST_CRC;             //передается CRC
    return(1);
  }
  else                               //если передача закончена,
  {
    TxState = WST_DONE;              //передача пакета закончена
    return(0);
  }
}

//----------------------------------------------------------------------------

#endif
