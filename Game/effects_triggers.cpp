#include "effects_triggers.h"
#include "ic_cards.h"
#include "relic.h"

using namespace std;

// useful functions for cards to use in their execute function: dealing damage, applying buffs/debuffs etc.
namespace effects
{
    void Damage(character& target, const int& amount) { target.takeDamageNoTrigger(amount); }
    bool enemyAdjustedDamage(character& enemy_, const int& amount_)
    {
        apci.getPlayer().incAtt = clamp(amount_ + enemy_.getTotalStrength(), 0, 10000);
        gm.callAlterBaseADFromEnemy();

        if (enemy_.getWeak()) apci.getPlayer().incAtt = static_cast<int>(apci.getPlayer().incAtt * enemy_.getWeakMulti());
        apci.getPlayer().incAtt = static_cast<int>(apci.getPlayer().incAtt * ((apci.getPlayer().getVulnerable() ? apci.getPlayer().getVulnerableMulti() : 1) + (enemy_.getBackAttack() ? 0.5 : 0)));

        gm.callAlterAdjustedADFromEnemy();

        return apci.getPlayer().takeDamage(static_cast<int>(apci.getPlayer().incAtt), enemy_);
    }
    bool playerAdjustedDamage(character& target, const int& amount_)
    {
        if (target.getHealth() == 0) return true;
        apci.getPlayer().outAtt = clamp(amount_ + apci.getPlayer().getTotalStrength(), 0, 10000);
        gm.callAlterBaseADFromPlayer();

        if (gm.getDAM()) apci.getPlayer().outAtt *= 2;
        if (apci.getPlayer().getWeak()) apci.getPlayer().outAtt = static_cast<int>(apci.getPlayer().outAtt *  apci.getPlayer().getWeakMulti());
        if (target.getVulnerable()) apci.getPlayer().outAtt = static_cast<int>(apci.getPlayer().outAtt * target.getVulnerableMulti());
        if (target.getSlowed()) apci.getPlayer().outAtt = static_cast<int>(apci.getPlayer().outAtt * ((static_cast<float>(target.getSlowed() + 10) / 10.0f)));

        gm.callAlterAdjustedADFromPlayer();

        return target.takeDamage(static_cast<int>(apci.getPlayer().outAtt), apci.getPlayer());
    }
    bool singleTargetMulti(character& self, character& target, const int& amount, const int& times)
    {
        for (int i{ 0 }; i < times; ++i)
        {
            &self == &apci.getPlayer() ? playerAdjustedDamage(target, amount) : enemyAdjustedDamage(self, amount);
            if (self.getHealth() == 0 || target.getHealth() == 0) return true;
        }
        return false;
    }
    bool randomDamage(const int& amount)
    {
        if (gm.getAllEnemies().empty()) return true;
        vector<int> validIndices;
        for (int i{ 0 }; i < gm.getAllEnemies().size(); ++i)
        {
            if (gm.getAllEnemies()[i]->getHealth() != 0) validIndices.push_back(i);
        }
        if (validIndices.size() == 0) return true;
        int res = random_int(0, validIndices.size() - 1);
        Damage(*gm.getAllEnemies()[validIndices[res]], amount);
        return gm.checkAllEnemyDeath();
    }
    bool randomAttackDamage(const int& amount)
    {
        if (gm.getAllEnemies().empty() || gm.checkAllEnemyDeath()) return true;
        vector<int> validIndices;
        for (int i{ 0 }; i < gm.getAllEnemies().size(); ++i)
        {
            if (gm.getAllEnemies()[i]->getHealth() != 0) validIndices.push_back(i);
        }
        if (validIndices.empty()) return true;
        int res = random_int(0, validIndices.size() - 1);
        return playerAdjustedDamage(*gm.getAllEnemies()[validIndices[res]], amount);
    }
    bool multiRandomDamage(const int& amount, const int& times)
    {
        for (int i{ 0 }; i < times; ++i)
        {
            if (randomDamage(amount)) return true;
        }
        return gm.checkAllEnemyDeath();
    }
    bool multiRandomAttackDamage(const int& amount, const int& times)
    {
        for (int i{ 0 }; i < times; ++i)
        {
            if (randomAttackDamage(amount) && gm.checkAllEnemyDeath()) return true;
        }
        return gm.checkAllEnemyDeath();
    }
    bool AOEAttackDamge(const int& amount_)
    {
        apci.getPlayer().outAtt = clamp(amount_ + apci.getPlayer().getTotalStrength(), 0, 10000);;
        gm.callAlterBaseADFromPlayer();

        if (gm.getDAM()) apci.getPlayer().outAtt *= 2;
        if (apci.getPlayer().getWeak()) apci.getPlayer().outAtt = static_cast<int>(apci.getPlayer().outAtt * apci.getPlayer().getWeakMulti());

        gm.callAlterAdjustedADFromPlayer();
        for (int i = static_cast<int>(gm.getAllEnemies().size()) - 1; i >= 0; --i)
        {
            if (gm.getAllEnemies()[i]->getHealth() == 0) continue;
            int dmg = static_cast<int>(apci.getPlayer().outAtt);
            if (gm.getAllEnemies()[i]->getVulnerable()) dmg = static_cast<int>(dmg * gm.getAllEnemies()[i]->getVulnerableMulti());
            gm.getAllEnemies()[i]->takeDamage(dmg, apci.getPlayer());
        }
        return false;
    }
    bool multiAOEAttackDamage(const int& amount, const int& times)
    {
        for (int i{ 0 }; i < times; ++i)
        {
            AOEAttackDamge(amount);
            if (gm.checkAllEnemyDeath()) return true;
        }
        return false;
    }
    bool AOEDamage(const int& amount)
    {
        for (int i = static_cast<int>(gm.getAllEnemies().size()) - 1; i >= 0; --i)
        {
            if(gm.getAllEnemies()[i]->getHealth() != 0) gm.getAllEnemies()[i]->takeDamageNoTrigger(amount);
        }
        return gm.checkAllEnemyDeath();
    }
    void Block(character& target, const int& amount) { target.gainBlock(amount); }
    void AdjustedBlock(const int& amount_)
    {
        if (apci.getPlayer().getNoBlock() > 0) return;
        float amount = static_cast<float>(clamp(amount_ + apci.getPlayer().getTotalDexterity(), 0, 10000));
        if (apci.getPlayer().getFrail()) amount *= apci.getPlayer().getFrailMulti();
        apci.getPlayer().gainBlock(static_cast<int>(amount));
    }
    void Vulnerable(character& target, const int& amount) { target.applyVulnerable(amount); }
    void Weak(character& target, const int& amount) { target.applyWeak(amount); }
    void Frail(character& target, const int& amount) { target.applyFrail(amount); }
    void Entangle(character& target, const int& amount) { target.applyEntangle(amount); }
    void Strength(character& target, const int& amount) { amount > 0 ? target.gainStrength(amount) : target.loseStrength(-amount); }
    void Dexterity(character& target, const int& amount) { amount > 0 ? target.gainDexterity(amount) : target.loseDexterity(-amount); }
    void upgradeCard(cardHolder& target)
    {
        (*target).upgradeThis();
    }
    void upgradeRandomCard()
    {
        cardDeckCH::iterator itr{ apci.getHand().begin() };
        vector<cardDeckCH::iterator> opts;
        while (itr != apci.getHand().end())
        {
            if ((**itr).getType() == Status || (**itr).getType() == Curse || (**itr).getUpgraded()) { ++itr; continue; }
            opts.push_back(itr);
            ++itr;
        }
        if (opts.size() == 0) return;
        (**opts[random_int(0, opts.size() - 1)]).upgradeThis();
    }
    //void upgradeCards(cardDeck&) {}
    void upgradeCards(cardDeckCH& deck)
    {
        for (int i{ 0 }; i < deck.size(); ++i)
        {
            (*deck[i]).upgradeThis();
        }
    }
    void Exhaust(cardHolder target, bool duped)
    {
        if (!duped) apci.addCardToExhaust(target);
        gm.callCardExhausted(target);
    }
    void exhaustHandCard(cardHolder target)
    {
        for (int i{ 0 }; i < apci.getHand().size(); ++i)
        {
            if (target == apci.getHand()[i])
            {
                auto temp = target;
                apci.removeHandCard(apci.getHand().begin() + i);
                Exhaust(temp);
                break;
            }
        }
    }
    void exhaustHand()
    {
        auto handCopy = apci.getHand();
        while (!apci.getHand().empty()) { apci.removeHandCard(apci.getHand().begin()); }
        while (!handCopy.empty())
        {
            Exhaust(handCopy[0]);
            handCopy.pop_front();
        }
    }
    void exhaustDrawPileCard(cardHolder target)
    {
        for (int i{ 0 }; i < apci.getDrawPile().size(); ++i)
        {
            if (target == apci.getDrawPile()[i])
            {
                auto temp = apci.getDrawPile()[i];
                apci.removeDrawCard(apci.getDrawPile().begin() + i);
                Exhaust(temp);
            }
        }
    }
}

