《UNIX程序设计》期末考试复习提纲

题型：

单选题 15分    填空题 15分    判断题 10分    简答题 36分    编程题 24分

复习要点：

（1）什么是操作系统？什么是UNIX操作系统？什么是Linux操作系统？它们之间的关系是怎样的？

（2）UNIX操作系统有哪些典型的分支？

（3）什么是Shell？内置变量$$、$*、$@、$?、$#的具体含义是什么？

（4）常用的Shell命令，如ls、cd、dd、pwd、ps、cp、chown、chmod、mkdir、size等的具体用法。

（5）什么是管道？哪些文件参与了管道？在命令行下和通过编程如何实现管道？

（6）常规文件和目录文件的差别是什么？常规文件、目录文件、软链接文件的长度如何计算？

（7）怎样移动文件？在命令行方式下和通过编程如何实现？

（8）怎样利用gcc编译源文件？gcc的-o、-e、-static、-Wall等选项的具体含义是什么？怎样使用？

（9）什么是动态链接库？什么是静态链接库？怎样用静态链接的方式编译C程序？

（10）什么是文件系统？UNIX的文件系统有什么特点？有哪些具体的文件类型？

（11）什么是文件的访问权限？使用chmod命令怎样改变访问权限？chmod 0777是什么意思？

（12）什么是文件操作是的偏移量（offset）？它的数据类型是什么？操作系统中文件的最大长度取决于什么？

（13）什么是管道？什么是文件重定向？dup()、dup2()函数怎样使用？

（14）什么是文件的静态属性和动态属性（文件描述符属性）？在文件描述符属性中，哪些是由进程维护的？哪些是由内核维护的？

（15）什么是会话（Session）、进程组？它们之间有什么关系？

（16）父进程和子进程之间是什么关系？怎样在父子进程之间共享文件描述符？

（17）在一个进程中，文件描述符的增长规律是怎样的？例如，如果已经有0、1、2、6这样几个文件描述符，那么用open()返回的下一个文件描述符是什么？

（18）什么是process id？父进程和子进程的pid之间有什么关系？（通常子进程的pid要大于父进程的pid）

（19）什么是C语言程序的入口函数？在C Startup Routine（start.S）中接受的main函数原型是什么？

（20）什么是系统调用？什么是C语言库函数？它们之间有什么区别和联系？

（21）什么是inode？里面存放什么信息？文件的文件名存放在哪里？

（22）C程序的内存布局是怎样的？从低地址到高地址依次存放哪些段？

（23）怎样利用fork()、exec()、waitpid()来创建和控制进程？

（24）什么是孤儿进程、什么是僵尸进程？它们有什么特点？怎样避免产生过多僵尸进程？

（25）什么是前台进程？什么是后台进程？一个会话有几个前台进程组和几个后台进程组？

（26）C程序如何退出并返回操作系统？exit()函数和_exit()/_Exit()函数的差别在哪里？

（27）exec函数族包含哪些具体的函数？其中execve是系统调用，其它都是普通函数。

（28）什么是信号？SIGINT、SIGSTOP、SIGHUP、SIGALARM、SIGQUIT等信号是如何产生的？缺省的处理动作是什么？

（29）什么是硬链接和软链接（符号链接）？读取软连接的函数是什么？（readlink）

（30）函数link()和unlink()的作用是什么？什么时候文件占用的磁盘空间才会真正被释放掉？（两个条件）

（31）什么是可重入函数？怎样判断一个函数是不是可重入函数？

（32）什么是带缓冲的输出和不带缓冲的输出？当父进程的输出缓冲区还未清空时，调用fork创建子进程，会出现什么情况？

（33）编程的题目在这次考试中占很大的比重。一共有两道编程的题目。第一道跟文件操作相关，要求详细掌握的用法包括：能用open,read,write,close等函数来操纵文件；能利用lseek来形成文件中的“空洞”；能动态申请和释放空间等。

（34）第二道编程的题目跟进程和信号相关。要求详细掌握的用法包括：能用pause函数阻塞进程；能用signal设置信号处理函数；能向特定进程发送信号；能用rename移动文件；能遍历目录；能用S_ISREG宏辨别文件类型等等。切记对最后两道编程的大题要提前做好准备。