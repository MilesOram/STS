#include "top.h"
#include "card.h"
#include "character.h"
#include "effects_triggers.h"
#include "gamestate.h"
#include "pc_info.h"
#include "savestate.h"
#include "visuals.h"
#include "events.h"
#include <cassert>

using namespace std;

void focusedEvent::removeObserver(observer* o)
{
    for (auto itr{ observers.begin() }; itr != observers.end(); ++itr) 
    {
        if ((*itr) == o) { observers.erase(itr); break; }
    }
}
void focusedEvent::removeAllObservers()
{
    for (auto it{ observers.begin() }; it != observers.end(); ++it)
    {
        (*it)->observeNull();
    }
    observers.clear();
}
void focusedEvent::notify(character& initiator)
{
    observer_set currentTriggers = observers;
    observer_set::iterator obs = currentTriggers.begin();
    while (obs != currentTriggers.end())
    {
        (*obs)->triggeredACT(initiator);
        ++obs;
    }
}
void focusedEvent::notify(cardHolder& ch)
{
    observer_set currentTriggers = observers;
    observer_set::iterator obs = currentTriggers.begin();
    int lastTrig{ 0 };
    while (obs != currentTriggers.end())
    {
        lastTrig = (*obs)->getTriggerPrio();
        (*obs)->triggeredACT(ch);
        ++obs;
    }
}
void focusedEvent::notify()
{
    observer_set currentTriggers = observers;
    observer_set::iterator obs = currentTriggers.begin();
    while (obs != currentTriggers.end())
    {
        (*obs)->triggeredACT();
        ++obs;
    }
}
int cardHolder::unique_id{ 0 };
holdsObservers::holdsObservers(const holdsObservers& ho) { observers_ = ho.observers_; }
holdsObservers::holdsObservers(holdsObservers&& ho) noexcept { observers_ = ho.observers_; }
bool observerSort::operator()(observer* const& left, observer* const& right) const 
{
    if (right == left) return false;
    return left->test(*right); 
}
void holdsObservers::stopObserving(focusedEvent* fe)
{
    for (observer* o : observers_)
    {
        if (o->getObserving() == fe)
        {
            auto temp = o;
            fe->removeObserver(temp);
            for (auto itr{ observers_.begin() }; itr != observers_.end(); ++itr)
            {
                if ((*itr) == o) { observers_.erase(itr); break; }
            }
            delete temp;
            break;
        }
    }
}
void holdsObservers::removeObserver(observer* obs)
{
    obs->getObserving()->removeObserver(obs);
    for (auto itr{ observers_.begin() }; itr != observers_.end(); ++itr)
    {
        if ((*itr) == obs) { observers_.erase(itr); break; }
    }
    delete obs;
}
void holdsObservers::safeRemoveObserver(observer* obs)
{
    obs->getObserving()->removeObserver(obs);
    for (auto itr{ observers_.begin() }; itr != observers_.end(); ++itr)
    {
        if ((*itr) == obs) { observers_.erase(itr); break; }
    }
}
void holdsObservers::deleteAllObservers()
{
    while (!observers_.empty())
    {
        observer_set::iterator it = observers_.begin();
        observer* temp = *it;
        if(temp->getObserving()!=nullptr) temp->getObserving()->removeObserver(temp);
        observers_.erase(it);
        delete temp;
    }
    observers_.clear();
}
void holdsObservers::clearNullObservers()
{
    observer_set newSet;
    for (auto o : observers_)
    {
        if (o && o->getObserving()) newSet.insert(o);
    }
    observers_.clear();
    observers_ = newSet;
}
observer_set& holdsObservers::getObservers() { return observers_; }
int random_int(int range_lower, size_t range_upper)
{
    return random_int(range_lower, static_cast<int>(range_upper));
}
int random_int(int range_lower, int range_upper)
{
    if (range_lower > range_upper) throw out_of_range("rand int bad" + to_string(range_lower) + " " + to_string(range_upper));
    if (range_upper == range_lower) return range_lower;
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<std::mt19937::result_type> rnd_dist(range_lower, range_upper);
    return rnd_dist(rng);
}
int random_intNot(int range_lower, size_t range_upper, int notIncluded)
{
    return random_intNot(range_lower, static_cast<int>(range_upper), notIncluded);
}
int random_intNot(int range_lower, int range_upper, int notIncluded)
{
    if (range_lower == range_upper || (notIncluded > range_upper || notIncluded < range_lower)) throw out_of_range("rand int bad");
    if (notIncluded == range_lower) return random_int(range_lower + 1, range_upper);
    else if (notIncluded == range_upper) return random_int(range_lower, range_upper - 1);
    else
    {
        int res = random_int(range_lower, range_upper - 1);
        if (res >= notIncluded) return res + 1;
        else { return res; }
    }
}
float random_float(float range_lower, float range_upper)
{
    if (range_lower > range_upper) throw out_of_range("rand float bad" + to_string(range_lower) + " " + to_string(range_upper));
    if (range_upper == range_lower) return range_lower;
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<float> rnd_dist(range_lower, range_upper);
    return rnd_dist(rng);
}
bool checkLastXAttacks(vector<string>& va, int x, string a)
{
    if (va.size() < x) return false;
    for (int i{ 1 }; i < x + 1; ++i)
    {
        if (*(va.end() - i) != a) return false;
    }
    return true;
}
bool checkLastX(vector<char>& va, int x, char a, int index)
{
    if (va.size() < x) return false;
    for (int i{ 1 }; i < x + 1; ++i)
    {
        if (*(va.begin() + index - i) != a) return false;
    }
    return true;
}

int factorial(const int& x)
{
    if (x <= 1) return 1;
    return factorial(x - 1) * x;
}
int NchooseR(const int& n, const int& r)
{
    return factorial(n) / (factorial(r) * factorial(n - r));
}

bool recIter(int s, int c, int level, int lowerbound, int indx, int& start, vector<int>& res)
{
    for (int i{ lowerbound }; i < s; ++i)
    {
        if (indx == start && level == c - 1)
        {
            res[level] = i + 1;
            return true;
        }
        if (level < c - 1)
        {
            if (recIter(s, c, level + 1, i + 1, indx, start, res))
            {
                res[level] = i + 1;
                return true;
            }
        }
        if (level == c - 1) ++start;
    }
    return false;
}

