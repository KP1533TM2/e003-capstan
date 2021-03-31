#pragma hdrstop

#include "WakeCom.h"
#include "registry.hpp"

#pragma package(smart_init)

//--------------------------- Конструктор: ----------------------------------

__fastcall TWakeCom::TWakeCom()
{
  CmdCrSec = new TCriticalSection();

  OSVERSIONINFO Ver;
  Ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  GetVersionEx(&Ver);
  Platform_NT = Ver.dwPlatformId == VER_PLATFORM_WIN32_NT;

  FBaudRate = DEFAULT_BAUD_RATE;
  FDeviceName = "";
  hCom = NULL;
  SetNumber(0);
}

//---------------------------- Деструктор: ----------------------------------

__fastcall TWakeCom::~TWakeCom()
{
  delete CmdCrSec;
}

//---------------------- Получение списка портов: ---------------------------

void __fastcall TWakeCom::GetPortList(TStringList *APortBox)
{
  TRegistry *Reg;
  Reg = new TRegistry(KEY_READ);
  try
  {
    Reg->RootKey = HKEY_LOCAL_MACHINE;
    if (Reg->OpenKeyReadOnly("\\HARDWARE\\DEVICEMAP\\SERIALCOMM") )
    {
      Reg->GetValueNames(APortBox);
      for (int i = 0; i < APortBox->Count; i++)
          APortBox->Strings[i] = Reg->ReadString(APortBox->Strings[i]);
      Reg->CloseKey();
    }
    else
    {
      APortBox->Add("None");
    }
  }
  __finally
  {
    Reg->Free();
  }
  APortBox->Sort();
}

//------------------ Преобразование имени порта в номер: --------------------

int __fastcall TWakeCom::NameToNumber(AnsiString name)
{
  if(name == "None") return(0);
  return(StrToInt(name.SubString(4, 2)));
}

//------------------ Преобразование номера порта в имя: ---------------------

AnsiString __fastcall TWakeCom::NumberToName(int number)
{
  if(number == 0) return("None");
  return("COM" + IntToStr(number));
}

//----------------------- Установка имени порта: ----------------------------

void __fastcall TWakeCom::SetName(AnsiString name)
{
  FName = name;
  FNumber = NameToNumber(FName);
}

//---------------------- Установка номера порта: ----------------------------

void __fastcall TWakeCom::SetNumber(int number)
{
  FNumber = number;
  FName = NumberToName(FNumber);
}

//---------------- Проверка доступности порта (по номеру): ------------------

bool __fastcall TWakeCom::Access(int PortNumber)
{
  AnsiString name = NumberToName(PortNumber);
  return(Access(name));
}

//---------------- Проверка доступности порта (по имени): -------------------

bool __fastcall TWakeCom::Access(AnsiString PortName)
{
  AnsiString PName = "\\\\.\\" + PortName;
  HANDLE hTemp = CreateFile(PCHAR(PName.c_str()),
                    GENERIC_READ | GENERIC_WRITE,
                    0,
                    NULL,
                    OPEN_EXISTING,
                    0,
                    NULL);
  if (hTemp != INVALID_HANDLE_VALUE)
   { CloseHandle(hTemp); return(1); }
   else return(0);
}

//---------------------- Открыть порт (по номеру): --------------------------

void __fastcall TWakeCom::Open(int PortNumber)
{
  SetNumber(PortNumber);
  Open(FName);
}

//---------------------- Открыть порт (по имени): ---------------------------

