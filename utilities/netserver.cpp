// Notice how much can we use purely from clib
#include <vector>
#include <thread>
#include <error.h>
#include <dlfcn.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
typedef unsigned int u32;
typedef unsigned short u16;
u32 sock, stat;
u16 port = 8081;
#define BUFFER_SIZE 512

struct Client {
    u32 index;
    long auth;
    sockaddr_in addr;
};
struct Message {
    char msg[BUFFER_SIZE];
    u32 index;
};
std::vector<Client> clients;
sockaddr_in local_addr{
    AF_INET,
    htons(port),
    {INADDR_ANY},
    {sizeof(sockaddr_in)}
};
typedef void (*funcinit)(std::vector<Client> *, void (*)(Message, Client));
typedef void (*functick)();
typedef void (*funcrecv)(Message, Client);
void help(int argc, char **argv) {
    if(argc > 1 && !strcmp(argv[1], "-h"))
        std::cout << "Run in directory with your handler.so and it will run dedicated engine server with it!\n";
    exit(0);
}
void send(Message m, Client c) {
    sendto(sock, m.msg, 512, 0, (sockaddr*)&c.addr, sizeof(sockaddr_in));
}
bool running = true;
void t2(funcrecv fr) {
    while(running){
        Message m;
        Client c;
        socklen_t trash;
        recvfrom(sock, m.msg, 512, 0, (sockaddr*)&c.addr, &trash);
        fr(m, c);
    }
}
void exiter(int _code = 2) {
    (void)_code;
    running = false;
}
int main(int argc, char** argv) {
    help(argc, argv);
    sockaddr* addr = (sockaddr*)&local_addr;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    stat = bind(sock, addr, sizeof(sockaddr_in));

    if(!sock) error(1, 0, "Socket couldn've been opened!");
    if( stat) error(1, 0, "Socket couldn've been bound(firewall?)!");

    std::cout << "Server listening at port " << port << "...\n";

    void* handle = dlopen("handler.so", RTLD_NOW);
    if(!handle) error(1, 0, "File handler.so not found!");

    funcinit fi = reinterpret_cast<funcinit>(dlsym(handle, "init"));
    functick ft = reinterpret_cast<functick>(dlsym(handle, "tick"));
    funcrecv fr = reinterpret_cast<funcrecv>(dlsym(handle, "recv"));

    fi(&clients, send);
    std::thread(t2, fr).detach();
    signal(SIGINT, exiter);
    while(running){
        // Sends data to clients 60times a second
        ft();
        usleep(16700);
    }
    close(sock);
    return 0;
}