vector<int> baseIter(int s, int c, int indx)
{
    int start{ 0 };
    vector<int> res(c);
    recIter(s, c, 0, 0, indx, start, res);
    return res;
}
void stateInfo::createAllVars(bool dataAugment)
{
    createPart1(dataAugment);
    createPart2(dataAugment);
}
void stateInfo::createPart1(bool dataAugment)
{
    handCardIDs.clear(); handCardEnergy.clear(); handCardUpgraded.clear(); drawCountPerCard.clear(); discardCountPerCard.clear(); exhaustCountPerCard.clear(); masksForHandCards.clear();
    playerVars.clear(); relicPresent.clear(); relicCounters.clear(); enemyAttackID.clear(); pastEnemyAttackIDs.clear(); enemyVars.clear(); incomingAttackDamage.clear(); potionIDs.clear();
    masksForHandCards = vector<bool>(296, false); // 261 + 33
    bool already = gamestate::denyCardPlay;
    if (gm.cardCount >= 3 && apci.checkCardInHand("Normality")) { gamestate::denyCardPlay = true; }
    for (auto itr{ apci.getHand().begin() }; itr != apci.getHand().end(); ++itr)
    {

        handCardIDs.push_back((**itr).getCardIDNum());
        handCardEnergy.push_back((**itr).getEnergy());
        handCardUpgraded.push_back((**itr).getUpgraded());
        if (!masksForHandCards[(((**itr).getCardIDNum() - 1) * 2 + ((**itr).getUpgraded() ? 1 : 0))])
        {
            masksForHandCards[(((**itr).getCardIDNum() - 1) * 2 + ((**itr).getUpgraded() ? 1 : 0))] = (**itr).castable() && (**itr).getEnergy() <= gm.getPlayerEnergy();
        }
    }
    if (!already) gamestate::denyCardPlay = false;
    while (handCardIDs.size() < 10)
    {
        handCardIDs.push_back(0);
        handCardEnergy.push_back(0);
        handCardUpgraded.push_back(0);
    }
    if (apci.getTopCardKnown() && apci.getDrawSize() > 0)
    {
        handCardIDs.push_back((*apci.getDrawPile()[0]).getCardIDNum());
        handCardEnergy.push_back((*apci.getDrawPile()[0]).getEnergy());
        handCardUpgraded.push_back((*apci.getDrawPile()[0]).getUpgraded());
    }
    else
    {
        handCardIDs.push_back(0);
        handCardEnergy.push_back(0);
        handCardUpgraded.push_back(0);
    }
    if (dataAugment)
    {
        std::random_device rd;
        std::mt19937 g(rd());
        auto g2 = g;
        auto g3 = g;

        std::shuffle(handCardIDs.begin(), handCardIDs.end(), g);
        std::shuffle(handCardEnergy.begin(), handCardEnergy.end(), g2);
        std::shuffle(handCardUpgraded.begin(), handCardUpgraded.end(), g3);
    }

    for (int i{ 0 }; i < apci.getPotions().size(); ++i)
    {
        masksForHandCards[apci.getPotion(i)->getPotionIDNum() + 262] = apci.getPotion(i)->canUse();
    }
    //masksForHandCards[166] = (!std::any_of(masksForHandCards.begin(), masksForHandCards.end(), [](bool i) {return i; }));
    masksForHandCards[262] = true;
    drawCountPerCard = vector<int>(262);
    discardCountPerCard = vector<int>(262);
    exhaustCountPerCard = vector<int>(262);
    for (auto itr{ apci.getDrawPile().begin() }; itr != apci.getDrawPile().end(); ++itr)
    {
        ++drawCountPerCard[((**itr).getCardIDNum() - 1) * 2 + ((**itr).getUpgraded() ? 1 : 0)];
    }
    for (auto itr{ apci.getDiscardPile().begin() }; itr != apci.getDiscardPile().end(); ++itr)
    {
        ++discardCountPerCard[((**itr).getCardIDNum() - 1) * 2 + ((**itr).getUpgraded() ? 1 : 0)];
    }
    for (auto itr{ apci.getExhausted().begin() }; itr != apci.getExhausted().end(); ++itr)
    {
        ++exhaustCountPerCard[((**itr).getCardIDNum() - 1) * 2 + ((**itr).getUpgraded() ? 1 : 0)];
    }
}
void stateInfo::createPart2(bool dataAugment)
{

    pc* plyr = &apci.getPlayer();
    // ignoring multis for now(not ints)
    playerVars = {
        plyr->getHealth(),plyr->getMaxHealth(),plyr->getBlock(),plyr->playerStartEnergy_,plyr->new_weak_,plyr->new_vulnerable_,plyr->new_frail_,plyr->ragee_,
        plyr->getWeak(),plyr->getFrail(),plyr->getVulnerable(),plyr->getTotalStrength(),plyr->getTotalDexterity(),plyr->getEntangle(),plyr->confused_,
        plyr->hexed_,plyr->getNoDraw(),plyr->no_block_,plyr->getEotDexterity(),plyr->getEotStrength(),
        plyr->getMetalliciseBuff(),plyr->getPlatedArmour(),plyr->getBuffer(),plyr->getIntangible(),plyr->getRegeneration(),plyr->bomb_damage_turn_.size() == 0 ? 0 : plyr->bomb_damage_turn_[0].first,plyr->bomb_damage_turn_.size() == 0 ? 0 : plyr->bomb_damage_turn_[0].second,
        plyr->getBarricadeBuff(),plyr->getBerserkBuff(),plyr->getBrutalityBuff(),plyr->getCombustBuff(),plyr->CombustBuffStacks,plyr->getCorruptionBuff(),plyr->getDarkEmbraceBuff(),plyr->getDemonFormBuff(),plyr->getEvolveBuff(),plyr->getFeelNoPainBuff(),
        plyr->getFireBreathingBuff(),plyr->getJuggernautBuff(),plyr->getRuptureBuff(),plyr->MagnetismBuff,plyr->MayhemBuff,plyr->PanacheBuff,plyr->PanacheCounter,plyr->SadisticBuff,
        plyr->DUPPotion,plyr->DUPDoubleTap,plyr->toDrawSOT,plyr->getThorns(),plyr->getFlameBar(),plyr->getRitual(),plyr->getArtifact(),
        gm.getPlayerEnergy(),gm.turnCount,gm.getDAM(),gm.getActNumber(),gm.cardCount,gm.attackCount,gm.skillCount,plyr->getDrawReduction(),plyr->getConstricted(),
        plyr->getRedKey(), plyr->getBlueKey(), plyr->getGreenKey(),apci.getGold(),apci.getDrawSize(),apci.getDiscardSize(),apci.getExhaustedSize(),apci.getHandSize(),apci.cursesInDeck()
    };
    relicCounters = vector<int>(147);
    relicPresent = vector<int>(147);

    for (int i{ 0 }; i < apci.getRelics().size(); ++i)
    {
        relicPresent[apci.getRelics()[i]->getRelicIDNum() - 1] = 1;
        relicCounters[apci.getRelics()[i]->getRelicIDNum() - 1] = clamp(apci.getRelics()[i]->getCounter(), 0, 99);
    }

    int numPots = static_cast<int>(apci.getPotions().size());
    for (int i{ 0 }; i < 5; ++i)
    {
        if (i + 1 > numPots) { potionIDs.push_back(0); }
        else { potionIDs.push_back(apci.getPotion(i)->getPotionIDNum()); }
    }

    if (dataAugment)
    {
        std::random_device rd;
        std::mt19937 g(rd());

        std::shuffle(potionIDs.begin(), potionIDs.end(), g);
    }

    enemyAttackID = vector<int>(5);
    pastEnemyAttackIDs = vector<int>(10);
    incomingAttackDamage = vector<int>(10);
    vector<int> order;
    int holdI{ 0 };
    if (dataAugment)
    {
        std::random_device rd;
        std::mt19937 g(rd());
        order = { 0,1,2,3,4 };
        std::shuffle(order.begin(), order.end(), g);
    }

    for (int i{ 0 }; i < 5; ++i)
    {
        if (dataAugment)
        {
            holdI = i;
            i = order[i];
        }
        if (i < gm.getAllEnemies().size())
        {
            nonpc* enmy = gm.getAllEnemies()[i].get();
            enemyAttackID[i] = enmy->getIntentID();
            auto PAs = enmy->getPastAttacks();
            pastEnemyAttackIDs[i] = enmy->getIntent2ID();
            pastEnemyAttackIDs[i + 5] = enmy->getIntent3ID();
            std::vector<int> AB = {
                enmy->getHealth(), enmy->getMaxHealth(), enmy->getBlock(), enmy->getWeak(), enmy->getSharpHide(), enmy->getVulnerable(), enmy->getTotalStrength(), enmy->getTotalDexterity(),
                enmy->getMetalliciseBuff(),enmy->getPlatedArmour(),enmy->getBuffer(),enmy->getIntangible(),enmy->getRegeneration(),enmy->getEotDexterity(),enmy->getEotStrength(), enmy->getThorns(),enmy->getRitual(),enmy->getArtifact(),enmy->getEnrage(),
                enmy->getSlowed(),enmy->getDormant(),enmy->getClock12(),enmy->getCuriosity(),enmy->getInvincible(),enmy->getBeatOfDeath(),enmy->getBackAttack(),enmy->getPainfulStabs(),enmy->getDormantTurns()
            };
            enemyVars.insert(enemyVars.end(), AB.begin(), AB.end());
            for (int j{ 0 }; j < gm.getIntents(i).size(); ++j)
            {
                if (gm.getIntents(i)[j].effect == "Attack")
                {
                    incomingAttackDamage[i] = adjusted::calcEnemyDamage(*(gm.getAllEnemies()[i]), gm.getIntents(i)[j].value);
                    incomingAttackDamage[i + 1] = gm.getIntents(i)[j].times;
                    break;
                }
            }
        }
        else
        {
            enemyAttackID[i] = 0;
            pastEnemyAttackIDs[i] = 0; pastEnemyAttackIDs[i + 5] = 0;
            std::vector<int> AB(28);
            enemyVars.insert(enemyVars.end(), AB.begin(), AB.end());
            incomingAttackDamage[i] = 0; incomingAttackDamage[i + 1] = 0;
        }
        if (dataAugment) i = holdI;
    }
    dormantEnemies = vector<int>(3);
    for (int i{ 0 }; i < gm.getDormantEnemies().size(); ++i) dormantEnemies[i] = static_cast<int>(gm.getDormantEnemies()[i].size());
}
const vector<int>& stateInfo::getEmbeddingInputsCA() const { return handCardIDs; }
vector<int> stateInfo::getEmbeddingInputsEN() const
{
    vector<int> stateSpace; // 15
    stateSpace.insert(stateSpace.end(), enemyAttackID.begin(), enemyAttackID.end());
    stateSpace.insert(stateSpace.end(), pastEnemyAttackIDs.begin(), pastEnemyAttackIDs.end());
    return stateSpace;
}
const vector<int>& stateInfo::getEmbeddingInputsPO() const { return potionIDs; }
vector<int> stateInfo::getNormalInputs() const
{
    vector<int> stateSpace; // 11,11,262,262,262,70,147,147,140,10,3 = 1325 (1219 (857 (529))) -> 1309
    stateSpace.insert(stateSpace.end(), handCardEnergy.begin(), handCardEnergy.end());
    stateSpace.insert(stateSpace.end(), handCardUpgraded.begin(), handCardUpgraded.end());
    stateSpace.insert(stateSpace.end(), drawCountPerCard.begin(), drawCountPerCard.end());
    stateSpace.insert(stateSpace.end(), discardCountPerCard.begin(), discardCountPerCard.end());
    stateSpace.insert(stateSpace.end(), exhaustCountPerCard.begin(), exhaustCountPerCard.end());
    stateSpace.insert(stateSpace.end(), playerVars.begin(), playerVars.end());
    stateSpace.insert(stateSpace.end(), relicPresent.begin(), relicPresent.end());
    stateSpace.insert(stateSpace.end(), relicCounters.begin(), relicCounters.end());
    stateSpace.insert(stateSpace.end(), enemyVars.begin(), enemyVars.end());
    stateSpace.insert(stateSpace.end(), incomingAttackDamage.begin(), incomingAttackDamage.end());
    stateSpace.insert(stateSpace.end(), dormantEnemies.begin(), dormantEnemies.end());

    return stateSpace;
}
const vector<bool>& stateInfo::getMasks() const { return masksForHandCards; }
void stateInfo::reshuffleDrawAndHand()
{
    // out of engine way to approximate variance in draw order
    // need to move all hand cards back into draw, then "draw" x random cards

    int toDraw{ 0 };
    for (int i{ 0 }; i < handCardIDs.size(); ++i)
    {
        if (handCardIDs[i] == 0) break;
        ++toDraw;
        ++drawCountPerCard[(handCardIDs[i] - 1) * 2 + handCardUpgraded[i]];
    }
    vector<int> newHand;
    for (int i{ 0 }; i < drawCountPerCard.size(); ++i)
    {
        if (drawCountPerCard[i] != 0)
        {
            for (int j{ 0 }; j < drawCountPerCard[i]; ++j)
            {
                newHand.push_back(i / 2 + 1);
            }
        }
    }
    while (newHand.size() > toDraw)
    {
        //newHand.erase()
    }
}
macroStateInfo::macroStateInfo(const macroStateInfo& _msi)
{
    deckCountPerCard = _msi.deckCountPerCard; 
    playerVars = _msi.playerVars;
    relicPresent = _msi.relicPresent;
    relicCounters = _msi.relicCounters;
    potCounts = _msi.potCounts;
    openingChoice = _msi.openingChoice;
    oneHotFloor = _msi.oneHotFloor;
    gmVars = _msi.gmVars;
    nextFloor = _msi.nextFloor;
    secondFloor = _msi.secondFloor;
    thirdFloor = _msi.thirdFloor;
    mapRepVars = _msi.mapRepVars;
}
macroStateInfo& macroStateInfo::operator=(const macroStateInfo& _msi)
{
    deckCountPerCard = _msi.deckCountPerCard;
    playerVars = _msi.playerVars;
    relicPresent = _msi.relicPresent;
    relicCounters = _msi.relicCounters;
    potCounts = _msi.potCounts;
    openingChoice = _msi.openingChoice;
    oneHotFloor = _msi.oneHotFloor;
    gmVars = _msi.gmVars;
    nextFloor = _msi.nextFloor;
    secondFloor = _msi.secondFloor;
    thirdFloor = _msi.thirdFloor;
    mapRepVars = _msi.mapRepVars;
    return *this;
}
void macroStateInfo::createAllVars()
{
    deckCountPerCard.clear();
    playerVars.clear(); relicPresent.clear(); relicCounters.clear(); potCounts.clear(); gmVars.clear();
    oneHotFloor.clear();

    deckCountPerCard = vector<int>(262);
    int attackCards{ 0 }, skillCards{ 0 }, powerCards{ 0 };
    for (auto itr{ apci.getDeck().begin() }; itr != apci.getDeck().end(); ++itr)
    {
        ++deckCountPerCard[(((**itr).getCardIDNum() - 1) * 2 + ((**itr).getUpgraded() ? 1 : 0))];

        switch ((**itr).getType())
        {
        case Attack:
            ++attackCards;
            break;
        case Skill:
            ++skillCards;
            break;
        case Power:
            ++powerCards;
            break;
        }
    }
    potCounts = vector<int>(34);
    for (int i{ 0 }; i < 5; ++i)
    {
        if (i < apci.getPotions().size()) ++potCounts[apci.getPotion(i)->getPotionIDNum()];
    }

    pc* plyr = &apci.getPlayer();
    // ignoring multis for now(not ints)
    int act = gm.getActNumber();
    int floor = gm.getFloorIndex();
    int tillBoss = (act * 17 - 1) - floor;
    playerVars = {
        plyr->getHealth(),plyr->getMaxHealth(),plyr->playerStartEnergy_,attackCards,skillCards,powerCards,
        plyr->toDrawSOT,plyr->getRedKey(), plyr->getBlueKey(), plyr->getGreenKey(),apci.getGold(),apci.cursesInDeck()
    };
    gmVars = {
            gm.shrineChance,gm.enemyChance,gm.chestChance,gm.shopChance,gm.upgraded_pc,gm.common_pc,gm.uncommon_pc,
        gm.rare_pc,(gm.rare_offset > 0 ? gm.rare_offset : -gm.rare_offset), gm.common_pc_elite,gm.uncommon_pc_elite,
        gm.rare_pc_elite,gm.common_pc_potion,gm.uncommon_pc_potion,gm.rare_pc_potion,gm.current_potion_pc,gm.easyDone,
        gm.getActNumber(),act==1 ? gm.bossIndexThisAct==0 : 0,act == 1 ? gm.bossIndexThisAct == 1 : 0,act == 1 ? gm.bossIndexThisAct == 2 : 0,
        act == 2 ? gm.bossIndexThisAct == 0 : 0, act == 2 ? gm.bossIndexThisAct == 1 : 0, act == 2 ? gm.bossIndexThisAct == 2 : 0,
        act == 3 ? gm.bossIndexThisAct == 0 : 0, act == 3 ? gm.bossIndexThisAct == 1 : 0, act == 4 ? gm.bossIndexThisAct == 2 : 0,
        shop::cardRemove,gm.getFloorIndex(),tillBoss
    };

    oneHotFloor = vector<int>(55, 0);
    oneHotFloor[floor] = 1;

    relicCounters = vector<int>(147);
    relicPresent = vector<int>(147);

    for (int i{ 0 }; i < apci.getRelics().size(); ++i)
    {
        relicPresent[apci.getRelics()[i]->getRelicIDNum() - 1] = 1;
        relicCounters[apci.getRelics()[i]->getRelicIDNum() - 1] = clamp(apci.getRelics()[i]->getCounter(), 0, 99);
    }
    if(!gm.getOpeningChoice().empty()) openingChoice = gm.getOpeningChoice();
    else { openingChoice = vector<int>(24); }

}

