#ifndef _GLSHAPE_CPP_
#define _GLSHAPE_CPP_

#ifndef _UNIT_METER_
#define UNIT_SCALING 1.0f
#else
#define UNIT_SCALING 0.001f
#endif

/*--------------------------------------------------------------
    履歴
2014.7.17 text_draw()を追加
2021.1.11 circle_draw()を追加

--------------------------------------------------------------*/
/*--------------------------------------------------------------
    材質
--------------------------------------------------------------*/
/* 材質の設定の例 */
GLfloat material0[5][4]={
    {0.8f,0.8f,0.8f,1.0f}, /* ambient    環境光に対する反射の色と強さ */
    {0.8f,0.8f,0.8f,1.0f}, /* diffuse    拡散反射の色と強さ */
    {0.0f,0.0f,0.0f,1.0f}, /* specular   鏡面的反射の色と強さ */
    {1.0f},                /* shininess  鏡面反射のスポットの広がり */
    {0.0f,0.0f,0.0f,1.0f}};/* emission   物体自体の発光の色と強さ*/
/* 材質の設定の例（石膏？）*/
float material1[5][4]={
    {0.9f,0.9f,0.9f,1.0f}, /* ambient    環境光に対する反射の色と強さ */
    {0.6f,0.6f,0.6f,1.0f}, /* diffuse    拡散反射の色と強さ */
    {0.3f,0.3f,0.3f,1.0f}, /* specular   鏡面的反射の色と強さ */
    {3.0f},                /* shininess  鏡面反射のスポットの広がり */
    {0.0f,0.0f,0.0f,1.0f}};/* emission   物体自体の発光の色と強さ*/
/* 材質の設定の例（金属？）*/
float material2[5][4]={
    {0.5f,0.5f,0.5f,1.0f}, /* ambient    環境光に対する反射の色と強さ */
    {0.3f,0.3f,0.3f,1.0f}, /* diffuse    拡散反射の色と強さ */
    {0.9f,0.9f,0.9f,1.0f}, /* specular   鏡面的反射の色と強さ */
    {30.0},                /* shininess  鏡面反射のスポットの広がり */
    {0.0f,0.0f,0.0f,1.0f}};/* emission   物体自体の発光の色と強さ*/
GLfloat materialw[5][4]={
    {1.0f,1.0f,1.0f,1.0f},    {1.0f,1.0f,1.0f,1.0f},
    {0.0f,0.0f,0.0f,1.0f},    {5.0f},
    {0.0f,0.0f,0.0f,1.0f}};
GLfloat materialr[5][4]={
    {1.0f,0.5f,0.5f,1.0f},    {1.0f,0.5f,0.5f,1.0f},
    {0.0f,0.0f,0.0f,1.0f},    {5.0f},
    {0.0f,0.0f,0.0f,1.0f}};
GLfloat materialg[5][4]={
    {0.5f,1.0f,0.5f,1.0f},    {0.5f,1.0f,0.5f,1.0f},
    {0.0f,0.0f,0.0f,1.0f},    {5.0f},
    {0.0f,0.0f,0.0f,1.0f}};
GLfloat materialb[5][4]={
    {0.5f,0.5f,1.0f,1.0f},    {0.5f,0.5f,1.0f,1.0f},
    {0.0f,0.0f,0.0f,1.0f},    {5.0f},
    {0.0f,0.0f,0.0f,1.0f}};
GLfloat materialy[5][4]={
    {1.0f,1.0f,0.5f,1.0f},    {1.0f,1.0f,0.5f,1.0f},
    {0.0f,0.0f,0.0f,1.0f},    {5.0f},
    {0.0f,0.0f,0.0f,1.0f}};
GLfloat materialc[5][4]={
    {0.5f,1.0f,1.0f,1.0f},    {0.5f,1.0f,1.0f,1.0f},
    {0.0f,0.0f,0.0f,1.0f},    {5.0f},
    {0.0f,0.0f,0.0f,1.0f}};
GLfloat materialm[5][4]={
    {1.0f,0.5f,1.0f,1.0f},    {1.0f,0.5f,1.0f,1.0f},
    {0.0f,0.0f,0.0f,1.0f},    {5.0f},
    {0.0f,0.0f,0.0f,1.0f}};

