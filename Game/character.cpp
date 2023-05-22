#include "character.h"
#include "effects_triggers.h"
#include "gamestate.h"
#include "pc_info.h"
#include "visuals.h"
#include "potions.h"

using namespace std;

// health gain and loss
void character::heal(int amount) { if (health_ == 0) return; health_ = clamp(health_ + amount, 0, max_health_); }
void pc::heal(int amount)
{
    if (gamestate::markOfTheBloomCheck) return;
    if (magicFlowerCheck && (gm.getMapPathEvent() == 'e' || gm.getMapPathEvent() == 'E' || gm.getMapPathEvent() == 'B'))
    {
        float a{ 1.5 }, b{ static_cast<float>(amount) };
        a = ceil(a * b);
        amount = static_cast<int>(a);
    }
    health_ = clamp(health_ + amount, 0, max_health_);
    gm.callPlayerHealthGain();
}
void character::healPercentage(int pc) { heal(max_health_ * pc / 100); }
void character::healToFull() { heal(max_health_ - health_); }
void character::gainMaxHP(int amount) { max_health_ += amount; heal(amount); if (!ai) std::cout << "Gained " << amount << " Max HP." << endl; }
void character::loseMaxHP(int amount) { max_health_ -= amount; if (health_ > max_health_) health_ = max_health_; }
void character::eventLoseHP(int amount) 
{
    if (gamestate::tungstenCheck) amount -= 1;
    health_ = clamp(health_ - amount, 0, max_health_);
    if(!ai) std::cout << "Lost "<<amount<<" HP. (Health: " << health_ << "/" << max_health_ <<")" << endl;
    if (health_ == 0) gm.callPlayerDied();
    if (health_ == 0) throw PlayerDiedError("dead");
}
void character::eventLosePercentageHP(int pc) { eventLoseHP(max_health_ * pc / 100); }
void character::losePercentageMaxHP(int pc) { loseMaxHP(max_health_ * pc / 100); }

// strength and dexterity
void character::gainStrength(int i)
{
    if (i < 0) throw runtime_error("gstr");
    if (m_strength_ == 0)
    {
        strength_ = clamp(strength_ + i, 0, 99);
    }
    else
    {
        m_strength_ -= i;
        if (m_strength_ >= 0) return;
        strength_ -= m_strength_;
        m_strength_ = 0;
    }
}
void character::gainDexterity(int i)
{
    if (i < 0) throw runtime_error("gdex");
    if (m_dexterity_ == 0)
    {
        dexterity_ = clamp(dexterity_ + i, 0, 99);
    }
    else
    {
        m_dexterity_ -= i;
        if (m_dexterity_ >= 0) return;
        dexterity_ -= m_dexterity_;
        m_dexterity_ = 0;
    }
}
void character::loseStrength(int i)
{
    if (useArtifactCharge()) return;
    if (i < 0) throw runtime_error("lstr");
    if (strength_ == 0)
    {
        m_strength_ = clamp(m_strength_ + i, 0, 99);
    }
    else
    {
        strength_ -= i;
        if (strength_ >= 0) return;
        m_strength_ -= strength_;
        strength_ = 0;
    }
}
void character::loseDexterity(int i)
{
    if (useArtifactCharge()) return;
    if (i < 0) throw runtime_error("ldex");
    if (dexterity_ == 0)
    {
        m_dexterity_ = clamp(m_dexterity_ + i, 0, 99);
    }
    else
    {
        dexterity_ -= i;
        if (dexterity_ >= 0) return;
        m_dexterity_ -= dexterity_;
        dexterity_ = 0;
    }
}
void character::changeEotStrength(int amount)
{
    if (amount < 0) { if (useArtifactCharge()) return; }
    eotStrength = clamp(eotStrength + amount, -99, 99);
    addAltBuffs(Intents("eotSTR", eotStrength, 1));
}
void character::changeEotDexterity(int amount) 
{
    if (amount < 0) { if (useArtifactCharge()) return; }
    eotDexterity = clamp(eotDexterity + amount, -99, 99);
    addAltBuffs(Intents("eotDEX", eotDexterity, 1)); 
}

void character::gainBlock(int amount)
{
    block_ = clamp(block_ + amount, 0, 99);
}
void pc::gainBlock(int amount)
{
    block_ = clamp(block_ + amount, 0, 999);
    gm.callGainBlock();
}

