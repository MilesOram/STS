#ifndef EFF_TRIG_H
#define EFF_TRIG_H
#include "top.h"
#include "card.h"
#include "character.h"
#include "pc_info.h"
#include "gamestate.h"
#include "enemies.h"


namespace effects
{
    void Damage(character& target, const int& amount);
    bool enemyAdjustedDamage(character& enemy_, const int& amount_);
    bool playerAdjustedDamage(character& target, const int& amount_);
    bool singleTargetMulti(character& self, character& target, const int& amount, const int& times);
    bool randomDamage(const int& amount);
    bool randomAttackDamage(const int& amount);
    bool multiRandomDamage(const int& amount, const int& times);
    bool multiRandomAttackDamage(const int& amount, const int& times);
    bool AOEAttackDamge(const int& amount_);
    bool multiAOEAttackDamage(const int& amount, const int& times);
    bool AOEDamage(const int& amount);
    void Block(character& target, const int& amount);
    void AdjustedBlock(const int& amount_);
    void Vulnerable(character& target, const int& amount);
    void Weak(character& target, const int& amount);
    void Frail(character& target, const int& amount);
    void Entangle(character& target, const int& amount);
    void Strength(character& target, const int& amount);
    void Dexterity(character& target, const int& amount);
    void upgradeCard(cardHolder& target);
    void upgradeRandomCard();
    //void upgradeCards(cardDeck&);
    void upgradeCards(cardDeckCH& deck);
    void Exhaust(cardHolder target, bool duped = false);
    void exhaustHandCard(cardHolder target);
    void exhaustHand();
    void exhaustDrawPileCard(cardHolder target);
}

// templated derived observer class, used for observers which need to act on something specific, CH: holdsObs_ = base class character, CA: holdsObs_ = base class card
template<typename D>
class observerCH : public observer
{
protected:
    D* holdsObs_;
    static void Constraints(D* p) { character* pb = p; pb; }
public:
    observerCH(D* d, focusedEvent* fe) : observer(fe), holdsObs_(d) { void(*p)(D*) = Constraints; p; }
    observerCH(D* d, focusedEvent* fe, std::string n) : observer(fe, n), holdsObs_(d) { void(*p)(D*) = Constraints; p; }
    observerCH(D* d, focusedEvent* fe, int trigPrio) : observer(fe, "", trigPrio), holdsObs_(d) { void(*p)(D*) = Constraints; p; }
    observerCH(D* d, focusedEvent* fe, std::string n, int trigPrio) : observer(fe, n, trigPrio), holdsObs_(d) { void(*p)(D*) = Constraints; p; }
    virtual ~observerCH() {}
};
template<typename D>
class observerCA : public observer
{
protected:
    D* holdsObs_;
    static void Constraints(D* p) { card* pb = p; pb; }
public:
    observerCA(D* d, focusedEvent* fe) : observer(fe), holdsObs_(d) { void(*p)(D*) = Constraints; p; }
    observerCA(D* d, focusedEvent* fe, std::string n) : observer(fe, n), holdsObs_(d) { void(*p)(D*) = Constraints; p; }
    observerCA(D* d, focusedEvent* fe, int trigPrio) : observer(fe, "", trigPrio), holdsObs_(d) { void(*p)(D*) = Constraints; p; }
    observerCA(D* d, focusedEvent* fe, std::string n, int trigPrio) : observer(fe, n, trigPrio), holdsObs_(d) { void(*p)(D*) = Constraints; p; }
    virtual ~observerCA() {}
    void changeHoldsObs(D* d) { holdsObs_ = d; }
};

// trigger to clear up other triggers, certain observers have a lifetime which ends on various triggers
template<typename D>
class haltObservingTriggerCH : public observerCH<D>
{
private:
    observer* obs_;
public:
    haltObservingTriggerCH(D* d, observer* obs, int trigP = 150);
    haltObservingTriggerCH(D* d, focusedEvent* fe, observer* obs, int trigP = 150) : obs_(obs), observerCH<D>(d, fe, "haltObserving", trigP) {}
    void triggered()
    {
        if (obs_)
        {
            this->holdsObs_->removeObserver(obs_);
            obs_ = nullptr;
            this->holdsObs_->safeRemoveObserver(static_cast<observer*>(this));
            delete this;
        }
        else { throw runtime_error("halt obs ch"); }
    }
    observer* getObs() const { return obs_; }
    void setObs(observer* o) { obs_ = o; }
    haltObservingTriggerCH<D>* clone(character* hO) { auto temp = new haltObservingTriggerCH<character>(hO, this->obs_, this->triggerPrio);  this->observing_->addObserver(temp); return temp; }
};


