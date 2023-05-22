#ifndef TOP_H
#define TOP_H

#include<iostream>
#include<cmath>
#include<limits>
#include<sstream>
#include<map>
#include<stack>
#include<iomanip>
#include<string>
#include<vector>
#include<fstream>
#include<istream>
#include<memory>
#include<algorithm>
#include<functional>
#include<map>
#include<random>
#include<iostream>
#include<stack>
#include<deque>
#include<queue>
#include<set>
#include<bitset>

// forward declarations
class card;
class character;
class gamestate;
class all_pc_info;
class relic;
class potion;
class events;
class pc;
class nonpc;
class easyHallway;
class hardHallway;
class elites;
class bosses;
class qSpecial;
class campfire;
class questionMark;
struct stateInfo;
struct macroStateInfo;
class pythonFunctions;

class cardHolder
{
private:
    static int unique_id;
    int id;
    card* card_;
public:
    cardHolder(card&);
    cardHolder(card* c) { card_ = c; id = unique_id++; }
    cardHolder(const cardHolder&);
    cardHolder(cardHolder&&) noexcept;
    cardHolder& operator=(const cardHolder&);
    cardHolder& operator=(cardHolder&&) noexcept;
    ~cardHolder();
    card& operator*() const { return *card_; }
    card& operator->() const { return *card_; }
    bool operator==(const cardHolder& ch) { return id == ch.id; }
    card* getCardPointer() const { return card_; }
    void nullThis() { card_ = nullptr; }
    friend void swap(cardHolder&, cardHolder&);
    int getID() const { return id; }
};
struct Intents
{
    std::string effect;
    int value;
    int adjustedValue;
    int times;
    Intents(std::string e, int v, int t) :effect(e), value(v), times(t), adjustedValue(v) {}
    Intents(const Intents& i) { effect = i.effect; value = i.value; adjustedValue = i.adjustedValue; times = i.times; }
    Intents& operator=(const Intents& i) { effect = i.effect; value = i.value; adjustedValue = i.adjustedValue; times = i.times; return *this; }
};
// triggers observe the relevant focusedEvent object
class observer;
struct observerSort
{
    bool operator()(observer* const&, observer* const&) const;
};
typedef std::multiset<observer*, observerSort> observer_set;

// stored in gamestate for each noteworthy event - cardDrawn, playerHPloss, cardPlayed etc.
class focusedEvent
{
private:
    observer_set observers;
    std::string name_;
public:
    focusedEvent() {}
    focusedEvent(std::string n) : name_(n) {}
    ~focusedEvent() { removeAllObservers(); }
    void addObserver(observer* o) { observers.insert(o); }
    void removeObserver(observer* o);
    void removeAllObservers();
    void notify(character& initiator);
    void notify(cardHolder& ch);
    void notify();
    observer_set getObservers() const { return observers; }
};
class observer
{
protected:
    focusedEvent* observing_;
    int triggerPrio{ 100 };
    std::string name_;
    bool active_{ true };
public:
    observer(focusedEvent* fe) : observing_(fe) {}
    observer(focusedEvent* fe, std::string name) : observing_(fe), name_(name) {}
    observer(focusedEvent* fe, std::string name, int trigPrio) : triggerPrio(trigPrio), observing_(fe), name_(name) {}
    observer(const observer& obs) { observing_ = obs.observing_; triggerPrio = obs.triggerPrio; name_ = obs.name_; }
    virtual void triggered(character&) { triggered(); }
    virtual void triggered(cardHolder&) { triggered(); }
    virtual void triggered() {}
    inline void triggeredACT(character& initiator) { if (active_) triggered(initiator); }
    inline void triggeredACT(cardHolder& ch) { if (active_) triggered(ch); }
    inline void triggeredACT() { if (active_) triggered(); }
    void observeThis(focusedEvent* fe) { if (fe == observing_) return; observing_ = fe; fe->addObserver(this); }
    // only used by fe
    void observeNull() { observing_ = nullptr; }
    //void cardTriggered(cardHolder&);
    virtual ~observer() { if (observing_) { observing_->removeObserver(this); } }
    focusedEvent* getObserving() { return observing_; }
    bool operator<(const observer& obs) const { return triggerPrio < obs.triggerPrio; }
    virtual void changeHoldsObs(card* ) { throw std::runtime_error(" shouldn't be using this (changeHoldsObs)"); }

