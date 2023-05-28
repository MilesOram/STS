#ifndef GAMESTATE_H
#define GAMESTATE_H
#include "top.h"

// node class for map generation, can link to up to three previous and three next
class node
{
public:
    char name{ ' ' };
    int floorIndex{ 0 };
    int widthIndex{ 0 };
    std::vector<node*> nextNodes;
    std::vector<node*> prevNodes;

    //node() {}
    node(int f, int w) : floorIndex(f), widthIndex(w) {}
    node(int f, int w, char n) : name(n), floorIndex(f), widthIndex(w) {}
    void addNextNode(node* n) { if (find(nextNodes.begin(), nextNodes.end(), n) == nextNodes.end()) nextNodes.push_back(n); }
    void addPrevNode(node* n) { if (find(prevNodes.begin(), prevNodes.end(), n) == prevNodes.end()) prevNodes.push_back(n); }
    bool checkN(node* n) { return find(nextNodes.begin(), nextNodes.end(), n) != nextNodes.end(); }
    bool checkP(node* n) { return find(prevNodes.begin(), prevNodes.end(), n) != prevNodes.end(); }
    bool setFirstType(int depth);
    static bool checkForC(char c, std::vector<node*> vn);
    static bool checkForRepeat(char c, std::vector<node*> vn);
};
// map generation class, comprises the nodes of the map and randomly generates a map per the constraints
class gen
{
private:
    node* start{ nullptr };
    std::vector<std::vector<node*>> nodeMap;
    node* end{ nullptr };
    std::map<int, std::vector<std::pair<int, int>>> lastUsedWidthIndexOfFloor;
public:
    gen();
    ~gen();
    void showMap(int f = 0, int w = 0, int act_ = 1, std::string bossname = "    ");
    int checkGoodForCross(int fl, int wi);
    node* getNode(int f = 0, int w = 0);
    // returns the next node
    node* giveNextRNGNode(node* current);
    bool fillMap();
    std::vector<int> makeStartIndices();
    bool checkValid();
    bool makeValid();
    std::map<int, int> discreteMapRep(node*, const macroStateInfo&, std::vector<macroStateInfo>&, bool = false);
    void recursivePathing(std::vector<std::vector<char>>&, std::vector<std::vector<int>>&, node*, int);
};
// singleton class, manages databases and game flow, various game functions
class gamestate
{
    friend events;
    friend gen;
    friend macroStateInfo;
private:
    C_DATABASE card_database;
    C_DATABASE card_database_spcl;
    C_DATABASE card_database_clr;
    C_DATABASE card_database_curse;
    C_DATABASE card_database_status;
    std::map<std::string, std::unique_ptr<nonpc>> enemy_database;
    std::map<std::string, std::unique_ptr<relic>> relic_database;
    std::map<std::string, std::unique_ptr<potion>> potion_database;
    std::deque<std::unique_ptr<relic>> commonRelicdeck;
    std::deque<std::unique_ptr<relic>> uncommonRelicdeck;
    std::deque<std::unique_ptr<relic>> rareRelicdeck;
    std::deque<std::unique_ptr<relic>> bossRelicdeck;
    std::deque<std::unique_ptr<relic>> shopRelicdeck;
    std::deque<std::unique_ptr<relic>> eventRelicdeck;
    std::deque<std::unique_ptr<potion>> commonPotiondeck;
    std::deque<std::unique_ptr<potion>> uncommonPotiondeck;
    std::deque<std::unique_ptr<potion>> rarePotiondeck;
    std::deque<std::unique_ptr<nonpc>> enemies_;
    std::vector<std::vector<std::unique_ptr<nonpc>>> dormant_enemies_;
    std::deque<std::vector<Intents>> intents_;
    easyHallway* events_eh{ nullptr };
    hardHallway* events_hh{ nullptr };
    elites* events_el{ nullptr };
    bosses* events_bo{ nullptr };
    campfire* events_ca{ nullptr };
    qSpecial* events_qs{ nullptr };

    std::vector<int> openingChoice;
    bool randomMacroThisFloor{ false };
    pythonFunctions* PY_FUNC;

    std::vector<questionMark*> questionEvents;
    int playerEnergy_{ 0 };
    int xCost_{ 0 };
    int doubleAttackMod{ 0 };
    int actNumber{ 1 };

