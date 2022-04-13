#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "blok3_richnakova.h"

/** run program:
 * cd "c:\Users\emari\Projects\API\blok3\" && "C:\Program Files (x86)\MinGW\bin\"gcc.exe blok3_richnakova.c -o blok3_richnakova -lws2_32 && "c:\Users\emari\Projects\API\blok3\"blok3_richnakova
 */

int init(struct addrinfo **resultP) {
    WSADATA wsaData; // struktura WSADATA pre pracu s Winsock
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData); // zakladna inicializacia
    if (iResult != 0) { // kontrola, ci nestala chyba
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    struct addrinfo *result = NULL; // struktura pre pracu s adresami
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
        return 1;
    } else {
        printf("getaddrinfo didn't fail...\n");
    }

    memmove(resultP, &result, sizeof(struct addrinfo));

    return 0;
}

int createConnection(struct addrinfo **resultP, struct addrinfo **ptrP) {
    int iResult;
    //vytvorenie socketu a pripojenie sa
    SOCKET ConnectSocket = INVALID_SOCKET;

    // Attempt to connect to the first address returned by
    // the call to getaddrinfo
    struct addrinfo *result = *resultP, *ptr = result;

    // Create a SOCKET for connecting to server => pokus o vytvorenie socketu
    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,ptr->ai_protocol);

    if (ConnectSocket == INVALID_SOCKET) { // kontrola, ci nenastala chyba
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
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
        return 1;
    }

    Sleep(250);
    
    return 0;
}

int main() {
    struct addrinfo *resultP = NULL, *ptrP = NULL;

    int initResult = init(&resultP);
    if (initResult != 0) {
        return initResult;
    }

    int connectResult = createConnection(&resultP, &ptrP);
    if (connectResult != 0) {
        return connectResult;
    }

    return 0;
}