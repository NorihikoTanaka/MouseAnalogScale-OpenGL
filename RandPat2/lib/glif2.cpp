#ifndef _GLIF2_CPP_
#define _GLIF2_CPP_

/*--------------------------------------------------------------
履歴：
2019.5.20 USE_GLUIのフラグを導入
2019.5.21 USE_3DXMOUSEのフラグを導入
--------------------------------------------------------------*/
/* OpenGL */
#include<GL/glew.h>
#include<GL/freeglut.h>

/* Windows */
#if defined(_WIN32)||defined(_WIN64)
/* (common) */
#undef XWINDOW_ENV
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"freeglut.lib")
/* (glui) */
#ifdef USE_GLUI
#include<GL/glui.h>
#pragma comment(lib,"glui32.lib")
#endif
/* (3dx) */
#ifdef USE_3DXMOUSE
#include<si.h>
#include<siapp.h>
#pragma comment(lib,"siapp.lib")
#endif
/* Linux */
#else
/* (common) */
#define XWINDOW_ENV
#include<X11/Xlib.h>
#include<X11/Xutil.h>
#include<X11/Xos.h>
#include<X11/Xatom.h>
#include<X11/keysym.h>
#include<GL/glx.h>
/* (glui) */
#ifdef USE_GLUI
#include<GL/glui.h>
#endif
/* (3dx) */
#ifdef USE_3DXMOUSE
#include"3dx/xdrvlib.h"
#endif
#endif

/*--------------------------------------------------------------
    3Dマウス（X11）
--------------------------------------------------------------*/
#if defined(XWINDOW_ENV) && defined(USE_3DXMOUSE)
/* window情報 */
Window window;
Display* display;

/* X11 補助関数 */
Window getWindowIdFromNameRec(
    Display *dpy, Window level,const char *win_name){
    /* このレベルのウィンドウの検索 */
    Window root,parent,*children;
    unsigned int num;
    XQueryTree(dpy,level,&root,&parent,&children,&num);
    for(int i=0;i<num;i++){
        char *name;
        /* fetchできたら、名前をチェック */
        if(XFetchName(dpy,children[i],&name)){
            //printf("%d:[%s]\n",children[i],name);
            if(strcmp(name,win_name)){XFree(name);}
            else{XFree(name);return children[i];}}
        /* fetchできなかったら、下の階層に */
        else{
            Window id;
            id=getWindowIdFromNameRec(dpy,children[i],win_name);
            if(id!=0){return id;}}}
    return 0;}
Window getWindowIdFromName(const char *win_name){
    Display* display=glXGetCurrentDisplay();
    Window root=DefaultRootWindow(display);
    return getWindowIdFromNameRec(display,root,win_name);}
/* 3dx mouse */
void si_open(const char* name){
    /* ★Xの情報取得★ */
    window=getWindowIdFromName(name);
    display=glXGetCurrentDisplay();
    printf("id=%d\n",window);
    if(!MagellanInit(display,window)){
        printf("no driver is running\n" );exit(0);}}
void si_close(void){
    MagellanClose(display);}
void si_getevent(float motion[6]){
    /* motionのクリア */
    for(int i=0;i<6;i++){motion[i]=0.0f;}
    /* 3dx関係の処理 */
    for(;;){
        /* イベント解釈 */
        XEvent event;
        if(!XCheckTypedEvent(display,ClientMessage,&event)){break;}
        if(event.type!=ClientMessage){
            printf("not client message event\n");break;}
        /* Magellanイベント取得 */
        MagellanFloatEvent MagellanEvent;
        int MagellanEventType=MagellanTranslateEvent(
            display,&event,&MagellanEvent,1.0,1.0);
        /* motion eventのみ扱う */
        if(MagellanEventType!=MagellanInputMotionEvent){continue;}
        /* eventの解釈 */
        MagellanRemoveMotionEvents(display);
        float *mag=motion;
        for(int i=0;i<6;i++){
            mag[i]=MagellanEvent.MagellanData[i];}
        //printf("%f %f %f %f %f %f\n",
        //    mag[0],mag[1],mag[2],mag[3],mag[4],mag[5]);
    }}
/* 表示 */
void si_print_data(float data[6]){
    printf("%f %f %f \n", data[0], data[1], data[2]);
    printf("%f %f %f \n", data[3], data[4], data[5]);}
#endif

