#include "blok1_richnakova.h"

void binary(int num) {
    // if num can be shifted to the right (divided by 2), shift it
    if (num >> 1) {
        binary(num >> 1);
    }
    // num AND 1 -> print the last bit of num (or the reminder after division of 2)
    printf((num & 1) ? "1" : "0");
}

void printInBinary(int num) {
    printf("\t-> ");
    binary(num);
    printf(" binary\n");
}

void printInDecimal(int num) {
    printf("\t-> %d decimal\n", num);
}

void printInHex(int num) {
    printf("\t-> %x hexadecimal\n", num);
}

int main(void) {
    int num = 33777;
    char ch = 'X';

    printf("%d decimal ->\n", num);
    printInBinary(num);
    printInDecimal(num);
    printInHex(num);
    printf("\n");

    printf("%c char ->\n", ch);
    printInBinary(ch);
    printInDecimal(ch);
    printInHex(ch);

    return 0;
}