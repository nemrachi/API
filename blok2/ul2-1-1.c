#include "blok2_richnakova.h"

static int a, b, result;

void ul2_1_1() {
    scanf("%d %d", &a, &b);

    asm("mov a,%eax \n"
        "mov b,%ebx \n"
        "add %eax,%ebx \n" 
        "mov %ebx,result \n"
    );   

    printf("%d + %d = %d", a, b, result);
}

int main() {
    ul2_1_1();
    return(0);
}