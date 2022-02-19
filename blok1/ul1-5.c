#include "blok1_richnakova.h"

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

int main(void) {
    printf("%f", cosine(8649, EPSILON));
    return 0;
}