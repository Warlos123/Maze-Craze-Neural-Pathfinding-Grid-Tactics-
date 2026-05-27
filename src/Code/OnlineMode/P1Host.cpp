#include "P1Host.hpp"
#include <cstring>
#include <iostream>

P1Host::P1Host() : listenFd_(-1), guestFd_(-1) {}

P1Host::~P1Host() {
    if (guestFd_ >= 0) close(guestFd_);
    if (listenFd_ >= 0) close(listenFd_);
}

void P1Host::waitForGuest(int port) {
    listenFd_ = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(listenFd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    bind(listenFd_, (sockaddr*)&addr, sizeof(addr));
    listen(listenFd_, 1);

    std::cout << "Waiting for P2 in port: " << port << "...\n";
    guestFd_ = accept(listenFd_, nullptr, nullptr);
    std::cout << "P2 is connected. Starting Game.\n";

    game_.init(Algorithm::DFS);

    sendState();
}

void P1Host::sendState() {
    Packet pkt{};
    pkt.action = ActionType::GAME_STATE;
    pkt.p1Node = game_.getP1Node();
    pkt.p2Node = game_.getP2Node();
    pkt.treasureNode = game_.getTreasureNode_();
    pkt.currentPlayer = (game_.getCurrentPlayer() == PlayerTurn::P1) ? 0 : 1;

    send(guestFd_, &pkt, sizeof(pkt), 0);
}

bool P1Host::receiveAndApply() {
    Packet pkt{};
    ssize_t n = recv(guestFd_, &pkt, sizeof(pkt), 0);
    if (n <= 0) return false;

    if (pkt.action == ActionType::MOVE)
        game_.move(pkt.targetNode);
    else if (pkt.action == ActionType::USE_JUMP)
        game_.useJumpWall(pkt.targetNode);

    sendState();
    return true;
}

Game& P1Host::getGame(){
    return game_;
}