#ifndef _STOPWATCH_CPP_
#define _STOPWATCH_CPP_

/*--------------------------------------------------------------
    Windows
--------------------------------------------------------------*/
#if defined(_WIN32)||defined(_WIN64)
#include<time.h>
/* 計時 */
class StopWatch{
public:
    LARGE_INTEGER start_time;
    LARGE_INTEGER stop_time;
    StopWatch(){
        QueryPerformanceCounter(&start_time);
        QueryPerformanceCounter(&stop_time);}
    ~StopWatch(){}
    inline void start(void){QueryPerformanceCounter(&start_time);}
    inline void stop(void){QueryPerformanceCounter(&stop_time);}
    double time_s(void){
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        double interval=(stop_time.QuadPart-start_time.QuadPart)/
               (double)freq.QuadPart;
        return(interval);}
    double time_ms(void){
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        double interval=(stop_time.QuadPart-start_time.QuadPart)/
               (double)freq.QuadPart;
        return(interval*1000.0);}
    int time_us(void){
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        double interval=(stop_time.QuadPart-start_time.QuadPart)/
               (double)freq.QuadPart;
        return((int)(interval*1000000));
    }
};
/* sleep */
void wait_ms(int t){Sleep(t);}

/*--------------------------------------------------------------
    Linux
--------------------------------------------------------------*/
#else
#include<time.h>
#include<unistd.h>
/* 計時 */
class StopWatch{
public:
    struct timespec start_time;
    struct timespec stop_time;
    StopWatch(){
        clock_gettime(CLOCK_REALTIME,&start_time);
        clock_gettime(CLOCK_REALTIME,&stop_time);}
    ~StopWatch(){}
    void start(void){clock_gettime(CLOCK_REALTIME,&start_time);}
    void stop(void){clock_gettime(CLOCK_REALTIME,&stop_time);}
    int time_us(void){
        return ((stop_time.tv_sec-start_time.tv_sec)*1000000+
                (stop_time.tv_nsec-start_time.tv_nsec)/1000);}
    double time_ms(void){
        return ((stop_time.tv_sec-start_time.tv_sec)*1000.0+
                (stop_time.tv_nsec-start_time.tv_nsec)/1000000.0);}
};
/* sleep */
void wait_ms(int t){usleep(t*1000);}
#endif

/*--------------------------------------------------------------
    テスト用メイン
--------------------------------------------------------------*/
#if 0
#include <stdio.h>
#include <time.h>
int main(void){
StopWatch sw;
    /* コピー */
    sw.start();
    sw.stop();
    printf("wait 1s x0 %d\n",sw.time_ms());
    wait_ms(1000);
    sw.stop();
    printf("wait 1s x1 %d\n",sw.time_ms());
    wait_ms(1000);
    sw.stop();
    printf("wait 1s x2 %d\n",sw.time_ms());
    wait_ms(1000);
    sw.stop();
    printf("wait 1s x3 %d\n",sw.time_ms());
}
#endif

/*--------------------------------------------------------------
--------------------------------------------------------------*/
#endif/*_STOPWATCH_CPP_*/
