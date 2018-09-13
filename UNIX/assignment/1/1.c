#include <stdio.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <stdlib.h>  
#include <string.h>
#include <unistd.h>  

#ifndef BUFFSIZE
#define BUFFSIZE 128
#endif //BUFFSIZE
#define EXIT_SUCCESS 0
#define EXIT_FAIL 1

int main()
{
	int fd;
	int n;
	char in_buf[BUFFSIZE];
	char buf[BUFFSIZE];
	
	//first arg should be "tcp"
	scanf("%s", in_buf);
	if(strcmp(in_buf, "tcp") != 0) 
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
	
	//read the source to the buffer
	n = read(fd, buf, BUFFSIZE);


	/********************************************\
	* We always suppose the last word is a file. *
	* SO if you want to copy a file to a dir,    *
	* You should input like this:                *
	* "dir1/dir2/dir3/"                          *
	\********************************************/
	//third arg is target file/directory
	scanf("%s", in_buf);
	
	const int length = strlen(in_buf);
	
	
	/*
	char directory_address[BUFFSIZE];
	int cur_length = 0;
	while(length > cur_length)
	{
		
		sscanf(in_buf + cur_length, "%[a-zA-Z0-9.]", directory_address);
		
		cur_length += strlen(directory_address) + 1;	//'1' represents the successed '/'.
		
		if(length >= cur_length)	//directory_address stores a dir
			system(strcat("cd ", directory_address));	//why does this not work????????
		
	}
	
	if(length < cur_length)		//directory_address stores the file name
	{
		 //O_TRUNC here lest there exists file with the same name as src
		if((fd = open(directory_address, O_RDWR|O_CREAT|O_TRUNC)) < 0)
		{
			perror("open target file error");
        	return EXIT_FAIL;
		}
	}
	else	//create the same name file with src
	{
		if((fd = open(src_name, O_RDWR|O_CREAT|O_TRUNC)) < 0) 
		{
			perror("open target file error");
        	return EXIT_FAIL;
		}
	}
	*/
	if(in_buf[length-1] == '/')	//target is a dir
		strcpy(in_buf, strcat(in_buf, src_name));
	if((fd = open(in_buf, O_RDWR|O_CREAT|O_TRUNC, 00777)) < 0) 
	{
		perror("open target file error");
        return EXIT_FAIL;
	}
	
	if((n = write(fd, buf, n)) < 0)
	{
		perror("write error");
        return EXIT_FAIL;
	}

	return EXIT_SUCCESS;
}