template<typename T>
T* saveCloneFromOrig(T* original, T* clone)
{
    // priv pc
    clone->name_ = original->name_;
    clone->pastAttacks = original->pastAttacks;
    clone->allAttacks = original->allAttacks;
    clone->enemyIDNumber = original->enemyIDNumber;
    clone->enrage_ = original->enrage_;
    // char
    clone->scrnIndex = original->scrnIndex;
    clone->health_ = original->health_;
    clone->max_health_ = original->max_health_;
    clone->block_ = original->block_;
    // debuffs
    clone->weak_ = original->weak_;
    clone->frail_ = original->frail_;
    clone->vulnerable_ = original->vulnerable_;
    clone->m_strength_ = original->m_strength_;
    clone->m_dexterity_ = original->m_dexterity_;
    clone->entangle_ = original->entangle_;
    clone->no_draw_ = original->no_draw_;
    clone->no_block_ = original->no_block_;
    // buffs
    clone->strength_ = original->strength_;
    clone->dexterity_ = original->dexterity_;
    clone->metallicise_ = original->metallicise_;
    clone->plated_armour_ = original->plated_armour_;
    clone->buffer_ = original->buffer_;
    clone->ragee_ = original->ragee_;
    clone->intangible_ = original->intangible_;
    clone->regeneration_ = original->regeneration_;
    clone->malleable_ = original->malleable_;
    clone->base_malleable = original->base_malleable;
    // multis
    clone->weakMulti = original->weakMulti;
    clone->frailMulti = original->frailMulti;
    clone->vulnerableMulti = original->vulnerableMulti;
    // power
    clone->BarricadeBuff = original->BarricadeBuff;
    clone->BerserkBuff = original->BerserkBuff;
    clone->BrutalityBuff = original->BrutalityBuff;
    clone->CombustBuff = original->CombustBuff;
    clone->CorruptionBuff = original->CorruptionBuff;
    clone->DarkEmbraceBuff = original->DarkEmbraceBuff;
    clone->DemonFormBuff = original->DemonFormBuff;
    clone->EvolveBuff = original->EvolveBuff;
    clone->FeelNoPainBuff = original->FeelNoPainBuff;
    clone->FireBreathingBuff = original->FireBreathingBuff;
    clone->JuggernautBuff = original->JuggernautBuff;
    clone->RuptureBuff = original->RuptureBuff;
    // alterations
    clone->toDrawSOT = original->toDrawSOT;
    clone->bottledCount = original->bottledCount;
    clone->eotStrength = original->eotStrength;
    clone->eotDexterity = original->eotDexterity;
    clone->thorns_ = original->thorns_;
    clone->flameBar_ = original->flameBar_;
    clone->sharp_hide_ = original->sharp_hide_;
    clone->ritual_ = original->ritual_;
    clone->artifact_ = original->artifact_;
    clone->altBuffs = original->altBuffs;

    clone->dormant_turns_ = original->dormant_turns_;
    clone->painful_stabs_ = original->painful_stabs_;
    clone->slowed_ = original->slowed_;
    clone->dormant_ = original->dormant_;
    clone->clock12_ = original->clock12_;
    clone->curiosity_ = original->curiosity_;
    clone->invincible_ = original->invincible_;
    clone->base_invincible_ = original->base_invincible_;
    clone->beat_of_death_ = original->beat_of_death_;
    clone->back_attack_ = original->back_attack_;

    clone->sotBlockObs = original->sotBlockObs ? original->sotBlockObs->clone(clone) : nullptr;
    if (clone->sotBlockObs) clone->sotBlockObs->setInactive();
    // holdsobs
    std::vector<std::pair<observer*, observer*>> lookFor;
    for (auto itr{ original->observers_.begin() }; itr != original->observers_.end(); ++itr)
    {
        haltObservingTriggerCH<character>* halt = dynamic_cast<haltObservingTriggerCH<character>*>(*itr);
        if (halt != nullptr)
        {
            lookFor.push_back({ halt->getObs(),*itr });
        }
    }
    for (auto itr{ original->observers_.begin() }; itr != original->observers_.end(); ++itr)
    {
        auto temp = (*itr)->clone(clone);
        temp->setInactive();
        clone->observers_.insert(temp);
        for (int i{ 0 }; i < lookFor.size(); ++i)
        {
            if (lookFor[i].first == *itr) lookFor[i].first = temp;
            else if (lookFor[i].second == *itr) lookFor[i].second = temp;
        }
    }
    for (auto itr{ clone->observers_.begin() }; itr != clone->observers_.end(); ++itr)
    {
        for (int i{ 0 }; i < lookFor.size(); ++i)
        {
            if (lookFor[i].second == *itr)
            {
                haltObservingTriggerCH<character>* halt = dynamic_cast<haltObservingTriggerCH<character>*>(*itr);
                if (halt != nullptr)
                {
                    halt->setObs(lookFor[i].first);
                }
            }
        }
    }
    return clone;
}