/* 材質をまとめて設定する関数 */
void material_set(GLfloat mat[][4]){
    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat[0]);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat[1]);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat[2]);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat[3]);
    glMaterialfv(GL_FRONT, GL_EMISSION,  mat[4]);}

/*--------------------------------------------------------------
    頂点の2次元配列
--------------------------------------------------------------*/
template <class Type>
class Array3d{
private:
    Type* p;
    int I,J,K;
public:
    Array3d(int i,int j,int k){
        I=i;J=j;K=k; p=new Type[I*J*K];}
    ~Array3d(){delete[] p;}
    Type& a(int i,int j,int k){return *(p+(i*J+j)*K+k);}
    Type* a(int i,int j){return p+(i*J+j)*K;}
};

/*--------------------------------------------------------------
    形状を描く
--------------------------------------------------------------*/
/* 立方体の頂点位置などの定義 */
float box_v[8][3]={
    {-0.5f,-0.5f,-0.5f},{ 0.5f,-0.5f,-0.5f},
    {-0.5f, 0.5f,-0.5f},{ 0.5f, 0.5f,-0.5f},
    {-0.5f,-0.5f, 0.5f},{ 0.5f,-0.5f, 0.5f},
    {-0.5f, 0.5f, 0.5f},{ 0.5f, 0.5f, 0.5f}};
float box_n[6][3]={
    {-1.0f, 0.0f, 0.0f},{ 1.0f, 0.0f, 0.0f},
    { 0.0f,-1.0f, 0.0f},{ 0.0f, 1.0f, 0.0f},
    { 0.0f, 0.0f,-1.0f},{ 0.0f, 0.0f, 1.0f}};
int box_vo[6][4]={
    {4,0,1,5},{5,1,3,7},{7,3,2,6},{5,7,6,4},{4,6,2,0},{0,2,3,1}};
int box_no[6]={2,1,3,5,0,4};

/* 直方体を描く関数 */
void box_draw(float sx,float sy,float sz){
    int i,j;float v[8][3];
    /* スケーリングされた頂点の位置を計算します */
    for(i=0;i<8;i++){          v[i][0]=box_v[i][0]*sx;
        v[i][1]=box_v[i][1]*sy;v[i][2]=box_v[i][2]*sz;}
    /* 頂点の接続順序にしたがってポリゴンを書きます */
    for(i=0;i<6;i++){
        glBegin(GL_POLYGON);
        glNormal3fv(box_n[box_no[i]]);
        for(j=0;j<4;j++){glVertex3fv(v[box_vo[i][j]]);}
        glEnd();}}

/* 直方体を描くディスプレイリストの作成 */
GLuint box_list(float sx,float sy,float sz){
    GLuint list=glGenLists(1);
    glNewList(list,GL_COMPILE);box_draw(sx,sy,sz);glEndList();
    return list;}
/* 引数バリエーション */
void box_draw(float w[3]){box_draw(w[0],w[1],w[2]);}
GLuint box_list(float w[3]){return box_list(w[0],w[1],w[2]);}

/*------------------------------------------------------------*/
/* 立方体の頂点位置などの定義 */
float cube_v[8][3]={
    {-0.5f,-0.5f,-0.5f},{ 0.5f,-0.5f,-0.5f},
    {-0.5f, 0.5f,-0.5f},{ 0.5f, 0.5f,-0.5f},
    {-0.5f,-0.5f, 0.5f},{ 0.5f,-0.5f, 0.5f},
    {-0.5f, 0.5f, 0.5f},{ 0.5f, 0.5f, 0.5f}};
float cube_n[6][3]={
    {-1.0f, 0.0f, 0.0f},{ 1.0f, 0.0f, 0.0f},
    { 0.0f,-1.0f, 0.0f},{ 0.0f, 1.0f, 0.0f},
    { 0.0f, 0.0f,-1.0f},{ 0.0f, 0.0f, 1.0f}};
int cube_vo[6][4]={
    {4,0,1,5},{5,1,3,7},{7,3,2,6},{5,7,6,4},{4,6,2,0},{0,2,3,1}};
int cube_no[6]={2,1,3,5,0,4};