// debuffs
void character::applyVulnerable(int amount)
{
    if (useArtifactCharge()) return;
    vulnerable_ = clamp(amount + vulnerable_, 0, 99);
}
void character::applyWeak(int amount)
{
    if (useArtifactCharge()) return;
    weak_ = clamp(amount + weak_, 0, 99);
}
void character::addAltBuffs(Intents buff)
{
    for (auto itr{ altBuffs.begin() }; itr < altBuffs.end(); ++itr)
    {
        if (itr->effect == buff.effect)
        {
            itr->value = buff.value;
            itr->times = buff.times;
            return;
        }
    }
    altBuffs.push_back(buff);
}
void character::noDrawDebuff() 
{
    if (useArtifactCharge()) return;
    apci.getPlayer().addAltBuffs({ Intents("NoDraw",1000,1) });
    apci.getPlayer().observeEvent<removeBuffVisTrigger>(gm.getPlayerTurnEnd(), new removeBuffVisTrigger(Intents("NoDraw", 1000, 1)), true);
    no_draw_ = true;
}
void character::noBlockDebuff(int i)
{
    if (useArtifactCharge()) return; 
    no_block_ += i; 
    apci.getPlayer().addAltBuffs({ Intents("NoBlock",no_block_,1) });
}
// no trigger damage is anything that's not attack damage, taking damage and losing health are distinct, the distinction is important, losing health always triggers
bool character::takeDamageNoTrigger(int amount)
{
    if (amount == 0) return false;
    int healthLoss{ 0 };

    if (block_ == 0) healthLoss = amount;
    else if (block_ > amount) { healthLoss = 0; block_ -= amount; }
    else { healthLoss = amount - block_; loseAllBlock(); }
    if (healthLoss == 0) return false;
    if (loseHealth(healthLoss))return true;
    return false;
}
// attack damage
bool character::takeDamage(int amount, character& initiator)
{
    //if (amount == 0) return false;
    int healthLoss{ 0 };

    if (block_ == 0) healthLoss = amount;
    else if (block_ > amount) { healthLoss = 0; block_ -= amount; }
    else { healthLoss = amount - block_; loseAllBlock(); }

    &initiator == &apci.getPlayer() ? gm.callEnemyTakingAttackDamage(*this) : gm.callPlayerTakingAttackDamage(initiator);
    if (healthLoss == 0) return false;
    if (plated_armour_) losePlatedArmour(1);
    if (malleable_ > 0) 
    {
        auto trig = new malleableTrigger<character>(this, gm.getAttackPlayed(), malleable_);
        auto haltTrig = new haltObservingTriggerCH<character>(this, gm.getAttackPlayed(), trig);
        observeEvent<malleableTrigger<character>>(gm.getAttackPlayed(), trig);
        observeEvent<haltObservingTriggerCH<character>>(gm.getAttackPlayed(), haltTrig);
        gainMalleable();
    }
    return loseHealth(healthLoss);
}
// start of turn block is governed by either barricade buff, calipers or normal (lose all)
void character::changeSOTBlocksObs(observer* obs)
{
    if (sotBlockObs)
    {
        sotBlockObs->getObserving()->removeObserver(sotBlockObs);
        sotBlockObs->getObserving()->addObserver(obs);
        delete sotBlockObs;
    }
    else
    {
        gm.getSOTBlock()->addObserver(obs);
        obs->observeThis(gm.getSOTBlock());
    }
    sotBlockObs = obs;
}

// power buffs
void character::addBARbuff(int amount) { BarricadeBuff = clamp(BarricadeBuff + amount, 0, 1); }
void character::addBSKbuff(int amount) { BerserkBuff = clamp(BerserkBuff + amount, 0, 99); }
void character::addCORbuff(int amount) { CorruptionBuff = clamp(CorruptionBuff + amount, 0, 1); }
void character::addDMNbuff(int amount) { DemonFormBuff = clamp(DemonFormBuff + amount, 0, 99); }
void character::addRUPbuff(int amount) { RuptureBuff = clamp(RuptureBuff + amount, 0, 99); }