/*--------------------------------------------------------------
    3Dマウス（Windows）
--------------------------------------------------------------*/
#if !defined(XWINDOW_ENV) && defined(USE_3DXMOUSE)
HWND hWnd;
SiHdl hsm;
int si_open(const char* name){
    /* GLコンテキスト */
    HDC glDc=wglGetCurrentDC();
    /* ウィンドウ */
    hWnd=WindowFromDC(glDc);
    /* Si初期化 */
    if(SiInitialize()==SPW_ERROR){
        printf("si init error\n");return(-1);}
    /* 開く */
    SiOpenData odata;
    SiOpenWinInit(&odata,hWnd);
    if((hsm=SiOpen(name,
        SI_ANY_DEVICE,SI_NO_MASK,SI_EVENT,&odata))==NULL){
        printf("si open error\n");return (-1);}
    printf("sm open\n");return 0;}
void si_close(void){
    /* 閉じる */
    SiTerminate();
    printf("sm close\n");}
int si_getevent_i(int data[6], int itvl[1]){
    int flag = 0; //set to 1 when received
    for (;;){
        /* メッセージ到着確認 */
        /* 0xc000以降の全てのメッセージ */
        MSG msg;
        if(PeekMessage(&msg,hWnd,0xc000,0xffff,PM_REMOVE)==0){break;}
        if(msg.message==WM_QUIT){
            printf("wm quit\n");return 1;}
        //printf("message=%x\n",msg.message);
        /* Siイベントの取得 */
        SiGetEventData edata;
        SiGetEventWinInit(&edata,msg.message,msg.wParam,msg.lParam);
        SiSpwEvent sievent;
        if(SiGetEvent(hsm,0,&edata,&sievent)==SI_IS_EVENT){
            /* メッセージの解釈 */
            if(sievent.type==SI_MOTION_EVENT){
                for(int i=0;i<6;i++){
                    data[i]=sievent.u.spwData.mData[i];}
                itvl[0]=sievent.u.spwData.period;
                flag=1;}
            else if(sievent.type==SI_ZERO_EVENT){
                for(int i=0;i<6;i++){data[i]=0;}
                itvl[0]=sievent.u.spwData.period;
                flag=0;}
            else{
                /*printf("other event\n");*/
                flag=0;}
        }//Si event
    }//loop
    return flag;}
void si_getevent(float data[6]){
    /* dataのクリア */
    for(int i=0;i<6;i++){data[i]=0.0f;}
    int idata[6],itvl[1];
    /* 取得 */
    int ret=si_getevent_i(idata,itvl);
    if(!ret){return;}
    /* 値の設定 */
    for(int i=0;i<6;i++){
        data[i]=(float)idata[i]/(float)itvl[0];}}
/* 表示 */
void si_print_data(float data[6]){
    printf("%f %f %f \n", data[0], data[1], data[2]);
    printf("%f %f %f \n", data[3], data[4], data[5]);}
#endif

/*--------------------------------------------------------------
    glut設定
--------------------------------------------------------------*/
/* 背景色（クリアカラー）の定義 */
GLfloat clearcolor[4]={0.3F,0.3F,0.3F,0.0F};
/* 環境光の定義 */
GLfloat lightmodel_ambient[]={0.2F,0.2F,0.2F,1.0F};
/* ライトモデルの定義 (light 0) */
GLfloat light0[4][4]={
    {0.3F,0.3F,0.3F,1.0F}, /* ambient */ 
    {0.6F,0.6F,0.7F,1.0F}, /* diffuse */ 
    {0.3F,0.3F,0.4F,1.0F}, /* specular */
    {0.8F,-0.8F,1.0F,0.0F}};/* position */
/* ライトモデルを設定する関数 */
void light_set(GLenum light,GLfloat lig[][4]){
    glLightfv(light, GL_AMBIENT,  lig[0]);
    glLightfv(light, GL_DIFFUSE,  lig[1]);
    glLightfv(light, GL_SPECULAR, lig[2]);
    glLightfv(light, GL_POSITION, lig[3]);}
/* ライトの位置のみを設定する関数 */
void light_set_pos(GLenum light,GLfloat lig[][4]){
    glLightfv(light, GL_POSITION, lig[3]);}
