#ifndef CHARACTER_H
#define CHARACTER_H
#include "top.h"


class character : public holdsObservers
{
protected:
    int scrnIndex{ 0 };
    int health_;
    int max_health_;
    int block_{ 0 };
    // debuffs
    int weak_{ 0 };
    int frail_{ 0 };
    int vulnerable_{ 0 };
    int m_strength_{ 0 };
    int m_dexterity_{ 0 };
    int entangle_{ 0 };
    bool confused_{ false };
    bool hexed_{ false };
    bool no_draw_{ false };
    int no_block_{ 0 };
    // buffs
    int strength_{ 0 };
    int dexterity_{ 0 };
    int metallicise_{ 0 };
    int plated_armour_{ 0 };
    int ragee_{ 0 };
    int buffer_{ 0 };
    int intangible_{ 0 };
    int regeneration_{ 0 };
    int malleable_{ 0 };
    int base_malleable{ 0 };
    std::vector<std::pair<int, int>> bomb_damage_turn_;
    // multis
    float weakMulti{ 0.75f };
    float frailMulti{ 0.6f };
    float vulnerableMulti{ 1.5f };
    // power
    int BarricadeBuff{ 0 };
    int BerserkBuff{ 0 };
    int BrutalityBuff{ 0 };
    int CombustBuff{ 0 };
    int CombustBuffStacks{ 0 };
    int CorruptionBuff{ 0 };
    int DarkEmbraceBuff{ 0 };
    int DemonFormBuff{ 0 };
    int EvolveBuff{ 0 };
    int FeelNoPainBuff{ 0 };
    int FireBreathingBuff{ 0 };
    int JuggernautBuff{ 0 };
    int RuptureBuff{ 0 };
    int MagnetismBuff{ 0 };
    int MayhemBuff{ 0 };
    int PanacheBuff{ 0 };
    int PanacheCounter{ 0 };
    int SadisticBuff{ 0 };
    int DUPPotion{ 0 };
    int DUPDoubleTap{ 0 };
    // alterations
    int toDrawSOT{ 5 };
    int bottledCount{ 0 };
    int eotStrength{ 0 };
    int eotDexterity{ 0 };
    int thorns_{ 0 };
    int flameBar_{ 0 };
    int sharp_hide_{ 0 };
    int ritual_{ 0 };
    int artifact_{ 0 };
    int enrage_{ 0 };
    int slowed_{ 0 };               
    bool dormant_{ 0 };      
    int draw_reduction_{ 0 }; 
    int clock12_{ 0 };       
    int curiosity_{ 0 };  
    int invincible_{ 0 };   
    int base_invincible_{ 0 };
    int beat_of_death_{ 0 }; 
    bool back_attack_{ 0 }; 
    std::vector<Intents> altBuffs;
    observer* sotBlockObs{ nullptr };
public:
    character(int health) : health_(health), max_health_(health) {}
    virtual ~character() {
        if (sotBlockObs) {
            if (sotBlockObs->getObserving()) sotBlockObs->getObserving()->removeObserver(sotBlockObs);
            delete sotBlockObs;
        }
    }
    // SOT = Start Of Fight
    virtual void startOfFight() {}
    void increaseToDrawSOTPerma(int i) { toDrawSOT += i; }
    void decreaseToDrawSOTPerma(int i) { toDrawSOT -= i; }
    void changeSOTBlocksObs(observer*);
    observer* getSOTBlocks() const { return sotBlockObs; }

    virtual void sotCleanup() = 0;
    virtual void eotCleanup() = 0;
    void incrementBottledCount() { ++bottledCount; }

    // health, damage and block
    virtual bool takeDamage(int amount, character& initiator);
    virtual bool takeDamageNoTrigger(int amount);
    virtual bool loseHealth(int amount) = 0;
    virtual void heal(int amount);
    void healPercentage(int pc);
    void healToFull();
    void gainMaxHP(int amount);
    void loseMaxHP(int amount);
    void losePercentageMaxHP(int amount);
    void eventLoseHP(int amount);
    void eventLosePercentageHP(int pc);
    virtual void gainBlock(int amount);
    virtual void loseAllBlock() { block_ = 0; }
    void setBlockZero() { block_ = 0; }