    std::vector<focusedEvent*> persistentFE;
    std::vector<focusedEvent*> temporaryFE;

    int shrineChance{ 25 };
    int enemyChance{ 10 };
    int chestChance{ 2 };
    int shopChance{ 3 };
    int upgraded_pc{ 0 };
    int common_pc{ 60 };
    int uncommon_pc{ 37 };
    int rare_pc{ 3 };
    int rare_offset{ -5 };
    int common_pc_elite{ 50 };
    int uncommon_pc_elite{ 40 };
    int rare_pc_elite{ 10 };
    int common_pc_potion{ 65 };
    int uncommon_pc_potion{ 25 };
    int rare_pc_potion{ 10 };
    int current_potion_pc{ 40 };
    int easyDone{ 0 };
    // no bash defend strike
    std::vector<std::string> commonCards{
        "Armaments_ID","Anger_ID","Bodyslam_ID","Clash_ID","Cleave_ID","Clothesline_ID","Flex_ID","Headbutt_ID","HeavyBlade_ID",
        "Havoc_ID","Ironwave_ID","PerfectedStrike_ID","PommelStrike_ID","ShrugItOff_ID","SwordBoomerang_ID",
        "Thunderclap_ID","TrueGrit_ID","TwinStrike_ID","WarCry_ID","WildStrike_ID"
    };
    std::vector<std::string> uncommonCards{
        "BattleTrance_ID","BloodForBlood_ID","Bloodletting_ID","BurningPact_ID","Carnage_ID","Combust_ID","DarkEmbrace_ID",
        "Disarm_ID","Dropkick_ID","DualWield_ID","Entrench_ID","Evolve_ID","FeelNoPain_ID","FireBreathing_ID","FlameBarrier_ID",
        "GhostlyArmor_ID","Hemokinesis_ID","InfernalBlade_ID","Inflame_ID","Intimidate_ID","Metallicize_ID","PowerThrough_ID","Pummel_ID","Rage_ID",
        "Rampage_ID","RecklessCharge_ID","Rupture_ID","SearingBlow_ID","SecondWind_ID","SeeingRed_ID","Sentinel_ID","SeverSoul_ID",
        "Shockwave_ID","SpotWeakness_ID","Uppercut_ID","Whirlwind_ID"
    };
    // no feed or reaper
    std::vector<std::string> rareCards{
        "Barricade_ID","Berserk_ID","Bludgeon_ID","Brutality_ID","Corruption_ID","DemonForm_ID","DoubleTap_ID","Exhume_ID",
        "FiendFire_ID","Immolate_ID","Impervious_ID","Juggernaut_ID","LimitBreak_ID","Offering_ID"
    };
    std::vector<std::string> curseCards{
        "Regret_ID","Injury_ID","Pain_ID","Doubt_ID","Parasite_ID","Clumsy_ID","Decay_ID","Normality_ID","Writhe_ID","Shame_ID"
    };
    int mapPathIndex{ 0 };
    events* nextEvent{ nullptr };
    bool firstMapItr{ true };
    gen* mapGen{ nullptr };
    macroStateInfo msi;
    std::vector<macroStateInfo> storedMSIs;
    std::vector<macroStateInfo> paths_CACHED;
    int currentFloorIndex{ 0 };
    int currentWidthIndex{ 0 };
    int bossIndexThisAct{ 0 };
    std::vector<char> mapPath{ 'N','e','e','?','?','c','E','s','e','C','c','?','e','E','?','s','c','B',
                                   'e','e','?','?','c','E','s','e','C','c','?','e','E','?','s','c','B',
                                   'e','e','?','?','c','E','s','e','C','c','?','e','E','?','s','c','B' };
public:
    gamestate();
    ~gamestate() { resetFocusedEvents(); clearEvents(); delete mapGen; }
    int cardCount{ 0 }, attackCount{ 0 }, skillCount{ 0 }, powerCount{ 0 }, turnCount{ 0 }, fightIndexPRED{ 0 };

    // used by savestates to reload to an earlier point
    void loadEnergy(const int& e) { playerEnergy_ = e; }
    void loadXCost(const int& x) { xCost_ = x; }
    void loadDAM(const int& d) { doubleAttackMod = d; }
    void loadActNumber(const int& d) { actNumber = d; }
    void loadCounts(const int& c, const int& a, const int& s, const int& p, const int& t) { cardCount = c; attackCount = a; skillCount = s; powerCount = p; turnCount = t; }
    void loadEnemies(std::vector<nonpc*>);
    void loadDormantEnemies(std::vector<std::vector<nonpc*>>);
    void loadIntents(const std::deque<std::vector<Intents>>& ints_) { intents_ = ints_; }

