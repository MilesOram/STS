#ifndef RELIC_H
#define RELIC_H
#include "top.h"

class akabeko : public relic
{
private:
    int firstAttack{ 1 };
public:
    akabeko(int fA = 1) :firstAttack(fA), relic(relicRarity::Common, "Akabeko", 90) {}
    void execute();
    void triggered();
    int getCounter() { return firstAttack; }
    void fightStartSetup();
    akabeko* clone() const { return new akabeko(firstAttack); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<akabeko>(); }
    int getRelicIDNum() const { return 1; }
    ~akabeko() {}
};
class anchor : public relic
{
private:
public:
    anchor() : relic(relicRarity::Common, "Anchor", 90) {}
    void fightStartSetup();
    anchor* clone() const { return new anchor(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<anchor>(); }
    int getRelicIDNum() const { return 2; }
    ~anchor() {}
};
class ancientTeaSet : public relic
{
private:
    bool rested{ false };
public:
    ancientTeaSet(bool r = false) : rested(r), relic(relicRarity::Common, "Ancient Tea Set", 90) {}
    void triggered();
    void onPickUp();
    void fightStartSetup();
    ancientTeaSet* clone() const { return new ancientTeaSet(rested); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<ancientTeaSet>(); }
    int getRelicIDNum() const { return 3; }
    ~ancientTeaSet() {}
};
class artOfWar : public relic
{
private:
    bool skillOnly{ false };
public:
    artOfWar(bool sO = false) : skillOnly(sO), relic(relicRarity::Common, "Art Of War") {}
    void execute();
    void triggered(cardHolder& ch);
    void turnStartSetup();
    int getCounter() { return skillOnly; }
    void fightStartSetup();
    artOfWar* clone() const { return new artOfWar(skillOnly); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<artOfWar>(); }
    int getRelicIDNum() const { return 4; }
    ~artOfWar() {}
};

class bagOfMarbles : public relic
{
private:
public:
    bagOfMarbles() : relic(relicRarity::Common, "Bag Of Marbles") {}
    void fightStartSetup();
    bagOfMarbles* clone() const { return new bagOfMarbles(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<bagOfMarbles>(); }
    int getRelicIDNum() const { return 5; }
    ~bagOfMarbles() {}
};
class bagOfPreparation : public relic
{
private:
public:
    bagOfPreparation() : relic(relicRarity::Common, "Bag Of Preparation") {}
    void fightStartSetup();
    bagOfPreparation* clone() const { return new bagOfPreparation(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<bagOfPreparation>(); }
    int getRelicIDNum() const { return 6; }
    ~bagOfPreparation() {}
};
class bloodVial : public relic
{
private:
public:
    bloodVial() : relic(relicRarity::Common, "Blood Vial") {}
    void fightStartSetup();
    bloodVial* clone() const { return new bloodVial(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<bloodVial>(); }
    int getRelicIDNum() const { return 7; }
    ~bloodVial() {}
};
class bronzeScales : public relic
{
private:
public:
    bronzeScales() : relic(relicRarity::Common, "Bronze Scales") {}
    void fightStartSetup();
    bronzeScales* clone() const { return new bronzeScales(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<bronzeScales>(); }
    int getRelicIDNum() const { return 8; }
    ~bronzeScales() {}
};
class centennialPuzzle : public relic
{
private:
    bool first{ true };
public:
    centennialPuzzle(bool f = true) : first(f), relic(relicRarity::Common, "Centennial Puzzle") {}
    void execute();
    void triggered();
    void fightStartSetup();
    int getCounter() { return first; }
    centennialPuzzle* clone() const { return new centennialPuzzle(first); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<centennialPuzzle>(); }
    int getRelicIDNum() const { return 9; }
    ~centennialPuzzle() {}
};
class ceramicFish : public relic
{
private:
public:
    ceramicFish() : relic(relicRarity::Common, "Ceramic Fish") {}
    void execute();
    void triggered();
    void onPickUp();
    ceramicFish* clone() const { return new ceramicFish(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<ceramicFish>(); }
    int getRelicIDNum() const { return 10; }
    ~ceramicFish() {}
};
class dreamCatcher : public relic
{
private:
public:
    dreamCatcher() : relic(relicRarity::Common, "Dream Catcher") {}
    void execute();
    void triggered();
    void onPickUp();
    dreamCatcher* clone() const { return new dreamCatcher(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<dreamCatcher>(); }
    int getRelicIDNum() const { return 11; }
    ~dreamCatcher() {}
};
class happyFlower : public relic
{
private:
    int turnCount{ 0 };
public:
    happyFlower(int tC = 0) : turnCount(tC), relic(relicRarity::Common, "Happy Flower") {}
    void execute();
    void turnStartSetup();
    void randomizeCounter() { turnCount = random_int(0, 2); }
    int getCounter() { return turnCount; }
    float getCounterReward() { return powf(static_cast<float>(turnCount) / 3, 2); }
    happyFlower* clone() const { return new happyFlower(turnCount); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<happyFlower>(); }
    int getRelicIDNum() const { return 12; }
    ~happyFlower() {}
};
class juzuBracelet : public relic
{
private:
public:
    juzuBracelet() : relic(relicRarity::Common, "Juzu Bracelet") {}
    void onDropped();
    void onPickUp();
    juzuBracelet* clone() const { return new juzuBracelet(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<juzuBracelet>(); }
    int getRelicIDNum() const { return 13; }
    ~juzuBracelet() {}
};
class lantern : public relic
{
private:
public:
    lantern() : relic(relicRarity::Common, "Lantern") {}
    void fightStartSetup();
    lantern* clone() const { return new lantern(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<lantern>(); }
    int getRelicIDNum() const { return 14; }
    ~lantern() {}
};
class mawBank : public relic
{
private:
public:
    static bool offSwitch;
    mawBank() : relic(relicRarity::Common, "Maw Bank") {}
    void execute();
    void triggered();
    void onPickUp();
    void onDropped();
    mawBank* clone() const { return new mawBank(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<mawBank>(); }
    int getRelicIDNum() const { return 15; }
    ~mawBank() { onDropped(); }
};
class mealTicket : public relic
{
private:
public:
    mealTicket() : relic(relicRarity::Common, "Meal Ticket") {}
    void execute();
    void triggered();
    void onPickUp();
    mealTicket* clone() const { return new mealTicket(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<mealTicket>(); }
    int getRelicIDNum() const { return 16; }
    ~mealTicket() {}
};
class nunchuku : public relic
{
private:
    int attackCounter{ 0 };
public:
    nunchuku(int aC = 0) : attackCounter(aC), relic(relicRarity::Common, "Nunchuku") {}
    void execute();
    void triggered();
    void randomizeCounter() { attackCounter = random_int(0, 9); }
    int getCounter() { return attackCounter; }
    float getCounterReward() { return powf(static_cast<float>(attackCounter) / 9, 2); }
    void fightStartSetup();
    nunchuku* clone() const { return new nunchuku(attackCounter); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<nunchuku>(); }
    int getRelicIDNum() const { return 17; }
    ~nunchuku() {}
};
class oddlySmoothStone : public relic
{
private:
public:
    oddlySmoothStone() : relic(relicRarity::Common, "Oddly Smooth Stone") {}
    void fightStartSetup();
    oddlySmoothStone* clone() const { return new oddlySmoothStone(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<oddlySmoothStone>(); }
    int getRelicIDNum() const { return 18; }
    ~oddlySmoothStone() {}
};
class omamori : public relic
{
private:
    int* cardCount;
public:
    omamori();
    void onDropped();
    void onPickUp();
    omamori* clone() const { return new omamori(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<omamori>(); }
    int getCounter() { return *cardCount; }
    int getRelicIDNum() const { return 19; }
    ~omamori() {}
};
class orichalcum : public relic
{
private:
public:
    orichalcum() : relic(relicRarity::Common, "Orichalcum") {}
    void execute();
    void triggered();
    void fightStartSetup();
    orichalcum* clone() const { return new orichalcum(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<orichalcum>(); }
    int getRelicIDNum() const { return 20; }
    ~orichalcum() {}
};
class penNib : public relic
{
private:
    int attackCounter{ 0 };
public:
    penNib(int aC = 0) : attackCounter(aC), relic(relicRarity::Common, "Pen Nib") {}
    void execute();
    void triggered();
    void randomizeCounter() { attackCounter = random_int(0, 9); }
    int getCounter() { return attackCounter; }
    float getCounterReward() { return powf(static_cast<float>(attackCounter) / 9, 2); }
    void fightStartSetup();
    penNib* clone() const { return new penNib(attackCounter); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<penNib>(); }
    int getRelicIDNum() const { return 21; }
    ~penNib() {}
};
class potionBelt : public relic
{
private:
public:
    potionBelt() : relic(relicRarity::Common, "Potion Belt") {}
    void onPickUp();
    potionBelt* clone() const { return new potionBelt(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<potionBelt>(); }
    int getRelicIDNum() const { return 22; }
    ~potionBelt() {}
};
class preservedInsect : public relic
{
private:
public:
    preservedInsect() : relic(relicRarity::Common, "Preserved Insect", 5) {}
    void fightStartSetup();
    preservedInsect* clone() const { return new preservedInsect(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<preservedInsect>(); }
    int getRelicIDNum() const { return 23; }
    ~preservedInsect() {}
};
class regalPillow : public relic
{
private:
public:
    regalPillow() : relic(relicRarity::Common, "Regal Pillow") {}
    void execute();
    void triggered();
    void onPickUp();
    regalPillow* clone() const { return new regalPillow(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<regalPillow>(); }
    int getRelicIDNum() const { return 24; }
    ~regalPillow() {}
};
class smilingMask : public relic
{
private:
public:
    smilingMask() : relic(relicRarity::Common, "Smiling Mask") {}
    void onPickUp();
    void onDropped();
    smilingMask* clone() const { return new smilingMask(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<smilingMask>(); }
    int getRelicIDNum() const { return 25; }
    ~smilingMask() {}
};
class strawberry : public relic
{
private:
public:
    strawberry() : relic(relicRarity::Common, "Strawberry") {}
    void onPickUp();
    strawberry* clone() const { return new strawberry(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<strawberry>(); }
    int getRelicIDNum() const { return 26; }
    ~strawberry() {}
};
class theBoot : public relic
{
private:
public:
    theBoot() : relic(relicRarity::Common, "The Boot", 1002) {}
    void execute();
    void triggered();
    void fightStartSetup();
    theBoot* clone() const { return new theBoot(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<theBoot>(); }
    int getRelicIDNum() const { return 27; }
    ~theBoot() {}
};
class tinyChest : public relic
{
private:
    int qCounter{ 0 };
public:
    tinyChest(int qC = 0) : qCounter(qC), relic(relicRarity::Common, "Tiny Chest") {}
    void onPickUp();
    void triggered();
    int getCounter() { return qCounter; }
    tinyChest* clone() const { return new tinyChest(qCounter); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<tinyChest>(); }
    int getRelicIDNum() const { return 28; }
    ~tinyChest() {}
};
class toyOrnithopter : public relic
{
private:
public:
    toyOrnithopter() : relic(relicRarity::Common, "Toy Ornithopter") {}
    void onPickUp();
    void triggered();
    toyOrnithopter* clone() const { return new toyOrnithopter(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<toyOrnithopter>(); }
    int getRelicIDNum() const { return 29; }
    ~toyOrnithopter() {}
};
class vajra : public relic
{
private:
public:
    vajra() : relic(relicRarity::Common, "Vajra") {}
    void fightStartSetup();
    vajra* clone() const { return new vajra(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<vajra>(); }
    int getRelicIDNum() const { return 30; }
    ~vajra() {}
};
class warPaint : public relic
{
private:
public:
    warPaint() : relic(relicRarity::Common, "War Paint") {}
    void onPickUp();
    warPaint* clone() const { return new warPaint(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<warPaint>(); }
    int getRelicIDNum() const { return 31; }
    ~warPaint() {}
};
class whetstone : public relic
{
private:
public:
    whetstone() : relic(relicRarity::Common, "Whetstone") {}
    void onPickUp();
    whetstone* clone() const { return new whetstone(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<whetstone>(); }
    int getRelicIDNum() const { return 32; }
    ~whetstone() {}
};

// ##############################
// ########## uncommon ##########
// ##############################
class blueCandle : public relic
{
private:
public:
    blueCandle() : relic(relicRarity::Uncommon, "Blue Candle") {}
    void onPickUp();
    void onDropped();
    blueCandle* clone() const { return new blueCandle(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<blueCandle>(); }
    int getRelicIDNum() const { return 33; }
    ~blueCandle() {}
};
class bottledFlame : public relic
{
private:
public:
    bottledFlame() : relic(relicRarity::Uncommon, "Bottled Flame") {}
    void onPickUp();
    bottledFlame* clone() const { return new bottledFlame(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<bottledFlame>(); }
    int getRelicIDNum() const { return 34; }
    ~bottledFlame() {}
};
class bottledLightning : public relic
{
private:
public:
    bottledLightning() : relic(relicRarity::Uncommon, "Bottled Lightning") {}
    void onPickUp();
    bottledLightning* clone() const { return new bottledLightning(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<bottledLightning>(); }
    int getRelicIDNum() const { return 35; }
    ~bottledLightning() {}
};
class bottledTornado : public relic
{
private:
public:
    bottledTornado() : relic(relicRarity::Uncommon, "Bottled Tornado") {}
    void onPickUp();
    bottledTornado* clone() const { return new bottledTornado(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<bottledTornado>(); }
    int getRelicIDNum() const { return 36; }
    ~bottledTornado() {}
};
class darkstonePeriapt : public relic
{
private:
public:
    darkstonePeriapt() : relic(relicRarity::Uncommon, "Darkstone Periapt") {}
    void execute();
    void triggered(cardHolder& ch);
    void onPickUp();
    darkstonePeriapt* clone() const { return new darkstonePeriapt(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<darkstonePeriapt>(); }
    int getRelicIDNum() const { return 37; }
    ~darkstonePeriapt() {}
};
class eternalFeather : public relic
{
private:
    int healNumber{ 0 };
public:
    eternalFeather(int hN = 0) : healNumber(hN), relic(relicRarity::Uncommon, "Eternal Feather") {}
    void execute();
    void triggered();
    void onPickUp();
    eternalFeather* clone() const { return new eternalFeather(healNumber); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<eternalFeather>(); }
    int getRelicIDNum() const { return 38; }
    ~eternalFeather() {}
};
class frozenEgg : public relic
{
private:
public:
    frozenEgg() : relic(relicRarity::Uncommon, "Frozen Egg") {}
    void execute();
    void triggered();
    void onPickUp();
    frozenEgg* clone() const { return new frozenEgg(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<frozenEgg>(); }
    int getRelicIDNum() const { return 39; }
    ~frozenEgg() {}
};
class gremlinHorn : public relic
{
private:
public:
    gremlinHorn() : relic(relicRarity::Uncommon, "Gremlin Horn") {}
    void execute();
    void triggered();
    void fightStartSetup();
    gremlinHorn* clone() const { return new gremlinHorn(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<gremlinHorn>(); }
    int getRelicIDNum() const { return 40; }
    ~gremlinHorn() {}
};
class hornCleat : public relic
{
private:
    int turnCount{ 0 };
public:
    hornCleat(int tC = 0) : turnCount(tC), relic(relicRarity::Uncommon, "Horn Cleat", 90) {}
    void turnStartSetup();
    void fightStartSetup();
    int getCounter() { return turnCount; }
    hornCleat* clone() const { return new hornCleat(turnCount); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<hornCleat>(); }
    int getRelicIDNum() const { return 41; }
    ~hornCleat() {}
};
class inkBottle : public relic
{
private:
    int cardCount{ 0 };
public:
    inkBottle(int cC = 0) : cardCount(cC), relic(relicRarity::Uncommon, "Ink Bottle") {}
    void execute();
    void triggered();
    void randomizeCounter() { cardCount = random_int(0, 9); }
    int getCounter() { return cardCount; }
    float getCounterReward() { return powf(static_cast<float>(cardCount) / 9, 2); }
    void fightStartSetup();
    inkBottle* clone() const { return new inkBottle(cardCount); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<inkBottle>(); }
    int getRelicIDNum() const { return 42; }
    ~inkBottle() {}
};
class kunai : public relic
{
private:
    int attackCounter{ 0 };
public:
    kunai(int aC = 0) : attackCounter(aC), relic(relicRarity::Uncommon, "Kunai") {}
    void execute();
    void triggered();
    void turnStartSetup();
    int getCounter() { return attackCounter; }
    void fightStartSetup();
    kunai* clone() const { return new kunai(attackCounter); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<kunai>(); }
    int getRelicIDNum() const { return 43; }
    ~kunai() {}
};
class letterOpener : public relic
{
private:
    int skillCounter{ 0 };
public:
    letterOpener(int sC = 0) : skillCounter(sC), relic(relicRarity::Uncommon, "Letter Opener") {}
    void execute();
    void triggered();
    void turnStartSetup();
    int getCounter() { return skillCounter; }
    void fightStartSetup();
    letterOpener* clone() const { return new letterOpener(skillCounter); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<letterOpener>(); }
    int getRelicIDNum() const { return 44; }
    ~letterOpener() {}
};
class matryoshka : public relic
{
private:
    int relicCounter{ 2 };
public:
    matryoshka(int rC = 2) : relicCounter(rC), relic(relicRarity::Uncommon, "Matryoshka") {}
    void execute();
    void triggered();
    void onPickUp();
    int getCounter() { return relicCounter; }
    matryoshka* clone() const { return new matryoshka(relicCounter); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<matryoshka>(); }
    int getRelicIDNum() const { return 45; }
    ~matryoshka() {}
};
class meatOnTheBone : public relic
{
private:
public:
    meatOnTheBone() : relic(relicRarity::Uncommon, "Meat On The Bone") {}
    void execute();
    void triggered();
    void fightStartSetup();
    meatOnTheBone* clone() const { return new meatOnTheBone(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<meatOnTheBone>(); }
    int getRelicIDNum() const { return 46; }
    ~meatOnTheBone() {}
};
class mercuryHourglass : public relic
{
private:
public:
    mercuryHourglass() : relic(relicRarity::Uncommon, "Mercury Hourglass") {}
    void turnStartSetup();
    mercuryHourglass* clone() const { return new mercuryHourglass(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<mercuryHourglass>(); }
    int getRelicIDNum() const { return 47; }
    ~mercuryHourglass() {}
};
class moltenEgg : public relic
{
private:
public:
    moltenEgg() : relic(relicRarity::Uncommon, "Molten Egg") {}
    void execute();
    void triggered();
    void onPickUp();
    moltenEgg* clone() const { return new moltenEgg(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<moltenEgg>(); }
    int getRelicIDNum() const { return 48; }
    ~moltenEgg() {}
};
class mummifiedHand : public relic
{
private:
public:
    mummifiedHand() : relic(relicRarity::Uncommon, "Mummified Hand") {}
    void execute();
    void triggered();
    void fightStartSetup();
    mummifiedHand* clone() const { return new mummifiedHand(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<mummifiedHand>(); }
    int getRelicIDNum() const { return 49; }
    ~mummifiedHand() {}
};
class ornamentalFan : public relic
{
private:
    int attackCounter{ 0 };
public:
    ornamentalFan(int aC = 0) : attackCounter(aC), relic(relicRarity::Uncommon, "Ornamental Fan") {}
    void execute();
    void triggered();
    void turnStartSetup();
    int getCounter() { return attackCounter; }
    void fightStartSetup();
    ornamentalFan* clone() const { return new ornamentalFan(attackCounter); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<ornamentalFan>(); }
    int getRelicIDNum() const { return 50; }
    ~ornamentalFan() {}
};
class pantograph : public relic
{
private:
public:
    pantograph() : relic(relicRarity::Uncommon, "Pantograph", 5) {}
    void fightStartSetup();
    pantograph* clone() const { return new pantograph(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<pantograph>(); }
    int getRelicIDNum() const { return 51; }
    ~pantograph() {}
};
class pear : public relic
{
private:
public:
    pear() : relic(relicRarity::Uncommon, "Pear") {}
    void onPickUp();
    pear* clone() const { return new pear(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<pear>(); }
    int getRelicIDNum() const { return 52; }
    ~pear() {}
};
class questionCard : public relic
{
private:
public:
    questionCard() : relic(relicRarity::Uncommon, "Question Card") {}
    void onPickUp();
    void onDropped();
    questionCard* clone() const { return new questionCard(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<questionCard>(); }
    int getRelicIDNum() const { return 53; }
    ~questionCard() {}
};
class shuriken : public relic
{
private:
    int attackCounter{ 0 };
public:
    shuriken(int aC = 0) : attackCounter(aC), relic(relicRarity::Uncommon, "Shuriken") {}
    void execute();
    void triggered();
    void turnStartSetup();
    int getCounter() { return attackCounter; }
    void fightStartSetup();
    shuriken* clone() const { return new shuriken(attackCounter); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<shuriken>(); }
    int getRelicIDNum() const { return 54; }
    ~shuriken() {}
};
class singingBowl : public relic
{
private:
public:
    singingBowl() : relic(relicRarity::Uncommon, "Singing Bowl") {}
    void onPickUp();
    void onDropped();
    singingBowl* clone() const { return new singingBowl(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<singingBowl>(); }
    int getRelicIDNum() const { return 55; }
    ~singingBowl() {}
};
class strikeDummy : public relic
{
private:
public:
    strikeDummy() : relic(relicRarity::Uncommon, "Strike Dummy") {}
    void onPickUp();
    void onDropped();
    strikeDummy* clone() const { return new strikeDummy(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<strikeDummy>(); }
    int getRelicIDNum() const { return 56; }
    ~strikeDummy() {}
};
class sundial : public relic
{
private:
    int shuffleCounter{ 0 };
public:
    sundial(int sC = 0) : shuffleCounter(sC), relic(relicRarity::Uncommon, "Sundial") {}
    void execute();
    void triggered();
    void randomizeCounter() { shuffleCounter = random_int(0, 1); }
    int getCounter() { return shuffleCounter; }
    float getCounterReward() { return powf(static_cast<float>(shuffleCounter) / 2, 2); }
    void fightStartSetup();
    sundial* clone() const { return new sundial(shuffleCounter); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<sundial>(); }
    int getRelicIDNum() const { return 57; }
    ~sundial() {}
};
class theCourier : public relic
{
private:
public:
    theCourier() : relic(relicRarity::Uncommon, "The Courier") {}
    void onPickUp();
    void onDropped();
    theCourier* clone() const { return new theCourier(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<theCourier>(); }
    int getRelicIDNum() const { return 58; }
    ~theCourier() {}
};
class toxicEgg : public relic
{
private:
public:
    toxicEgg() : relic(relicRarity::Uncommon, "Toxic Egg") {}
    void execute();
    void onPickUp();
    void triggered();
    toxicEgg* clone() const { return new toxicEgg(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<toxicEgg>(); }
    int getRelicIDNum() const { return 59; }
    ~toxicEgg() {}
};
class whiteBeastStatue : public relic
{
private:
public:
    whiteBeastStatue() : relic(relicRarity::Uncommon, "White Beast Statue") {}
    void onPickUp();
    void onDropped();
    whiteBeastStatue* clone() const { return new whiteBeastStatue(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<whiteBeastStatue>(); }
    int getRelicIDNum() const { return 60; }
    ~whiteBeastStatue() {}
};
// ############################
// ############################
// ############################
class birdFacedUrn : public relic
{
private:
public:
    birdFacedUrn() : relic(relicRarity::Rare, "Bird Faced Urn") {}
    void execute();
    void triggered();
    void fightStartSetup();
    birdFacedUrn* clone() const { return new birdFacedUrn(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<birdFacedUrn>(); }
    int getRelicIDNum() const { return 61; }
    ~birdFacedUrn() {}
};
class captainsWheel : public relic
{
private:
    int turnCount{ 0 };
public:
    captainsWheel(int tC = 0) : turnCount(tC), relic(relicRarity::Rare, "Captain's Wheel", 90) {}
    void fightStartSetup();
    void turnStartSetup();
    int getCounter() { return turnCount; }
    captainsWheel* clone() const { return new captainsWheel(turnCount); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<captainsWheel>(); }
    int getRelicIDNum() const { return 62; }
    ~captainsWheel() {}
};
class calipers : public relic
{
private:
public:
    calipers() : relic(relicRarity::Rare, "Calipers") {}
    void fightStartSetup();
    calipers* clone() const { return new calipers(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<calipers>(); }
    int getRelicIDNum() const { return 63; }
    ~calipers() {}
};
class deadBranch : public relic
{
private:
public:
    deadBranch() : relic(relicRarity::Rare, "Dead Branch") {}
    void execute();
    void triggered();
    void fightStartSetup();
    deadBranch* clone() const { return new deadBranch(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<deadBranch>(); }
    int getRelicIDNum() const { return 64; }
    ~deadBranch() {}
};
class duVuDoll : public relic
{
private:
    int curseCounter{ 0 };
public:
    duVuDoll(int cC = 0) : curseCounter(cC), relic(relicRarity::Rare, "Du-Vu Doll") {}
    void fightStartSetup();
    int getCounter() { return curseCounter; }
    duVuDoll* clone() const { return new duVuDoll(curseCounter); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<duVuDoll>(); }
    int getRelicIDNum() const { return 65; }
    ~duVuDoll() {}
};
class fossilizedHelix : public relic
{
private:
public:
    fossilizedHelix() : relic(relicRarity::Rare, "Fossilized Helix") {}
    void fightStartSetup();
    fossilizedHelix* clone() const { return new fossilizedHelix(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<fossilizedHelix>(); }
    int getRelicIDNum() const { return 66; }
    ~fossilizedHelix() {}
};
class gamblingChip : public relic
{
private:
public:
    gamblingChip() : relic(relicRarity::Rare, "Gambling Chip", 900) {}
    void turnStartSetup();
    gamblingChip* clone() const { return new gamblingChip(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<gamblingChip>(); }
    int getRelicIDNum() const { return 67; }
    ~gamblingChip() {}
};
class ginger : public relic
{
private:
public:
    ginger() : relic(relicRarity::Rare, "Ginger") {}
    void execute();
    void triggered();
    void fightStartSetup();
    ginger* clone() const { return new ginger(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<ginger>(); }
    int getRelicIDNum() const { return 68; }
    ~ginger() {}
};
class girya : public relic
{
private:
    int strengthNumber{ 0 };
public:
    girya(int sN = 0) : strengthNumber(sN), relic(relicRarity::Rare, "Girya") {}
    void onPickUp();
    void onDropped();
    void randomizeCounter() { strengthNumber = random_int(0, 3); }
    void incrStr() { ++strengthNumber; }
    int getCounter() { return strengthNumber; }
    void fightStartSetup();
    girya* clone() const { return new girya(strengthNumber); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<girya>(); }
    int getRelicIDNum() const { return 69; }
    ~girya() {}
};
class iceCream : public relic
{
private:
public:
    iceCream() : relic(relicRarity::Rare, "Ice Cream") {}
    void onPickUp();
    void onDropped();
    iceCream* clone() const { return new iceCream(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<iceCream>(); }
    int getRelicIDNum() const { return 70; }
    ~iceCream() {}
};
class incenseBurner : public relic
{
private:
    int turnCount{ 0 };
public:
    incenseBurner(int tC = 0) : turnCount(tC), relic(relicRarity::Rare, "Incense Burner") {}
    int getCounter() { return turnCount; }
    void randomizeCounter() { turnCount = random_int(0, 5); }
    float getCounterReward() { return powf(static_cast<float>(turnCount) / 5, 2); }
    void turnStartSetup();
    incenseBurner* clone() const { return new incenseBurner(turnCount); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<incenseBurner>(); }
    int getRelicIDNum() const { return 71; }
    ~incenseBurner() {}
};
class lizardTail : public relic
{
private:
    bool usedUp{ false };
public:
    lizardTail(bool uU = false) : usedUp(uU), relic(relicRarity::Rare, "Lizard Tail") {}
    void execute();
    void onPickUp();
    void triggered();
    int getCounter() { return !usedUp; }
    lizardTail* clone() const { return new lizardTail(usedUp); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<lizardTail>(); }
    int getRelicIDNum() const { return 72; }
    ~lizardTail() {}
};
class mango : public relic
{
private:
public:
    mango() : relic(relicRarity::Rare, "Mango") {}
    void onPickUp();
    mango* clone() const { return new mango(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<mango>(); }
    int getRelicIDNum() const { return 73; }
    ~mango() {}
};
class oldCoin : public relic
{
private:
public:
    oldCoin() : relic(relicRarity::Rare, "Old Coin") {}
    void onPickUp();
    oldCoin* clone() const { return new oldCoin(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<oldCoin>(); }
    int getRelicIDNum() const { return 74; }
    ~oldCoin() {}
};
class peacePipe : public relic
{
private:
public:
    peacePipe() : relic(relicRarity::Rare, "Peace Pipe") {}
    void onPickUp();
    void onDropped();
    peacePipe* clone() const { return new peacePipe(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<peacePipe>(); }
    int getRelicIDNum() const { return 75; }
    ~peacePipe() {}
};
class pocketwatch : public relic
{
private:
    int cardCounter{ 0 };
public:
    pocketwatch(int cC = 0) : cardCounter(cC), relic(relicRarity::Rare, "Pocketwatch") {}
    void execute();
    void triggered();
    void turnStartSetup();
    void fightStartSetup();
    int getCounter() { return cardCounter; }
    pocketwatch* clone() const { return new pocketwatch(cardCounter); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<pocketwatch>(); }
    int getRelicIDNum() const { return 76; }
    ~pocketwatch() {}
};
class prayerWheel : public relic
{
private:
public:
    prayerWheel() : relic(relicRarity::Rare, "Prayer Wheel") {}
    void onPickUp();
    void onDropped();
    prayerWheel* clone() const { return new prayerWheel(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<prayerWheel>(); }
    int getRelicIDNum() const { return 77; }
    ~prayerWheel() {}
};
class shovel : public relic
{
private:
public:
    shovel() : relic(relicRarity::Rare, "Shovel") {}
    void onDropped();
    void onPickUp();
    shovel* clone() const { return new shovel(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<shovel>(); }
    int getRelicIDNum() const { return 78; }
    ~shovel() {}
};
class stoneCalendar : public relic
{
private:
    int turnCount{ 0 };
public:
    stoneCalendar(int tC = 0) : turnCount(tC), relic(relicRarity::Rare, "Stone Calendar") {}
    void execute();
    void triggered();
    void turnStartSetup();
    void fightStartSetup();
    int getCounter() { return turnCount; }
    stoneCalendar* clone() const { return new stoneCalendar(turnCount); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<stoneCalendar>(); }
    int getRelicIDNum() const { return 79; }
    ~stoneCalendar() {}
};
class threadAndNeedle : public relic
{
private:
public:
    threadAndNeedle() : relic(relicRarity::Rare, "Thread And Needle") {}
    void fightStartSetup();
    threadAndNeedle* clone() const { return new threadAndNeedle(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<threadAndNeedle>(); }
    int getRelicIDNum() const { return 80; }
    ~threadAndNeedle() {}
};
class torii : public relic
{
private:
public:
    torii() : relic(relicRarity::Rare, "Torii") {}
    void execute();
    void triggered();
    void fightStartSetup();
    torii* clone() const { return new torii(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<torii>(); }
    int getRelicIDNum() const { return 81; }
    ~torii() {}
};
class tungsten : public relic
{
private:
public:
    tungsten() : relic(relicRarity::Rare, "Tungsten") {}
    void execute();
    void onPickUp();
    void onDropped();
    void triggered();
    void fightStartSetup();
    tungsten* clone() const { return new tungsten(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<tungsten>(); }
    int getRelicIDNum() const { return 82; }
    ~tungsten() {}
};
class turnip : public relic
{
private:
public:
    turnip() : relic(relicRarity::Rare, "Turnip") {}
    void execute();
    void triggered();
    void fightStartSetup();
    turnip* clone() const { return new turnip(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<turnip>(); }
    int getRelicIDNum() const { return 83; }
    ~turnip() {}
};
class unceasingTop : public relic
{
private:
public:
    unceasingTop() : relic(relicRarity::Rare, "Unceasing Top", 10000000) {}
    void execute();
    void triggered();
    void fightStartSetup();
    unceasingTop* clone() const { return new unceasingTop(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<unceasingTop>(); }
    int getRelicIDNum() const { return 84; }
    ~unceasingTop() {}
};
// ######################
class cauldron : public relic
{
private:
public:
    cauldron() : relic(relicRarity::Shop, "Cauldron") {}
    void onPickUp();
    cauldron* clone() const { return new cauldron(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<cauldron>(); }
    int getRelicIDNum() const { return 85; }
    ~cauldron() {}
};
class chemicalX : public relic
{
private:
public:
    chemicalX() : relic(relicRarity::Shop, "Chemical X") {}
    void onPickUp();
    void onDropped();
    chemicalX* clone() const { return new chemicalX(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<chemicalX>(); }
    int getRelicIDNum() const { return 86; }
    ~chemicalX() {}
};
class clockworkSouvenir : public relic
{
private:
public:
    clockworkSouvenir() : relic(relicRarity::Shop, "Clockwork Souvenir", 50) {}
    void fightStartSetup();
    clockworkSouvenir* clone() const { return new clockworkSouvenir(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<clockworkSouvenir>(); }
    int getRelicIDNum() const { return 87; }
    ~clockworkSouvenir() {}
};
class dollysMirror : public relic
{
private:
public:
    dollysMirror() : relic(relicRarity::Shop, "Dolly's Mirror") {}
    void onPickUp();
    dollysMirror* clone() const { return new dollysMirror(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<dollysMirror>(); }
    int getRelicIDNum() const { return 88; }
    ~dollysMirror() {}
};
class frozenEye : public relic
{
private:
public:
    frozenEye() : relic(relicRarity::Shop, "Frozen Eye") {}
    void onPickUp();
    void onDropped();
    frozenEye* clone() const { return new frozenEye(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<frozenEye>(); }
    int getRelicIDNum() const { return 89; }
    ~frozenEye() {}
};
class handDrill : public relic
{
private:
public:
    handDrill() : relic(relicRarity::Shop, "Hand Drill") {}
    void execute();
    void triggered();
    void fightStartSetup();
    handDrill* clone() const { return new handDrill(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<handDrill>(); }
    int getRelicIDNum() const { return 90; }
    ~handDrill() {}
};
class leesWaffle : public relic
{
private:
public:
    leesWaffle() : relic(relicRarity::Shop, "Lee's Waffle") {}
    void onPickUp();
    leesWaffle* clone() const { return new leesWaffle(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<leesWaffle>(); }
    int getRelicIDNum() const { return 91; }
    ~leesWaffle() {}
};
class medKit : public relic
{
private:
public:
    medKit() : relic(relicRarity::Shop, "Med Kit") {}
    void onPickUp();
    void onDropped();
    medKit* clone() const { return new medKit(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<medKit>(); }
    int getRelicIDNum() const { return 92; }
    ~medKit() {}
};
class membershipCard : public relic
{
private:
public:
    membershipCard() : relic(relicRarity::Shop, "Membership Card") {}
    void onPickUp();
    void onDropped();
    membershipCard* clone() const { return new membershipCard(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<membershipCard>(); }
    int getRelicIDNum() const { return 93; }
    ~membershipCard() {}
};
class orangePellets : public relic
{
private:
    bool pwr;
    bool att;
    bool skl;
public:
    orangePellets(bool p = false, bool a = false, bool s = false) : pwr(p), att(a), skl(s), relic(relicRarity::Shop, "Orange Pellets") { triggerPrio = 1500; }
    void execute();
    void triggered(cardHolder&);
    void fightStartSetup();
    int getCounter() { return pwr + att + skl; }
    void turnStartSetup();
    orangePellets* clone() const { return new orangePellets(pwr,att,skl); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<orangePellets>(); }
    int getRelicIDNum() const { return 94; }
    ~orangePellets() {}
};
class orrery : public relic
{
private:
public:
    orrery() : relic(relicRarity::Shop, "Orrery") {}
    void onPickUp();
    orrery* clone() const { return new orrery(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<orrery>(); }
    int getRelicIDNum() const { return 95; }
    ~orrery() {}
};
class slingOfCourage : public relic
{
private:
public:
    slingOfCourage() : relic(relicRarity::Shop, "Sling Of Courage") {}
    void execute() {}
    void triggered() {}
    void fightStartSetup();
    slingOfCourage* clone() const { return new slingOfCourage(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<slingOfCourage>(); }
    int getRelicIDNum() const { return 96; }
    ~slingOfCourage() {}
};
class strangeSpoon : public relic
{
private:
public:
    strangeSpoon() : relic(relicRarity::Shop, "Strange Spoon") {}
    void onPickUp();
    void onDropped();
    strangeSpoon* clone() const { return new strangeSpoon(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<strangeSpoon>(); }
    int getRelicIDNum() const { return 97; }
    ~strangeSpoon() {}
};
class theAbacus : public relic
{
private:
public:
    theAbacus() : relic(relicRarity::Shop, "The Abacus") {}
    void execute();
    void triggered();
    void fightStartSetup();
    theAbacus* clone() const { return new theAbacus(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<theAbacus>(); }
    int getRelicIDNum() const { return 98; }
    ~theAbacus() {}
};
// #####################
class bloodyIdol : public relic
{
private:
public:
    bloodyIdol() : relic(relicRarity::Event, "Bloody Idol") {}
    void triggered();
    void onPickUp();
    bloodyIdol* clone() const { return new bloodyIdol(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<bloodyIdol>(); }
    int getRelicIDNum() const { return 99; }
    ~bloodyIdol() {}
};
class cultistHeadpiece : public relic
{
private:
public:
    cultistHeadpiece() : relic(relicRarity::Event, "Cultist Headpiece") {}
    cultistHeadpiece* clone() const { return new cultistHeadpiece(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<cultistHeadpiece>(); }
    int getRelicIDNum() const { return 100; }
    ~cultistHeadpiece() {}
};
class faceOfCleric : public relic
{
private:
public:
    faceOfCleric() : relic(relicRarity::Event, "Face Of Cleric") {}
    void triggered();
    void fightStartSetup();
    faceOfCleric* clone() const { return new faceOfCleric(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<faceOfCleric>(); }
    int getRelicIDNum() const { return 101; }
    ~faceOfCleric() {}
};
class goldenIdol : public relic
{
private:
public:
    goldenIdol() : relic(relicRarity::Event, "Golden Idol") {}
    void onPickUp();
    void onDropped();
    goldenIdol* clone() const { return new goldenIdol(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<goldenIdol>(); }
    int getRelicIDNum() const { return 102; }
    ~goldenIdol() {}
};
class gremlinVisage : public relic
{
private:
public:
    gremlinVisage() : relic(relicRarity::Event, "Gremlin Visage", 111) {}
    void fightStartSetup();
    gremlinVisage* clone() const { return new gremlinVisage(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<gremlinVisage>(); }
    int getRelicIDNum() const { return 103; }
    ~gremlinVisage() {}
};
class neowsLament : public relic
{
private:
    int fightCount{ 3 };
public:
    neowsLament(int fC = 3) : fightCount(fC), relic(relicRarity::Event, "Neow's Lament") {}
    void execute();
    int getCounter() { return fightCount; }
    void fightStartSetup();
    neowsLament* clone() const { return new neowsLament(fightCount); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<neowsLament>(); }
    int getRelicIDNum() const { return 104; }
    ~neowsLament() {}
};
class nlothsHungryFace : public relic
{
private:
public:
    nlothsHungryFace() : relic(relicRarity::Event, "N'loth's Hungry Face") {}
    void onPickUp();
    void onDropped();
    nlothsHungryFace* clone() const { return new nlothsHungryFace(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<nlothsHungryFace>(); }
    int getRelicIDNum() const { return 105; }
    ~nlothsHungryFace() {}
};
class oddMushroom : public relic
{
private:
public:
    oddMushroom( ) : relic(relicRarity::Event, "Odd Mushroom") {}
    void onPickUp();
    void onDropped();
    oddMushroom* clone() const { return new oddMushroom(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<oddMushroom>(); }
    int getRelicIDNum() const { return 106; }
    ~oddMushroom() {}
};
class ssserpentHead : public relic
{
private:
public:
    ssserpentHead() : relic(relicRarity::Event, "Ssserpent Head") {}
    void onPickUp();
    void triggered();
    ssserpentHead* clone() const { return new ssserpentHead(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<ssserpentHead>(); }
    int getRelicIDNum() const { return 107; }
    ~ssserpentHead() {}
};
class warpedTongs : public relic
{
private:
public:
    warpedTongs() : relic(relicRarity::Event, "Warped Tongs") {}
    void turnStartSetup();
    warpedTongs* clone() const { return new warpedTongs(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<warpedTongs>(); }
    int getRelicIDNum() const { return 108; }
    ~warpedTongs() {}
};

class burningBlood : public relic
{
private:
public:
    burningBlood() : relic(relicRarity::Event, "Burning Blood") {}
    void triggered();
    void fightStartSetup();
    burningBlood* clone() const { return new burningBlood(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<burningBlood>(); }
    int getRelicIDNum() const { return 109; }
    ~burningBlood() {}
};

class redSkull : public relic
{
private:
public:
    redSkull() : relic(relicRarity::Common, "Red Skull") {}
    void triggered();
    void fightStartSetup();
    redSkull* clone() const { return new redSkull(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<redSkull>(); }
    int getRelicIDNum() const { return 110; }
    ~redSkull() {}
};
class paperPhrog : public relic
{
private:
public:
    paperPhrog() : relic(relicRarity::Uncommon, "Paper Phrog") {}
    void onPickUp();
    void onDropped();
    paperPhrog* clone() const { return new paperPhrog(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<paperPhrog>(); }
    int getRelicIDNum() const { return 111; }
    ~paperPhrog() {}
};
class selfFormingClay : public relic
{
private:
    int trigCount{ 0 };
public:
    selfFormingClay(int tC = 0) : trigCount(tC), relic(relicRarity::Uncommon, "Self-Forming Clay") {}
    void triggered();
    void turnStartSetup();
    void fightStartSetup();
    int getCounter() { return trigCount; }
    selfFormingClay* clone() const { return new selfFormingClay(trigCount); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<selfFormingClay>(); }
    int getRelicIDNum() const { return 112; }
    ~selfFormingClay() {}
};
class championsBelt : public relic
{
private:
public:
    championsBelt() : relic(relicRarity::Rare, "Champion's Belt") {}
    void triggered(character&);
    void fightStartSetup();
    championsBelt* clone() const { return new championsBelt(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<championsBelt>(); }
    int getRelicIDNum() const { return 113; }
    ~championsBelt() {}
};
class charonsAshes : public relic
{
private:
public:
    charonsAshes() : relic(relicRarity::Rare, "Charon's Ashes") {}
    void triggered();
    void fightStartSetup();
    charonsAshes* clone() const { return new charonsAshes(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<charonsAshes>(); }
    int getRelicIDNum() const { return 114; }
    ~charonsAshes() {}
};
class magicFlower : public relic
{
private:
public:
    magicFlower() : relic(relicRarity::Rare, "Magic Flower") {}
    void onPickUp();
    void onDropped();
    magicFlower* clone() const { return new magicFlower(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<magicFlower>(); }
    int getRelicIDNum() const { return 115; }
    ~magicFlower() {}
};
class brimstone : public relic
{
private:
public:
    brimstone() : relic(relicRarity::Shop, "Brimstone") {}
    void turnStartSetup();
    brimstone* clone() const { return new brimstone(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<brimstone>(); }
    int getRelicIDNum() const { return 116; }
    ~brimstone() {}
};

class astrolabe : public relic, public bossRelic
{
private:
public:
    astrolabe() : relic(relicRarity::Boss, "Astrolabe"), bossRelic(4, 3, 0, 0) {}
    void onPickUp();
    astrolabe* clone() const { return new astrolabe(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<astrolabe>(); }
    int getRelicIDNum() const { return 117; }
    ~astrolabe() {}
};
class blackStar : public relic, public bossRelic
{
private:
public:
    blackStar();
    void triggered();
    void fightStartSetup();
    blackStar* clone() const { return new blackStar(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<blackStar>(); }
    int getRelicIDNum() const { return 118; }
    ~blackStar() {}
};
class bustedCrown : public relic, public bossRelic
{
private:
public:
    bustedCrown() : relic(relicRarity::Boss, "Busted Crown"), bossRelic(1, 2, 1, 0) {}
    void onPickUp();
    void onDropped();
    bustedCrown* clone() const { return new bustedCrown(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<bustedCrown>(); }
    int getRelicIDNum() const { return 119; }
    ~bustedCrown() {}
};
class callingBell : public relic, public bossRelic
{
private:
public:
    callingBell() : relic(relicRarity::Boss, "Calling Bell"), bossRelic(3, 3, 0, 0) {}
    void onPickUp();
    callingBell* clone() const { return new callingBell(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<callingBell>(); }
    int getRelicIDNum() const { return 120; }
    ~callingBell() {}
};
class coffeeDripper : public relic, public bossRelic
{
private:
public:
    coffeeDripper() : relic(relicRarity::Boss, "Coffee Dripper"), bossRelic(3, 4, 1, 0) {}
    void onPickUp();
    void onDropped();
    coffeeDripper* clone() const { return new coffeeDripper(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<coffeeDripper>(); }
    int getRelicIDNum() const { return 121; }
    ~coffeeDripper() {}
};
class cursedKey : public relic, public bossRelic
{
private:
public:
    cursedKey();
    void triggered();
    void onPickUp();
    void onDropped();
    cursedKey* clone() const { return new cursedKey(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<cursedKey>(); }
    int getRelicIDNum() const { return 122; }
    ~cursedKey() {}
};
class ectoplasm : public relic, public bossRelic
{
private:
public:
    ectoplasm() : relic(relicRarity::Boss, "Ectoplasm"), bossRelic(2, 3, 1, 0) {}
    void onPickUp();
    void onDropped();
    ectoplasm* clone() const { return new ectoplasm(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<ectoplasm>(); }
    int getRelicIDNum() const { return 123; }
    ~ectoplasm() {}
};
class emptyCage : public relic, public bossRelic
{
private:
public:
    emptyCage() : relic(relicRarity::Boss, "Empty Cage"), bossRelic(2, 2, 0, 0) {}
    void onPickUp();
    emptyCage* clone() const { return new emptyCage(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<emptyCage>(); }
    int getRelicIDNum() const { return 124; }
    ~emptyCage() {}
};
class fusionHammer : public relic, public bossRelic
{
private:
public:
    fusionHammer() : relic(relicRarity::Boss, "Fusion Hammer"), bossRelic(2, 4, 1, 0) {}
    void onPickUp();
    void onDropped();
    fusionHammer* clone() const { return new fusionHammer(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<fusionHammer>(); }
    int getRelicIDNum() const { return 125; }
    ~fusionHammer() {}
};
class pandorasBox : public relic, public bossRelic
{
private:
public:
    pandorasBox() : relic(relicRarity::Boss, "Pandora's Box"), bossRelic(4, 2, 0, 0) {}
    void onPickUp();
    pandorasBox* clone() const { return new pandorasBox(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<pandorasBox>(); }
    int getRelicIDNum() const { return 126; }
    ~pandorasBox() {}
};
class philosophersStone : public relic, public bossRelic
{
private:
public:
    philosophersStone() : relic(relicRarity::Boss, "Philosopher's Stone"), bossRelic(3, 3, 1, 0) {}
    void onPickUp();
    void onDropped();
    void fightStartSetup();
    philosophersStone* clone() const { return new philosophersStone(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<philosophersStone>(); }
    int getRelicIDNum() const { return 127; }
    ~philosophersStone() {}
};
class runicPyramid : public relic, public bossRelic
{
private:
public:
    runicPyramid();
    void onPickUp();
    void onDropped();
    runicPyramid* clone() const { return new runicPyramid(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<runicPyramid>(); }
    int getRelicIDNum() const { return 128; }
    ~runicPyramid() {}
};
class slaversCollar : public relic, public bossRelic
{
private:
public:
    slaversCollar() : relic(relicRarity::Boss, "Slaver's Collar"), bossRelic(4, 4, 1, 0) {}
    void turnStartSetup();
    slaversCollar* clone() const { return new slaversCollar(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<slaversCollar>(); }
    int getRelicIDNum() const { return 129; }
    ~slaversCollar() {}
};
class sneckoEye : public relic, public bossRelic
{
private:
public:
    sneckoEye();
    void onDropped();
    void onPickUp();
    sneckoEye* clone() const { return new sneckoEye(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<sneckoEye>(); }
    int getRelicIDNum() const { return 130; }
    ~sneckoEye() {}
};
class sozu : public relic, public bossRelic
{
private:
public:
    sozu() : relic(relicRarity::Boss, "Sozu"), bossRelic(2, 3, 1, 0) {}
    void onPickUp();
    void onDropped();
    sozu* clone() const { return new sozu(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<sozu>(); }
    int getRelicIDNum() const { return 131; }
    ~sozu() {}
};
class tinyHouse : public relic, public bossRelic
{
private:
public:
    tinyHouse() : relic(relicRarity::Boss, "Tiny House"), bossRelic(2, 2, 0, 0) {}
    void onPickUp();
    tinyHouse* clone() const { return new tinyHouse(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<tinyHouse>(); }
    int getRelicIDNum() const { return 132; }
    ~tinyHouse() {}
};
class velvetChoker : public relic, public bossRelic
{
private:
    int cardCounter{0};
public:
    velvetChoker(int cC = 0) : cardCounter(cC), relic(relicRarity::Boss, "Velvet Choker"), bossRelic(3, 2, 1, 0) {}
    void triggered();
    void turnStartSetup();
    void fightStartSetup();
    void onPickUp();
    void onDropped();
    int getCounter() { return cardCounter; }
    velvetChoker* clone() const { return new velvetChoker(cardCounter); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<velvetChoker>(); }
    int getRelicIDNum() const { return 133; }
    ~velvetChoker() {}
};

class mutagenicStrength : public relic
{
private:
public:
    mutagenicStrength() : relic(relicRarity::Event, "Mutagenic Strength") {}
    void fightStartSetup();
    mutagenicStrength* clone() const { return new mutagenicStrength(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<mutagenicStrength>(); }
    int getRelicIDNum() const { return 134; }
    ~mutagenicStrength() {}
};
class redMask : public relic
{
private:
public:
    redMask() : relic(relicRarity::Event, "Red Mask") {}
    void fightStartSetup();
    redMask* clone() const { return new redMask(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<redMask>(); }
    int getRelicIDNum() const { return 135; }
    ~redMask() {}
};

class enchridion : public relic
{
private:
public:
    enchridion() : relic(relicRarity::Event, "Enchridion") {}
    void fightStartSetup();
    enchridion* clone() const { return new enchridion(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<enchridion>(); }
    int getRelicIDNum() const { return 136; }
    ~enchridion() {}
};
class nilrysCodex : public relic
{
private:
public:
    nilrysCodex() : relic(relicRarity::Event, "Nilry's Codex") {}
    void fightStartSetup();
    void triggered();
    nilrysCodex* clone() const { return new nilrysCodex(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<nilrysCodex>(); }
    int getRelicIDNum() const { return 137; }
    ~nilrysCodex() {}
};
class necronomicon : public relic
{
private:
    bool thisTurn{ false };
public:
    necronomicon(bool tT = false ) : thisTurn(tT), relic(relicRarity::Event, "Necronomicon") { triggerPrio = 100001; }
    void onPickUp();
    void onDropped();
    void fightStartSetup();
    void turnStartSetup();
    void triggered(cardHolder& ch);
    int getCounter() { return !thisTurn; }
    necronomicon* clone() const { return new necronomicon(thisTurn); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<necronomicon>(); }
    int getRelicIDNum() const { return 138; }
    ~necronomicon() {}
};

class wingBoots : public relic
{
private:
    int* flyCount;
public:
    wingBoots();
    void onDropped();
    void onPickUp();
    wingBoots* clone() const { return new wingBoots(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<wingBoots>(); }
    int getCounter() { return *flyCount; }
    int getRelicIDNum() const { return 139; }
    ~wingBoots() {}
};
class toolbox : public relic
{
private:
public:
    toolbox() : relic(relicRarity::Shop, "Toolbox") { }
    void fightStartSetup();
    void triggered();
    toolbox* clone() const { return new toolbox(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<toolbox>(); }
    int getRelicIDNum() const { return 140; }
    ~toolbox() {}
};
class nlothsGift : public relic
{
private:
public:
    nlothsGift() : relic(relicRarity::Event, "Nloth's Gift") { }
    void onPickUp();
    void onDropped();
    nlothsGift* clone() const { return new nlothsGift(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<nlothsGift>(); }
    int getRelicIDNum() const { return 141; }
    ~nlothsGift() {}
};
class markOfTheBloom : public relic
{
private:
public:
    markOfTheBloom() : relic(relicRarity::Event, "Mark Of The Bloom") { }
    void onPickUp();
    void onDropped();
    markOfTheBloom* clone() const { return new markOfTheBloom(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<markOfTheBloom>(); }
    int getRelicIDNum() const { return 142; }
    ~markOfTheBloom() {}
};

class blackBlood : public relic, public bossRelic
{
private:
public:
    blackBlood() : relic(relicRarity::Boss, "Black Blood"), bossRelic(2, 2, 0, 0) {}
    void triggered();
    void fightStartSetup();
    void onPickUp();
    blackBlood* clone() const { return new blackBlood(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<blackBlood>(); }
    int getRelicIDNum() const { return 143; }
    ~blackBlood() {}
};
class markOfPain : public relic, public bossRelic
{
private:
public:
    markOfPain() : relic(relicRarity::Boss, "Mark Of Pain"), bossRelic(3, 2, 1, 0) {}
    void onPickUp();
    void onDropped();
    markOfPain* clone() const { return new markOfPain(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<markOfPain>(); }
    int getRelicIDNum() const { return 144; }
    ~markOfPain() {}
};
class runicCube : public relic, public bossRelic
{
private:
public:
    runicCube() : relic(relicRarity::Boss, "Runic Cube"), bossRelic(2, 2, 1, 0) {}
    void triggered();
    void fightStartSetup();
    runicCube* clone() const { return new runicCube(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<runicCube>(); }
    int getRelicIDNum() const { return 145; }
    ~runicCube() {}
};
class sacredBark : public relic
{
private:
public:
    sacredBark() : relic(relicRarity::Boss, "Sacred Bark") {}
    void triggered();
    void fightStartSetup();
    sacredBark* clone() const { return new sacredBark(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<sacredBark>(); }
    int getRelicIDNum() const { return 146; }
    ~sacredBark() {}
};
class runicDome : public relic
{
private:
public:
    runicDome() : relic(relicRarity::Boss, "Runic Dome") {}
    void triggered();
    void fightStartSetup();
    runicDome* clone() const { return new runicDome(); }
    std::unique_ptr<relic> cloneU() const { return std::make_unique<runicDome>(); }
    int getRelicIDNum() const { return 147; }
    ~runicDome() {}
};
#endif