template<typename D>
class haltObservingTriggerCA : public observerCA<D>
{
private:
    observer* obs_;
public:
    haltObservingTriggerCA(D* d, observer* obs, int trigP = 150);
    haltObservingTriggerCA(D* d, focusedEvent* fe, observer* obs, int trigP = 150) : obs_(obs), observerCA<D>(d, fe, "haltObserving", trigP) {}
    void triggered()
    {
        if (obs_)
        {
            this->holdsObs_->removeObserver(obs_);
            obs_ = nullptr;
            this->holdsObs_->safeRemoveObserver(static_cast<observer*>(this));
            delete this;
        }
        else { throw runtime_error("halt obs ca"); }
    }
    haltObservingTriggerCA<D>* clone(card* hO) { auto temp = new haltObservingTriggerCA<card>(hO, this->obs_, this->triggerPrio);  this->observing_->addObserver(temp); return temp;}
};
class removeBuffVisTrigger : public observer
{
private:
    Intents buff;
public:
    removeBuffVisTrigger(Intents i);
    removeBuffVisTrigger(focusedEvent* fe, Intents i) : buff(i), observer(fe, "removeBuff") {}
    void triggered();
    removeBuffVisTrigger* clone() { auto temp = new removeBuffVisTrigger(observing_, buff);  this->observing_->addObserver(temp); return temp;}
};
class gainEnergyTrigger : public observer
{
private:
    int nrg{ 1 };
public:
    gainEnergyTrigger();
    gainEnergyTrigger(focusedEvent* fe, int i = 1) : nrg(i), observer(fe, "gainEnergy") {}
    void triggered();
    gainEnergyTrigger* clone() { auto temp = new gainEnergyTrigger(observing_, nrg);  this->observing_->addObserver(temp); return temp;}
};
class intangibleTriggerPC : public observer
{
private:
public:
    intangibleTriggerPC();
    void triggered();
    intangibleTriggerPC* clone() { auto temp = new intangibleTriggerPC(); this->observing_->addObserver(temp); return temp; }
};
class intangibleTriggerNonPC : public observer
{
private:
public:
    intangibleTriggerNonPC();
    void triggered();
    intangibleTriggerNonPC* clone() { auto temp = new intangibleTriggerNonPC(); this->observing_->addObserver(temp); return temp;}
};
class gainBlockTrigger : public observer
{
private:
    int blockNumber{ 0 };
    bool delAfter{ false };
public:
    gainBlockTrigger(focusedEvent* fe, int i, bool dA = false) : delAfter(dA), blockNumber(i), observer(fe, "gainBlock") {}
    void triggered();
    gainBlockTrigger* clone() { auto temp = new gainBlockTrigger(observing_, blockNumber, delAfter); this->observing_->addObserver(temp); return temp;}
};
class loseLifeTrigger : public observer
{
private:
    int lifeLoss;
public:
    loseLifeTrigger(int i = 1);
    loseLifeTrigger(focusedEvent* fe, int i = 1) : lifeLoss(i), observer(fe, "loseLife") {}
    void triggered();
    loseLifeTrigger* clone() { auto temp = new loseLifeTrigger(observing_, lifeLoss); this->observing_->addObserver(temp); return temp;}
};
class healHPTrigger : public observer
{
private:
    int lifeGain;
public:
    healHPTrigger(int i = 1);
    healHPTrigger(focusedEvent* fe, int i = 1) : lifeGain(i), observer(fe, "lifeGain") {}
    void triggered();
    healHPTrigger* clone() { auto temp = new healHPTrigger(observing_, lifeGain); this->observing_->addObserver(temp); return temp;}
};
class hexTrigger : public observer
{
private:
public:
    hexTrigger(focusedEvent* fe) : observer(fe, "hex") {}
    void triggered();
    void triggered(cardHolder& ch);
    hexTrigger* clone() { auto temp = new hexTrigger(observing_); this->observing_->addObserver(temp); return temp; }
};
class goldTrigger : public observer
{
private:
    int goldNum;
public:
    goldTrigger(focusedEvent* fe, int gold) : goldNum(gold), observer(fe, "gold") {}
    void triggered();
    void triggered(character&);
    goldTrigger* clone() { auto temp = new goldTrigger(observing_,goldNum); this->observing_->addObserver(temp); return temp; }
};
class mawBankSwitchTrigger : public observer
{
private:
public:
    mawBankSwitchTrigger(focusedEvent* fe) : observer(fe, "mawBankSwitch") {}
    void triggered();
    mawBankSwitchTrigger* clone() { auto temp = new mawBankSwitchTrigger(observing_); this->observing_->addObserver(temp); return temp; }
};
class combustTrigger : public observer
{
private:
    int lhp{ 0 };
    int dmg{ 0 };
public:
    combustTrigger(focusedEvent* fe, int l = 0, int d = 0) : lhp(l), dmg(d), observer(fe, "combust") {}
    void triggered();
    combustTrigger* clone() { auto temp = new combustTrigger(this->observing_, lhp, dmg); this->observing_->addObserver(temp); return temp; }
};
class constrictedTrigger : public observer
{
private:
    int dmg{ 0 };
public:
    constrictedTrigger(focusedEvent* fe, int d = 0) : dmg(d), observer(fe, "constricted") {}
    void triggered();
    constrictedTrigger* clone() { auto temp = new constrictedTrigger(this->observing_, dmg); this->observing_->addObserver(temp); return temp; }
};
class feelNoPainTrigger : public observer
{
private:
    int n_{ 0 };
public:
    feelNoPainTrigger(focusedEvent* fe, int n = 0) : n_(n), observer(fe, "feelNoPain") {}
    void triggered();
    feelNoPainTrigger* clone() { auto temp = new feelNoPainTrigger(this->observing_, n_); this->observing_->addObserver(temp); return temp; }
};
class magnetismTrigger : public observer
{
private:
    int n_{ 0 };
public:
    magnetismTrigger(focusedEvent* fe, int n=0) : n_(n), observer(fe, "magnetism") { triggerPrio = 105; }
    void triggered();
    magnetismTrigger* clone() { auto temp = new magnetismTrigger(observing_,n_); this->observing_->addObserver(temp); return temp; }
};
class mayhemTrigger : public observer
{
private:
    int n_{ 0 };
public:
    mayhemTrigger(focusedEvent* fe, int n=0) : n_(n), observer(fe, "mayhem") { triggerPrio = 1009; }
    void triggered();
    mayhemTrigger* clone() { auto temp = new mayhemTrigger(observing_,n_); this->observing_->addObserver(temp); return temp; }
};
class panacheTrigger : public observer
{
private:
    int n_{ 0 };
public:
    panacheTrigger(focusedEvent* fe, int n=0) : n_(n), observer(fe, "panache") { }
    void triggered();
    panacheTrigger* clone() { auto temp = new panacheTrigger(observing_, n_); this->observing_->addObserver(temp); return temp; }
};
class sadisticTrigger : public observer
{
private:
    int n_{ 0 };
public:
    sadisticTrigger(focusedEvent* fe, int n=0) : n_(n), observer(fe, "sadistic") { }
    void triggered(character& enemy);
    sadisticTrigger* clone() { auto temp = new sadisticTrigger(observing_, n_); this->observing_->addObserver(temp); return temp; }
};
class bombTrigger : public observer
{
private:
    int dmg{ 0 };
    int turns{ 0 };
public:
    bombTrigger(focusedEvent* fe, int d = 0, int t = 0) : dmg(d), turns(t), observer(fe, "bomb") { }
    void triggered();
    bombTrigger* clone() { auto temp = new bombTrigger(observing_, dmg, turns); this->observing_->addObserver(temp); return temp; }
};
class increaseADFromPlayerTrigger : public observer
{
private:
    int dmg{ 0 };
public:
    increaseADFromPlayerTrigger(focusedEvent* fe, int _dmg) : dmg(_dmg), observer(fe, "increaseADFromPlayer") {}
    void triggered();
    increaseADFromPlayerTrigger* clone() { auto temp = new increaseADFromPlayerTrigger(observing_,dmg); this->observing_->addObserver(temp); return temp; }
};
class painStabTrigger : public observer
{
private:
public:
    painStabTrigger(focusedEvent* fe) : observer(fe, "painStab") {}
    void triggered();
    painStabTrigger* clone() { auto temp = new painStabTrigger(observing_); this->observing_->addObserver(temp); return temp; }
};
class ritualDaggerFatalPermTrigger : public observer
{
private:
    int dmgForSearch{ 15 };
public:
    ritualDaggerFatalPermTrigger(focusedEvent* fe, int dN) : dmgForSearch(dN), observer(fe, "ritualDaggerFatalPerm",98) {}
    void triggered(character&);
    ritualDaggerFatalPermTrigger* clone() { auto temp = new ritualDaggerFatalPermTrigger(observing_,dmgForSearch); this->observing_->addObserver(temp); return temp; }
};
template<typename D>
class ritualDaggerFatalTrigger : public observerCA<D>
{
private:
public:
    ritualDaggerFatalTrigger(D* d, focusedEvent* fe) : observerCA<D>(d, fe, "ritualDaggerFatal",98) {}
    void triggered(character&);
    ritualDaggerFatalTrigger<D>* clone(card* hO) { auto temp = new ritualDaggerFatalTrigger<card>(hO, this->observing_); this->observing_->addObserver(temp); return temp; }
};
template<typename D>
class slowTrigger : public observerCH<D>
{
private:
public:
    slowTrigger(D* d, focusedEvent* fe) : observerCH<D>(d, fe, "slow") {}
    void triggered();
    slowTrigger<D>* clone(character* hO) { auto temp = new slowTrigger<character>(hO, this->observing_); this->observing_->addObserver(temp); return temp; }
};
class maxHPTrigger : public observer
{
private:
    int lifeGain;
public:
    maxHPTrigger(focusedEvent* fe, int i = 1) : lifeGain(i), observer(fe, "maxlifeGain",98) {}
    void triggered();
    void triggered(character&);
    maxHPTrigger* clone() { auto temp = new maxHPTrigger(observing_, lifeGain); this->observing_->addObserver(temp); return temp;}
};
class drawCardTrigger : public observer
{
private:
    bool evolveCheck{ false };
    int drawNumber{ 1 };
public:
    drawCardTrigger(int i = 1);
    drawCardTrigger(focusedEvent* fe, int i = 1) : drawNumber(i), observer(fe, "drawCard") {}
    drawCardTrigger(focusedEvent* fe, bool evolvecheck, int i = 1) : evolveCheck(evolvecheck), drawNumber(i), observer(fe, "drawCard") {}
    void triggered();
    void triggered(cardHolder& ch);
    drawCardTrigger* clone() { auto temp = new drawCardTrigger(observing_, evolveCheck, drawNumber); this->observing_->addObserver(temp); return temp;}
};
class firebTrigger : public observer
{
private:
    int damageNumber{ 1 };
public:
    firebTrigger(int i = 1);
    firebTrigger(focusedEvent* fe, int i = 1) : damageNumber(i), observer(fe, "FB") {}
    void triggered() { effects::randomDamage(damageNumber); }
    void triggered(cardHolder& ch);
    firebTrigger* clone() { auto temp = new firebTrigger(observing_, damageNumber); this->observing_->addObserver(temp); return temp;}
};
class beatOfDeathTrigger : public observer
{
private:
    int dmg;
public:
    beatOfDeathTrigger(focusedEvent* fe, int i) : dmg(i), observer(fe, "beatOfDeath",150) {}
    void triggered() { effects::Damage(apci.getPlayer(),dmg); }
    beatOfDeathTrigger* clone() { auto temp = new beatOfDeathTrigger(observing_, dmg); this->observing_->addObserver(temp); return temp;}
};
class damageRandomTrigger : public observer
{
private:
    int dmg;
public:
    damageRandomTrigger(focusedEvent* fe, int i) : dmg(i), observer(fe, "randomDamage") {}
    void triggered() { effects::randomDamage(dmg); }
    damageRandomTrigger* clone() { auto temp = new damageRandomTrigger(observing_, dmg); this->observing_->addObserver(temp); return temp;}
};
class damageAllTrigger : public observer
{
private:
    int dmg;
public:
    damageAllTrigger(focusedEvent* fe, int i) : dmg(i), observer(fe, "damageAll") {}
    void triggered() { effects::AOEDamage(dmg); }
    damageAllTrigger* clone() { auto temp = new damageAllTrigger(observing_, dmg); this->observing_->addObserver(temp); return temp;}
};
template<typename D>
class gainEnergyTrigger2 : public observerCA<D>
{
private:
    int nrg{ 1 };
public:
    gainEnergyTrigger2(D* d, focusedEvent* fe, int i) : nrg(i), observerCA<D>(d, fe, "gainEnergy2",98) {}
    void triggered();
    void triggered(cardHolder& ch);
    gainEnergyTrigger2<D>* clone(card* hO) { auto temp = new gainEnergyTrigger2<card>(hO, this->observing_, this->nrg); this->observing_->addObserver(temp); return temp;}
};
template<typename D>
class necroCurseTrigger : public observerCA<D>
{
private:
public:
    necroCurseTrigger(D* d, focusedEvent* fe) : observerCA<D>(d, fe, "necroCurse", 80) {}
    void triggered(cardHolder& ch);
    necroCurseTrigger<D>* clone(card* hO) { auto temp = new necroCurseTrigger<card>(hO, this->observing_); this->observing_->addObserver(temp); return temp; }
};
template<typename D>
class strengthTrigger : public observerCH<D>
{
private:
    int str;
public:
    strengthTrigger(D* d, int i);
    strengthTrigger(D* d, focusedEvent* fe, int i) : str(i), observerCH<D>(d, fe, "strength") {}
    void triggered();
    void triggered(character& enemy);
    strengthTrigger<D>* clone(character* hO) { auto temp = new strengthTrigger<character>(hO, this->observing_, str); this->observing_->addObserver(temp); return temp;}
};
template<typename D>
class vulnerableTrigger : public observerCH<D>
{
private:
    int vulnNumber;
public:
    vulnerableTrigger(D* d, focusedEvent* fe, int i = 1) : vulnNumber(i), observerCH<D>(d, fe, "vulnerable") {}
    void triggered(character&);
    vulnerableTrigger<D>* clone(character* hO) { auto temp = new vulnerableTrigger<character>(hO, this->observing_, vulnNumber); this->observing_->addObserver(temp); return temp;}
};
template<typename D>
class returnStolenTrigger : public observerCH<D>
{
private:
    card* stolenCard{ nullptr };
public:
    returnStolenTrigger(D* d, focusedEvent* fe, card* sC) : stolenCard(sC), observerCH<D>(d, fe, "stolenCardReturn",70) {}
    void triggered(character&);
    returnStolenTrigger<D>* clone(character* hO) 
    {
        auto temp = new returnStolenTrigger<character>(hO, this->observing_,this->stolenCard->safeClone());
        for (auto itr{ temp->stolenCard->getObservers().begin() }; itr != temp->stolenCard->getObservers().end(); ++itr)
        {
            (**itr).setInactive();
        }
        this->observing_->addObserver(temp);
        return temp;
    }
    ~returnStolenTrigger() { if (stolenCard) delete stolenCard; }
};
template<typename D>
class bufferTrigger : public observerCH<D>
{
private:
    int str;
public:
    bufferTrigger(D* d, int i);
    bufferTrigger(D* d, focusedEvent* fe, int i) : str(i), observerCH<D>(d, fe, "buffer") {}
    void triggered();
    bufferTrigger<D>* clone(character* hO) { auto temp = new bufferTrigger<character>(hO, this->observing_, str); this->observing_->addObserver(temp); return temp;}
};
template<typename D>
class clockTrigger : public observerCH<D>
{
private:
public:
    clockTrigger(D* d, focusedEvent* fe) : observerCH<D>(d, fe, "clock") {}
    void triggered();
    clockTrigger<D>* clone(character* hO) { auto temp = new clockTrigger<character>(hO, this->observing_); this->observing_->addObserver(temp); return temp; }
};
template<typename D>
class curiosityTrigger : public observerCH<D>
{
private:
    int str;
public:
    curiosityTrigger(D* d, focusedEvent* fe, int s) : str(s), observerCH<D>(d, fe, "curiosity") {}
    void triggered();
    curiosityTrigger<D>* clone(character* hO) { auto temp = new curiosityTrigger<character>(hO, this->observing_, str); this->observing_->addObserver(temp); return temp; }
};
template<typename D>
class backTrigger : public observerCH<D>
{
private:
public:
    backTrigger(D* d, focusedEvent* fe) :  observerCH<D>(d, fe, "back") {}
    void triggered(cardHolder& ch);
    backTrigger<D>* clone(character* hO) { auto temp = new backTrigger<character>(hO, this->observing_); this->observing_->addObserver(temp); return temp; }
};
template<typename D>
class awakenedDeathTrigger : public observerCH<D>
{
private:
public:
    awakenedDeathTrigger(D* d, focusedEvent* fe) :  observerCH<D>(d, fe, "awakened") {}
    void triggered(character&);
    awakenedDeathTrigger<D>* clone(character* hO) { auto temp = new awakenedDeathTrigger<character>(hO, this->observing_); this->observing_->addObserver(temp); return temp; }
};
template<typename D>
class painLoseLifeTrigger : public observerCA<D>
{
private:
    int lifeLoss;
public:
    painLoseLifeTrigger(D* d, focusedEvent* fe, int i = 1) : lifeLoss(i), observerCA<D>(d, fe, "pain",99) {}
    void triggered();
    painLoseLifeTrigger<D>* clone(card* hO) { auto temp = new painLoseLifeTrigger<card>(hO, this->observing_, lifeLoss); this->observing_->addObserver(temp); return temp;}
};
template<typename D>
class voidTrigger : public observerCA<D>
{
private:
public:
    voidTrigger(D* d, focusedEvent* fe) : observerCA<D>(d, fe, "void", 99) {}
    void triggered(cardHolder& ch);
    voidTrigger<D>* clone(card* hO) { auto temp = new voidTrigger<card>(hO, this->observing_); this->observing_->addObserver(temp); return temp; }
};
template<typename D>
class zeroCostTrigger : public observerCA<D>
{
private:
public:
    zeroCostTrigger(D* d, focusedEvent* fe) : observerCA<D>(d, fe, "zeroCost",98) {}
    void triggered();
    void triggered(cardHolder& ch);
    zeroCostTrigger<D>* clone(card* hO) { auto temp = new zeroCostTrigger<card>(hO, this->observing_); this->observing_->addObserver(temp); return temp;}
};
template<typename D>
class oneCostTrigger : public observerCA<D>
{
private:
public:
    oneCostTrigger(D* d, focusedEvent* fe) : observerCA<D>(d, fe, "oneCost", 98) {}
    void triggered();
    void triggered(cardHolder& ch);
    oneCostTrigger<D>* clone(card* hO) { auto temp = new oneCostTrigger<card>(hO, this->observing_); this->observing_->addObserver(temp); return temp; }
};
template<typename D>
class incrementCostTrigger : public observerCA<D>
{
private:
public:
    incrementCostTrigger(D* d, focusedEvent* fe) : observerCA<D>(d, fe, "incrementCost",98) {}
    void triggered();
    incrementCostTrigger<D>* clone(card* hO)
    {
        auto temp = new incrementCostTrigger<card>(hO, this->observing_); 
        this->observing_->addObserver(temp); 
        return temp;
    }
};
template<typename D>
class rageTrigger : public observerCH<D>
{
public:
    rageTrigger(D* d, focusedEvent* fe) : observerCH<D>(d, fe, "rage") {}
    void triggered();
    rageTrigger<D>* clone(character* hO) { auto temp = new rageTrigger<character>(hO, this->observing_); this->observing_->addObserver(temp); return temp;}
};
template<typename D>
class resetXMultiTrigger : public observerCA<D>
{
public:
    resetXMultiTrigger(D* d, focusedEvent* fe) : observerCA<D>(d, fe, "resetXMulti", 100002) {}
    void triggered(cardHolder& ch);
    resetXMultiTrigger<D>* clone(card* hO)
    {
        auto temp = new resetXMultiTrigger<card>(hO, this->observing_); this->observing_->addObserver(temp); return temp;
    }
};
template<typename D>
class confusedTrigger : public observerCH<D>
{
public:
    confusedTrigger(D* d, focusedEvent* fe) : observerCH<D>(d, fe, "confused") {}
    void triggered(cardHolder& ch);
    confusedTrigger<D>* clone(character* hO)
    {
        auto temp = new confusedTrigger<character>(hO, this->observing_); this->observing_->addObserver(temp); return temp;
    }
};
template<typename D>
class ritualTrigger : public observerCH<D>
{
public:
    ritualTrigger(D* d, focusedEvent* fe) : observerCH<D>(d, fe, "ritual") {}
    void triggered();
    ritualTrigger<D>* clone(character* hO) { auto temp = new ritualTrigger<character>(hO, this->observing_); this->observing_->addObserver(temp); return temp;}
};