    // map and event related
    char getMapPathEvent() const { return mapPath[mapPathIndex]; }
    std::vector<char> getMP() const { return mapPath; }
    void setMP(char c) { mapPath[mapPathIndex] = c; }
    events* makeNextEvent(char c);
    events* getEvent() const { return nextEvent; }
    campfire* getCampfire() { return events_ca; }
    void advanceEvent(); // not used
    void advanceRealEvent();
    void playNextEvent();
    node* getCurrentNode();
    questionMark* chooseNextQuestionEvent();
    void cycleAct();
    int getActNumber() const { return actNumber; }
    int getEasyDone() const { return easyDone; }
    
    int getDAM() const { return doubleAttackMod; }
    void incremenetDAM() { doubleAttackMod = std::clamp(doubleAttackMod + 1, 0, 4); }
    void decrementDAM() { doubleAttackMod = std::clamp(doubleAttackMod - 1, 0, 4); }
    
    // database-related
    void makeDatabase();
    void makeCardDatabase();
    void makeEnemyDatabase();
    void makeRelicDatabase();
    void makePotionDatabase();
    C_DATABASE& getDatabase() { return card_database; }
    C_DATABASE::iterator getDatabaseEnd() { return card_database.end(); }
    C_DATABASE::iterator getColourlessDatabaseEnd() { return card_database_clr.end(); }
    void makeEvents();
    void makeEnemyEvents();
    void makeEnemyEventsA2();
    void makeEnemyEventsA3();
    void makeEnemyEventsA4();
    void makeQuestionEvents();
    void makeQuestionEventsA2();
    void makeQuestionEventsA3();
    void makeRelicStacks();
    void makePotionStacks();

    // manage game flow
    void StartGame();
    void StartGame(int);
    void StartGameV();
    void outerTurnLoop();
    void startEnemyTurn();
    void endFight();
    void AIFightEnd();
    void Act1to2();
    void Act2to3();
    void Act3to4();
    void startAct4();
    void ResetRun();

    // map gen
    void showPath();
    void showBasicPath();
    void generateMapPath();
    void genRealMap();
    int getFloorIndex() const;
    char calcOdds(std::vector<char> events_, std::vector<int> percentages, std::vector<int> maxInARow, int);
    
    // retrieve from databases
    std::unique_ptr<card> getCardByID(const std::string&);
    std::unique_ptr<card> getSpecialCardByID(const std::string&);
    std::unique_ptr<card> getColourlessCardByID(const std::string&);
    std::unique_ptr<card> getCurseCardByID(const std::string&);
    std::unique_ptr<card> getStatusCardByID(const std::string&);
    card* getCardPByID(const std::string&);
    std::unique_ptr<nonpc> getEnemyByID(const std::string&);
    relic* getRelicByID(const std::string&);
    potion* getPotionByID(const std::string&);
    
    // unique fights/encounters and generic fight start
    void startMushroomFight();
    void startDeadAdventurerFight();
    void startBanditFight();
    void startOrbFight();
    void startColosseumFight();
    void startColosseumFight2();
    void startAct1BossFight();
    void startRandomFight(events* e);
    void startRandomFightV(events* e);
    void startRandomBossFight();
    void startRandomBossFightV();
    void teleportToBoss();
    
    // enemy management
    void addDormantEnemy(const nonpc& original, int turns);
    void addEnemiesByID(std::string, int);
    void addEnemiesByID(std::string, int, bool);
    void addEnemyFrontByIDMidFight(std::string);
    void addEasyEnemies(int, int);
    void addHardEnemies(int, int);
    void addEliteEnemies(int, int);
    void addBossEnemies(int, int);
    
    bool checkEnemyDeath();
    bool checkAllEnemyDeath();
    void handleEnemyDeath();
    void reAddDormant();
    void removeFromCombat(nonpc*);
    