    // gets
    int getSCRNIndex() const { return scrnIndex; }
    virtual std::string getName() const { return "Ironclad"; }
    virtual float getWeakMulti() const { return weakMulti; }
    virtual bool isMinion() const { return false; }
    float getFrailMulti() const { return frailMulti; }
    virtual float getVulnerableMulti() const { return vulnerableMulti; }
    void setVulnerableMulti(float i) { vulnerableMulti = i; }
    int getWeak() const { return weak_; }
    int getFrail() const { return frail_; }
    int getEntangle() const { return entangle_; }
    int getVulnerable() const { return vulnerable_; }
    int getTotalStrength() const { return strength_ - m_strength_; }
    int getTotalDexterity() const { return dexterity_ - m_dexterity_; }
    bool getNoDraw() const { return no_draw_; }
    int getNoBlock() const { return no_block_; }
    int getHealth() const { return health_; }
    int getMaxHealth() const { return max_health_; }
    float getPercentageHealth() const { return static_cast<float>(health_) / static_cast<float>(max_health_); }
    int getBlock() const { return block_; }
    int getDrawSOT() { int temp = toDrawSOT - bottledCount; bottledCount = 0; return temp; }
    int getEotStrength() const { return eotStrength; }
    int getEotDexterity() const { return eotDexterity; }
    int getThorns() const { return thorns_; }
    int getFlameBar() const { return flameBar_; }
    std::vector<std::pair<int,int>>& getBomb() { return bomb_damage_turn_; }
    int getSharpHide() const { return sharp_hide_; }
    int getRitual() const { return ritual_; }
    int getArtifact() const { return artifact_; }
    int getIntangible() const { return intangible_; }
    int getPlatedArmour() const { return plated_armour_; }
    int getBuffer() const { return buffer_; }
    int getRage() const { return ragee_; }
    int getRegeneration() const { return regeneration_; }
    int getEnrage() const { return enrage_; }
    int getSlowed() const { return slowed_; }
    int getDrawReduction() const { return draw_reduction_; }
    int getClock12() const { return clock12_; }
    int getCuriosity() const { return curiosity_; }
    int getInvincible() const { return invincible_; }
    int getBeatOfDeath() const { return invincible_; }
    bool getBackAttack() const { return back_attack_; }
    bool getDormant() const { return dormant_; }
    std::vector<Intents>& getAltBuffs() { return altBuffs; }

    // buffs/debuffs
    virtual void applyVulnerable(int amount);
    virtual void applyWeak(int amount);
    virtual void applyFrail(int) {}
    virtual void applyEntangle(int) {}

    void applyHexed();
    void removeHexed();
    void applyConfused();
    void removeConfused();
    void applyDrawReduction(int);
    void downtickDrawReduction();

    void gainArtifactCharge(int);
    bool useArtifactCharge();
    void gainIntangible(int amount);
    virtual void addIntangibleTrigger() = 0;
    virtual void downtickIntangible() = 0;
    void applyBomb(int, int);
    void resetBombBuffs();
    void gainMalleable();
    void sofMalleable(int);

    void gainStrength(int);
    void loseStrength(int);
    void changeEotStrength(int amount);
    void gainDexterity(int);
    void loseDexterity(int);
    void changeEotDexterity(int amount);

    void uptickClock();
    void addInvincible(int);
    void loseInvincible(int);
    void addBeatOfDeath(int);

    void noDrawDebuff();
    void noBlockDebuff(int);
    void downtickNoBlock();

    void addAltBuffs(Intents);
    virtual void clearDebuffs() = 0;
    void setBlock(int amount) { block_ = amount; }
    void setHealth(int amount) { health_ = amount; }
    void setMaxHealth(int amount) { max_health_ = amount; }
    void setHealthPercentage(int percent) { health_ = health_ * percent / 100; }
    void setBackAttack(bool ba) { back_attack_ = ba; addAltBuffs(Intents("Back", (ba ? 50 : 0), 1)); }