removeBuffVisTrigger::removeBuffVisTrigger(Intents i) : buff(i), observer(gm.getPlayerTurnEnd(), "removeBuff") {}
void removeBuffVisTrigger::triggered() { buff.value = 0; apci.getPlayer().addAltBuffs(buff); apci.getPlayer().removeObserver(this); }

gainEnergyTrigger::gainEnergyTrigger() : observer(gm.getPlayerTurnStart(), "gainEnergy") {}
void gainEnergyTrigger::triggered() { gm.increasePlayerEnergy(nrg); }

intangibleTriggerNonPC::intangibleTriggerNonPC() : observer(gm.getAlterAdjustedADFromPlayer(), "enemyIntangible", 1001) {}
void intangibleTriggerNonPC::triggered() { if (apci.getPlayer().outAtt > 0) apci.getPlayer().outAtt = 1; }

intangibleTriggerPC::intangibleTriggerPC() : observer(gm.getAlterAdjustedADFromEnemy(), "playerIntangible", 1001) {}
void intangibleTriggerPC::triggered() { if (apci.getPlayer().incAtt > 0) apci.getPlayer().incAtt = 1; }

void gainBlockTrigger::triggered() 
{
    effects::Block(apci.getPlayer(), blockNumber); 
    if (delAfter)
    {
        apci.getPlayer().safeRemoveObserver(this);
        delete this;
    }
}