    // energy-related
    int getPlayerEnergy() { return playerEnergy_; }
    void setPlayerEnergySOT(int i);
    void decreasePlayerEnergy(int i);
    int getXCost(bool reset_ = true) { int temp = xCost_; if(reset_) xCost_ = 0; if (chemicalXCheck) { temp += 2; } return temp; }
    void increasePlayerEnergy(int i) { playerEnergy_ += i; if (playerEnergy_ < 0) throw std::runtime_error("negative energy"); }
    int getTotalIncomingDamage();
    
    inline std::deque<std::unique_ptr<nonpc>>& getAllEnemies() { return enemies_; }
    std::vector<std::vector<std::unique_ptr<nonpc>>>& getDormantEnemies() { return dormant_enemies_; }

    // handle each enemies intent
    const std::vector<Intents>& getIntents() const { return intents_[0]; }
    const std::deque<std::vector<Intents>>& getAllIntents() const { return intents_; }
    const std::vector<Intents>& getIntents(int num) const
    {
        if (num >= intents_.size() ) throw std::runtime_error("intents index error");
        return intents_[num];
    }
    void reshuffleIntents();
    void adjustAllIntentsAttacks();
    void setIntents(std::vector<Intents> vi, int i) { intents_[i] = vi; }

    // miscellaneous
    void resetVisuals();
    void setBossIndex();
    int getBossIndex() const { return bossIndexThisAct; };
    std::string getHighestHealthEnemyName();
    void actionChoicesForSim(combatChoices actionChoices, int action);
    void chooseFromThree(std::vector<C_DATABASE::iterator> options, bool skipAllowed = true, bool zeroCost = true);
    void prelude();
    deckGen makeDGEnum();
    bool checkIfBanned();
    void addCheatCard();

    // potion-related
    std::vector<potion*> retrieveXRandomPotions(int x);
    void XPotionSelection(std::vector<potion*>);
    void potionReward();
    void setPotionChance(int x) { current_potion_pc = x; }
    int getPotionChance() const { return current_potion_pc; }
    
    // card rewards after each fight
    void rareCardReward();
    void hallwayReward();
    void hallwayReward(bool);
    void colourlessReward(cardRarity = cardRarity::Other);
    void eliteReward();
    void eliteReward(bool);
    void eliteRelicReward();
    void libraryCardOpts();
    
    // upgrades, removes transforms of cards from deck
    void transformCard(cardDeck::iterator chosenCard, bool print = false);
    void transformCurse(cardDeck::iterator chosenCard, bool print = false);
    void transformColourless(cardDeck::iterator chosenCard, bool print = false);
    cardDeck::iterator chooseCardFromDeck(bool bottledAllowed = false);
    cardDeck::iterator chooseCardFromDeck(cardType, bool bottledAllowed = false);
    int chooseCardFromDraw(cardType);
    void transformCardFromDeck();
    void transformTwoFromDeck();
    void removeCardFromDeck();
    void upgradeCardFromDeck(bool print = false);
    void upgradeRandomFromDeck(cardType ct);
    void upgradeRandomFromDeck();
    
    // selecting cards to be offered for rewards
    cardHolder getRandomAttack();
    cardHolder getRandomPower();
    cardHolder getRandomCard();
    card* getCurse();
    card* getCardOfRarity(cardRarity);
    std::vector<std::string> getCommonCards() const;
    std::vector<std::string> getUncommonCards() const;
    std::vector<std::string> getRareCards() const;
    std::vector<C_DATABASE::iterator> getXRandomCardsOfType(int x, cardType ct = Status);
    std::vector<C_DATABASE::iterator> getXRandomColourlessCards(int x, cardRarity cr = cardRarity::Other);
    C_DATABASE::iterator getCardReward(std::vector<C_DATABASE::iterator> notIncluding);
    C_DATABASE::iterator getCardReward(std::vector<C_DATABASE::iterator> notIncluding, cardType ct);
    C_DATABASE::iterator getColourlessCardReward(std::vector<C_DATABASE::iterator> notIncluding, cardRarity = cardRarity::Other);
    
