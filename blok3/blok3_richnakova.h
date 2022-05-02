#include <winsock2.h>
#include <ws2tcpip.h>

#define AP_SERVER_IP_ADDR "147.175.115.34"
#define AP_SERVER_PORT "777"
#define DEFAULT_BUFLEN 4096
typedef char bool;
#define true 1
#define false 0

WORD getConsoleAttr();
SOCKET initConnection();
void sendMessage(SOCKET ConnectSocket, char *sendbuf);
void receiveMessage(SOCKET ConnectSocket, char *recvbuf);
void disconnect(SOCKET ConnectSocket);
void getSendbuf(char *sendbuf);
int getIndexOfNewLine(char *str);
void aisIdReminder(char *sendbuf, char *aisId);
void decipherMsg(char *sendbuf, char *recvbuf, char key);
void primeNumberLetters(char *sendbuf, char *recvbuf);
void caesarCipher(char *sendbuf, char *str, int shift);
void printSubstr(char *str, int start, int end);
void printOntheSide(char *str, bool right);
void printLog(char *str, int bytes, bool client);