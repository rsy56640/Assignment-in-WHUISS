#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void show_ids(){
    printf("real user-id:%d\n",getuid());
    printf("effective user-id:%d\n",geteuid());
}

int main(int argc,char* argv[]){
    int uid;
    show_ids();
    if(argc >1){
        uid=atoi(argv[1]);
        if(setuid(uid)<0)
            perror("setuid() failed");
        show_ids();
    }
    return 0;
}