/* 立方体を描く関数 */
void cube_draw(float scale){int i,j; float v[8][3];
    /* スケーリングされた頂点の位置を計算します */
    for(i=0;i<8;i++)for(j=0;j<3;j++){v[i][j]=cube_v[i][j]*scale;}
    /* 頂点の接続順序にしたがってポリゴンを書きます */
    for(i=0;i<6;i++){
        glBegin(GL_POLYGON);
        glNormal3fv(cube_n[cube_no[i]]);
        for(j=0;j<4;j++){glVertex3fv(v[cube_vo[i][j]]);}
        glEnd();}}

/* 立方体を描くディスプレイリストの作成 */
GLuint cube_list(float scale){
    GLuint list=glGenLists(1);
    glNewList(list,GL_COMPILE);cube_draw(scale);glEndList();
    return list;}

/*------------------------------------------------------------*/
/* 球を描く関数 */
void sphere_draw(float rad,int lon=24,int lat=12){
    int i,j;
    /* メモリ領域の確保 */
    Array3d<float> n(lat+1,lon+1,3);
    Array3d<float> p(lat+1,lon+1,3);
    /* ポリゴンの頂点位置と法線を計算します */
    for(i=0;i<lat+1;i++)for(j=0;j<lon+1;j++){
        float tla,tlo,sla,cla,slo,clo;
        tla=((float)i/(float)lat)*(float)M_PI;
        tlo=((float)j/(float)lon)*2*(float)M_PI;
        sla=sinf(tla);cla=cosf(tla);slo=sinf(tlo);clo=cosf(tlo);
        n.a(i,j,0)=(float)(sla*clo); p.a(i,j,0)=n.a(i,j,0)*rad;
        n.a(i,j,1)=(float)(sla*slo); p.a(i,j,1)=n.a(i,j,1)*rad;
        n.a(i,j,2)=(float)(cla);     p.a(i,j,2)=n.a(i,j,2)*rad;}
    /* 北極は三角形の扇形 */
    glBegin(GL_TRIANGLE_FAN);
    glNormal3fv(n.a(0,0));glVertex3fv(p.a(0,0));
    for(j=0;j<lon+1;j++){
        glNormal3fv(n.a(1,j));glVertex3fv(p.a(1,j));}
    glEnd();
    /* 北極／南極以外は三角形の連続パッチで描く */
    for(i=1;i<lat-1;i++){
    glBegin(GL_TRIANGLE_STRIP);
    for(j=0;j<lon;j++){
        glNormal3fv(n.a(i,j));glVertex3fv(p.a(i,j));
        glNormal3fv(n.a(i+1,j));glVertex3fv(p.a(i+1,j));}
    glNormal3fv(n.a(i,0));glVertex3fv(p.a(i,0));
    glNormal3fv(n.a(i+1,0));glVertex3fv(p.a(i+1,0));
    glEnd();}
    /* 南極は三角形の扇形 */
    glBegin(GL_TRIANGLE_FAN);
    glNormal3fv(n.a(lat,0));glVertex3fv(p.a(lat,0));
    for(j=lon;j>=0;j--){
        glNormal3fv(n.a(lat-1,j));glVertex3fv(p.a(lat-1,j));}
    glEnd();}

/* 球を描くディスプレイリストの作成 */
GLuint sphere_list(float rad,int lon=24,int lat=12){
    GLuint list=glGenLists(1);
    glNewList(list,GL_COMPILE);
    sphere_draw(rad,lon,lat);glEndList();
    return list;}

