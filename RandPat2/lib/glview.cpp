#ifndef _GLVIEW_CPP_
#define _GLVIEW_CPP_


/*
2020.6.30: BasicOrthoを追加

*/ 
/*--------------------------------------------------------------
    Simple View
--------------------------------------------------------------*/
class SimpleView{
public:
//パラメータ
    float near_clip=0.05f;//m
    float far_clip=3.0f;//m
    float screen_dis=0.3f;//m
    float screen_reso=0.25e-3f;//m/dot
//ビュー
    float eye_az=20;
    float eye_el=20;
    float eye_dis=0.5f;
//コンストラクタ
    SimpleView(){}
//メソッド
    void clear(float color[4]);
    void projection(int *viewport=0);
    void viewing(void);
    void keyinput(unsigned char key);
};

/*------------------------------------------------------------*/
void SimpleView::clear(float color[4]){
    glClearColor(color[0],color[1],color[2],color[3]);
    glClearDepth(1.0F);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);}
void SimpleView::projection(int *viewport){
    /* set viewport */
    int vp[4];
    if(viewport){for(int i=0;i<4;i++){vp[i]=viewport[i];}}
    else{glGetIntegerv(GL_VIEWPORT,vp);}
    glViewport(vp[0],vp[1],vp[2],vp[3]);
    /* calc viewing volume */
    float wd,ht,k;
    wd=screen_reso*vp[2];//width
    ht=screen_reso*vp[3];//height
    k=near_clip/screen_dis;
    float l,r,b,t,n,f;
    l=k*(0-0.5f*wd);
    r=k*(0+0.5f*wd);
    b=k*(0-0.5f*ht);
    t=k*(0+0.5f*ht);
    n=near_clip;
    f=far_clip;
    /* projection */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(l,r,b,t,n,f);}
void SimpleView::viewing(void){
    /* modelview */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    /* カメラの位置／方向 */
    glTranslated(0.0,0.0,-eye_dis);
    glRotated(-(90.0-eye_el), 1.0,0.0,0.0);
    glRotated(-eye_az, 0.0,0.0,1.0);}
void SimpleView::keyinput(unsigned char key){
    switch(key){
    /* 視点移動 */
    case ',': eye_az -=10;break;
    case '.': eye_az +=10;break;
    case '<': eye_el -=10;break;
    case '>': eye_el +=10;break;
    case '[': eye_dis-=0.01f;break;
    case ']': eye_dis+=0.01f;break;
    default: break;}}

/*--------------------------------------------------------------
    Simple Ortho
--------------------------------------------------------------*/
class SimpleOrtho{
public:
//パラメータ
    float near_clip=-0.5f;//m
    float far_clip=0.5f;//m
    float width=0.5f;//m
//ビュー
    float eye_az=20;
    float eye_el=20;
//コンストラクタ
    SimpleOrtho(){}
//メソッド
    void clear(float color[4]);
    void projection(int *viewport=0);
    void viewing(void);
    void keyinput(unsigned char key);
};

/*------------------------------------------------------------*/
void SimpleOrtho::clear(float color[4]){
    glClearColor(color[0],color[1],color[2],color[3]);
    glClearDepth(1.0F);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);}
void SimpleOrtho::projection(int *viewport){
    /* set viewport */
    int vp[4];
    if(viewport){for(int i=0;i<4;i++){vp[i]=viewport[i];}}
    else{glGetIntegerv(GL_VIEWPORT,vp);}
    glViewport(vp[0],vp[1],vp[2],vp[3]);
    /* projection */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float wh=width*0.5f;
    float hh=wh*(float)vp[3]/(float)vp[2];
    glOrtho(-wh,wh,-hh,hh,near_clip,far_clip);}
void SimpleOrtho::viewing(void){
    /* modelview */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    /* カメラの位置／方向 */
    glRotated(-(90.0-eye_el), 1.0,0.0,0.0);
    glRotated(-eye_az, 0.0,0.0,1.0);}
void SimpleOrtho::keyinput(unsigned char key){
    switch(key){
    /* 視点移動 */
    case ',': eye_az -=10;break;
    case '.': eye_az +=10;break;
    case '<': eye_el -=10;break;
    case '>': eye_el +=10;break;
    default: break;}}

