#ifndef PC_INFO_H
#define PC_INFO_H
#include "top.h"

// singleton class, holds player and other game-related info that the player "owns" (cards,relics,potions)
class all_pc_info
{
private:
    cardDeck deck;
    cardDeckCH draw_pile;
    cardDeckCH discard_pile;
    cardDeckCH hand;
    cardDeckCH exhausted;
    cardDeckCH the_stack;
    std::unique_ptr<pc> player;
    std::vector<relic*> relics;
    std::vector<potion*> potions;
    int potionSlots{ 3 };
    int gold_{ 0 };
    bool sim{false};
    int simCount{5};
    bool topCardOfDeckKnown{ false };
public:
    all_pc_info() {}
    ~all_pc_info();

    // initialising
    void initIronclad();
    void initVisuals();
    void resetIronclad();
    void changeDeck(int);
    void masterDeckMaker(curriculumStage,int);
    void makeDeck(std::vector<std::string>);
    void generateRandomDeck(deckGen dG);
    void changeDeck1();
    void changeDeck2();
    void changeDeck3();
    void makeStageDeck(curriculumStage);
    
    // reload from sim
    void loadNewPlayer(pc* _pc) { player = std::unique_ptr<pc>(_pc); }
    void loadNewPotions(std::vector<potion*> vp) { for (potion* p : potions) { delete p; } potions.clear(); potions = vp; }
    void loadNewRelics(std::vector<relic*> vr) { for (relic* r : relics) { delete r; } relics.clear(); relics = vr; }
    void loadNewHand(const cardDeckCH& h) { hand = h; }
    void loadNewDraw(const cardDeckCH& d) { draw_pile = d; }
    void loadNewDiscard(const cardDeckCH& d) { discard_pile = d; }
    void loadNewExhaust(const cardDeckCH& e) { exhausted = e; }
    void loadVars(const int& g, const int& tc) { gold_ = g; topCardOfDeckKnown = tc; }
    
    cardDeckCH& shuffleDeck(cardDeckCH& deck, bool trig);
    cardDeckCH& shuffleDeck(cardDeckCH& deck, int i);
    inline pc& getPlayer() { return *player; }
    
    // relics
    std::vector<relic*>& getRelics() { return relics; }
    void addRelic(relic* r);
    void loseRelicByIndex(int );
    void removeRelicByName(std::string);
    bool checkForRelic(std::string);
    
    // potions
    void addPotionInputs(std::vector<std::string>& validInps);
    int getPotionSlots() const { return potionSlots; }
    void addPotionSlots(int amount) { potionSlots += amount; }
    bool potionsFull() const { return potions.size() == potionSlots; }
    std::vector<potion*>& getPotions() { return potions; }
    potion* getPotion(int index) { if (index > potionSlots) { throw std::runtime_error("pot"); } return potions[index]; }
    bool addPotion(potion* p);
    void losePotion(int);
    void potionMenu(int index);
    void removeSmokeBomb();
    void removeFIAB();
    std::vector<int> bestPotionsInFight(bool onlyUseOne);
    bool checkForEntropic();

    // in-fight
    void relicsStartOfFight();
    void relicsStartOfTurn();
    void cardsStartOfFight();
    void readyDeckForFight();
    void addCardToDeck(std::map<std::string, std::unique_ptr<card>>::iterator& dtbCard, bool upgrade_ = false);
    void addCardToDeck(std::unique_ptr<card> dtbCard);
    void addCardToDeck(card* card_);
    void addCardToDrawFront(cardHolder);
    void addCardToDrawBack(cardHolder);
    void addCardToDrawRandom(cardHolder);
    void addCardToDiscard(cardHolder);
    void addCardToExhaust(cardHolder);
    void addCardToHand(cardHolder);
    void removeDiscardCard(cardDeckCH::iterator ch) { discard_pile.erase(ch); }
    void removeHandCard(cardDeckCH::iterator ch) { hand.erase(ch); }
    void removeDrawCard(cardDeckCH::iterator ch) { draw_pile.erase(ch); }
    void removeCard(const std::deque<std::unique_ptr<card>>::iterator& deckCard, bool print = false);
    void startTurn();
    void startTurnV();
    combatChoices needForSim(int);
    combatChoices needForSim(combatChoices);
    bool turnLoop();
    void turnLoop(int);
    void endTurn();
    void drawX(int);

    // utility
    void discardToDraw();
    void reshuffleDrawAndHand();
    void addCardToBackOfStack(cardHolder&& ch);
    void randomHandCostZero();
    void chooseHandCostZero();
    void makeCostZero(cardHolder& ch);
    void makeCostOne(cardHolder& ch);
    void makeCostZero(card* c);
    void makeCostOne(card* c);
    bool altToBasicCurseBottled();
    bool altToBasicCurseBottled(cardType);
    bool checkCardInHand(const std::string&);
    bool checkCardInHand(card*);
    void sendHandCardToDiscard(cardHolder&);
    void sendCardFromToHand(cardDeckCH& a, cardHolder& ch);
    void sendCardFromDrawToHand(card* c);
    void putDrawPileCardAtFront(card* c);
    int numberInHand(std::vector<cardType> cts);
    int numberInDraw(std::vector<cardType> cts);

    // card locations
    void printDeck();
    inline cardDeck& getDeck() { return deck; }
    inline cardDeckCH& getDiscardPile() { return discard_pile; }
    inline cardDeckCH& getDrawPile() { return draw_pile; }
    inline cardDeckCH& getExhausted() { return exhausted; }
    inline cardDeckCH& getHand() { return hand; }
    bool getTopCardKnown() const { return topCardOfDeckKnown; }
    int getDiscardSize() { return static_cast<int>(discard_pile.size()); }
    int getDrawSize() { return static_cast<int>(draw_pile.size()); }
    int getExhaustedSize() { return static_cast<int>(exhausted.size()); }
    int getHandSize() { return static_cast<int>(hand.size()); }

    void endFight();
    int getGold() const { return gold_; }
    void gainGold(int i);
    void loseGold(int i);
    void spendGold(int i);
    
    std::vector<int> deckForStats();
    std::vector<int> relicForStats();
    std::vector<int> potionsForStats();
    bool fineForCurse();
    bool checkForCurse();
    int cursesInDeck();
    void aiPotionChoice(std::vector<potion*> options, int maxHeld);
    int getGiryaCounter() const;
    void incrGiryaCounter();
    bool isEnergyRelicGood();
    bool isSneckoGood();
    bool couldHavePowered();
    bool couldHavePlayedAnother();
    bool fineToBlockMore(int);
    int validRemoveCount();
    int validTransformCount();
    int validDuplicateCount();
    int validUpgradeCount();
    int compareBossRelics(std::vector<relic*>&, int);
    std::vector<int> twoRandomSkillIDs();
    std::vector<int> twoRandomAttackIDs();
    int randomUpgradeID();

};

extern all_pc_info apci;

#endif