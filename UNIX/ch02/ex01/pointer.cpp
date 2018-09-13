#include <iostream>

void testptr(){
    int i=5;
    int j=6;
    int *p;
    p=&i;
    std::cout << p <<std::endl;
    p=&j;
    std::cout << p <<std::endl;
    p++;
    std::cout << p << std::endl;
}