/*------------------------------------------------------------*/
/* 半球を描く関数 */
void hemsphere_draw(float rad,int lon=24,int lat=6){
    /* メモリ領域の確保 */
    Array3d<float> n(lat+1,lon+1,3);
    Array3d<float> p(lat+1,lon+1,3);
    /* ポリゴンの頂点位置と法線を計算します */
    for(int i=0;i<lat+1;i++)for(int j=0;j<lon+1;j++){
        float tla,tlo,sla,cla,slo,clo;
        tla=((float)i/(float)lat)*0.5f*(float)M_PI;
        tlo=((float)j/(float)lon)*2.0f*(float)M_PI;
        sla=sinf(tla);cla=cosf(tla);slo=sinf(tlo);clo=cosf(tlo);
        n.a(i,j,0)=(float)(sla*clo); p.a(i,j,0)=n.a(i,j,0)*rad;
        n.a(i,j,1)=(float)(sla*slo); p.a(i,j,1)=n.a(i,j,1)*rad;
        n.a(i,j,2)=(float)(cla);     p.a(i,j,2)=n.a(i,j,2)*rad;}
    /* 北極は三角形の扇形 */
    glBegin(GL_TRIANGLE_FAN);
    glNormal3fv(n.a(0,0));glVertex3fv(p.a(0,0));
    for(int j=0;j<lon+1;j++){
        glNormal3fv(n.a(1,j));glVertex3fv(p.a(1,j));}
    glEnd();
    /* 北極以外は三角形の連続パッチで描く */
    for(int i=1;i<lat;i++){
        glBegin(GL_TRIANGLE_STRIP);
        for(int j=0;j<lon;j++){
            glNormal3fv(n.a(i,j));glVertex3fv(p.a(i,j));
            glNormal3fv(n.a(i+1,j));glVertex3fv(p.a(i+1,j));}
        glNormal3fv(n.a(i,0));glVertex3fv(p.a(i,0));
        glNormal3fv(n.a(i+1,0));glVertex3fv(p.a(i+1,0));
        glEnd();}}

/* 半球を描くディスプレイリストの作成 */
GLuint hemsphere_list(float rad,int lon=24,int lat=12){
    GLuint list=glGenLists(1);
    glNewList(list,GL_COMPILE);
    hemsphere_draw(rad,lon,lat);glEndList();
    return list;}

/*------------------------------------------------------------*/
/* 円柱を描く関数 */
void cylinder_draw(float rad,float height,int lon=24,int lat=12){
    /* メモリ領域の確保 */
    Array3d<float> n(1,lon+1,3);
    Array3d<float> p(lat+1,lon+1,3);
    float n0[3]={0,0,1};
    float p0[3]={0,0,height*0.5f};
    float n1[3]={0,0,-1};
    float p1[3]={0,0,-height*0.5f};
    /* ポリゴンの頂点位置と法線を計算します */
    for(int j=0;j<lon+1;j++){
        float tlo,slo,clo;
        tlo=((float)j/(float)lon)*2.0f*(float)M_PI;
        slo=sinf(tlo);clo=cosf(tlo);
        n.a(0,j,0)=(float)(clo);
        n.a(0,j,1)=(float)(slo);
        n.a(0,j,2)=(float)(0);}
    for(int i=0;i<lat+1;i++)for(int j=0;j<lon+1;j++){
        float hgt=(0.5f-((float)i/(float)lat))*height;
        p.a(i,j,0)=n.a(0,j,0)*rad;
        p.a(i,j,1)=n.a(0,j,1)*rad;
        p.a(i,j,2)=(float)(hgt);}
    /* 上面は三角形の扇形 */
    glBegin(GL_TRIANGLE_FAN);
    glNormal3fv(n0);glVertex3fv(p0);
    for(int j=0;j<lon+1;j++){glVertex3fv(p.a(0,j));}
    glEnd();
    /* draw middle */
    /* 北極／南極以外は三角形の連続パッチで書きます */
    for(int i=0;i<lat;i++){
    glBegin(GL_TRIANGLE_STRIP);
    for(int j=0;j<lon+1;j++){
        glNormal3fv(n.a(0,j));glVertex3fv(p.a(i,j));
        glNormal3fv(n.a(0,j));glVertex3fv(p.a(i+1,j));}
    glEnd();}
    /* 下面は三角形の扇形 */
    glBegin(GL_TRIANGLE_FAN);
    glNormal3fv(n1);glVertex3fv(p1);
    for(int j=lon;j>=0;j--){glVertex3fv(p.a(lat,j));}
    glEnd();}

/* 円柱を描くディスプレイリストの作成 */
GLuint cylinder_list(float r,float h,int lo=24,int la=12){
    GLuint list=glGenLists(1);
    glNewList(list,GL_COMPILE);cylinder_draw(r,h,lo,la);glEndList();
    return list;}

