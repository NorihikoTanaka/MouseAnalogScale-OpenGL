#ifndef _VEC3MAT33C2_CPP_
#define _VEC3MAT33C2_CPP_

#define _USE_MATH_DEFINES
#include<math.h>
/*--------------------------------------------------------------
履歴
2013.12.15 cuda用に前置子？を追加
2014.6.11 クラスメソッドDiag()、Rn()などを追加
2014.6.19 定数の定義を追加、mat44::mult_vec3()を追加
2014.12.2 mrt3を追加
2014.12.3 定義全体をテンプレートクラス化
2015.2.11 Mat44.get()を追加
2017.9.19 mrt3のコンストラクタにHOST DEVICE修飾を付加
～
2019.8.29 Vec3のlength()をrealの型により別に定義
--------------------------------------------------------------*/

/*--------------------------------------------------------------
    定数など
--------------------------------------------------------------*/
/* util const */
#define DEG_TO_RAD (M_PI/180.0)
#define RAD_TO_DEG (180.0/M_PI)

/* util const */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef DEG_TO_RAD
#define DEG_TO_RAD (M_PI/180.0)
#define RAD_TO_DEG (180.0/M_PI)
#endif
#ifndef MM_TO_M
#define MM_TO_M 0.001
#define M_TO_MM 1000.0
#define INCH_TO_MM 25.4
#endif

/* CUDAの使用 */
#ifdef __CUDACC__
#define HOST __host__
#define DEVICE __device__ __inline__
#else
#define HOST
#define DEVICE
#endif

/* クラスの仮宣言 */
template<class real> class Vec3;
template<class real> class Mat33;
template<class real> class Mat44;
template<class real> class Mrt3;

#define MAT33_SMALL_VALUE (1.0e-10)
/*--------------------------------------------------------------
    Vec3クラス
--------------------------------------------------------------*/
template<class real>
class Vec3{
    friend class Mat33<real>;
    friend class Mat44<real>;
public:
    /* データ */
    real v[3];
    /* コンストラクタ */
    HOST DEVICE Vec3<real>(void);
    HOST DEVICE Vec3<real>(real x,real y,real z);
    /* クリア */
    HOST DEVICE Vec3<real>& zero(void);
    /* 値の設定 */
    //HOST DEVICE Vec3<real>& set(real s[3]);
    HOST DEVICE Vec3<real>& set(float s[3]);
    HOST DEVICE Vec3<real>& set(double s[3]);
    //HOST DEVICE void get(real s[3]);
    HOST DEVICE void get(float s[3]);
    HOST DEVICE void get(double s[3]);
    HOST DEVICE Vec3<real>& set(real x,real y,real z);
    /* 値の参照 */
    HOST DEVICE real& a(int i);
    HOST DEVICE real& x();
    HOST DEVICE real& y();
    HOST DEVICE real& z();
    HOST DEVICE real& operator[](int i);
    /* 加減算 */
    HOST DEVICE Vec3<real> operator+(Vec3<real> sv);
    HOST DEVICE Vec3<real>& operator+=(Vec3<real> sv);
    HOST DEVICE Vec3<real> operator-(Vec3<real> sv);
    HOST DEVICE Vec3<real>& operator-=(Vec3<real> sv);
    /* 定数倍 */
    HOST DEVICE Vec3<real> operator*(real a);
    HOST DEVICE Vec3<real>& operator*=(real a);
    /* 内積/外積 */
    HOST DEVICE real operator,(Vec3<real> sv);
    HOST DEVICE Vec3<real> operator*(Vec3<real> sv);
    /* 正規化 */
    HOST DEVICE Vec3<real> norm(void);
    HOST DEVICE Vec3<real>& norm_self(void);
    /* 長さ */
    HOST DEVICE real length(void);
    HOST DEVICE real length2(void);
    /* 表示 */
    HOST void print(const char* name=NULL);
    /* クラスメソッド */
    HOST DEVICE static Vec3<real> Z(void);
};

/*------------------------------------------------------------*/
/* コンストラクタ */
template<class real>
HOST DEVICE Vec3<real>::Vec3(void){}
template<class real>
HOST DEVICE Vec3<real>::Vec3(real x,real y,real z){set(x,y,z);}
/* クリア */
template<class real>
HOST DEVICE Vec3<real>& Vec3<real>::zero(void){
    v[0]=v[1]=v[2]=0;
    return *this;}
/* 値の設定 */
template<class real>
HOST DEVICE Vec3<real>& Vec3<real>::set(float s[3]){
    for(int i=0;i<3;i++){v[i]=(real)s[i];} return *this;}
template<class real>
HOST DEVICE Vec3<real>& Vec3<real>::set(double s[3]){
    for(int i=0;i<3;i++){v[i]=(real)s[i];} return *this;}
template<class real>
HOST DEVICE void Vec3<real>::get(float s[3]){
    for(int i=0;i<3;i++){s[i]=(float)v[i];}}
template<class real>
HOST DEVICE void Vec3<real>::get(double s[3]){
    for(int i=0;i<3;i++){s[i]=(double)v[i];}}
template<class real>
HOST DEVICE Vec3<real>& Vec3<real>::set(real x,real y,real z){
    v[0]=x;v[1]=y;v[2]=z; return *this;}
