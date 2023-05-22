#include "potions.h"
#include "effects_triggers.h"
#include "gamestate.h"
#include "pc_info.h"
#include "character.h"

using namespace std;

bool potion::canUse() const
{
    switch (potionUsageReq_)
    {
    case potionUse::Enemy:
        return gm.getAllEnemies().size() > 0;
    case potionUse::SelfAny:
        return true;
    case potionUse::SelfCombat:
        return gm.getAllEnemies().size() > 0;
    case potionUse::NonActive:
        return false;
    }
    return false;
}

void ancientPotion::execute() { apci.getPlayer().gainArtifactCharge(artifactNumber); }
void attackPotion::execute()
{
    auto options = gm.getXRandomCardsOfType(3, Attack);
    gm.chooseFromThree(options);
}
void blessingOFPotion::execute() { effects::upgradeCards(apci.getHand()); }
void blockPotion::execute() { effects::Block(apci.getPlayer(), blockNumber); }
void bloodPotion::execute() { apci.getPlayer().healPercentage(healPercentageNumber); }
void colourlessPotion::execute()
{
    auto options = gm.getXRandomColourlessCards(3);
    gm.chooseFromThree(options);
}
void cultistPotion::execute() { apci.getPlayer().gainRitual(ritualNumber); }
void dexterityPotion::execute() { effects::Dexterity(apci.getPlayer(), dexterityNumber); }
void distilledChaos::execute()
{
    if (apci.getDrawSize() < playNumber) apci.discardToDraw();
    if (apci.getDrawSize() == 0) return;
    int i{ 0 };
    while (i < playNumber && apci.getDrawSize()>0)
    {
        apci.addCardToBackOfStack(std::move(apci.getDrawPile()[0]));
        apci.getDrawPile().pop_front();
        ++i;
    }
}
void duplicationPotion::execute()
{
    apci.getPlayer().addDUPPotion(1);
}
void elixir::execute()
{
    if(!ai) std::cout << "Select cards to exhaust (eg 1 3 4 8 9)" << endl;
    set<int> temp = getIndicesForCardBoolFlip(1, static_cast<int>(apci.getHand().size()), static_cast<int>(apci.getHand().size()));
    vector<cardHolder> temps;
    auto itr{ temp.begin() };
    int i{ 1 };
    while (itr != temp.end())
    {
        temps.push_back(apci.getHand()[*itr - i]);
        apci.removeHandCard(apci.getHand().begin() + (*itr - i));
        ++i; ++itr;
    }
    for (int j{ 0 }; j< temp.size(); ++j)
    {
        effects::Exhaust(*temps[j]);
    }
}
void energyPotion::execute() { gm.increasePlayerEnergy(2); }
void entropicBrew::execute()
{
    int newPots = apci.getPotionSlots() - static_cast<int>(apci.getPotions().size());
    if (newPots > 0)
    {
        auto pots = gm.retrieveXRandomPotions(newPots);
        for (int i{ 0 }; i < newPots; ++i) { apci.addPotion(pots[i]); }
    }
}
void essenceOfSteel::execute() { apci.getPlayer().addPlatedArmour(4); }
void explosivePotion::execute() { effects::AOEDamage(damageNumber); }
void fairyInABottle::pickUp() { observeThis(gm.getPlayerDied()); }
void fairyInABottle::execute() {}
void fairyInABottle::triggered()
{
    if (apci.getPlayer().getHealth() == 0)
    {
        apci.getPlayer().healPercentage(30);
        observing_->removeObserver(this);
        observeNull();
        apci.removeFIAB();
        std::cout << "FIAB used." << endl;
        delete this;
    }
}
void fearPotion::execute() { nonpc& target = singleTarget(); effects::Vulnerable(target, vulnNumber); }
void firePotion::execute() { nonpc& target = singleTarget(); effects::Damage(target, damageNumber); }
void flexPotion::execute()
{
    pc& target = apci.getPlayer();
    effects::Strength(target, strengthNumber);
    target.changeEotStrength(-strengthNumber);
}
void fruitJuice::execute() { apci.getPlayer().gainMaxHP(HPNumber); }
void gamblersBrew::execute()
{
    if (apci.getHand().size() == 0) return;
    if(!ai) std::cout << "Select cards to discard and redraw (eg 1 3 4 8 9)" << endl;
    set<int> temp = getIndicesForCardBoolFlip(1, static_cast<int>(apci.getHand().size()), static_cast<int>(apci.getHand().size()));
    auto itr{ temp.begin() };
    int i{ 1 };
    while (itr != temp.end())
    {
        apci.sendHandCardToDiscard(apci.getHand()[*itr - i]);
        ++i; ++itr;
    }
    apci.drawX(static_cast<int>(temp.size()));
}
void heartOfIron::execute()
{
    apci.getPlayer().addMTL(metalNumber);
    apci.getPlayer().addAltBuffs(Intents("MTL", apci.getPlayer().getMetalliciseBuff(), 1));
}
void liquidBronze::execute()
{
    apci.getPlayer().addThorns(3);
    apci.getPlayer().addAltBuffs(Intents("Thorns", apci.getPlayer().getThorns(), 1));
    apci.getPlayer().observeEvent<thornsTrigger<character>>(gm.getPlayerTakingAttackDamage(), new thornsTrigger<character>(&apci.getPlayer(), gm.getPlayerTakingAttackDamage(), 3), true);
}
void liquidMemories::execute()
{
    if (apci.getDiscardSize() == 0) return;
    cardHolder* target;
    if (apci.getDiscardSize() == 1) { target = &apci.getDiscardPile().back(); }
    else { target = &singleTargetCard(apci.getDiscardPile()); }
    apci.makeCostZero(*target);
    apci.sendCardFromToHand(apci.getDiscardPile(), *target);
}
void powerPotion::execute()
{
    auto options = gm.getXRandomCardsOfType(3, Power);
    gm.chooseFromThree(options);
}
void regenPotion::execute() { apci.getPlayer().addRegeneration(5); }
void skillPotion::execute()
{
    auto options = gm.getXRandomCardsOfType(3, Skill);
    gm.chooseFromThree(options);
}
void smokeBomb::execute()
{
    if (gm.getActNumber() == 4 && gm.getMapPathEvent() == 'E' && gm.getAllEnemies().size() > 1) return;
    if (gm.getMapPathEvent() == 'e' || gm.getMapPathEvent() == 'E')
    {
        while (!gm.getAllEnemies().empty())
        {
            gm.removeFromCombat(gm.getAllEnemies()[0].get());
        }
        gm.endFight();
        gm.callPotionUsed();
        throw SmokeBombError("escaped");
    }
}
void sneckoOil::execute()
{
    apci.drawX(drawNumber);
    for (int i{ 0 }; i < apci.getHand().size(); ++i)
    {
        (*apci.getHand()[i]).changeTempEnergy(random_int(0, 3));
    }
}
void speedPotion::execute()
{
    pc& target = apci.getPlayer();
    effects::Dexterity(target, dexNumber);
    target.changeEotDexterity(-dexNumber);
}
void strengthPotion::execute() { effects::Strength(apci.getPlayer(), strNumber); }
void swiftPotion::execute() { apci.drawX(drawNumber); }
void weakPotion::execute() { nonpc& target = singleTarget(); effects::Weak(target, weakNumber); }
