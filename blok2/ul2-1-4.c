#include "blok2_richnakova.h"

static char arr[3];

void ul2_1_4() {
    asm("mov $0,%eax \n"
        "cpuid \n"
        "mov %ebx,arr+0 \n"
        "mov %edx,arr+4 \n"
        "mov %ecx,arr+8 \n"
    );
        
    printf("Processor: %s", arr);
}

int main() {
    ul2_1_4();
    return(0);
}