/* 初期化 */
void gl_init(void){
    /* デプスバッファを使用 */
    glEnable(GL_DEPTH_TEST);
    glDepthRange(0.0,1.0);
    glDepthFunc(GL_LESS);
    /* ポリゴンは反時計回りに書くものとし、裏面は書かない */
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    /*glDisable(GL_CULL_FACE);*/
    /* グーローシェーディングを行う */
    glShadeModel(GL_SMOOTH);
    /* 法線の単位ベクトル化処理をする */
    glEnable(GL_NORMALIZE);
    /* 環境光を設定、視点位置の近似計算 */
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,lightmodel_ambient);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_FALSE);
    /*glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);*/
    /* クリアカラー */
    glClearColor(clearcolor[0],clearcolor[1],clearcolor[2],clearcolor[3]);
    glClearDepth(1.0);
    /* ライトの設定 */
    light_set(GL_LIGHT0,light0);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);}

/*--------------------------------------------------------------
    インタフェース
--------------------------------------------------------------*/
/* イベント関数プロトタイプ */
void init_func(int argc,char *argv[]);
void exit_func(void);
void display_func(void);
void key_func(unsigned char key,int x,int y);
void special_func(int key,int x,int y);
void mouse_func(int button,int state,int x,int y);
void motion_func(int x, int y);
void idle_func(void);
/* ウィンドウサイズ */
int view_port[4];
/* 3dx状態 */
float T_3dx[3],R_3dx[3];
float T_3dx_gain=0.00001f;
float R_3dx_gain=0.0001f;
/* glui */
void glui_init(void);

/*--------------------------------------------------------------
    main
--------------------------------------------------------------*/
/* window名 */
const char window_name[128]="Test";

/* glui初期化関数プロトとタイプ */
int main_win;

#ifdef USE_GLUI
GLUI* sub_win;
#endif

void reshape_func(int w,int h){
#ifdef USE_GLUI
    GLUI_Master.get_viewport_area(
        &view_port[0],&view_port[1],
        &view_port[2],&view_port[3]);
#else
    view_port[0]=0;view_port[1]=0;
    view_port[2]=w;view_port[3]=h;
#endif
}

/* メインループ */
int main(int argc,char *argv[]){
    /* ウィンドウを開く */
    glutInit(&argc,argv);
    glutInitWindowSize(800,600);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutCreateWindow(window_name);
    main_win=glutGetWindow();
    /* glew初期化 */
    if(glewInit()!=GLEW_OK){printf("glew error\n");exit(0);}
    /* テクスチャ最大数 */
    int num_tex;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,&num_tex);
    printf("num_tex=%d\n",num_tex);
    /* OpenGLの設定 */
    gl_init();
#ifdef USE_3DXMOUSE
    /* 3Dx初期化 */
    si_open(window_name);
    atexit(si_close);
#endif
    /* glutイベント関数 */
    atexit(exit_func); //終了処理
    glutDisplayFunc(display_func);
    glutMotionFunc(motion_func);
    /* Glui/glutイベント関数 */
#ifdef USE_GLUI
    GLUI_Master.set_glutKeyboardFunc(key_func);
    GLUI_Master.set_glutSpecialFunc(special_func);
    GLUI_Master.set_glutMouseFunc(mouse_func);
    GLUI_Master.set_glutReshapeFunc(reshape_func);
    GLUI_Master.set_glutIdleFunc(idle_func);
#else
    glutKeyboardFunc(key_func);
    glutSpecialFunc(special_func);
    glutMouseFunc(mouse_func);
    glutReshapeFunc(reshape_func);
    glutIdleFunc(idle_func);
#endif
    /* Gluiサブウィンドウ */
#ifdef USE_GLUI
    sub_win=GLUI_Master.create_glui_subwindow(
        main_win,GLUI_SUBWINDOW_RIGHT);
    sub_win->set_main_gfx_window(main_win);
#endif
    /* Glui初期化 */
    init_func(argc,argv);
#ifdef USE_GLUI
    glui_init();
#endif
    /* イベントループの実行 */
    for(;;){
#ifdef USE_3DXMOUSE
        /* 3dx関係の処理 */
        float m[6];
        //getMagellanMotion(m);
        si_getevent(m);
        /* cg座標->物理座標（z手前） */
        T_3dx[0]=m[0]*T_3dx_gain;
        T_3dx[1]=m[1]*T_3dx_gain;
        T_3dx[2]=-m[2]*T_3dx_gain;
        R_3dx[0]=m[3]*R_3dx_gain;
        R_3dx[1]=m[4]*R_3dx_gain;
        R_3dx[2]=-m[5]*R_3dx_gain;
#endif
        /* glut関係の処理 */   
        glutMainLoopEvent();
        idle_func();
    }//for
    /*return(0);*/}

