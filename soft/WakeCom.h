#ifndef WakeComH
#define WakeComH

#include <SysUtils.hpp>
#include <SyncObjs.hpp>
#include <Classes.hpp>

#define DEFAULT_BAUD_RATE  19200 //�������� �������� �� ���������, ���
#define DEFAULT_TX_TIMEOUT    50 //timeout �� �������� �� ���������, ��
#define DEFAULT_RX_TIMEOUT    50 //timeout �� ����� �� ���������, ��
#define LONG_RX_TIMEOUT      400 //������� timeout �� �����, ��

#define DEFAULT_RTS_STATE  RTS_CONTROL_TOGGLE  //��������� RTS �� ���������
#define DEFAULT_DTR_STATE  DTR_CONTROL_DISABLE //��������� DTR �� ���������

#define FRAME     250   //������ ������ Wake

//���� ������:
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

//���� ����������� ������:
enum WakeCmd_t
{
  CMD_NOP,     //��� ��������
  CMD_ERR,     //������ ������ ������
  CMD_ECHO,    //���
  CMD_INFO,    //������ ���������� �� ����������
  CMD_SETADDR, //��������� ������
  CMD_GETADDR  //������ ������
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

//------------------ ����� ���������� ��������� WAKE: -----------------------

class TWakeCom : public TObject
{
private:
  HANDLE hCom;         //handle �����
  DCB dcb;             //��������� DCB
  DCB dcbc;            //����������� ��������� DCB
  COMMTIMEOUTS ComTo;  //��������� timeout-��
  COMMTIMEOUTS ComToc; //����������� ��������� timeout-��
  AnsiString FName;    //���������� ��� �����
  int FNumber;         //����� �����
  int FBaudRate;       //�������� ������
  bool Platform_NT;    //���� ���� ���������
  WORD BaseAddress;    //������� ����� UART (not for NT systems)
  bool RtsToggle;      //���� ������ RTS toggle (not for NT systems)

  void __fastcall SetNumber(int number);     //��������� ������ �����
  void __fastcall SetName(AnsiString name);  //��������� ����� �����

  enum WStuff_t
  {
    FEND  = 0xC0,        //Frame END
    FESC  = 0xDB,        //Frame ESCape
    TFEND = 0xDC,        //Transposed Frame END
    TFESC = 0xDD         //Transposed Frame ESCape
  };
  enum WCrc_t
  {
    CRC_INIT = 0xDE      //��������� �������� ����������� �����
  };
  int FTxAddr;           //������������ �����
  int FTxCmd;            //��� ������������ �������
  int FTxPtr;            //��������� ������ ��������
  int FRxPtr;            //��������� ������ ������
  unsigned char RxN;     //���������� �������� ����
  AnsiString FTxCmdName; //��� ������������ �������
  AnsiString FDeviceName; //��� ����������
  void __fastcall RxByte(unsigned char &b); //����� �����
  BYTE __fastcall PortIn(WORD Address); //������ ����� (not for NT systems)
  void __fastcall ByteStuff(unsigned char b, DWORD &bptr, unsigned char *buff);
  void __fastcall ByteDeStuff(unsigned char &b);
  void __fastcall DowCRC(unsigned char b, unsigned char &crc); //���������� CRC
  void __fastcall RxFrame(unsigned char &Addr,
                          unsigned char &Cmd,
                          unsigned char &Nbt,
                          unsigned char *Data); //����� ������
  void __fastcall TxFrame(unsigned char Addr,
                          unsigned char Cmd,
                          unsigned char Nbt,
                          unsigned char *Data); //�������� ������
  unsigned char FTxData[FRAME]; //����� ��������
  unsigned char FRxData[FRAME]; //����� ������

protected:
  TCriticalSection *CmdCrSec; //����������� ������ ��� ���������� ������
  void __fastcall NewCmd(int cmd, AnsiString name); //������ ������������ ������
  void __fastcall AddByte(unsigned char b);  //���������� ����� � �����
  void __fastcall AddWord(unsigned short b); //���������� ����� � �����
  void __fastcall AddDWord(unsigned int b);  //���������� �������� ����� � �����
  void __fastcall AddData(char *d, int count); //���������� ������ � �����
  void __fastcall SendCmd(int a = 0); //�������� ������ �� ��������� ������
  unsigned char __fastcall  GetByte(void);   //������ ����� �� ������
  unsigned short __fastcall  GetWord(void);  //������ ����� �� ������
  unsigned int __fastcall  GetDWord(void);   //������ �������� ����� �� ������
  void __fastcall GetData(char *d, int count); //������ ������ �� ������
  int __fastcall GetRxN(void); //������ ���������� �������� ����

public:
  __fastcall TWakeCom();  //�����������
  __fastcall ~TWakeCom(); //����������

  int __fastcall NameToNumber(AnsiString name); //�������������� ����� � �����
  AnsiString __fastcall NumberToName(int number); //�������������� ������ � ���
  void __fastcall GetPortList(TStringList *APortBox); //��������� ������ ������
  bool __fastcall Access(int PortNumber); //�������� ����������� ����� �� ������
  bool __fastcall Access(AnsiString PortName); //�������� ����������� ����� �� �����
  void __fastcall Open(int PortNumber); //������� ���� �� ������
  void __fastcall Open(AnsiString PortName); //������� ���� �� �����
  void __fastcall GetDeviceList(TStringList *ADeviceBox); //������� ������ ����.
  void __fastcall GetNetDeviceList(AnsiString PortName,      //������� ������
                                   TStringList *ADeviceBox); //������� ����.
  void __fastcall OpenDevice(AnsiString DevName); //������� ���� �� ����� ����.
  void __fastcall Close(); //������� ����
  void __fastcall SetBaudRate(int baudrate); //��������� �������� ������
  void __fastcall SetMode(DWORD RtsMode, DWORD DtrMode); //��������� ������
  void __fastcall SetTo(DWORD RxTo, DWORD TxTo); //��������� ���������
  void __fastcall Purge(void); //������� ������ �����

  __property int Number = {read=FNumber, write=SetNumber}; //����� �����
  __property AnsiString PortName = {read=FName, write=SetName}; //��� �����
  __property int BaudRate = {read=FBaudRate, write=SetBaudRate}; //��������
  __property AnsiString DeviceName = {read=FDeviceName}; //��� �����

  AnsiString __fastcall GetInfo(int a = 0); //������ ���������� �� ����������
};

//---------------------------------------------------------------------------

#endif