    virtual observer* clone() { throw std::runtime_error(" shouldn't be using this either (clone)"); }
    virtual observer* clone(card* ) { return clone(); }
    virtual observer* clone(character* ) { return clone(); }
    void setInactive() { active_ = false; }
    void setActive() { active_ = true; }
    bool getActive() const { return active_; }
    int getTriggerPrio() const { return triggerPrio; }
    bool test(observer const& right) const { return triggerPrio < right.triggerPrio; }
};

class holdsObservers
{
protected:
    observer_set observers_;
public:
    holdsObservers() {}
    virtual ~holdsObservers() { deleteAllObservers(); }
    holdsObservers(const holdsObservers&);
    holdsObservers(holdsObservers&&) noexcept;
    template<typename T> void observeEvent(focusedEvent*, T* );
    template<typename T> void observeEvent(focusedEvent*, T* , bool);
    template<typename T> observer* checkForObserving(focusedEvent*, T* );
    void stopObserving(focusedEvent* );
    template<typename T> void removeObserver();
    template<typename T> void removeObserverIfFE(focusedEvent*);
    void removeObserver(observer* );
    void deleteAllObservers();
    void safeRemoveObserver(observer* );
    void clearNullObservers();
    observer_set& getObservers();
};

typedef std::deque<std::unique_ptr<card>> cardDeck;
typedef std::deque<cardHolder> cardDeckCH;

enum cardType { Attack = 0, Skill = 1, Power = 2, Curse = 3, Status = 4 };
enum cardRarity { Common = 0, Uncommon = 1, Rare = 2, Other = 3 };
enum class relicRarity { Common = 0, Uncommon = 1, Rare = 2, Boss = 3, Shop = 4, Event = 5 };
enum class chestSize { Small, Medium, Large, Boss };
enum class potionUse { SelfAny, SelfCombat, Enemy, NonActive };
enum class combatChoices { None, HandDW, HandAR, HandWC, HandTA, Hand, HandMulti3, HandMulti5, HandMultiAll, Exhaust, Discard,
                            EnemyTarget, Headbutt, Choice3Skip, Choice3NoSkip, DrawAtt, DrawSkl, Havoc, Potion };
enum class cardClass { IC, SI, DE, WA, Colourless, Curse};
enum class deckGen { EH1, HH1, EL1, BO1, EH2, HH2, EL2, BO2, EH3, HH3, EL3, BO3, EL4, BO4, QS1, QS2, QS3 };
enum class curriculumStage { Stage1, Stage2, Stage3, Stage4, Stage5, Stage6 };
enum class aiEventCardChoice { Add, Remove, Upgrade, Duplicate, Transform, Transform2 };

std::string request_input(std::vector<std::string> );
int random_int(int, int);
int random_int(int, size_t);
int random_intNot(int , int , int );
int random_intNot(int , size_t, int );
float random_float(float, float);
bool checkLastXAttacks(std::vector<std::string>& , int , std::string );
bool checkLastX(std::vector<char>& , int , char , int );
struct incomingAttack
{
    int baseDamage{0};
    int adjustedDamage{0};
    int times{0};
    incomingAttack(int bd, int ad, int t) : baseDamage(bd), adjustedDamage(ad), times(t) {}
    incomingAttack() {}
};

void initVisualsPlayer();
void initVisualsOne();
void initVisualsTwo();
void initVisualsThree();
void initVisualsFour();
void initVisualsFive();

