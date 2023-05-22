#ifndef EVENTS_H
#define EVENTS_H
#include "top.h"

typedef std::vector<std::deque<std::unique_ptr<nonpc>>> fights;
typedef std::deque<std::unique_ptr<nonpc>> fight;
class events
{
    public:
        virtual void startEvent()=0;
        virtual ~events(){}
        static int getRandomIndex(std::vector<float>);
        virtual void placeRandomForNext(int) {}
        virtual std::vector<std::string> namesForFight(int = -1) const { throw std::runtime_error("don't use this"); }
};
class easyHallway: public events
{
    friend gamestate;
    private:
        std::vector<std::vector<std::string>> hallwayOptions;
        std::vector<float> pFight;
        int lastFight{-1};
    public:
        int selectRandom() {}
        void startEvent();
        void placeRandomForNext(int act_);
        std::vector<std::string> namesForFight(int) const { return hallwayOptions.back(); }
        void setPFight(std::vector<float> p) { pFight = p; lastFight = -1; }
        easyHallway(){}
        ~easyHallway(){}
};
class hardHallway: public events
{
    friend gamestate;
    private:
        std::vector<std::vector<std::string>> hallwayOptions;
        std::vector<float> pFight;
        int lastFight{-1};
    public:
        int selectRandom() {}
        void startEvent();
        void placeRandomForNext(int act_);
        std::vector<std::string> namesForFight(int) const { return hallwayOptions.back(); }
        void setPFight(std::vector<float> p) { pFight = p; lastFight = -1; }
        hardHallway(){}
        ~hardHallway(){}
};
class elites: public events
{
    friend gamestate;
    private:
        std::vector<std::vector<std::string>> eliteOptions;
        int lastElite{-1};
    public:
        int selectRandom() {}
        void startEvent();
        void placeRandomForNext(int act_);
        std::vector<std::string> namesForFight(int) const { return eliteOptions.back(); }
        elites() { lastElite = -1; }
        ~elites(){}

};
class qSpecial: public events
{
    friend gamestate;
    private:
        std::vector<std::vector<std::string>> specialOptions;
    public:
        int selectRandom() {}
        void startEvent();
        void placeRandomForNext(int act_);
        std::vector<std::string> namesForFight(int) const { return specialOptions.back(); }
        qSpecial() {}
        ~qSpecial(){}

};
class bosses: public events
{
    friend gamestate;
    private:
        std::vector<std::vector<std::string>> bossOptions;
    public:
        int selectRandom();
        void startEvent();
        void placeRandomForNext(int act_);
        std::string getBossName(int index) const { return bossOptions[index].back(); }
        std::vector<std::string> namesForFight(int index) const {
            if (index == -1) return bossOptions[random_int(0, static_cast<int>(bossOptions.size()) - 1)];
            else { return bossOptions[index]; }
        }
        bosses(){}
        ~bosses(){}

};
class peaceEvents : public events
{
    public:
        void prelude();
        virtual ~peaceEvents(){}
};
struct questionChoices 
{
    std::string (*textFunction)();
    void (*eventOutcome)();
    bool (*requirementsCheck)();
    
    questionChoices(std::string (*s)(), void (*a)(), bool (*b)()): textFunction(s),eventOutcome(a),requirementsCheck(b){}
};
class questionMark : public peaceEvents
{
    friend gamestate;
    friend questionChoices;
    protected:
        std::string intro;
        bool shrine_{false};
        int act_{ 0 };
        std::function<void()> aiDecisionMaker;
        std::function<bool()> enterRequirement;
        std::vector<questionChoices> questionOptions;
    public:
        static int storedValue1, storedValue2, storedValue3, storedValue4;
        void startEvent();
        bool getShrine() const { return shrine_; }
        int getAct() const { return act_; }
        static bool noReqs() { return true; };
        questionMark(int a, std::string intro_, std::vector<questionChoices> vqc, std::function<void()> aiD, bool shrine = false) :
            act_(a), intro(intro_), shrine_(shrine), questionOptions(vqc), aiDecisionMaker(aiD) {enterRequirement = &questionMark::noReqs;}

