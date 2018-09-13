#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <string.h>

#define BUFFER_SIZE 1


void my_copy1()
{
    int fin,fout;   //文件描述符
    void *start;
    void *end;
    struct stat sb;
    if((fin = open("file.in",O_RDONLY)) < 0){
        perror("open error");
        exit(EXIT_FAILURE);
    }
    if((fout = open( "file.out",O_RDWR | O_CREAT | O_TRUNC,00600)) < 0 ){
        perror( "write error" );
        exit( EXIT_FAILURE );
    }

    fstat(fin,&sb);

    //这块必须给fout一个需求大小的偏移，因为mmap没有扩展空间的能力
    if(lseek(fout,sb.st_size-1,SEEK_SET) < 0 ){
        exit(EXIT_FAILURE);
    }
    if(write(fout, &sb,1) != 1 ){
        exit(EXIT_FAILURE);
    }

    start = mmap(NULL,sb.st_size,PROT_READ,MAP_PRIVATE,fin,0);
    if(start == MAP_FAILED)
        return;

    end = mmap(0,(size_t)sb.st_size,PROT_WRITE,MAP_SHARED,fout,0);
    if(end == MAP_FAILED){
        return ;
    }

    memcpy(end,start,(size_t)sb.st_size);

    munmap(start,sb.st_size); //关闭映射
    munmap(end,sb.st_size);

    close(fin);
    close(fout);
    return;
}

void my_copy2()
{
    int fin,fout;
    int bytes_read,bytes_write;
    char buffer[BUFFER_SIZE];
    char *ptr;
    if((fin = open("file.in",O_RDONLY)) < 0){
        perror("open error");
        exit(EXIT_FAILURE);
    }
    if((fout = open( "file.out",O_RDWR | O_CREAT | O_TRUNC,00700)) < 0 ){
        perror( "write error" );
        exit( EXIT_FAILURE );
    }

    while(bytes_read=read(fin,buffer,BUFFER_SIZE)){
        if((bytes_read==-1)&&(errno!=EINTR))
            break;
        else if(bytes_read>0){
            ptr=buffer;
            while(bytes_write=write(fout,ptr,bytes_read)){
                if((bytes_write==-1)&&(errno!=EINTR))
                    break;
                else if(bytes_write==bytes_read)
                    break;
                else if(bytes_write>0){
                    ptr+=bytes_write;
                    bytes_read-=bytes_write;
                }
            }
            if(bytes_write==-1)
               break;
         }
    }

    close(fin);
    close(fout);
    return;
}

main()
{
    struct timeval tv;
    struct timezone tz;
    long time_start,time_end;
    gettimeofday(&tv,&tz);
    time_start = tv.tv_sec*1000000 + tv.tv_usec;
    my_copy1();
    printf("\ndone.\n\n");
    gettimeofday(&tv,&tz);
    time_end = tv.tv_sec*1000000 + tv.tv_usec;
    printf("using \"mmap()\" to copy costs %ld microseconds \n",time_end - time_start);

    gettimeofday(&tv,&tz);
    time_start = tv.tv_sec*1000000 + tv.tv_usec;
    my_copy2();
    gettimeofday(&tv,&tz);
    time_end = tv.tv_sec*1000000 + tv.tv_usec;

//这块之前有时会出现打印出的负数，后来查看gettimeofday()函数，才知道，使用的微秒位在满一秒
//的时候会进位到秒，也即是微秒位清零。所以，正确的方法是在计算time_start和time_end的时候加上
//秒这一位，不过要首先转换其成为微秒。
    printf("using \"read() and write()\" to copy costs %ld microseconds \n",time_end - time_start);
}
