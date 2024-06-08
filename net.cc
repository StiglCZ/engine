#ifdef _WIN32
#define TODO
#pragma message(TODO "Add windows support")
#endif

#ifdef __unix__
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define NET_SOURCE
#include "net.hh"

typedef struct NetClient Client;
typedef struct NetMessage Message;
Client Begin() {
    Client cl;
    cl.socket = socket(AF_INET, SOCK_DGRAM, 0);
    if(!cl.socket)return {false, 0, {}, 0};
    return cl;
}
void Host(Client& cl, u16 port) {
    struct sockaddr_in addr ={
        AF_INET,
        htons(port),
        {INADDR_ANY},
        {cl.addrSize}
    };
    cl.addr = addr;
    cl.success =
        bind(cl.socket, (struct sockaddr*)&cl.addr, sizeof(cl.addrSize));
}

u32 CreateIP(char* addr) {
    return inet_addr(addr);
}
void Send(Client& cl, char* msg, u32 msglen) {
    send(cl.socket, msg, msglen, 0);
}
Message Recieve(Client& cl) {
    i8 buffer[NET_BUFFERSIZE];
    u8 addrBuffer[sizeof(sockaddr_in)];
    socklen_t addrSize;
    int buffSize =
        recvfrom(cl.socket, buffer, NET_BUFFERSIZE, 0, (sockaddr*)&addrBuffer, &addrSize);
    Message msg;
    msg.addrSize = addrSize;
    msg.buffSize = buffSize;
    memcpy(msg.message, buffer, buffSize);
    msg.addr = *(sockaddr_in*)addrBuffer;
    return msg;
}
void End(Client& cl) {
    close(cl.socket);
}
#endif
