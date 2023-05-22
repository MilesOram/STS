#ifndef SAVE_H
#define SAVE_H
#include "top.h"

// save during fight, make clones of everything that could change in one action
class savestate
{
private:
    // copies
    pc* player_{ nullptr };
    std::vector<relic*> relics_;
    std::vector<potion*> potions_;
    std::vector<nonpc*> enemies_;
    std::vector<std::vector<nonpc*>> dormants_;
    cardDeckCH cards_draw_;
    cardDeckCH cards_discard_;
    cardDeckCH cards_hand_;
    cardDeckCH cards_exhaust_;
    // vars
    std::vector<int> gm_ints_; // playerEnergy_, xCost, doubleAttackMode, allcounts
    std::vector<bool> gm_static_bools_;
    std::vector<int> gm_static_ints_;
    std::deque<std::vector<Intents>> gm_intents_;

    std::vector<int> apci_ints; // gold_
    std::vector<bool> card_static_bools_;

    std::vector<float> nonpc_static_floats_;
    std::vector<bool> pc_static_bools_;

public:
    savestate() {}
    ~savestate();
    void saveGame();
    void loadGame();
    void clearAll();
};

// currently unused
class macro_savestate
{
private:
    // copies
    pc* player_{ nullptr };
    std::vector<relic*> relics_;
    std::vector<potion*> potions_;
    cardDeck deck_;
    // vars
    std::vector<bool> gm_static_bools_;
    std::vector<int> apci_ints; // gold_
    std::vector<bool> card_static_bools_;

public:
    macro_savestate() {}
    ~macro_savestate();
    void saveGame();
    void loadGame();
    void clearAll();
};


//extern macro_savestate msv;
extern savestate sv;

#endif