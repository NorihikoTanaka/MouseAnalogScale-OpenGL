#ifndef _POLKAMOVE_CPP_
#define _POLKAMOVE_CPP_
/*--------------------------------------------------------------
    PolkaMoveクラス
--------------------------------------------------------------*/
template<int DIV_W,int DIV_H,int PIN_N>
class PolkaMove{
public:
//変数
    /* 水玉 */
    PolkaCol<DIV_W,DIV_H> polka;
    /* コールバック */
    CallbackLoop cl;
    /* グリッド（手の上）*/
    static const int pin_n=PIN_N;
    float pin_p[PIN_N][2];
    volatile float pin_f[PIN_N];
    volatile float cur_t,cur_x,cur_y;
    /* パラメータ */
    vec3f v,o;
    float dur;
    /* 状態 */
    volatile float ptime;//処理時間
//メソッド
    void make_pattern(float w,float h,float rad_a,float rad_s,float gap,int rep){
        /* 水玉の生成 */
        polka.set_area(w/*m*/,h/*m*/);
        polka.list.clear();
        polka.pack(rad_a,rad_s,gap,rep);
        polka.make_grid();}
#ifdef _SIMELE_OBJ_CPP_
    void load_pin_list(char* name){
        /* ピン位置・強度 */
        std::vector<vec3f> tmp;
        pos_read(name,tmp);
        if(tmp.size()!=pin_n){printf("pin num error\n");exit(0);}
        for(int i=0;i<pin_n;i++){pin_p[i][0]=tmp[i][0];pin_p[i][1]=tmp[i][1];}
        for(int i=0;i<pin_n;i++){pin_f[i]=0.0f;}}
#endif
#ifdef _PINMAP_CPP_
    void load_pinmap(char* name){
        PinMap pmp;
        pmp.load(name);
        /* ピン位置・強度 */
        for(int i=0;i<pin_n;i++){
            vec3f p=pmp.position[i];
            pin_p[i][0]=p[0];pin_p[i][1]=p[1];}
        for(int i=0;i<pin_n;i++){pin_f[i]=0.0f;}}
#endif

    bool PortOpenFlag = true;
    void start_callback(void){
#ifdef USE_DEVICE
        /* 開栓は一度のみ */
        if (PortOpenFlag == true) {
            device_open();
            PortOpenFlag = false;
        }
#endif
        cl.start(&(PolkaMove::callback),(void*)this,1/*ms*/);
        wait_ms(100/*ms*/);}
    void stop_callback(void){
        cl.stop();
#ifdef USE_DEVICE
        device_close();
#endif
    }
    void present(/*vx,vy*/vec3f v_in,/*x0,v0*/vec3f o_in,
        /*duration*/float dur_in/*s*/){
        /* set parameter */
        v=v_in;o=o_in;dur=dur_in;
        /* present */
        cl.command(0);cl.wait_done();
        cl.command(1);}
private:
//コールバック
    static int callback(int cmd,void* data){
        PolkaMove* mpp=(PolkaMove*)data;
        static StopWatch sw,sw2;
        static int reset_flag=0;
        sw2.start();
        int done_flag=0;
        if(cmd==0){
            /* リセット */
            reset_flag=1;done_flag=1;}
        else if(cmd==1){
            /* 移動 */
            if(reset_flag){sw.start();reset_flag=0;}
            /* 計時 */
            sw.stop();
            float t=sw.time_us()*1e-6f;//s
            /* 移動位置 */
            vec3f p=((mpp->v)*(t))+(mpp->o);
            (mpp->cur_t)=t;
            (mpp->cur_x)=p[0];
            (mpp->cur_y)=p[1];
            /* ピン位置 */
            float (*pp)[2]=mpp->pin_p;
            volatile float* fp=mpp->pin_f;
            /* 干渉計算 */
            float f[pin_n];
            for(int i=0;i<pin_n;i++){
                if(mpp->polka.is_collision(pp[i][0]-p[0],pp[i][1]-p[1])){f[i]=1;}
                else{f[i]=0;}}
            /* デバイス出力 */
#ifdef USE_DEVICE
            mpp->device_output(f);
#endif
            for(int i=0;i<pin_n;i++){fp[i]=f[i];}
            /* ==== 未実装 ==== */
            /* 終了判定 */
            if(t>(mpp->dur)){done_flag=1;}}
        sw2.stop();
        mpp->ptime=sw2.time_us()*1e-3f;//ms
        return done_flag;}
    /* デバイス */
#ifdef USE_DEVICE
    void device_open(void);
    void device_close(void);
    void device_output(float data[PIN_N]);
#endif
};
    
/*--------------------------------------------------------------
--------------------------------------------------------------*/
#endif/*_POLKAMOVE_CPP_*/