/*------------------------------------------------------------*/
/* 円筒側面を描く関数 */
void tube_draw(float rad,float height,int lon=24,int lat=12){
    /* メモリ領域の確保 */
    Array3d<float> n(1,lon+1,3);
    Array3d<float> p(lat+1,lon+1,3);
    /* ポリゴンの頂点位置と法線を計算します */
    for(int j=0;j<lon+1;j++){
        float tlo,slo,clo;
        tlo=((float)j/(float)lon)*2.0f*(float)M_PI;
        slo=sinf(tlo);clo=cosf(tlo);
        n.a(0,j,0)=(float)(clo);
        n.a(0,j,1)=(float)(slo);
        n.a(0,j,2)=(float)(0);}
    for(int i=0;i<lat+1;i++)for(int j=0;j<lon+1;j++){
        float hgt=(0.5f-((float)i/(float)lat))*height;
        p.a(i,j,0)=n.a(0,j,0)*rad;
        p.a(i,j,1)=n.a(0,j,1)*rad;
        p.a(i,j,2)=(float)(hgt);}
    /* 側面を三角形の連続パッチで描く */
    for(int i=0;i<lat+1;i++){
        glBegin(GL_TRIANGLE_STRIP);
        for(int j=0;j<lon+1;j++){
            glNormal3fv(n.a(0,j));glVertex3fv(p.a(i,j));
            glNormal3fv(n.a(0,j));glVertex3fv(p.a(i+1,j));}
        glEnd();}}

/* 円筒側面を描くディスプレイリストの作成 */
GLuint tube_list(float r,float h,int lo=24,int la=12){
    GLuint list=glGenLists(1);
    glNewList(list,GL_COMPILE);tube_draw(r,h,lo,la);glEndList();
    return list;}

/* 円筒側面をワイヤフレームで描く関数 */
void tube_wire_draw(float rad,float height,int lon=24,int lat=12){
    /* メモリ領域の確保 */
    Array3d<float> n(1,lon+1,3);
    Array3d<float> p(lat+1,lon+1,3);
    /* ポリゴンの頂点位置と法線を計算します */
    for(int j=0;j<lon+1;j++){
        float tlo,slo,clo;
        tlo=((float)j/(float)lon)*2.0f*(float)M_PI;
        slo=sinf(tlo);clo=cosf(tlo);
        n.a(0,j,0)=(float)(clo);
        n.a(0,j,1)=(float)(slo);
        n.a(0,j,2)=(float)(0);}
    for(int i=0;i<lat+1;i++)for(int j=0;j<lon+1;j++){
        float hgt=(0.5f-((float)i/(float)lat))*height;
        p.a(i,j,0)=n.a(0,j,0)*rad;
        p.a(i,j,1)=n.a(0,j,1)*rad;
        p.a(i,j,2)=(float)(hgt);}
    /* 描く */
    glBegin(GL_LINES);
    for(int j=0;j<lon;j++){
        glNormal3fv(n.a(0,j));glVertex3fv(p.a(0,j));
        glNormal3fv(n.a(0,j));glVertex3fv(p.a(lat,j));}
    for(int i=0;i<lat+1;i++)for(int j=0;j<lon;j++){
        glNormal3fv(n.a(0,j));glVertex3fv(p.a(i,j));
        glNormal3fv(n.a(0,j+1));glVertex3fv(p.a(i,j+1));}
    glEnd();}

/* 円筒側面をワイヤフレームで描くディスプレイリストの作成 */
GLuint tube_wire_list(float r,float h,int lo=24,int la=12){
    GLuint list=glGenLists(1);
    glNewList(list,GL_COMPILE);tube_wire_draw(r,h,lo,la);glEndList();
    return list;}

/*------------------------------------------------------------*/
/* 床を描く関数（ワイヤフレーム） */
void floor_draw(float w){
    glBegin(GL_LINES);
    glNormal3f(0,0,1);
    float step=w/20;
    for(int i=0;i<=20;i++){
        float d=-0.5f*w+i*step;
        glVertex3f(d,-0.5f*w,0);glVertex3f(d,0.5f*w,0);
        glVertex3f(-0.5f*w,d,0);glVertex3f(0.5f*w,d,0);}
    glEnd();}

/* 床を描くディスプレイリストの作成 */
GLuint floor_list(float w){
    GLuint list=glGenLists(1);
    glNewList(list,GL_COMPILE);floor_draw(w);glEndList();
    return list;}

