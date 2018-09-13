#include <stdio.h>
struct  opaque_ds{
    int a;
    char c;
};

typedef struct opaque_ds opaque_ds;

opaque_ds * opaque_func(){
    static opaque_ds ds;
    ds.a=1;
    ds.c='0';
    return &ds;
}

void display_ods(opaque_ds * ods){
    printf("%d --%c\n",ods->a,ods->c);
}
