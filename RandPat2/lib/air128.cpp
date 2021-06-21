#ifndef _AIR128_CPP_
#define _AIR128_CPP_
/*--------------------------------------------------------------
    Air128
--------------------------------------------------------------*/
class Air128{
public:
//Object
    ComIF comif;
    const unsigned short a_sys_id  =0x0000; //r r
    const unsigned short a_sys_err =0x0001; //r/w r:err,w:clear 
    const unsigned short a_sys_drv =0x0002; //r/w
    const unsigned short a_sys_opt =0x0003; //r/w 0:id,1:err
    const unsigned short a_ad_conv =0x0010; //r   0x0010-0x0011
    const unsigned short a_da_conv =0x0100; //r/w 0x0100-0x017f
    const unsigned short a_da_tmr  =0x0180; //r/w msec
    const unsigned short a_mem     =0x0400; //r/w 0x0400-0x07ff
    unsigned short legacy_map[128];
//Constructor
    Air128(){make_legacy_map();}
    ~Air128(){}
//Method
    /* open/close */
    void open(const char* port){comif.open(port);}
    void close(void){comif.close();}
    /* system */
    unsigned short sys_get_id(void);
    unsigned short sys_get_err(void);
    void sys_clear_err(void);
    void sys_set_valve(unsigned short val);
    void sys_get_valve(unsigned short *val);
    void sys_set_option(unsigned short m);//
    void sys_get_option(unsigned short *m);
    /* ad input */
    void ad_get_s2(unsigned short daval[2]);
    void ad_get_v2(float volt[2]);
    void ad_get_p2(float prs[2]);
    /* da output 1 */
    void da_set_s1(unsigned short ch,unsigned short daval);
    void da_get_s1(unsigned short ch,unsigned short *daval);
    void da_set_v1(unsigned short addr,float volt);
    void da_get_v1(unsigned short addr,float *volt);
    void da_set_p1(unsigned short ch,float prs);
    void da_get_p1(unsigned short ch,float prs[1]);
    /* da output 128 */
    void da_set_s128(unsigned short daval[128]);
    void da_get_s128(unsigned short daval[128]);
    void da_set_v128(float volt[128]);
    void da_get_v128(float volt[128]);
    void da_set_p128(float prs[128]);
    void da_get_p128(float prs[128]);
    /* da timer */
    void da_set_timer(unsigned short t/*ms*/);
    void da_get_timer(unsigned short *t/*ms*/);
    /* da conversion (LTC1660/VY1B00) */
    unsigned short da_volt_to_daval(float v/*V*/);
    float da_press_to_volt(float p/*MPa*/);
    float da_daval_to_volt(unsigned short daval);
    float da_volt_to_press(float v/*V*/);
    /* ad conversion (LTC1298/PSE540) */
    unsigned short ad_volt_to_adval(float v/*V*/);
    float ad_press_to_volt(float p/*MPa*/);
    float ad_adval_to_volt(unsigned short adval);
    float ad_volt_to_press(float v/*V*/);
    /* error text */
    void sys_err_print(unsigned short err);
    /* legacy support */
private:
    unsigned short map_legacy(unsigned short leg);
    void make_legacy_map(void);
public:
    void da_set_s128_l(unsigned short daval[128]);
    void da_get_s128_l(unsigned short daval[128]);
    void da_set_v128_l(float volt[128]);
    void da_get_v128_l(float volt[128]);
    void da_set_p128_l(float prs[128]);
    void da_get_p128_l(float prs[128]);
};

/*------------------------------------------------------------*/
/* system */
unsigned short Air128::sys_get_id(void){unsigned short d[1];
    comif.read_1(a_sys_id,d); return d[0];}
unsigned short Air128::sys_get_err(void){unsigned short d[1];
    comif.read_1(a_sys_err,d); return d[0];}
void Air128::sys_clear_err(void){
    comif.write_1(a_sys_err,0);}
void Air128::sys_set_valve(unsigned short val){
    comif.write_1(a_sys_drv,val);}
void Air128::sys_get_valve(unsigned short *val){
    comif.read_1(a_sys_drv,val);}
void Air128::sys_set_option(unsigned short m){
    comif.write_1(a_sys_opt,m);}