template<typename D>
class doublePlayTrigger : public observerCH<D>
{
private:
    int doublePlays_;
    haltObservingTriggerCH<D>* halt{ nullptr };
public:
    doublePlayTrigger(D* d, focusedEvent* fe, int i) : doublePlays_(i), observerCH<D>(d, fe, "doublePlay", 100000) {}
    void triggered(cardHolder& ch);
    void addHalt(haltObservingTriggerCH<D>* h);
    doublePlayTrigger<D>* clone(character* hO) { auto temp = new doublePlayTrigger<character>(hO, this->observing_, doublePlays_); this->observing_->addObserver(temp); return temp;}
};
template<typename D>
class thornsTrigger : public observerCH<D>
{
private:
    int dmg;
public:
    thornsTrigger(D* d, focusedEvent* fe, int i) : dmg(i), observerCH<D>(d, fe, "thorns") {}
    void triggered(character& initiator);
    void triggered();
    thornsTrigger<D>* clone(character* hO) { auto temp = new thornsTrigger<character>(hO, this->observing_, dmg); this->observing_->addObserver(temp); return temp;}
};
template<typename D>
class thiefTrigger : public observerCH<D>
{
private:
    int gold;
public:
    thiefTrigger(D* d, focusedEvent* fe, int i) : gold(i), observerCH<D>(d, fe, "thief") {}
    void triggered(character& initiator);
    thiefTrigger<D>* clone(character* hO) { auto temp = new thiefTrigger<character>(hO, this->observing_, gold); this->observing_->addObserver(temp); return temp; }
};
template<typename D>
class flameBarrierTrigger : public observerCH<D>
{
private:
    int dmg;
public:
    flameBarrierTrigger(D* d, focusedEvent* fe, int i) : dmg(i), observerCH<D>(d, fe, "flamebar") {}
    void triggered(character& initiator);
    void triggered();
    flameBarrierTrigger<D>* clone(character* hO) { auto temp = new flameBarrierTrigger<character>(hO, this->observing_, dmg); this->observing_->addObserver(temp); return temp;}
};
template<typename D>
class sharpHideTrigger : public observerCH<D>
{
public:
    sharpHideTrigger(D* d, focusedEvent* fe) : observerCH<D>(d, fe, "sharpHide") {}
    void triggered();
    sharpHideTrigger<D>* clone(character* hO) { auto temp = new sharpHideTrigger<character>(hO, this->observing_); this->observing_->addObserver(temp); return temp;}
};
template<typename D>
class louseArmourTrigger : public observerCH<D>
{
private:
    int blockNumber;
public:
    louseArmourTrigger(D* d, focusedEvent* fe, int i) : blockNumber(i), observerCH<D>(d, fe, "louseArmour") {}
    void triggered();
    louseArmourTrigger<D>* clone(character* hO) { auto temp = new louseArmourTrigger<character>(hO, this->observing_, blockNumber); this->observing_->addObserver(temp); return temp;}
};
template<typename D>
class malleableTrigger : public observerCH<D>
{
private:
    int blockNumber;
public:
    malleableTrigger(D* d, focusedEvent* fe, int i) : blockNumber(i), observerCH<D>(d, fe, "malleable") {}
    void triggered();
    malleableTrigger<D>* clone(character* hO) { auto temp = new malleableTrigger<character>(hO, this->observing_, blockNumber); this->observing_->addObserver(temp); return temp; }
};
template<typename D>
class sotBlockTrigger : public observerCH<D>
{
    friend character;
public:
    sotBlockTrigger(D* d, focusedEvent* fe) : observerCH<D>(d, fe, "sotBlock") {}
    void triggered();
    sotBlockTrigger<D>* clone(character* hO) { auto temp = new sotBlockTrigger<character>(hO, this->observing_); this->observing_->addObserver(temp); return temp;}
};
template<typename D>
class calipersTrigger : public observerCH<D>
{
public:
    calipersTrigger(D* d, focusedEvent* fe) : observerCH<D>(d, fe, "calipers") {}
    void triggered();
    calipersTrigger<D>* clone(character* hO) { auto temp = new calipersTrigger<character>(hO, this->observing_); this->observing_->addObserver(temp); return temp;}
};
template<typename D>
class barricadeTrigger : public observerCH<D>
{
public:
    barricadeTrigger(D* d, focusedEvent* fe) : observerCH<D>(d, fe, "barricade") {}
    void triggered() {}
    barricadeTrigger<D>* clone(character* hO) { auto temp = new barricadeTrigger<character>(hO, this->observing_); this->observing_->addObserver(temp); return temp;}
};

