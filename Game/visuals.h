#ifndef VISUALS_H
#define VISUALS_H
#include "top.h"

enum class aiu { one, two, three, four, five };
namespace visuals
{
    extern aiu AIU_;
    extern std::string numbers;
    extern std::string vis_five;
    extern std::string vis_four;
    extern std::string vis_three;
    extern std::string vis_two;
    extern std::string vis_one;
    extern std::string visInUse;
    extern int linesOfIntent;
    extern int linesOfIntent0;
    extern int linesOfIntent1;
    extern int linesOfIntent2;
    extern int linesOfIntent3;
    extern int linesOfIntent4;
    extern int WIDTH;
    extern const int WIDTH1;
    extern const int WIDTH3;
    extern const int WIDTH4;
    extern const int WIDTH5;
    extern const int HEIGHT;
    void setvisInUse(aiu AIU);
    void changeTwoDigit(int number, int firstIndex);
    void changeThreeDigit(int number, int firstIndex);
    void editEnergy(int number);
    void editDrawPile(int number);
    void editDiscardPile(int number);
    void editExhausted(int number);
    void editPlayerHealth(int number);
    void editPlayerMaxHealth(int number);
    void editVulnerablePC(int number);
    void editFrailPC(int number);
    void editWeakPC(int number);
    void editStrengthPC(int number);
    void editDexterityPC(int number);
    void editBlockPC(int number);

    void clearEnemyIntents();
    void addEnemyIntents(std::vector<Intents> vIntents, int index, bool goRegardless = false);
    void addEnemyIntents(std::deque<std::vector<Intents>> vIntents, bool goRegardless = false);
    void intentsWriting(int& first, int& i, int& j, int& k,std::vector<Intents>&);
    void clearEnemyAltBuffs();
    void clearPlayerAltBuffs();
    void addEnemyAltBuffs(std::vector<Intents>& vBuffs);
    void addPCAltBuffs(std::vector<Intents>& vBuffs);
    void update();
    void editCardX(const cardHolder& c, int index);
    void editCardEmpty(int index);
    void editAllHand(cardDeckCH& hand);
    void clearRelics();
    void editRelics(std::vector<relic*>& relics);
    void clearPotions();
    void editPotions(std::vector<potion*>& potions, int maxPots);

    void clearEnemyIntents2(int);
    void clearEnemyAltBuffs2();
    void addEnemyAltBuffs(std::vector<Intents>& vBuffs, std::vector<Intents>& vBuffs2);

    void clearEnemyIntents3();
    void clearEnemyAltBuffs3();
    void addEnemyAltBuffs(std::vector<Intents>& vBuffs, std::vector<Intents>& vBuffs2, std::vector<Intents>& vBuffs3);

    void clearEnemyIntents4();
    void clearEnemyAltBuffs4();
    void addEnemyAltBuffs(std::vector<Intents>& vBuffs, std::vector<Intents>& vBuffs2, std::vector<Intents>& vBuffs3, std::vector<Intents>& vBuffs4);

    void clearEnemyIntents5();
    void clearEnemyAltBuffs5();
    void addEnemyAltBuffs(std::vector<Intents>& vBuffs, std::vector<Intents>& vBuffs2, std::vector<Intents>& vBuffs3, std::vector<Intents>& vBuffs4, std::vector<Intents>& vBuffs5);
    
    void processIntPM(int n, int first);
    void processInt(int n, int first);
    void processIntHP(int n, int first);
    void processIntMHP(int n, int first);
    void processString(std::string name, int first);

    template<typename T>
    void editEnemyHealth(T arg, bool onlyOne = false);
    template<typename T, typename... Args>
    void editEnemyHealth(T arg, Args... args);

    template<typename T>
    void editEnemyMaxHealth(T arg, bool onlyOne = false);
    template<typename T, typename... Args>
    void editEnemyMaxHealth(T arg, Args... args);

    template<typename T>
    void editVulnerableEnemy(T arg, bool onlyOne = false);
    template<typename T, typename... Args>
    void editVulnerableEnemy(T arg, Args... args);

    template<typename T>
    void editWeakEnemy(T arg, bool onlyOne = false);
    template<typename T, typename... Args>
    void editWeakEnemy(T arg, Args... args);

    template<typename T>
    void editStrengthEnemy(T arg, bool onlyOne = false);
    template<typename T, typename... Args>
    void editStrengthEnemy(T arg, Args... args);

    template<typename T>
    void editDexterityEnemy(T arg, bool onlyOne = false);
    template<typename T, typename... Args>
    void editDexterityEnemy(T arg, Args... args);

    template<typename T>
    void editBlockEnemy(T arg, bool onlyOne = false);
    template<typename T, typename... Args>
    void editBlockEnemy(T arg, Args... args);
    
    template<typename T>
    void editEnemyNames(T arg, bool onlyOne = false);
    template<typename T, typename... Args>
    void editEnemyNames(T arg, Args... args);