    //  macro ai choices 
    void makeCardChoices(aiEventCardChoice, bool = false);
    void makePathChoiceFromNeutral(bool = true);
    void makeShopChoices(std::vector<std::pair<relic*,int>>,std::vector<std::pair<potion*,int>>,std::vector<std::pair<C_DATABASE::iterator,int>>,int,bool);
    void makeCampfireChoices(bool rest_,bool upgrade_,bool girya_,bool pipe_,bool shovel_);
    void makeBossRelicChoices(std::vector<relic*>);
    void makeNeowChoices(int,int,int,int);
    void addToRelicPairChoice(relic*, macroStateInfo&);
    void addToRelicPairChoice(int, macroStateInfo&);
    int getIndexForAIAdd(C_DATABASE::iterator);
    
    // temporary solution for helping macro model off the ground
    int tierListAddChoice();
    int tierListRemoveChoice();
    int tierListUpgradeChoice();

    // learn and evaluate outcome of first floor reward choice
    std::vector<int> getOpeningChoice() const { return openingChoice; }
    void setOpeningChoice(const std::vector<int> & oc) { openingChoice = oc; }
    // avoid recalculation of all path permutations
    const std::vector<macroStateInfo>& getCachedPaths() const { return paths_CACHED; }
    void setCachedPaths(const std::vector<macroStateInfo> & cp) { paths_CACHED = cp; }
    void clearCachedPaths() { paths_CACHED.clear(); }
    bool inline getRandomMacroThisFloor() const { return randomMacroThisFloor; }
    void setRandomMacroThisFloor(bool b) { randomMacroThisFloor = b; }

    // python function holder access, and msi management
    void holdPyFunc(pythonFunctions& _pf) { PY_FUNC = &_pf; }
    pythonFunctions& getPyFunc() { if (PY_FUNC == nullptr) throw std::runtime_error("no_py"); return *PY_FUNC; }
    std::vector<macroStateInfo>& MSIsForUpdate() { return storedMSIs; }
    const macroStateInfo& mostRecentMSI() const { if (storedMSIs.empty()) throw std::runtime_error("no msi"); return storedMSIs.back(); }
    void zeroMSI() { msi.mapRepZeroes(); openingChoice = std::vector<int>(24); }
    void thinOutMSIs();

    // sim question mark events for best desired outcome
    void qEventMatchAndKeep(std::vector<card*>& known_, const std::vector<std::string>& letters_, int first_ = -1);
    void qEventOminousForge();
    void qEventTheCleric();
    void qEventWingStatue();
    void qEventApparitions();
    void qEventFalling();
    void qEvent1();
    void qEvent2();
    void qEvent3();

    // get events for triggers, if not present, makes new FE
    focusedEvent* getCardDrawn();
    focusedEvent* getCardPlayed();
    focusedEvent* getAttackPlayed();
    focusedEvent* getSkillPlayed();
    focusedEvent* getPowerPlayed();
    focusedEvent* getCardExhausted();

    focusedEvent* getPlayerTurnStart();
    focusedEvent* getPlayerTurnEnd();
    focusedEvent* getEnemyTurnStart();
    focusedEvent* getEnemyTurnEnd();

    focusedEvent* getPlayerAboutToLoseHealth();
    focusedEvent* getPlayerHealthGain();
    focusedEvent* getPlayerHealthLoss();
    focusedEvent* getPlayerTakingAttackDamage();
    focusedEvent* getEnemyHealthLoss();
    focusedEvent* getEnemyTakingAttackDamage();

    focusedEvent* getAlterBaseADFromPlayer();
    focusedEvent* getAlterBaseADFromEnemy();
    focusedEvent* getAlterAdjustedADFromPlayer();
    focusedEvent* getAlterAdjustedADFromEnemy();

    focusedEvent* getGainBlock();
    focusedEvent* getApplyingVulnerable();
    focusedEvent* getApplyingFrail();
    focusedEvent* getApplyingWeak();
    focusedEvent* getApplyingDebuff();

    focusedEvent* getBlockBroken();
    focusedEvent* getSOTBlock();
    focusedEvent* getShuffledDraw();
    focusedEvent* getFightEnd();
    focusedEvent* getEnemyDied();
    focusedEvent* getEnemyFatal();

    focusedEvent* getNewFloorEntered();
    focusedEvent* getCampfireEntered();
    focusedEvent* getCampfireRest();
    focusedEvent* getShopEntered();
    focusedEvent* getQuesitonEntered();
    focusedEvent* getChestOpened();
    focusedEvent* getCardAdded();
    focusedEvent* getGoldSpent();
    focusedEvent* getGoldGained();
    focusedEvent* getPotionUsed();
    focusedEvent* getPlayerDied();