template<typename D>
haltObservingTriggerCH<D>::haltObservingTriggerCH(D* d, observer* obs, int trigP) : obs_(obs), observerCH<D>(d, gm.getPlayerTurnEnd(), "haltObserving", trigP) {}
template<typename D>
haltObservingTriggerCA<D>::haltObservingTriggerCA(D* d, observer* obs, int trigP) : obs_(obs), observerCA<D>(d, gm.getPlayerTurnEnd(), "haltObserving", trigP) {}

template<typename D>
void gainEnergyTrigger2<D>::triggered() { gm.increasePlayerEnergy(nrg); }

template<typename D>
void gainEnergyTrigger2<D>::triggered(cardHolder& ch) { if (ch.getCardPointer() == this->holdsObs_) triggered(); }

template<typename D>
void necroCurseTrigger<D>::triggered(cardHolder& ch) 
{
    if (ch.getCardPointer() == this->holdsObs_)
    {
        apci.addCardToHand(ch);
    }
}

template<typename D>
strengthTrigger<D>::strengthTrigger(D* d, int i) : str(i), observerCH<D>(d, gm.getPlayerTurnStart(), "strength") {}
template<typename D>
void strengthTrigger<D>::triggered() { effects::Strength(static_cast<character&>(*this->holdsObs_), str); }
template<typename D>
void strengthTrigger<D>::triggered(character& enemy)
{
    if (enemy.getSCRNIndex() == this->holdsObs_->getSCRNIndex())
    {
        effects::Strength(static_cast<character&>(*this->holdsObs_), str);
    }
}