        questionMark(int a, std::string intro_, std::function<bool()> eReqs, std::vector<questionChoices> vqc, std::function<void()> aiD, bool shrine = false) :
            act_(a), intro(intro_), shrine_(shrine), enterRequirement(eReqs), questionOptions(vqc), aiDecisionMaker(aiD) {}


        questionMark(int a, std::string intro_): act_(a), intro(intro_){}
        ~questionMark(){}
};
struct campfireChoices 
{
    std::function<std::string ()> textFunction;
    std::function<void ()> eventOutcome;
    std::function<bool ()> requirementsCheck;
    
    campfireChoices(std::function<std::string ()> s, std::function<void ()> a, std::function<bool ()> b): textFunction(s),eventOutcome(a),requirementsCheck(b){}
};
class campfire : public peaceEvents
{
    private:
        std::string intro;
        std::vector<campfireChoices> campfireOptions;
    public:
        void addOption(campfireChoices cc){campfireOptions.push_back(cc);}
        campfire();
        void startEvent();
        void aiCampfireDecision(std::vector<bool>);
};
struct shopChoices 
{
    std::string name_;
    std::function<void ()> eventOutcome;
    int goldCost;
    std::string tag_;
    shopChoices(std::string s, std::function<void ()> a, int g, std::string t = ""): name_(s),eventOutcome(a),goldCost(g),tag_(t){}
};
class shop : public peaceEvents
{
    friend gamestate;
    private:
        C_DATABASE::iterator attack1;
        C_DATABASE::iterator attack2;
        C_DATABASE::iterator skill1;
        C_DATABASE::iterator skill2;
        C_DATABASE::iterator power1;

        C_DATABASE::iterator clrU1;
        C_DATABASE::iterator clrR2;

        relic* relic1;
        relic* relic2;
        relic* relic3;

        potion* potion1;
        potion* potion2;
        potion* potion3;

        std::vector<shopChoices> shopOptions;
        std::string intro{"Shop"};
        int fiftyDiscountIndex{ 0 };
        int fiftyDiscountCount{ 0 };
    public:
        static bool membershipDiscount;
        static bool courierDiscount;
        static bool smilingmaskDiscount;
        static int cardRemove;
        void reCalcMC();
        static void reCalcTC(){}
        void assignOptions();
        void startEvent();
        shop();
        ~shop();
        int calcGold(cardRarity cr);
        int calcGoldColourless(cardRarity cr);
        int calcGold(cardRarity cr, bool potion);
        int calcGold(relicRarity rr);
        void reRollPotion(int,std::string);
        void reRollRelic(int,std::string);
        void reRollCard(int, C_DATABASE::iterator&,std::string);
        //void reRollCard(int, cardType, std::string);
        void reRollColourlessCard(int, std::string);
        void handleShopStuff(shopStuff&);
};
class chest: public peaceEvents
{
    private:
        chestSize size_;
    public:
        static bool nlothCheck;
        chest()
        {
            int res = random_int(1,100);
            if      (res>=1 && res<=50) size_ = chestSize::Small;
            else if (res>=51 && res<=83) size_ = chestSize::Medium;
            else    {size_ = chestSize::Large;}
        }
        chest(bool boss_){if (boss_) size_ = chestSize::Boss; else{throw std::runtime_error("why");}}
        void startEvent();
};
class neowEvent : public peaceEvents
{
private:
    int firstRNG{ 0 };
    int secondRNG{ 0 };
    int thirdGoodRNG{ 0 };
    int thirdBadRNG{ 0 };
public:
    neowEvent() {}
    void startEvent();
    void choiceOutcomes(int choice);
    void printOptions();
};
#endif