void __fastcall TWakeCom::Open(AnsiString PortName)
{
  SetName(PortName);
  AnsiString PName = "\\\\.\\" + PortName;
  hCom = CreateFile(PCHAR(PName.c_str()),
                   GENERIC_READ | GENERIC_WRITE,
                   0,
                   NULL,
                   OPEN_EXISTING,
                   0,
                   NULL);
  if(hCom == INVALID_HANDLE_VALUE)
   throw EInOutError("Port " + FName + " open error.");

  if(!Platform_NT)
  {
    //Undocumented function returns the base address in edx:
    EscapeCommFunction(hCom, 10);
    BaseAddress = _DX;
  }

  if(!GetCommState(hCom, &dcb))
   throw EInOutError("Port " + FName + " DCB read error.");
  if(!GetCommTimeouts(hCom, &ComTo))
   throw EInOutError("Port " + FName + " timeouts read error.");
  dcbc = dcb;                //сохранение DCB
  ComToc = ComTo;            //сохранение timeouts

  dcb.BaudRate = FBaudRate;  //set boud rate
  dcb.fBinary = 1;           //binary mode
  dcb.fParity = 0;           //disable parity checking
  dcb.fOutxCtsFlow = 0;      // CTS output flow control
  dcb.fOutxDsrFlow = 0;      // DSR output flow control
  dcb.fDsrSensitivity = 0;   // DSR sensitivity
  dcb.fTXContinueOnXoff = 0; // XOFF continues Tx
  dcb.fOutX = 0;             // XON/XOFF out flow control
  dcb.fInX = 0;              // XON/XOFF in flow control
  dcb.fErrorChar = 0;        // enable error replacement
  dcb.fNull = 0;             // enable null stripping
  dcb.fAbortOnError = 0;     // abort reads/writes on error
  dcb.ByteSize = 8;          //set byte size
  dcb.Parity = NOPARITY;     //set no parity
  dcb.StopBits = ONESTOPBIT; //set one stop bit
  //dcb.StopBits = TWOSTOPBITS; //set two stop bits
  SetMode(DEFAULT_RTS_STATE, DEFAULT_DTR_STATE);

  if(!SetupComm(hCom, 512, 512))
   throw EInOutError("Port " + FName + " buffer size set error.");

  ComTo.ReadIntervalTimeout = MAXDWORD;
  ComTo.ReadTotalTimeoutMultiplier = MAXDWORD;
  ComTo.WriteTotalTimeoutMultiplier = 0;
  SetTo(DEFAULT_RX_TIMEOUT, DEFAULT_TX_TIMEOUT);
  Purge();
}

//----------------------- Создать список устройств: --------------------------

void __fastcall TWakeCom::GetDeviceList(TStringList *ADeviceBox)
{
  TStringList *APortBox = new TStringList;
  try
  {
    GetPortList(APortBox);
    for (int i = 0; i < APortBox->Count; i++)
      try
      {
        Open(APortBox->Strings[i]);
        ADeviceBox->Add(GetInfo(0));
        Close();
      }
      catch(const EInOutError &e) { Close(); }
  }
   __finally { delete APortBox; }
}

//------------------- Создать список сетевых устройств: ----------------------

void __fastcall TWakeCom::GetNetDeviceList(AnsiString PortName,
                                            TStringList *ADeviceBox)
{
  Open(PortName);
  for (int i = 1; i < 128; i++)
  try
  {
    ADeviceBox->Add(GetInfo(i));
  }
  catch(const EInOutError &e) { }
}

//------------------ Открыть порт (по имени устройства): ---------------------

void __fastcall TWakeCom::OpenDevice(AnsiString DevName)
{
  TStringList *APortBox = new TStringList;
  try
  {
    GetPortList(APortBox);
    for (int i = 0; i < APortBox->Count; i++)
      try
      {
        Open(APortBox->Strings[i]);
        if(GetInfo(0).SubString(1, DevName.Length()) == DevName)
          { FDeviceName = DevName; return; }
            else Close();
      }
      catch(const EInOutError &e) { }
   throw EInOutError("Device " + DevName + " not found.");
   }
   __finally { delete APortBox; }
}

//--------------------------- Закрыть порт: ---------------------------------

void __fastcall TWakeCom::Close(void)
{
  SetNumber(0);
  FDeviceName = "";
  if(hCom)
  {
    SetCommState(hCom, &dcbc);
    SetCommTimeouts(hCom, &ComToc);
    if(RtsToggle) EscapeCommFunction(hCom, CLRRTS);
    CloseHandle(hCom);
    hCom = NULL;
  }
}

//--------------------- Установка скорости обмена: --------------------------

void __fastcall TWakeCom::SetBaudRate(int baudrate)
{
  FBaudRate = baudrate;
  if(hCom)
  {
    dcb.BaudRate = FBaudRate;
    if(!SetCommState(hCom, &dcb))
      throw EInOutError("Port " + FName + " set baud error.");
  }
}

