#include "blok1_richnakova.h"

void fibonacci(int k, int a, int b) {
   if (b > k) {
       return;
   }

   printf("%d ", b);   

   fibonacci(k, b, a+b);
}

void getFibonacciSeqTill(int k) {
    // fibonacci <= k
    fibonacci(k, 0, 1);
}

int main(void) {
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

    return 0;
}