void Air128::sys_get_option(unsigned short *m){
    comif.read_1(a_sys_opt,m);}

/*------------------------------------------------------------*/
/* ad input */
void Air128::ad_get_s2(unsigned short adval[2]){
    comif.read_n(a_ad_conv,2,adval);}
void Air128::ad_get_v2(float volt[2]){
    unsigned short adval[2];
    ad_get_s2(adval);
    volt[0]=ad_adval_to_volt(adval[0]);
    volt[1]=ad_adval_to_volt(adval[1]);}
void Air128::ad_get_p2(float prs[2]){
    float volt[2];
    ad_get_v2(volt);
    prs[0]=ad_volt_to_press(volt[0]);
    prs[1]=ad_volt_to_press(volt[1]);}

/*------------------------------------------------------------*/
/* da output 1 */
void Air128::da_set_s1(unsigned short ch,unsigned short daval){
    comif.write_1(a_da_conv+ch,daval);}
void Air128::da_get_s1(unsigned short ch,unsigned short *daval){
    comif.read_1(a_da_conv+ch,daval);}
void Air128::da_set_v1(unsigned short ch,float volt){
    unsigned short daval;
    daval=da_volt_to_daval(volt);
    da_set_s1(ch,daval);}
void Air128::da_get_v1(unsigned short ch,float *volt){
    unsigned short daval;
    da_get_s1(ch,&daval);
    (*volt)=da_daval_to_volt(daval);}
void Air128::da_set_p1(unsigned short ch,float prs){
    float volt=(prs<=0)?0:da_press_to_volt(prs);
    da_set_v1(ch,volt);}
void Air128::da_get_p1(unsigned short ch,float *prs){
    float volt;
    da_get_v1(ch,&volt);
    (*prs)=da_volt_to_press(volt);}

/* da output 128 */
void Air128::da_set_s128(unsigned short daval[128]){
    comif.write_n(a_da_conv,128,daval);}
void Air128::da_get_s128(unsigned short daval[128]){
    comif.read_n(a_da_conv,128,daval);}
void Air128::da_set_v128(float volt[128]){
    unsigned short daval[128];
    for(int i=0;i<128;i++){
        daval[i]=da_volt_to_daval(volt[i]);}
    da_set_s128(daval);}
void Air128::da_get_v128(float volt[128]){
    unsigned short daval[128];
    da_set_s128(daval);
    for(int i=0;i<128;i++){
        volt[i]=da_daval_to_volt(daval[i]);}}
void Air128::da_set_p128(float prs[128]){
    float volt[128];
    for(int i=0;i<128;i++){
        volt[i]=(prs[i]<=0)?0:da_press_to_volt(prs[i]);}
    da_set_v128(volt);}
void Air128::da_get_p128(float prs[128]){
    float volt[128];
    da_get_v128(volt);
    for(int i=0;i<128;i++){
        prs[i]=da_volt_to_press(volt[i]);}}

/* da timer */
void Air128::da_set_timer(unsigned short t/*ms*/){
    comif.write_1(a_da_tmr,t);}
void Air128::da_get_timer(unsigned short *t/*ms*/){
    comif.read_1(a_da_tmr,t);}

/*------------------------------------------------------------*/
/* da conversion (LTC1660/VY1B00) */
unsigned short Air128::da_volt_to_daval(float v/*V*/){
    int d=(int)((v/5)*0xffff);
    if(d>0xffff){d=0xffff;}
    if(d<0     ){d=0;}
    return (unsigned short)d;}
float Air128::da_press_to_volt(float p/*MPa*/){
    /* 0.0MPa@1V, 0.9MPa@5V */
    float v=p*(5-1)/(0.9f-0.0f)+1;
    return v;}
float Air128::da_daval_to_volt(unsigned short daval){
    return 5.0f*(float)daval/(float)0xffff;}
float Air128::da_volt_to_press(float v/*V*/){
    /* 0.0MPa@1V, 0.9MPa@5V */
    return (v-1)*(0.9f-0.0f)/(5-1);}

/* ad conversion (LTC1298/PSE540) */
unsigned short Air128::ad_volt_to_adval(float v/*V*/){
    int d=(int)((v/5)*0xffff);
    if(d>0xffff){d=0xffff;}
    if(d<0     ){d=0;}
    return (unsigned short)d;}
