#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *getSubstring(char* str, int start, int end) {
    if (start > end || end > sizeof(str)) {
        printf("Error: substring");
        exit(1); 
    }

    char *substr = malloc((end-start+1)*sizeof(char));
    int i = 0;
    
    while((i+start) != end) {
        substr[i] = str[i+start];
        i++;
    }
    substr[i+start] = '\0';

    return substr;
}

int getIndexOfNewLine(char *str) {
    int i = 0, len = strlen(str);
    while (str[i] != '\n' && i < len) {
        i++;
    }
    return i;
}

int main() {
    //printf("%s", getSubstring("Hello", 0, 2));
    printf("%d", getIndexOfNewLine("Hello"));
    return 0;
}