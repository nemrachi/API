#include "blok3_richnakova.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

typedef int bool;
#define TRUE  1
#define FALSE 0

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
        printf("Error at socket(): %ld\n", WSAGetLastError());
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
    int iResult = send(ConnectSocket, sendbuf, sizeof(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed: %d\n", WSAGetLastError());
        disconnect(ConnectSocket);
        exit(1);
    }

    printf("Bytes Sent: %ld\n", iResult); // vypisanie poctu odoslanych dat
}

char *receiveMessage(SOCKET ConnectSocket) {
    char *recvbuf = malloc(DEFAULT_BUFLEN);
    memset(recvbuf, 0, DEFAULT_BUFLEN);
    int iResult = recv(ConnectSocket, recvbuf, DEFAULT_BUFLEN, 0); // funkcia na príjimanie
    if (iResult > 0) {
        printf("Bytes received: %d\n", iResult); // prisli validne data, vypis poctu
    } else if (iResult == 0) {
        printf("Connection closed\n"); // v tomto pripade server ukoncil komunikaciu
    } else {
        printf("recv failed with error: %d\n", WSAGetLastError()); // ina chyba
    }
    
    return recvbuf;
}

int getIndexOfNewLine(char *str) {
    int i = 0, len = strlen(str);
    while (str[i] != '\n' && i < len) {
        i++;
    }
    return i;
}

char *setSendbuf(char *sendbuf) {
    memset(sendbuf, 0, DEFAULT_BUFLEN);
    fgets(sendbuf, DEFAULT_BUFLEN, stdin);
    sendbuf[getIndexOfNewLine(sendbuf)] = '\0';
    return sendbuf;
}

char *aisIdReminder(char *aisId) {
    int sum = 0;
    for (int i = 0; i < 5; i++) {
        sum += aisId[i] - '0';
    }

    int div = aisId[4] - '0';
    div = (!div) ? 9 : div;

    char *result = malloc(2*sizeof(char));
    result[0] = (sum % div) + '0';
    result[1] = '\0';

    return result;
}

void printRecvbuf(HANDLE hConsole, bool separated, char *recvbuf) {
    if (separated) {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        COORD point = csbi.dwCursorPosition;
        point.X = csbi.dwSize.X/2;

        for (int i = 0; i < strlen(recvbuf); i++) {
            point.X += 1;
            if (point.X == csbi.dwSize.X) {
                point.X = csbi.dwSize.X/2;
                point.Y += 1;
            }
            SetConsoleCursorPosition(hConsole, point);
            putchar(recvbuf[i]);
        }
    } else {
        printf("%s\n", recvbuf);
    }
}

int main() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    WORD origAttr = consoleInfo.wAttributes;
    bool separatedPrint = FALSE;

    SOCKET ConnectSocket;
    char sendbuf[DEFAULT_BUFLEN]; // buffer (v zasade retazec), kam sa budu ukladat data, ktore chcete posielat
    char recvbuf[DEFAULT_BUFLEN];

    for (int i = 0; i < 4; i++) {
        ConnectSocket = initConnection();

        sendMessage(ConnectSocket, ""); // > random string
        strcpy(recvbuf, receiveMessage(ConnectSocket)); // < connected to server, authentication
        printRecvbuf(hConsole, separatedPrint, recvbuf);

        sendMessage(ConnectSocket, setSendbuf(sendbuf)); // > ais id
        strcpy(recvbuf, receiveMessage(ConnectSocket)); // < do something
        printRecvbuf(hConsole, separatedPrint, recvbuf);

        // Do something for me -> What?
        sendMessage(ConnectSocket, setSendbuf(sendbuf)); // > What?
        strcpy(recvbuf, receiveMessage(ConnectSocket)); // < disconnect, reprogram (display diacriticts), reconnect, send 7545477
        printRecvbuf(hConsole, separatedPrint, recvbuf);

        if (i == 0) {
            disconnect(ConnectSocket);
            SetConsoleOutputCP(CP_UTF8);
            continue;
        }

        sendMessage(ConnectSocket, setSendbuf(sendbuf)); // > 7545477
        strcpy(recvbuf, receiveMessage(ConnectSocket)); // < disconnect, reprogram (display console in green color), reconnect, send 753421
        printRecvbuf(hConsole, separatedPrint, recvbuf);

        if (i == 1) {
            disconnect(ConnectSocket);
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
            continue;
        }

        sendMessage(ConnectSocket, setSendbuf(sendbuf)); // > 753421
        strcpy(recvbuf, receiveMessage(ConnectSocket)); // < sum of first 5 digits of ais id / 5th digit (if 0 -> 9)
        printRecvbuf(hConsole, separatedPrint, recvbuf);

        sendMessage(ConnectSocket, aisIdReminder(setSendbuf(sendbuf))); // < computed ais id reminder
        strcpy(recvbuf, receiveMessage(ConnectSocket)); // > disconnect, reprogram (display my messages left, his right), reconnect, send 333222111
        printRecvbuf(hConsole, separatedPrint, recvbuf);

        if (i == 2) {
            disconnect(ConnectSocket);
            separatedPrint = TRUE;
            continue;
        }

        // >> send code -> 333222111
        sendMessage(ConnectSocket, setSendbuf(sendbuf));
        strcpy(recvbuf, receiveMessage(ConnectSocket));
        printRecvbuf(hConsole, separatedPrint, recvbuf);
    };

    disconnect(ConnectSocket);
    SetConsoleTextAttribute(hConsole, origAttr);

    return 0;
}