float Air128::ad_press_to_volt(float p/*MPa*/){
    /* 1V@0.0MPa, 5V@1.0MPa */
    float v=p*(5-1)/(1.0f-0.0f)+1;
    return v;}
float Air128::ad_adval_to_volt(unsigned short adval){
    return 5.0f*(float)adval/(float)0xffff;}
float Air128::ad_volt_to_press(float v/*V*/){
    /* 1V@0.0MPa, 5V@1.0MPa */
    return (v-1)*(1.0f-0.0f)/(5-1);}

/*------------------------------------------------------------*/
/* error text */
void Air128::sys_err_print(unsigned short err){
    if(err==0){printf("(no error)\n");return;}
    if(err&0x0020){printf("fifo ");}
    if(err&0x0010){printf("state ");}
    if(err&0x0008){printf("csum ");}
    if(err&0x0004){printf("ack ");}
    if(err&0x0002){printf("cmnd ");}
    if(err&0x0001){printf("head ");}
    printf("\n");}

/*------------------------------------------------------------*/
/* legacy support */
unsigned short Air128::map_legacy(unsigned short leg){
    return
        ((leg<<3)&0x0018)+    //00000ba->00ba000
        ((leg>>2)&0x0003)+    //000dc00->00000dc
        ((leg<<1)&0x0060)+    //0fe0000->fe00000
        ((leg>>4)&0x0004);}   //g000000->0000g00
void Air128::make_legacy_map(void){
    for(unsigned short i=0;i<128;i++){
        legacy_map[i]=map_legacy(i);}}
void Air128::da_set_s128_l(unsigned short daval[128]){
    unsigned short daleg[128];
    for(int i=0;i<128;i++){daleg[legacy_map[i]]=daval[i];}
    da_set_s128(daleg);}
void Air128::da_get_s128_l(unsigned short daval[128]){
    unsigned short daleg[128];
    da_get_s128(daleg);
    for(int i=0;i<128;i++){daval[i]=daleg[legacy_map[i]];}}
void Air128::da_set_v128_l(float volt[128]){
    float vleg[128];
    for(int i=0;i<128;i++){vleg[legacy_map[i]]=volt[i];}
    da_set_v128(vleg);}
void Air128::da_get_v128_l(float volt[128]){
    float vleg[128];
    da_get_v128(vleg);
    for(int i=0;i<128;i++){volt[i]=vleg[legacy_map[i]];}}
void Air128::da_set_p128_l(float prs[128]){
    float pleg[128];
    for(int i=0;i<128;i++){pleg[legacy_map[i]]=prs[i];}
    da_set_p128(pleg);}
void Air128::da_get_p128_l(float prs[128]){
    float pleg[128];
    da_get_p128(pleg);
    for(int i=0;i<128;i++){prs[i]=pleg[legacy_map[i]];}}

/*--------------------------------------------------------------
    DA128 emulation
--------------------------------------------------------------*/
class DA128:private Air128{
public:
//Object
    RS232 rs232;
//Method
    /* open/close */
    void open(const char* port){
        Air128::open(port);
        Air128::sys_set_valve(1);}
    void close(void){Air128::close();}
    /* conversion */
    short volt_to_daval(float v/*V*/){
        return Air128::da_volt_to_daval(v);}
    float press_to_volt(float p/*MPa*/){
        return Air128::da_press_to_volt(p);}
    /* data transmission */
    void set_v128(float volt[128]){
        Air128::da_set_v128_l(volt);}
    void set_p128(float prs[128]){
        Air128::da_set_p128_l(prs);}
    void set_v1(short addr,float volt){
        unsigned short a;
        a=Air128::legacy_map[(unsigned short)addr];
        Air128::da_set_v1(a,volt);}
    void set_p1(short addr,float prs){
        unsigned short a;
        a=Air128::legacy_map[(unsigned short)addr];
        Air128::da_set_v1(a,prs);}
    /* commmand */
    int get_id(void){
        return (int)Air128::sys_get_id();}
    void set_timer(float tmr/*s*/){
        da_set_timer((unsigned short)(tmr*1000));}
};

/*--------------------------------------------------------------
--------------------------------------------------------------*/
#endif/*_AIR128_CPP_*/