/* 値の参照 */
template<class real>
HOST DEVICE real& Vec3<real>::a(int i){return v[i];}
template<class real>
HOST DEVICE real& Vec3<real>::x(){return v[0];}
template<class real>
HOST DEVICE real& Vec3<real>::y(){return v[1];}
template<class real>
HOST DEVICE real& Vec3<real>::z(){return v[2];}
template<class real>
HOST DEVICE real& Vec3<real>::operator[](int i){return v[i];}
/* 加減算 */
template<class real>
HOST DEVICE Vec3<real> Vec3<real>::operator+(Vec3<real> sv){
    Vec3<real> tmp; real *d,*s1,*s2; d=tmp.v; s1=v; s2=sv.v;
    for(int i=0;i<3;i++){d[i]=s1[i]+s2[i];}
    return tmp;}
template<class real>
HOST DEVICE Vec3<real>& Vec3<real>::operator+=(Vec3<real> sv){
    real *d,*s; d=v; s=sv.v;
    for(int i=0;i<3;i++){d[i]+=s[i];}
    return *this;}
template<class real>
HOST DEVICE Vec3<real> Vec3<real>::operator-(Vec3<real> sv){
    Vec3<real> tmp; real *d,*s1,*s2; d=tmp.v; s1=v; s2=sv.v;
    for(int i=0;i<3;i++){d[i]=s1[i]-s2[i];}
    return tmp;}
template<class real>
HOST DEVICE Vec3<real>& Vec3<real>::operator-=(Vec3<real> sv){
    real *d,*s; d=v; s=sv.v;
    for(int i=0;i<3;i++){d[i]-=s[i];}
    return *this;}
/* 定数倍 */
template<class real>
HOST DEVICE Vec3<real> Vec3<real>::operator*(real a){
    Vec3<real> tmp; real *d,*s; d=tmp.v; s=v;
    for(int i=0;i<3;i++){d[i]=s[i]*a;}
    return tmp;}
template<class real>
HOST DEVICE Vec3<real>& Vec3<real>::operator*=(real a){
    for(int i=0;i<3;i++){v[i]*=a;}
    return *this;}
/* 内積/外積 */
template<class real>
HOST DEVICE real Vec3<real>::operator,(Vec3<real> sv){
    real *s1,*s2; s1=v; s2=sv.v;
    return (s1[0]*s2[0]+s1[1]*s2[1]+s1[2]*s2[2]);}
template<class real>
HOST DEVICE Vec3<real> Vec3<real>::operator*(Vec3<real> sv){
    Vec3<real> tmp; real *d,*s1,*s2; d=tmp.v; s1=v; s2=sv.v;
    d[0]=s1[1]*s2[2]-s1[2]*s2[1];
    d[1]=s1[2]*s2[0]-s1[0]*s2[2];
    d[2]=s1[0]*s2[1]-s1[1]*s2[0];
    return tmp;}
/* 正規化 */
#if 0
template<class real>
HOST DEVICE Vec3<real> Vec3<real>::norm(void){
    Vec3<real> tmp; real div,*d,*s; d=tmp.v;s=v;
    div=1/(sqrt(s[0]*s[0]+s[1]*s[1]+s[2]*s[2]));
    d[0]=s[0]*div;d[1]=s[1]*div;d[2]=s[2]*div;
    return tmp;}
template<class real>
HOST DEVICE Vec3<real>& Vec3<real>::norm_self(void){
    real div,*s; s=v;
    div=1/(sqrt(s[0]*s[0]+s[1]*s[1]+s[2]*s[2]));
    s[0]*=div;s[1]*=div;s[2]*=div;
    return *this;}
#endif
template<>
HOST DEVICE Vec3<float> Vec3<float>::norm(void){
    Vec3<float> tmp; float div,*d,*s; d=tmp.v;s=v;
    div=1/(sqrtf(s[0]*s[0]+s[1]*s[1]+s[2]*s[2]));
    d[0]=s[0]*div;d[1]=s[1]*div;d[2]=s[2]*div;
    return tmp;}
template<>
HOST DEVICE Vec3<double> Vec3<double>::norm(void){
    Vec3<double> tmp; double div,*d,*s; d=tmp.v;s=v;
    div=1/(sqrt(s[0]*s[0]+s[1]*s[1]+s[2]*s[2]));
    d[0]=s[0]*div;d[1]=s[1]*div;d[2]=s[2]*div;
    return tmp;}
template<>
HOST DEVICE Vec3<float>& Vec3<float>::norm_self(void){
    float div,*s; s=v;
    div=1/(sqrtf(s[0]*s[0]+s[1]*s[1]+s[2]*s[2]));
    s[0]*=div;s[1]*=div;s[2]*=div;
    return *this;}
template<>
HOST DEVICE Vec3<double>& Vec3<double>::norm_self(void){
    double div,*s; s=v;
    div=1/(sqrt(s[0]*s[0]+s[1]*s[1]+s[2]*s[2]));
    s[0]*=div;s[1]*=div;s[2]*=div;
    return *this;}
/* 長さ */
template<>
HOST DEVICE float Vec3<float>::length(void){
    float *s; s=v;
    return (sqrtf(s[0]*s[0]+s[1]*s[1]+s[2]*s[2]));}
template<>
HOST DEVICE double Vec3<double>::length(void){
    double *s; s=v;
    return (sqrt(s[0]*s[0]+s[1]*s[1]+s[2]*s[2]));}

template<class real>
HOST DEVICE real Vec3<real>::length2(void){
    real *s; s=v;
    return (s[0]*s[0]+s[1]*s[1]+s[2]*s[2]);}
/* 表示 */
template<class real>
HOST void Vec3<real>::print(const char* name){
    if(name!=NULL){printf("---- %s:\n",name);}
    printf("%8g %8g %8g\n",v[0],v[1],v[2]);}
