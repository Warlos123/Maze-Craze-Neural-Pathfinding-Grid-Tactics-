#include "P2Guest.hpp"
#include <cstring>
#include <iostream>

P2Guest::P2Guest() : sockFd_(-1) {}

P2Guest::~P2Guest() {
    if (sockFd_ >= 0) close(sockFd_);
}

void P2Guest::connectToHost(const char* ip, int port) {
    sockFd_ = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(port);
    inet_pton(AF_INET, ip, &addr.sin_addr);

    if (connect(sockFd_, (sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "Can't connect with the host.\n";
        return;
    }
    std::cout << "Connected to the host.\n";

    receiveState();
}

void P2Guest::sendMove(int targetNode) {
    Packet pkt{};
    pkt.action     = ActionType::MOVE;
    pkt.targetNode = targetNode;
    send(sockFd_, &pkt, sizeof(pkt), 0);
}

void P2Guest::sendJumpWall(int targetNode) {
    Packet pkt{};
    pkt.action     = ActionType::USE_JUMP;
    pkt.targetNode = targetNode;
    send(sockFd_, &pkt, sizeof(pkt), 0);
}

bool P2Guest::receiveState() {
    ssize_t n = recv(sockFd_, &lastState_, sizeof(lastState_), 0);
    return n > 0;
}

const Packet& P2Guest::getState() const { return lastState_; }