loseLifeTrigger::loseLifeTrigger(int i) : lifeLoss(i), observer(gm.getPlayerTurnStart(), "loseLife") {}
void loseLifeTrigger::triggered() { apci.getPlayer().loseHealth(lifeLoss); }

healHPTrigger::healHPTrigger(int i) : lifeGain(i), observer(gm.getFightEnd(), "lifeGain") {}
void healHPTrigger::triggered() { apci.getPlayer().heal(lifeGain); }

void goldTrigger::triggered() { apci.gainGold(goldNum); }
void goldTrigger::triggered(character& enemy) { if (!enemy.isMinion()) triggered(); }


void painStabTrigger::triggered() { apci.addCardToDiscard(cardHolder(gm.getStatusCardByID("Wound_ID")->clone())); }

void mawBankSwitchTrigger::triggered()
{
    mawBank::offSwitch = true; 
    apci.getPlayer().safeRemoveObserver(this);
    delete this;
}

void combustTrigger::triggered()
{
    apci.getPlayer().loseHealth(lhp);
    effects::AOEDamage(dmg); 
}

void constrictedTrigger::triggered()
{
    apci.getPlayer().takeDamageNoTrigger(dmg);
}

void feelNoPainTrigger::triggered() { effects::Block(apci.getPlayer(), n_); }