/*--------------------------------------------------------------
    Basic Ortho
--------------------------------------------------------------*/
class BasicOrtho{
public:
//パラメータ
    float origin[3];
    float near_clip;
    float far_clip;
    float width;
//コンストラクタ
    BasicOrtho(){
        origin[0]=origin[1]=origin[2]=0;
        near_clip=-0.5f;
        far_clip=0.5f;
        width=0.5f;}
//メソッド
    void clear(float color[4]);
    void projection(int *viewport=0);
    void viewing(void);
    void point_to_pos(int x,int y,float &vx,float &vy);
    void pos_to_point(float &vx,float &vy,int x,int y);
};

/*------------------------------------------------------------*/
void BasicOrtho::clear(float color[4]){
    glClearColor(color[0],color[1],color[2],color[3]);
    glClearDepth(1.0F);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);}
void BasicOrtho::projection(int *viewport){
    /* set viewport */
    int vp[4];
    if(viewport){for(int i=0;i<4;i++){vp[i]=viewport[i];}}
    else{glGetIntegerv(GL_VIEWPORT,vp);}
    glViewport(vp[0],vp[1],vp[2],vp[3]);
    /* projection */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float wh=width*0.5f;
    float hh=wh*(float)vp[3]/(float)vp[2];
    glOrtho(origin[0]-wh,origin[0]+wh,
        origin[1]-hh,origin[1]+hh,
        origin[2]+near_clip,origin[2]+far_clip);}
void BasicOrtho::viewing(void){
    /* modelview */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();}
void BasicOrtho::point_to_pos(int x_in,int y_in,float &vx,float &vy){
    int vp[4];
    glGetIntegerv(GL_VIEWPORT,vp);
    int x=x_in;
    int y=vp[3]-y_in-1;
    float height=width*(float)vp[3]/(float)vp[2];
    float ax=width/vp[2];
    float bx=origin[0]-width*0.5f;
    float ay=height/vp[3];
    float by=origin[1]-height*0.5f;
    vx=ax*(x+0.5f)+bx;
    vy=ay*(y+0.5f)+by;}
void BasicOrtho::pos_to_point(float &vx,float &vy,int x_out,int y_out){
    int vp[4];
    glGetIntegerv(GL_VIEWPORT,vp);
    float height=width*(float)vp[3]/(float)vp[2];
    float ax=vp[2]/width;
    float bx=vp[2]/2-ax*origin[0];
    float ay=vp[3]/height;
    float by=vp[3]/2-ay*origin[1];
    int x=(int)floor(ax*vx+bx+0.5f);
    int y=(int)floor(ay*vy+by+0.5f);
    x_out=x;
    y_out=vp[3]-y-1;}
        
/*--------------------------------------------------------------
    Pixel View
--------------------------------------------------------------*/
class PixelView{
public:
//メソッド
    void clear(float color[4]);
    void projection(int *viewport=0);
    void viewing(void);
    void keyinput(unsigned char key);
    void text(const char *text,int x=10,int y=10,float scale=0.3f,float lskip=200);
};

/*------------------------------------------------------------*/
void PixelView::clear(float color[4]){
    glClearColor(color[0],color[1],color[2],color[3]);
    glClearDepth(1.0F);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);}
void PixelView::projection(int *viewport){
    int vp[4];
    if(viewport){for(int i=0;i<4;i++){vp[i]=viewport[i];}}
    else{glGetIntegerv(GL_VIEWPORT,vp);}
    glViewport(vp[0],vp[1],vp[2],vp[3]);
    /* projection */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(vp[0],vp[0]+vp[2],vp[1],vp[1]+vp[3],-1,1);}
void PixelView::viewing(void){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();}
void PixelView::text(const char *text,int x,int y,float s,float lskip){
    glPushMatrix();
    glTranslatef((float)x,(float)y,0);
    glDisable(GL_LIGHTING);
    glScaled(s,s,s);
    float m[4][4];
    glGetFloatv(GL_MODELVIEW_MATRIX,m[0]);
    int line=0;
    for(char *p=(char*)text;*p!=0;p++){
        if(*p=='\n'){
            line++; glLoadMatrixf(m[0]);
            glTranslatef(0,-lskip*line,0);}
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,*p);}
    glPopMatrix();}