void showAIDecisions(pythonFunctions& pyFunc_);
int callMacroIndexCONSTPRED(const std::vector<macroStateInfo>&, pythonFunctions& pyFunc_);
int callMacroIndexREROLLPRED(const std::vector<macroStateInfo>&, pythonFunctions& pyFunc_);
float callMacroSINGLE(const macroStateInfo&, pythonFunctions& pyFunc_);
std::vector<float> callPredSINGLE(const macroStateInfo&, pythonFunctions& pyFunc_);
std::vector<std::vector<float>> callPredBATCH(const std::vector<macroStateInfo>&, pythonFunctions& pyFunc_);
float callMacroValueCONSTPRED(const std::vector<macroStateInfo>&, pythonFunctions& pyFunc_);
float callMacroValueREROLLPRED(const std::vector<macroStateInfo>&, pythonFunctions& pyFunc_);
void outerTurnLoopAI(pythonFunctions& pyFunc);
int callMicroIndex(const std::vector<stateInfo>&, pythonFunctions& pyFunc_);
std::vector<float> callMicroValues(const std::vector<stateInfo>&, pythonFunctions& pyFunc_);

class potion
{
protected:
    cardRarity rarity_;
    std::string name_;
    potionUse potionUsageReq_;
    int ranking_{0};
    bool situational_{ false };
    bool reliable_{ false };
public:
    potion(std::string s, cardRarity pr, potionUse pur) : name_(s), rarity_(pr), potionUsageReq_(pur) {}
    potion(std::string s, cardRarity pr, potionUse pur, int rank, bool sit, bool rel) :
        name_(s), rarity_(pr), potionUsageReq_(pur), ranking_(rank), situational_(sit),reliable_(rel) {}
    virtual ~potion() {}
    bool canUse() const;
    virtual void pickUp() {}
    std::string getName() const { return name_; }
    cardRarity getRarity() const { return rarity_; }
    potionUse getPotionUsage() const { return potionUsageReq_; }
    int getRank() const { return ranking_; }
    bool getSituational() const { return situational_; }
    bool getReliable() const { return reliable_; }
    virtual combatChoices getCombatChoices() const { return combatChoices::None; }
    virtual void execute() = 0;
    virtual potion* clone() const = 0;
    virtual std::unique_ptr<potion> cloneU() const = 0;
    virtual int getPotionIDNum() const = 0;
};

class relic : public observer
{
protected:
    relicRarity rarity_;
    std::string name_;
public:
    relic(relicRarity r, std::string s) : rarity_(r), name_(s), observer(nullptr, s) {}
    relic(relicRarity r, std::string s, int trigPrio) : rarity_(r), name_(s), observer(nullptr, s, trigPrio) {}
    relic(relicRarity r, std::string s, focusedEvent* fe) : rarity_(r), name_(s), observer(fe, s) {}
    relic(relicRarity r, std::string s, focusedEvent* fe, int trigPrio) : rarity_(r), name_(s), observer(fe, s, trigPrio) {}
    virtual ~relic() {}
    virtual void onPickUp() {}
    virtual void onDropped() {}
    virtual void execute() {}
    virtual void turnStartSetup() {}
    virtual void fightStartSetup() {};
    virtual void randomizeCounter() {};
    virtual int getCounter() { return -1; }
    virtual float getCounterReward() { return 0; }
    std::string getName() { return name_; }
    virtual relic* clone() const = 0;
    virtual std::unique_ptr<relic> cloneU() const = 0;
    virtual int getRelicIDNum() const = 0;
    relicRarity getRarity() { return rarity_; }
    static std::vector<int> IDsForNloth;
};
class bossRelic
{
protected:
    int A1_{ 0 };
    int A2_{ 0 };
    bool NRG_{ 0 };
    bool UNQ_{ 0 };
    std::function<bool()> UNQcheck;
public:
    bossRelic() {}
    bossRelic(int A1, int A2, bool nrg, bool unq, std::function<bool()> unqc = nullptr) : A1_(A1), A2_(A2), NRG_(nrg), UNQ_(unq), UNQcheck(unqc) {}
    int getAct1Rank() const { return A1_; }
    int getAct2Rank() const { return A1_; }
    bool getEnergyRelic() const { return NRG_; }
    bool getUnique() const { return UNQ_; }
    bool uniqueCheck() { return UNQcheck(); }
};
std::deque<std::unique_ptr<relic>>& shuffleRelics(std::deque<std::unique_ptr<relic>>& deck, int i);
std::deque<std::unique_ptr<potion>>& shufflePotions(std::deque<std::unique_ptr<potion>>& deck, int i);

