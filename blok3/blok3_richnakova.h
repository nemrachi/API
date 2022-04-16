#define AP_SERVER_IP_ADDR "147.175.115.34"
#define AP_SERVER_PORT "777"
#define DEFAULT_BUFLEN 4096

int init(struct addrinfo *resultMain);
int createConnection(struct addrinfo *resultMain);
int sendMessage(char *sendbuf);
void receiveMessage();
void endSocket();