#pragma once
#include <cstdint>

enum class ActionType : uint8_t {
    MOVE        = 1,
    USE_JUMP    = 2,
    GAME_STATE  = 3,
    GAME_OVER   = 4,
    READY       = 5
};

struct Packet {
    ActionType action;
    int        targetNode;
    int        p1Node;
    int        p2Node;
    int        treasureNode;
    int        currentPlayer;
};