extern bool ai;
extern bool fullRuns;

// templated stuff

template<typename T>
void holdsObservers::observeEvent(focusedEvent* fe, T* obs)
{
    auto obs_ = checkForObserving(fe, obs);
    if (obs_) { removeObserver(obs); }
    observers_.insert(obs);
    fe->addObserver(obs);
}
template<typename T>
void holdsObservers::observeEvent(focusedEvent* fe, T* obs, bool noReplace)
{
    noReplace;
    observers_.insert(obs);
    fe->addObserver(obs);
}
template<typename T>
observer* holdsObservers::checkForObserving(focusedEvent* fe, T* obs)
{
    fe;
    auto obs_ = observers_.find(obs);
    if (obs_ != observers_.end() && typeid(obs) == typeid(*obs_))
    {
        return *obs_;
    }
    return nullptr;
}
template<typename T>
void holdsObservers::removeObserver()
{
    for (auto itr{ observers_.begin() }; itr != observers_.end(); ++itr)
    {
        if ((*itr)->getActive())
        {
            T* t = dynamic_cast<T*>(*itr);
            if (t != nullptr)
            {
                auto temp = *itr;
                temp->getObserving()->removeObserver(temp);
                observers_.erase(itr);
                delete temp;
                break;
            }
        }
    }
}
template<typename T>
void holdsObservers::removeObserverIfFE(focusedEvent* fe)
{
    for (auto itr{ observers_.begin() }; itr != observers_.end(); ++itr)
    {
        if ((*itr)->getActive())
        {
            T* t = dynamic_cast<T*>(*itr);
            if (t != nullptr && t->getObserving() == fe)
            {
                auto temp = *itr;
                temp->getObserving()->removeObserver(temp);
                observers_.erase(itr);
                delete temp;
                break;
            }
        }
    }
}

class SmokeBombError : public std::exception
{
public:
    SmokeBombError(const std::string& message) : message_(message) {}
    const char* what() const noexcept override { return message_.c_str(); }
private:
    std::string message_;
};
class PlayerDiedError : public std::exception
{
public:
    PlayerDiedError(const std::string& message) : message_(message) {}
    const char* what() const noexcept override { return message_.c_str(); }
private:
    std::string message_;
};

int factorial(const int& x);
int NchooseR(const int& n, const int& r);
bool recIter(int s, int c, int level, int lowerbound, int indx, int& start, std::vector<int>& res);
std::vector<int> baseIter(int s, int c, int indx);
void rngRelicAddID(const std::vector<int>&, std::vector<int>&, int);
void rngCardAddID(std::vector<int>&, int);
void printCardDeck(cardDeckCH& d);
void playCard(cardHolder& _card);
void playCardDup(cardHolder& _card);
void playTopXCards(int x, bool exh = true);
nonpc& askForEnemy(const int num_targets);
nonpc& singleTarget();
nonpc& randomTarget();
cardHolder& askForCard(const int num_targets, std::vector<cardType> cts = {});
cardHolder& askForCard(const int num_targets, cardDeckCH& d);
cardHolder& singleTargetCard(std::vector<cardType> cts = {});
cardHolder& randomTargetCard();
cardHolder& singleTargetCard(cardDeckCH& d);
std::set<int> getIndicesForCardBoolFlip(const int& lowerIndex, const int& upperIndex, int maxChoose = 0);
bool checkInHand(cardHolder& ch);
void forcedAIActions(int& _action, float& _reward);

