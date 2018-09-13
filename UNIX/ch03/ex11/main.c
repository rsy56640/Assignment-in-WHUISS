    #include <sys/types.h>  
    #include <sys/stat.h>  
    #include <stdio.h>  
      
    int main(int argc, char *argv[])  
    {  
        int            i;  
        struct stat    buf;  
      
        for (i = 1; i < argc; i++) {  
            printf("%s: ", argv[i]);  
            if (stat(argv[i], &buf) == -1) {  
                printf("stat error for %s\n", argv[i]);  
                continue;  
            }  
      
            printf("dev = %d/%d", major(buf.st_dev), minor(buf.st_dev));  
      
            if (S_ISCHR(buf.st_mode) || S_ISBLK(buf.st_mode)) {  
                printf(" (%s) rdev = %d/%d",  
                        (S_ISCHR(buf.st_mode)) ? "character" : "block",  
                        major(buf.st_rdev), minor(buf.st_rdev));  
            }  
      
            printf("\n");  
      
        }  
      
        return 0;  
      
    }  
    
    
//    echo "123" > /tmp/myfile
//    ./a.out /tmp/myfile /dev/sda /dev/sda1 /dev/tty0
//   /tmp/myfile: dev = 8/18
//   /dev/sda: dev = 0/5 (block) rdev = 8/0
//   /dev/sda1: dev = 0/5 (block) rdev = 8/1
//   /dev/tty0: dev = 0/5 (character) rdev = 4/0


    
    