/*--------------------------------------------------------------
    3dx View
--------------------------------------------------------------*/
class Simple3dxView{
public:
//projectionパラメータ
    float near_clip=0.05f;//m
    float far_clip=3.0f;//m
    float screen_dis=0.3f;//m
    float screen_reso=0.25e-3f;//m/dot
//viewintパラメータ
    mat33f Rwv;
    vec3f Twv;
//コンストラクタ
    Simple3dxView(){
        Rwv=mat33f::Rx(-90*(float)DEG_TO_RAD);
        Twv=vec3f(0,0,-0.5f);}
//メソッド
    void clear(float color[4]);
    void projection(int *viewport=0);
    void viewing(void);
    void move_camera(float R3dx[3],float T3dx[3]);
    void move_object(float R3dx[3],float T3dx[3],mat33f& Row,vec3f& Tow);
//座標変換設定
    void set_Mwv(mat33f rwv,vec3f twv){Rwv=rwv;Twv=twv;}
    void get_Mwv(mat33f& rwv,vec3f& twv){rwv=Rwv;twv=Twv;}
    void set_Mvw(mat33f rvw,vec3f tvw){
        /* xw=rvw.xv+tvw */
        /* xv=rvw~.xw-rvw~.tvw */
        Rwv=rvw.trans();
        Twv=Rwv*tvw*(-1);}
    void get_Mvw(mat33f& rvw,vec3f& tvw){
        /* xv=Rwv.xw+Twv */
        /* xw=Rwv~.xv-Rwv~.Twv */
        rvw=Rwv.trans();
        tvw=rvw*Twv*(-1);}
};

/*------------------------------------------------------------*/
void Simple3dxView::clear(float color[4]){
    glClearColor(color[0],color[1],color[2],color[3]);
    glClearDepth(1.0F);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);}
void Simple3dxView::projection(int *viewport){
    /* set viewport */
    int vp[4];
    if(viewport){for(int i=0;i<4;i++){vp[i]=viewport[i];}}
    else{glGetIntegerv(GL_VIEWPORT,vp);}
    glViewport(vp[0],vp[1],vp[2],vp[3]);
    /* calc viewing volume */
    float wd,ht,k;
    wd=screen_reso*vp[2];//width
    ht=screen_reso*vp[3];//height
    k=near_clip/screen_dis;
    float l,r,b,t,n,f;
    l=k*(0-0.5f*wd);
    r=k*(0+0.5f*wd);
    b=k*(0-0.5f*ht);
    t=k*(0+0.5f*ht);
    n=near_clip;
    f=far_clip;
    /* projection */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(l,r,b,t,n,f);}
void Simple3dxView::viewing(void){
    /* modelview */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    /* カメラの位置／方向 */
    mat44f m;
    m.set(Rwv,Twv);
    glMultMatrixf(m.m[0]);}
void Simple3dxView::move_camera(float R3dx[3],float T3dx[3]){
    mat33f tmp_r=mat33f::Rn(R_3dx[0],R_3dx[1],R_3dx[2]);
    vec3f tmp_t=vec3f(T_3dx[0],T_3dx[1],T_3dx[2]);
    mat33f Rvw=Rwv.trans();
    mat33f rw=Rvw*tmp_r*Rwv;
    vec3f tw=Rvw*tmp_t;
    /* x_v=Rwv*(rw*x_w+tw)+Tvw */
    Twv=Twv+Rwv*tw;
    Rwv=Rwv*rw;Rwv.norm_self();}
void Simple3dxView::move_object(float R3dx[3],float T3dx[3],
    mat33f& Row,vec3f& Tow){
    mat33f tmp_r=mat33f::Rn(R_3dx[0],R_3dx[1],R_3dx[2]);
    vec3f tmp_t=vec3f(T_3dx[0],T_3dx[1],T_3dx[2]);
    mat33f Rvw=Rwv.trans();
    mat33f rw=Rvw*tmp_r*Rwv;
    vec3f tw=Rvw*tmp_t;
    Tow=Tow+tw;
    Row=rw*Row;Row.norm_self();}

/*--------------------------------------------------------------
--------------------------------------------------------------*/
#endif /*_GLVIEW_CPP_*/