/* クラスメソッド */
template<class real>
HOST DEVICE Vec3<real> Vec3<real>::Z(void){
    Vec3<real> tmp; tmp.zero();
    return tmp;}

/*--------------------------------------------------------------
    Mat33クラス
--------------------------------------------------------------*/
template<class real>
class Mat33{
    friend class Mat44<real>;
public:
    /* データ */
    real m[3][3];
    /* クリア */
    HOST DEVICE Mat33<real>& zero(void);
    HOST DEVICE Mat33<real>& identity(void);
    /* 値の設定 */
    //HOST DEVICE Mat33<real>& set(real s[3][3]);
    HOST DEVICE Mat33<real>& set(float s[3][3]);
    HOST DEVICE Mat33<real>& set(double s[3][3]);
    HOST DEVICE Mat33<real>& set(Vec3<real>& sv0,Vec3<real>& sv1,Vec3<real>& sv2);
    /* 値の参照 */
    HOST DEVICE real& a(int r,int c);
    /* 加減算 */
    HOST DEVICE Mat33<real> operator+(Mat33<real> sm);
    HOST DEVICE Mat33<real>& operator+=(Mat33<real> sm);
    HOST DEVICE Mat33<real> operator-(Mat33<real> sm);
    HOST DEVICE Mat33<real>& operator-=(Mat33<real> sm);
    /* 定数倍 */
    HOST DEVICE Mat33<real> operator*(real a);
    HOST DEVICE Mat33<real>& operator*=(real a);
    /* 逆行列・反転 */
    HOST DEVICE Mat33<real> inv(void);
    HOST DEVICE Mat33<real> trans(void);
    HOST DEVICE Mat33<real> operator~(void);
    /* 乗算 */
    HOST DEVICE Mat33<real> operator*(Mat33<real> sm);
    HOST DEVICE Mat33<real>& operator*=(Mat33<real> sm);
    HOST DEVICE Vec3<real> operator*(Vec3<real> sv);
    /* 値の取り出し */
    HOST DEVICE Vec3<real> Vx(void);
    HOST DEVICE Vec3<real> Vy(void);
    HOST DEVICE Vec3<real> Vz(void);
    /* 正規直交化 */
    HOST DEVICE Mat33<real>& norm_self(void);
    /* スケール/回転 */
    HOST DEVICE Mat33<real>& load_scale(real x,real y,real z);
    HOST DEVICE Mat33<real>& load_scale(real v[3]);
    HOST DEVICE Mat33<real>& load_rotx(real a);
    HOST DEVICE Mat33<real>& load_roty(real a);
    HOST DEVICE Mat33<real>& load_rotz(real a);
    /* 回転ベクトル */
    HOST DEVICE void rotvec(Vec3<real>& n, real& ang);
    /* 表示 */
    HOST void print(const char* name=NULL);
    /* クラスメソッド */
    HOST DEVICE static Mat33<real> I(void);
    HOST DEVICE static Mat33<real> Diag(real s);
    HOST DEVICE static Mat33<real> Diag(Vec3<real>& sv);
    HOST DEVICE static Mat33<real> Rx(real a);
    HOST DEVICE static Mat33<real> Ry(real a);
    HOST DEVICE static Mat33<real> Rz(real a);
    HOST DEVICE static Mat33<real> Rn(double x,double y,double z);
    HOST DEVICE static Mat33<real> Rn(Vec3<real> v);
};

/*------------------------------------------------------------*/
/* クリア */
template<class real>
HOST DEVICE Mat33<real>& Mat33<real>::zero(void){
    for(int c=0;c<3;c++){
        for(int r=0;r<3;r++){m[c][r]=0;}}
    return *this;}
template<class real>
HOST DEVICE Mat33<real>& Mat33<real>::identity(void){
    for(int c=0;c<3;c++){
        for(int r=0;r<3;r++){
            if(c==r){m[c][r]=1;}else{m[c][r]=0;}}}
    return *this;}
/* 値の設定 */
template<class real>
HOST DEVICE Mat33<real>& Mat33<real>::set(float s[3][3]){
    for(int c=0;c<3;c++){
        for(int r=0;r<3;r++){
            m[c][r]=(real)s[c][r];}}
    return *this;}
template<class real>
HOST DEVICE Mat33<real>& Mat33<real>::set(double s[3][3]){
    for(int c=0;c<3;c++){
        for(int r=0;r<3;r++){
            m[c][r]=(real)s[c][r];}}
    return *this;}
template<class real>
HOST DEVICE Mat33<real>& Mat33<real>::set(
    Vec3<real>& sv0,Vec3<real>& sv1,Vec3<real>& sv2){
    real *v0,*v1,*v2; v0=sv0.v;v1=sv1.v;v2=sv2.v;
    m[0][0]=v0[0];m[1][0]=v1[0];m[2][0]=v2[0];
    m[0][1]=v0[1];m[1][1]=v1[1];m[2][1]=v2[1];
    m[0][2]=v0[2];m[1][2]=v1[2];m[2][2]=v2[2];
    return *this;}
/* 値の参照 */
template<class real>
HOST DEVICE real& Mat33<real>::a(int r,int c){return m[c][r];}
/* 加減算 */
template<class real>
HOST DEVICE Mat33<real> Mat33<real>::operator+(Mat33<real> sm){
    Mat33<real> tmp; real (*d)[3],(*s1)[3],(*s2)[3];
    d=tmp.m; s1=m; s2=sm.m;
    for(int c=0;c<3;c++){
        for(int r=0;r<3;r++){
            d[c][r]=s1[c][r]+s2[c][r];}}
    return tmp;}