struct macroStateInfo
{
    std::vector<int> deckCountPerCard; // 
    std::vector<int> playerVars; // 

    std::vector<int> relicPresent; // 
    std::vector<int> relicCounters; // 
    std::vector<int> potCounts; // 

    std::vector<int> gmVars; // 

    std::vector<int> oneHotFloor;
    std::vector<int> openingChoice;
    // map rep
    std::vector<int> nextFloor;
    std::vector<int> secondFloor;
    std::vector<int> thirdFloor;

    std::vector<int> mapRepVars;

    macroStateInfo() {}
    macroStateInfo(const macroStateInfo&);
    macroStateInfo& operator=(const macroStateInfo&);
    void createAllVars();
    void createPredVars();
    std::vector<int> getNormalInputs() const;
    std::vector<int> getPredInputs() const;
    void copyNonMapRep(const macroStateInfo&);
    void mapRepZeroes();
};
struct stateInfo
{
    std::vector<int> handCardIDs; // = 10*card IDs -> 10*embedding size input vars
    std::vector<int> handCardEnergy; // = 10*card IDs -> 10*embedding size input vars
    std::vector<int> handCardUpgraded; // = 10*card IDs -> 10*embedding size input vars
    std::vector<int> drawCountPerCard; // indexed by ID = 129 ints
    std::vector<int> discardCountPerCard; // indexed by ID = 129 ints
    std::vector<int> exhaustCountPerCard; // indexed by ID = 129 ints
    std::vector<bool> masksForHandCards; // 10 bools, for each hand card castable - not an input

    std::vector<int> playerVars; // 33 : strength, vuln, DarkEmb, health etc.

    std::vector<int> relicPresent; // = ~134 (+24)
    std::vector<int> relicCounters; // not 100% on this = ~134 (+24)

    std::vector<int> potionIDs; // 33 potions ->34len

    std::vector<int> enemyAttackID; // need to id enemy-attack pairs, use 4 enemy slots - embedded
    std::vector<int> pastEnemyAttackIDs; // need past two enemy-attacks for each so 8 more slots - embedded
    std::vector<int> enemyVars; // need ~30 for each enemy slot must include weird alt buffs e.g. boss stuff
    std::vector<int> incomingAttackDamage; // need damage and times for each slot = 8 - should help smooth learning seems like best alternative to giving exact intent to model, let learn from enemy-attack

    std::vector<int> dormantEnemies; // dormant

    stateInfo() {}
    stateInfo(bool _) { createAllVars(); _; }
    void createAllVars(bool dataAugment = true);
    void createPart1(bool dataAugment);
    void createPart2(bool dataAugment);
    const std::vector<int>& getEmbeddingInputsCA() const;
    std::vector<int> getEmbeddingInputsEN() const;
    const std::vector<int>& getEmbeddingInputsPO() const;
    std::vector<int> getNormalInputs() const;
    const std::vector<bool>& getMasks() const;
    void reshuffleDrawAndHand();
};
struct statistics
{
    deckGen dg;
    std::vector<int> deckIDs;
    std::vector<int> relicIDs;
    std::vector<int> potionIDs;
    int deckSize;
    int HP;

    statistics(deckGen d, std::vector<int> di, std::vector<int> ri, std::vector<int> pi, int ds, int h) :
        dg(d), deckIDs(di), relicIDs(ri), potionIDs(pi), deckSize(ds), HP(h) {}
};

typedef std::map<std::string, std::unique_ptr<card>> C_DATABASE;

struct shopStuff
{
    relic* r1{nullptr};
    relic* r2{nullptr};

    potion* p1{nullptr};
    potion* p2{nullptr};

    C_DATABASE::iterator c1;
    C_DATABASE::iterator c2;
    
    C_DATABASE::iterator cc1;
    C_DATABASE::iterator cc2;

    bool rm{false};

    float expval;
    shopStuff();
    bool isSingle() const;
};

#endif