    template<typename T>
    void editEnemyHealth(T arg, bool onlyOne)
    {
        int first{ WIDTH * 13 + 61 };
        if (!onlyOne) first = WIDTH * 13 + 48;
        processIntHP(arg, first);
    }
    template<typename T, typename... Args>
    void editEnemyHealth(T arg, Args... args)
    {
        int first{ WIDTH * 13 };
        switch (sizeof...(args))
        {
        case 4:
            first += 156; break;
        case 3:
            first += 129; break;
        case 2:
            first += 102; break;
        case 1:
            first += 77; break;
        }
        processIntHP(arg, first);
        editEnemyHealth(args...);
    }
    template<typename T>
    void editEnemyMaxHealth(T arg, bool onlyOne)
    {
        int first{ WIDTH * 13 + 64 };
        if (!onlyOne) first = WIDTH * 13 + 51;
        processIntMHP(arg, first);
    }
    template<typename T, typename... Args>
    void editEnemyMaxHealth(T arg, Args... args)
    {
        int first{ WIDTH * 13 };
        switch (sizeof...(args))
        {
        case 4:
            first += 159; break;
        case 3:
            first += 132; break;
        case 2:
            first += 105; break;
        case 1:
            first += 80; break;
        }
        processIntMHP(arg, first);
        editEnemyMaxHealth(args...);
    }
    template<typename T>
    void editVulnerableEnemy(T arg, bool onlyOne)
    {
        int first{ WIDTH * 15 + 53 };
        if (!onlyOne) first = WIDTH * 15 + 44;
        processInt(arg, first);
    }
    template<typename T, typename... Args>
    void editVulnerableEnemy(T arg, Args... args)
    {
        int first{ WIDTH * 15 };
        switch (sizeof...(args))
        {
        case 4:
            first += 152; break;
        case 3:
            first += 125; break;
        case 2:
            first += 98; break;
        case 1:
            first += 71; break;
        }
        processInt(arg, first);
        editVulnerableEnemy(args...);
    }
    template<typename T>
    void editWeakEnemy(T arg, bool onlyOne)
    {
        int first{ WIDTH * 15 + 58 };
        if (!onlyOne) first = WIDTH * 15 + 49;
        processInt(arg, first);
    }
    template<typename T, typename... Args>
    void editWeakEnemy(T arg, Args... args)
    {
        int first{ WIDTH * 15 };
        switch (sizeof...(args))
        {
        case 4:
            first += 157; break;
        case 3:
            first += 130; break;
        case 2:
            first += 103; break;
        case 1:
            first += 76; break;
        }
        processInt(arg, first);
        editWeakEnemy(args...);
    }
    template<typename T>
    void editStrengthEnemy(T arg, bool onlyOne)
    {
        int first{ WIDTH * 15 + 64 };
        if (!onlyOne) first = WIDTH * 15 + 55;
        processIntPM(arg, first);
    }
    template<typename T, typename... Args>
    void editStrengthEnemy(T arg, Args... args)
    {
        int first{ WIDTH * 15 };
        switch (sizeof...(args))
        {
        case 4:
            first += 163; break;
        case 3:
            first += 136; break;
        case 2:
            first += 109; break;
        case 1:
            first += 82; break;
        }
        processIntPM(arg, first);
        editStrengthEnemy(args...);
    }
    template<typename T>
    void editDexterityEnemy(T arg, bool onlyOne)
    {
        int first{ WIDTH * 15 + 70 };
        if (!onlyOne) first = WIDTH * 15 + 61;
        processIntPM(arg, first);
    }
    template<typename T, typename... Args>
    void editDexterityEnemy(T arg, Args... args)
    {
        int first{ WIDTH * 15 };
        switch (sizeof...(args))
        {
        case 4:
            first += 169; break;
        case 3:
            first += 142; break;
        case 2:
            first += 115; break;
        case 1:
            first += 88; break;
        }
        processIntPM(arg, first);
        editDexterityEnemy(args...);
    }
    template<typename T>
    void editBlockEnemy(T arg, bool onlyOne)
    {
        int first{ WIDTH * 13 + 69 };
        if (!onlyOne) first = WIDTH * 13 + 56;
        processInt(arg, first);
    }
    template<typename T, typename... Args>
    void editBlockEnemy(T arg, Args... args)
    {
        int first{ WIDTH * 13 };
        switch (sizeof...(args))
        {
        case 4:
            first += 164; break;
        case 3:
            first += 137; break;
        case 2:
            first += 110; break;
        case 1:
            first += 85; break;
        }
        processInt(arg, first);
        editBlockEnemy(args...);
    }

    template<typename T>
    void editEnemyNames(T arg, bool onlyOne)
    {
        int first{ WIDTH * 12 + 57 };
        if (!onlyOne) first = WIDTH * 12 + 46;
        processString(arg, first);
    }
    template<typename T, typename... Args>
    void editEnemyNames(T arg, Args... args)
    {
        int first{ WIDTH * 12 };
        switch (sizeof...(args))
        {
        case 4:
            first += 157; break;
        case 3:
            first += 130; break;
        case 2:
            first += 103; break;
        case 1:
            first += 75; break;
        }
        processString(arg, first);
        editEnemyNames(args...);
    }

}
#endif
