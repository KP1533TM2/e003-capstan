#ifndef WakeComH
#define WakeComH

#include <SysUtils.hpp>
#include <SyncObjs.hpp>
#include <Classes.hpp>

#define DEFAULT_BAUD_RATE  19200 //значение скорости по умолчанию, бод
#define DEFAULT_TX_TIMEOUT    50 //timeout на передачу по умолчанию, мс
#define DEFAULT_RX_TIMEOUT    50 //timeout на прием по умолчанию, мс
#define LONG_RX_TIMEOUT      400 //длинный timeout на прием, мс

#define DEFAULT_RTS_STATE  RTS_CONTROL_TOGGLE  //состояние RTS по умолчанию
#define DEFAULT_DTR_STATE  DTR_CONTROL_DISABLE //состояние DTR по умолчанию

#define FRAME     250   //размер буфера Wake

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

#define LO(x)    (x & 0xFF)
#define HI(x)    ((x >> 8) & 0xFF)
#define BYTE1(x) LO(x)
#define BYTE2(x) HI(x)
#define BYTE3(x) ((x >> 16) & 0xFF)
#define BYTE4(x) ((x >> 24) & 0xFF)
#define WORD(h, l) ((h << 8) | l)
#define DWORD(b3,b2,b1,b0) ((WORD(b3,b2) << 16) | WORD(b1,b0))

#define _ENTER_COMMAND(x) x->Enter();try{
#define _LEAVE_COMMAND(x) }__finally{x->Leave();}

//------------------ Класс реализации протокола WAKE: -----------------------

class TWakeCom : public TObject
{
private:
  HANDLE hCom;         //handle порта
  DCB dcb;             //структора DCB
  DCB dcbc;            //сохраненная структура DCB
  COMMTIMEOUTS ComTo;  //структура timeout-ов
  COMMTIMEOUTS ComToc; //сохраненная структура timeout-ов
  AnsiString FName;    //логическое имя порта
  int FNumber;         //номер порта
  int FBaudRate;       //скорость обмена
  bool Platform_NT;    //флаг типа платформы
  WORD BaseAddress;    //базовый адрес UART (not for NT systems)
  bool RtsToggle;      //флаг режима RTS toggle (not for NT systems)

  void __fastcall SetNumber(int number);     //установка номера порта
  void __fastcall SetName(AnsiString name);  //установка имени порта

  enum WStuff_t
  {
    FEND  = 0xC0,        //Frame END
    FESC  = 0xDB,        //Frame ESCape
    TFEND = 0xDC,        //Transposed Frame END
    TFESC = 0xDD         //Transposed Frame ESCape
  };
  enum WCrc_t
  {
    CRC_INIT = 0xDE      //начальное значение контрольной суммы
  };
  int FTxAddr;           //передаваемый адрес
  int FTxCmd;            //код передаваемой команды
  int FTxPtr;            //указатель буфера передачи
  int FRxPtr;            //указатель буфера приема
  unsigned char RxN;     //количество принятых байт
  AnsiString FTxCmdName; //имя передаваемой команды
  AnsiString FDeviceName; //имя устройства
  void __fastcall RxByte(unsigned char &b); //прием байта
  BYTE __fastcall PortIn(WORD Address); //чтение порта (not for NT systems)
  void __fastcall ByteStuff(unsigned char b, DWORD &bptr, unsigned char *buff);
  void __fastcall ByteDeStuff(unsigned char &b);
  void __fastcall DowCRC(unsigned char b, unsigned char &crc); //вычисление CRC
  void __fastcall RxFrame(unsigned char &Addr,
                          unsigned char &Cmd,
                          unsigned char &Nbt,
                          unsigned char *Data); //прием пакета
  void __fastcall TxFrame(unsigned char Addr,
                          unsigned char Cmd,
                          unsigned char Nbt,
                          unsigned char *Data); //передача пакета
  unsigned char FTxData[FRAME]; //буфер передачи
  unsigned char FRxData[FRAME]; //буфер приема

protected:
  TCriticalSection *CmdCrSec; //критическая секция для выполнения команд
  void __fastcall NewCmd(int cmd, AnsiString name); //начало формирования пакета
  void __fastcall AddByte(unsigned char b);  //добавление байта в буфер
  void __fastcall AddWord(unsigned short b); //добавление слова в буфер
  void __fastcall AddDWord(unsigned int b);  //добавление двойного слова в буфер
  void __fastcall AddData(char *d, int count); //добавление данных в буфер
  void __fastcall SendCmd(int a = 0); //передача пакета по заданному адресу
  unsigned char __fastcall  GetByte(void);   //чтение байта из буфера
  unsigned short __fastcall  GetWord(void);  //чтение слова из буфера
  unsigned int __fastcall  GetDWord(void);   //чтение двойного слова из буфера
  void __fastcall GetData(char *d, int count); //чтение данных из буфера
  int __fastcall GetRxN(void); //чтение количества принятых байт

public:
  __fastcall TWakeCom();  //конструктор
  __fastcall ~TWakeCom(); //деструктор

  int __fastcall NameToNumber(AnsiString name); //преобразование имени в номер
  AnsiString __fastcall NumberToName(int number); //преобразование номера в имя
  void __fastcall GetPortList(TStringList *APortBox); //получение списка портов
  bool __fastcall Access(int PortNumber); //проверка доступности порта по номеру
  bool __fastcall Access(AnsiString PortName); //проверка доступности порта по имени
  void __fastcall Open(int PortNumber); //открыть порт по номеру
  void __fastcall Open(AnsiString PortName); //открыть порт по имени
  void __fastcall GetDeviceList(TStringList *ADeviceBox); //создать список устр.
  void __fastcall GetNetDeviceList(AnsiString PortName,      //создать список
                                   TStringList *ADeviceBox); //сетевых устр.
  void __fastcall OpenDevice(AnsiString DevName); //открыть порт по имени устр.
  void __fastcall Close(); //закрыть порт
  void __fastcall SetBaudRate(int baudrate); //установка скорости обмена
  void __fastcall SetMode(DWORD RtsMode, DWORD DtrMode); //установка режима
  void __fastcall SetTo(DWORD RxTo, DWORD TxTo); //установка таймаутов
  void __fastcall Purge(void); //очистка буфера порта

  __property int Number = {read=FNumber, write=SetNumber}; //номер порта
  __property AnsiString PortName = {read=FName, write=SetName}; //имя порта
  __property int BaudRate = {read=FBaudRate, write=SetBaudRate}; //скорость
  __property AnsiString DeviceName = {read=FDeviceName}; //имя порта

  AnsiString __fastcall GetInfo(int a = 0); //чтение информации об устройстве
};

//---------------------------------------------------------------------------

#endif