template<typename D>
void vulnerableTrigger<D>::triggered(character& c) { if (c.getSCRNIndex() == this->holdsObs_->getSCRNIndex()) effects::Vulnerable(apci.getPlayer(), vulnNumber); }

template<typename D>
void thiefTrigger<D>::triggered(character& enemy) { if (enemy.getSCRNIndex() == this->holdsObs_->getSCRNIndex()) apci.gainGold(this->gold); }

template<typename D>
void ritualDaggerFatalTrigger<D>::triggered(character& c) 
{
    if (c.getSCRNIndex() == gamestate::lastAttackedSCRNIndex)
    {
        this->holdsObs_->incrementEnergy();
    }
}

template<typename D>
void slowTrigger<D>::triggered() { this->holdsObs_->addSlowed(1); }


template<typename D>
void returnStolenTrigger<D>::triggered(character& c) 
{
    if (c.getSCRNIndex() == this->holdsObs_->getSCRNIndex())
    {
        for (auto itr{ this->stolenCard->getObservers().begin() }; itr != this->stolenCard->getObservers().end(); ++itr)
        {
            (**itr).setActive();
        }
        apci.addCardToHand(cardHolder(this->stolenCard));
        this->stolenCard = nullptr;
        this->holdsObs_->safeRemoveObserver(this);
        delete this;
    }
}