template<class real>
HOST DEVICE Mat33<real>& Mat33<real>::operator+=(Mat33<real> sm){
    real (*d)[3],(*s)[3]; d=m; s=sm.m;
    for(int c=0;c<3;c++){
        for(int r=0;r<3;r++){
            d[c][r]+=s[c][r];}}
    return *this;}
template<class real>
HOST DEVICE Mat33<real> Mat33<real>::operator-(Mat33<real> sm){
    Mat33<real> tmp; real (*d)[3],(*s1)[3],(*s2)[3];
    d=tmp.m; s1=m; s2=sm.m;
    for(int c=0;c<3;c++){
        for(int r=0;r<3;r++){
            d[c][r]=s1[c][r]-s2[c][r];}}
    return tmp;}
template<class real>
HOST DEVICE Mat33<real>& Mat33<real>::operator-=(Mat33<real> sm){
    real (*d)[3],(*s)[3]; d=m; s=sm.m;
    for(int c=0;c<3;c++){
        for(int r=0;r<3;r++){
            d[c][r]-=s[c][r];}}
    return *this;}
/* 定数倍 */
template<class real>
HOST DEVICE Mat33<real> Mat33<real>::operator*(real a){
    Mat33<real> tmp; real (*d)[3],(*s)[3];
    d=tmp.m; s=m;
    for(int c=0;c<3;c++){
        for(int r=0;r<3;r++){
            d[c][r]=s[c][r]*a;}}
    return tmp;}
template<class real>
HOST DEVICE Mat33<real>& Mat33<real>::operator*=(real a){
    for(int c=0;c<3;c++){
        for(int r=0;r<3;r++){
            m[c][r]*=a;}}
    return *this;}

/* 逆行列・反転 */
template<class real>
HOST DEVICE Mat33<real> Mat33<real>::inv(void){real det;
    Mat33<real> tmp; real (*d)[3],(*s)[3]; d=tmp.m;s=m;
    det=1/(
    -s[0][2]*s[1][1]*s[2][0] +s[0][1]*s[1][2]*s[2][0]
    +s[0][2]*s[1][0]*s[2][1] -s[0][0]*s[1][2]*s[2][1]
    -s[0][1]*s[1][0]*s[2][2] +s[0][0]*s[1][1]*s[2][2]);
    d[0][0]=(-s[1][2]*s[2][1]+s[1][1]*s[2][2])*det;
    d[0][1]=( s[0][2]*s[2][1]-s[0][1]*s[2][2])*det;
    d[0][2]=(-s[0][2]*s[1][1]+s[0][1]*s[1][2])*det;
    d[1][0]=( s[1][2]*s[2][0]-s[1][0]*s[2][2])*det;
    d[1][1]=(-s[0][2]*s[2][0]+s[0][0]*s[2][2])*det;
    d[1][2]=( s[0][2]*s[1][0]-s[0][0]*s[1][2])*det;
    d[2][0]=(-s[1][1]*s[2][0]+s[1][0]*s[2][1])*det;
    d[2][1]=( s[0][1]*s[2][0]-s[0][0]*s[2][1])*det;
    d[2][2]=(-s[0][1]*s[1][0]+s[0][0]*s[1][1])*det;
    return tmp;}
template<class real>
HOST DEVICE Mat33<real> Mat33<real>::trans(void){
    Mat33<real> tmp; real (*d)[3],(*s)[3]; d=tmp.m;s=m;
    d[0][0]=s[0][0];d[0][1]=s[1][0];d[0][2]=s[2][0];
    d[1][0]=s[0][1];d[1][1]=s[1][1];d[1][2]=s[2][1];
    d[2][0]=s[0][2];d[2][1]=s[1][2];d[2][2]=s[2][2];
    return tmp;}
template<class real>
HOST DEVICE Mat33<real> Mat33<real>::operator~(void){
    return (*this).trans();}
/* 乗算 */
template<class real>
HOST DEVICE Mat33<real> Mat33<real>::operator*(Mat33<real> sm){
    Mat33<real> tmp; real (*s1)[3],(*s2)[3],(*d)[3];
    d=tmp.m;s1=m;s2=sm.m;
    for(int c=0;c<3;c++){
        for(int r=0;r<3;r++){
            d[c][r]=s1[0][r]*s2[c][0]
                +s1[1][r]*s2[c][1]+s1[2][r]*s2[c][2];}}
    return tmp;}
template<class real>
HOST DEVICE Mat33<real>& Mat33<real>::operator*=(Mat33<real> sm){
    Mat33<real> tmp; real (*s1)[3],(*s2)[3],(*d)[3];
    d=tmp.m;s1=m;s2=sm.m;
    for(int c=0;c<3;c++){
        for(int r=0;r<3;r++){
            d[c][r]=s1[0][r]*s2[c][0]
                +s1[1][r]*s2[c][1]+s1[2][r]*s2[c][2];}}
    (*this)=tmp;
    return *this;}
template<class real>
HOST DEVICE Vec3<real> Mat33<real>::operator*(Vec3<real> sv){
    Vec3<real> tmp; real (*s)[3],*v,*d;
    d=tmp.v; s=m, v=sv.v;
    d[0]=s[0][0]*v[0]+s[1][0]*v[1]+s[2][0]*v[2];
    d[1]=s[0][1]*v[0]+s[1][1]*v[1]+s[2][1]*v[2];
    d[2]=s[0][2]*v[0]+s[1][2]*v[1]+s[2][2]*v[2];
    return tmp;}
