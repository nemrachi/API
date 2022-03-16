#include "blok2_richnakova.h"

static int a, result;

void ul2_1_2() {
    scanf("%d", &a);

    asm("mov a,%eax \n"
        "shl $1,%eax \n"
        "mov %eax,result \n"
    );
    
    printf("%d * 2 = %d", a, result);
}

int main() {
    ul2_1_2();
    return(0);
}