void character::addBRTbuff(int amount) 
{
    BrutalityBuff = clamp(BrutalityBuff + amount, 0, 99);
    addAltBuffs(Intents("BRT", BrutalityBuff, 1));
    removeObserverIfFE<loseLifeTrigger>(gm.getPlayerTurnStart());
    removeObserverIfFE<drawCardTrigger>(gm.getPlayerTurnStart());
    observeEvent<loseLifeTrigger>(gm.getPlayerTurnStart(), new loseLifeTrigger(gm.getPlayerTurnStart(), BrutalityBuff), true);
    observeEvent<drawCardTrigger>(gm.getPlayerTurnStart(), new drawCardTrigger(gm.getPlayerTurnStart(), BrutalityBuff), true);
}
void character::addCMBbuff(int amount) 
{
    CombustBuff = clamp(CombustBuff + amount, 0, 99);
    CombustBuffStacks = clamp(CombustBuffStacks + 1, 0, 99);
    addAltBuffs(Intents("CMB", CombustBuff, 1));
    removeObserver<combustTrigger>();
    observeEvent<combustTrigger>(gm.getPlayerTurnEnd(), new combustTrigger(gm.getPlayerTurnEnd(), CombustBuffStacks, CombustBuff), true);
}
void character::addDRKbuff(int amount) 
{
    DarkEmbraceBuff = clamp(DarkEmbraceBuff + amount, 0, 99); 
    addAltBuffs(Intents("DRK", DarkEmbraceBuff, 1));
    removeObserverIfFE<drawCardTrigger>(gm.getCardExhausted());
    observeEvent<drawCardTrigger>(gm.getCardExhausted(), new drawCardTrigger(gm.getCardExhausted(), DarkEmbraceBuff), true);
}
void character::addEVObuff(int amount) 
{
    EvolveBuff = clamp(EvolveBuff + amount, 0, 99); 
    addAltBuffs(Intents("EVO", EvolveBuff, 1));
    removeObserverIfFE<drawCardTrigger>(gm.getCardDrawn());
    observeEvent<drawCardTrigger>(gm.getCardDrawn(), new drawCardTrigger(gm.getCardDrawn(), true, EvolveBuff), true);
}
void character::addFNPbuff(int amount) 
{
    FeelNoPainBuff = clamp(FeelNoPainBuff + amount, 0, 99); 
    addAltBuffs(Intents("FNP", FeelNoPainBuff, 1));
    removeObserver<feelNoPainTrigger>();
    observeEvent<feelNoPainTrigger>(gm.getCardExhausted(), new feelNoPainTrigger(gm.getCardExhausted(), FeelNoPainBuff), true);
}
void character::addFRBbuff(int amount) 
{
    FireBreathingBuff = clamp(FireBreathingBuff + amount, 0, 99); 
    addAltBuffs(Intents("FRB", FireBreathingBuff, 1));
    removeObserver<firebTrigger>();
    observeEvent<firebTrigger>(gm.getCardDrawn(), new firebTrigger(gm.getCardDrawn(), FireBreathingBuff), true);
}
void character::addJGRbuff(int amount) 
{
    JuggernautBuff = clamp(JuggernautBuff + amount, 0, 99);
    addAltBuffs(Intents("JUG", JuggernautBuff, 1));
    removeObserverIfFE<damageRandomTrigger>(gm.getGainBlock());
    observeEvent<damageRandomTrigger>(gm.getGainBlock(), new damageRandomTrigger(gm.getGainBlock(), JuggernautBuff), true);
}
void character::addMAGbuff(int amount)
{
    MagnetismBuff = clamp(MagnetismBuff + amount, 0, 99); 
    addAltBuffs(Intents("MAG", MagnetismBuff, 1));
    removeObserver<magnetismTrigger>();
    observeEvent<magnetismTrigger>(gm.getPlayerTurnStart(), new magnetismTrigger(gm.getPlayerTurnStart(), MagnetismBuff), true);

}
void character::addMAYbuff(int amount) 
{
    MayhemBuff = clamp(MayhemBuff + amount, 0, 99); 
    addAltBuffs(Intents("MAY", MayhemBuff, 1));
    removeObserver<mayhemTrigger>();
    observeEvent<mayhemTrigger>(gm.getPlayerTurnStart(), new mayhemTrigger(gm.getPlayerTurnStart(), MayhemBuff), true);
} 
void character::addPANbuff(int amount) 
{
    PanacheBuff = clamp(PanacheBuff + amount, 0, 99); 
    addAltBuffs(Intents("PAN", PanacheBuff, PanacheCounter));
    removeObserver<panacheTrigger>();
    observeEvent<panacheTrigger>(gm.getCardPlayed(), new panacheTrigger(gm.getCardPlayed(), PanacheBuff), true);
}
void character::addSADbuff(int amount)
{
    SadisticBuff = clamp(PanacheBuff + amount, 0, 99);
    addAltBuffs(Intents("SAD", SadisticBuff, 1));
    removeObserver<sadisticTrigger>();
    observeEvent<sadisticTrigger>(gm.getApplyingDebuff(), new sadisticTrigger(gm.getApplyingDebuff(), SadisticBuff), true);
}
void character::addFlameBar(int amount)
{
    flameBar_ = clamp(flameBar_ + amount, 0, 99);
    addAltBuffs(Intents("FLM B", flameBar_, 1));
    removeObserver<flameBarrierTrigger<character>>();
    observeEvent<flameBarrierTrigger<character>>(gm.getPlayerTakingAttackDamage(), new flameBarrierTrigger<character>(&apci.getPlayer(), gm.getPlayerTakingAttackDamage(), flameBar_), true);
}
void character::addDUPPotion(int amount)
{
    DUPPotion += amount;
    addAltBuffs(Intents("DUP", DUPPotion, 1));
    auto endTrig = new dupPotEndTrigger(gm.getCardPlayed());
    observeEvent<dupPotEndTrigger>(gm.getCardPlayed(), endTrig);
}
void character::addDUPDoubleTap(int amount)
{
    DUPDoubleTap += amount;
    addAltBuffs(Intents("DBLT", DUPDoubleTap, 1));
    auto endTrig = new dupDBLTEndTrigger(gm.getAttackPlayed());
    observeEvent<dupDBLTEndTrigger>(gm.getAttackPlayed(), endTrig);
}
void character::loseDUPPotion(int amount)
{
    DUPPotion = clamp(DUPPotion - amount,0,99);
    addAltBuffs(Intents("DUP", DUPPotion, 1));
}
void character::loseDUPDoubleTap(int amount, bool eot)
{
    DUPDoubleTap = clamp(DUPDoubleTap - amount, 0, 99);
    addAltBuffs(Intents("DBLT", DUPDoubleTap, 1));
    if (DUPDoubleTap == 0 && eot)
    {
        removeObserver<dupDBLTEndTrigger>();
    }
}
void character::applyBomb(int dmg, int turns)
{
    bomb_damage_turn_.push_back({ dmg, turns });
    resetBombBuffs();
    observeEvent<bombTrigger>(gm.getPlayerTurnEnd(), new bombTrigger(gm.getPlayerTurnEnd(),dmg,turns), true);
}
void character::resetBombBuffs()
{
    for (int i{ 0 }; i < bomb_damage_turn_.size(); ++i)
    {
        if (bomb_damage_turn_[i].first == 0 && bomb_damage_turn_[i].second == 0)
        {
            bomb_damage_turn_.erase(bomb_damage_turn_.begin() + i);
            break;
        } 
    }
    for (int i{ 0 }; i < bomb_damage_turn_.size(); ++i)
    {
        addAltBuffs(Intents("BOMB"+to_string(i), bomb_damage_turn_[i].first, bomb_damage_turn_[i].second));
    }
}
// debuffs
void character::applyHexed()
{
    if (useArtifactCharge()) return;
    hexed_ = true;
    addAltBuffs(Intents("HEX", 1000, 1));
    observeEvent<hexTrigger>(gm.getCardPlayed(), new hexTrigger(gm.getCardPlayed()), true);
}
void character::applyConfused()
{
    if (useArtifactCharge()) return;
    confused_ = true;
    addAltBuffs(Intents("CONF", 1000, 1));
    observeEvent<confusedTrigger<character>>(gm.getCardDrawn(), new confusedTrigger<character>(&apci.getPlayer(), gm.getCardDrawn()), true);
}
void character::applyDrawReduction(int amount)
{
    if (useArtifactCharge()) return;
    draw_reduction_ += amount;
    addAltBuffs(Intents("DrawRed", 1000, 1));
}
void pc::applyConstricted(int amount)
{
    if (useArtifactCharge()) return;
    constricted_ += amount;
    addAltBuffs(Intents("CONS", constricted_, 1));
    observeEvent<constrictedTrigger>(gm.getPlayerTurnEnd(), new constrictedTrigger(gm.getPlayerTurnEnd(), constricted_), true);
}
// clearing debuffs
void pc::clearConstricted()
{
    constricted_ = 0;
    addAltBuffs(Intents("CONS", 0, 1));
    removeObserver<constrictedTrigger>();
}
void character::removeHexed()
{
    if (!hexed_) return;
    hexed_ = false;
    addAltBuffs(Intents("HEX", 0, 1));
    removeObserver<hexTrigger>();
}
void character::removeConfused()
{
    if (!confused_) return;
    confused_ = false;
    addAltBuffs(Intents("CONF", 0, 1));
    removeObserver<confusedTrigger<character>>();
}
void character::downtickNoBlock()
{
    if (no_block_ == 0) return;
    --no_block_;
    addAltBuffs(Intents("NoBlock", no_block_, 1));
}
void character::downtickDrawReduction()
{
    if (draw_reduction_ == 0) return;
    --draw_reduction_;
    addAltBuffs(Intents("DrawRed", draw_reduction_, 1));
}
void character::sofMalleable(int amount)
{
    base_malleable = amount;
    malleable_ = amount;
    addAltBuffs(Intents("MALL", malleable_, 1));
}
void character::gainMalleable()
{
    ++malleable_;
    addAltBuffs(Intents("MALL", malleable_, 1));
}
void pc::applyVulnerable(int amount)
{
    if (useArtifactCharge()) return;
    new_vulnerable_ = amount;
    vulnerable_ = clamp(amount + vulnerable_, 0, 99);
}
void pc::applyWeak(int amount)
{
    if (useArtifactCharge()) return;
    new_weak_ = amount;
    weak_ = clamp(amount + weak_, 0, 99);
}
void pc::applyFrail(int amount)
{
    if (useArtifactCharge()) return;
    new_frail_ = amount;
    frail_ = clamp(amount + frail_, 0, 99);
    gm.callApplyingFrail();
}
void nonpc::applyVulnerable(int amount)
{
    character::applyVulnerable(amount);
    gm.callApplyingVulnerable(*this);
}
void nonpc::applyWeak(int amount)
{
    character::applyWeak(amount);
    gm.callApplyingWeak(*this);
}
void pc::applyEntangle(int amount)
{
    if (useArtifactCharge()) return;
    new_entangle_ = amount;
    entangle_ += amount;
    addAltBuffs(Intents("Entangled", 1000, 1));
    card::canPlayAttack = false;
}
// eot means the end of the player's turn, this is the same for both overrides pc and nonpc
void pc::eotCleanup()
{
    new_weak_ = 0; new_frail_ = 0; new_vulnerable_ = 0;
    gm.callPlayerTurnEnd();
    if (metallicise_) gainBlock(metallicise_);
    if (plated_armour_) gainBlock(plated_armour_);
    if (regeneration_) { heal(regeneration_); loseRegeneration(1); }
    if (DUPDoubleTap) { loseDUPDoubleTap(DUPDoubleTap,true); }
    if (eotDexterity != 0)
    {
        eotDexterity > 0 ? gainDexterity(eotDexterity) : loseDexterity(-eotDexterity);
        eotDexterity = 0;
        addAltBuffs(Intents("eotDEX", 0, 1));
    }
    if (eotStrength != 0)
    {
        eotStrength > 0 ? gainStrength(eotStrength) : loseStrength(-eotStrength);
        eotStrength = 0;
        addAltBuffs(Intents("eotSTR", 0, 1));
    }
}
void nonpc::eotCleanup()
{
    //block_ = 0;
    if (slowed_) { slowed_ = 0; addAltBuffs(Intents("Slowed", 0, 1)); }
    if (base_invincible_ > 0) invincible_ = base_invincible_;
    gm.callEnemyTurnStart();
}
void nonpc::sotCleanup()
{
    if (weak_) --weak_;
    if (vulnerable_) --vulnerable_;
    if (frail_) --frail_;
    if (metallicise_) gainBlock(metallicise_);
    if (plated_armour_) gainBlock(plated_armour_);
    if (malleable_ != base_malleable) { malleable_ = base_malleable; addAltBuffs({ Intents("MALL",malleable_,1) }); }
    downtickIntangible();
    if (eotDexterity != 0)
    {
        eotDexterity > 0 ? gainDexterity(eotDexterity) : loseDexterity(-eotDexterity);
        eotDexterity = 0;
        addAltBuffs(Intents("eotDEX", 0, 1));
    }
    if (eotStrength != 0)
    {
        eotStrength > 0 ? gainStrength(eotStrength) : loseStrength(-eotStrength);
        eotStrength = 0;
        addAltBuffs(Intents("eotSTR", 0, 1));
    }
    if (regeneration_) { heal(regeneration_); loseRegeneration(1); }
}
void pc::clearEntangle()
{
    addAltBuffs(Intents("Entangled", 0, 1));
    entangle_ = 0;
    new_entangle_ = 0;
    card::canPlayAttack = true;
}
void pc::clearFlameBarrier()
{
    if(ragee_) addAltBuffs(Intents("RAGE", 0, 1));
    ragee_ = 0;
    if(flameBar_) addAltBuffs(Intents("FLM B", 0, 1));
    flameBar_ = 0;
    removeObserver<flameBarrierTrigger<character>>();
}
void pc::clearDebuffs()
{
    weak_ = 0; frail_ = 0; vulnerable_ = 0; m_strength_ = 0; m_dexterity_ = 0; eotStrength = 0; eotDexterity = 0;
    addAltBuffs(Intents("eotSTR", 0, 1));
    addAltBuffs(Intents("eotDEX", 0, 1));
    clearEntangle();
    clearConstricted();
    removeConfused();
    removeHexed();
    while (draw_reduction_ > 0) downtickDrawReduction();
    while (no_block_ > 0) downtickNoBlock();
    no_draw_ = false;
    addAltBuffs(Intents("NoDraw", 0, 1));
}
void nonpc::clearDebuffs()
{
    weak_ = 0; vulnerable_ = 0; m_strength_ = 0; m_dexterity_ = 0; eotStrength = 0; eotDexterity = 0;
    addAltBuffs(Intents("eotSTR", 0, 1));
    addAltBuffs(Intents("eotDEX", 0, 1));
}
void pc::sotCleanup()
{
    gm.cardCount = 0; gm.attackCount = 0; gm.skillCount = 0; gm.powerCount = 0;
    no_draw_ = false;
    if (no_block_ > 0) { downtickNoBlock(); }
    if (draw_reduction_ > 0) { downtickDrawReduction(); }
    if (weak_ && weak_ > new_weak_) { --weak_; new_weak_ = 0; }
    if (vulnerable_ && vulnerable_ > new_vulnerable_) { --vulnerable_; new_vulnerable_ = 0; }
    if (frail_ && frail_ > new_frail_) { --frail_; new_frail_ = 0; }
    if (entangle_ && entangle_ > new_entangle_) { clearEntangle(); }
    clearFlameBarrier();
    new_weak_ = 0; new_vulnerable_ = 0; new_frail_ = 0; new_entangle_ = 0;
    downtickIntangible();
    gm.callSOTBlock();
    gm.callPlayerTurnStart();
}
void nonpc::loseAllBlock() { block_ = 0; apci.getPlayer().enemyTargetTriggerIndex = scrnIndex; gm.callBlockBroken(); }
bool nonpc::loseHealth(int amount)
{
    if (base_invincible_ > 0 && invincible_ == 0) return false;
    if (base_invincible_ > 0 && amount > invincible_) amount = invincible_;
    int healthTemp{ health_ };
    health_ = clamp(health_ - amount, 0, max_health_);
    if (!ai)
    {
        if (health_ != 0) std::cout << "Health loss of " << amount << " HP for " << getName() << endl;
        else { std::cout << "Health loss of " << healthTemp << " HP for " << getName() << endl; }
    }
    if (base_invincible_ > 0) loseInvincible(amount);
    gm.callEnemyHealthLoss();
    if (health_ == 0)
    {
        if (dormant_)
        {
            gm.addDormantEnemy(*this, dormant_turns_);
        }
        else { gm.callEnemyFatal(*this); }
        gm.callEnemyDied(*this);
        if (gamestate::lastAttackedSCRNIndex == scrnIndex) gamestate::lastAttackedSCRNIndex = -1;
    }
    return health_ == 0;
}
bool pc::loseHealth(int amount)
{
    imminentHPLoss = amount;
    if (buffer_ > 0) { loseBuffer(1); return false; }
    gm.callPlayerAboutToLoseHealth();
    if (imminentHPLoss == 0) return false;
    health_ = clamp(health_ - imminentHPLoss, 0, max_health_);
    if (!ai) if (health_ != 0) std::cout << "Health loss of " << amount << " HP for " << getName() << endl;
    gm.callPlayerHealthLoss();
    if (health_ == 0)
    {
        gm.callPlayerDied();
        if (health_ == 0) throw PlayerDiedError("dead");
    }
    return false;
}
void pc::gainRitual(int amount)
{
    ritual_ += amount;
    observeEvent<ritualTrigger<character>>(gm.getPlayerTurnEnd(), new ritualTrigger<character>(this, gm.getPlayerTurnEnd()));
    addAltBuffs(Intents("RIT", ritual_, 1));
}
void character::gainIntangible(int amount)
{
    if (intangible_ == 0) addIntangibleTrigger();
    intangible_ += amount; 
    addAltBuffs(Intents("Intan", intangible_, 1)); 
}
void character::uptickClock()
{
    ++clock12_;
    if (clock12_ == 12) clock12_ = 0;
    addAltBuffs(Intents("Clock", clock12_, 12));
}

