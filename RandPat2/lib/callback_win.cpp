#ifndef _CALLBACK_WIN_CPP_
#define _CALLBACK_WIN_CPP_
/*--------------------------------------------------------------
    タイマーコールバックループ
--------------------------------------------------------------*/
class CallbackLoop{
private:
//変数
    /* 実行管理 */
    PTP_TIMER timer;
    /* 制御 */
    static const unsigned int cmd_bit =0x3fffffff;
    static const unsigned int ack_bit =0x40000000;
    static const unsigned int done_bit=0x80000000;
    std::atomic<unsigned int>ctrl;
    /* 実行関数 */
    int (*func)(int cmd,void* data)=0;//return 1 when cmd done else 0
    void* data;
//コールバック
    static void CALLBACK callback(
        PTP_CALLBACK_INSTANCE inst,void* para,PTP_TIMER timer){
        CallbackLoop* clp=(CallbackLoop*)para;
        /* スキップ */
        /* done以降writeすることはない */
        if((clp->ctrl)&done_bit){return;}
        /* コマンド受付 */
        (clp->ctrl)|=ack_bit;
        /* 実行 */
        int c=(clp->ctrl)&cmd_bit;
        void* d=(clp->data);
        if((*(clp->func))(c,d)){(clp->ctrl)|=done_bit;};}
public:
//コンストラクタ
    CallbackLoop(){timer=0;}
    ~CallbackLoop(){if(timer!=0){stop();}}
//メソッド
    int start(int (*funcp)(int cmd,void* data),void* datap,int period=1/*ms*/){
        /* チェック */
        if(timer!=0){stop();}
        /* 関数とデータ */
        func=funcp;
        data=datap;
        /* タイマー生成 */
        timer=CreateThreadpoolTimer(&(CallbackLoop::callback),(void*)this,0);
        if(timer==0){printf("create thread pool timer error\n");return 1;}
        /* タイマー設定 */
        FILETIME ftime;
        ftime.dwLowDateTime=0;
        ftime.dwHighDateTime=0;
        SetThreadpoolTimer(timer,&ftime,period,0/*delay*/);
        return 0;}
    void stop(void){
        /* チェック */
        if(timer==0){return;}
        /* タイマー終了 */
        CloseThreadpoolTimer(timer);
        timer=0;}
    void command(int cmd_in){
        /* コマンド */
        /* ctrlに書き込んだあとdoneまでwriteすることはない */
        ctrl=(cmd_in&cmd_bit);}
    void wait_ack(void){
        /* 終了待ち */
        for(;;){Sleep(100/*ms*/);if(ctrl&ack_bit){break;}}}
    void wait_done(void){
        /* 終了待ち */
        for(;;){Sleep(100/*ms*/);if(ctrl&done_bit){break;}}}
    int is_ack(void){return (ctrl&ack_bit);}
    int is_done(void){return (ctrl&done_bit);}
};
/*--------------------------------------------------------------
    main
--------------------------------------------------------------*/
#if 0
CallbackLoop cl;
int data;
        
int test_func(int cmd,void* dat){
    static int count=0;
    printf("%d %d %d\n",cmd,count,*(int*)dat);
    switch(cmd){
    case 0: count=0; return 1;
    case 1: count++;if(count>=5){return 1;}else{return 0;}
    default: return 1;}}

int main(void){
    cl.start(test_func,(void*)&data,100/*ms*/);
    data=10; cl.command(0);cl.wait_done();
    data=20; cl.command(1);cl.wait_done();
    cl.stop();
}
#endif
/*--------------------------------------------------------------
--------------------------------------------------------------*/
#endif/*_CALLBACK_WIN_CPP_*/
