#ifndef NET
#define NET
#include "types.hh"
#define NET_BUFFERSIZE 1024

#ifndef NET_SOURCE
struct sockaddr_in {
    u16 sin_family;
    u16 sin_port;
    u32 sin_addr;
    i8 sin_zero[8];
};
#endif

struct NetClient {
    i8 success;
    u32 socket;
    sockaddr_in addr;
    const u8 addrSize =
        sizeof(sockaddr_in);
};
struct NetMessage {
    u32 buffSize;
    u8  addrSize;
    u8 message[NET_BUFFERSIZE];
    sockaddr_in addr;
};
extern u32 CreateIP(char* addr);
extern NetClient Begin();
extern void Host(NetClient &cl, u16 port);
extern void Send(NetClient& cl, char* msg, u32 msglen);
extern NetMessage Recieve(NetClient& cl);
#endif
