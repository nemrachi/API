#include "blok3_richnakova.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

/** run program:
 * cd "c:\Users\emari\Projects\API\blok3\" && "c:\Program Files (x86)\MinGW\bin\"gcc.exe blok3_richnakova.c -o blok3_richnakova -lws2_32 && .\blok3_richnakova < .\in.txt
 */

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

void disconnect(SOCKET ConnectSocket) {
    closesocket(ConnectSocket);
    WSACleanup();
}

void sendMessage(SOCKET ConnectSocket, char *sendbuf) {
    printf("%s\n", sendbuf);

    int iResult = send(ConnectSocket, sendbuf, strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed: %d\n", WSAGetLastError());
        disconnect(ConnectSocket);
        exit(1);
    }

    //printf("Bytes Sent: %ld\n", iResult); // vypisanie poctu odoslanych dat // TODO: log
}

void receiveMessage(SOCKET ConnectSocket, char *recvbuf) {
    memset(recvbuf, 0, DEFAULT_BUFLEN);

    int iResult = recv(ConnectSocket, recvbuf, DEFAULT_BUFLEN, 0); // funkcia na príjimanie
    if (iResult > 0) {
        //printf("Bytes received: %d\n", iResult); // prisli validne data, vypis poctu // TODO: log
    } else if (iResult == 0) {
        printf("Connection closed\n"); // v tomto pripade server ukoncil komunikaciu
    } else {
        printf("recv failed with error: %d\n", WSAGetLastError()); // ina chyba
    }
}

int getIndexOfNewLine(char *str) {
    int i = 0, len = strlen(str);
    while (str[i] != '\n' && i < len) {
        i++;
    }
    return i;
}

void getSendbuf(char *sendbuf) {
    memset(sendbuf, 0, DEFAULT_BUFLEN);
    fgets(sendbuf, DEFAULT_BUFLEN, stdin);
    sendbuf[getIndexOfNewLine(sendbuf)] = '\0';
}

void aisIdReminder(char *sendbuf, char *aisId) {
    int sum = 0, div;

    for (int i = 0; i < 5; i++) {
        sum += aisId[i] - '0';
    }
    div = aisId[4] - '0';
    div = (!div) ? 9 : div;
    aisId[0] = (sum % div) + '0';
    aisId[1] = '\0';

    strcpy(sendbuf, aisId);
}

void decipherMsg(char *sendbuf, char *recvbuf) {
    int len = 132;
    char *decipher = malloc(DEFAULT_BUFLEN);
    char key = 55;

    for (int i = 0; i < len; i++) {
        if (isprint(recvbuf[i]^key)) {
            decipher[i] = recvbuf[i]^key;
        } else {
            len--;
        }
    }
    decipher[len] = '\0';

    strcpy(sendbuf, decipher);
    free(decipher);
    printf("\n");
}

// void integralPart(char *sendbuf) {
//     int i = 0;
//     int j = 0;
//     char *intPart = malloc(DEFAULT_BUFLEN);
//     while (1) {
//         if (isdigit(sendbuf[i])) {
//             intPart[j++] = sendbuf[i];
//         } else if (sendbuf[i] == '.' && j > 0) {
//             intPart[j] = '\0';
//             return intPart;
//         } else {
//             memset(intPart, 0, j);
//             j = 0;
//         }
//         i++;
//     }
//     strcpy(sendbuf, intPart);
// }

void primeNumberLetters(char *sendbuf, char *recvbuf) {
    int count, k = 0, len = strlen(recvbuf);
    char *str = malloc(DEFAULT_BUFLEN);

    for (int i = 2; i < len; i++) {
        count = 0;
        for (int j = i-1; j >= 2; j--) {
            if (i % j == 0) { count++; }
        }
        if (!count) { str[k++] = recvbuf[i-1]; }
    }
    str[k] = '\0';
    
    strcpy(sendbuf, str);
    free(str);
}

void printRecvbuf(HANDLE hConsole, char *recvbuf) { // display my messages left, his right, no word split
    int len = strlen(recvbuf);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    COORD point = csbi.dwCursorPosition;
    short half = csbi.dwSize.X/2;
    int end;
    point.X = half;

    // while(1) {
    //     end = csbi.dwSize.X-half;
    //     while(recvbuf[end] != ' ') {
    //         end--;
    //     }
    // } // TODO: rozdelovat cez medzery

    for (int i = 0; i < len; i++) {
        if (point.X >= csbi.dwSize.X) {
            point.X = half;
            point.Y += 1;
        }
        SetConsoleCursorPosition(hConsole, point);
        putchar(recvbuf[i]);
        point.X += 1;
    }
}

// TODO: bytes send/recv log to file

int main() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    WORD origAttr = consoleInfo.wAttributes; // save original white console text

    SOCKET ConnectSocket;
    char *sendbuf = malloc(DEFAULT_BUFLEN);
    strcpy(sendbuf, " ");
    char *recvbuf = malloc(DEFAULT_BUFLEN);
    char *aisId = malloc(DEFAULT_BUFLEN);

    SetConsoleOutputCP(CP_UTF8); // 1.quest: display diacriticts
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN); // 2.quest: display console in green color

    ConnectSocket = initConnection();

    // >>>>>>>>>>>>> communication <<<<<<<<<<<<<
    while (1) {

        sendMessage(ConnectSocket, sendbuf);
        receiveMessage(ConnectSocket, recvbuf);
        printRecvbuf(hConsole, recvbuf);

        if (strstr(recvbuf, "I need you to do something for me...")) {
            strcpy(aisId, sendbuf);
        }
        if (strstr(recvbuf, "5 digits of your student ID")) {
            aisIdReminder(sendbuf, aisId); // 3.quest: computed ais id reminder
            continue;
        }
        if (strcmp(sendbuf, "123") == 0) {
            decipherMsg(sendbuf, recvbuf); // 4.quest: decipher message
            continue;
        }
        if (strcmp(sendbuf, "PRIMENUMBER") == 0) {
            memset(sendbuf, 0, DEFAULT_BUFLEN);
            primeNumberLetters(sendbuf, recvbuf); // 5.quest: take every character on prime number letter positions
            continue;
        }
        
        getSendbuf(sendbuf);
    };

    free(sendbuf);
    free(recvbuf);
    free(aisId);
    disconnect(ConnectSocket);
    SetConsoleTextAttribute(hConsole, origAttr);

    return 0;
}