/*--------------------------------------------------------------
    テスト
--------------------------------------------------------------*/
#if 0
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include"lib/vec3mat33c3.cpp"

#define USE_GLUI
#define USE_3DXMOUSE
#include"lib/glif2.cpp"
#include"lib/glshape.cpp"

/*------------------------------------------------------------*/
/* glui */
int test_val=0;
#ifdef USE_GLUI
void test_func(int id){
    printf("test! (%d)\n",id);
    exit(0);}
void glui_init(void){
    /* 全体フレーム */
    GLUI_Panel *frame_panel;
    frame_panel=sub_win->add_panel("",GLUI_PANEL_NONE);
    /* テスト */
    GLUI_Panel *pan;
    pan=sub_win->add_panel_to_panel(
        frame_panel,"Test",GLUI_PANEL_EMBOSSED);
    sub_win->add_button_to_panel(
        pan,"test",0,test_func)->set_w(20);
    GLUI_RadioGroup *test_group;
    test_group=sub_win->add_radiogroup_to_panel(pan,&test_val);
    sub_win->add_radiobutton_to_group(test_group,"object");
    sub_win->add_radiobutton_to_group(test_group,"view");
    test_group-> set_int_val(0);
}
#endif

/*------------------------------------------------------------*/
/* イベント関数 */
float eye_dis=0.3f; //to display
float dot_pitch=0.00025f;//mm/pix
float eye_near=0.1f;
float eye_far=1.0f;

/* 物体 */
mat33f Row;
vec3f Tow;
/* x_v=Rview*x_w+Tview */
mat33f Rwv;
vec3f Twv;

void init_func(int argc,char *argv[]){
    Rwv=mat33f::Rx(-90*(float)DEG_TO_RAD);
    Twv=vec3f(0,0,-0.5f);
    Row.identity();Tow.zero();}
void exit_func(void){}
void display_func(void){
    /* 画面クリア */
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    /* ビューポート */
    glViewport(view_port[0],view_port[1],view_port[2],view_port[3]);
    /* 視体積 */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float k=eye_near/eye_dis;
    float hw=view_port[2]*dot_pitch/2;
    float hh=view_port[3]*dot_pitch/2;
    glFrustum(-hw*k,hw*k,-hh*k,hh*k,eye_near,eye_far);
    /* カメラの位置・姿勢 */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    mat44f m;
    m.set(Rwv,Twv);
    glMultMatrixf(m.m[0]);
    /* ライトの設定 */
    light_set_pos(GL_LIGHT0,light0);
    /* 座標軸 */
    glDisable(GL_LIGHTING);
    axis_draw(100.0);
    /* 移動 */
    mat33f Rvw=Rwv.trans();
#ifdef USE_3DXMOUSE
    mat33f tmp_r=mat33f::Rn(R_3dx[0],R_3dx[1],R_3dx[2]);
    vec3f tmp_t=vec3f(T_3dx[0],T_3dx[1],T_3dx[2]);
    mat33f rw=Rvw*tmp_r*Rwv;
    vec3f tw=Rvw*tmp_t;
#else
    mat33f rw=mat33f::I();
    vec3f tw=vec3f::Z();
#endif
    if(test_val==0){//物体
        Tow=Tow+tw;
        Row=rw*Row;Row.norm_self();}
    else{//世界
        /* x_v=Rwv*(rw*x_w+tw)+Tvw */
        Twv=Twv+Rwv*tw;
        Rwv=Rwv*rw;Rwv.norm_self();
    }
    /* 描画設定 */
    glEnable(GL_LIGHTING);
    material_set(materialw);
    /* 物体座標に入る */
    glPushMatrix();
    m.set(Row,Tow);
    glMultMatrixf(m.m[0]);
    /* 物体を描画 */
    cube_draw(0.06);
    /* 物体座標から出る */
    glPopMatrix();
    /* swap buffers */
    glutSwapBuffers();
    glFlush();}
void key_func(unsigned char key,int x,int y){
    //printf("key %d (%d,%d)\n",key,x,y);
    if(key==0x1b){exit(0);}
}
void mouse_func(int button,int state,int x,int y){}
void motion_func(int x, int y){}
void idle_func(void){
    /* 再描画の指示 */
    glutSetWindow(main_win);
    glutPostRedisplay();}
#endif

/*--------------------------------------------------------------
--------------------------------------------------------------*/
#endif /*_GLIF2_CPP_*/
