#ifndef _PINMAP_CPP_
#define _PINMAP_CPP_
/*--------------------------------------------------------------
    ピン位置
--------------------------------------------------------------*/
class PinMap{
public:
    std::vector<vec3f> position;
    std::vector<vec3f> orientation;
    std::vector<float> area;
    std::vector<int> check;
    /* 形状保存 */
    void load(const char *name){
        printf("pinmap loading.. \n");
        /* ファイルのオープン */
        FILE *fp;
        if((fp=fopen(name,"rt"))==NULL){
            printf("open err\n");exit(0);}
        /* 点数 */
        int num;
        for(;;){
            /* 1行読む */
            char buf[256];
            if(fgets(buf,sizeof(buf),fp)==0){break;}
            /* コメント？ */
            if(buf[0]=='#'){continue;}
            /* 点数？ */
            if(sscanf(buf,"%d",&num)!=1){
                printf("read num error\n");exit(0);}
            else{break;}}
        printf("pinmap num %d\n",num);
        /* 領域取得 */
        position.resize(num);
        orientation.resize(num);
        area.resize(num);
        check.resize(num,0);
        /* ピンデータ */
        for(;;){
            /* 1行読む */
            char buf[256];
            if(fgets(buf,sizeof(buf),fp)==0){break;}
            /* コメント？ */
            if(buf[0]=='#'){continue;}
            /* 座標 */
            int i;vec3f p,o;float a;
            if(sscanf(buf,"%d,%f,%f,%f,%f,%f,%f,%f",
                &i,&(p[0]),&(p[1]),&(p[2]),&(o[0]),&(o[1]),&(o[2]),&a)!=8){
                printf("read data error\n");exit(0);}
            else{
                if((i<0)||(i>=num)){printf("data i error\n");exit(0);}
                position[i]=p; orientation[i]=o; area[i]=a;check[i]=1;}}
        /* 点数チェック */
        for(int i=0;i<num;i++){
            if(!check[i]){printf("pin %d not defined\n",i);}}
        /* ファイルを閉じる */
        fclose(fp);}
#ifdef __GL_H__
    /* ピン位置の描画　*/
    void draw_pos(int lon=24,int lat=12){
        const float pi10=3.14159265358979f;
        static int sphere=(-1);
        if(sphere<0){sphere=sphere_list(1,lon,lat);}
        int num=(int)position.size();
        for(int i=0;i<num;i++){
            vec3f p=position[i];
            float r=sqrtf(area[i]/pi10);
            glPushMatrix();
            glTranslatef(p[0],p[1],p[2]);
            glScalef(r,r,r);
            glCallList(sphere);
            glPopMatrix();}}
#endif
};
/*--------------------------------------------------------------
--------------------------------------------------------------*/
#endif/*_PINMAP_CPP_*/