/* 床を描く関数（ワイヤフレーム） */
void floor2_draw(float pitch,int nhalf){
    glBegin(GL_LINES);
    glNormal3f(0,0,1);
    float h=pitch*nhalf;
    for(int i=-nhalf;i<=nhalf;i++){
        float d=pitch*i;
        glVertex3f(d,-h,0);glVertex3f(d,h,0);
        glVertex3f(-h,d,0);glVertex3f(h,d,0);}
    glEnd();}

/* 床を描くディスプレイリストの作成 */
GLuint floor2_list(float w){
    GLuint list=glGenLists(1);
    glNewList(list,GL_COMPILE);floor_draw(w);glEndList();
    return list;}

/*------------------------------------------------------------*/
/* 床を描く関数 */
void texfloor_draw(float w,float pitch=20*UNIT_SCALING){
/* 床の法線 */
static float n[3]={0,0,1};
/* 床のテクスチャ */
static unsigned char check[2][2][3]={
    {{0,0,0},{255,255,255}},{{255,255,255},{0,0,0}}};
    /* 座標計算パラメータ */
    float p_=1/pitch;
    float xgenparam[4]={p_,0,0,0};
    float ygenparam[4]={0,p_,0,0}; 
    /* テクスチャマッピングの指定 */
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glTexImage2D(GL_TEXTURE_2D,0,3,2,2,0,GL_RGB,GL_UNSIGNED_BYTE,check);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
    glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
    glTexGenfv(GL_S,GL_OBJECT_PLANE,xgenparam);
    glTexGenfv(GL_T,GL_OBJECT_PLANE,ygenparam);
    glEnable (GL_TEXTURE_GEN_S);
    glEnable (GL_TEXTURE_GEN_T);
    glEnable (GL_TEXTURE_2D);
    /* 描画 */
    glColor3f(0.7f,0.5f,0.3f);
    glBegin(GL_TRIANGLE_STRIP);glNormal3fv(n);
    glVertex3f(-0.5f*w,-0.5f*w,0); 
    glVertex3f( 0.5f*w,-0.5f*w,0);
    glVertex3f(-0.5f*w, 0.5f*w,0);
    glVertex3f( 0.5f*w, 0.5f*w,0);glEnd();
    /* テクスチャ使用終了 */
    glDisable (GL_TEXTURE_2D);}

/* 床を描くディスプレイリストの作成 */
GLuint texfloor_list(float w,float p=0.02f){
    GLuint list=glGenLists(1);
    glNewList(list,GL_COMPILE);texfloor_draw(w,p);glEndList();
    return list;}

/*------------------------------------------------------------*/
/* 座標軸を描く関数 */
void axis_draw(float len){
    glBegin(GL_LINES);
    glColor3f(1.0f,0.5f,0.5f);
    glVertex3f(0,0,0);glVertex3f(len,0,0);
    glColor3f(0.5f,1.0f,0.5f);
    glVertex3f(0,0,0);glVertex3f(0,len,0);
    glColor3f(0.5f,0.5f,1.0f);
    glVertex3f(0,0,0);glVertex3f(0,0,len);
    glEnd();}

/* 座標軸を描くディスプレイリストの作成 */
GLuint axis_list(float len){
    GLuint list=glGenLists(1);
    glNewList(list,GL_COMPILE);axis_draw(len);glEndList();
    return list;}

/*------------------------------------------------------------*/
/* Polhemus Sensor */
/* センサ形状 */
/* 原点はセンサの計測中心 */
float pol_RX1_base[2][7][3]={
   {{ 0.00764f, 0.00754f, 0.00795f},
    {-0.00318f, 0.01414f, 0.00795f},
    {-0.01461f, 0.00754f, 0.00795f},
    {-0.01461f,-0.00754f, 0.00795f},
    {-0.00318f,-0.01414f, 0.00795f},
    { 0.00764f,-0.00754f, 0.00795f},
    { 0.00764f, 0.00754f, 0.00795f}},
   {{ 0.00764f, 0.00754f, 0.00160f},
    {-0.00318f, 0.01414f, 0.00160f},
    {-0.01461f, 0.00754f, 0.00160f},
    {-0.01461f,-0.00754f, 0.00160f},
    {-0.00318f,-0.01414f, 0.00160f},
    { 0.00764f,-0.00754f, 0.00160f},
    { 0.00764f, 0.00754f, 0.00160f}}};