void macroStateInfo::createPredVars()
{
    deckCountPerCard.clear();
    playerVars.clear(); relicPresent.clear(); relicCounters.clear(); potCounts.clear(); gmVars.clear();
    oneHotFloor.clear();

    deckCountPerCard = vector<int>(262);
    int attackCards{ 0 }, skillCards{ 0 }, powerCards{ 0 };
    for (auto itr{ apci.getDeck().begin() }; itr != apci.getDeck().end(); ++itr)
    {
        ++deckCountPerCard[(((**itr).getCardIDNum() - 1) * 2 + ((**itr).getUpgraded() ? 1 : 0))];

        switch ((**itr).getType())
        {
        case Attack:
            ++attackCards;
            break;
        case Skill:
            ++skillCards;
            break;
        case Power:
            ++powerCards;
            break;
        }
    }
    potCounts = vector<int>(34);
    for (int i{ 0 }; i < 5; ++i)
    {
        if (i < apci.getPotions().size()) ++potCounts[apci.getPotion(i)->getPotionIDNum()];
    }

    pc* plyr = &apci.getPlayer();
    playerVars = {
        plyr->getHealth(),plyr->getMaxHealth(),plyr->playerStartEnergy_,attackCards,skillCards,powerCards,
        plyr->toDrawSOT,plyr->getRedKey(), plyr->getBlueKey(), plyr->getGreenKey(),apci.getGold(),apci.cursesInDeck()
    };

    relicCounters = vector<int>(147);
    relicPresent = vector<int>(147);

    for (int i{ 0 }; i < apci.getRelics().size(); ++i)
    {
        relicPresent[apci.getRelics()[i]->getRelicIDNum() - 1] = 1;
        relicCounters[apci.getRelics()[i]->getRelicIDNum() - 1] = clamp(apci.getRelics()[i]->getCounter(), 0, 99);
    }
}
vector<int> macroStateInfo::getNormalInputs() const
{
    vector<int> stateSpace; // 11,11,262,262,262,70,147,147,140,10,3 = 1325 (1219 (857 (529))) -> 1309
    stateSpace.insert(stateSpace.end(), deckCountPerCard.begin(), deckCountPerCard.end());
    stateSpace.insert(stateSpace.end(), playerVars.begin(), playerVars.end());
    stateSpace.insert(stateSpace.end(), relicPresent.begin(), relicPresent.end());
    stateSpace.insert(stateSpace.end(), relicCounters.begin(), relicCounters.end());
    stateSpace.insert(stateSpace.end(), gmVars.begin(), gmVars.end());
    stateSpace.insert(stateSpace.end(), oneHotFloor.begin(), oneHotFloor.end());
    stateSpace.insert(stateSpace.end(), openingChoice.begin(), openingChoice.end());
    stateSpace.insert(stateSpace.end(), potCounts.begin(), potCounts.end());
    stateSpace.insert(stateSpace.end(), nextFloor.begin(), nextFloor.end());
    stateSpace.insert(stateSpace.end(), secondFloor.begin(), secondFloor.end());
    stateSpace.insert(stateSpace.end(), thirdFloor.begin(), thirdFloor.end());
    stateSpace.insert(stateSpace.end(), mapRepVars.begin(), mapRepVars.end());

    return stateSpace;
}
vector<int> macroStateInfo::getPredInputs() const
{
    vector<int> stateSpace; // 
    stateSpace.insert(stateSpace.end(), deckCountPerCard.begin(), deckCountPerCard.end());
    stateSpace.insert(stateSpace.end(), playerVars.begin(), playerVars.end());
    stateSpace.insert(stateSpace.end(), relicPresent.begin(), relicPresent.end());
    stateSpace.insert(stateSpace.end(), relicCounters.begin(), relicCounters.end());
    stateSpace.insert(stateSpace.end(), potCounts.begin(), potCounts.end());

    return stateSpace;
}
void macroStateInfo::copyNonMapRep(const macroStateInfo& _msi)
{
    deckCountPerCard = _msi.deckCountPerCard;
    playerVars = _msi.playerVars;
    relicPresent = _msi.relicPresent;
    relicCounters = _msi.relicCounters;
    potCounts = _msi.potCounts;
    gmVars = _msi.gmVars;
    oneHotFloor = _msi.oneHotFloor;
}
void macroStateInfo::mapRepZeroes()
{
    nextFloor = vector<int>(6);
    secondFloor = vector<int>(6);
    thirdFloor = vector<int>(6);
    mapRepVars = vector<int>(23);
}