//------------------ Управление сигналами RTS и DTR: ------------------------

// winbase.h

// #define DTR_CONTROL_DISABLE    0x00
// #define DTR_CONTROL_ENABLE     0x01
// #define DTR_CONTROL_HANDSHAKE  0x02

// #define RTS_CONTROL_DISABLE    0x00
// #define RTS_CONTROL_ENABLE     0x01
// #define RTS_CONTROL_HANDSHAKE  0x02
// #define RTS_CONTROL_TOGGLE     0x03

void __fastcall TWakeCom::SetMode(DWORD RtsMode, DWORD DtrMode)
{
  RtsToggle = RtsMode == RTS_CONTROL_TOGGLE;
  if(hCom)
  {
    if(RtsToggle)
    {
      if(Platform_NT)
        dcb.fRtsControl = RTS_CONTROL_TOGGLE;
      else dcb.fRtsControl = RTS_CONTROL_DISABLE;
    }
    else dcb.fRtsControl = RtsMode;
    dcb.fDtrControl = DtrMode;
    if(!SetCommState(hCom, &dcb))
     throw EInOutError("Port " + FName + " setup error.");
  }
}

//------------------------ Установка таймаутов: -----------------------------

void __fastcall TWakeCom::SetTo(DWORD RxTo, DWORD TxTo)
{
  if(hCom)
  {
    ComTo.ReadTotalTimeoutConstant = RxTo;
    ComTo.WriteTotalTimeoutConstant = TxTo;
    if(!SetCommTimeouts(hCom, &ComTo))
     throw EInOutError("Port " + FName + " timeouts set error.");
  }
}

//----------------------- Очистка буфера порта: -----------------------------

void __fastcall TWakeCom::Purge(void)
{
  if(hCom)
  {
    PurgeComm(hCom, PURGE_TXABORT |
                    PURGE_RXABORT |
                    PURGE_TXCLEAR |
                    PURGE_RXCLEAR);
  }
}

//----------------------------- Прием байта: --------------------------------

void __fastcall TWakeCom::RxByte(unsigned char &b)
{
  DWORD r; bool x;
  x =  ReadFile(hCom, &b, 1, &r, NULL);
  if(!x || (r != 1))
    throw EInOutError("No data to receive.");
}

//--------- Чтение порта по физическому адресу (not for NT systems): --------

BYTE __fastcall TWakeCom::PortIn(WORD Address)
{
  asm
  {
    mov dx,Address
    in  al,dx
  }
  return(_AL);
}

//--------------------------- Byte Stuffing: --------------------------------

void __fastcall TWakeCom::ByteStuff(unsigned char b, DWORD &bptr,
                                 unsigned char *buff)
{
  if ((b == FEND) || (b == FESC))
  {
    buff[bptr++] = FESC;
    buff[bptr++] = (b == FEND)? TFEND : TFESC;
  }
  else buff[bptr++] = b;
}

//-------------------------- Byte DeStuffing: -------------------------------

void __fastcall TWakeCom::ByteDeStuff(unsigned char &b)
{
  RxByte(b);
  if(b == FESC)
  {
    RxByte(b);
    if(b == TFEND) b = FEND;
      else if(b == TFESC) b = FESC;
        else throw EInOutError("WAKE destuffing error.");
  }
}

//--------------------------- Вычисление CRC: -------------------------------

void __fastcall TWakeCom::DowCRC(unsigned char b, unsigned char &crc)
{
  for(int i = 0; i < 8; i++)
  {
    if(((b ^ crc) & 1) != 0)
      crc = ((crc ^ 0x18) >> 1) | 0x80;
        else crc = (crc >> 1) & ~0x80;
    b = b >> 1;
  }
}

//--------------------------- Прием пакета: ---------------------------------

