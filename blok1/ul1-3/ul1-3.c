#include "ul1-3.h"

void getFibonacciSeqTill(int k, int a, int b) {
   if (b > k) {
       return;
   }

   printf("%d ", b);   

   getFibonacciSeqTill(k, b, a+b);
}

int main(void) {
    int k;
       
    printf("Enter number: ");
    scanf("%d", &k);
    if (k < 2) {
        printf("--- Error --- Number must be >= 2");
    } else {
        getFibonacciSeqTill(k, 0, 1);
    }

    return 0;
}