void printCardDeck(cardDeckCH& d)
{
    if (!gamestate::frozenEyeCheck)
    {
        auto temp = d;
        sort(temp.begin(), temp.end(), [](const cardHolder& a, const cardHolder& b) {return (*a).getName() < (*b).getName(); });
        for (int i{ 0 }; i < temp.size(); ++i)
        {
            std::cout << (*temp[i]).getName();
            if ((i + 1) % 5 == 0) { std::cout << "," << endl; continue; }
            if (i != temp.size() - 1) std::cout << ", ";
        }
        std::cout << "." << endl;
    }
    else
    {
        for (int i{ 0 }; i < d.size(); ++i)
        {
            std::cout << (*d[i]).getName();
            if ((i + 1) % 5 == 0) { std::cout << "," << endl; continue; }
            if (i != d.size() - 1) std::cout << ", ";
        }
        std::cout << "." << endl;
    }
}
deque<unique_ptr<relic>>& shuffleRelics(deque<unique_ptr<relic>>& deck, int i)
{
    if (i == 0) return deck;
    shuffleRelics(deck, i - 1);
    int k = random_int(0, i);
    deck[i].swap(deck[k]);
    return deck;
}
deque<unique_ptr<potion>>& shufflePotions(deque<unique_ptr<potion>>& deck, int i)
{
    if (i == 0) return deck;
    shufflePotions(deck, i - 1);
    int k = random_int(0, i);
    deck[i].swap(deck[k]);
    return deck;
}