void pc::downtickIntangible()
{
    if (intangible_ > 0)
    {
        --intangible_;
        addAltBuffs(Intents("Intan", intangible_, 1));
        if (intangible_ == 0) { removeObserver<intangibleTriggerPC>(); }
    }
}
void nonpc::downtickIntangible()
{
    if (intangible_ > 0)
    {
        --intangible_;
        addAltBuffs(Intents("Intan", intangible_, 1));
        if (intangible_ == 0) { removeObserver<intangibleTriggerNonPC>(); }
    }
}
void character::addBeatOfDeath(int amount)
{
    if (beat_of_death_ > 0) removeObserver<beatOfDeathTrigger>();
    beat_of_death_ += amount;
    addAltBuffs(Intents("BeatOfDeath", beat_of_death_, 1));
    observeEvent<beatOfDeathTrigger>(gm.getCardPlayed(), new beatOfDeathTrigger(gm.getCardPlayed(), beat_of_death_));
}
void character::addInvincible(int amount)
{
    base_invincible_ = amount;
    invincible_ = amount;
    addAltBuffs(Intents("Invincible", base_invincible_, invincible_)); 
}
void character::loseInvincible(int amount)
{
    invincible_ = clamp(invincible_ - amount, 0, base_invincible_);
    addAltBuffs(Intents("Invincible", base_invincible_, invincible_));
}
void pc::startOfFight()
{
    if (sotBlockObs) { delete sotBlockObs; sotBlockObs = nullptr; }
    sotBlockObs = new sotBlockTrigger<character>(this, gm.getSOTBlock());
    gm.getSOTBlock()->addObserver(sotBlockObs);
}
void nonpc::startOfFight()
{
    if (sotBlockObs) { delete sotBlockObs; sotBlockObs = nullptr; }
    sotBlockObs = new sotBlockTrigger<character>(this, gm.getEnemyTurnStart());
    gm.getEnemyTurnStart()->addObserver(sotBlockObs);
    if (isMinion()) addAltBuffs(Intents("Minion", 1000, 1));
}
bool character::useArtifactCharge()
{
    if (artifact_ > 0)
    {
        artifact_ -= 1;
        addAltBuffs(Intents("Artifact", artifact_, 1));
        return true;
    }
    return false;
}
void character::gainArtifactCharge(int i)
{
    artifact_ += i;
    addAltBuffs(Intents("Artifact", artifact_, 1));
}
void pc::addIntangibleTrigger()
{
    observeEvent<intangibleTriggerPC>(gm.getAlterAdjustedADFromEnemy(), new intangibleTriggerPC);
}
void nonpc::addIntangibleTrigger()
{
    observeEvent<intangibleTriggerNonPC>(gm.getAlterAdjustedADFromPlayer(), new intangibleTriggerNonPC);
}
void pc::endFight()
{
    block_ = 0; weak_ = 0; frail_ = 0; vulnerable_ = 0; m_strength_ = 0; m_dexterity_ = 0; entangle_ = 0;
    confused_ = false; hexed_ = false; no_draw_ = false; no_block_ = 0; constricted_ = 0; draw_reduction_ = 0;
    strength_ = 0; dexterity_ = 0; metallicise_ = 0; plated_armour_ = 0; buffer_ = 0; ragee_ = 0; intangible_ = 0; regeneration_ = 0;
    BarricadeBuff = 0; BerserkBuff = 0; BrutalityBuff = 0; CombustBuff = 0; CombustBuffStacks = 0; CorruptionBuff = 0; 
    DarkEmbraceBuff = 0; DemonFormBuff = 0; EvolveBuff = 0; FeelNoPainBuff = 0; FireBreathingBuff = 0; JuggernautBuff = 0;
    RuptureBuff = 0; MagnetismBuff = 0; MayhemBuff = 0; PanacheBuff = 0; PanacheCounter = 0; SadisticBuff = 0;
    DUPPotion = 0; DUPDoubleTap = 0; bomb_damage_turn_.clear();
    eotDexterity = 0; eotStrength = 0; thorns_ = 0; flameBar_ = 0; sharp_hide_ = 0; ritual_ = 0; artifact_ = 0;
    card::canPlayAttack = true;
    altBuffs.clear();
    clearNullObservers();
    if (sotBlockObs) { delete sotBlockObs; sotBlockObs = nullptr; }
    card::corruptionCheck = false;
}

