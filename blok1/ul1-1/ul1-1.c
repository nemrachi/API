#include "ul1-1.h"

void binary(int num) {
    // if num can be shifted to the right (divided by 2), shift it
    if (num >> 1) {
        binary(num >> 1);
    }
    // last bit of num AND 1 -> print last bit of num
    printf((num & 1) ? "1" : "0");
}

void printInBinary(int num) {
    printf("%d decimal -> ", num);
    binary(num);
    printf(" binary\n");
}

void printInDecimal(int num) {
    printf("%d decimal -> %d decimal\n", num, num);
}

void printInHex(int num) {
    printf("%d decimal -> %x hexadecimal\n", num, num);
}

int main(void) {
    int num = 33777;
    char ch = 'X';

    printInBinary(num);
    printf("%c char -> ", ch);
    printInBinary(ch);
    printf("\n");

    printInDecimal(num);
    printf("%c char -> ", ch);
    printInDecimal(ch);
    printf("\n");

    printInHex(num);
    printf("%c char -> ", ch);
    printInHex(ch);

    return 0;
}