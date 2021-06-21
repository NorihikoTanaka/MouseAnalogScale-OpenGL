#ifndef _COMIF_CPP_
#define _COMIF_CPP_

/*--------------------------------------------------------------
    Command Interface
--------------------------------------------------------------*/
class ComIF{
private:
//Object
    RS232 rs232;
    /* constant */
    static const unsigned char HEADER=0xff;
    const unsigned char COM_W1=0xf8;
    const unsigned char COM_WN=0xf9;
    const unsigned char COM_R1=0xfa;
    const unsigned char COM_RN=0xfb;
    const unsigned char ACK_D1=0xfc;
    const unsigned char ACK_DN=0xfd;
    /* common address */
    const unsigned short SYS_ID=0x0000;//read only
//Method
    /* private method */
    int write_op_1(unsigned char com,unsigned short addr,
        unsigned short data);
    int write_op_n(unsigned char com,unsigned short addr,
        unsigned short size,unsigned short* data);
    int read_op_1(unsigned char ack,unsigned short addr,
        unsigned short* data);
    int read_op_n(unsigned char ack,unsigned short addr,
        unsigned short size,unsigned short* data);
public:
    /* open/close */
    void open(const char* port){rs232.open(port);}
    void close(void){rs232.close();}
    /* public method (original) */
    int write_1(unsigned short addr,unsigned short data);
    int write_n(unsigned short addr,
        unsigned short size,unsigned short* data);
    int read_1(unsigned short addr,unsigned short* data);
    int read_n(unsigned short addr,
        unsigned short size,unsigned short* data);
    /* specific method */
    int read_device_id(unsigned short* data);
    /* functional test */
private:
    int write_op_1_NG(int type);
    int write_op_n_NG(int type);
public:
    int write_1_NG(int type);
    int write_n_NG(int type);
    /* listup functions (class function) */
#if defined(_WIN32)||defined(_WIN64)
    static const char *target_prop[];
    static const int com_name_length=128;
    typedef char com_name_type[com_name_length];
    typedef struct{
        unsigned short dev_type;
        unsigned short dev_no;
        com_name_type com_name;} devlist;
    static int list_port(com_name_type comp[],int nmax,int *ncomp);
    static int list_device(std::vector<devlist>& list);
#endif
};

/*------------------------------------------------------------*/
/* private method */
int ComIF::write_op_1(unsigned char com,unsigned short addr,
    unsigned short data){
    unsigned char buf[8];
    unsigned short *bufs=(unsigned short *)buf;
    // body
    buf[0]=HEADER;
    buf[1]=com;
    bufs[1]=addr;
    bufs[2]=data;
    // csum
    unsigned char csum=0;
    int n=6;
    for(int i=1;i<n;i++){csum+=buf[i];}
    buf[n]=csum;
    // send
    rs232.write((char*)buf,n+1);
    return 0;}
int ComIF::write_op_n(unsigned char com,unsigned short addr,
    unsigned short size,unsigned short* data){
    unsigned char buf[1024+8];
    unsigned short *bufs=(unsigned short *)buf;
    // check size
    if(size>512){printf("size>512 error\n");return (-5);}
    // body
    buf[0]=HEADER;
    buf[1]=com;
    bufs[1]=addr;
    bufs[2]=size;
    for(int i=0;i<(int)size;i++){bufs[3+i]=data[i];}
    // csum
    unsigned char csum=0;
    int n=6+((int)size)*2;
    for(int i=1;i<n;i++){csum+=buf[i];}
    buf[n]=csum;
    // send
    rs232.write((char*)buf,n+1);
    return 0;}