void playCard(cardHolder& _card)
{
    if (!ai) std::cout << "Playing " << (*_card).getName() << "." << endl;
    if (gamestate::strikeDummyCheck &&
        ((*_card).getName().find("strike") != std::string::npos || (*_card).getName().find("Strike") != std::string::npos))
    {
        apci.getPlayer().observeEvent<increaseADFromPlayerTrigger>(gm.getAlterBaseADFromPlayer(), new increaseADFromPlayerTrigger(gm.getAlterBaseADFromPlayer(), 3));
    }
    (*_card).execute();
    gm.callCardPlayed(_card);
    if ((*_card).getExhaust())
    {
        if (gamestate::strangeSpoonCheck) {
            if (random_int(0, 1)) { gamestate::strangeSpoonLE = true; return; }
            else { gamestate::strangeSpoonLE = false; }
        }
        effects::Exhaust(_card);
    }
}
void playCardDup(cardHolder& _card)
{
    gamestate::dontDupe = true;
    if (!ai) std::cout << "Playing duplicated " << (*_card).getName() << "." << endl;
    if (gamestate::strikeDummyCheck &&
        ((*_card).getName().find("strike") != std::string::npos || (*_card).getName().find("Strike") != std::string::npos))
    {
        apci.getPlayer().observeEvent<increaseADFromPlayerTrigger>(gm.getAlterBaseADFromPlayer(), new increaseADFromPlayerTrigger(gm.getAlterBaseADFromPlayer(), 3));
    }
    if ((*_card).getCombatChoices() == combatChoices::EnemyTarget && gamestate::lastAttackedSCRNIndex == -1) { gamestate::dontDupe = true; return; }
    if ((*_card).getCombatChoices() == combatChoices::EnemyTarget) gamestate::nextCardSameTarget = true;
    (*_card).execute();
    if ((*_card).getCombatChoices() == combatChoices::EnemyTarget) gamestate::nextCardSameTarget = false;
    gm.callCardPlayed(_card);
    gamestate::dontDupe = false;
}
void playTopXCards(int x, bool exh)
{
    gamestate::nextCardsNoTarget = x;
    if (apci.getDrawSize() < x) apci.discardToDraw();
    if (apci.getDrawSize() == 0) return;
    vector<cardHolder> targets;
    int n{ 0 };
    while (n < x && apci.getDrawSize()>0)
    {
        targets.push_back(std::move(apci.getDrawPile()[0]));
        apci.getDrawPile().pop_front();
        ++n;
    }
    for (int i{ 0 }; i < targets.size(); ++i)
    {
        if ((*targets[i]).castable())
        {
            if (gamestate::strikeDummyCheck &&
                ((*targets[i]).getName().find("strike") != std::string::npos || (*targets[i]).getName().find("Strike") != std::string::npos))
            {
                apci.getPlayer().observeEvent<increaseADFromPlayerTrigger>(gm.getAlterBaseADFromPlayer(), new increaseADFromPlayerTrigger(gm.getAlterBaseADFromPlayer(), 3));
            }
            playCard(targets[i]);
            --gamestate::nextCardsNoTarget;
        }
        gamestate::nextCardsNoTarget = 0;
        if (!(*targets[i]).getExhaust() && (*targets[i]).getType() != cardType::Power && exh)
        {
            if (gamestate::strangeSpoonCheck) {
                if (random_int(0, 1)) { gamestate::strangeSpoonLE = true; return; }
                else { gamestate::strangeSpoonLE = false; }
            }
            effects::Exhaust(targets[i]);
        }
    }
}

void initVisualsOne()
{
    initVisualsPlayer();
    // enemy
    visuals::editEnemyNames(gm.getAllEnemies()[0]->getName(),true);
    visuals::editEnemyHealth(gm.getAllEnemies()[0]->getHealth(),true);
    visuals::editEnemyMaxHealth(gm.getAllEnemies()[0]->getMaxHealth(),true);
    visuals::editStrengthEnemy(gm.getAllEnemies()[0]->getTotalStrength(),true);
    visuals::editDexterityEnemy(gm.getAllEnemies()[0]->getTotalDexterity(),true);
    visuals::editVulnerableEnemy(gm.getAllEnemies()[0]->getVulnerable(),true);
    visuals::editWeakEnemy(gm.getAllEnemies()[0]->getWeak(),true);
    visuals::editBlockEnemy(gm.getAllEnemies()[0]->getBlock(),true);
    visuals::addEnemyAltBuffs(gm.getAllEnemies()[0]->getAltBuffs());
    visuals::editAllHand(apci.getHand());
    //intents
    gm.adjustAllIntentsAttacks();
    visuals::addEnemyIntents(gm.getIntents(0), 0, true);
    visuals::update();
}
void initVisualsTwo()
{
    initVisualsPlayer();
    // enemy
    visuals::editEnemyNames(gm.getAllEnemies()[1]->getName(), gm.getAllEnemies()[0]->getName());
    visuals::editEnemyHealth(gm.getAllEnemies()[1]->getHealth(), gm.getAllEnemies()[0]->getHealth());
    visuals::editEnemyMaxHealth(gm.getAllEnemies()[1]->getMaxHealth(), gm.getAllEnemies()[0]->getMaxHealth());
    visuals::editStrengthEnemy(gm.getAllEnemies()[1]->getTotalStrength(), gm.getAllEnemies()[0]->getTotalStrength());
    visuals::editDexterityEnemy(gm.getAllEnemies()[1]->getTotalDexterity(), gm.getAllEnemies()[0]->getTotalDexterity());
    visuals::editVulnerableEnemy(gm.getAllEnemies()[1]->getVulnerable(), gm.getAllEnemies()[0]->getVulnerable());
    visuals::editWeakEnemy(gm.getAllEnemies()[1]->getWeak(), gm.getAllEnemies()[0]->getWeak());
    visuals::editBlockEnemy(gm.getAllEnemies()[1]->getBlock(), gm.getAllEnemies()[0]->getBlock());
    visuals::addEnemyAltBuffs(gm.getAllEnemies()[0]->getAltBuffs(), gm.getAllEnemies()[1]->getAltBuffs());
    visuals::editAllHand(apci.getHand());
    //intents
    gm.adjustAllIntentsAttacks();
    visuals::addEnemyIntents(gm.getAllIntents(), true);
    visuals::update();
}

void initVisualsThree()
{
    initVisualsPlayer();
    // enemy
    visuals::editEnemyNames(gm.getAllEnemies()[2]->getName(), gm.getAllEnemies()[1]->getName(), gm.getAllEnemies()[0]->getName());
    visuals::editEnemyHealth(gm.getAllEnemies()[2]->getHealth(), gm.getAllEnemies()[1]->getHealth(), gm.getAllEnemies()[0]->getHealth());
    visuals::editEnemyMaxHealth(gm.getAllEnemies()[2]->getMaxHealth(), gm.getAllEnemies()[1]->getMaxHealth(), gm.getAllEnemies()[0]->getMaxHealth());
    visuals::editStrengthEnemy(gm.getAllEnemies()[2]->getTotalStrength(), gm.getAllEnemies()[1]->getTotalStrength(), gm.getAllEnemies()[0]->getTotalStrength());
    visuals::editDexterityEnemy(gm.getAllEnemies()[2]->getTotalDexterity(), gm.getAllEnemies()[1]->getTotalDexterity(), gm.getAllEnemies()[0]->getTotalDexterity());
    visuals::editVulnerableEnemy(gm.getAllEnemies()[2]->getVulnerable(), gm.getAllEnemies()[1]->getVulnerable(), gm.getAllEnemies()[0]->getVulnerable());
    visuals::editWeakEnemy(gm.getAllEnemies()[2]->getWeak(), gm.getAllEnemies()[1]->getWeak(), gm.getAllEnemies()[0]->getWeak());
    visuals::editBlockEnemy(gm.getAllEnemies()[2]->getBlock(), gm.getAllEnemies()[1]->getBlock(), gm.getAllEnemies()[0]->getBlock());
    visuals::addEnemyAltBuffs(gm.getAllEnemies()[0]->getAltBuffs(), gm.getAllEnemies()[1]->getAltBuffs(), gm.getAllEnemies()[2]->getAltBuffs());
    visuals::editAllHand(apci.getHand());
    //intents
    gm.adjustAllIntentsAttacks();
    visuals::addEnemyIntents(gm.getAllIntents(), true);
    visuals::update();
}

