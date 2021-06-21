#ifndef _POLKADOTS_CPP_
#define _POLKADOTS_CPP_
/*--------------------------------------------------------------
    水玉クラス
--------------------------------------------------------------*/
class PolkaDots{
public:
    /* 領域サイズ */
    float area_w,area_h;
    /* 生成された水玉 */
    std::vector<vec3f> list; //(x,y,r)
private:
    /*--------------------------------*/
    /* 乱数(一様、0-1) */
    float rand1(void){
        return (float)rand()/(float)RAND_MAX;}
    /* 乱数(正規) */
    float randn(float s,int n=20){
        float sum=0;
        for(int i=0;i<n;i++){sum+=(float)rand()/(float)RAND_MAX;}
        return s*(sum-(n*0.5f))/sqrtf(n/12.0f);}
    /* 衝突判定（サイクリック） */
    int is_collision(vec3f p,vec3f q,float gap){
        vec3f r=p-q;
        /* サイクリック */
        r[0]=r[0]-area_w*floorf(r[0]/area_w);
        if(r[0]>area_w*0.5f){r[0]-=area_w;}
        r[1]=r[1]-area_h*floorf(r[1]/area_h);
        if(r[1]>area_h*0.5f){r[1]-=area_h;}
        /* 2次元 */
        r[2]=0.0f;
        /* 判定 */
        float d=r.length();
        if(d<(p[2]+q[2]+gap)){return 1;}else{return 0;}}
    /*--------------------------------*/
    /* 水玉追加 */
    int pack_1(float rad_a,float rad_s,float gap,int ntry){
        int added=0;
        for(int i=0;i<ntry;i++){
            /* 新しい水玉 */
            vec3f p;
            p[0]=area_w*rand1();//x
            p[1]=area_h*rand1();//y
            if(rad_s==0.0f){p[2]=rad_a;}else{p[2]=rad_a+randn(rad_s);}
            /* 半径チェック */
            if(p[2]<0){continue;}
            /* 衝突チェック */
            int flag=0;//衝突
            for(int j=0;j<(int)list.size();j++){
                /* 既存の円 */
                vec3f q=list[j];
                /* 衝突判定（サイクリック） */
                if(is_collision(p,q,gap)){flag=1;break;}
            }
            /* 衝突なしなら採用 */
            if(flag==0){list.push_back(p);added++;}
        }
        return added;}
public:
    /*--------------------------------*/
    void set_area(float w,float h){area_w=w;area_h=h;}
    void pack(float rad_a,float rad_s,float gap,int ntry){
        for(int i=0;;i++){
            /* 生成 */
            int n=pack_1(rad_a,rad_s,gap,ntry);
            printf("%d: %d (%d)\n",i,n,(int)list.size());
            if(n==0){break;}}}
    int save(const char* name){
        /* ファイルのオープン */
        FILE *fp;
        if((fp=fopen(name,"wt"))==NULL){
            printf("open err\n");return 1;}
        /* 領域 */
        fprintf(fp,"%f,%f\n",area_w,area_h);
        /* 円の数 */
        fprintf(fp,"%d\n",(int)list.size());
        /* x,y,r */
        for(int i=0;i<(int)list.size();i++){
            vec3f p=list[i];
            fprintf(fp,"%f,%f,%f\n",p[0],p[1],p[2]);}
        /* ファイルを閉じる */
        fclose(fp);return 0;}
    int load(const char* name,int append=0){
        /* リストのクリア */
        if(!append){list.clear();}
        /* ファイルのオープン */
        FILE *fp;char buf[256];
        if((fp=fopen(name,"rt"))==NULL){
            printf("open err\n");return 1;}
        /* 領域 */
        float w,h;
        if(fgets(buf,sizeof(buf),fp)==0){
             printf("area error\n");fclose(fp);return 2;}
        if(sscanf(buf,"%f,%f",&w,&h)!=2){
            printf("area format error\n");fclose(fp);return 3;}
        area_w=w;area_h=h;
        /* 円の数 */
        int n;
        if(fgets(buf,sizeof(buf),fp)==0){
             printf("num error\n");fclose(fp);return 4;}
        if(sscanf(buf,"%d",&n)!=1){
            printf("num format error\n");fclose(fp);return 5;}
        /* x,y,r */
        for(int i=0;i<n;i++){
            vec3f p;
            if(fgets(buf,sizeof(buf),fp)==0){
                printf("dot error\n");fclose(fp);return 6;}
            if(sscanf(buf,"%f,%f,%f",&p[0],&p[1],&p[2])!=3){
                printf("dot format error\n");fclose(fp);return 7;}
            list.push_back(p);}
        /* ファイルを閉じる */
        fclose(fp);return 0;}
    /*--------------------------------*/
#ifdef _GLSHAPE_CPP_
    void draw_area(void){
        /* box */
        glBegin(GL_LINE_STRIP);
        glVertex3f(0,     0,     0);
        glVertex3f(area_w,0,     0);
        glVertex3f(area_w,area_h,0);
        glVertex3f(0,     area_h,0);
        glVertex3f(0,     0,     0);
        glEnd();}
    void draw(void){
        for(int i=0;i<(int)list.size();i++){
            vec3f p=list[i];
            glPushMatrix();
            glTranslatef(p[0],p[1],0);
            circle_draw(p[2],36);
            glPopMatrix();}}
#endif
};