template<typename D>
void confusedTrigger<D>::triggered(cardHolder& ch) { (*ch).changeTempEnergy(random_int(0, 3)); }

template<typename D>
bufferTrigger<D>::bufferTrigger(D* d, int i) : str(i), observerCH<D>(d, gm.getPlayerTurnStart(), "buffer") {}
template<typename D>
void bufferTrigger<D>::triggered() {}

template<typename D>
void painLoseLifeTrigger<D>::triggered()
{
    if (apci.checkCardInHand(this->holdsObs_)) { apci.getPlayer().loseHealth(lifeLoss); }
}
template<typename D>
void voidTrigger<D>::triggered(cardHolder& ch)
{
    if (ch.getCardPointer() == this->holdsObs_ && gm.getPlayerEnergy() > 0)
    {
        gm.decreasePlayerEnergy(1);
    }
}

template<typename D>
void zeroCostTrigger<D>::triggered()
{
    this->holdsObs_->setZeroUntilPlayed(false);
    this->holdsObs_->safeRemoveObserver(this);
    delete this;
}
template<typename D>
void zeroCostTrigger<D>::triggered(cardHolder& ch)
{
    if (ch.getCardPointer() == this->holdsObs_)
    {
        this->holdsObs_->setZeroUntilPlayed(false);
        this->holdsObs_->safeRemoveObserver(this);
        delete this;
    }
}
template<typename D>
void oneCostTrigger<D>::triggered()
{
    this->holdsObs_->setOneUntilPlayed(false);
    this->holdsObs_->safeRemoveObserver(this);
    delete this;
}
template<typename D>
void oneCostTrigger<D>::triggered(cardHolder& ch)
{
    if (ch.getCardPointer() == this->holdsObs_)
    {
        this->holdsObs_->setOneUntilPlayed(false);
        this->holdsObs_->safeRemoveObserver(this);
        delete this;
    }
}
template<typename D>
void incrementCostTrigger<D>::triggered() { this->holdsObs_->incrementEnergy(); }
template<typename D>
void rageTrigger<D>::triggered()
{
    effects::Strength(*this->holdsObs_, this->holdsObs_->getEnrage());
    if (this->holdsObs_->getEnrage() == 0) {
        this->holdsObs_->safeRemoveObserver(this);
        delete this;
    }
}

template<typename D>
void ritualTrigger<D>::triggered()
{
    effects::Strength(*this->holdsObs_, this->holdsObs_->getRitual());
    if (this->holdsObs_->getRitual() == 0) {
        this->holdsObs_->safeRemoveObserver(this);
        delete this;
    }
}
template<typename D>
void doublePlayTrigger<D>::triggered(cardHolder& ch)
{
    int dblCopy = doublePlays_;
    if (halt) this->holdsObs_->removeObserver(halt);
    this->holdsObs_->safeRemoveObserver(this);
    for (int i{ 0 }; i < dblCopy; ++i)
    {
        playCardDup(ch);
    }
    apci.getPlayer().addAltBuffs(Intents("DBT", 0, 1));
    apci.getPlayer().addAltBuffs(Intents("DUP", 0, 1));
    delete this;
}
template<typename D>
void resetXMultiTrigger<D>::triggered(cardHolder& ch)
{
    if (ch.getCardPointer() == this->holdsObs_ && !gamestate::dontDupe)
    {
        this->holdsObs_->incrementEnergy();
        this->holdsObs_->safeRemoveObserver(this);
        delete this;
    }
}
template<typename D>
void doublePlayTrigger<D>::addHalt(haltObservingTriggerCH<D>* h) { halt = h; }