void __fastcall TWakeCom::RxFrame(unsigned char &Addr,
                               unsigned char &Cmd,
                               unsigned char &Nbt,
                               unsigned char *Data)
{
  int DataPtr;                             //указатель буфера данных
  unsigned char data_byte;                 //принятый байт
  unsigned char crc = CRC_INIT;            //контрольная сумма

  for(int i = 0; i < 512 && data_byte != FEND; i++)
    RxByte(data_byte);                     //поиск FEND
  if(data_byte != FEND)
    throw EInOutError("WAKE frame marker not found.");

  DowCRC(data_byte, crc);
  ByteDeStuff(data_byte);                  //прием адреса
  if(data_byte & 0x80)
  {
    Addr = data_byte & 0x7F;
    DowCRC(data_byte & 0x7F, crc);
    ByteDeStuff(data_byte);                //прием команды
  }
  else Addr = 0;
  Cmd = data_byte;
  DowCRC(data_byte, crc);
  ByteDeStuff(data_byte);                  //прием длины пакета
  Nbt = data_byte;
  DowCRC(data_byte, crc);
  for(DataPtr = 0; DataPtr < Nbt; DataPtr++)
  {
    ByteDeStuff(data_byte);                //прием данных
    Data[DataPtr] = data_byte;
    DowCRC(data_byte, crc);
  }
  ByteDeStuff(data_byte);                  //прием CRC
  DowCRC(data_byte, crc);
  if(crc)
    throw EInOutError("WAKE CRC error.");
}

//--------------------------- Передача пакета: ------------------------------

void __fastcall TWakeCom::TxFrame(unsigned char Addr,
                               unsigned char Cmd,
                               unsigned char Nbt,
                               unsigned char *Data)
{
  unsigned char Buff[518];                 //буфер для передачи
  DWORD r, BuffPtr = 0;                    //указатель буфера
  unsigned char data_byte;                 //передаваемый байт
  unsigned char crc = CRC_INIT;            //контрольная сумма

  //заполнение буфера:
  Buff[BuffPtr++] = FEND;                  //передача FEND
  DowCRC(FEND, crc);
  if(Addr)
  {
    ByteStuff(Addr | 0x80, BuffPtr, Buff); //передача адреса
    DowCRC(Addr & 0x7F, crc);
  }
  ByteStuff(Cmd & 0x7F, BuffPtr, Buff);    //передача команды
  DowCRC(Cmd & 0x7F, crc);
  ByteStuff(Nbt, BuffPtr, Buff);           //передача длины пакета
  DowCRC(Nbt, crc);
  for(int i = 0; i < Nbt; i++)
  {
    ByteStuff(Data[i], BuffPtr, Buff);     //передача данных
    DowCRC(Data[i], crc);
  }
  ByteStuff(crc, BuffPtr, Buff);           //передача CRC

  //передача буфера:
  if(!Platform_NT && RtsToggle)
  {
    EscapeCommFunction(hCom, SETRTS);      //set RTS (not NT systems)
  }
  bool x = WriteFile(hCom, Buff, BuffPtr, &r, NULL); //передача буфера
  if(!Platform_NT && RtsToggle)
  {
    while(!(PortIn(BaseAddress + 5) & 0x40));
    EscapeCommFunction(hCom, CLRRTS);      //clear RTS (not NT systems)
  }
  if(!x || (r != BuffPtr))
    throw EInOutError("WAKE framing error.");
}

//--------------------------- Передача команды: -----------------------------

void __fastcall TWakeCom::SendCmd(int a)
{
  unsigned char rxAdd = 0, rxCmd = 0;
  int err;

  if(hCom)                                 //если порт открыт
  {
    RxN = 0;
    FTxAddr = a;                           //сохранение адреса
    TxFrame((unsigned char)FTxAddr,        //передача пакета
            (unsigned char)FTxCmd,
            (unsigned char)FTxPtr,
            FTxData);
    for(int i = 0; i < FRAME; i++) FRxData[i] = 0; //очистка буфера
    RxFrame(rxAdd,                         //прием пакета
            rxCmd,
            RxN,
            FRxData);

    if((rxCmd == CMD_INFO) || (rxCmd == CMD_ECHO)) err = ERR_NO;
      else err = FRxData[0];
    if(err > ERR_NC) err = ERR_TX;
    if(rxAdd != FTxAddr) err = ERR_NC;
      else if(rxCmd != FTxCmd) err = ERR_PA;

    if(err)
    {
      AnsiString EStr = "Error of command " + FTxCmdName + ":\r";
      switch(err)
      {
        case ERR_TX: EStr += "invalid packet."; break;
        case ERR_BU: EStr += "devise busy."; break;
        case ERR_RE: EStr += "device not ready."; break;
        case ERR_PA: EStr += "invalid parameters."; break;
        case ERR_NC: EStr += "invalid address."; break;
        case ERR_NR: EStr += "timeout reached."; break;
        default: EStr += "unknown error.";
      }
      throw EInOutError(EStr);
    }
  }
}