    void addThorns(int amount) { thorns_ += amount; }
    void addFlameBar(int amount);    
    void addPlatedArmour(int amount) { plated_armour_ += amount; addAltBuffs(Intents("PLTD ARM", plated_armour_, 1)); }
    void losePlatedArmour(int amount) { plated_armour_ -= amount; addAltBuffs(Intents("PLTD ARM", plated_armour_, 1)); }
    void addBuffer(int amount) { buffer_ += amount; addAltBuffs(Intents("BUFF", buffer_, 1)); }
    void loseBuffer(int amount) { buffer_ -= amount; addAltBuffs(Intents("BUFF", buffer_, 1)); }
    void addRage(int amount) { ragee_ += amount; addAltBuffs(Intents("RAGE", ragee_, 1)); }
    void addRegeneration(int amount) { regeneration_ += amount; addAltBuffs(Intents("REGEN", regeneration_, 1)); }
    void loseRegeneration(int amount) { regeneration_ -= amount; addAltBuffs(Intents("REGEN", regeneration_, 1)); }
    void addSlowed(int amount) { slowed_ += amount; addAltBuffs(Intents("Slowed", slowed_, 1)); }

    // duplication
    void addDUPPotion(int amount);
    void addDUPDoubleTap(int amount);
    void loseDUPPotion(int amount);
    void loseDUPDoubleTap(int amount, bool eot = false);
    int& getDUPPotion() { return DUPPotion; }
    int& getDUPDoubleTap() { return DUPDoubleTap; }

    // powers
    void addBARbuff(int amount);
    void addBSKbuff(int amount);
    void addBRTbuff(int amount);
    void addCMBbuff(int amount);
    void addCORbuff(int amount);
    void addDRKbuff(int amount);
    void addDMNbuff(int amount);
    void addEVObuff(int amount);
    void addFNPbuff(int amount);
    void addFRBbuff(int amount);
    void addJGRbuff(int amount);
    void addRUPbuff(int amount);
    void addMAGbuff(int amount);
    void addMAYbuff(int amount);
    void addPANbuff(int amount);
    void addSADbuff(int amount);
    
    void addMTL(int amount) { metallicise_ = std::clamp(metallicise_ + amount, 0, 99); }

    int getBarricadeBuff() const { return BarricadeBuff; }
    int getBerserkBuff() const { return BerserkBuff; }
    int getBrutalityBuff() const { return BrutalityBuff; }
    int getCombustBuff() const { return CombustBuff; }
    int getCorruptionBuff() const { return CorruptionBuff; }
    int getDarkEmbraceBuff() const { return DarkEmbraceBuff; }
    int getDemonFormBuff() const { return DemonFormBuff; }
    int getEvolveBuff() const { return EvolveBuff; }
    int getFeelNoPainBuff() const { return FeelNoPainBuff; }
    int getMetalliciseBuff() const { return metallicise_; }
    int getFireBreathingBuff() const { return FireBreathingBuff; }
    int getJuggernautBuff() const { return JuggernautBuff; }
    int getRuptureBuff() const { return RuptureBuff; }
    int getMagnetismBuff() const { return MagnetismBuff; }
    int getMayhemBuff() const { return MayhemBuff; }
    int getPanacheBuff() const { return PanacheBuff; }
    int& getPanacheCounter() { return PanacheCounter; }
    int getSadisticBuff() const { return SadisticBuff; }
    friend stateInfo;
    friend macroStateInfo;
    
};

