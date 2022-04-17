#include <winsock2.h>
#include <ws2tcpip.h>

#define AP_SERVER_IP_ADDR "147.175.115.34"
#define AP_SERVER_PORT "777"
#define DEFAULT_BUFLEN 4096

SOCKET initConnection();
void disconnect(SOCKET ConnectSocket);
void sendMessage(SOCKET ConnectSocket, char *sendbuf);
char *receiveMessage(SOCKET ConnectSocket);
int getIndexOfNewLine(char *str);
char *setSendbuf(char *sendbuf);
char *aisIdReminder(char *aisId);