float pol_RX1_top[2][5][3]={
   {{ 0.00764f, 0.00754f, 0.00160f},
    {-0.01461f, 0.00754f, 0.00160f},
    {-0.01461f,-0.00754f, 0.00160f},
    { 0.00764f,-0.00754f, 0.00160f},
    { 0.00764f, 0.00754f, 0.00160f}},
   {{ 0.00764f, 0.00754f,-0.00729f},
    {-0.00572f, 0.00754f,-0.00729f},
    {-0.00572f,-0.00754f,-0.00729f},
    { 0.00764f,-0.00754f,-0.00729f},
    { 0.00764f, 0.00754f,-0.00729f}}};
/* 描画 */
void pol_RX1_draw(void){
    glBegin(GL_LINE_STRIP);
    for(int i=0;i<7;i++){glVertex3fv(pol_RX1_base[0][i]);}
    for(int i=0;i<7;i++){glVertex3fv(pol_RX1_base[1][i]);}
    for(int i=0;i<5;i++){glVertex3fv(pol_RX1_top[0][i]);}
    for(int i=0;i<5;i++){glVertex3fv(pol_RX1_top[1][i]);}
    glEnd();
    glBegin(GL_LINES);
    for(int i=0;i<6;i++){
        glVertex3fv(pol_RX1_base[0][i]);
        glVertex3fv(pol_RX1_base[1][i]);}
    for(int i=0;i<4;i++){
        glVertex3fv(pol_RX1_top[0][i]);
        glVertex3fv(pol_RX1_top[1][i]);}
    glEnd();}
/* ディスプレイリストの作成 */
GLuint pol_RX1_list(void){
    GLuint list=glGenLists(1);
    glNewList(list,GL_COMPILE);pol_RX1_draw();glEndList();
    return list;}

/*--------------------------------------------------------------
    hand.cpp用
--------------------------------------------------------------*/
/* 半球を描く関数 */
void hsp_draw(float rad,int lon=12,int lat=3){
    Array3d<float> n(lat+1,lon,3);
    Array3d<float> p(lat+1,lon,3);
    float tla,tlo,sla,cla,slo,clo;
    /* ポリゴンの頂点位置と法線を計算します */
    for(int i=0;i<lat+1;i++)for(int j=0;j<lon;j++){
        tla=((float)i/(float)lat)*0.5f*(float)M_PI;
        tlo=0-((float)j/(float)lon)*2.0f*(float)M_PI;
        sla=sinf(tla);cla=cosf(tla);
        slo=sinf(tlo);clo=cosf(tlo);
        n.a(i,j,0)=(float)(sla*clo); p.a(i,j,0)=n.a(i,j,0)*rad;
        n.a(i,j,1)=(float)(cla);     p.a(i,j,1)=n.a(i,j,1)*rad;
        n.a(i,j,2)=(float)(sla*slo); p.a(i,j,2)=n.a(i,j,2)*rad;}
    /* 北極は三角形の扇形 */
    glBegin(GL_TRIANGLE_FAN);
    glNormal3fv(n.a(0,0));glVertex3fv(p.a(0,0));
    for(int j=0;j<lon;j++){
        glNormal3fv(n.a(1,j));glVertex3fv(p.a(1,j));}
    glNormal3fv(n.a(1,0));glVertex3fv(p.a(1,0));
    glEnd();
    /* draw middle */
    /* 北極から赤道までは三角形の連続パッチで書きます */
    for(int i=1;i<lat;i++){
        glBegin(GL_TRIANGLE_STRIP);
        for(int j=0;j<lon;j++){
            glNormal3fv(n.a(i,j));glVertex3fv(p.a(i,j));
            glNormal3fv(n.a(i+1,j));glVertex3fv(p.a(i+1,j));}
        glNormal3fv(n.a(i,0));glVertex3fv(p.a(i,0));
        glNormal3fv(n.a(i+1,0));glVertex3fv(p.a(i+1,0));
        glEnd();}}