void initVisualsFour()
{
    initVisualsPlayer();
    // enemy
    visuals::editEnemyNames(gm.getAllEnemies()[3]->getName(), gm.getAllEnemies()[2]->getName(), gm.getAllEnemies()[1]->getName(), gm.getAllEnemies()[0]->getName());
    visuals::editEnemyHealth(gm.getAllEnemies()[3]->getHealth(), gm.getAllEnemies()[2]->getHealth(), gm.getAllEnemies()[1]->getHealth(), gm.getAllEnemies()[0]->getHealth());
    visuals::editEnemyMaxHealth(gm.getAllEnemies()[3]->getMaxHealth(), gm.getAllEnemies()[2]->getMaxHealth(), gm.getAllEnemies()[1]->getMaxHealth(), gm.getAllEnemies()[0]->getMaxHealth());
    visuals::editStrengthEnemy(gm.getAllEnemies()[3]->getTotalStrength(), gm.getAllEnemies()[2]->getTotalStrength(), gm.getAllEnemies()[1]->getTotalStrength(), gm.getAllEnemies()[0]->getTotalStrength());
    visuals::editDexterityEnemy(gm.getAllEnemies()[3]->getTotalDexterity(), gm.getAllEnemies()[2]->getTotalDexterity(), gm.getAllEnemies()[1]->getTotalDexterity(), gm.getAllEnemies()[0]->getTotalDexterity());
    visuals::editVulnerableEnemy(gm.getAllEnemies()[3]->getVulnerable(), gm.getAllEnemies()[2]->getVulnerable(), gm.getAllEnemies()[1]->getVulnerable(), gm.getAllEnemies()[0]->getVulnerable());
    visuals::editWeakEnemy(gm.getAllEnemies()[3]->getWeak(), gm.getAllEnemies()[2]->getWeak(), gm.getAllEnemies()[1]->getWeak(), gm.getAllEnemies()[0]->getWeak());
    visuals::editBlockEnemy(gm.getAllEnemies()[3]->getBlock(), gm.getAllEnemies()[2]->getBlock(), gm.getAllEnemies()[1]->getBlock(), gm.getAllEnemies()[0]->getBlock());
    visuals::addEnemyAltBuffs(gm.getAllEnemies()[0]->getAltBuffs(), gm.getAllEnemies()[1]->getAltBuffs(), gm.getAllEnemies()[2]->getAltBuffs(), gm.getAllEnemies()[3]->getAltBuffs());
    visuals::editAllHand(apci.getHand());
    //intents
    gm.adjustAllIntentsAttacks();
    visuals::addEnemyIntents(gm.getAllIntents(), true);
    visuals::update();
}
void initVisualsFive()
{
    initVisualsPlayer();
    // enemy
    visuals::editEnemyNames(gm.getAllEnemies()[4]->getName(), gm.getAllEnemies()[3]->getName(), gm.getAllEnemies()[2]->getName(), gm.getAllEnemies()[1]->getName(), gm.getAllEnemies()[0]->getName());
    visuals::editEnemyHealth(gm.getAllEnemies()[4]->getHealth(), gm.getAllEnemies()[3]->getHealth(), gm.getAllEnemies()[2]->getHealth(), gm.getAllEnemies()[1]->getHealth(), gm.getAllEnemies()[0]->getHealth());
    visuals::editEnemyMaxHealth(gm.getAllEnemies()[4]->getMaxHealth(), gm.getAllEnemies()[3]->getMaxHealth(), gm.getAllEnemies()[2]->getMaxHealth(), gm.getAllEnemies()[1]->getMaxHealth(), gm.getAllEnemies()[0]->getMaxHealth());
    visuals::editStrengthEnemy(gm.getAllEnemies()[4]->getTotalStrength(), gm.getAllEnemies()[3]->getTotalStrength(), gm.getAllEnemies()[2]->getTotalStrength(), gm.getAllEnemies()[1]->getTotalStrength(), gm.getAllEnemies()[0]->getTotalStrength());
    visuals::editDexterityEnemy(gm.getAllEnemies()[4]->getTotalDexterity(), gm.getAllEnemies()[3]->getTotalDexterity(), gm.getAllEnemies()[2]->getTotalDexterity(), gm.getAllEnemies()[1]->getTotalDexterity(), gm.getAllEnemies()[0]->getTotalDexterity());
    visuals::editVulnerableEnemy(gm.getAllEnemies()[4]->getVulnerable(), gm.getAllEnemies()[3]->getVulnerable(), gm.getAllEnemies()[2]->getVulnerable(), gm.getAllEnemies()[1]->getVulnerable(), gm.getAllEnemies()[0]->getVulnerable());
    visuals::editWeakEnemy(gm.getAllEnemies()[4]->getWeak(), gm.getAllEnemies()[3]->getWeak(), gm.getAllEnemies()[2]->getWeak(), gm.getAllEnemies()[1]->getWeak(), gm.getAllEnemies()[0]->getWeak());
    visuals::editBlockEnemy(gm.getAllEnemies()[4]->getBlock(), gm.getAllEnemies()[3]->getBlock(), gm.getAllEnemies()[2]->getBlock(), gm.getAllEnemies()[1]->getBlock(), gm.getAllEnemies()[0]->getBlock());
    visuals::addEnemyAltBuffs(gm.getAllEnemies()[0]->getAltBuffs(), gm.getAllEnemies()[1]->getAltBuffs(), gm.getAllEnemies()[2]->getAltBuffs(), gm.getAllEnemies()[3]->getAltBuffs(), gm.getAllEnemies()[4]->getAltBuffs());
    visuals::editAllHand(apci.getHand());
    //intents
    gm.adjustAllIntentsAttacks();
    visuals::addEnemyIntents(gm.getAllIntents(), true);
    visuals::update();
}
void initVisualsPlayer()
{
    visuals::editEnergy(gm.getPlayerEnergy());
    visuals::editRelics(apci.getRelics());
    visuals::editPotions(apci.getPotions(), apci.getPotionSlots());
    // player
    visuals::editPlayerHealth(apci.getPlayer().getHealth());
    visuals::editPlayerMaxHealth(apci.getPlayer().getMaxHealth());
    visuals::editDiscardPile(apci.getDiscardSize());
    visuals::editDrawPile(apci.getDrawSize());
    visuals::editExhausted(static_cast<int>(apci.getExhausted().size()));
    visuals::editStrengthPC(apci.getPlayer().getTotalStrength());
    visuals::editDexterityPC(apci.getPlayer().getTotalDexterity());
    visuals::editVulnerablePC(apci.getPlayer().getVulnerable());
    visuals::editFrailPC(apci.getPlayer().getFrail());
    visuals::editWeakPC(apci.getPlayer().getWeak());
    visuals::editBlockPC(apci.getPlayer().getBlock());
    visuals::addPCAltBuffs(apci.getPlayer().getAltBuffs());
}

