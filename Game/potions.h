#ifndef POTION_H
#define POTION_H
#include "top.h"

class ancientPotion : public potion
{
private:
    int artifactNumber{ 1 };
public:
    ancientPotion() : potion("Ancient Pot", Uncommon, potionUse::SelfCombat,1, true, false) {}
    ~ancientPotion() {}
    void execute();
    ancientPotion* clone() const { return new ancientPotion(); }
    std::unique_ptr<potion> cloneU() const { return std::make_unique<ancientPotion>(); }
    int getPotionIDNum() const { return 1; }
};
class attackPotion : public potion
{
public:
    attackPotion() : potion("Attack Pot", Common, potionUse::SelfCombat, 3, false, true) {}
    ~attackPotion() {}
    void execute();
    attackPotion* clone() const { return new attackPotion(); }
    std::unique_ptr<potion> cloneU() const { return std::make_unique<attackPotion>(); }
    int getPotionIDNum() const { return 2; }
    combatChoices getCombatChoices() const { return combatChoices::Choice3Skip; }
};
class blessingOFPotion : public potion
{
public:
    blessingOFPotion() : potion("Blessing of the Forge", Common, potionUse::SelfCombat, 1, false, true) {}
    ~blessingOFPotion() {}
    void execute();
    blessingOFPotion* clone() const { return new blessingOFPotion(); }
    std::unique_ptr<potion> cloneU() const { return std::make_unique<blessingOFPotion>(); }
    int getPotionIDNum() const { return 3; }
};
class blockPotion : public potion
{
private:
    int blockNumber{ 12 };
public:
    blockPotion() : potion("Block Pot", Common, potionUse::SelfCombat, 3, false, true) {}
    ~blockPotion() {}
    void execute();
    blockPotion* clone() const { return new blockPotion(); }
    std::unique_ptr<potion> cloneU() const { return std::make_unique<blockPotion>(); }
    int getPotionIDNum() const { return 4; }
};
class bloodPotion : public potion
{
private:
    int healPercentageNumber{ 20 };
public:
    bloodPotion() : potion("Blood Pot", Common, potionUse::SelfAny, 4, false, true) {}
    ~bloodPotion() {}
    void execute();
    bloodPotion* clone() const { return new bloodPotion(); }
    std::unique_ptr<potion> cloneU() const { return std::make_unique<bloodPotion>(); }
    int getPotionIDNum() const { return 5; }
};
class colourlessPotion : public potion
{
private:
public:
    colourlessPotion() : potion("Colourless Pot", Common, potionUse::SelfCombat, 2, false, false) {}
    ~colourlessPotion() {}
    void execute();
    colourlessPotion* clone() const { return new colourlessPotion(); }
    std::unique_ptr<potion> cloneU() const { return std::make_unique<colourlessPotion>(); }
    int getPotionIDNum() const { return 6; }
    combatChoices getCombatChoices() const { return combatChoices::Choice3Skip; }
};
class cultistPotion : public potion
{
private:
    int ritualNumber{ 1 };
public:
    cultistPotion() : potion("Cultist Pot", Rare, potionUse::SelfCombat, 4, false, true) {}
    ~cultistPotion() {}
    void execute();
    cultistPotion* clone() const { return new cultistPotion(); }
    std::unique_ptr<potion> cloneU() const { return std::make_unique<cultistPotion>(); }
    int getPotionIDNum() const { return 7; }
};
class dexterityPotion : public potion
{
private:
    int dexterityNumber{ 2 };
public:
    dexterityPotion() : potion("Dexterity Pot", Common, potionUse::SelfCombat, 2, false, true) {}
    ~dexterityPotion() {}
    void execute();
    dexterityPotion* clone() const { return new dexterityPotion(); }
    std::unique_ptr<potion> cloneU() const { return std::make_unique<dexterityPotion>(); }
    int getPotionIDNum() const { return 8; }
};
class distilledChaos : public potion
{
private:
    int playNumber{ 3 };
public:
    distilledChaos() : potion("Distilled Chaos", Uncommon, potionUse::SelfCombat, 3, false, false) {}
    ~distilledChaos() {}
    void execute();
    distilledChaos* clone() const { return new distilledChaos(); }
    std::unique_ptr<potion> cloneU() const { return std::make_unique<distilledChaos>(); }
    int getPotionIDNum() const { return 9; }
    combatChoices getCombatChoices() const { return combatChoices::Havoc; }
};
class duplicationPotion : public potion
{
private:
    int playNumber{ 1 };
public:
    duplicationPotion() : potion("Duplication Pot", Uncommon, potionUse::SelfCombat, 3, false, false) {}
    ~duplicationPotion() {}
    void execute();
    duplicationPotion* clone() const { return new duplicationPotion(); }
    std::unique_ptr<potion> cloneU() const { return std::make_unique<duplicationPotion>(); }
    int getPotionIDNum() const { return 10; }
};
class elixir : public potion
{
private:
    int playNumber{ 1 };
public:
    elixir() : potion("Elixir", Uncommon, potionUse::SelfCombat, 1, true, false) {}
    ~elixir() {}
    void execute();
    elixir* clone() const { return new elixir(); }
    std::unique_ptr<potion> cloneU() const { return std::make_unique<elixir>(); }
    int getPotionIDNum() const { return 11; }
    combatChoices getCombatChoices() const { return combatChoices::HandMultiAll; }
};
class energyPotion : public potion
{
private:
    int playNumber{ 1 };
public:
    energyPotion() : potion("Energy Pot", Common, potionUse::SelfCombat, 2, false, true) {}
    ~energyPotion() {}
    void execute();
    energyPotion* clone() const { return new energyPotion(); }
    std::unique_ptr<potion> cloneU() const { return std::make_unique<energyPotion>(); }
    int getPotionIDNum() const { return 12; }
};
class entropicBrew : public potion
{
private:
    int playNumber{ 1 };
public:
    entropicBrew() : potion("Entropic Brew", Rare, potionUse::SelfAny, 5, false, true) {}
    ~entropicBrew() {}
    void execute();
    entropicBrew* clone() const { return new entropicBrew(); }
    std::unique_ptr<potion> cloneU() const { return std::make_unique<entropicBrew>(); }
    int getPotionIDNum() const { return 13; }
};
class essenceOfSteel : public potion
{
private:
    int platedNumber{ 4 };
public:
    essenceOfSteel() : potion("Essence Of Steel", Uncommon, potionUse::SelfCombat, 2, false, true) {}
    ~essenceOfSteel() {}
    void execute();
    essenceOfSteel* clone() const { return new essenceOfSteel(); }
    std::unique_ptr<potion> cloneU() const { return std::make_unique<essenceOfSteel>(); }
    int getPotionIDNum() const { return 14; }
};
class explosivePotion : public potion
{
private:
    int damageNumber{ 10 };
public:
    explosivePotion() : potion("Explosive Pot", Common, potionUse::SelfCombat, 3, false, true) {}
    ~explosivePotion() {}
    void execute();
    explosivePotion* clone() const { return new explosivePotion(); }
    std::unique_ptr<potion> cloneU() const { return std::make_unique<explosivePotion>(); }
    int getPotionIDNum() const { return 15; }
};
class fairyInABottle : public potion, public observer
{
private:
public:
    fairyInABottle(focusedEvent* fe = nullptr) : observer(fe, "fairy", 1000), potion("Fairy In A Bottle", Rare, potionUse::NonActive, 4, false, true) {}
    ~fairyInABottle() {}
    void pickUp();
    void triggered();
    void execute();
    fairyInABottle* clone() const {
        if (observing_ == nullptr) return new fairyInABottle(nullptr);
        auto temp = new fairyInABottle(observing_); this->observing_->addObserver(temp); return temp; }
    std::unique_ptr<potion> cloneU() const { return std::make_unique<fairyInABottle>(); }
    int getPotionIDNum() const { return 16; }
};
class fearPotion : public potion
{
private:
    int vulnNumber{ 3 };
public:
    fearPotion() : potion("Fear Pot", Common, potionUse::Enemy, 3, true, true) {}
    ~fearPotion() {}
    void execute();
    fearPotion* clone() const { return new fearPotion(); }
    std::unique_ptr<potion> cloneU() const { return std::make_unique<fearPotion>(); }
    int getPotionIDNum() const { return 17; }
};
class firePotion : public potion
{
private:
    int damageNumber{ 20 };
public:
    firePotion() : potion("Fire Pot", Common, potionUse::Enemy, 3, false, true) {}
    ~firePotion() {}
    void execute();
    firePotion* clone() const { return new firePotion(); }
    std::unique_ptr<potion> cloneU() const { return std::make_unique<firePotion>(); }
    int getPotionIDNum() const { return 18; }
};
class flexPotion : public potion
{
private:
    int strengthNumber{ 5 };
public:
    flexPotion() : potion("Flex Pot", Common, potionUse::SelfCombat, 3, true, true) {}
    ~flexPotion() {}
    void execute();
    flexPotion* clone() const { return new flexPotion(); }
    std::unique_ptr<potion> cloneU() const { return std::make_unique<flexPotion>(); }
    int getPotionIDNum() const { return 19; }
};
class fruitJuice : public potion
{
private:
    int HPNumber{ 5 };
public:
    fruitJuice() : potion("Fruit Juice", Rare, potionUse::SelfAny, 3, false, true) {}
    ~fruitJuice() {}
    void execute();
    fruitJuice* clone() const { return new fruitJuice(); }
    std::unique_ptr<potion> cloneU() const { return std::make_unique<fruitJuice>(); }
    int getPotionIDNum() const { return 20; }
};
class gamblersBrew : public potion
{
private:
public:
    gamblersBrew() : potion("Gambler's Brew", Uncommon, potionUse::SelfCombat, 1, true, false) {}
    ~gamblersBrew() {}
    void execute();
    gamblersBrew* clone() const { return new gamblersBrew(); }
    std::unique_ptr<potion> cloneU() const { return std::make_unique<gamblersBrew>(); }
    int getPotionIDNum() const { return 21; }
    combatChoices getCombatChoices() const { return combatChoices::HandMultiAll; }
};
class heartOfIron : public potion
{
private:
    int metalNumber{ 6 };
public:
    heartOfIron() : potion("Heart Of Iron", Rare, potionUse::SelfCombat, 4, false, true) {}
    ~heartOfIron() {}
    void execute();
    heartOfIron* clone() const { return new heartOfIron(); }
    std::unique_ptr<potion> cloneU() const { return std::make_unique<heartOfIron>(); }
    int getPotionIDNum() const { return 22; }
};
class liquidBronze : public potion
{
private:
    int thornsNumber{ 3 };
public:
    liquidBronze() : potion("Liquid Bronze", Uncommon, potionUse::SelfCombat, 3, true, true) {}
    ~liquidBronze() {}
    void execute();
    liquidBronze* clone() const { return new liquidBronze(); }
    std::unique_ptr<potion> cloneU() const { return std::make_unique<liquidBronze>(); }
    int getPotionIDNum() const { return 23; }
};
class liquidMemories : public potion
{
private:
public:
    liquidMemories() : potion("Liquid Memories", Uncommon, potionUse::SelfCombat, 3, true, true) {}
    ~liquidMemories() {}
    void execute();
    liquidMemories* clone() const { return new liquidMemories(); }
    std::unique_ptr<potion> cloneU() const { return std::make_unique<liquidMemories>(); }
    int getPotionIDNum() const { return 24; }
    combatChoices getCombatChoices() const { return combatChoices::Discard; }
};
class powerPotion : public potion
{
private:
public:
    powerPotion() : potion("Power Pot", Common, potionUse::SelfCombat, 2, true, false) {}
    ~powerPotion() {}
    void execute();
    powerPotion* clone() const { return new powerPotion(); }
    std::unique_ptr<potion> cloneU() const { return std::make_unique<powerPotion>(); }
    int getPotionIDNum() const { return 25; }
    combatChoices getCombatChoices() const { return combatChoices::Choice3Skip; }
};
class regenPotion : public potion
{
private:
    int regenNumber{ 5 };
public:
    regenPotion() : potion("Regen Pot", Uncommon, potionUse::SelfCombat, 3, false, true) {}
    ~regenPotion() {}
    void execute();
    regenPotion* clone() const { return new regenPotion(); }
    std::unique_ptr<potion> cloneU() const { return std::make_unique<regenPotion>(); }
    int getPotionIDNum() const { return 26; }
};
class skillPotion : public potion
{
private:
public:
    skillPotion() : potion("Skill Pot", Common, potionUse::SelfCombat, 3, false, false) {}
    ~skillPotion() {}
    void execute();
    skillPotion* clone() const { return new skillPotion(); }
    std::unique_ptr<potion> cloneU() const { return std::make_unique<skillPotion>(); }
    int getPotionIDNum() const { return 27; }
    combatChoices getCombatChoices() const { return combatChoices::Choice3Skip; }
};
class smokeBomb : public potion
{
private:
public:
    smokeBomb() : potion("Snecko Pot", Rare, potionUse::SelfCombat, 2, true, true) {}
    ~smokeBomb() {}
    void execute();
    smokeBomb* clone() const { return new smokeBomb(); }
    std::unique_ptr<potion> cloneU() const { return std::make_unique<smokeBomb>(); }
    int getPotionIDNum() const { return 28; }
};
class sneckoOil : public potion
{
private:
    int drawNumber{ 5 };
public:
    sneckoOil() : potion("Snecko Oil", Rare, potionUse::SelfCombat, 3, true, false) {}
    ~sneckoOil() {}
    void execute();
    sneckoOil* clone() const { return new sneckoOil(); }
    std::unique_ptr<potion> cloneU() const { return std::make_unique<sneckoOil>(); }
    int getPotionIDNum() const { return 29; }
};
class speedPotion : public potion
{
private:
    int dexNumber{ 5 };
public:
    speedPotion() : potion("Speed Pot", Common, potionUse::SelfCombat, 2, true, true) {}
    ~speedPotion() {}
    void execute();
    speedPotion* clone() const { return new speedPotion(); }
    std::unique_ptr<potion> cloneU() const { return std::make_unique<speedPotion>(); }
    int getPotionIDNum() const { return 30; }
};
class strengthPotion : public potion
{
private:
    int strNumber{ 2 };
public:
    strengthPotion() : potion("Strength Pot", Common, potionUse::SelfCombat, 3, false, true) {}
    ~strengthPotion() {}
    void execute();
    strengthPotion* clone() const { return new strengthPotion(); }
    std::unique_ptr<potion> cloneU() const { return std::make_unique<strengthPotion>(); }
    int getPotionIDNum() const { return 31; }
};
class swiftPotion : public potion
{
private:
    int drawNumber{ 3 };
public:
    swiftPotion() : potion("Swift Pot", Common, potionUse::SelfCombat, 2, false, false) {}
    ~swiftPotion() {}
    void execute();
    swiftPotion* clone() const { return new swiftPotion(); }
    std::unique_ptr<potion> cloneU() const { return std::make_unique<swiftPotion>(); }
    int getPotionIDNum() const { return 32; }
};
class weakPotion : public potion
{
private:
    int weakNumber{ 3 };
public:
    weakPotion() : potion("Weak Pot", Common, potionUse::Enemy, 2, true, true) {}
    ~weakPotion() {}
    void execute();
    weakPotion* clone() const { return new weakPotion(); }
    std::unique_ptr<potion> cloneU() const { return std::make_unique<weakPotion>(); }
    int getPotionIDNum() const { return 33; }
};
#endif