// player character, owned by all_pc_info
class pc : public character
{
private:
    int playerStartEnergy_{ 3 };
    int new_weak_{ 0 };
    int new_vulnerable_{ 0 };
    int new_frail_{ 0 };
    int new_entangle_{ 0 };
    int constricted_{ 0 };      
    bool red_key_{ false };  
    bool blue_key_{ false }; 
    bool green_key_{ false };
public:
    pc(int health) :character(health) { scrnIndex = -1; }
    int getEnergy() { return playerStartEnergy_; }
    void incrementStartEnergy() { ++playerStartEnergy_; }
    void decrementStartEnergy() { --playerStartEnergy_; }
    void endFight();
    void startOfFight();
    void sotCleanup();
    void eotCleanup();

    void startOfTurnBlock() {}
    void gainBlock(int amount);
    void heal(int amount);
    bool loseHealth(int amount);

    bool getRedKey() const { return red_key_; }
    bool getBlueKey() const { return blue_key_; }
    bool getGreenKey() const { return green_key_; }

    void applyVulnerable(int amount);
    void applyWeak(int amount);
    void applyFrail(int amount);
    void applyEntangle(int amount);
    void zeroWeak() { weak_ = 0; }
    void zeroFrail() { frail_ = 0; }

    void clearEntangle();
    void clearFlameBarrier();

    void gainRitual(int amount);
    void addIntangibleTrigger();
    void downtickIntangible();
    void clearDebuffs();

    int getConstricted() const { return constricted_; }
    void applyConstricted(int );
    void clearConstricted();

    pc* saveClone();

    int incAtt{0};
    int outAtt{0};
    int imminentHPLoss{0};
    int enemyTargetTriggerIndex{ 0 };
    static bool magicFlowerCheck;

    friend stateInfo;
    friend macroStateInfo;
};

// all enemy characters
class nonpc : public character
{
protected:
    std::string name_{""};
    static int _unique_id_;
    int _id_{0};
    std::vector<std::string> pastAttacks;
    std::vector<std::string> allAttacks;
    const static std::vector<int> attsPerEnemy; //194
    int enemyIDNumber{0};
    int dormant_turns_{ 0 };
    bool painful_stabs_{ false }; 
public:
    static float weakMulti__;
    static float vulnerableMulti__;
    nonpc(int health, std::string name) : character(health), name_(name) { _id_ = _unique_id_++; }
    nonpc(int lowerHealth, int upperHealth, std::string name) : character(random_int(lowerHealth, upperHealth)), name_(name) { _id_ = _unique_id_++; }
    std::string getName() const { return name_; }
    int getUniqueID() const { return _id_; }
    void sotCleanup();
    void eotCleanup();
    void setSCRNindex(int i) { scrnIndex = i; }

    virtual void startOfFight();
    virtual void chooseAndDeclareIntent() = 0;
    virtual void EnactIntent() = 0;
    
    // buffs debuffs alterations
    bool loseHealth(int amount);
    void loseAllBlock();
    void clearDebuffs();
    void applyVulnerable(int amount);
    void applyWeak(int amount);
    float getWeakMulti() const { return weakMulti__; }
    float getVulnerableMulti() const { return vulnerableMulti__; }
    bool getPainfulStabs() const { return painful_stabs_; }
    int getDormantTurns() const { return dormant_turns_; }
    
    void addIntangibleTrigger();
    void downtickIntangible();
    virtual void returnFromDormant() {}
    
    // intentions and past attacks
    void constructIntentions(std::string, std::vector<Intents>, std::vector<Intents> = {});
    int calcIntentOdds(std::vector<std::string>, std::vector<int>, std::vector<int>);
    
    virtual std::vector<int> giveIntent() { return { 0,0,0 }; }
    int getIntentID();
    int getIntent2ID();
    int getIntent3ID();
    int getCumulativeByIndex(int i);
    std::vector<std::string> getPastAttacks() const { return pastAttacks; }
    void addPastAttack(std::string s) { pastAttacks.push_back(s); }

    // clone
    virtual std::unique_ptr<nonpc> clone() const = 0;
    virtual nonpc* saveClone() = 0;
    template<typename T>
    friend T* saveCloneFromOrig(T*, T*);

    virtual ~nonpc() {}
};



#endif