#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<GL/glew.h>
#include<GL/freeglut.h>
#include<vector>
#include<atomic>

const float pi10=3.14159365358979f;
const float pi20=pi10*2.0f;
const float pi05=pi10*0.5f;

#include"lib/vec3mat33c3.cpp"
#include"lib/glif2.cpp"
#include"lib/glview.cpp"
#include"lib/glshape.cpp"
#include"lib/polkadots.cpp"
#include"lib/stopwatch.cpp"
#include"lib/callback_win.cpp"
#include"lib/pinmap.cpp"
#define USE_DEVICE
#include"lib/polkamove.cpp"
#include"lib/rs232.cpp"
#include"lib/comif.cpp"
#include"lib/air128.cpp"

/*--------------------------------------------------------------
    デバイスインタフェース (PolkaMoveの関数の定義)
--------------------------------------------------------------*/
Air128 air[3];

template<int DIV_W,int DIV_H,int PIN_N>
void PolkaMove::device_open(void){
    std::vector<ComIF::devlist> list;
    ComIF::list_device(list);
    for(int id=0;id<3;id++){
        int flag=0;
        for(int i=0;i<(int)list.size();i++){
            if((list[i].dev_type== )&&(list[i].dev_no==id)){
                air[id].open(list[i].com_name);
                printf("open %s for device %d\n",list[i].com_name,id);
                flag=1;}}
        if(!flag){printf("device #%d not found\n",id);exit(0);}}}
template<int DIV_W,int DIV_H,int PIN_N>
void PolkaMove::device_close(void){
    for(int id=0;id<3;id++){air[id].close();}}
template<int DIV_W,int DIV_H,int PIN_N>
void PolkaMove::device_output(float data[PIN_N]){
    for(int id=0;id<3;id++){
        air[id].da_set_p128(&(data[id*128]));}}

/*--------------------------------------------------------------
    glutイベント関数
--------------------------------------------------------------*/
/* パラメータ */
const float RAD_A=0.010f;  //半径平均 m
const float RAD_S=0.000f; //半径標準偏差 m
const float GAP  =0.002f;    //間隔 m
/* 試行回数 */
const int REP=100000;
/* 領域 */
const float AREA_W=0.5f;
const float AREA_H=0.5f;
/* ピン */
char pin_file[]="Data/pinmap.csv";
/* 移動 */
vec3f v(0.05f,0.05f,0);
vec3f o(0.0f, 0.0f, 0);

PolkaMove</*DIV_W*/20,/*DIV_H*/20,/*PIN_N*/384> pm;

/* view */
SimpleOrtho view;
StopWatch sw;

void init_func(int argc,char *argv[]){
    pm.make_pattern(AREA_W,AREA_H,RAD_A,RAD_S,GAP,REP);
    pm.load_pinmap(pin_file);
    pm.start_callback();}

void display_func(void){
    /* clear view */
    view.clear(clearcolor);
    /* projection */
    view.projection();
    glScalef(2,2,2);
    glTranslatef(0,-0.1f,0);
    glDisable(GL_DEPTH_TEST);
    /* draw */
    glDisable(GL_LIGHTING);
    glColor3f(1,1,1);
    for(int j=-2;j<=1;j++){
        for(int i=-2;i<=1;i++){
            glPushMatrix();
            glTranslatef(pm.polka.area_w*i+pm.cur_x,pm.polka.area_h*j+pm.cur_y,0);
            glColor3f(0.5f,0.5f,0.5f);pm.polka.draw();
            glColor3f(1,1,0);pm.polka.draw_area();
            glPopMatrix();}}
    /* ピン描画 */
    glDisable(GL_LIGHTING);
    for(unsigned int i=0;i<pm.pin_n;i++){
        float* p=pm.pin_p[i];
        if(pm.pin_f[i]>0.5f){glColor3f(1,0,0);}else{glColor3f(0,1,1);}
        glPushMatrix();
        glTranslatef(p[0],p[1],0);
        circle_draw(0.001f,12);
        glPopMatrix();}
    /* テキスト */
    char text[pm.pin_n];
    sprintf(text,"%f",pm.ptime);
    glPushMatrix();
    glTranslatef(0,0.01,0);
    glColor3f(1,1,0);
    glLineWidth(2);
    text_draw(text,0.0001f);
    glLineWidth(1);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    /* swap buffers */
    glutSwapBuffers();
    glFlush();}

void idle_func(void){glutPostRedisplay();}
void exit_func(void){pm.stop_callback();}
void key_func(unsigned char key,int x,int y){
    view.keyinput(key);
    if(key==' '){pm.present(v,o,3.0f);}
    if(key==0x1b){exit(0);}}
void special_func(int key,int x,int y){}
void mouse_func(int button,int state,int x,int y){}
void motion_func(int x, int y){}

/*--------------------------------------------------------------
--------------------------------------------------------------*/