void swap(cardHolder& a, cardHolder& b)
{
    auto temp = a.card_;
    a.card_ = b.card_;
    b.card_ = temp;
}
cardHolder::cardHolder(card& c) { card_ = c.safeClone(); id = unique_id++; }
cardHolder::~cardHolder() { delete card_; }
cardHolder::cardHolder(const cardHolder& ch)
{
    card_ = ch.card_->safeClone();
    id = ch.id;
}
cardHolder::cardHolder(cardHolder&& ch) noexcept
{
    card_ = ch.card_;
    ch.card_ = nullptr;
    id = ch.id;
}
cardHolder& cardHolder::operator=(const cardHolder& ch)
{
    if (&ch == this) return *this;
    if (card_ != nullptr) delete card_;
    card_ = ch.card_->safeClone();
    id = ch.id;
    return *this;
}
cardHolder& cardHolder::operator=(cardHolder&& ch) noexcept
{
    if (card_ != nullptr) delete card_;
    card_ = ch.card_;
    ch.card_ = nullptr;
    id = ch.id;
    return *this;
}

nonpc& askForEnemy(const int num_targets)
{
    vector<string> inputs_str;
    for (int i{ 1 }; i < gm.getAllEnemies().size() + 1; ++i)
    {
        if (!ai) std::cout << i << " for " << gm.getAllEnemies()[i - 1]->getName() << "(" << gm.getAllEnemies()[i - 1]->getHealth()
            << "/" << gm.getAllEnemies()[i - 1]->getMaxHealth() << ").";
        inputs_str.push_back(to_string(i));
    }
    std::string result{ request_input(inputs_str) };
    for (int i{ 0 }; i < gm.getAllEnemies().size(); ++i)
    {
        if (inputs_str[i] == result)
        {
            gamestate::lastAttackedSCRNIndex = i;
            return *(gm.getAllEnemies()[i]);
        }
    }
    throw runtime_error("error");
    num_targets;
}
nonpc& singleTarget()
{
    assert(gm.getAllEnemies().size() > 0);
    int temp = static_cast<int>(gm.getAllEnemies().size());
    if (gamestate::nextCardsNoTarget > 0) return randomTarget();
    if (gamestate::nextCardSameTarget) return *(gm.getAllEnemies()[gamestate::lastAttackedSCRNIndex]);
    if (temp > 1)
    {
        if (gamestate::nextCardsNoTarget) return *(gm.getAllEnemies()[gamestate::lastAttackedSCRNIndex]);
        return askForEnemy(1);
    }
    else if (temp == 1)
    {
        gamestate::lastAttackedSCRNIndex = 0;
        return *(gm.getAllEnemies()[0]);
    }
    throw runtime_error("single target");
}
nonpc& randomTarget()
{
    assert(gm.getAllEnemies().size() > 0);
    int rng = random_int(0, gm.getAllEnemies().size() - 1);
    gamestate::lastAttackedSCRNIndex = rng;
    return *(gm.getAllEnemies()[rng]);
}
cardHolder& askForCard(const int num_targets, vector<cardType> cts)
{
    if (cts.empty())
    {
        vector<string> inputs_str;
        for (int i{ 1 }; i < apci.getHand().size() + 1; ++i)
        {
            if (!ai) std::cout << i << " for " << (*(apci.getHand()[i - 1])).getName() << ". ";
            inputs_str.push_back(to_string(i));
        }
        std::string result{ request_input(inputs_str) };
        for (int i{ 0 }; i < apci.getHand().size(); ++i)
        {
            if (inputs_str[i] == result)
            {
                return apci.getHand()[i];
            }
        }
        throw runtime_error("error");
    }
    else
    {
        vector<string> inputs_str;
        for (int i{ 1 }; i < apci.getHand().size() + 1; ++i)
        {
            if (find(cts.begin(), cts.end(), (*apci.getHand()[i - 1]).getType()) != cts.end())
            {
                if (!ai) std::cout << i << " for " << (*(apci.getHand()[i - 1])).getName() << ". ";
                inputs_str.push_back(to_string(i));
            }
        }
        std::string result{ request_input(inputs_str) };
        return apci.getHand()[stoi(result) - 1];
    }
    num_targets;
}
cardHolder& askForCard(const int num_targets, cardDeckCH& d)
{
    vector<string> inputs_str;
    for (int i{ 1 }; i < d.size() + 1; ++i)
    {
        if (!ai) std::cout << i << " for " << (*(d[i - 1])).getName() << ". ";
        inputs_str.push_back(to_string(i));
    }
    std::string result{ request_input(inputs_str) };
    for (int i{ 0 }; i < d.size(); ++i)
    {
        if (inputs_str[i] == result)
        {
            return d[i];
        }
    }
    throw runtime_error("error");
    num_targets;
}
cardHolder& singleTargetCard(vector<cardType> cts)
{
    if (cts.empty())
    {
        int temp = static_cast<int>(apci.getHand().size());
        if (temp > 1) return askForCard(1);
        else if (temp == 1) return apci.getHand()[0];
        throw runtime_error("no cards");
    }
    else
    {
        int temp = apci.numberInHand(cts);
        if (temp > 1) return askForCard(1, cts);
        else if (temp == 1)
        {
            for (int i{ 0 }; i < apci.getHand().size(); ++i)
            {
                if (find(cts.begin(), cts.end(), (*apci.getHand()[i]).getType()) != cts.end()) return apci.getHand()[i];
            }
        }
        throw runtime_error("no cards");
    }
}
cardHolder& randomTargetCard()
{
    int temp = static_cast<int>(apci.getHand().size());
    if (temp > 1) return apci.getHand()[random_int(0, temp - 1)];
    else if (temp == 1) return apci.getHand()[0];
    throw runtime_error("no enemies");
}
cardHolder& singleTargetCard(cardDeckCH& d)
{
    int temp = static_cast<int>(d.size());
    if (temp > 1) return askForCard(1, d);
    else if (temp == 1) return d[0];
    throw runtime_error("no cards");
}
set<int> getIndicesForCardBoolFlip(const int& lowerIndex, const int& upperIndex, int maxChoose)
{
    static map<pair<int, int>, int> cache;
    if (ai)
    {
        int choice;
        int s = static_cast<int>(apci.getHand().size());
        if (s == 0) return {};
        if (maxChoose > s) maxChoose = s;
        if (gamestate::randomAIChoice)
        {
            if (cache.find({ maxChoose,s }) != cache.end()) choice = random_int(0, cache[{maxChoose, s}]);
            else
            {
                int perms{ 0 };
                for (int j{ 0 }; j < maxChoose; ++j)
                {
                    perms += NchooseR(s, j);
                }
                cache[{maxChoose, s}] = perms - 1;
                choice = random_int(0, perms - 1);
            }
        }
        else
        {
            choice = gamestate::aiSimActionBuffer[0];
            gamestate::aiSimActionBuffer.pop_front();
        }
        int sum{ 0 }, num{ 0 };
        for (int i{ 0 }; i < maxChoose + 1; ++i)
        {
            sum += NchooseR(s, i);
            if (choice < sum)
            {
                sum -= NchooseR(s, i);
                num = i;
                break;
            }
        }

        auto vec = baseIter(s, num, choice - sum);
        set<int> res;
        while (!vec.empty())
        {
            res.insert(vec.back());
            vec.pop_back();
        }
        gamestate::randomAIChoice = true;
        return res;
    }
    else
    {
        std::string input{ "repeat" };
        set<int> res;
        if (apci.getHand().size() == 0) return res;
        if (maxChoose == 0) maxChoose = static_cast<int>(apci.getHand().size());
        while (true)
        {
            try
            {
                res.clear();
                std::getline(std::cin, input);
                stringstream temp(input);
                int c, x{ maxChoose };
                while (temp >> c && --x >= 0)
                {
                    if (c <= upperIndex && c >= lowerIndex)
                    {
                        res.insert(c);
                    }
                    else { continue; }
                }
                break;
            }
            catch (...) { continue; }
        }
        return res;
    }
}
bool checkInHand(cardHolder& ch)
{
    cardDeckCH* temp = &apci.getHand();
    for (int i{ 0 }; i < temp->size(); ++i)
    {
        if (&ch == &(*temp)[i])
        {
            return true;
        }
    }
    return false;
}