/* 値の取り出し */
template<class real>
HOST DEVICE Vec3<real> Mat33<real>::Vx(void){
    Vec3<real> tmp; real *d,*s; d=tmp.v; s=m[0];
    d[0]=s[0];d[1]=s[1];d[2]=s[2];
    return tmp;}
template<class real>
HOST DEVICE Vec3<real> Mat33<real>::Vy(void){
    Vec3<real> tmp; real *d,*s; d=tmp.v; s=m[1];
    d[0]=s[0];d[1]=s[1];d[2]=s[2];
    return tmp;}
template<class real>
HOST DEVICE Vec3<real> Mat33<real>::Vz(void){
    Vec3<real> tmp; real *d,*s; d=tmp.v; s=m[2];
    d[0]=s[0];d[1]=s[1];d[2]=s[2];
    return tmp;}
/* 正規直交化 */
template<>
HOST DEVICE Mat33<double>& Mat33<double>::norm_self(void){
    double *v0,*v1,*v2,div;
    v0=m[0];v1=m[1];v2=m[2];
    v1[0]=v2[1]*v0[2]-v2[2]*v0[1];/*y=z*x*/
    v1[1]=v2[2]*v0[0]-v2[0]*v0[2];
    v1[2]=v2[0]*v0[1]-v2[1]*v0[0];
    v2[0]=v0[1]*v1[2]-v0[2]*v1[1];/*z=x*y*/
    v2[1]=v0[2]*v1[0]-v0[0]*v1[2];
    v2[2]=v0[0]*v1[1]-v0[1]*v1[0];
    div=1/(sqrt(v0[0]*v0[0] +v0[1]*v0[1] +v0[2]*v0[2]));
    v0[0]*=div;v0[1]*=div;v0[2]*=div;
    div=1/(sqrt(v1[0]*v1[0] +v1[1]*v1[1] +v1[2]*v1[2]));
    v1[0]*=div;v1[1]*=div;v1[2]*=div;
    div=1/(sqrt(v2[0]*v2[0] +v2[1]*v2[1] +v2[2]*v2[2]));
    v2[0]*=div;v2[1]*=div;v2[2]*=div;
    return *this;}
template<>
HOST DEVICE Mat33<float>& Mat33<float>::norm_self(void){
    float *v0,*v1,*v2,div;
    v0=m[0];v1=m[1];v2=m[2];
    v1[0]=v2[1]*v0[2]-v2[2]*v0[1];/*y=z*x*/
    v1[1]=v2[2]*v0[0]-v2[0]*v0[2];
    v1[2]=v2[0]*v0[1]-v2[1]*v0[0];
    v2[0]=v0[1]*v1[2]-v0[2]*v1[1];/*z=x*y*/
    v2[1]=v0[2]*v1[0]-v0[0]*v1[2];
    v2[2]=v0[0]*v1[1]-v0[1]*v1[0];
    div=1/(sqrtf(v0[0]*v0[0] +v0[1]*v0[1] +v0[2]*v0[2]));
    v0[0]*=div;v0[1]*=div;v0[2]*=div;
    div=1/(sqrtf(v1[0]*v1[0] +v1[1]*v1[1] +v1[2]*v1[2]));
    v1[0]*=div;v1[1]*=div;v1[2]*=div;
    div=1/(sqrtf(v2[0]*v2[0] +v2[1]*v2[1] +v2[2]*v2[2]));
    v2[0]*=div;v2[1]*=div;v2[2]*=div;
    return *this;}
/* スケール/回転 */
template<class real>
HOST DEVICE Mat33<real>& Mat33<real>::load_scale(real x,real y,real z){
    zero();m[0][0]=x;m[1][1]=y;m[2][2]=z;
    return *this;}
template<class real>
HOST DEVICE Mat33<real>& Mat33<real>::load_scale(real v[3]){
    zero();m[0][0]=v[0];m[1][1]=v[1];m[2][2]=v[2];
    return *this;}
template<class real>
HOST DEVICE Mat33<real>& Mat33<real>::load_rotx(real a){
    real c,s; c=cos(a);s=sin(a); identity();
    m[1][1]=c;m[1][2]=s;m[2][1]=-s;m[2][2]=c;
    return *this;}
template<class real>
HOST DEVICE Mat33<real>& Mat33<real>::load_roty(real a){
    real c,s; c=cos(a);s=sin(a); identity();
    m[2][2]=c;m[2][0]=s;m[0][2]=-s;m[0][0]=c;
    return *this;}
template<class real>
HOST DEVICE Mat33<real>& Mat33<real>::load_rotz(real a){
    real c,s; c=cos(a);s=sin(a); identity();
    m[0][0]=c;m[0][1]=s;m[1][0]=-s;m[1][1]=c;
    return *this;}
/* 回転ベクトル */
/* 行列は回転変換であること */
template<class real>
HOST DEVICE void Mat33<real>::rotvec(Vec3<real>& n, real& ang){
    double c=(m[0][0]+m[1][1]+m[2][2]-1)*0.5;
    n[0]=m[1][2]-m[2][1];
    n[1]=m[2][0]-m[0][2];
    n[2]=m[0][1]-m[1][0];
    double s =n.length()*0.5;
    /* 特異点でない */
    if(fabs(s)>MAT33_SMALL_VALUE){
        ang=(real)atan2(s,c);
        n*=(real)(1/(s*2));}
    /* 特異点(ang=0 or 180 deg) */
    /**** 不完全：180degの場合の処理が未実装 ****/
    else{
        ang=0;
        n.set(1,0,0);}}
