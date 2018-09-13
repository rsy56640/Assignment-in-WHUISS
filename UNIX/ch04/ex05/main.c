#include <time.h>
#include <stdio.h>

int main(void)
{
    struct tm *ptr;
    time_t lt;
 
    lt=time(NULL);
    
    ptr=localtime(&lt);   
   
    printf("%s\n",asctime(ptr)); 
   
    printf("%s\n",ctime(&lt));
    return 0;
}
