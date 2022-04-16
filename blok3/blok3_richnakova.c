#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "blok3_richnakova.h"

/** run program:
 * cd "c:\Users\emari\Projects\API\blok3\" && "C:\Program Files (x86)\MinGW\bin\"gcc.exe blok3_richnakova.c -o blok3_richnakova -lws2_32 && "c:\Users\emari\Projects\API\blok3\"blok3_richnakova
 */

SOCKET ConnectSocket = INVALID_SOCKET;

int init(struct addrinfo *resultMain) {
    WSADATA wsaData; // struktura WSADATA pre pracu s Winsock
    // Initialize Winsock
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData); // zakladna inicializacia
    if (iResult != 0) { // kontrola, ci nestala chyba
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    struct addrinfo *result = NULL;
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

    memmove(resultMain, result, sizeof(struct addrinfo));

    return 0;
}

int createConnection(struct addrinfo *resultMain) {
    int iResult;
    // vytvorenie socketu a pripojenie sa
    // Attempt to connect to the first address returned by
    // the call to getaddrinfo
    struct addrinfo *ptr = resultMain;

    // Create a SOCKET for connecting to server => pokus o vytvorenie socketu
    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,ptr->ai_protocol);

    if (ConnectSocket == INVALID_SOCKET) { // kontrola, ci nenastala chyba
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(resultMain);
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

int sendMessage(char *sendbuf) {
    int iResult = send(ConnectSocket, sendbuf, sizeof(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed: %d\n", WSAGetLastError());
        endSocket();
        return 1;
    }

    printf("Bytes Sent: %ld\n", iResult); // vypisanie poctu odoslanych dat

    return 0;
}

void receiveMessage() {
    char recvbuf[DEFAULT_BUFLEN];
    int iResult = recv(ConnectSocket, recvbuf, DEFAULT_BUFLEN, 0); // funkcia na príjimanie
    if (iResult > 0) {
        printf("Bytes received: %d\n", iResult); // prisli validne data, vypis poctu
    } else if (iResult == 0) {
        printf("Connection closed\n"); // v tomto pripade server ukoncil komunikaciu
    } else {
        printf("recv failed with error: %d\n", WSAGetLastError()); // ina chyba
    }
    printf("\n%s", recvbuf);

    return;
}

void endSocket() {
    closesocket(ConnectSocket);
    WSACleanup();
}

int main() {
    // ############### connect to the server ###############
    struct addrinfo result; // struktura pre pracu s adresami

    int initResult = init(&result);
    if (initResult != 0) {
        return initResult;
    }

    int connectResult = createConnection(&result);
    if (connectResult != 0) {
        return connectResult;
    }

    // ############### send/receive message ###############
    // >> contact server
    char sendbuf[DEFAULT_BUFLEN] = ""; // buffer (v zasade retazec), kam sa budu ukladat data, ktore chcete posielat
    int sendResult = sendMessage(sendbuf);
    if (sendResult != 0) {
        return sendResult;
    }
    
    receiveMessage();

    // >> send AIS ID
    memset(&sendbuf, 0, DEFAULT_BUFLEN);
    memcpy(&sendbuf, "97025", sizeof(sendbuf));
    
    //fgets(sendbuf, DEFAULT_BUFLEN, stdin); // \n -> \0
    sendResult = sendMessage(sendbuf);
    if (sendResult != 0) {
        return sendResult;
    }

    receiveMessage();

    // ############### close socket ###############
    endSocket();

    return 0;
}