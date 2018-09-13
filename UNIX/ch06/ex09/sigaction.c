#include <stdio.h>  
#include <signal.h>
#include <pthread.h>
#include <unistd.h>  
  
  
void WrkProcess(int nsig)  
{  
        printf("WrkProcess .I get signal.%d threadid:%d\n",nsig,(int)pthread_self());  
  
  
        int i=0;  
        while(i<5){  
                printf("%d\n",i);  
                sleep(1);  
                i++;  
        }  
}  
  
int main()  
{  
        struct sigaction act,oldact;  
        act.sa_handler  = WrkProcess;  
        sigaddset(&act.sa_mask,SIGQUIT);  
//      sigaddset(&act.sa_mask,SIGTERM)  
        act.sa_flags = SA_NODEFER | SA_RESETHAND;    
//        act.sa_flags = 0;  
  
        sigaction(SIGINT,&act,&oldact);  
  
        printf("main threadid:%d\n",(int)pthread_self());  
  
        while(1)sleep(5);  
  
        return 0;  
} 
