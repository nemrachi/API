#include "blok2_richnakova.h"

static int a, result;

void ul2_1_3() {
    scanf("%d", &a);
    if (a < 0 || a > 15) {
        return;
    }

    asm("mov a,%eax \n"
        "cmp $10,%eax \n"
        "je L1 \n"
        "jg L1 \n"
        "jmp L2 \n"
        "L1: \n"
        "add $55,%eax \n"
        "jmp L3 \n"
        "L2: \n"
        "add $48,%eax \n"
        "L3: \n"
        "mov %eax,result \n"
    );

    printf("dec %d = char %c", result, (char)result);
}

int main() {
    ul2_1_3();
    return(0);
}