void nonpc::constructIntentions(string pastAttack_, vector<Intents> intents_, vector<Intents> altBuffs_)
{
    pastAttacks.push_back(pastAttack_);
    gm.setIntents(intents_, scrnIndex);
    for (auto aB : altBuffs_) addAltBuffs(aB);
    visuals::addEnemyIntents(intents_, scrnIndex);
}
// takes e.g. ("chomp","bellow","thrash") (45,30,25) (2,2,3) and gives indexed intent to go with
int nonpc::calcIntentOdds(vector<string> intents_, vector<int> percentages, vector<int> maxInARow)
{
    if (percentages.size() != maxInARow.size() || percentages.empty()) throw runtime_error("calc intent bad 1");
    int oneInHundred{ 0 };
    vector<int> cumulative = { percentages[0] };
    for (int j{ 0 }; j < percentages.size(); ++j)
    {
        if (j != 0) cumulative.push_back(percentages[j] + cumulative[j - 1]);
    }
    if (*(cumulative.end() - 1) != 100) throw runtime_error("calc intent bad 2");
    int i{ 0 };
    while (i != percentages.size() && oneInHundred == 0)
    {
        if (checkLastXAttacks(pastAttacks, maxInARow[i], intents_[i]))
        {
            if (i == 0) oneInHundred = random_int(percentages[i] + 1, 100);
            else if (i == percentages.size() - 1) oneInHundred = random_int(1, 100 - percentages[i]);
            else
            {
                int temp = random_int(1, 100 - percentages[i]);
                if (temp > cumulative[i] - percentages[i]) oneInHundred = temp + percentages[i];
                else { oneInHundred = temp; }
            }
        }
        ++i;
    }
    if (oneInHundred == 0) oneInHundred = random_int(1, 100);
    for (int j{ 0 }; j < cumulative.size(); ++j)
    {
        if (oneInHundred <= cumulative[j]) return j;
    }
    throw runtime_error("bad calc intent 3");
}
int nonpc::getCumulativeByIndex(int i)
{
    int res{ 0 };
    for (int j{ 0 }; j < i; ++j)
    {
        res += attsPerEnemy[j];
    }
    return res;
}

