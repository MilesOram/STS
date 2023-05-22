#ifndef ENEMIES_H
#define ENEMIES_H
#include "top.h"
#include "character.h"
#include "effects_triggers.h"

// hallway
class jawWorm : public nonpc
{
private:
    bool firstAttackDone{ false };
    // next attack function is stored in declared intent, can then be called as necessary
    void (jawWorm::* declaredIntent)();
public:
    jawWorm(bool fAD = false, void (jawWorm::* dI)() = nullptr) : firstAttackDone(fAD), declaredIntent(dI), nonpc(42, "Jaw Worm") { enemyIDNumber = 0; allAttacks = { "chomp","thrash","bellow" }; }
    ~jawWorm() {}
    // intents
    void chomp();
    void thrash();
    void bellow();

    void chooseAndDeclareIntent();
    void EnactIntent();
    void startOfFight();
    std::vector<int> giveIntent();
    jawWorm* saveClone() { return saveCloneFromOrig<jawWorm>(this, new jawWorm(firstAttackDone, declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<jawWorm>(); }
};
class cultist : public nonpc
{
private:
    bool firstAttackDone{ false };
    int ritualNumber{ 3 };
    void (cultist::* declaredIntent)();
public:
    cultist(bool fAD = false, void (cultist::* dI)() = nullptr) : firstAttackDone(fAD), declaredIntent(dI), nonpc(45, "Cultist") { enemyIDNumber = 1; allAttacks = { "darkStrike","incantation" }; }
    ~cultist() {}
    void darkStrike();
    void incantation();
    void chooseAndDeclareIntent();
    void EnactIntent();
    cultist* saveClone() { return saveCloneFromOrig<cultist>(this, new cultist(firstAttackDone, declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<cultist>(); }
};
class greenLouse : public nonpc
{
private:
    bool firstAttackDone{ false };
    int attackNumber{ 0 };
    int shieldNumber{ 0 };
    void (greenLouse::* declaredIntent)();
public:
    greenLouse(int sN = 0, bool fAD = false, void (greenLouse::* dI)() = nullptr, int aN = 0) : attackNumber(aN), shieldNumber(sN), firstAttackDone(fAD), declaredIntent(dI), nonpc(14, "Green Louse") { enemyIDNumber = 2; allAttacks = { "bite","spitWeb" }; }
    ~greenLouse() {}
    void bite();
    void spitWeb();
    void chooseAndDeclareIntent();
    bool takeDamage(int amount, character& initiator);
    void EnactIntent();
    greenLouse* saveClone() { return saveCloneFromOrig<greenLouse>(this, new greenLouse(shieldNumber, firstAttackDone, declaredIntent, attackNumber)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<greenLouse>(); }
};
class redLouse : public nonpc
{
private:
    bool firstAttackDone{ false };
    int attackNumber{ 0 };
    int shieldNumber{ 0 };
    void (redLouse::* declaredIntent)();
public:
    redLouse(int sN = 0, bool fAD = false, void (redLouse::* dI)() = nullptr, int aN = 0) : attackNumber(aN), shieldNumber(sN), firstAttackDone(fAD), declaredIntent(dI), nonpc(13, "Red Louse") { enemyIDNumber = 3; allAttacks = { "bite","grow" }; }
    ~redLouse() {}
    void bite();
    void grow();
    void chooseAndDeclareIntent();
    bool takeDamage(int amount, character& initiator);
    void EnactIntent();
    redLouse* saveClone() { return saveCloneFromOrig<redLouse>(this, new redLouse(shieldNumber, firstAttackDone, declaredIntent, attackNumber)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<redLouse>(); }
};

class acidSlimeM : public nonpc
{
private:
    bool firstAttackDone{ false };
    int attackNumber{ 10 };
    void (acidSlimeM::* declaredIntent)();
public:
    acidSlimeM(bool fAD = false, void (acidSlimeM::* dI)() = nullptr) : firstAttackDone(fAD), declaredIntent(dI), nonpc(28, 32, "Medium Acid Slime") { enemyIDNumber = 4; allAttacks = { "tackle","lick","corrosiveSpit" }; }
    ~acidSlimeM() {}
    void corrosiveSpit();
    void lick();
    void tackle();
    void chooseAndDeclareIntent();
    void EnactIntent();
    acidSlimeM* saveClone() { return saveCloneFromOrig<acidSlimeM>(this, new acidSlimeM(firstAttackDone, declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<acidSlimeM>(); }
};
class spikeSlimeM : public nonpc
{
private:
    bool firstAttackDone{ false };
    int attackNumber{ 8 };
    void (spikeSlimeM::* declaredIntent)();
public:
    spikeSlimeM(bool fAD = false, void (spikeSlimeM::* dI)() = nullptr) : firstAttackDone(fAD), declaredIntent(dI), nonpc(28, 32, "Medium Spike Slime") { enemyIDNumber = 5; allAttacks = { "flameTackle","lick" }; }
    ~spikeSlimeM() {}
    void lick();
    void flameTackle();
    void chooseAndDeclareIntent();
    void EnactIntent();
    spikeSlimeM* saveClone() { return saveCloneFromOrig<spikeSlimeM>(this, new spikeSlimeM(firstAttackDone, declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<spikeSlimeM>(); }
};
class acidSlimeS : public nonpc
{
private:
    bool firstAttackDone{ false };
    int attackNumber{ 3 };
    void (acidSlimeS::* declaredIntent)();
public:
    acidSlimeS(bool fAD = false, void (acidSlimeS::* dI)() = nullptr) : firstAttackDone(fAD), declaredIntent(dI), nonpc(8, 12, "Small Acid Slime") { enemyIDNumber = 6; allAttacks = { "tackle","lick" }; }
    ~acidSlimeS() {}
    void lick();
    void tackle();
    void chooseAndDeclareIntent();
    void EnactIntent();
    acidSlimeS* saveClone() { return saveCloneFromOrig<acidSlimeS>(this, new acidSlimeS(firstAttackDone, declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<acidSlimeS>(); }
};
class spikeSlimeS : public nonpc
{
private:
    int attackNumber{ 5 };
    void (spikeSlimeS::* declaredIntent)();
public:
    spikeSlimeS(void (spikeSlimeS::* dI)() = nullptr) : declaredIntent(dI), nonpc(10, 14, "Small Spike Slime") { enemyIDNumber = 7; allAttacks = { "tackle" }; }
    ~spikeSlimeS() {}
    void tackle();
    void chooseAndDeclareIntent();
    void EnactIntent();
    spikeSlimeS* saveClone() { return saveCloneFromOrig<spikeSlimeS>(this, new spikeSlimeS(declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<spikeSlimeS>(); }
};

class blueSlaver : public nonpc
{
private:
    int attackNumber{ 12 };
    void (blueSlaver::* declaredIntent)();
public:
    blueSlaver(void (blueSlaver::* dI)() = nullptr) : declaredIntent(dI), nonpc(46, 50, "Blue Slaver") { enemyIDNumber = 8; allAttacks = { "rake","stab" }; }
    ~blueSlaver() {}
    void stab();
    void rake();
    void chooseAndDeclareIntent();
    void EnactIntent();
    blueSlaver* saveClone() { return saveCloneFromOrig<blueSlaver>(this, new blueSlaver(declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<blueSlaver>(); }
};
class redSlaver : public nonpc
{
private:
    bool firstAttackDone{ false };
    bool entangled_{ false };
    int attackNumber{ 13 };
    void (redSlaver::* declaredIntent)();
public:
    redSlaver(bool en = false, bool fAD = false, void (redSlaver::* dI)() = nullptr) :entangled_(en), firstAttackDone(fAD), declaredIntent(dI), nonpc(46, 50, "Red Slaver") { enemyIDNumber = 9; allAttacks = { "stab","scrape","entangle" }; }
    ~redSlaver() {}
    void stab();
    void scrape();
    void entangle();
    void chooseAndDeclareIntent();
    void EnactIntent();
    redSlaver* saveClone() { return saveCloneFromOrig<redSlaver>(this, new redSlaver(entangled_, firstAttackDone, declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<redSlaver>(); }
};
class fungiBeast : public nonpc
{
private:
    bool firstAttackDone{ false };
    int attackNumber{ 6 };
    void (fungiBeast::* declaredIntent)();
public:
    fungiBeast(bool fAD = false, void (fungiBeast::* dI)() = nullptr) : firstAttackDone(fAD), declaredIntent(dI), nonpc(22, 28, "Fungi Beast") { enemyIDNumber = 10; allAttacks = { "bite","grow" }; }
    ~fungiBeast() {}
    void bite();
    void grow();
    void startOfFight();
    void chooseAndDeclareIntent();
    void EnactIntent();
    fungiBeast* saveClone() { return saveCloneFromOrig<fungiBeast>(this, new fungiBeast(firstAttackDone, declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<fungiBeast>(); }
};
class looter : public nonpc
{
private:
    bool firstAttackDone{ false };
    int attackNumber{ 10 };
    int thieveryNumber{ 15 };
    int goldStolen{ 0 };
    void (looter::* declaredIntent)();
public:
    looter(bool fAD = false, void (looter::* dI)() = nullptr, int gS = 0) : goldStolen(gS), firstAttackDone(fAD), declaredIntent(dI), nonpc(44, 48, "Looter") { enemyIDNumber = 11; allAttacks = { "smokeBomb","mug","lunge","escape" }; }
    ~looter() {}
    void mug();
    void lunge();
    void startOfFight();
    void smokeBomb();
    void escape();
    void chooseAndDeclareIntent();
    void EnactIntent();
    looter* saveClone() { return saveCloneFromOrig<looter>(this, new looter(firstAttackDone, declaredIntent,goldStolen)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<looter>(); }
};


// elite
class gremlinNob : public nonpc
{
private:
    bool firstAttackDone{ false };
    void (gremlinNob::* declaredIntent)();
public:
    gremlinNob(bool fAD = false, void (gremlinNob::* dI)() = nullptr) : firstAttackDone(fAD), declaredIntent(dI), nonpc(82, 86, "Gremlin Nob") { enemyIDNumber = 12; allAttacks = { "bellow","rush","skullBash" }; }
    ~gremlinNob() {}
    void bellow();
    void rush();
    void skullBash();
    void chooseAndDeclareIntent();
    void EnactIntent();
    gremlinNob* saveClone() { return saveCloneFromOrig<gremlinNob>(this, new gremlinNob(firstAttackDone, declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<gremlinNob>(); }
};
class sentry1 : public nonpc
{
private:
    bool firstAttackDone{ false };
    int attackNumber{ 9 };
    void (sentry1::* declaredIntent)();
public:
    sentry1(bool fAD = false, void (sentry1::* dI)() = nullptr) : firstAttackDone(fAD), declaredIntent(dI), nonpc(38, 42, "Sentry") { enemyIDNumber = 13; allAttacks = { "beam","bolt" }; }
    ~sentry1() {}
    void beam();
    void bolt();
    void startOfFight();
    void chooseAndDeclareIntent();
    void EnactIntent();
    sentry1* saveClone() { return saveCloneFromOrig<sentry1>(this, new sentry1(firstAttackDone, declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<sentry1>(); }
};
class sentry2 : public nonpc
{
private:
    bool firstAttackDone{ false };
    int attackNumber{ 9 };
    void (sentry2::* declaredIntent)();
public:
    sentry2(bool fAD = false, void (sentry2::* dI)() = nullptr) : firstAttackDone(fAD), declaredIntent(dI), nonpc(38, 42, "Sentry") { enemyIDNumber = 14; allAttacks = { "bolt","beam" }; }
    ~sentry2() {}
    void beam();
    void bolt();
    void startOfFight();
    void chooseAndDeclareIntent();
    void EnactIntent();
    sentry2* saveClone() { return saveCloneFromOrig<sentry2>(this, new sentry2(firstAttackDone, declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<sentry2>(); }
};
class lagavulin : public nonpc
{
private:
    bool awoken{ false };
    void (lagavulin::* declaredIntent)();
public:
    lagavulin(bool aw = false, void (lagavulin::* dI)() = nullptr) : awoken(aw), declaredIntent(dI), nonpc(109, 111, "Lagavulin") { metallicise_ = 8; addAltBuffs(Intents("MTL", 8, 1)); enemyIDNumber = 15; allAttacks = { "asleep","baseAttack","siphonSoul" }; }
    ~lagavulin() {}
    void baseAttack();
    void siphonSoul();
    void asleep();
    void wokeUp();
    void chooseAndDeclareIntent();
    void EnactIntent();
    bool takeDamage(int amount, character& initiator);
    bool takeDamageNoTrigger(int amount);
    lagavulin* saveClone() { return saveCloneFromOrig<lagavulin>(this, new lagavulin(awoken, declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<lagavulin>(); }
};

// boss
class theGuardian : public nonpc
{
private:
    int modeShift{ 30 }; //35,40
    int currentModeShift{ 30 };
    bool firstAttackDone{ false };
    bool defMode{ false };
    void (theGuardian::* declaredIntent)();
public:
    theGuardian(int mS = 30, int cMS = 30, bool dM = false, bool fAD = false, void (theGuardian::* dI)() = nullptr) :
        modeShift(mS), currentModeShift(cMS), defMode(dM), firstAttackDone(fAD), declaredIntent(dI), nonpc(240, "The Guardian") { enemyIDNumber = 16; allAttacks = { "chargingUp","fierceBash","ventSteam","whirlwind","roll","twinSlam","defensiveMode"}; }
    ~theGuardian() {}
    void offensiveMode();
    void chargingUp();
    void fierceBash();
    void ventSteam();
    void whirlwind();
    void defensiveMode();
    void roll();
    void twinSlam();
    void chooseAndDeclareIntent();
    void EnactIntent();
    void gainSharpHide();
    bool takeDamage(int amount, character& initiator);
    bool takeDamageNoTrigger(int amount);
    theGuardian* saveClone() { return saveCloneFromOrig<theGuardian>(this, new theGuardian(modeShift, currentModeShift, defMode, firstAttackDone, declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<theGuardian>(); }
};
class hexaGhost : public nonpc
{
private:
    bool firstAttackDone{ false };
    int dividerDamage{ 0 };
    int attackIndex{ 0 };
    void (hexaGhost::* declaredIntent)();
public:
    hexaGhost(int dD = 0, int aI = 0, bool fAD = false, void (hexaGhost::* dI)() = nullptr) :
        dividerDamage(dD), attackIndex(aI), firstAttackDone(fAD), declaredIntent(dI), nonpc(250, "HexaGhost") { enemyIDNumber = 17; allAttacks = { "sear","tackle","activate","divider","inflame","inferno" }; }
    ~hexaGhost() {}
    void activate();
    void divider();
    void sear();
    void tackle();
    void inflame();
    void inferno();
    void chooseAndDeclareIntent();
    void EnactIntent();
    void calcDivider();
    int getIntentID();
    int getIntent2ID();
    int getIntent3ID();
    hexaGhost* saveClone() { return saveCloneFromOrig<hexaGhost>(this, new hexaGhost(dividerDamage, attackIndex, firstAttackDone, declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<hexaGhost>(); }
};
class madGremlin : public nonpc
{
private:
    void (madGremlin::* declaredIntent)();
public:
    madGremlin(void (madGremlin::* dI)() = nullptr) : declaredIntent(dI), nonpc(20, 24, "Mad Gremlin")
    {
        enemyIDNumber = 18; allAttacks = { "scratch" };
    }
    ~madGremlin() {}
    void scratch();
    void startOfFight();
    void chooseAndDeclareIntent();
    void EnactIntent();
    bool isMinion() const;
    madGremlin* saveClone() { return saveCloneFromOrig<madGremlin>(this, new madGremlin(declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<madGremlin>(); }
};
class fatGremlin : public nonpc
{
private:
    void (fatGremlin::* declaredIntent)();
public:
    fatGremlin(void (fatGremlin::* dI)() = nullptr) : declaredIntent(dI), nonpc(13, 17, "Fat Gremlin")
    {
        enemyIDNumber = 19; allAttacks = { "smash" };
    }
    ~fatGremlin() {}
    void smash();
    void chooseAndDeclareIntent();
    void EnactIntent();
    bool isMinion() const;
    fatGremlin* saveClone() { return saveCloneFromOrig<fatGremlin>(this, new fatGremlin(declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<fatGremlin>(); }
};
class shieldGremlin : public nonpc
{
private:
    void (shieldGremlin::* declaredIntent)();
public:
    shieldGremlin(void (shieldGremlin::* dI)() = nullptr) : declaredIntent(dI), nonpc(12, 15, "Shield Gremlin")
    {
        enemyIDNumber = 20; allAttacks = { "protect","shieldBash" };
    }
    ~shieldGremlin() {}
    void protect();
    void shieldBash();
    void chooseAndDeclareIntent();
    void EnactIntent();
    bool isMinion() const;
    shieldGremlin* saveClone() { return saveCloneFromOrig<shieldGremlin>(this, new shieldGremlin(declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<shieldGremlin>(); }
};
class sneakyGremlin : public nonpc
{
private:
    void (sneakyGremlin::* declaredIntent)();
public:
    sneakyGremlin(void (sneakyGremlin::* dI)() = nullptr) : declaredIntent(dI), nonpc(10, 14, "Sneaky Gremlin")
    {
        enemyIDNumber = 21; allAttacks = { "puncture" };
    }
    ~sneakyGremlin() {}
    void puncture();
    void chooseAndDeclareIntent();
    void EnactIntent();
    bool isMinion() const;
    sneakyGremlin* saveClone() { return saveCloneFromOrig<sneakyGremlin>(this, new sneakyGremlin(declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<sneakyGremlin>(); }
};

class gremlinWizard : public nonpc
{
private:
    int chargeCount{ 2 };
    void (gremlinWizard::* declaredIntent)();
public:
    gremlinWizard(int cC = 2, void (gremlinWizard::* dI)() = nullptr) : chargeCount(cC), declaredIntent(dI), nonpc(23, 25, "Gremlin Wizard")
    {
        enemyIDNumber = 22; allAttacks = { "charging", "ultimateBlast" };
    }
    ~gremlinWizard() {}
    void charging();
    void ultimateBlast();
    void chooseAndDeclareIntent();
    void EnactIntent();
    bool isMinion() const;
    gremlinWizard* saveClone() { return saveCloneFromOrig<gremlinWizard>(this, new gremlinWizard(chargeCount, declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<gremlinWizard>(); }
};


class acidSlimeL : public nonpc
{
private:
    void (acidSlimeL::* declaredIntent)();
public:
    acidSlimeL(void (acidSlimeL::* dI)() = nullptr) : declaredIntent(dI), nonpc(65, 69, "Large Acid Slime")
    {
        enemyIDNumber = 23; allAttacks = { "tackle","lick","corrosiveSpit","split" };
    }
    ~acidSlimeL() {}
    void corrosiveSpit();
    void lick();
    void tackle();
    void split();
    void chooseAndDeclareIntent();
    void EnactIntent();
    bool takeDamage(int amount, character& initiator);
    bool takeDamageNoTrigger(int amount);
    acidSlimeL* saveClone() { return saveCloneFromOrig<acidSlimeL>(this, new acidSlimeL(declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<acidSlimeL>(); }
};
class spikeSlimeL : public nonpc
{
private:
    void (spikeSlimeL::* declaredIntent)();
public:
    spikeSlimeL(void (spikeSlimeL::* dI)() = nullptr) : declaredIntent(dI), nonpc(64, 70, "Large Spike Slime")
    {
        enemyIDNumber = 24; allAttacks = { "flameTackle","lick","split" };
    }
    ~spikeSlimeL() {}
    void lick();
    void flameTackle();
    void split();
    void chooseAndDeclareIntent();
    void EnactIntent();
    bool takeDamage(int amount, character& initiator);
    bool takeDamageNoTrigger(int amount);
    spikeSlimeL* saveClone() { return saveCloneFromOrig<spikeSlimeL>(this, new spikeSlimeL(declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<spikeSlimeL>(); }
};
class slimeBoss : public nonpc
{
private:
    void (slimeBoss::* declaredIntent)();
public:
    slimeBoss(void (slimeBoss::* dI)() = nullptr) : declaredIntent(dI), nonpc(140, "Slime Boss")
    {
        enemyIDNumber = 25; allAttacks = { "goopSpray","preparing","slam","split" };
    }
    ~slimeBoss() {}
    void goopSpray();
    void preparing();
    void slam();
    void split();
    void chooseAndDeclareIntent();
    void EnactIntent();
    bool takeDamage(int amount, character& initiator);
    bool takeDamageNoTrigger(int amount);
    slimeBoss* saveClone() { return saveCloneFromOrig<slimeBoss>(this, new slimeBoss(declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<slimeBoss>(); }
};

class byrd : public nonpc
{
private:
    int flying{3};
    bool firstAttackDone{ false };
    void (byrd::* declaredIntent)();
public:
    byrd(int fl = 3, bool fAD = false, void (byrd::* dI)() = nullptr) : flying(fl), firstAttackDone(fAD), declaredIntent(dI), nonpc(25,31, "Byrd")
    {
        enemyIDNumber = 26; allAttacks = { "caw","peck","swoop","fly","headbutt","stunned" };
    }
    ~byrd() {}
    void caw();
    void peck();
    void swoop();
    void fly();
    void headbutt();
    void stunned();
    void chooseAndDeclareIntent();
    void EnactIntent();
    bool takeDamage(int amount, character& initiator);
    byrd* saveClone() { return saveCloneFromOrig<byrd>(this, new byrd(flying, firstAttackDone, declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<byrd>(); }
};

class mugger : public nonpc
{
private:
    bool firstAttackDone{ false };
    int thieveryNumber{ 15 };
    int goldStolen{ 0 };
    void (mugger::* declaredIntent)();
public:
    mugger(bool fAD = false, void (mugger::* dI)() = nullptr, int gS = 0) : goldStolen(gS), firstAttackDone(fAD), declaredIntent(dI), nonpc(48, 52, "Mugger") 
        { enemyIDNumber = 27; allAttacks = { "smokeBomb","mug","lunge","escape" }; }
    ~mugger() {}
    void mug();
    void lunge();
    void startOfFight();
    void smokeBomb();
    void escape();
    void chooseAndDeclareIntent();
    void EnactIntent();
    mugger* saveClone() { return saveCloneFromOrig<mugger>(this, new mugger(firstAttackDone, declaredIntent,goldStolen)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<mugger>(); }
};

class shelledParasite : public nonpc
{
private:
    bool firstAttackDone{ false };
    void (shelledParasite::* declaredIntent)();
public:
    shelledParasite(bool fAD = false, void (shelledParasite::* dI)() = nullptr) : firstAttackDone(fAD), declaredIntent(dI), nonpc(68, 72, "Shelled Parasite")
        { enemyIDNumber = 28; allAttacks = { "doubleStrike","suck","fell","stunned" }; }
    ~shelledParasite() {}
    void doubleStrike();
    void suck();
    void stunned() {}
    void fell();
    void startOfFight();
    void chooseAndDeclareIntent();
    void EnactIntent();
    bool takeDamage(int amount, character& initiator);
    bool takeDamageNoTrigger(int amount);
    shelledParasite* saveClone() { return saveCloneFromOrig<shelledParasite>(this, new shelledParasite(firstAttackDone, declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<shelledParasite>(); }
};

class theChosen : public nonpc
{
private:
    bool firstAttackDone{ false };
    void (theChosen::* declaredIntent)();
public:
    theChosen(bool fAD = false, void (theChosen::* dI)() = nullptr) : firstAttackDone(fAD), declaredIntent(dI), nonpc(95, 99, "Chosen")
        { enemyIDNumber = 29; allAttacks = { "poke","zap","debilitate","drain","hex" }; }
    ~theChosen() {}
    void poke();
    void zap();
    void debilitate();
    void drain();
    void hex();
    void chooseAndDeclareIntent();
    void EnactIntent();
    theChosen* saveClone() { return saveCloneFromOrig<theChosen>(this, new theChosen(firstAttackDone, declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<theChosen>(); }
};

class sphericGuardian : public nonpc
{
private:
    bool firstAttackDone{ false };
    void (sphericGuardian::* declaredIntent)();
public:
    sphericGuardian(bool fAD = false, void (sphericGuardian::* dI)() = nullptr) : firstAttackDone(fAD), declaredIntent(dI), nonpc(20, "Spheric Guardian")
    { 
        enemyIDNumber = 30; allAttacks = { "slam","activate","harden","attdeb" }; 
    }
    ~sphericGuardian() {}
    void slam();
    void activate();
    void harden();
    void attdeb();
    void startOfFight();
    void chooseAndDeclareIntent();
    void EnactIntent();
    sphericGuardian* saveClone() { return saveCloneFromOrig<sphericGuardian>(this, new sphericGuardian(firstAttackDone, declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<sphericGuardian>(); }
};

class snakePlant : public nonpc
{
private:
    bool firstAttackDone{ false };
    void (snakePlant::* declaredIntent)();
public:
    snakePlant(bool fAD = false, void (snakePlant::* dI)() = nullptr) : firstAttackDone(fAD), declaredIntent(dI), nonpc(75, 79, "Snake Plant")
    {
        enemyIDNumber = 31; allAttacks = { "chomp","enfeeblingSpores" };
    }
    ~snakePlant() {}
    void chomp();
    void enfeeblingSpores();
    void startOfFight();
    void chooseAndDeclareIntent();
    void EnactIntent();
    snakePlant* saveClone() { return saveCloneFromOrig<snakePlant>(this, new snakePlant(firstAttackDone, declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<snakePlant>(); }
};

class snecko : public nonpc
{
private:
    bool firstAttackDone{ false };
    void (snecko::* declaredIntent)();
public:
    snecko(bool fAD = false, void (snecko::* dI)() = nullptr) : firstAttackDone(fAD), declaredIntent(dI), nonpc(114, 120, "Snecko")
    {
        enemyIDNumber = 32; allAttacks = { "bite","perplexingGlare","tailWhip"};
    }
    ~snecko() {}
    void bite();
    void perplexingGlare();
    void tailWhip();
    void chooseAndDeclareIntent();
    void EnactIntent();
    snecko* saveClone() { return saveCloneFromOrig<snecko>(this, new snecko(firstAttackDone, declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<snecko>(); }
};

class centurion : public nonpc
{
private:
    bool mysticGone{ false };
    void (centurion::* declaredIntent)();
public:
    centurion(bool mG = false, void (centurion::* dI)() = nullptr) : mysticGone(mG), declaredIntent(dI), nonpc(76, 80, "Centurion")
    {
        enemyIDNumber = 33; allAttacks = { "slash","fury","defend" };
    }
    ~centurion() {}
    void slash();
    void fury();
    void defend();
    void chooseAndDeclareIntent();
    void EnactIntent();
    centurion* saveClone() { return saveCloneFromOrig<centurion>(this, new centurion(mysticGone, declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<centurion>(); }
};

class mystic : public nonpc
{
private:
    void (mystic::* declaredIntent)();
public:
    mystic( void (mystic::* dI)() = nullptr) :  declaredIntent(dI), nonpc(48, 56, "Mystic")
    {
        enemyIDNumber = 34; allAttacks = { "heal","buff","attdeb" };
    }
    ~mystic() {}
    void heal__();
    void buff();
    void attdeb();
    void chooseAndDeclareIntent();
    void EnactIntent();
    mystic* saveClone() { return saveCloneFromOrig<mystic>(this, new mystic( declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<mystic>(); }
};

class bookOfStabbing : public nonpc
{
private:
    int multiCount{ 2 };
    void (bookOfStabbing::* declaredIntent)();
public:
    bookOfStabbing(int mC = 2, void (bookOfStabbing::* dI)() = nullptr) :multiCount(mC), declaredIntent(dI), nonpc(160, 162, "Book Of Stabbing")
    {
        enemyIDNumber = 35; allAttacks = { "multiStab","singleStab" };
    }
    ~bookOfStabbing() {}
    void multiStab();
    void singleStab();
    void startOfFight();
    void chooseAndDeclareIntent();
    void EnactIntent();
    bookOfStabbing* saveClone() { return saveCloneFromOrig<bookOfStabbing>(this, new bookOfStabbing(multiCount, declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<bookOfStabbing>(); }
};

class gremlinLeader : public nonpc
{
private:
    void (gremlinLeader::* declaredIntent)();
public:
    gremlinLeader(void (gremlinLeader::* dI)() = nullptr) : declaredIntent(dI), nonpc(140, 148, "Gremlin Leader")
    {
        enemyIDNumber = 36; allAttacks = { "encourage","rally","stab"};
    }
    ~gremlinLeader() {}
    void encourage();
    void rally();
    void stab();
    void chooseAndDeclareIntent();
    void EnactIntent();
    gremlinLeader* saveClone() { return saveCloneFromOrig<gremlinLeader>(this, new gremlinLeader(declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<gremlinLeader>(); }
};

class taskmaster : public nonpc
{
private:
    void (taskmaster::* declaredIntent)();
public:
    taskmaster(void (taskmaster::* dI)() = nullptr) : declaredIntent(dI), nonpc(54, 60, "Taskmaster")
    {
        enemyIDNumber = 37; allAttacks = { "scouringWhip" };
    }
    ~taskmaster() {}
    void scouringWhip();
    void chooseAndDeclareIntent();
    void EnactIntent();
    taskmaster* saveClone() { return saveCloneFromOrig<taskmaster>(this, new taskmaster(declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<taskmaster>(); }
};

class theChamp : public nonpc
{
private:
    void (theChamp::* declaredIntent)();
    int defStanceCount{ 0 };
    bool phaseTwo{ false };
public:
    theChamp(int dSC = 0, void (theChamp::* dI)() = nullptr, bool p2 = false) :phaseTwo(p2), defStanceCount(dSC), declaredIntent(dI), nonpc(420, "The Champ")
    {
        enemyIDNumber = 38; allAttacks = { "defensiveStance","faceSlap","taunt","heavySlash","gloat","execute","anger"};
    }
    ~theChamp() {}
    void defensiveStance();
    void faceSlap();
    void taunt();
    void heavySlash();
    void gloat();
    void execute();
    void anger();
    void chooseAndDeclareIntent();
    void EnactIntent();
    theChamp* saveClone() { return saveCloneFromOrig<theChamp>(this, new theChamp(defStanceCount,declaredIntent,phaseTwo)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<theChamp>(); }
};

class theCollector : public nonpc
{
private:
    bool firstAttackDone{ false };
    void (theCollector::* declaredIntent)();
public:
    theCollector(bool fAD = false, void (theCollector::* dI)() = nullptr) : firstAttackDone(fAD), declaredIntent(dI), nonpc(282, "The Collector")
    {
        enemyIDNumber = 39; allAttacks = { "buff","fireball","megaDebuff","spawn" };
    }
    ~theCollector() {}
    void buff();
    void fireball();
    void megaDebuff();
    void spawn();
    void chooseAndDeclareIntent();
    void EnactIntent();
    theCollector* saveClone() { return saveCloneFromOrig<theCollector>(this, new theCollector(firstAttackDone,declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<theCollector>(); }
};

class torchHead : public nonpc
{
private:
    void (torchHead::* declaredIntent)();
public:
    torchHead(void (torchHead::* dI)() = nullptr) : declaredIntent(dI), nonpc(38,40, "Torch Head")
    {
        enemyIDNumber = 40; allAttacks = { "tackle" };
    }
    ~torchHead() {} 
    void tackle();
    void chooseAndDeclareIntent();
    void EnactIntent();
    bool isMinion() const { return true; }
    torchHead* saveClone() { return saveCloneFromOrig<torchHead>(this, new torchHead(declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<torchHead>(); }
};

class bronzeAutomaton : public nonpc
{
private:
    bool firstAttackDone{ false };
    void (bronzeAutomaton::* declaredIntent)();
public:
    bronzeAutomaton(bool fAD = false, void (bronzeAutomaton::* dI)() = nullptr) : firstAttackDone(fAD), declaredIntent(dI), nonpc(300, "Bronze Automaton")
    {
        enemyIDNumber = 41; allAttacks = { "stunned","boost","flail","hyperBeam","spawnOrbs"};
    }
    ~bronzeAutomaton() {}
    void stunned() {}
    void boost();
    void flail();
    void hyperBeam();
    void spawnOrbs();
    void startOfFight();
    void chooseAndDeclareIntent();
    void EnactIntent();
    bronzeAutomaton* saveClone() { return saveCloneFromOrig<bronzeAutomaton>(this, new bronzeAutomaton(firstAttackDone, declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<bronzeAutomaton>(); }
};

class bronzeOrb : public nonpc
{
private:
    bool stasised{false};
    void (bronzeOrb::* declaredIntent)();
public:
    bronzeOrb(bool s = false, void (bronzeOrb::* dI)() = nullptr) : stasised(s), declaredIntent(dI), nonpc(52, 58, "Bronze Orb")
    {
        enemyIDNumber = 42; allAttacks = { "stasis","beam","supportBeam"};
    }
    ~bronzeOrb() {}
    void stasis();
    void beam();
    void supportBeam();
    void chooseAndDeclareIntent();
    void EnactIntent();
    bool isMinion() const { return true; }
    bronzeOrb* saveClone() { return saveCloneFromOrig<bronzeOrb>(this, new bronzeOrb(stasised, declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<bronzeOrb>(); }
};

class pointy : public nonpc
{
private:
    void (pointy::* declaredIntent)();
public:
    pointy(void (pointy::* dI)() = nullptr) : declaredIntent(dI), nonpc(30, "Pointy")
    {
        enemyIDNumber = 43; allAttacks = { "attack" };
    }
    ~pointy() {}
    void attack();
    void chooseAndDeclareIntent();
    void EnactIntent();
    pointy* saveClone() { return saveCloneFromOrig<pointy>(this, new pointy(declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<pointy>(); }
};
class romeo : public nonpc
{
private:
    void (romeo::* declaredIntent)();
public:
    romeo(void (romeo::* dI)() = nullptr) : declaredIntent(dI), nonpc(35,39, "Romeo")
    {
        enemyIDNumber = 44; allAttacks = { "mock","agonizingSlash","crossSlash"};
    }
    ~romeo() {}
    void mock() {}
    void agonizingSlash();
    void crossSlash();
    void chooseAndDeclareIntent();
    void EnactIntent();
    romeo* saveClone() { return saveCloneFromOrig<romeo>(this, new romeo(declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<romeo>(); }
};
class bear : public nonpc
{
private:
    void (bear::* declaredIntent)();
public:
    bear(void (bear::* dI)() = nullptr) : declaredIntent(dI), nonpc(38, 42, "Bear")
    {
        enemyIDNumber = 45; allAttacks = { "bearHug","lunge","maul" };
    }
    ~bear() {}
    void bearHug();
    void lunge();
    void maul();
    void chooseAndDeclareIntent();
    void EnactIntent();
    bear* saveClone() { return saveCloneFromOrig<bear>(this, new bear(declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<bear>(); }
};
class orbWalker : public nonpc
{
private:
    void (orbWalker::* declaredIntent)();
    int strengthNumber{ 3 };
public:
    orbWalker(void (orbWalker::* dI)() = nullptr) : declaredIntent(dI), nonpc(90, 96, "Orb Walker")
    {
        enemyIDNumber = 46; allAttacks = { "laser","claw" };
    }
    ~orbWalker() {}
    void laser();
    void claw();
    void chooseAndDeclareIntent();
    void EnactIntent();
    void startOfFight();
    orbWalker* saveClone() { return saveCloneFromOrig<orbWalker>(this, new orbWalker(declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<orbWalker>(); }
};
class spiker : public nonpc
{
private:
    void (spiker::* declaredIntent)();
    int thornsNumber{ 3 };
    int spikeCount{ 0 };
public:
    spiker(void (spiker::* dI)() = nullptr, int sC = 0) : spikeCount(sC), declaredIntent(dI), nonpc(42, 56, "Spiker")
    {
        enemyIDNumber = 47; allAttacks = { "cut","spike"};
    }
    ~spiker() {}
    void cut();
    void spike();
    void chooseAndDeclareIntent();
    void EnactIntent();
    void startOfFight();
    spiker* saveClone() { return saveCloneFromOrig<spiker>(this, new spiker(declaredIntent, spikeCount)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<spiker>(); }
};
class exploder : public nonpc
{
private:
    void (exploder::* declaredIntent)();
    int untilExplode{ 0 };
public:
    exploder(void (exploder::* dI)() = nullptr, int uE = 0) : untilExplode(uE), declaredIntent(dI), nonpc(30, "Exploder")
    {
        enemyIDNumber = 48; allAttacks = { "slam","explode" };
    }
    ~exploder() {}
    void slam();
    void explode();
    void chooseAndDeclareIntent();
    void EnactIntent();
    void startOfFight();
    exploder* saveClone() { return saveCloneFromOrig<exploder>(this, new exploder(declaredIntent, untilExplode)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<exploder>(); }
};
class repulsor : public nonpc
{
private:
    void (repulsor::* declaredIntent)();
public:
    repulsor(void (repulsor::* dI)() = nullptr) :  declaredIntent(dI), nonpc(29,35, "Repulsor")
    {
        enemyIDNumber = 49; allAttacks = { "bashh","repulse" };
    }
    ~repulsor() {}
    void bashh();
    void repulse();
    void chooseAndDeclareIntent();
    void EnactIntent();
    repulsor* saveClone() { return saveCloneFromOrig<repulsor>(this, new repulsor(declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<repulsor>(); }
};
class theMaw : public nonpc
{
private:
    void (theMaw::* declaredIntent)();
public:
    theMaw(void (theMaw::* dI)() = nullptr) : declaredIntent(dI), nonpc(300, "The Maw")
    {
        enemyIDNumber = 50; allAttacks = { "roar","drool","slam","nom" };
    }
    ~theMaw() {}
    void roar();
    void drool();
    void slam();
    void nom();
    void chooseAndDeclareIntent();
    void EnactIntent();
    theMaw* saveClone() { return saveCloneFromOrig<theMaw>(this, new theMaw(declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<theMaw>(); }
};
class spireGrowth : public nonpc
{
private:
    void (spireGrowth::* declaredIntent)();
public:
    spireGrowth(void (spireGrowth::* dI)() = nullptr) : declaredIntent(dI), nonpc(170, "Spire Growth")
    {
        enemyIDNumber = 51; allAttacks = { "quickTackle","smash","constrict" };
    }
    ~spireGrowth() {}
    void quickTackle();
    void smash();
    void constrict();
    void chooseAndDeclareIntent();
    void EnactIntent();
    spireGrowth* saveClone() { return saveCloneFromOrig<spireGrowth>(this, new spireGrowth(declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<spireGrowth>(); }
};
class transient : public nonpc
{
private:
    void (transient::* declaredIntent)();
public:
    transient(void (transient::* dI)() = nullptr) : declaredIntent(dI), nonpc(999, "Transient")
    {
        enemyIDNumber = 52; allAttacks = { "attack" };
    }
    ~transient() {}
    void attack();
    void chooseAndDeclareIntent();
    void EnactIntent();
    bool takeDamage(int amount, character& initiator);
    bool takeDamageNoTrigger(int amount);
    transient* saveClone() { return saveCloneFromOrig<transient>(this, new transient(declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<transient>(); }
};
class writhingMass : public nonpc
{
private:
    void (writhingMass::* declaredIntent)();
    bool implanted{ false };
public:
    writhingMass(void (writhingMass::* dI)() = nullptr, bool imp = false) : implanted(imp), declaredIntent(dI), nonpc(160, "Writhing Mass")
    {
        enemyIDNumber = 53; allAttacks = { "implant","flail","whither","multiStrike","strongStrike" };
    }
    ~writhingMass() {}
    void implant();
    void flail();
    void whither();
    void multiStrike();
    void strongStrike();
    void chooseAndDeclareIntent();
    void EnactIntent();
    bool takeDamage(int amount, character& initiator);
    void startOfFight();
    writhingMass* saveClone() { return saveCloneFromOrig<writhingMass>(this, new writhingMass(declaredIntent,implanted)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<writhingMass>(); }
};
class giantHead : public nonpc
{
private:
    void (giantHead::* declaredIntent)();
    int augment{ 0 };
public:
    giantHead(void (giantHead::* dI)() = nullptr, int aug = 0) : augment(aug), declaredIntent(dI), nonpc(500, "Giant Head")
    {
        enemyIDNumber = 54; allAttacks = { "count","glare","itsTime" };
    }
    ~giantHead() {}
    void count();
    void glare();
    void itsTime();
    void chooseAndDeclareIntent();
    void EnactIntent();
    void startOfFight();
    giantHead* saveClone() { return saveCloneFromOrig<giantHead>(this, new giantHead(declaredIntent,augment)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<giantHead>(); }
};
class nemesis : public nonpc
{
private:
    void (nemesis::* declaredIntent)();
public:
    nemesis(void (nemesis::* dI)() = nullptr) : declaredIntent(dI), nonpc(185, "Nemesis")
    {
        enemyIDNumber = 55; allAttacks = { "debuff","attack","scythe" };
    }
    ~nemesis() {}
    void debuff();
    void attack();
    void scythe();
    void chooseAndDeclareIntent();
    void EnactIntent();
    nemesis* saveClone() { return saveCloneFromOrig<nemesis>(this, new nemesis(declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<nemesis>(); }
};
class reptomancer : public nonpc
{
private:
    void (reptomancer::* declaredIntent)();
public:
    reptomancer(void (reptomancer::* dI)() = nullptr) : declaredIntent(dI), nonpc(180,190, "Reptomancer")
    {
        enemyIDNumber = 56; allAttacks = { "summon","snakeStrike","bigBite" };
    }
    ~reptomancer() {}
    void summon();
    void snakeStrike();
    void bigBite();
    void chooseAndDeclareIntent();
    void EnactIntent();
    reptomancer* saveClone() { return saveCloneFromOrig<reptomancer>(this, new reptomancer(declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<reptomancer>(); }
};
class dagger : public nonpc
{
private:
    void (dagger::* declaredIntent)();
public:
    dagger(void (dagger::* dI)() = nullptr) : declaredIntent(dI), nonpc(20, 25, "Dagger")
    {
        enemyIDNumber = 57; allAttacks = { "stab","explode" };
    }
    ~dagger() {}
    void stab();
    void explode();
    void chooseAndDeclareIntent();
    void EnactIntent();
    bool isMinion() const { return true; }
    dagger* saveClone() { return saveCloneFromOrig<dagger>(this, new dagger(declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<dagger>(); }
};
class darkling : public nonpc
{
private:
    void (darkling::* declaredIntent)();
    int attackNumber{ 0 };
    bool middleOne{ false };
public:
    darkling(void (darkling::* dI)() = nullptr, int aN = 0, bool mO = false) : middleOne(mO), attackNumber(aN), declaredIntent(dI), nonpc(48, 56, "Darkling")
    {
        enemyIDNumber = 58; allAttacks = { "nip","chomp","harden" }; dormant_ = true; dormant_turns_ = 1;
    }
    ~darkling() {}
    void nip();
    void chomp();
    void harden();
    void chooseAndDeclareIntent();
    void EnactIntent();
    void returnFromDormant() { health_ = max_health_ / 2; }
    darkling* saveClone() { return saveCloneFromOrig<darkling>(this, new darkling(declaredIntent, attackNumber, middleOne)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<darkling>(nullptr,attackNumber,middleOne); }
};
class donu : public nonpc
{
private:
    void (donu::* declaredIntent)();
public:
    donu(void (donu::* dI)() = nullptr) : declaredIntent(dI), nonpc(250, "Donu")
    {
        enemyIDNumber = 59; allAttacks = { "circleOfPower","beam" };
    }
    ~donu() {}
    void circleOfPower();
    void beam();
    void chooseAndDeclareIntent();
    void EnactIntent();
    void startOfFight();
    donu* saveClone() { return saveCloneFromOrig<donu>(this, new donu(declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<donu>(); }
};
class deca : public nonpc
{
private:
    void (deca::* declaredIntent)();
public:
    deca(void (deca::* dI)() = nullptr) : declaredIntent(dI), nonpc(250, "Deca")
    {
        enemyIDNumber = 60; allAttacks = { "squareOfProtection","beam" };
    }
    ~deca() {}
    void squareOfProtection();
    void beam();
    void chooseAndDeclareIntent();
    void EnactIntent();
    void startOfFight();
    deca* saveClone() { return saveCloneFromOrig<deca>(this, new deca(declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<deca>(); }
};
class awakenedOne : public nonpc
{
private:
    void (awakenedOne::* declaredIntent)();
    bool awakened{ false };
public:
    awakenedOne(void (awakenedOne::* dI)() = nullptr, bool a = false) : awakened(a), declaredIntent(dI), nonpc(300, "Awakened One")
    {
        enemyIDNumber = 61; allAttacks = { "slash","soulStrike","darkEcho","sludge","tackle" }; dormant_ = true; dormant_turns_ = 0;
    }
    static int storedStrength;
    ~awakenedOne() {}
    void slash();
    void soulStrike();
    void darkEcho();
    void sludge();
    void tackle();
    void chooseAndDeclareIntent();
    void EnactIntent();
    void startOfFight();
    void returnFromDormant();
    awakenedOne* saveClone() { return saveCloneFromOrig<awakenedOne>(this, new awakenedOne(declaredIntent, awakened)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<awakenedOne>(); }
};
class timeEater : public nonpc
{
private:
    void (timeEater::* declaredIntent)();
    bool alreadyHalf{ false };
public:
    timeEater(void (timeEater::* dI)() = nullptr, bool aH = false) : alreadyHalf(aH), declaredIntent(dI), nonpc(456, "Time Eater")
    {
        enemyIDNumber = 62; allAttacks = { "reverberate","headSlam","ripple","haste" };
    }
    ~timeEater() {}
    void reverberate();
    void headSlam();
    void ripple();
    void haste();
    void chooseAndDeclareIntent();
    void EnactIntent();
    void startOfFight();
    timeEater* saveClone() { return saveCloneFromOrig<timeEater>(this, new timeEater(declaredIntent, alreadyHalf)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<timeEater>(); }
};
class spireShield : public nonpc
{
private:
    void (spireShield::* declaredIntent)();
public:
    spireShield(void (spireShield::* dI)() = nullptr) : declaredIntent(dI), nonpc(110, "Spire Shield")
    {
        enemyIDNumber = 63; allAttacks = { "bashh","fortify","smash" };
    }
    ~spireShield() {}
    void bashh();
    void fortify();
    void smash();
    void chooseAndDeclareIntent();
    void EnactIntent();
    void startOfFight();
    spireShield* saveClone() { return saveCloneFromOrig<spireShield>(this, new spireShield(declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<spireShield>(); }
};
class spireSword: public nonpc
{
private:
    void (spireSword::* declaredIntent)();
public:
    spireSword(void (spireSword::* dI)() = nullptr) : declaredIntent(dI), nonpc(160, "Spire Sword")
    {
        enemyIDNumber = 64; allAttacks = { "burnStrike","piercer","skewer" };
    }
    ~spireSword() {}
    void burnStrike();
    void piercer();
    void skewer();
    void chooseAndDeclareIntent();
    void EnactIntent();
    void startOfFight();
    spireSword* saveClone() { return saveCloneFromOrig<spireSword>(this, new spireSword(declaredIntent)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<spireSword>(); }
};
class corruptHeart : public nonpc
{
private:
    void (corruptHeart::* declaredIntent)();
    int buffCount{ 0 };
public:
    corruptHeart(void (corruptHeart::* dI)() = nullptr, int bC = 0) : buffCount(bC), declaredIntent(dI), nonpc(750, "Corrupt Heart")
    {
        enemyIDNumber = 65; allAttacks = { "debilitate","bloodShots","echo","buff" };
    }
    ~corruptHeart() {}
    void debilitate();
    void bloodShots();
    void echo();
    void buff();
    void chooseAndDeclareIntent();
    void EnactIntent();
    void startOfFight();
    corruptHeart* saveClone() { return saveCloneFromOrig<corruptHeart>(this, new corruptHeart(declaredIntent, buffCount)); }
    std::unique_ptr<nonpc> clone() const { return std::make_unique<corruptHeart>(); }
};
#endif