//vector<vector<stateInfo>> startFutureSimsAdd(cardDeck& cd, vector<int> cardOptIds, vector<int> cardOptEnergy, vector<int> cardOptUpgrade)
//{
//    msv.saveGame();
//    auto tempFE = gm.getCardAdded();
//    for (auto obs : tempFE->getObservers())
//    {
//        obs->setInactive();
//    }
//    apci.addCardToDeck(gm.getSpecialCardByID("RitualDagger_ID"));
//    vector<int> counts_(263), ids;
//    for (int i{ 0 }; i < cd.size(); ++i)
//    {
//        ids.push_back(cd[i]->getCardIDNum());
//        ++counts_[(cd[i]->getCardIDNum() - 1) * 2 + cd[i]->getUpgraded()];
//    }
//    // needs to be given chosen fight type, and iterate through all
//    gm.startRandomFight(gm.getEvent());
//    // ^^^ placeholder ^^^^
//    stateInfo baseSI;
//    baseSI.createAllVars();
//    vector<vector<stateInfo>> SINFOs(cardOptIds.size());
//    tempFE = gm.getCardDrawn();
//    for (auto obs : tempFE->getObservers())
//    {
//        obs->setInactive();
//    }
//    for (int n{ 0 }; n < 10; ++n)
//    {
//        if (baseSI.drawCountPerCard[186] > 0)
//        {
//            --baseSI.drawCountPerCard[186];
//            for (int i{ 0 }; i < cardOptIds.size(); ++i)
//            {
//                ++baseSI.drawCountPerCard[cardOptIds[i]];
//                stateInfo temp;
//                temp.createAllVars(false);
//                SINFOs[i].push_back(temp);
//                --baseSI.drawCountPerCard[cardOptIds[i]];
//            }
//        }
//        else
//        {
//            for (int i{ 0 }; i < baseSI.handCardIDs.size(); ++i)
//            {
//                if (baseSI.handCardIDs[i] == 93)
//                {
//                    for (int j{ 0 }; j < cardOptIds.size(); ++j)
//                    {
//                        baseSI.handCardIDs[i] = cardOptIds[j];
//                        baseSI.handCardEnergy[i] = cardOptEnergy[j];
//                        baseSI.handCardUpgraded[i] = cardOptUpgrade[j];
//                        stateInfo temp;
//                        temp.createAllVars(false);
//                        SINFOs[i].push_back(temp);
//                    }
//                    break;
//                }
//            }
//        }
//        apci.reshuffleDrawAndHand();
//    }
//    // some sort of clear fight thing and then
//    msv.loadGame();
//    /*tempFE = gm.getCardAdded();
//    for (auto obs : tempFE->getObservers())
//    {
//        obs->setActive();
//    }
//    tempFE = gm.getCardDrawn();
//    for (auto obs : tempFE->getObservers())
//    {
//        obs->setActive();
//    }*/
//    return SINFOs;
//}
//vector<vector<stateInfo>> startFutureSimsUpgrade(cardDeck& cd)
//{
//    msv.saveGame();
//    set<int> nonUpgradedIDs;
//    for (int i{ 0 }; i < cd.size(); ++i)
//    {
//        if (!cd[i]->getUpgraded() && cd[i]->getType() != Curse)
//        {
//            nonUpgradedIDs.insert(cd[i]->getCardIDNum());
//        }
//    }
//    // go through fights with each unique card upgraded
//    vector<vector<stateInfo>> SINFOs(nonUpgradedIDs.size());
//    auto SINFOitr = SINFOs.begin();
//    for (auto itr{ nonUpgradedIDs.begin() }; itr != nonUpgradedIDs.end(); ++itr)
//    {
//        msv.loadGame();
//        msv.saveGame();
//        for (int j{ 0 }; j < cd.size(); ++j)
//        {
//            if (cd[j]->getCardIDNum() == *itr && !cd[j]->getUpgraded())
//            {
//                cd[j]->upgradeThis();
//
//                // needs to be given chosen fight type, and iterate through all
//                gm.startRandomFight(gm.getEvent());
//                // ^^^ placeholder ^^^^
//
//                auto tempFE = gm.getCardDrawn();
//                for (auto obs : tempFE->getObservers())
//                {
//                    obs->setInactive();
//                }
//                for (int n{ 0 }; n < 10; ++n)
//                {
//                    stateInfo baseSI;
//                    baseSI.createAllVars(false);
//                    SINFOitr->push_back(baseSI);
//
//                    apci.reshuffleDrawAndHand();
//                }
//                break;
//            }
//        }
//        // clearup fight
//        //auto tempFE = gm.getCardDrawn();
//        /*for (auto obs : tempFE->getObservers())
//        {
//            obs->setActive();
//        }*/
//        ++SINFOitr;
//    }
//
//    msv.loadGame();
//
//    return SINFOs;
//}


void forcedAIActions(int& _action, float& _reward)
{
    if ((_action == 142 || _action == 143) && apci.getPlayer().getTotalStrength() <= 0) _reward -= 1;
    if (_action == 262 && apci.couldHavePlayedAnother() && apci.fineToBlockMore(_action)) _reward -= 0.25;
    if (_action == 262 && apci.couldHavePowered()) _reward -= 1;
    if (!apci.getPotions().empty())
    {
        for (int i{ 0 }; i < apci.getPotions().size(); ++i)
        {
            int _pid = apci.getPotion(i)->getPotionIDNum();
            if (_pid == 7 || _pid == 8 || _pid == 14 || _pid == 20 || _pid == 22 || _pid == 23 || _pid == 31)
            {
                _action = 262 + _pid;
                break;
            }
        }
    }
}

shopStuff::shopStuff()
{
    c1 = gm.getDatabaseEnd();
    c2 = gm.getDatabaseEnd();

    cc1 = gm.getColourlessDatabaseEnd();
    cc2 = gm.getColourlessDatabaseEnd();
}
bool shopStuff::isSingle() const
{
    int res{ 0 };
    res += r1 != nullptr;
    res += r2 != nullptr;
    res += p1 != nullptr;
    res += p2 != nullptr;
    res += c1 != gm.getDatabaseEnd();
    res += c2 != gm.getDatabaseEnd();
    res += cc1 != gm.getColourlessDatabaseEnd();
    res += cc2 != gm.getColourlessDatabaseEnd();
    res += rm;
    return res == 1;
}

void rngRelicAddID(const vector<int>& taken_, vector<int>& free_, int length_)
{
    int rng{ 0 };
    while (free_.size()<length_)
    {
        rng = random_int(1, 98);
        if (find(taken_.begin(), taken_.end(), rng) != taken_.end()) continue;
        if (find(free_.begin(), free_.end(), rng) != free_.end()) continue;
        free_.push_back(rng);
    }
}

void rngCardAddID(vector<int>& free_, int length_)
{
    // 4 is first non basic, 74 is last
    int rng{ 0 };
    while (free_.size() < length_)
    {
        rng = random_int(4, 74);
        if (random_int(0, 71) == 0) rng = 130; // cleave
        free_.push_back(rng);
    }
}