/* 表示 */
template<class real>
HOST void Mat33<real>::print(const char* name){
    if(name!=NULL){printf("---- %s:\n",name);}
    printf("%8g %8g %8g\n",m[0][0],m[1][0],m[2][0]);
    printf("%8g %8g %8g\n",m[0][1],m[1][1],m[2][1]);
    printf("%8g %8g %8g\n",m[0][2],m[1][2],m[2][2]);}

/*------------------------------------------------------------*/
/* クラスメソッド */
template<class real>
HOST DEVICE Mat33<real> Mat33<real>::I(void){
    Mat33<real> tmp; tmp.zero();
    tmp.m[0][0]=tmp.m[1][1]=tmp.m[2][2]=1;
    return tmp;}
template<class real>
HOST DEVICE Mat33<real> Mat33<real>::Diag(real s){
    Mat33<real> tmp; tmp.zero();
    tmp.m[0][0]=tmp.m[1][1]=tmp.m[2][2]=s;
    return tmp;}
template<class real>
HOST DEVICE Mat33<real> Mat33<real>::Diag(Vec3<real>& sv){
    Mat33<real> tmp; tmp.zero();
    tmp.m[0][0]=sv[0];tmp.m[1][1]=sv[1];tmp.m[2][2]=sv[2];
    return tmp;}
template<>
HOST DEVICE Mat33<double> Mat33<double>::Rx(double a){
    Mat33<double> tmp; tmp.zero();
    double c,s; c=cos(a);s=sin(a); 
    tmp.m[0][0]=1;
    tmp.m[1][1]=c;tmp.m[1][2]=s;tmp.m[2][1]=-s;tmp.m[2][2]=c;
    return tmp;}
template<>
HOST DEVICE Mat33<float> Mat33<float>::Rx(float a){
    Mat33<float> tmp; tmp.zero();
    float c,s; c=cosf(a);s=sinf(a); 
    tmp.m[0][0]=1;
    tmp.m[1][1]=c;tmp.m[1][2]=s;tmp.m[2][1]=-s;tmp.m[2][2]=c;
    return tmp;}
template<>
HOST DEVICE Mat33<double> Mat33<double>::Ry(double a){
    Mat33<double> tmp; tmp.zero();
    double c,s; c=cos(a);s=sin(a);
    tmp.m[1][1]=1;
    tmp.m[2][2]=c;tmp.m[2][0]=s;tmp.m[0][2]=-s;tmp.m[0][0]=c;
    return tmp;}
template<>
HOST DEVICE Mat33<float> Mat33<float>::Ry(float a){
    Mat33<float> tmp; tmp.zero();
    float c,s; c=cosf(a);s=sinf(a);
    tmp.m[1][1]=1;
    tmp.m[2][2]=c;tmp.m[2][0]=s;tmp.m[0][2]=-s;tmp.m[0][0]=c;
    return tmp;}
template<>
HOST DEVICE Mat33<double> Mat33<double>::Rz(double a){
    Mat33<double> tmp; tmp.identity();
    double c,s; c=cos(a);s=sin(a);
    tmp.m[2][2]=1;
    tmp.m[0][0]=c;tmp.m[0][1]=s;tmp.m[1][0]=-s;tmp.m[1][1]=c;
    return tmp;}
template<>
HOST DEVICE Mat33<float> Mat33<float>::Rz(float a){
    Mat33<float> tmp; tmp.identity();
    float c,s; c=cosf(a);s=sinf(a);
    tmp.m[2][2]=1;
    tmp.m[0][0]=c;tmp.m[0][1]=s;tmp.m[1][0]=-s;tmp.m[1][1]=c;
    return tmp;}
template<class real>
HOST DEVICE Mat33<real> Mat33<real>::Rn(double x,double y,double z){
    double mag,div,s,c,xx,yy,zz,xy,yz,zx,xs,ys,zs,oc;Mat33<real> tmp;
    if((mag=sqrt(x*x+y*y+z*z))<MAT33_SMALL_VALUE){tmp.identity();return tmp;}
    div=1/mag; x*=div;y*=div;z*=div;
    s=sin(mag);c=cos(mag);
    xx=x*x;xy=x*y;xs=x*s;
    yy=y*y;yz=y*z;ys=y*s;
    zz=z*z;zx=z*x;zs=z*s;oc=1-c;
    tmp.m[0][0]=(real)((oc*xx)+c);
    tmp.m[1][0]=(real)((oc*xy)-zs);
    tmp.m[2][0]=(real)((oc*zx)+ys);
    tmp.m[0][1]=(real)((oc*xy)+zs);
    tmp.m[1][1]=(real)((oc*yy)+c);
    tmp.m[2][1]=(real)((oc*yz)-xs);
    tmp.m[0][2]=(real)((oc*zx)-ys);
    tmp.m[1][2]=(real)((oc*yz)+xs);
    tmp.m[2][2]=(real)((oc*zz)+c);
    return tmp;}
template<class real>
HOST DEVICE Mat33<real> Mat33<real>::Rn(Vec3<real> v){
    return Mat33<real>::Rn(v[0],v[1],v[2]);}

