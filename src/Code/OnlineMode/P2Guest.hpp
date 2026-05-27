#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "Packet.hpp"

class P2Guest {
private:
    int sockFd_;
    Packet lastState_;

public:
    P2Guest();
    ~P2Guest();

    void connectToHost(const char* ip, int port);
    void sendMove(int targetNode);
    void sendJumpWall(int targetNode);
    bool receiveState();
    const Packet& getState() const;
};