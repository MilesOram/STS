#ifndef CARD_H
#define CARD_H
#include "top.h"

// base class from which all cards are derived
class card : public holdsObservers
{
protected:
    cardType type_{ cardType::Status };
    cardRarity rarity_{ cardRarity::Other };
    int energy_{1};
    int temp_energy_{1};
    std::string name_{""};
    std::string sixCharName_{"      "};
    bool innate_{false};
    bool upgraded_{false};
    bool ethereal_{false};
    bool exhaust_{false};
    bool zeroUntilPlayed{false};
    bool oneUntilPlayed{false};
    bool bottled_{false};
public:
    card(cardType type, cardRarity rarity, int energy, std::string name, std::string scn, bool innate, bool upgraded):
                        type_(type),rarity_(rarity),energy_(energy),temp_energy_(energy),name_(name),sixCharName_(scn),
                        innate_(innate),upgraded_(upgraded),ethereal_(false),exhaust_(false)
                        {if(sixCharName_.size()!=6) throw std::runtime_error("not 6");}
    card(cardType type, cardRarity rarity, int energy, std::string name, std::string scn, bool innate, bool upgraded, bool ethereal, bool exhaust):
                        type_(type),rarity_(rarity),energy_(energy),temp_energy_(energy),name_(name),sixCharName_(scn),
                        innate_(innate),upgraded_(upgraded),ethereal_(ethereal),exhaust_(exhaust)
                        {if(sixCharName_.size()!=6) throw std::runtime_error("not 6");}
    virtual ~card(){};
    virtual void execute() = 0;
    virtual void upgradeThis() = 0;
    virtual void upgradeStatus(){};
    virtual int getCardIDNum() const = 0;
    card* safeClone();
    void clonedObservers(observer_set);
    virtual card* clone() const = 0;
    virtual bool castable();
    virtual bool endStepDiscard(){return false;}
    virtual bool endStepDiscardPrio(){return false;}
    virtual void cardDrawn(){}
    virtual void fightStart(bool midFight = false);
    virtual combatChoices getCombatChoices() const { return combatChoices::None; }
    virtual std::unique_ptr<card> cloneU() const = 0;
    
    // gets
    std::string getSixChar() const {if(sixCharName_.size() !=6) throw std::runtime_error("not 6"); return sixCharName_;}
    std::string getName() const {return name_;}
    cardType getType() const { return type_; }
    virtual cardClass getClass() const { return cardClass::IC; }
    bool getZeroUntilPlayed() const { return zeroUntilPlayed; }
    bool getOneUntilPlayed() const {return oneUntilPlayed;}
    cardRarity getRarity() const {return rarity_;}
    virtual int getEnergy() const;
    virtual void incrementEnergy(){}
    int getBaseEnergy() const { return temp_energy_; }
    void resetEnergy() {temp_energy_=energy_;}
    bool getInnate() const {return innate_;}
    void giveBottled() { bottled_ = true; }
    bool getBottled() const {return bottled_;}
    bool getUpgraded() const {return upgraded_;}
    bool getEthereal() const { return ethereal_; }
    virtual bool getExhaust() const {if(corruptionCheck && type_==Skill) return true;
                                    return exhaust_;}
    // energy change
    void changeTempEnergy(int nrg) { if (temp_energy_ == -1) return; temp_energy_ = nrg; }
    void setZeroUntilPlayed(bool b) { zeroUntilPlayed = b; }
    void setOneUntilPlayed(bool b) { oneUntilPlayed = b; }
    
    static bool blueCandleCheck;
    static bool medKitCheck;
    static bool canPlayAttack;
    static bool corruptionCheck;

    static std::map<int, int> tierList;
};
#endif