//--------------------- Начало формирования пакета: -------------------------

void __fastcall TWakeCom::NewCmd(int cmd, AnsiString name)
{
  FTxCmd = cmd;
  FTxCmdName = name;
  FTxPtr = 0;
  FRxPtr = 1;
}

//---------------------- Добавление байта в буфер: --------------------------

void __fastcall TWakeCom::AddByte(unsigned char b)
{
  if(FTxPtr < FRAME)
    FTxData[FTxPtr++] = LO(b);
}

//---------------------- Добавление слова в буфер: --------------------------

void __fastcall TWakeCom::AddWord(unsigned short b)
{
  if(FTxPtr < FRAME - 1)
  {
    FTxData[FTxPtr++] = LO(b);
    FTxData[FTxPtr++] = HI(b);
  }
}

//---------------- Добавление двойного слова в буфер: -----------------------

void __fastcall TWakeCom::AddDWord(unsigned int b)
{
  if(FTxPtr < FRAME - 3)
  {
    FTxData[FTxPtr++] = BYTE1(b);
    FTxData[FTxPtr++] = BYTE2(b);
    FTxData[FTxPtr++] = BYTE3(b);
    FTxData[FTxPtr++] = BYTE4(b);
  }
}

//-------------------- Добавление данных в буфер: ---------------------------

void __fastcall TWakeCom::AddData(char *d, int count)
{
  if(FTxPtr <= FRAME - count)
    for(int i = 0; i < count; i++)
      FTxData[FTxPtr++] = *d++;
}

//-------------------- Чтение байта из буфера: ------------------------------

unsigned char __fastcall TWakeCom::GetByte(void)
{
  return(FRxData[FRxPtr++]);
}

//-------------------- Чтение слова из буфера: ------------------------------

unsigned short __fastcall TWakeCom::GetWord(void)
{
  unsigned char l = FRxData[FRxPtr++];
  unsigned char h = FRxData[FRxPtr++];
  return((unsigned short)WORD(h, l));
}

//--------------- Чтение двойного слова из буфера: --------------------------

unsigned int __fastcall TWakeCom::GetDWord(void)
{
  unsigned char b0 = FRxData[FRxPtr++];
  unsigned char b1 = FRxData[FRxPtr++];
  unsigned char b2 = FRxData[FRxPtr++];
  unsigned char b3 = FRxData[FRxPtr++];
  return((unsigned int)DWORD(b3, b2, b1, b0));
}

//-------------------- Чтение данных из буфера: -----------------------------

void __fastcall TWakeCom::GetData(char *d, int count)
{
  for(int i = 0; i < count; i++)
    *d++ = FRxData[FRxPtr++];
}

//------------------- Чтение количества принятых байт: ----------------------

int __fastcall TWakeCom::GetRxN(void)
{
  return(RxN);
}

//---------------------------------------------------------------------------
//---------------------------- Общие команды: -------------------------------
//---------------------------------------------------------------------------

//------------------------------ CMD_NOP: ------------------------------------

//Not used

//------------------------------ CMD_ERR: ------------------------------------

//Not used

//------------------------------ CMD_ECHO: -----------------------------------

//Not used

//------------------------------ CMD_INFO: -----------------------------------

AnsiString __fastcall TWakeCom::GetInfo(int a)
{
  AnsiString s;
  _ENTER_COMMAND(CmdCrSec)
  NewCmd(CMD_INFO, "GetInfo");
  SendCmd(a);
  s = AnsiString((char*)FRxData);
  _LEAVE_COMMAND(CmdCrSec)
  return(s);
}

//---------------------------------------------------------------------------