int ComIF::read_op_1(unsigned char ack,unsigned short addr,
    unsigned short* data){
    unsigned char buf[8];
    unsigned short *bufs=(unsigned short *)buf;
    // read
    int n=6;
    rs232.read((char*)buf,n+1);
#ifdef DEBUG_PRINT
    printf("R1: h %02x a %02x A %04x D %04x c %02x\n",
           buf[0],buf[1],bufs[1],bufs[2],buf[n]);
#endif
    // check header
    if(buf[0]!=HEADER){printf("header error (%02x)\n",buf[0]);return (-1);}
    // check ack
    if(buf[1]!=ack){printf("ack error (%02x)\n",buf[1]);return (-2);}
    // check addr
    if(bufs[1]!=addr){printf("addr error\n");return (-3);}
    // check csum
    unsigned char csum=0;
    for(int i=1;i<n;i++){csum+=buf[i];}
    if(buf[n]!=csum){printf("csum error\n");return (-4);}
    // data
    (*data)=bufs[2];
    return 0;}
int ComIF::read_op_n(unsigned char ack,unsigned short addr,
    unsigned short size,unsigned short* data){
    unsigned char buf[1024+8];
    unsigned short *bufs=(unsigned short *)buf;
    // check size
    if(size>512){printf("size>512 error\n");return (-5);}
    int n=6+((int)size)*2;
    rs232.read((char*)buf,n+1);
#ifdef DEBUG_PRINT
    printf("Rn: h %02x a %02x A %04x S %04x\n",
        buf[0],buf[1],bufs[1],bufs[2]);
    for(int i=0;i<(int)size;i++){
        printf("%04x ",bufs[3+i]);}
    printf("\n");
    printf("c %02x\n",buf[n]);
#endif
    // check header
    if(buf[0]!=HEADER){printf("header error\n");return (-1);}
    // check ack
    if(buf[1]!=ack){printf("ack error\n");return (-2);}
    // check addr
    if(bufs[1]!=addr){printf("addr error\n");return (-3);}
    // check csum
    unsigned char csum=0;
    for(int i=1;i<n;i++){csum+=buf[i];}
    if(buf[n]!=csum){printf("csum error\n");return (-4);}
    // data
    for(int i=0;i<(int)size;i++){data[i]=bufs[3+i];}
    return 0;}

/*------------------------------------------------------------*/
/* public method (original) */
int ComIF::write_1(unsigned short addr,unsigned short data){
    int ret;
    ret=write_op_1(COM_W1,addr,data);
    return ret;}
int ComIF::write_n(unsigned short addr,
    unsigned short size,unsigned short* data){
    int ret;
    ret=write_op_n(COM_WN,addr,size,data);
    return ret;}
int ComIF::read_1(unsigned short addr,unsigned short* data){
    int ret;
    ret=write_op_1(COM_R1,addr,0);
    if(ret<0){return ret;}
    ret=read_op_1(ACK_D1,addr,data);
    return ret;}
int ComIF::read_n(unsigned short addr,
    unsigned short size,unsigned short* data){
    int ret;
    ret=write_op_1(COM_RN,addr,size);
    if(ret<0){return ret;}
    ret=read_op_n(ACK_DN,addr,size,data);
    return ret;}
int ComIF::read_device_id(unsigned short* data){
    return read_1(SYS_ID,data);}

/*------------------------------------------------------------*/
/* functional test */
/* private */
int ComIF::write_op_1_NG(int type){
    unsigned char buf[8];
    unsigned short *bufs=(unsigned short *)buf;
    // body
    buf[0]=HEADER;
    buf[1]=COM_W1;
    bufs[1]=0x0004;//address
    bufs[2]=0x5555;//data
    // csum
    unsigned char csum=0;
    int n=6;
    for(int i=1;i<n;i++){csum+=buf[i];}
    buf[n]=csum;
    // error
    switch(type){
    case 1:buf[0]+=1;break; //header
    case 2:buf[1]=0; break; //command
    case 3:buf[n]+=1;break; //csum
    default: break;}
    // send
    rs232.write((char*)buf,n+1);
    return 0;}
int ComIF::write_op_n_NG(int type){
    unsigned char buf[1024+8];
    unsigned short *bufs=(unsigned short *)buf;
    // body
    buf[0]=HEADER;
    buf[1]=COM_WN;
    bufs[1]=0x0004; //address
    bufs[2]=0x0002; //size
    bufs[3+0]=0x5555;
    bufs[3+1]=0xaaaa;
    // csum
    unsigned char csum=0;
    int n=6+(2*2);
    for(int i=1;i<n;i++){csum+=buf[i];}
    buf[n]=csum;
    // error
    switch(type){
    case 1:buf[0]+=1;break; //header
    case 2:buf[1]=0; break; //command
    case 3:buf[n]+=1;break; //csum
    default: break;}
    // send
    rs232.write((char*)buf,n+1);
    return 0;}