    // calls relevant FE and leads to triggering of effects
    void callCardDrawn(cardHolder& ch);
    void callCardPlayed(cardHolder& ch);
    void callCardExhausted(cardHolder& ch);

    void callPlayerTurnStart();
    void callPlayerTurnEnd();
    void callEnemyTurnStart();
    void callEnemyTurnEnd();

    void callPlayerAboutToLoseHealth();
    void callPlayerHealthLoss();
    void callPlayerHealthGain();
    void callPlayerTakingAttackDamage();
    void callEnemyHealthLoss();
    void callEnemyTakingAttackDamage();

    void callPlayerTakingAttackDamage(character& initiator);
    void callEnemyTakingAttackDamage(character& initiator);

    void callAlterBaseADFromPlayer();
    void callAlterBaseADFromEnemy();
    void callAlterAdjustedADFromPlayer();
    void callAlterAdjustedADFromEnemy();

    void callGainBlock();
    void callApplyingVulnerable(character& enemy);
    void callApplyingFrail();
    void callApplyingWeak(character& enemy);
    void callApplyingDebuff(character& enemy);

    void callBlockBroken();
    void callSOTBlock();
    void callShuffledDraw();
    void callFightEnd();
    void callEnemyDied(character& c);
    void callEnemyFatal(character& c);

    void callNewFloorEntered();
    void callCampfireEntered();
    void callCampfireRest();
    void callShopEntered();
    void callQuesitonEntered();
    void callChestOpened();
    void callCardAdded(cardHolder& ch);
    void callGoldSpent();
    void callGoldGained();
    void callPotionUsed();
    void callPlayerDied();

    void resetFocusedEvents();
    void clearEvents();

    // take from decks
    relic* retrieveCommonRelic();
    relic* retrieveUncommonRelic();
    relic* retrieveRareRelic();
    relic* retrieveBossRelic();
    relic* retrieveShopRelic();

    relic* retrieveRandomRelic();

    // clone from database
    relic* getCommonRelic();
    relic* getUncommonRelic();
    relic* getRareRelic();
    relic* getBossRelic();
    relic* getBossRelic(relic* other);
    relic* getShopRelic();
    relic* getEventRelic();

    potion* retrieveCommonPotion();
    potion* retrieveUncommonPotion();
    potion* retrieveRarePotion();

    // relic checks for relics that change core functionality when held
    static bool ectoplasmCheck;
    static bool coffeeDripperCheck;
    static bool fusionHammerCheck;
    static bool sneckoEyeCheck;
    static bool markOfPainCheck;
    static bool runicPyramidCheck;
    static bool sozuCheck;
    static bool peacePipeCheck;
    static bool giryaCheck;
    static bool shovelCheck;
    static bool iceCreamCheck;
    static bool prayerWheelCheck;
    static bool frozenEyeCheck;
    static bool goldenIdolCheck; 
    static bool nlothsGiftCheck;
    static bool strikeDummyCheck;
    static bool juzuBraceletCheck;
    static bool tinyChestCheck;
    static bool tungstenCheck;
    static bool chemicalXCheck;
    static bool strangeSpoonCheck;
    static bool strangeSpoonLE;
    static bool markOfTheBloomCheck;
    static bool sacredBarkCheck;
    static bool runicDomeCheck;
    static bool singingBowlCheck;
    static int omamoriCheck;
    static int wingBootsCheck;

    // help with more complex game mechanics
    static bool clockEnd;
    static bool dontDupe;
    static bool denyCardPlay;
    static bool nextCardSameTarget;
    static int nextCardsNoTarget;
    static int lastAttackedSCRNIndex;

    // vars and containers that need to be readily accessed and changed 
    static int cardRewardNumber;
    static std::deque<int> aiSimActionBuffer;
    static std::deque<std::string> aiEventActionBuffer;
    static std::deque<std::string> aiSimPotionBuffer;
    static std::vector<C_DATABASE::iterator> aiCardBuffer;
    static std::vector<C_DATABASE::iterator> aiCardChoices;
    static std::vector<std::vector<int>> orderedChoiceTree;

    static bool randomAIChoice;
};
namespace adjusted { int calcEnemyDamage(const nonpc& enemy_, const int& amount_); }
void setAOstored(int);

extern gamestate gm;

#endif

