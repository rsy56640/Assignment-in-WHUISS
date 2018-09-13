#include <stdio.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <stdlib.h>  
#include <string.h>
#include <unistd.h>  
#include <sys/mman.h>
#include <sys/file.h>
#include <sys/wait.h>

#ifndef BUFFSIZE
#define BUFFSIZE 32 
#endif //BUFFSIZE
#define EXIT_SUCCESS 0
#define EXIT_FAIL 1

int main()
{
	int fd;
	char in_buf[BUFFSIZE];
	
	//first arg should be "tcp"
	scanf("%s", in_buf);
	if(strcmp(in_buf, "tcpm") != 0) 
	{
		printf("command error\n");  
        return EXIT_FAIL;
	}
	
	//second arg is source file
	scanf("%s", in_buf);
	if((fd = open(in_buf, O_RDONLY)) < 0) 
	{
		perror("open source file error");
        return EXIT_FAIL;
	}
	
	char src_name[BUFFSIZE];	//[[maybe_unused]]: lest the target dir doesnot give a file name.
	strcpy(src_name, in_buf);
	
	off_t filesize = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	
	//map the source to the memory
	char* map_address1 = (char*) mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd, 0);
	if (map_address1 == MAP_FAILED)
	{
		perror("mmap source file error");
		return EXIT_FAIL;
	}
    	
    close(fd);
    
    
	//third arg is target file/directory
	scanf("%s", in_buf);
	const int length = strlen(in_buf);
	
	if(in_buf[length-1] == '/')	//target is a dir
		strcpy(in_buf, strcat(in_buf, src_name));
	if((fd = open(in_buf, O_RDWR|O_CREAT, 00777)) < 0) 
	{
		perror("open target file error");
        return EXIT_FAIL;
	}
	
	ftruncate(fd, filesize);

	
	//map the source to the memory
	char* map_address2 = (char*) mmap(NULL, filesize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (map_address2 == MAP_FAILED)
	{
		perror("mmap target file error");
		return EXIT_FAIL;
	}
		
	close(fd);
		
	memcpy(map_address2, map_address1, filesize);
	
	munmap(map_address1, filesize);
	munmap(map_address2, filesize);
	
	return EXIT_SUCCESS;
}

