for  n  in  1048576  32768  16384  4096  512  256  128  64  1  ;  do
	cc  -Wall  -DBUFFSIZE=$n  simple-cat.c;
	time  ./a.out  <boost.exe  >temp.exe;
done