void magnetismTrigger::triggered()
{
    auto c_s = gm.getXRandomColourlessCards(n_);
    for (int i{ 0 }; i < c_s.size(); ++i)
    {
        auto c = cardHolder(c_s[i]->second->clone());
        (*c).fightStart(true);
        apci.addCardToHand(c);
    }
}

void mayhemTrigger::triggered() {playTopXCards(n_, false);}

void panacheTrigger::triggered() 
{
    if (++apci.getPlayer().getPanacheCounter() == 5)
    {
        effects::AOEDamage(n_);
        apci.getPlayer().getPanacheCounter() = 0;
    }
    apci.getPlayer().addAltBuffs(Intents("PAN", n_, apci.getPlayer().getPanacheCounter()));
}

void sadisticTrigger::triggered(character& enemy) {effects::Damage(enemy, n_);}

void bombTrigger::triggered()
{
    vector<pair<int,int>>* bombStuff = &apci.getPlayer().getBomb();
    if (--turns != 0)
    {
        for (int i{ 0 }; i < bombStuff->size(); ++i)
        {
            if ((*bombStuff)[i].first == dmg && (*bombStuff)[i].second == turns + 1)
            {
                (*bombStuff)[i].second = turns;
                break;
            }
        }
        apci.getPlayer().resetBombBuffs();
    }
    else
    {
        effects::AOEDamage(dmg);
        for (int i{ 0 }; i < bombStuff->size(); ++i)
        {
            if ((*bombStuff)[i].first == dmg && (*bombStuff)[i].second == turns + 1)
            {
                (*bombStuff)[i].first = 0;
                (*bombStuff)[i].second = 0;
                break;
            }
        }
        apci.getPlayer().addAltBuffs(Intents("BOMB" + to_string(bombStuff->size() - 1), 0, 0));
        apci.getPlayer().resetBombBuffs();
        apci.getPlayer().safeRemoveObserver(this);
        delete this;
    }
}


void increaseADFromPlayerTrigger::triggered()
{
    apci.getPlayer().outAtt += dmg;
    apci.getPlayer().safeRemoveObserver(this);
    delete this;
}

void ritualDaggerFatalPermTrigger::triggered(character& c) 
{
    if (c.getSCRNIndex() == gamestate::lastAttackedSCRNIndex)
    {
        for (auto itr = apci.getDeck().begin(); itr != apci.getDeck().end(); ++itr)
        {
            if ((*itr)->getCardIDNum() == 87)
            {
                if (static_cast<ritualDagger>(itr->get()).getDamageNumber() == this->dmgForSearch)
                {
                    (*itr)->incrementEnergy();
                    break;
                }
            }
        }
    }
}

void hexTrigger::triggered() { apci.addCardToDrawRandom(cardHolder(gm.getStatusCardByID("Dazed_ID")->clone())); }
void hexTrigger::triggered(cardHolder& ch)
{
    if ((*ch).getType() != Attack) triggered();
}

void maxHPTrigger::triggered() { apci.getPlayer().gainMaxHP(lifeGain); }
void maxHPTrigger::triggered(character& enemy) { if (!enemy.isMinion()) triggered(); }


drawCardTrigger::drawCardTrigger(int i) : drawNumber(i), observer(gm.getPlayerTurnStart(), "drawCard") {}
void drawCardTrigger::triggered() { apci.drawX(drawNumber); }
void drawCardTrigger::triggered(cardHolder& ch)
{
    if (evolveCheck && (*ch).getType() == cardType::Status) triggered();
    if (!evolveCheck) triggered();
}

firebTrigger::firebTrigger(int i) : damageNumber(i), observer(gm.getPlayerTurnStart(), "FB") {}
void firebTrigger::triggered(cardHolder& ch)
{
    if ((*ch).getType() == cardType::Status || (*ch).getType() == cardType::Curse) triggered();
}