template<typename D>
void clockTrigger<D>::triggered()
{
    if (this->holdsObs_->getClock12() == 11)
    {
        gamestate::denyCardPlay = true;
        this->holdsObs_->gainStrength(2);
        gamestate::clockEnd = true;
    }
    this->holdsObs_->uptickClock();
}
template<typename D>
void curiosityTrigger<D>::triggered()
{
    if (this->holdsObs_->getCuriosity() > 0)
    {
        this->holdsObs_->gainStrength(str);
    }
}
template<typename D>
void backTrigger<D>::triggered(cardHolder& ch)
{
    if ((*ch).getCombatChoices() != combatChoices::EnemyTarget) return;
    this->holdsObs_->setBackAttack(gamestate::lastAttackedSCRNIndex != this->holdsObs_->getSCRNIndex());
}
template<typename D>
void awakenedDeathTrigger<D>::triggered(character& enemy)
{
    if (enemy.getSCRNIndex() == this->holdsObs_->getSCRNIndex())
    {
        setAOstored(clamp(this->holdsObs_->getTotalStrength(), 0, 99));
    }
}
template<typename D>
void thornsTrigger<D>::triggered(character& enemy) 
{
    if (enemy.getSCRNIndex() == this->holdsObs_->getSCRNIndex())
    {
        apci.getPlayer().takeDamageNoTrigger(dmg);
    }
    else if (this->holdsObs_->getSCRNIndex() == -1)
    {
        enemy.takeDamageNoTrigger(dmg);
    }
}
template<typename D>
void thornsTrigger<D>::triggered()
{
    effects::Damage(apci.getPlayer(), this->holdsObs_->getThorns());
    if (this->holdsObs_->getThorns() == 0) {
        this->holdsObs_->safeRemoveObserver(this);
        delete this;
    }
}
template<typename D>
void flameBarrierTrigger<D>::triggered(character& initiator)
{
    if (this->holdsObs_->getFlameBar() == 0) {
        this->holdsObs_->safeRemoveObserver(this);
        delete this;
    }
    else { initiator.takeDamageNoTrigger(dmg); }
}
template<typename D>
void flameBarrierTrigger<D>::triggered() {}
template<typename D>
void sharpHideTrigger<D>::triggered()
{
    effects::Damage(apci.getPlayer(), this->holdsObs_->getSharpHide());
    if (this->holdsObs_->getSharpHide() == 0) {
        this->holdsObs_->safeRemoveObserver(this);
        delete this;
    }
}
template<typename D>
void louseArmourTrigger<D>::triggered() { effects::Block(*(this->holdsObs_), blockNumber); }

template<typename D>
void malleableTrigger<D>::triggered() { effects::Block(*(this->holdsObs_), blockNumber); }

template<typename D>
void sotBlockTrigger<D>::triggered() { this->holdsObs_->setBlockZero(); }
template<typename D>
void calipersTrigger<D>::triggered()
{
    this->holdsObs_->getBlock() > 15 ?
        this->holdsObs_->setBlock(clamp(this->holdsObs_->getBlock() - 15, 0, 15)) :
        this->holdsObs_->setBlock(0);
}
//class dupDBLTEndTrigger;
//class dupDBLTStartTrigger : public observer
//{
//private:
//    dupDBLTEndTrigger* DUPe;
//public:
//    dupDBLTStartTrigger(focusedEvent* fe, dupDBLTEndTrigger* de) : DUPe(de), observer(fe, "dupDBLTStart", 1) {}
//    void triggered(cardHolder& ch);
//    dupDBLTStartTrigger* clone() { auto temp = new dupDBLTStartTrigger(observing_, DUPe); this->observing_->addObserver(temp); return temp; }
//};
//class dupDBLTEndTrigger : public observer
//{
//    friend dupDBLTStartTrigger;
//private:
//    vector<card*> lookFor;
//    bool AC{ false };
//public:
//    dupDBLTEndTrigger(focusedEvent* fe, bool ac = false, vector<card*> lF = {}) : lookFor(lF), AC(ac), observer(fe, "dupDBLTEnd", 100000) {}
//    void triggered(cardHolder& ch);
//    dupDBLTEndTrigger* clone() { auto temp = new dupDBLTEndTrigger(observing_, AC, lookFor); this->observing_->addObserver(temp); return temp; }
//};
//class dupPotEndTrigger;
//class dupPotStartTrigger : public observer
//{
//private:
//    dupPotEndTrigger* DUPe;
//public:
//    dupPotStartTrigger(focusedEvent* fe, dupPotEndTrigger* de) : DUPe(de), observer(fe, "dupPotStart",1) {}
//    void triggered(cardHolder& ch);
//    dupPotStartTrigger* clone() { auto temp = new dupPotStartTrigger(observing_,DUPe); this->observing_->addObserver(temp); return temp; }
//};
//class dupPotEndTrigger : public observer
//{
//friend dupPotStartTrigger;
//private:
//    vector<card*> lookFor;
//    bool AC{ false };
//public:
//    dupPotEndTrigger(focusedEvent* fe, bool ac = false, vector<card*> lF = {}) : lookFor(lF), AC(ac), observer(fe, "dupPotEnd",100000) {}
//    void triggered(cardHolder& ch);
//    dupPotEndTrigger* clone() { auto temp = new dupPotEndTrigger(observing_,AC,lookFor); this->observing_->addObserver(temp); return temp; }
//};
//
//class necroStartTrigger : public observer
//{
//private:
//    necronomicon* DUPe;
//public:
//    necroStartTrigger(focusedEvent* fe, necronomicon* de) : DUPe(de), observer(fe, "necroStart", 1) {}
//    void triggered(cardHolder& ch);
//    necroStartTrigger* clone() { auto temp = new necroStartTrigger(observing_, DUPe); this->observing_->addObserver(temp); return temp; }
//};
class dupDBLTEndTrigger : public observer
{
private:
    int alreadyDuped;
public:
    dupDBLTEndTrigger(focusedEvent* fe, int aD = 0) : alreadyDuped(aD), observer(fe, "dupDBLTEnd", 100000) {}
    void triggered(cardHolder& ch);
    dupDBLTEndTrigger* clone() { auto temp = new dupDBLTEndTrigger(observing_,alreadyDuped); this->observing_->addObserver(temp); return temp; }
};
class dupPotEndTrigger : public observer
{
private:
    int alreadyDuped;
public:
    dupPotEndTrigger(focusedEvent* fe, int aD = 0) : alreadyDuped(aD), observer(fe, "dupPotEnd",100000) {}
    void triggered(cardHolder& ch);
    dupPotEndTrigger* clone() { auto temp = new dupPotEndTrigger(observing_,alreadyDuped); this->observing_->addObserver(temp); return temp; }
};

#endif