/* 半球を描くディスプレイリストの作成 */
GLuint hsp_list(float rad){GLuint list;
    list=glGenLists(1);
    glNewList(list,GL_COMPILE);hsp_draw(rad);glEndList();
    return list;}

/*------------------------------------------------------------*/
/* 円柱を描く関数 */
void cyl_draw(float rad,float height,int lon=12,int lat=3){
    Array3d<float> n(1,lon,3);
    Array3d<float> p(2,lon,3);
    /* ポリゴンの頂点位置と法線を計算します */
    for(int j=0;j<lon;j++){
        float tlo=((float)j/(float)lon)*2*(float)M_PI;
        float slo=sinf(tlo);
        float clo=cosf(tlo);
        n.a(0,j,0)=(float)(clo);
        n.a(0,j,1)=(float)(0);
        n.a(0,j,2)=(float)(slo);
        p.a(0,j,0)=p.a(1,j,0)=n.a(0,j,0)*rad;
        p.a(0,j,1)=0; p.a(1,j,1)=height;
        p.a(0,j,2)=p.a(1,j,2)=n.a(0,j,2)*rad;}
    /* draw */
    glBegin(GL_TRIANGLE_STRIP);
    for(int j=0;j<lon;j++){
        glNormal3fv(n.a(0,j));glVertex3fv(p.a(0,j));
        glNormal3fv(n.a(0,j));glVertex3fv(p.a(1,j));}
    glNormal3fv(n.a(0,0));glVertex3fv(p.a(0,0));
    glNormal3fv(n.a(0,0));glVertex3fv(p.a(1,0));glEnd();}

/* 円柱を描くディスプレイリストの作成 */
GLuint cyl_list(float rad,float height){GLuint list;
    list=glGenLists(1);
    glNewList(list,GL_COMPILE);cyl_draw(rad,height);glEndList();
    return list;}

/*------------------------------------------------------------*/
/* 円を描く関数 */
void circle_draw(float rad,int div=36){
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0,0,0);
    for(int i=0;i<(div+1);i++){
        float th=pi20*((float)i/(float)div);
        float s=sinf(th);
        float c=cosf(th);
        glVertex3f(c*rad,s*rad,0);}
    glEnd();}
/* 円を描くディスプレイリストの作成 */
GLuint circle_list(float rad,int div=36){
    GLuint list=glGenLists(1);
    glNewList(list,GL_COMPILE);circle_draw(rad,div);glEndList();
    return list;}

/*------------------------------------------------------------*/
/* 文字列を描く関数 */
void text_draw(char *text,float s=0.0001f){
    glPushMatrix();
    glScaled(s,s,s);
    for(char *p=text;*p!=0;p++){
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,*p);}
    glPopMatrix();}

/*--------------------------------------------------------------
    テスト用メイン
--------------------------------------------------------------*/
#if 0
/* ディスプレイリストの定義 */
unsigned int l_box,l_cube,l_sphere,l_cylinder;
unsigned int l_floor,l_texfloor,l_axis;
/* イベント関数 */
void my_init(void){
    l_box=box_list(0.1f,0.08f,0.12f);
    l_cube=cube_list(0.08f);
    l_sphere=sphere_list(0.1f,72,36);
    l_cylinder=cylinder_list(0.03f,0.12f,72,36);
    l_floor=floor_list(0.2f);
    l_texfloor=texfloor_list(0.2f,0.02f);
    l_axis=axis_list(0.1f);
}
void my_draw(void){
    glEnable(GL_LIGHTING);
    material_set(materialg);
    //glCallList(l_box);
    //glCallList(l_cube);
    //glCallList(l_sphere);
    //glCallList(l_cylinder);
    glDisable(GL_LIGHTING);
    glColor3f(1.0f,1.0f,0.0f);  
    //glCallList(l_floor);
    glEnable(GL_LIGHTING);
    material_set(materialw);
    //glCallList(l_texfloor);
    glDisable(GL_LIGHTING);
    glCallList(l_axis);
}
void my_idle(void){}
void my_exit(void){}
void my_key(unsigned char){}

#endif
/*--------------------------------------------------------------
--------------------------------------------------------------*/
#endif /*_GLSHAPE_CPP_*/