//void dupDBLTStartTrigger::triggered(cardHolder& ch)
//{
//    if (apci.getPlayer().getDUPDoubleTap() == 0)
//    {
//        if (DUPe->lookFor.empty())
//        {
//            apci.getPlayer().removeObserver(DUPe);
//            apci.getPlayer().safeRemoveObserver(this);
//            delete this;
//        }
//        else {return;}
//    }
//    if (find(DUPe->lookFor.begin(), DUPe->lookFor.end(), ch.getCardPointer()) != DUPe->lookFor.end()) { DUPe->AC = false; }
//    else { DUPe->AC = true; DUPe->lookFor.push_back(ch.getCardPointer()); --apci.getPlayer().getDUPDoubleTap(); }
//}
//void dupDBLTEndTrigger::triggered(cardHolder& ch)
//{
//    auto it = find(lookFor.begin(), lookFor.end(), ch.getCardPointer());
//    if (AC) { if (it != lookFor.end()) { lookFor.erase(it); AC = !lookFor.empty(); playCardDup(ch); } }
//    else { AC = true; }
//}
//void dupPotStartTrigger::triggered(cardHolder& ch)
//{
//    if (apci.getPlayer().getDUPPotion() == 0)
//    {
//        if (DUPe->lookFor.empty())
//        {
//            apci.getPlayer().removeObserver(DUPe);
//            apci.getPlayer().safeRemoveObserver(this);
//            delete this;
//        }
//        else {return;}
//    }
//    else
//    {
//        if (find(DUPe->lookFor.begin(), DUPe->lookFor.end(), ch.getCardPointer()) != DUPe->lookFor.end()) { DUPe->AC = false; }
//        else { DUPe->AC = true; DUPe->lookFor.push_back(ch.getCardPointer()); --apci.getPlayer().getDUPPotion(); }
//    }
//}
//void dupPotEndTrigger::triggered(cardHolder& ch)
//{
//    auto it = find(lookFor.begin(), lookFor.end(), ch.getCardPointer());
//    if (AC) { if (it != lookFor.end()) { lookFor.erase(it); AC = !lookFor.empty(); playCardDup(ch); } }
//    else { AC = true; }
//}
//
//void necroStartTrigger::triggered(cardHolder& ch)
//{
//    if (DUPe->thisTurn) return;
//    else
//    {
//        if (find(DUPe->lookFor.begin(), DUPe->lookFor.end(), ch.getCardPointer()) != DUPe->lookFor.end()) { DUPe->AC = false; }
//        else
//        {
//            int eCost = (*ch).getEnergy();
//            if (eCost == -1) eCost = gm.getXCost(false);
//            if (eCost < 2) { DUPe->AC = false; return; }
//            DUPe->AC = true;
//            DUPe->lookFor.push_back(ch.getCardPointer());
//            --apci.getPlayer().getDUPPotion();
//        }
//    }
//}

void dupDBLTEndTrigger::triggered(cardHolder& ch)
{
    if (apci.getPlayer().getDUPDoubleTap() != 0 && !gamestate::dontDupe)
    {
        if (alreadyDuped == ch.getID()) return;
        alreadyDuped = ch.getID();
        apci.getPlayer().loseDUPDoubleTap(1);
        playCardDup(ch);
        if (apci.getPlayer().getDUPDoubleTap() == 0)
        {
            apci.getPlayer().safeRemoveObserver(this);
            delete this;
        }
    }
    
}
void dupPotEndTrigger::triggered(cardHolder& ch)
{
    if (apci.getPlayer().getDUPPotion() != 0 && !gamestate::dontDupe)
    {
        if (alreadyDuped == ch.getID()) return;
        alreadyDuped = ch.getID();
        apci.getPlayer().loseDUPPotion(1);
        playCardDup(ch);
        if (apci.getPlayer().getDUPPotion() == 0)
        {
            apci.getPlayer().safeRemoveObserver(this);
            delete this;
        }
    }
    
}