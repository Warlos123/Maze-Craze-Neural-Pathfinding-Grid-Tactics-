#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "Packet.hpp"
#include "../Game/Game.hpp"

class P1Host {
private:
    int listenFd_;
    int guestFd_;
    Game game_;

public:
    P1Host();
    ~P1Host();

    void waitForGuest(int port);
    void sendState();
    bool receiveAndApply();
    Game& getGame();
};