/*--------------------------------------------------------------
    Mat44<real>クラス
--------------------------------------------------------------*/
template<class real>
class Mat44{
public:
    /* データ */
    real m[4][4];
    /* クリア */
    HOST DEVICE Mat44<real>& zero(void);
    HOST DEVICE Mat44<real>& identity(void);
    /* 値の設定 */
    HOST DEVICE Mat44<real>& set(Mat33<double>& r,Vec3<double>& t);
    HOST DEVICE Mat44<real>& set(Mat33<float>& r,Vec3<float>& t);
    HOST DEVICE Mat44<real>& set(double s[4][4]);
    HOST DEVICE Mat44<real>& set(float s[4][4]);
    HOST DEVICE Mat44<real>& get(double s[4][4]);
    HOST DEVICE Mat44<real>& get(float s[4][4]);
    /* 値の参照 */
    HOST DEVICE real& a(int r,int c);
    /* 値の設定/取り出し */
    HOST DEVICE Mat33<real> R(void);
    HOST DEVICE Vec3<real> T(void);
    /* 演算 */
    HOST DEVICE Vec3<real> mult_vec3(Vec3<real>& sv);
    /* 表示 */
    HOST void print(const char* name=NULL);
};

/*------------------------------------------------------------*/
/* クリア */
template<class real>
HOST DEVICE Mat44<real>& Mat44<real>::zero(void){
    for(int c=0;c<4;c++){
        for(int r=0;r<4;r++){m[c][r]=0;}}
    return *this;}
template<class real>
HOST DEVICE Mat44<real>& Mat44<real>::identity(void){
    for(int c=0;c<4;c++){
        for(int r=0;r<4;r++){
            if(c==r){m[c][r]=1;}else{m[c][r]=0;}}}
    return *this;}
/* 値の設定 */
template<class real>
HOST DEVICE Mat44<real>& Mat44<real>::set(Mat33<double>& rm,Vec3<double>& tv){
    double (*rr)[3],*tt; rr=rm.m; tt=tv.v;
    for(int c=0;c<3;c++){
        for(int r=0;r<3;r++){m[c][r]=(real)rr[c][r];}}
    for(int r=0;r<3;r++){m[3][r]=(real)tt[r];}
    m[0][3]=m[1][3]=m[2][3]=(real)0;m[3][3]=(real)1;
    return *this;}

template<class real>
HOST DEVICE Mat44<real>& Mat44<real>::set(Mat33<float>& rm,Vec3<float>& tv){
    float (*rr)[3],*tt; rr=rm.m; tt=tv.v;
    for(int c=0;c<3;c++){
        for(int r=0;r<3;r++){m[c][r]=(real)rr[c][r];}}
    for(int r=0;r<3;r++){m[3][r]=(real)tt[r];}
    m[0][3]=m[1][3]=m[2][3]=0;m[3][3]=(real)1;
    return *this;}

template<class real>
HOST DEVICE Mat44<real>& Mat44<real>::set(double s[4][4]){
    for(int c=0;c<4;c++){
        for(int r=0;r<4;r++){m[c][r]=(real)s[c][r];}}
    return *this;}
template<class real>
HOST DEVICE Mat44<real>& Mat44<real>::set(float s[4][4]){
    for(int c=0;c<4;c++){
        for(int r=0;r<4;r++){m[c][r]=(real)s[c][r];}}
    return *this;}
template<class real>
HOST DEVICE Mat44<real>& Mat44<real>::get(double s[4][4]){
    for(int c=0;c<4;c++){
        for(int r=0;r<4;r++){s[c][r]=(double)m[c][r];}}
    return *this;}
template<class real>
HOST DEVICE Mat44<real>& Mat44<real>::get(float s[4][4]){
    for(int c=0;c<4;c++){
        for(int r=0;r<4;r++){s[c][r]=(float)m[c][r];}}
    return *this;}
/* 値の参照 */
template<class real>
HOST DEVICE real& Mat44<real>::a(int r,int c){return m[c][r];}
/* 取り出し */
template<class real>
HOST DEVICE Mat33<real> Mat44<real>::R(void){
    Mat33<real> tmp; real (*rr)[3]; rr=tmp.m;
    for(int c=0;c<3;c++){
        for(int r=0;r<3;r++){rr[c][r]=m[c][r];}}
    return tmp;}
template<class real>
HOST DEVICE Vec3<real> Mat44<real>::T(void){
    Vec3<real> tmp; real *tt; tt=tmp.v;
    for(int r=0;r<3;r++){tt[r]=m[3][r];}
    return tmp;}
/* 演算 */
template<class real>
HOST DEVICE Vec3<real> Mat44<real>::mult_vec3(Vec3<real>& sv){
    Vec3<real> tmp; real (*s)[4],*v,*d;
    d=tmp.v; s=m, v=sv.v;
    d[0]=s[0][0]*v[0]+s[1][0]*v[1]+s[2][0]*v[2]+s[3][0];
    d[1]=s[0][1]*v[0]+s[1][1]*v[1]+s[2][1]*v[2]+s[3][1];
    d[2]=s[0][2]*v[0]+s[1][2]*v[1]+s[2][2]*v[2]+s[3][2];
    return tmp;}
/* 表示 */
template<class real>
HOST void Mat44<real>::print(const char* name){
    if(name!=NULL){printf("---- %s:\n",name);}
    printf("%8g %8g %8g %8g\n",m[0][0],m[1][0],m[2][0],m[3][0]);
    printf("%8g %8g %8g %8g\n",m[0][1],m[1][1],m[2][1],m[3][1]);
    printf("%8g %8g %8g %8g\n",m[0][2],m[1][2],m[2][2],m[3][2]);
    printf("%8g %8g %8g %8g\n",m[0][3],m[1][3],m[2][3],m[3][3]);}

