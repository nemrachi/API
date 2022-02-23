#include "blok1_richnakova.h"

void printRomanNumber(int num) {
    while (num != 0) {
        if (num >= 1000) {
            printf("M");
            num -= 1000;
        } else if (num >= 900) {
            printf("CM");
            num -= 900;
        } else if (num >= 500) {
            printf("D");
            num -= 500;
        } else if (num >= 400) {
            printf("CD");
            num -= 400;
        } else if (num >= 100) {
            printf("C");
            num -= 100;
        } else if (num >= 90) {
            printf("XC");
            num -= 90;
        } else if (num >= 50) {
            printf("L");
            num -= 50;
        } else if (num >= 40) {
            printf("XL");
            num -= 40;
        } else if (num >= 10) {
            printf("X");
            num -= 10;
        } else if (num >= 9) {
            printf("IX");
            num -= 9;
        } else if (num >= 5) {
            printf("V");
            num -= 5;
        } else if (num >= 4) {
            printf("IV");
            num -= 4;
        } else if (num >= 1) {
            printf("I");
            num -= 1;
        }        
    }
}

int main(void) {
    int num;

    printf("Enter 0 to end the program\n");
    while(1) {        
        printf("Enter a number (max 3999): ");
        scanf("%d", &num);

        if (!num) { 
            break; 
        } else if (num < 0 || num > 3999) {
            printf("--- Error --- Input must be between <0, 3999>\n");
            continue;
        }
        
        printf("\t\t\t > ");
        printRomanNumber(num);
        printf("\n");
    }    

    return 0;
}