#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int getIndexOfNewLine(char *str) {
    int i = 0, len = strlen(str);
    while (str[i] != '\n' && i < len) {
        i++;
    }
    return i;
}

int main() {
    //printf("%s", getSubstring("Hello", 0, 2));
    char c = 'b';
    c -= 32;
    printf("%c", c);
    return 0;
}