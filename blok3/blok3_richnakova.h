#include <winsock2.h>
#include <ws2tcpip.h>

#define AP_SERVER_IP_ADDR "147.175.115.34"
#define AP_SERVER_PORT "777"
#define DEFAULT_BUFLEN 4096
typedef int bool;
#define TRUE  1
#define FALSE 0

SOCKET initConnection();
void disconnect(SOCKET ConnectSocket);
void sendMessage(SOCKET ConnectSocket, char *sendbuf);
void receiveMessage(SOCKET ConnectSocket, char *recvbuf);
int getIndexOfNewLine(char *str);
void setSendbuf(char *sendbuf);
void aisIdReminder(char *sendbuf, char *aisId);
void printRecvbuf(HANDLE hConsole, char *recvbuf);