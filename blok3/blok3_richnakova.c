#include "blok3_richnakova.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>

HANDLE hConsole;
FILE *LOG;

/** run program:
 * cd c:\Users\emari\Projects\API\blok3\ && gcc blok3_richnakova.c -o blok3_richnakova -lws2_32 && .\blok3_richnakova < .\in.txt
 */

WORD getConsoleAttr() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    return consoleInfo.wAttributes;
}

SOCKET initConnection() { //uvodne nastavovacky
    WSADATA wsaData; // struktura WSADATA pre pracu s Winsock
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData); // zakladna inicializacia
    if (iResult != 0) { // kontrola, ci nestala chyba
        printf("WSAStartup failed: %d\n", iResult);
        exit(1);
    }

    struct addrinfo *result = NULL, *ptr = NULL; //struktura pre pracu s adresami
    struct addrinfo hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP; // pracujeme s protokolom TCP/IP

    // Resolve the server address and port
    iResult = getaddrinfo(AP_SERVER_IP_ADDR, AP_SERVER_PORT, &hints, &result);
    if (iResult != 0) { // kontrola, ci nenastala chyba
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        exit(1);
    } else {
        printf("getaddrinfo didn't fail...\n");
    }

    // vytvorenie socketu a pripojenie sa
    
    SOCKET ConnectSocket = INVALID_SOCKET;

    // Attempt to connect to the first address returned by
    // the call to getaddrinfo
    ptr = result;

    // Create a SOCKET for connecting to server => pokus o vytvorenie socketu
    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,ptr->ai_protocol);

    if (ConnectSocket == INVALID_SOCKET) { // kontrola, ci nenastala chyba
        printf("Error at socket(): %d\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        exit(1);
    } else {
        printf("Error at socket DIDN'T occur...\n");
    }

    // Connect to server. => pokus o pripojenie sa na server
    iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) { // kontrola, ci nenastala chyba
        printf("Not connected to server…\n");
    } else {
        printf("Connected to server!\n");
    }

    if (iResult == SOCKET_ERROR) { // osetrenie chyboveho stavu
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
        WSACleanup();
        exit(1);
    }

    Sleep(250);

    return ConnectSocket;
}

void sendMessage(SOCKET ConnectSocket, char *sendbuf) {
    int iResult = send(ConnectSocket, sendbuf, strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed: %d\n", WSAGetLastError());
        disconnect(ConnectSocket);
        exit(1);
    }

    printLog(sendbuf, iResult, true); // iResult = pocet odoslanych dat, client msg (client -> true)
}

void receiveMessage(SOCKET ConnectSocket, char *recvbuf) {
    memset(recvbuf, 0, DEFAULT_BUFLEN);
    int iResult = recv(ConnectSocket, recvbuf, DEFAULT_BUFLEN, 0); // funkcia na príjimanie
    if (iResult > 0) {
        printLog(recvbuf, iResult, false); // iResult = pocet prijatych dat, server msg (no client -> false)
    } else if (iResult == 0) {
        printf("Connection closed\n"); // v tomto pripade server ukoncil komunikaciu
    } else {
        printf("recv failed with error: %d\n", WSAGetLastError()); // ina chyba
    }
}

void disconnect(SOCKET ConnectSocket) {
    closesocket(ConnectSocket);
    WSACleanup();
}

void getSendbuf(char *sendbuf) {
    memset(sendbuf, 0, DEFAULT_BUFLEN); // reset sendbuf
    fgets(sendbuf, DEFAULT_BUFLEN, stdin); // get sendbuf from stdin
    sendbuf[getIndexOfNewLine(sendbuf)] = '\0'; // replace '\n' by '\0'
}

int getIndexOfNewLine(char *str) {
    int i = 0, len = strlen(str);
    while (str[i] != '\n' && i < len) { i++; }
    return i;
}

void aisIdReminder(char *sendbuf, char *aisId) {
    int sum = 0, div;

    for (int i = 0; i < 5; i++) {
        sum += aisId[i] - '0'; // get number from letter
    }
    div = aisId[4] - '0'; // get 5th number of ais id
    div = (!div) ? 9 : div; // check if it is 0
    sendbuf[0] = (sum % div) + '0'; // get reminder
    sendbuf[1] = '\0';
}

void decipherMsg(char *sendbuf, char *recvbuf, char key) {
    int len = 132;
    char *decipherStr = malloc(DEFAULT_BUFLEN);

    for (int i = 0; i < len; i++) {
        if (isprint(recvbuf[i]^key)) { // only decode when decoded char is printable
            decipherStr[i] = recvbuf[i]^key;
        } else {
            len--;
        }
    }
    decipherStr[len] = '\0';

    strcpy(sendbuf, decipherStr);
    free(decipherStr);
}