int nonpc::getIntentID()
{
    if (pastAttacks.size() == 0) return 0;
    for (int i{ 0 }; i < allAttacks.size(); ++i)
    {
        if (allAttacks[i] == pastAttacks.back()) return getCumulativeByIndex(enemyIDNumber) + i;
    }
    throw runtime_error("attack embed " + name_);
}
int nonpc::getIntent2ID()
{
    if (pastAttacks.size() <= 1) return 0;
    for (int i{ 0 }; i < allAttacks.size(); ++i)
    {
        if (allAttacks[i] == *(pastAttacks.end() - 2)) return getCumulativeByIndex(enemyIDNumber) + i;
    }
    throw runtime_error("attack embed " + name_);
}
int nonpc::getIntent3ID()
{
    if (pastAttacks.size() <= 2) return 0;
    for (int i{ 0 }; i < allAttacks.size(); ++i)
    {
        if (allAttacks[i] == *(pastAttacks.end() - 3)) return getCumulativeByIndex(enemyIDNumber) + i;
    }
    throw runtime_error("attack embed " + name_);
}

pc* pc::saveClone()
{
    // priv pc
    pc* clone = new pc(max_health_);
    clone->playerStartEnergy_ = playerStartEnergy_;
    clone->new_weak_ = new_weak_;
    clone->new_vulnerable_ = new_vulnerable_;
    clone->new_frail_ = new_frail_;
    clone->new_entangle_ = new_entangle_;
    // pub pc
    clone->incAtt = incAtt;
    clone->outAtt = outAtt;
    clone->imminentHPLoss = imminentHPLoss;
    clone->enemyTargetTriggerIndex = enemyTargetTriggerIndex;
    // char
    clone->scrnIndex = scrnIndex;
    clone->health_ = health_;
    clone->max_health_ = max_health_;
    clone->block_ = block_;
    // debuffs
    clone->weak_ = weak_;
    clone->frail_ = frail_;
    clone->vulnerable_ = vulnerable_;
    clone->m_strength_ = m_strength_;
    clone->m_dexterity_ = m_dexterity_;
    clone->entangle_ = entangle_;
    clone->confused_ = confused_;
    clone->hexed_ = hexed_;
    clone->no_draw_ = no_draw_;
    clone->no_block_ = no_block_;
    // buffs
    clone->strength_ = strength_;
    clone->dexterity_ = dexterity_;
    clone->metallicise_ = metallicise_;
    clone->plated_armour_ = plated_armour_;
    clone->buffer_ = buffer_;
    clone->ragee_ = ragee_;
    clone->intangible_ = intangible_;
    clone->regeneration_ = regeneration_;
    clone->bomb_damage_turn_ = bomb_damage_turn_;
    // multis
    clone->weakMulti = weakMulti;
    clone->frailMulti = frailMulti;
    clone->vulnerableMulti = vulnerableMulti;
    // power
    clone->BarricadeBuff = BarricadeBuff;
    clone->BerserkBuff = BerserkBuff;
    clone->BrutalityBuff = BrutalityBuff;
    clone->CombustBuff = CombustBuff;
    clone->CombustBuffStacks = CombustBuffStacks;
    clone->CorruptionBuff = CorruptionBuff;
    clone->DarkEmbraceBuff = DarkEmbraceBuff;
    clone->DemonFormBuff = DemonFormBuff;
    clone->EvolveBuff = EvolveBuff;
    clone->FeelNoPainBuff = FeelNoPainBuff;
    clone->FireBreathingBuff = FireBreathingBuff;
    clone->JuggernautBuff = JuggernautBuff;
    clone->RuptureBuff = RuptureBuff;
    clone->MagnetismBuff = MagnetismBuff;
    clone->MayhemBuff = MayhemBuff;
    clone->PanacheBuff = PanacheBuff;
    clone->PanacheCounter = PanacheCounter;
    clone->SadisticBuff = SadisticBuff;
    clone->DUPPotion = DUPPotion;
    clone->DUPDoubleTap = DUPDoubleTap;
    // alterations
    clone->toDrawSOT = toDrawSOT;
    clone->bottledCount = bottledCount;
    clone->eotStrength = eotStrength;
    clone->eotDexterity = eotDexterity;
    clone->thorns_ = thorns_;
    clone->flameBar_ = flameBar_;
    clone->sharp_hide_ = sharp_hide_;
    clone->ritual_ = ritual_;
    clone->artifact_ = artifact_;

    clone->slowed_ = slowed_;
    clone->draw_reduction_ = draw_reduction_;
    clone->constricted_ = constricted_;

    clone->red_key_ = constricted_;
    clone->blue_key_ = constricted_;
    clone->green_key_ = constricted_;

    clone->altBuffs = altBuffs;
    clone->sotBlockObs = sotBlockObs!=nullptr ? sotBlockObs->clone(clone) : nullptr;
    if (clone->sotBlockObs) 
    {
        clone->sotBlockObs->setInactive(); 
        clone->sotBlockObs->getObserving()->addObserver(clone->sotBlockObs);
    }
    // holdsobs
    vector<pair<observer*,observer*>> lookFor;
    for (auto itr{ observers_.begin() }; itr != observers_.end(); ++itr)
    {
        auto halt = dynamic_cast<haltObservingTriggerCH<character>*>(*itr);
        if (halt != nullptr)
        {
            lookFor.push_back({ halt->getObs(),*itr });
        }
    }
    for (auto itr{ observers_.begin() }; itr != observers_.end(); ++itr)
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
                auto halt = dynamic_cast<haltObservingTriggerCH<character>*>(*itr);
                if (halt != nullptr)
                {
                    halt->setObs(lookFor[i].first);
                }
            }
        }
    }
    
    return clone;
}

// statics
float nonpc::weakMulti__{ 0.75 };
float nonpc::vulnerableMulti__{ 1.5 };
int nonpc::_unique_id_{ 0 };
const vector<int> nonpc::attsPerEnemy{ 3,2,2,2,3,2,2,1,2,3,2,4,3,2,2,3,7,6,1,1,2,1,2,4,3,4,6,4,4,5,4,2,3,3,3,2,3,1,7,4,1,5,3,1,3,3,2,2,2,2,4,3,1,5,3,3,3,2,3,2,2,5,4,3,3,4 }; //194
bool pc::magicFlowerCheck{ false };