/*--------------------------------------------------------------
    rt3クラス
--------------------------------------------------------------*/
template<class real>
class Mrt3{
public:
    /* データ */
    Mat33<real> r;Vec3<real> t;
    /* コンストラクタ */
    HOST DEVICE Mrt3<real>(void){}
    HOST DEVICE Mrt3<real>(Mat33<real> m,Vec3<real> v){r=m;t=v;}
    /* クリア */
    HOST DEVICE Mrt3<real>& identity(void);
    /* 値の設定 */
  //HOST DEVICE Mrt3<real>& set(Mat33<real> sr,Vec3<real> st);
    HOST DEVICE Mrt3<real>& set(Mat33<float> sr,Vec3<float> st);
    HOST DEVICE Mrt3<real>& set(Mat33<double> sr,Vec3<double> st);
    /* 反転 */
    HOST DEVICE Mrt3<real> trans(void);
    HOST DEVICE Mrt3<real> operator~(void);
    /* 乗算 */
    HOST DEVICE Mrt3<real> operator*(Mrt3<real> mv);
    HOST DEVICE Mrt3<real>& operator*=(Mrt3<real> mv);
    HOST DEVICE Mrt3<real> operator*(Mat33<real> m);
    HOST DEVICE Mrt3<real>& operator*=(Mat33<real> m);
    HOST DEVICE Mrt3<real> operator*(Vec3<real> v);
    HOST DEVICE Mrt3<real>& operator*=(Vec3<real> v);
    /* 変換 */
    HOST DEVICE Vec3<real> mult(Vec3<real> v);
    /* 表示 */
    HOST void print(const char* name=NULL);
};

/*------------------------------------------------------------*/
/* クリア */
template<class real>
HOST DEVICE Mrt3<real>& Mrt3<real>::identity(void){
    r.identity();t.zero();
    return *this;}
/* 値の設定 */
//template<class real>
//HOST DEVICE Mrt3<real>& Mrt3<real>::set(
//    Mat33<real> sr,Vec3<real> st){
//    r=sr;t=st; return *this;}
template<>
HOST DEVICE Mrt3<float>& Mrt3<float>::set(
    Mat33<float> sr,Vec3<float> st){
    r=sr;t=st; return *this;}
template<>
HOST DEVICE Mrt3<double>& Mrt3<double>::set(
    Mat33<double> sr,Vec3<double> st){
    r=sr;t=st; return *this;}
template<>
HOST DEVICE Mrt3<float>& Mrt3<float>::set(
    Mat33<double> sr,Vec3<double> st){
    for(int ci=0;ci<3;ci++){
        for(int ri=0;ri<3;ri++){
            r.m[ci][ri]=(float)sr.m[ci][ri];}}
    for(int ri=0;ri<3;ri++){
        t.v[ri]=(float)st.v[ri];}
    return *this;}
template<>
HOST DEVICE Mrt3<double>& Mrt3<double>::set(
    Mat33<float> sr,Vec3<float> st){
    for(int ci=0;ci<3;ci++){
        for(int ri=0;ri<3;ri++){
            r.m[ci][ri]=(double)sr.m[ci][ri];}}
    for(int ri=0;ri<3;ri++){
        t.v[ri]=(double)st.v[ri];}
    return *this;}
/* 反転 */
template<class real>
HOST DEVICE Mrt3<real> Mrt3<real>::trans(void){
    /* y=r.x+t -> x=(r~).(y-t)= (r~).y+(-r~.t)*/
    Mat33<real> ir=r.trans();
    Vec3<real> it=(ir*t)*(-1);
    Mrt3<real> tmp; tmp.r=ir;tmp.t=it;
    return tmp;}
template<class real>
HOST DEVICE Mrt3<real> Mrt3<real>::operator~(void){
    return (*this).trans();}
/* 乗算 */
template<class real>
HOST DEVICE Mrt3<real> Mrt3<real>::operator*(Mrt3<real> mv){
    Mrt3<real> tmp;tmp.r=r*mv.r;tmp.t=r*mv.t+t;
    return tmp;}
template<class real>
HOST DEVICE Mrt3<real>& Mrt3<real>::operator*=(Mrt3<real> mv){
    (*this)=(*this)*mv;
    return *this;}
template<class real>
HOST DEVICE Mrt3<real> Mrt3<real>::operator*(Mat33<real> m){
    Mrt3<real> tmp;tmp.r=r*m;tmp.t=t;
    return tmp;}
template<class real>
HOST DEVICE Mrt3<real>& Mrt3<real>::operator*=(Mat33<real> m){
    (*this)=(*this)*m;
    return *this;}
template<class real>
HOST DEVICE Mrt3<real> Mrt3<real>::operator*(Vec3<real> v){
    Mrt3<real> tmp;tmp.r=r;tmp.t=r*v+t;
    return tmp;}
template<class real>
HOST DEVICE Mrt3<real>& Mrt3<real>::operator*=(Vec3<real> v){
    (*this)=(*this)*v;
    return *this;}
/* 変換 */
template<class real>
HOST DEVICE Vec3<real> Mrt3<real>::mult(Vec3<real> v){
    return (r*v+t);}
/* 表示 */
template<class real>
HOST void Mrt3<real>::print(const char* name){
    if(name!=NULL){printf("---- %s:\n",name);}
    r.print(NULL);
    t.print(NULL);}

/*--------------------------------------------------------------
    型定義
--------------------------------------------------------------*/
typedef class Vec3<double> vec3d;
typedef class Mat33<double> mat33d;
typedef class Mat44<double> mat44d;
typedef class Mrt3<double> mrt3d;
typedef class Vec3<float> vec3f;
typedef class Mat33<float> mat33f;
typedef class Mat44<float> mat44f;
typedef class Mrt3<float> mrt3f;

/*--------------------------------------------------------------
--------------------------------------------------------------*/
#endif /*_VEC3MAT33C2_CPP_*/