void primeNumberLetters(char *sendbuf, char *recvbuf) {
    int count, k = 0, len = strlen(recvbuf);
    char *str = malloc(DEFAULT_BUFLEN);

    for (int i = 2; i < len; i++) {
        count = 0;
        for (int j = i-1; j >= 2; j--) {
            if (i % j == 0) { count++; } // count how many divisors number(i) has (except 1 and itself)
        }
        if (!count) { str[k++] = recvbuf[i-1]; } // if count of divisors == 0, number(i) is prime, get i.th letter
    }
    str[k] = '\0';

    strcpy(sendbuf, str);
    free(str);
}

void caesarCipher(char *sendbuf, char *str, int shift) {
    int len = strlen(str);
    char decipherCh;

    for (int i = 0; i < len; i++) { // turn lower letters to upper
        if (islower(sendbuf[i])) {
            sendbuf[i] -= 32;
        }
    }
    
    for (int i = 0; i < len; i++) { // shift and check, if after shift it is still in range [A-Z]
        decipherCh = str[i] + shift;
        sendbuf[i] = isalpha(decipherCh) ? decipherCh : ((decipherCh - 'A') % ('Z' - 'A' + 1)) + 'A';
    }
    sendbuf[len] = '\0';
}

void printSubstr(char *str, int start, int end) {
    for (int i = start; i <= end; i++) {
        printf("%c", str[i]);
    }
}

void printOntheSide(char *str, bool right) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    COORD point = csbi.dwCursorPosition;
    int len = strlen(str), half = csbi.dwSize.X/2, end = 0;
    point.X = (right) ? half : 0;

    for (int i = 0; i < len; i++) {
        for (int j = i; j < len; j++) {
            if (((str[j] == ' ' || str[j] == '\n' || str[j] == '\0') 
                && ((right && abs(point.X) < abs(csbi.dwSize.X)) || (!right && abs(point.X) < half-1))) 
                || j+1 == len) {
                end = j;
            }
            if (((right && abs(point.X) >= abs(csbi.dwSize.X)) || (!right && abs(point.X) >= half-1)) || j+1 == len) {
                point.X = (right) ? half : 0;
                SetConsoleCursorPosition(hConsole, point);

                printSubstr(str, i, end);
                i += (end-i);

                point.X = (right) ? half : 0;
                point.Y += 1;
                SetConsoleCursorPosition(hConsole, point);
                break;
            }
            point.X++;
        }
    }

    point.X = 0;
    SetConsoleCursorPosition(hConsole, point);
}

void printLog(char *str, int bytes, bool client) {
    if (client) {
        printOntheSide(str, false); // left side -> false right
        fprintf(LOG, "C > %s [bytes send: %d]\n", str, bytes);
    } else {
        printOntheSide(str, true); // right side -> true right
        fprintf(LOG, "S < %s [bytes received: %d]\n", str, bytes);
    }
}

int main() {
    WORD origAttr = getConsoleAttr(); // save original white console color of text
    SOCKET ConnectSocket;
    char *sendbuf = malloc(DEFAULT_BUFLEN);
    strcpy(sendbuf, " ");
    char *recvbuf = malloc(DEFAULT_BUFLEN);
    char *aisId = malloc(DEFAULT_BUFLEN);
    LOG = fopen("./log.chat", "w");

    SetConsoleOutputCP(CP_UTF8); // display diacriticts
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN); // display console in green color

    ConnectSocket = initConnection();

    // >>>>>>>>>>>>> communication <<<<<<<<<<<<<
    while (1) {
        sendMessage(ConnectSocket, sendbuf);
        receiveMessage(ConnectSocket, recvbuf);

        if (strstr(recvbuf, "I need you to do something for me...")) {
            strcpy(aisId, sendbuf);
        }
        if (strstr(recvbuf, "5 digits of your student ID")) {
            aisIdReminder(sendbuf, aisId); // computed reminder from sum of first 5 numbers of ais id
            continue;
        }
        if (strcmp(sendbuf, "123") == 0) {
            decipherMsg(sendbuf, recvbuf, 55); // decipher message
            continue;
        }
        if (strcmp(sendbuf, "PRIMENUMBER") == 0) {
            primeNumberLetters(sendbuf, recvbuf); // from recvbuf take every character on prime number letter positions
            continue;
        }
        if (strstr(recvbuf, "THE BONUS IS HERE")) {
            caesarCipher(sendbuf, "OBVMHKR", 7); // ceaser cipher shift by 7
            continue;
        }
        if (strcmp(sendbuf, "VICTORY") == 0) { // end of communication
            break;
        }
        
        getSendbuf(sendbuf);
    };

    free(sendbuf);
    free(recvbuf);
    free(aisId);
    fclose(LOG);
    disconnect(ConnectSocket);
    SetConsoleTextAttribute(hConsole, origAttr);

    return 0;
}