/* public */
int ComIF::write_1_NG(int type){
    int ret;
    ret=write_op_1_NG(type);
    return ret;}
int ComIF::write_n_NG(int type){
    int ret;
    ret=write_op_n_NG(type);
    return ret;}

/*------------------------------------------------------------*/
#if defined(_WIN32)||defined(_WIN64)
#include <string.h>
#include <windows.h>
#include <setupapi.h>
#pragma comment(lib,"setupapi.lib")

/* property list */
const char* ComIF::target_prop[]={
    "FTDIBUS\\COMPORT&VID_0403&PID_6010",
    "FTDIBUS\\COMPORT&VID_0F87&PID_1046",
    /* add target device property here */
    0};
/* list up port */
int ComIF::list_port(com_name_type comp[],int nmax,int *ncomp){
    /* device info. */
    HDEVINFO hDevInfo;
    hDevInfo=SetupDiGetClassDevs(
        &GUID_DEVINTERFACE_COMPORT,0,0,DIGCF_PRESENT|DIGCF_DEVICEINTERFACE);
    if(hDevInfo==0){printf("dev info error\n");return(-1);}
    /* get device info */
    SP_DEVINFO_DATA Data;
    Data.cbSize=sizeof(Data);
    /* for each device */
    int nport=0,nair=0;
    while(SetupDiEnumDeviceInfo(hDevInfo,nport,&Data)){
        /* get COM port */
        HKEY key=SetupDiOpenDevRegKey(
            hDevInfo,&Data,DICS_FLAG_GLOBAL,0,DIREG_DEV,KEY_QUERY_VALUE);
        /* name and property */
        if(key){
            DWORD dtype,dsize;
            /* port name */
            char name[512];
            dsize=sizeof(name);
            if(RegQueryValueEx(key,"PortName",NULL,&dtype,(LPBYTE)name,&dsize)==
               ERROR_FILE_NOT_FOUND){continue;}
            RegCloseKey(key);
            /* device property */
            char prop[512];
            dsize=sizeof(prop);
            if(!SetupDiGetDeviceRegistryProperty(hDevInfo,&Data,SPDRP_HARDWAREID,
                &dtype,(PBYTE)prop,dsize,&dsize)){continue;}
            /* display */
            printf("%d %s %s \n",nport,name,prop);
            /* matching property */
            for(int i=0;;i++){
                /* is end of list? */
                if(target_prop[i]==0){break;}
                /* match! */
                if(strncmp(prop,target_prop[i],512)==0){
                    /* copy name */
                    strncpy_s(comp[nair],name,com_name_length);nair++;
                    break;}}
            /* check list capacity */
            if(nair>=nmax){break;}
        }
        /* next port */
        nport++;
    }//device info
    /* release device info */
    SetupDiDestroyDeviceInfoList(hDevInfo);
    /* num. of ports */
    (*ncomp)=nair;return 0;}
/* device list */
int ComIF::list_device(std::vector<devlist>& list){
    /* clear */
    list.clear();
    /* list up */
    const int comp_max=10;
    com_name_type comp[comp_max];
    int ncomp;
    if(ComIF::list_port(comp,comp_max,&ncomp)!=0){
        printf("list com port error\n");exit(0);}
    /* get type and id */
    ComIF ci;
    for(int i=0;i<ncomp;i++){
        unsigned short data;
        ci.open(comp[i]);
        ci.read_device_id(&data);
        ci.close();
        devlist tmp;
        tmp.dev_type=data>>4;
        tmp.dev_no=data&0x0f;
        strncpy_s(tmp.com_name,comp[i],sizeof(tmp.com_name));
        list.push_back(tmp);}
    return ncomp;}

#endif
/*--------------------------------------------------------------
--------------------------------------------------------------*/
#endif/*_COMIF_CPP_*/
