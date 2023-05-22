#ifndef IC_CARDS_H
#define IC_CARDS_H
#include "top.h"
#include "card.h"

class strike : public card
{
private:
    int damageNumber{ 6 };
public:
    strike(bool upgraded) : card(cardType::Attack, cardRarity::Common, 1, "Strike", "Strike", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    strike* clone() const { auto temp = new strike(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<strike>(upgraded_); }
    int getCardIDNum() const { return 1; }
    combatChoices getCombatChoices() const { return combatChoices::EnemyTarget; }
    ~strike() {}
};

class bash : public card
{
private:
    int damageNumber{ 8 };
    int vulnerableNumber{ 2 };
public:
    bash(bool upgraded) : card(cardType::Attack, cardRarity::Common, 2, "Bash", " Bash ", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    bash* clone() const { auto temp = new bash(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<bash>(upgraded_); }
    int getCardIDNum() const { return 2; }
    combatChoices getCombatChoices() const { return combatChoices::EnemyTarget; }
    ~bash() {}
};

class defend : public card
{
private:
    int blockNumber{ 5 };
public:
    defend(bool upgraded) : card(cardType::Skill, cardRarity::Common, 1, "Defend", "Defend", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    defend* clone() const { auto temp = new defend(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<defend>(upgraded_); }
    int getCardIDNum() const { return 3; }
    ~defend() {}
};


class armaments : public card
{
private:
    int blockNumber{ 5 };
    bool targetAll{ false };
public:
    armaments(bool upgraded) : card(cardType::Skill, cardRarity::Common, 1, "Armaments", "Armnts", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    armaments* clone() const { auto temp = new armaments(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<armaments>(upgraded_); }
    int getCardIDNum() const { return 4; }
    combatChoices getCombatChoices() const { return upgraded_ ? combatChoices::None : combatChoices::HandAR; }
    ~armaments() {}
};
class anger : public card
{
private:
    int damageNumber{ 6 };
public:
    anger(bool upgraded) : card(cardType::Attack, cardRarity::Common, 0, "Anger", " Anger", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    anger* clone() const { auto temp = new anger(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<anger>(upgraded_); }
    int getCardIDNum() const { return 5; }
    combatChoices getCombatChoices() const { return combatChoices::EnemyTarget; }
    ~anger() {}
};
class bodyslam : public card
{
private:
    int damageNumber{ 0 };
public:
    bodyslam(bool upgraded) : card(cardType::Attack, cardRarity::Common, 1, "Bodyslam", "Bodysl", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    bodyslam* clone() const { auto temp = new bodyslam(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<bodyslam>(upgraded_); }
    int getCardIDNum() const { return 6; }
    combatChoices getCombatChoices() const { return combatChoices::EnemyTarget; }
    ~bodyslam() {}
};
class clash : public card
{
private:
    int damageNumber{ 14 };
public:
    clash(bool upgraded) : card(cardType::Attack, cardRarity::Common, 0, "Clash", " Clash", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    bool castable();
    clash* clone() const { auto temp = new clash(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<clash>(upgraded_); }
    int getCardIDNum() const { return 7; }
    combatChoices getCombatChoices() const { return combatChoices::EnemyTarget; }
    ~clash() {}
};
class clothesline : public card
{
private:
    int damageNumber{ 12 };
    int weakNumber{ 2 };
public:
    clothesline(bool upgraded) : card(cardType::Attack, cardRarity::Common, 2, "Clothesline", "Clothl", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    clothesline* clone() const { auto temp = new clothesline(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<clothesline>(upgraded_); }
    int getCardIDNum() const { return 8; }
    combatChoices getCombatChoices() const { return combatChoices::EnemyTarget; }
    ~clothesline() {}
};
class flex : public card
{
private:
    int strengthNumber{ 2 };
public:
    flex(bool upgraded) : card(cardType::Skill, cardRarity::Common, 0, "Flex", "  Flex", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    flex* clone() const { auto temp = new flex(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<flex>(upgraded_); }
    int getCardIDNum() const { return 9; }
    ~flex() {}
};
class havoc : public card
{
public:
    havoc(bool upgraded) : card(cardType::Skill, cardRarity::Common, 1, "Havoc", " Havoc", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    static void playCardByHavoc(cardHolder&& ch);
    void execute();
    void upgradeThis();
    havoc* clone() const { auto temp = new havoc(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<havoc>(upgraded_); }
    int getCardIDNum() const { return 10; }
    combatChoices getCombatChoices() const { return combatChoices::Havoc; }
    ~havoc() {}
};

class headbutt : public card
{
private:
    int damageNumber{ 9 };
public:
    headbutt(bool upgraded) : card(cardType::Attack, cardRarity::Common, 1, "Headbutt", "Headbt", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    headbutt* clone() const { auto temp = new headbutt(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<headbutt>(upgraded_); }
    int getCardIDNum() const { return 11; }
    combatChoices getCombatChoices() const { return combatChoices::Headbutt; }
    ~headbutt() {}
};
class heavyBlade : public card
{
private:
    int damageNumber{ 14 };
    int strengthMulti{ 3 };
public:
    heavyBlade(bool upgraded) : card(cardType::Attack, cardRarity::Common, 2, "Heavy Blade", "HvyBld", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    heavyBlade* clone() const { auto temp = new heavyBlade(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<heavyBlade>(upgraded_); }
    int getCardIDNum() const { return 12; }
    combatChoices getCombatChoices() const { return combatChoices::EnemyTarget; }
    ~heavyBlade() {}
};
class ironwave : public card
{
private:
    int damageNumber{ 5 };
    int blockNumber{ 5 };
public:
    ironwave(bool upgraded) : card(cardType::Attack, cardRarity::Common, 1, "Ironwave", "Ironwv", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    ironwave* clone() const { auto temp = new ironwave(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<ironwave>(upgraded_); }
    int getCardIDNum() const { return 13; }
    combatChoices getCombatChoices() const { return combatChoices::EnemyTarget; }
    ~ironwave() {}
};

class perfectedStrike : public card
{
private:
    int damageNumber{ 6 };
    int perStrikeIncrease{ 2 };
public:
    perfectedStrike(bool upgraded) : card(cardType::Attack, cardRarity::Common, 2, "Perfected Strike", "PrfStk", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    perfectedStrike* clone() const { auto temp = new perfectedStrike(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<perfectedStrike>(upgraded_); }
    int getCardIDNum() const { return 14; }
    combatChoices getCombatChoices() const { return combatChoices::EnemyTarget; }
    ~perfectedStrike() {}
};
class pommelStrike : public card
{
private:
    int damageNumber{ 9 };
    int drawNumber{ 1 };
public:
    pommelStrike(bool upgraded) : card(cardType::Attack, cardRarity::Common, 1, "Pommel Strike", "PmlStk", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    pommelStrike* clone() const { auto temp = new pommelStrike(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<pommelStrike>(upgraded_); }
    int getCardIDNum() const { return 15; }
    combatChoices getCombatChoices() const { return combatChoices::EnemyTarget; }
    ~pommelStrike() {}
};
class shrugItOff : public card
{
private:
    int blockNumber{ 8 };
public:
    shrugItOff(bool upgraded) : card(cardType::Skill, cardRarity::Common, 1, "Shrug It Off", " Shrug", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    shrugItOff* clone() const { auto temp = new shrugItOff(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<shrugItOff>(upgraded_); }
    int getCardIDNum() const { return  16; }
    ~shrugItOff() {}
};
class swordBoomerang : public card
{
private:
    int damageNumber{ 3 };
    int multiAttack{ 3 };
public:
    swordBoomerang(bool upgraded) : card(cardType::Attack, cardRarity::Common, 1, "Sword Boomerang", "SBoomr", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    swordBoomerang* clone() const { auto temp = new swordBoomerang(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<swordBoomerang>(upgraded_); }
    int getCardIDNum() const { return 17; }
    ~swordBoomerang() {}
};
class thunderclap : public card
{
private:
    int damageNumber{ 4 };
    int vulnerableNumber{ 1 };
public:
    thunderclap(bool upgraded) : card(cardType::Attack, cardRarity::Common, 1, "Thunderclap", "Thclap", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    thunderclap* clone() const { auto temp = new thunderclap(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<thunderclap>(upgraded_); }
    int getCardIDNum() const { return 18; }
    ~thunderclap() {}
};
class trueGrit : public card
{
private:
    int blockNumber{ 7 };
public:
    trueGrit(bool upgraded) : card(cardType::Skill, cardRarity::Common, 1, "True Grit", "TrueGr", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    trueGrit* clone() const { auto temp = new trueGrit(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<trueGrit>(upgraded_); }
    int getCardIDNum() const { return 19; }
    combatChoices getCombatChoices() const { return upgraded_ ? combatChoices::Hand : combatChoices::None; }
    ~trueGrit() {}
};
class twinStrike : public card
{
private:
    int damageNumber{ 5 };
    int multiAttack{ 2 };
public:
    twinStrike(bool upgraded) : card(cardType::Attack, cardRarity::Common, 1, "Twin Strike", "TwinSt", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    twinStrike* clone() const { auto temp = new twinStrike(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<twinStrike>(upgraded_); }
    int getCardIDNum() const { return 20; }
    combatChoices getCombatChoices() const { return combatChoices::EnemyTarget; }
    ~twinStrike() {}
};
class warCry : public card
{
private:
    int drawNumber{ 1 };
public:
    warCry(bool upgraded) : card(cardType::Skill, cardRarity::Common, 0, "War Cry", "WarCry", false, upgraded, false, true)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    warCry* clone() const { auto temp = new warCry(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<warCry>(upgraded_); }
    int getCardIDNum() const { return 21; }
    combatChoices getCombatChoices() const { return upgraded_ ? combatChoices::HandTA : combatChoices::HandWC; }
    ~warCry() {}
};
class wildStrike : public card
{
private:
    int damageNumber{ 12 };
public:
    wildStrike(bool upgraded) : card(cardType::Attack, cardRarity::Common, 1, "Wild Strike", "WildSt", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    wildStrike* clone() const { auto temp = new wildStrike(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<wildStrike>(upgraded_); }
    int getCardIDNum() const { return 22; }
    combatChoices getCombatChoices() const { return combatChoices::EnemyTarget; }
    ~wildStrike() {}
};
// uncommon
class battleTrance : public card
{
private:
    int drawNumber{ 3 };
public:
    battleTrance(bool upgraded) : card(cardType::Skill, cardRarity::Uncommon, 0, "Battle Trance", "BTranc", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    battleTrance* clone() const { auto temp = new battleTrance(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<battleTrance>(upgraded_); }
    int getCardIDNum() const { return 23; }
    ~battleTrance() {}
};
class bloodForBlood : public card
{
private:
    int damageNumber{ 18 };
    int lessEnergy{ 0 };
public:
    bloodForBlood(bool upgraded) : card(cardType::Attack, cardRarity::Uncommon, 4, "Blood For Blood", "BldFrB", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    bloodForBlood(bool upgraded, int dmgN, int lE) : damageNumber(dmgN), lessEnergy(lE), card(cardType::Attack, cardRarity::Uncommon, 4, "Blood For Blood", "BldFrB", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    void fightStart(bool midFight);
    void incrementEnergy() { ++lessEnergy; }
    int getEnergy() const;
    bloodForBlood* clone() const { auto temp = new bloodForBlood(upgraded_, damageNumber, lessEnergy); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<bloodForBlood>(upgraded_); }
    int getCardIDNum() const { return 24; }
    combatChoices getCombatChoices() const { return combatChoices::EnemyTarget; }
    ~bloodForBlood() {}
};
class bloodletting : public card
{
private:
    int energyNumber{ 2 };
public:
    bloodletting(bool upgraded) : card(cardType::Skill, cardRarity::Uncommon, 0, "Bloodletting", "Bldlet", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    bloodletting* clone() const { auto temp = new bloodletting(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<bloodletting>(upgraded_); }
    int getCardIDNum() const { return 25; }
    ~bloodletting() {}
};
class burningPact : public card
{
private:
    int drawNumber{ 2 };
public:
    burningPact(bool upgraded) : card(cardType::Skill, cardRarity::Uncommon, 1, "Burning Pact", "BurnPc", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    burningPact* clone() const { auto temp = new burningPact(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<burningPact>(upgraded_); }
    int getCardIDNum() const { return 26; }
    combatChoices getCombatChoices() const { return combatChoices::Hand; }
    ~burningPact() {}
};
class carnage : public card
{
private:
    int damageNumber{ 20 };
public:
    carnage(bool upgraded) : card(cardType::Attack, cardRarity::Uncommon, 2, "Carnage", "Carnge", false, upgraded, true, false)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    carnage* clone() const { auto temp = new carnage(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<carnage>(upgraded_); }
    int getCardIDNum() const { return 27; }
    combatChoices getCombatChoices() const { return combatChoices::EnemyTarget; }
    ~carnage() {}
};
class combust : public card
{
private:
    int damageNumber{ 5 };
public:
    combust(bool upgraded) : card(cardType::Power, cardRarity::Uncommon, 1, "Combust", "Combst", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    combust* clone() const { auto temp = new combust(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<combust>(upgraded_); }
    int getCardIDNum() const { return 28; }
    ~combust() {}
};
class darkEmbrace : public card
{
private:
public:
    darkEmbrace(bool upgraded) : card(cardType::Power, cardRarity::Uncommon, 2, "Dark Embrace", "DrkEmb", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    darkEmbrace* clone() const { auto temp = new darkEmbrace(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<darkEmbrace>(upgraded_); }
    int getCardIDNum() const { return 29; }
    ~darkEmbrace() {}
};
class disarm : public card
{
private:
    int strengthNumber{ 2 };
public:
    disarm(bool upgraded) : card(cardType::Skill, cardRarity::Uncommon, 1, "Disarm", "Disarm", false, upgraded, false, true)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    disarm* clone() const { auto temp = new disarm(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<disarm>(upgraded_); }
    int getCardIDNum() const { return 30; }
    combatChoices getCombatChoices() const { return combatChoices::EnemyTarget; }
    ~disarm() {}
};
class dropkick : public card
{
private:
    int damageNumber{ 5 };
public:
    dropkick(bool upgraded) : card(cardType::Attack, cardRarity::Uncommon, 1, "Dropkick", "DropKc", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    dropkick* clone() const { auto temp = new dropkick(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<dropkick>(upgraded_); }
    int getCardIDNum() const { return 31; }
    combatChoices getCombatChoices() const { return combatChoices::EnemyTarget; }
    ~dropkick() {}
};
class dualWield : public card
{
private:
    int copyNumber{ 1 };
public:
    dualWield(bool upgraded) : card(cardType::Skill, cardRarity::Uncommon, 1, "Dual Wield", "DualWd", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    dualWield* clone() const { auto temp = new dualWield(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<dualWield>(upgraded_); }
    int getCardIDNum() const { return 32; }
    combatChoices getCombatChoices() const { return combatChoices::HandDW; }
    ~dualWield() {}
};
class entrench : public card
{
private:
public:
    entrench(bool upgraded) : card(cardType::Skill, cardRarity::Uncommon, 2, "Entrench", "Entrnc", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    entrench* clone() const { auto temp = new entrench(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<entrench>(upgraded_); }
    int getCardIDNum() const { return 33; }
    ~entrench() {}
};
class evolve : public card
{
private:
    int drawNumber{ 1 };
public:
    evolve(bool upgraded) : card(cardType::Power, cardRarity::Uncommon, 1, "Evolve", "Evolve", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    evolve* clone() const { auto temp = new evolve(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<evolve>(upgraded_); }
    int getCardIDNum() const { return 34; }
    ~evolve() {}
};
class feelNoPain : public card
{
private:
    int blockNumber{ 3 };
public:
    feelNoPain(bool upgraded) : card(cardType::Power, cardRarity::Uncommon, 1, "Feel No Pain", " FNoP ", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    feelNoPain* clone() const { auto temp = new feelNoPain(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<feelNoPain>(upgraded_); }
    int getCardIDNum() const { return 35; }
    ~feelNoPain() {}
};
class fireBreathing : public card
{
private:
    int damageNumber{ 6 };
public:
    fireBreathing(bool upgraded) : card(cardType::Power, cardRarity::Uncommon, 1, "Fire Breathing", "FireBr", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    fireBreathing* clone() const { auto temp = new fireBreathing(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<fireBreathing>(upgraded_); }
    int getCardIDNum() const { return 36; }
    ~fireBreathing() {}
};
class flameBarrier : public card
{
private:
    int blockNumber{ 12 };
    int damageNumber{ 4 };
public:
    flameBarrier(bool upgraded) : card(cardType::Skill, cardRarity::Uncommon, 2, "Flame Barrier", "FlameB", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    flameBarrier* clone() const { auto temp = new flameBarrier(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<flameBarrier>(upgraded_); }
    int getCardIDNum() const { return 37; }
    ~flameBarrier() {}
};
class ghostlyArmor : public card
{
private:
    int blockNumber{ 10 };
public:
    ghostlyArmor(bool upgraded) : card(cardType::Skill, cardRarity::Uncommon, 1, "Ghostly Armor", "GhstAr", false, upgraded, true, false)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    ghostlyArmor* clone() const { auto temp = new ghostlyArmor(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<ghostlyArmor>(upgraded_); }
    int getCardIDNum() const { return 38; }
    ~ghostlyArmor() {}
};
class hemokinesis : public card
{
private:
    int damageNumber{ 15 };
public:
    hemokinesis(bool upgraded) : card(cardType::Attack, cardRarity::Uncommon, 1, "Hemokinesis", "Hemoki", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    hemokinesis* clone() const { auto temp = new hemokinesis(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<hemokinesis>(upgraded_); }
    int getCardIDNum() const { return 39; }
    combatChoices getCombatChoices() const { return combatChoices::EnemyTarget; }
    ~hemokinesis() {}
};
class infernalBlade : public card
{
private:
public:
    infernalBlade(bool upgraded) : card(cardType::Skill, cardRarity::Uncommon, 1, "Infernal Blade", "InfBld", false, upgraded, false, true)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    infernalBlade* clone() const { auto temp = new infernalBlade(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<infernalBlade>(upgraded_); }
    int getCardIDNum() const { return 40; }
    ~infernalBlade() {}
};
class inflame : public card
{
private:
    int strengthNumber{ 2 };
public:
    inflame(bool upgraded) : card(cardType::Power, cardRarity::Uncommon, 1, "Inflame", "Inflme", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    inflame* clone() const { auto temp = new inflame(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<inflame>(upgraded_); }
    int getCardIDNum() const { return 41; }
    ~inflame() {}
};
class intimidate : public card
{
private:
    int weakNumber{ 1 };
public:
    intimidate(bool upgraded) : card(cardType::Skill, cardRarity::Uncommon, 0, "Intimidate", "Intimd", false, upgraded, false, true)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    intimidate* clone() const { auto temp = new intimidate(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<intimidate>(upgraded_); }
    int getCardIDNum() const { return 42; }
    ~intimidate() {}
};
class metallicize : public card
{
private:
    int blockNumber{ 3 };
public:
    metallicize(bool upgraded) : card(cardType::Power, cardRarity::Uncommon, 1, "Metallicize", "Metalc", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    metallicize* clone() const { auto temp = new metallicize(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<metallicize>(upgraded_); }
    int getCardIDNum() const { return 43; }
    ~metallicize() {}
};
class powerThrough : public card
{
private:
    int blockNumber{ 15 };
public:
    powerThrough(bool upgraded) : card(cardType::Skill, cardRarity::Uncommon, 1, "Power Through", "PowerT", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    powerThrough* clone() const { auto temp = new powerThrough(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<powerThrough>(upgraded_); }
    int getCardIDNum() const { return 44; }
    ~powerThrough() {}
};
class pummel : public card
{
private:
    int damageNumber{ 2 };
    int multi{ 4 };
public:
    pummel(bool upgraded) : card(cardType::Attack, cardRarity::Uncommon, 1, "Pummel", "Pummel", false, upgraded, false, true)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    pummel* clone() const { auto temp = new pummel(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<pummel>(upgraded_); }
    int getCardIDNum() const { return 45; }
    combatChoices getCombatChoices() const { return combatChoices::EnemyTarget; }
    ~pummel() {}
};
class ragee : public card
{
private:
    int blockNumber{ 3 };
public:
    ragee(bool upgraded) : card(cardType::Skill, cardRarity::Uncommon, 0, "Rage", " Rage ", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    ragee* clone() const { auto temp = new ragee(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<ragee>(upgraded_); }
    int getCardIDNum() const { return 46; }
    ~ragee() {}
};
class rampage : public card
{
private:
    int damageNumber{ 8 };
    int dIncrease{ 5 };
public:
    rampage(bool upgraded) : card(cardType::Attack, cardRarity::Uncommon, 1, "Rampage", "Rampge", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    rampage(bool upgraded, int dN, int dI) : damageNumber(dN), dIncrease(dI), card(cardType::Attack, cardRarity::Uncommon, 1, "Rampage", "Rampge", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    rampage* clone() const { 
        auto temp = new rampage(upgraded_, damageNumber, dIncrease); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp;
    }
    std::unique_ptr<card> cloneU() const { return std::make_unique<rampage>(upgraded_); }
    int getCardIDNum() const { return 47; }
    combatChoices getCombatChoices() const { return combatChoices::EnemyTarget; }
    ~rampage() {}
};
class recklessCharge : public card
{
private:
    int damageNumber{ 7 };
public:
    recklessCharge(bool upgraded) : card(cardType::Attack, cardRarity::Uncommon, 0, "Reckless Charge", "ReckCh", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    recklessCharge* clone() const { auto temp = new recklessCharge(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<recklessCharge>(upgraded_); }
    int getCardIDNum() const { return 48; }
    combatChoices getCombatChoices() const { return combatChoices::EnemyTarget; }
    ~recklessCharge() {}
};
class rupture : public card
{
private:
    int strengthNumber{ 1 };
public:
    rupture(bool upgraded) : card(cardType::Power, cardRarity::Uncommon, 1, "Rupture", "Ruptre", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    rupture* clone() const { auto temp = new rupture(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<rupture>(upgraded_); }
    int getCardIDNum() const { return 49; }
    ~rupture() {}
};
class searingBlow : public card
{
private:
    int damageNumber{ 12 };
    int incr{ 0 };
    int upgradeCount{ 0 };
public:
    searingBlow(bool upgraded) : card(cardType::Attack, cardRarity::Uncommon, 2, "Searing Blow", "SearBl", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    searingBlow(bool upgraded, int dmg, int incr_, int upgradeC) : damageNumber(dmg), incr(incr_), upgradeCount(upgradeC), card(cardType::Attack, cardRarity::Uncommon, 2, "Searing Blow", "SearBl", false, upgraded)
    {
        if (upgradeCount > 0)
        {
            sixCharName_ = "Sear+" + std::to_string(upgradeCount);
            name_ = "Searing Blow+" + std::to_string(upgradeCount);
        }
    }
    void execute();
    void upgradeThis();
    searingBlow* clone() const {
        auto temp = new searingBlow(upgraded_, damageNumber, incr, upgradeCount); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp;
    }
    std::unique_ptr<card> cloneU() const { return std::make_unique<searingBlow>(upgraded_, damageNumber, incr, upgradeCount); }
    int getCardIDNum() const { return 50; }
    combatChoices getCombatChoices() const { return combatChoices::EnemyTarget; }
    ~searingBlow() {}
};
class secondWind : public card
{
private:
    int blockNumber{ 5 };
public:
    secondWind(bool upgraded) : card(cardType::Skill, cardRarity::Uncommon, 1, "Second Wind", "SnWind", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    secondWind* clone() const { auto temp = new secondWind(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<secondWind>(upgraded_); }
    int getCardIDNum() const { return 51; }
    ~secondWind() {}
};
class seeingRed : public card
{
private:
public:
    seeingRed(bool upgraded) : card(cardType::Skill, cardRarity::Uncommon, 1, "Seeing Red", "SeeRed", false, upgraded, false, true)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    seeingRed* clone() const { auto temp = new seeingRed(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<seeingRed>(upgraded_); }
    int getCardIDNum() const { return 52; }
    ~seeingRed() {}
};
class sentinel : public card
{
private:
    int blockNumber{ 5 };
    int energyNumber{ 2 };
public:
    sentinel(bool upgraded) : card(cardType::Skill, cardRarity::Uncommon, 1, "Sentinel", "Sentin", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    void fightStart(bool midFight);
    sentinel* clone() const { auto temp = new sentinel(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<sentinel>(upgraded_); }
    int getCardIDNum() const { return 53; }
    ~sentinel() {}
};
class severSoul : public card
{
private:
    int damageNumber{ 16 };
public:
    severSoul(bool upgraded) : card(cardType::Attack, cardRarity::Uncommon, 2, "Sever Soul", "SeverS", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    severSoul* clone() const { auto temp = new severSoul(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<severSoul>(upgraded_); }
    int getCardIDNum() const { return 54; }
    combatChoices getCombatChoices() const { return combatChoices::EnemyTarget; }
    ~severSoul() {}
};
class shockwave : public card
{
private:
    int weakNumber{ 3 };
    int vulnerableNumber{ 3 };
public:
    shockwave(bool upgraded) : card(cardType::Skill, cardRarity::Uncommon, 2, "Shockwave", "ShckWv", false, upgraded, false, true)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    shockwave* clone() const { auto temp = new shockwave(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<shockwave>(upgraded_); }
    int getCardIDNum() const { return 55; }
    ~shockwave() {}
};
class spotWeakness : public card
{
private:
    int strengthNumber{ 3 };
public:
    spotWeakness(bool upgraded) : card(cardType::Skill, cardRarity::Uncommon, 1, "Spot Weakness", "SpotWk", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    spotWeakness* clone() const { auto temp = new spotWeakness(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<spotWeakness>(upgraded_); }
    int getCardIDNum() const { return 56; }
    combatChoices getCombatChoices() const { return combatChoices::EnemyTarget; }
    ~spotWeakness() {}
};
class uppercut : public card
{
private:
    int damageNumber{ 13 };
    int vulnerableNumber{ 1 };
    int weakNumber{ 1 };
public:
    uppercut(bool upgraded) : card(cardType::Attack, cardRarity::Uncommon, 2, "Uppercut", "UpperC", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    uppercut* clone() const { auto temp = new uppercut(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<uppercut>(upgraded_); }
    int getCardIDNum() const { return 57; }
    combatChoices getCombatChoices() const { return combatChoices::EnemyTarget; }
    ~uppercut() {}
};
class whirlwind : public card
{
private:
    int damageNumber{ 5 };
    int multiX{ 0 };
public:
    whirlwind(bool upgraded, int mX = 0) : multiX(mX), card(cardType::Attack, cardRarity::Uncommon, -1, "Whirlwind", "Whirlw", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    // whirlwind(bool upgraded, int multiX_): multiX(multiX_), card(cardType::Attack,cardRarity::Uncommon,-1,"Whirlwind","Whirlw",false,upgraded)
    //     {if(upgraded) upgradeThis();}
    void execute();
    void upgradeThis();
    void incrementEnergy() { multiX = 0; }
    whirlwind* clone() const { auto temp = new whirlwind(upgraded_,multiX); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<whirlwind>(upgraded_); }
    int getCardIDNum() const { return 58; }
    ~whirlwind() {}
};

// rare
class barricade : public card
{
private:
public:
    barricade(bool upgraded) : card(cardType::Power, cardRarity::Rare, 3, "Barricade", "Barrcd", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    barricade* clone() const { auto temp = new barricade(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<barricade>(upgraded_); }
    int getCardIDNum() const { return 59; }
    ~barricade() {}
};
class berserk : public card
{
private:
    int vulnerableNumber{ 2 };
public:
    berserk(bool upgraded) : card(cardType::Power, cardRarity::Rare, 0, "Berserk", "Bersrk", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    berserk* clone() const { auto temp = new berserk(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<berserk>(upgraded_); }
    int getCardIDNum() const { return 60; }
    ~berserk() {}
};
class bludgeon : public card
{
private:
    int damageNumber{ 32 };
public:
    bludgeon(bool upgraded) : card(cardType::Attack, cardRarity::Rare, 3, "Bludgeon", "Bludgn", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    bludgeon* clone() const { auto temp = new bludgeon(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<bludgeon>(upgraded_); }
    int getCardIDNum() const { return 61; }
    combatChoices getCombatChoices() const { return combatChoices::EnemyTarget; }
    ~bludgeon() {}
};
class brutality : public card
{
private:
public:
    brutality(bool upgraded) : card(cardType::Power, cardRarity::Rare, 0, "Brutality", "Brutal", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    brutality* clone() const { auto temp = new brutality(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<brutality>(upgraded_); }
    int getCardIDNum() const { return 62; }
    ~brutality() {}
};
class corruption : public card
{
private:
public:
    corruption(bool upgraded) : card(cardType::Power, cardRarity::Rare, 3, "Corruption", "Corrup", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    corruption* clone() const { auto temp = new corruption(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<corruption>(upgraded_); }
    int getCardIDNum() const { return 63; }
    ~corruption() {}
};
class demonForm : public card
{
private:
    int strengthNumber{ 2 };
public:
    demonForm(bool upgraded) : card(cardType::Power, cardRarity::Rare, 3, "Demon Form", "DemonF", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    demonForm* clone() const { auto temp = new demonForm(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<demonForm>(upgraded_); }
    int getCardIDNum() const { return 64; }
    ~demonForm() {}
};
class doubleTap : public card
{
private:
    int doublePlayNumber{ 1 };
public:
    doubleTap(bool upgraded) : card(cardType::Skill, cardRarity::Rare, 1, "Double Tap", "DoublT", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    doubleTap* clone() const { auto temp = new doubleTap(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<doubleTap>(upgraded_); }
    int getCardIDNum() const { return 65; }
    ~doubleTap() {}
};
class exhume : public card
{
private:
public:
    exhume(bool upgraded) : card(cardType::Skill, cardRarity::Rare, 1, "Exhume", "Exhume", false, upgraded, false, true)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    exhume* clone() const { auto temp = new exhume(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<exhume>(upgraded_); }
    int getCardIDNum() const { return 66; }
    combatChoices getCombatChoices() const { return combatChoices::Exhaust; }
    ~exhume() {}
};
class feed : public card
{
private:
    int damageNumber{ 10 };
    int healthNumber{ 3 };
public:
    feed(bool upgraded) : card(cardType::Attack, cardRarity::Rare, 1, "Feed", " Feed ", false, upgraded, false, true)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    feed* clone() const { auto temp = new feed(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<feed>(upgraded_); }
    int getCardIDNum() const { return 67; }
    combatChoices getCombatChoices() const { return combatChoices::EnemyTarget; }
    ~feed() {}
};
class fiendFire : public card
{
private:
    int damageNumber{ 7 };
public:
    fiendFire(bool upgraded) : card(cardType::Attack, cardRarity::Rare, 2, "Fiend Fire", "FiendF", false, upgraded, false, true)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    fiendFire* clone() const { auto temp = new fiendFire(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<fiendFire>(upgraded_); }
    int getCardIDNum() const { return 68; }
    combatChoices getCombatChoices() const { return combatChoices::EnemyTarget; }
    ~fiendFire() {}
};
class immolate : public card
{
private:
    int damageNumber{ 21 };
public:
    immolate(bool upgraded) : card(cardType::Attack, cardRarity::Rare, 2, "Immolate", "Immola", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    immolate* clone() const { auto temp = new immolate(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<immolate>(upgraded_); }
    int getCardIDNum() const { return 69; }
    ~immolate() {}
};
class impervious : public card
{
private:
    int blockNumber{ 30 };
public:
    impervious(bool upgraded) : card(cardType::Skill, cardRarity::Rare, 2, "Impervious", "Imperv", false, upgraded, false, true)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    impervious* clone() const { auto temp = new impervious(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<impervious>(upgraded_); }
    int getCardIDNum() const { return 70; }
    ~impervious() {}
};
class juggernaut : public card
{
private:
    int damageNumber{ 5 };
public:
    juggernaut(bool upgraded) : card(cardType::Power, cardRarity::Rare, 2, "Juggernaut", "Jugger", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    juggernaut* clone() const { auto temp = new juggernaut(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<juggernaut>(upgraded_); }
    int getCardIDNum() const { return 71; }
    ~juggernaut() {}
};
class limitBreak : public card
{
private:
public:
    limitBreak(bool upgraded) : card(cardType::Skill, cardRarity::Rare, 1, "Limit Break", "LimitB", false, upgraded, false, true)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    limitBreak* clone() const { auto temp = new limitBreak(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<limitBreak>(upgraded_); }
    int getCardIDNum() const { return 72; }
    ~limitBreak() {}
};
class offering : public card
{
private:
    int lifeLoss{ 6 };
    int energyGain{ 2 };
    int drawNumber{ 3 };
public:
    offering(bool upgraded) : card(cardType::Skill, cardRarity::Rare, 0, "Offering", "Offrin", false, upgraded, false, true)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    offering* clone() const { auto temp = new offering(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<offering>(upgraded_); }
    int getCardIDNum() const { return 73; }
    ~offering() {}
};
class reaper : public card
{
private:
    int damageNumber{ 4 };
public:
    reaper(bool upgraded) : card(cardType::Attack, cardRarity::Rare, 2, "Reaper", "Reaper", false, upgraded, false, true)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    reaper* clone() const { auto temp = new reaper(upgraded_); temp->bottled_=bottled_; temp->temp_energy_=temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<reaper>(upgraded_); }
    int getCardIDNum() const { return 74; }
    ~reaper() {}
};

// curses
class injury : public card
{
public:
    injury() : card(cardType::Curse, cardRarity::Other, 0, "Injury", "Injury", false, false) {}
    bool castable() { return blueCandleCheck && card::castable(); }
    bool getExhaust() const { return blueCandleCheck; }
    void execute();
    void upgradeThis() {}
    injury* clone() const { return new injury(); }
    std::unique_ptr<card> cloneU() const { return std::make_unique<injury>(); }
    int getCardIDNum() const { return 75; }
    cardClass getClass() const { return cardClass::Curse; }
    ~injury() {}
};
class regret : public card
{
public:
    regret() : card(cardType::Curse, cardRarity::Other, 0, "Regret", "Regret", false, false) {}
    bool castable() { return blueCandleCheck && card::castable(); }
    bool getExhaust() const { return blueCandleCheck; }
    void execute();
    void upgradeThis() {}
    bool endStepDiscardPrio();
    regret* clone() const { return new regret(); }
    std::unique_ptr<card> cloneU() const { return std::make_unique<regret>(); }
    int getCardIDNum() const { return 76; }
    cardClass getClass() const { return cardClass::Curse; }
    ~regret() {}
};
class pain : public card
{
public:
    pain() : card(cardType::Curse, cardRarity::Other, 0, "Pain", " Pain ", false, false) {}
    bool castable() { return blueCandleCheck && card::castable(); }
    bool getExhaust() const { return blueCandleCheck; }
    void execute();
    void fightStart(bool midFight);
    void upgradeThis() {}
    pain* clone() const { return new pain(); }
    std::unique_ptr<card> cloneU() const { return std::make_unique<pain>(); }
    int getCardIDNum() const { return 77; }
    cardClass getClass() const { return cardClass::Curse; }
    ~pain() {}
};
class doubt : public card
{
public:
    doubt() : card(cardType::Curse, cardRarity::Other, 0, "Doubt", "Doubt ", false, false) {}
    bool castable() { return blueCandleCheck && card::castable(); }
    bool getExhaust() const { return blueCandleCheck; }
    bool endStepDiscard();
    void execute();
    void upgradeThis() {}
    doubt* clone() const { return new doubt(); }
    std::unique_ptr<card> cloneU() const { return std::make_unique<doubt>(); }
    int getCardIDNum() const { return 78; }
    cardClass getClass() const { return cardClass::Curse; }
    ~doubt() {}
};
class parasite : public card
{
public:
    parasite() : card(cardType::Curse, cardRarity::Other, 0, "Parasite", "Parast", false, false) {}
    bool castable() { return blueCandleCheck && card::castable(); }
    bool getExhaust() const { return blueCandleCheck; }
    void execute();
    void upgradeThis() {}
    parasite* clone() const { return new parasite(); }
    std::unique_ptr<card> cloneU() const { return std::make_unique<parasite>(); }
    int getCardIDNum() const { return 79; }
    cardClass getClass() const { return cardClass::Curse; }
    ~parasite() {}
};
class necronomicurse : public card
{
public:
    necronomicurse() : card(cardType::Curse, cardRarity::Other, 0, "Necronomicurse", "Necron", false, false) {}
    bool castable() { return blueCandleCheck && card::castable(); }
    bool getExhaust() const { return blueCandleCheck; }
    void execute();
    void fightStart(bool midFight);
    void upgradeThis() {}
    necronomicurse* clone() const { return new necronomicurse(); }
    std::unique_ptr<card> cloneU() const { return std::make_unique<necronomicurse>(); }
    int getCardIDNum() const { return 80; }
    cardClass getClass() const { return cardClass::Curse; }
    ~necronomicurse() {}
};
class clumsy : public card
{
public:
    clumsy() : card(cardType::Curse, cardRarity::Other, 0, "Clumsy", "Clumsy", false, false,true,false) {}
    bool castable() { return blueCandleCheck && card::castable(); }
    bool getExhaust() const { return blueCandleCheck; }
    void execute();
    void upgradeThis() {}
    clumsy* clone() const { return new clumsy(); } 
    std::unique_ptr<card> cloneU() const { return std::make_unique<clumsy>(); }
    int getCardIDNum() const { return 81; }
    cardClass getClass() const { return cardClass::Curse; }
    ~clumsy() {}
};
class curseOfTheBell : public card
{
public:
    curseOfTheBell() : card(cardType::Curse, cardRarity::Other, 0, "Curse Of The Bell", "COTBel", false, false) {}
    bool castable() { return blueCandleCheck && card::castable(); }
    bool getExhaust() const { return blueCandleCheck; }
    void execute();
    void upgradeThis() {}
    curseOfTheBell* clone() const { return new curseOfTheBell(); }
    std::unique_ptr<card> cloneU() const { return std::make_unique<curseOfTheBell>(); }
    int getCardIDNum() const { return 82; }
    cardClass getClass() const { return cardClass::Curse; }
    ~curseOfTheBell() {}
};
class decay : public card
{
public:
    decay() : card(cardType::Curse, cardRarity::Other, 0, "Decay", "Decay ", false, false) {}
    bool castable() { return blueCandleCheck && card::castable(); }
    bool getExhaust() const { return blueCandleCheck; }
    void execute();
    bool endStepDiscard();
    void upgradeThis() {}
    decay* clone() const { return new decay(); }
    std::unique_ptr<card> cloneU() const { return std::make_unique<decay>(); }
    int getCardIDNum() const { return 83; }
    cardClass getClass() const { return cardClass::Curse; }
    ~decay() {}
};
class normality : public card
{
public:
    normality() : card(cardType::Curse, cardRarity::Other, 0, "Normality", "Nrmlty", false, false) {}
    bool castable() { return blueCandleCheck && card::castable(); }
    bool getExhaust() const { return blueCandleCheck; }
    void execute();
    void fightStart(bool midFight);
    void upgradeThis() {}
    normality* clone() const { return new normality(); }
    std::unique_ptr<card> cloneU() const { return std::make_unique<normality>(); }
    int getCardIDNum() const { return 84; }
    cardClass getClass() const { return cardClass::Curse; }
    ~normality() {}
};
class writhe : public card
{
public:
    writhe() : card(cardType::Curse, cardRarity::Other, 0, "Writhe", "Writhe", true, false) {}
    bool castable() { return blueCandleCheck && card::castable(); }
    bool getExhaust() const { return blueCandleCheck; }
    void execute();
    void upgradeThis() {}
    writhe* clone() const { return new writhe(); }
    std::unique_ptr<card> cloneU() const { return std::make_unique<writhe>(); }
    int getCardIDNum() const { return 85; }
    cardClass getClass() const { return cardClass::Curse; }
    ~writhe() {}
};
class shame : public card
{
public:
    shame() : card(cardType::Curse, cardRarity::Other, 0, "Shame", "Shame ", false, false) {}
    bool castable() { return blueCandleCheck && card::castable(); }
    bool getExhaust() const { return blueCandleCheck; }
    void execute();
    bool endStepDiscard();
    void upgradeThis() {}
    shame* clone() const { return new shame(); }
    std::unique_ptr<card> cloneU() const { return std::make_unique<shame>(); }
    int getCardIDNum() const { return 86; }
    cardClass getClass() const { return cardClass::Curse; }
    ~shame() {}
};


// status
class burn : public card
{
private:
    int damageNumber{ 2 };
public:
    burn(bool upgraded = false) : card(cardType::Status, cardRarity::Other, 0, "Burn", " Burn ", false, upgraded) {}
    bool castable() { return medKitCheck && card::castable(); }
    bool getExhaust() const { return medKitCheck; }
    void execute() {}
    void upgradeThis() {}
    void upgradeStatus();
    bool endStepDiscard();
    burn* clone() const { return new burn(upgraded_); }
    std::unique_ptr<card> cloneU() const { return std::make_unique<burn>(upgraded_); }
    int getCardIDNum() const { return 87; }
    ~burn() {}
};
class dazed : public card
{
private:
    int damageNumber{ 2 };
public:
    dazed() : card(cardType::Status, cardRarity::Other, 0, "Dazed", "Dazed ", false, false, true, false) {}
    bool castable() { return medKitCheck && card::castable(); }
    bool getExhaust() const { return medKitCheck; }
    void execute() {}
    void upgradeThis() {}
    dazed* clone() const { return new dazed(); }
    std::unique_ptr<card> cloneU() const { return std::make_unique<dazed>(); }
    int getCardIDNum() const { return 88; }
    ~dazed() {}
};
class slime : public card
{
private:
public:
    slime() : card(cardType::Status, cardRarity::Other, 1, "Slime", "Slime ", false, false, false, true) {}
    bool castable() { return card::castable(); }
    void execute() {}
    void upgradeThis() {}
    slime* clone() const { auto temp = new slime(); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<slime>(); }
    int getCardIDNum() const { return 89; }
    ~slime() {}
};
class wound : public card
{
private:
public:
    wound() : card(cardType::Status, cardRarity::Other, 0, "Wound", "Wound ", false, false) {}
    bool castable() { return medKitCheck && card::castable(); }
    bool getExhaust() const { return medKitCheck; }
    void execute() {}
    void upgradeThis() {}
    // void endStepDiscard(){}
    wound* clone() const { return new wound(); }
    std::unique_ptr<card> cloneU() const { return std::make_unique<wound>(); }
    int getCardIDNum() const { return 90; }
    ~wound() {}
};


// SPECIAL
class bite : public card
{
private:
    int damageNumber{ 7 };
    int healNumber{ 2 };
public:
    bite(bool upgraded) : card(cardType::Attack, cardRarity::Other, 1, "Bite", " Bite ", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    bite* clone() const { auto temp = new bite(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<bite>(upgraded_); }
    int getCardIDNum() const { return 91; }
    combatChoices getCombatChoices() const { return combatChoices::EnemyTarget; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~bite() {}
};
class jax : public card
{
private:
    int strengthNumber{ 2 };
    int hpNumber{ 3 };
public:
    jax(bool upgraded) : card(cardType::Skill, cardRarity::Other,0, "Jax", " Jax  ", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    jax* clone() const { auto temp = new jax(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed;  return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<jax>(upgraded_); }
    int getCardIDNum() const { return 92; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~jax() {}
};
class apparition : public card
{
private:
public:
    apparition(bool upgraded) : card(cardType::Skill, cardRarity::Other, 1, "Apparition", "Appari", false, upgraded, true, true)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    apparition* clone() const { auto temp = new apparition(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed;  return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<apparition>(upgraded_); }
    int getCardIDNum() const { return 93; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~apparition() {}
};
class ritualDagger : public card
{
private:
    int damageNumber{ 15 };
    int damageIncr{ 3 };
public:
    ritualDagger(bool upgraded,int dN = 15) : card(cardType::Attack, cardRarity::Other, 1, "Ritual Dagger", "RitDag", false, upgraded, false, true)
    {
        if (upgraded) upgradeThis();
        damageNumber = dN;
    }
    void execute();
    void upgradeThis();
    void incrementEnergy() { damageNumber += damageIncr; }
    int getDamageNumber() const { return damageNumber; }
    ritualDagger* clone() const { auto temp = new ritualDagger(upgraded_,damageNumber); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<ritualDagger>(upgraded_,damageNumber); }
    int getCardIDNum() const { return 94; }
    combatChoices getCombatChoices() const { return combatChoices::EnemyTarget; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~ritualDagger() {}
};




// COLOURLESS
class bandageUp : public card
{
private:
    int healNumber{ 4 };
public:
    bandageUp(bool upgraded) : card(cardType::Skill, cardRarity::Uncommon, 0, "Bandage Up", "Bandge", false, upgraded, false, true)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    bandageUp* clone() const { auto temp = new bandageUp(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<bandageUp>(upgraded_); }
    int getCardIDNum() const { return 95; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~bandageUp() {}
};
class blind : public card
{
private:
    combatChoices pickChoice{ combatChoices::EnemyTarget };
public:
    blind(bool upgraded) : card(cardType::Skill, cardRarity::Uncommon, 0, "Blind", "Blind ", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    blind* clone() const { auto temp = new blind(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<blind>(upgraded_); }
    int getCardIDNum() const { return 96; }
    combatChoices getCombatChoices() const { return pickChoice; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~blind() {}
};
class darkShackles : public card
{
private:
    int strengthNumber{ 9 };
public:
    darkShackles(bool upgraded) : card(cardType::Skill, cardRarity::Uncommon, 0, "Dark Shackles", "DrkShk", false, upgraded,false,true)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    darkShackles* clone() const { auto temp = new darkShackles(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<darkShackles>(upgraded_); }
    int getCardIDNum() const { return 97; }
    combatChoices getCombatChoices() const { return combatChoices::EnemyTarget; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~darkShackles() {}
};
class deepBreath : public card
{
private:
    int drawNumber{ 1 };
public:
    deepBreath(bool upgraded) : card(cardType::Skill, cardRarity::Uncommon, 0, "Deep Breath", "DeepBr", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    deepBreath* clone() const { auto temp = new deepBreath(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<deepBreath>(upgraded_); }
    int getCardIDNum() const { return 98; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~deepBreath() {}
};
class discovery : public card
{
private:
public:
    discovery(bool upgraded) : card(cardType::Skill, cardRarity::Uncommon, 1, "Discovery", "Dscvry", false, upgraded,false, true)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    discovery* clone() const { auto temp = new discovery(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<discovery>(upgraded_); }
    int getCardIDNum() const { return 99; }
    combatChoices getCombatChoices() const { return combatChoices::Choice3NoSkip; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~discovery() {}
};
class dramaticEntrance : public card
{
private:
    int damageNumber{ 8 };
public:
    dramaticEntrance(bool upgraded) : card(cardType::Attack, cardRarity::Uncommon, 0, "Dramatic Entrance", "DramaE", true, upgraded, false, true)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    dramaticEntrance* clone() const { auto temp = new dramaticEntrance(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<dramaticEntrance>(upgraded_); }
    int getCardIDNum() const { return 100; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~dramaticEntrance() {}
};
class enlightenment : public card
{
private:
public:
    enlightenment(bool upgraded) : card(cardType::Skill, cardRarity::Uncommon, 0, "Enlightenment", "Enlght", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    enlightenment* clone() const { auto temp = new enlightenment(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<enlightenment>(upgraded_); }
    int getCardIDNum() const { return 101; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~enlightenment() {}
};
class finesse : public card
{
private:
    int blockNumber{ 2 };
public:
    finesse(bool upgraded) : card(cardType::Skill, cardRarity::Uncommon, 0, "Finesse", "Finese", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    finesse* clone() const { auto temp = new finesse(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<finesse>(upgraded_); }
    int getCardIDNum() const { return 102; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~finesse() {}
};
class flashOfSteel : public card
{
private:
    int damageNumber{ 3 };
public:
    flashOfSteel(bool upgraded) : card(cardType::Attack, cardRarity::Uncommon, 0, "Flash Of Steel", "FlshOS", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    flashOfSteel* clone() const { auto temp = new flashOfSteel(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<flashOfSteel>(upgraded_); }
    int getCardIDNum() const { return 103; }
    combatChoices getCombatChoices() const { return combatChoices::EnemyTarget; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~flashOfSteel() {}
};
class forethought : public card
{
private:
    combatChoices pickChoice{ combatChoices::Hand };
public:
    forethought(bool upgraded) : card(cardType::Skill, cardRarity::Uncommon, 0, "Forethought", "Foreth", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    forethought* clone() const { auto temp = new forethought(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<forethought>(upgraded_); }
    int getCardIDNum() const { return 104; }
    combatChoices getCombatChoices() const { return pickChoice; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~forethought() {}
};
class goodInstincts : public card
{
private:
    int blockNumber{ 6 };
public:
    goodInstincts(bool upgraded) : card(cardType::Skill, cardRarity::Uncommon, 0, "Good Instincts", "GoodIn", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    goodInstincts* clone() const { auto temp = new goodInstincts(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<goodInstincts>(upgraded_); }
    int getCardIDNum() const { return 105; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~goodInstincts() {}
};
class impatience : public card
{
private:
    int drawNumber{ 2 };
public:
    impatience(bool upgraded) : card(cardType::Skill, cardRarity::Uncommon, 0, "Impatience", "Impati", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    impatience* clone() const { auto temp = new impatience(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<impatience>(upgraded_); }
    int getCardIDNum() const { return 106; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~impatience() {}
};
class jackOfAllTrades : public card
{
private:
    int cardNumber{ 1 };
public:
    jackOfAllTrades(bool upgraded) : card(cardType::Skill, cardRarity::Uncommon, 0, "Jack Of All Trades", "JakOAT", false, upgraded, false, true)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    jackOfAllTrades* clone() const { auto temp = new jackOfAllTrades(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<jackOfAllTrades>(upgraded_); }
    int getCardIDNum() const { return 107; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~jackOfAllTrades() {}
};
class madness : public card
{
private:
public:
    madness(bool upgraded) : card(cardType::Skill, cardRarity::Uncommon, 1, "Madness", "Madnes", false, upgraded, false, true)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    madness* clone() const { auto temp = new madness(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<madness>(upgraded_); }
    int getCardIDNum() const { return 108; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~madness() {}
};
class mindBlast : public card
{
private:
public:
    mindBlast(bool upgraded) : card(cardType::Attack, cardRarity::Uncommon, 2, "Mind Blast", "MindBl", true, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    mindBlast* clone() const { auto temp = new mindBlast(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<mindBlast>(upgraded_); }
    int getCardIDNum() const { return 109; }
    combatChoices getCombatChoices() const { return combatChoices::EnemyTarget; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~mindBlast() {}
};
class panacea : public card
{
private:
    int artifactNumber{ 1 };
public:
    panacea(bool upgraded) : card(cardType::Skill, cardRarity::Uncommon, 0, "Panacea", "Pancea", false, upgraded, false, true)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    panacea* clone() const { auto temp = new panacea(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<panacea>(upgraded_); }
    int getCardIDNum() const { return 110; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~panacea() {}
};
class panicButton : public card
{
private:
    int blockNumber{ 30 };
public:
    panicButton(bool upgraded) : card(cardType::Skill, cardRarity::Uncommon, 0, "Panic Button", "PncBtn", false, upgraded, false, true)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    panicButton* clone() const { auto temp = new panicButton(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<panicButton>(upgraded_); }
    int getCardIDNum() const { return 111; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~panicButton() {}
};
class purity : public card
{
private:
    int cardNumber{ 3 };
public:
    purity(bool upgraded) : card(cardType::Skill, cardRarity::Uncommon, 0, "Purity", "Purity", false, upgraded, false, true)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    purity* clone() const { auto temp = new purity(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<purity>(upgraded_); }
    int getCardIDNum() const { return 112; }
    combatChoices getCombatChoices() const { if(cardNumber == 3) return combatChoices::HandMulti3;
                                             else { return combatChoices::HandMulti5; } }
    cardClass getClass() const { return cardClass::Colourless; }
    ~purity() {}
};
class swiftStrike : public card
{
private:
    int damageNumber{ 7 };
public:
    swiftStrike(bool upgraded) : card(cardType::Attack, cardRarity::Uncommon, 0, "Swift Strike", "SwftSt", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    swiftStrike* clone() const { auto temp = new swiftStrike(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<swiftStrike>(upgraded_); }
    int getCardIDNum() const { return 113; }
    combatChoices getCombatChoices() const { return combatChoices::EnemyTarget; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~swiftStrike() {}
};
class trip : public card
{
private:
    combatChoices pickChoice{ combatChoices::EnemyTarget };
public:
    trip(bool upgraded) : card(cardType::Skill, cardRarity::Uncommon, 0, "Trip", " Trip ", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    trip* clone() const { auto temp = new trip(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<trip>(upgraded_); }
    int getCardIDNum() const { return 114; }
    combatChoices getCombatChoices() const { return pickChoice; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~trip() {}
};


class apotheosis : public card
{
private:
public:
    apotheosis(bool upgraded) : card(cardType::Skill, cardRarity::Rare, 2, "Apotheosis", "Apoths", false, upgraded,false,true)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    apotheosis* clone() const { auto temp = new apotheosis(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<apotheosis>(upgraded_); }
    int getCardIDNum() const { return 115; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~apotheosis() {}
};
class chrysalis : public card
{
private:
    int cardNumber{ 3 };
public:
    chrysalis(bool upgraded) : card(cardType::Skill, cardRarity::Rare, 2, "Chrysalis", "Chrysl", false, upgraded, false, true)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    chrysalis* clone() const { auto temp = new chrysalis(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<chrysalis>(upgraded_); }
    int getCardIDNum() const { return 116; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~chrysalis() {}
};
class handOfGreed : public card
{
private:
    int bothNumber{ 20 };
public:
    handOfGreed(bool upgraded) : card(cardType::Attack, cardRarity::Rare, 2, "Hand Of Greed", "HGreed", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    handOfGreed* clone() const { auto temp = new handOfGreed(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<handOfGreed>(upgraded_); }
    int getCardIDNum() const { return 117; }
    combatChoices getCombatChoices() const { return combatChoices::EnemyTarget; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~handOfGreed() {}
};
class magnetism : public card
{
private:
public:
    magnetism(bool upgraded) : card(cardType::Power, cardRarity::Rare, 2, "Magnetism", "Magnet", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    magnetism* clone() const { auto temp = new magnetism(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<magnetism>(upgraded_); }
    int getCardIDNum() const { return 118; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~magnetism() {}
};
class masterOfStrategy : public card
{
private:
    int drawNumber{ 3 };
public:
    masterOfStrategy(bool upgraded) : card(cardType::Skill, cardRarity::Rare, 0, "Master Of Strategy", "MsOStr", false, upgraded, false, true)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    masterOfStrategy* clone() const { auto temp = new masterOfStrategy(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<masterOfStrategy>(upgraded_); }
    int getCardIDNum() const { return 119; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~masterOfStrategy() {}
};
class mayhem : public card
{
private:
public:
    mayhem(bool upgraded) : card(cardType::Power, cardRarity::Rare, 2, "Mayhem", "Mayhem", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    mayhem* clone() const { auto temp = new mayhem(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<mayhem>(upgraded_); }
    int getCardIDNum() const { return 120; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~mayhem() {}
};
class metamorphisis : public card
{
private:
    int cardNumber{ 3 };
public:
    metamorphisis(bool upgraded) : card(cardType::Skill, cardRarity::Rare, 2, "Metamorphisis", "Metamo", false, upgraded, false, true)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    metamorphisis* clone() const { auto temp = new metamorphisis(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<metamorphisis>(upgraded_); }
    int getCardIDNum() const { return 121; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~metamorphisis() {}
};
class panache : public card
{
private:
    int damageNumber{ 10 };
public:
    panache(bool upgraded) : card(cardType::Power, cardRarity::Rare, 0, "Panache", "Panach", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    panache* clone() const { auto temp = new panache(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<panache>(upgraded_); }
    int getCardIDNum() const { return 122; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~panache() {}
};
class sadisticNature : public card
{
private:
    int damageNumber{ 5 };
public:
    sadisticNature(bool upgraded) : card(cardType::Power, cardRarity::Rare, 0, "Sadistic Nature", "SadNat", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    sadisticNature* clone() const { auto temp = new sadisticNature(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<sadisticNature>(upgraded_); }
    int getCardIDNum() const { return 123; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~sadisticNature() {}
};
class secretTechnique : public card
{
private:
public:
    secretTechnique(bool upgraded) : card(cardType::Skill, cardRarity::Rare, 0, "Secret Technique", "SecTec", false, upgraded, false, true)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    secretTechnique* clone() const { auto temp = new secretTechnique(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<secretTechnique>(upgraded_); }
    int getCardIDNum() const { return 124; }
    combatChoices getCombatChoices() const { return combatChoices::DrawSkl; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~secretTechnique() {}
};
class secretWeapon : public card
{
private:
public:
    secretWeapon(bool upgraded) : card(cardType::Skill, cardRarity::Rare, 0, "Secret Weapon", "SecWep", false, upgraded, false, true)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    secretWeapon* clone() const { auto temp = new secretWeapon(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<secretWeapon>(upgraded_); }
    int getCardIDNum() const { return 125; }
    combatChoices getCombatChoices() const { return combatChoices::DrawAtt; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~secretWeapon() {}
};
class theBomb : public card
{
private:
    int damageNumber{ 40 };
public:
    theBomb(bool upgraded) : card(cardType::Skill, cardRarity::Rare, 2, "The Bomb", "TheBmb", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    theBomb* clone() const { auto temp = new theBomb(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<theBomb>(upgraded_); }
    int getCardIDNum() const { return 126; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~theBomb() {}
};
class thinkingAhead : public card
{
private:
public:
    thinkingAhead(bool upgraded) : card(cardType::Skill, cardRarity::Rare, 0, "Thinking Ahead", "ThAhed", false, upgraded, false, true)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    thinkingAhead* clone() const { auto temp = new thinkingAhead(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<thinkingAhead>(upgraded_); }
    int getCardIDNum() const { return 127; }
    combatChoices getCombatChoices() const { return combatChoices::HandTA; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~thinkingAhead() {}
};
class transmutation : public card
{
private:
    int multiX{ 0 };
public:
    transmutation(bool upgraded, int mX = 0) : multiX(mX), card(cardType::Skill, cardRarity::Rare, -1, "Transmutation", "Transm", false, upgraded, false, true)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    void incrementEnergy() { multiX = 0; }
    transmutation* clone() const { auto temp = new transmutation(upgraded_, multiX); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<transmutation>(upgraded_); }
    int getCardIDNum() const { return 128; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~transmutation() {}
};
class violence : public card
{
private:
    int cardNumber{ 3 };
public:
    violence(bool upgraded) : card(cardType::Skill, cardRarity::Rare, 0, "Violence", "Violnc", false, upgraded, false, true)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    violence* clone() const { auto temp = new violence(upgraded_); temp->bottled_=bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<violence>(upgraded_); }
    int getCardIDNum() const { return 129; }
    cardClass getClass() const { return cardClass::Colourless; }
    ~violence() {}
};
class cleave : public card
{
private:
    int damageNumber{ 8 };
public:
    cleave(bool upgraded) : card(cardType::Attack, cardRarity::Common, 1, "Cleave", "Cleave", false, upgraded)
    {
        if (upgraded) upgradeThis();
    }
    void execute();
    void upgradeThis();
    cleave* clone() const { auto temp = new cleave(upgraded_); temp->bottled_ = bottled_; temp->temp_energy_ = temp_energy_; temp->zeroUntilPlayed = zeroUntilPlayed; temp->oneUntilPlayed = oneUntilPlayed; return temp; }
    std::unique_ptr<card> cloneU() const { return std::make_unique<cleave>(upgraded_); }
    int getCardIDNum() const { return 130; }
    ~cleave() {}
};
class voidx : public card
{
private:
public:
    voidx(bool upgraded = false) : card(cardType::Status, cardRarity::Other, 0, "Void", " Void ", false, upgraded, true, false) {}
    bool castable() { return medKitCheck && card::castable(); }
    bool getExhaust() const { return medKitCheck; }
    void execute() {}
    void upgradeThis() {}
    void fightStart(bool midFight);
    voidx* clone() const { return new voidx(upgraded_); }
    std::unique_ptr<card> cloneU() const { return std::make_unique<voidx>(upgraded_); }
    int getCardIDNum() const { return 131; }
    ~voidx() {}
};
#endif

