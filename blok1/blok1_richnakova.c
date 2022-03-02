#include "blok1_richnakova.h"

///////////////////////////ul1-1///////////////////////////
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

void ul1_1() {
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
}

///////////////////////////ul1-2///////////////////////////
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

void ul1_2() {
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
}

///////////////////////////ul1-3///////////////////////////
void fibonacci(int k, int a, int b) {
   if (b > k) {
       return;
   }

   printf("%d ", b);   

   fibonacci(k, b, a+b);
}

void getFibonacciSeqTill(int k) {
    // print fibonacci seq numbers that are less or equal with k
    fibonacci(k, 0, 1);
}

void ul1_3(){
    int k;

    printf("Enter 0 to end the program\n");
    while (1) {
        printf("Enter a number: ");
        scanf("%d", &k);
        
        if (k == 0) {
            break;
        } else if (k < 2) {
            printf("--- Error --- The number must be >= 2 to get Fibonacci sequence\n");
        } else {
            getFibonacciSeqTill(k);
            printf("\n");
        }
    }
}

///////////////////////////ul1-5///////////////////////////
float absolute(float num) {
    return (num >= 0) ? num : -num;
}

float modulo(float a, float b) {
    a = absolute(a);
    return ((a/b) - (int)(a/b))*b;
}

float power(float base, float power) {
    if (power == 0) {
        return 1;
    }

    float result = 1;
    for (int i = 0; i < power; i++) {
        result *= base;
    }
    
    return result;
}

float factorial(float number) {
    float result = 1;
    while(number > 1) {
        result *= number;
        number--;        
    }
    return result;
}

float cosine(float degrees, float epsilon) {
    float cos = 0;
    float tmp = cos;
    int n = 0;
    float radians = modulo(degrees, 360.0) * (PI / 180.0);

    while(1) {
        tmp = cos;
        cos += (power(-1, n)*power(radians, 2*n))/factorial(2*n);
        if (absolute(cos - tmp) < epsilon) {
            return cos;
        }
        n++;
    }
}

void ul1_5(){
    float deg;
    scanf("%f", &deg);
    printf("%f\n", cosine(deg, EPSILON));
}

///////////////////////////main///////////////////////////
int main() {
    //ul1_1();
    //ul1_2();
    //ul1_3();
    ul1_5();

    return 0;
}