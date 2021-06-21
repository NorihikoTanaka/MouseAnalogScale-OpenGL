#ifndef _RS232_CPP_
#define _RS232_CPP_

/*--------------------------------------------------------------
    Windows Version
--------------------------------------------------------------*/
#if defined(_WIN32)||defined(_WIN64)
/* Windows */
#include<windows.h>

#define RS232_19200 CBR_19200
#define RS232_38400 CBR_38400
#define RS232_115200 CBR_115200

class RS232{
    HANDLE hCom;
public:
//コンストラクタ
    RS232(){hCom=INVALID_HANDLE_VALUE;}
    ~RS232(){
        if(hCom!=INVALID_HANDLE_VALUE){close();}
        hCom=INVALID_HANDLE_VALUE;}
    //メソッド
    int open(const char *dev,int brate=RS232_38400){
        /* 既に開かれていれば閉じる */
        if(hCom!=INVALID_HANDLE_VALUE){close();}
        /* ポートのオープン */
        hCom=CreateFile(dev,GENERIC_READ|GENERIC_WRITE,0,
            NULL,OPEN_EXISTING,0,NULL);
        if(hCom==INVALID_HANDLE_VALUE){
            printf("port open err\n");return(-1);}
        /* ポートの設定(読み出し) */
        DCB dcb;
        dcb.DCBlength = sizeof(DCB);
        GetCommState(hCom, &dcb);
        /* ポートの設定(変更) */
        dcb.BaudRate=brate;
        dcb.fBinary = TRUE;
        dcb.fParity = TRUE;
        dcb.fOutxCtsFlow = FALSE;
        dcb.fOutxDsrFlow = FALSE;
        dcb.fDtrControl = DTR_CONTROL_ENABLE;
        /*dcb.fDsrSensitivity;*/
        /*dcb.fTXContinueOnXoff;*/
        dcb.fOutX = FALSE;
        dcb.fInX = FALSE;
        dcb.fErrorChar = 0;
        dcb.fNull = FALSE;
        dcb.fRtsControl = RTS_CONTROL_ENABLE;
        dcb.fAbortOnError = FALSE;
        /* dcb.fDummy2; */
        dcb.wReserved = 0;
        dcb.XonLim = 100;
        dcb.XoffLim = 100;
        dcb.ByteSize = 8;
        dcb.Parity = NOPARITY;
        dcb.StopBits = ONESTOPBIT;
        /* dcb.XonChar      = ASCII_XON; */
        /* dcb.XoffChar     = ASCII_XOFF; */
        dcb.ErrorChar = 0;
        dcb.EvtChar = 0;
        /* dcb.wReserved; */
        /* ポートの設定(書き込み) */
        if (SetCommState(hCom, &dcb) == 0){
            printf("set comm state err\n");exit(0);}
        /* タイムアウトの設定 */
        COMMTIMEOUTS timeouts;
        timeouts.ReadIntervalTimeout = 100;
        timeouts.ReadTotalTimeoutMultiplier = 0;
        timeouts.ReadTotalTimeoutConstant = 100;
        timeouts.WriteTotalTimeoutMultiplier = 0;
        timeouts.WriteTotalTimeoutConstant = 100;
        /* タイムアウト設定の書き込み */
        if (SetCommTimeouts(hCom, &timeouts) == FALSE){
            printf("set comm state err\n");exit(0);}
        return(0);}
    void write(const char *buf, unsigned long size){
        unsigned long wsize;
        /* データの書き出し */
        if(WriteFile(hCom,buf,size,&wsize,NULL)==0){
            printf("write err\n");exit(0);}
        if (wsize!=size){printf("write size err\n");}}
    void read(char *buf, unsigned long size){
        unsigned long rsize;
        if(ReadFile(hCom,buf,size,&rsize,NULL)==0){
            printf("read err\n"); exit(0);}
        if (rsize!=size){printf("read size err\n"); }}
    void mode(int min,int time){
        /* dummy */}
    void flush(int iomode){//dummy
        FlushFileBuffers(hCom);}
    void close(void){
        if(hCom==INVALID_HANDLE_VALUE){return;}
        if(CloseHandle(hCom)==0){
            printf("close err\n");exit(0);}
        hCom=INVALID_HANDLE_VALUE;}
    static void sleep_ms(int ms){Sleep(ms);}
};

/*--------------------------------------------------------------
    Linux Version
--------------------------------------------------------------*/
#else
/* Linux */
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<termios.h>
#include<stdio.h>
#include<unistd.h>

#define RS232_19200 B19200
#define RS232_38400 B38400
#define RS232_115200 B115200

class RS232{
    /* file handle */
    int fd;
public:
//データ
//コンストラクタ
    RS232(){fd=(-1);}
    ~RS232(){if(fd>=0){close();}fd=(-1);}
//メソッド
    int open(const char *dev,int brate=RS232_38400){
        /* 既に開かれていれば閉じる */
        if(fd>=0){close();}
        /* ポートのオープン */
        fd=::open(dev,O_RDWR);
    	if(fd<0){printf("rs232 open error\n");return(-1);}
        /* 現在のポートの設定取得 */
        struct termios status;
        tcgetattr(fd,&status);
        /* 新しい設定 */
        status.c_cflag=CS8|CREAD|CLOCAL;  //no CRTSCTS, no CSTOPB
        status.c_iflag=IGNBRK|IGNPAR;     //no IXON, noIXOFF
        status.c_lflag=0;                 //no ICANON
        status.c_oflag=0;               
        /* スピード */
        cfsetispeed(&status,brate);
        /* とりあえずブロッキングモード */
        status.c_cc[VMIN]=1;
        status.c_cc[VTIME]=0;
        /* Set these new parameters. */
        tcsetattr(fd,TCSANOW,&status);
        /* 終了 */
        tcflush(fd,2);
        return fd;}
    void write(const char *buf,unsigned long size){
        /* データの書き出し */
        if(::write(fd,buf,size)!=size){
            printf("write err\n");exit(0);}}
    void read(char *buf,unsigned long size){
        int size_r=0;
        if((size_r=::read(fd,buf,size))!=size){
            printf(" read err, size_r = %d\n",size_r);}}
    void mode(int min,int time){
        struct termios status;
        tcgetattr(fd,&status);
        status.c_cc[VMIN] =min;  /*chars*/
        status.c_cc[VTIME]=time; /*x0.1s*/
        tcsetattr(fd,TCSANOW,&status);}
    void flush(int iomode){//0:in,1:out,2:in/out
        tcflush(fd,iomode);}
    void close(void){
        tcflush(fd,2);
        ::close(fd);fd=(-1);}
    static void sleep_ms(int ms){usleep(ms*1000);}
};

#endif
/*--------------------------------------------------------------
--------------------------------------------------------------*/
#endif/*_RS232_CPP_*/