/*--------------------------------------------------------------
    水玉干渉クラス
--------------------------------------------------------------*/
template<int DIV_W,int DIV_H>
class PolkaCol:public PolkaDots{
public:
//変数
    float grid_w,grid_h;
    int serach_w,serach_h;
    std::vector<int> grid[DIV_H][DIV_W];
//メソッド
    void make_grid(void){
        /* クリア */
        for(int j=0;j<DIV_H;j++){
            for(int i=0;i<DIV_W;i++){
                grid[j][i].clear();}}
        /* グリッドサイズ */
        grid_w=area_w/DIV_W;
        grid_h=area_h/DIV_H;
        /* 円の登録 */
        float max_rad=0.0f;
        for(int i=0;i<(int)list.size();i++){
            vec3f q=list[i];
            /* グリッド */
            int x=(int)(q[0]/grid_w);
            int y=(int)(q[1]/grid_h);
            if(x<0){x=0;}else if(x>=DIV_W){x=DIV_W-1;}
            if(y<0){y=0;}else if(y>=DIV_H){y=DIV_H-1;}
            grid[y][x].push_back(i);
            /* 半径 */
            if(max_rad<q[2]){max_rad=q[2];}}
        serach_w=(int)(max_rad/grid_w)+1;
        serach_h=(int)(max_rad/grid_h)+1;}
    int is_collision(float px,float py){
        /* grid内の座標 */
        float pxg=px-area_w*floorf(px/area_w); 
        float pyg=py-area_h*floorf(py/area_h);
        int pig=(int)(pxg/grid_w); //pのgrid値
        int pjg=(int)(pyg/grid_h); //pのgrid値
        /* グリッド内の探索 */
        for(int j=(pjg-serach_h);j<=(pjg+serach_h);j++){
            int jg=(j+DIV_H)%DIV_H; //grid内
            for(int i=(pig-serach_w);i<=(pig+serach_w);i++){
                int ig=(i+DIV_W)%DIV_W; //grid内
                for(int k=0;k<(int)grid[jg][ig].size();k++){
                    int n=grid[jg][ig][k]; //list id
                    /* 円の座標 */
                    vec3f q=list[n];
                    /* 距離の計算 */
                    float rx=fabsf(pxg-q[0]);
                    float ry=fabsf(pyg-q[1]);
                    /* サイクリック */
                    if(rx>area_w*0.5f){rx=area_w-rx;}
                    if(ry>area_h*0.5f){ry=area_h-ry;}
                    /* 干渉 */
                    if(((rx*rx)+(ry*ry))<(q[2]*q[2])){return 1;}}}}
        /* 干渉なし */
        return 0;}
};

/*--------------------------------------------------------------
--------------------------------------------------------------*/
#endif /*_POLKADOTS_CPP_*/
