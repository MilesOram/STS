#include "visuals.h"
#include "relic.h"
#include "card.h"
#include "potions.h"

using namespace std;

namespace visuals
{
    aiu AIU_;
    std::string numbers = R"(0123456789)";
    std::string vis_five = R"(
________________________________________________________________________________________________________________________________________________________________________________________
|                Potions:                                                                                                                                                              |
|  Draw:xx                                                                                                                                                                             |
|  Discard:xx    Relics:                                                                                                                                                               |
|  Exhausted:xx                                                                                                                                                                        |
|  Energy:0                                                                                                                                                                            |
|                                                                                                         /\                                                  __                       |
|          _________                            ____                           /\                        /  \                    __    __                  __|  |__                    |
|          |       |                           /    \                         /  \                      /    \                   \ \__/ /                 |__    __|                   |
|          |       |                          /      \                       /    \                     \    /                    \    /                     |  |                      |
|          |       |                          \      /                      /      \                     \  /                     / __ \                     |  |                      |
|          |_______|                           \____/                      /________\                     \/                     /_/  \_\                    |__|                      |
|                                                                                                                                                                                      |
|            xx/xx  [xxx]                      xx/xx  [xx]                  xx/xx  [xx]              xx/xx  [xx]                xx/xx  [xx]                xx/xx  [xx]                 |
|                                                                                                                                                                                      |
|   v:xx f:xx w:xx s:+xx d:+xx           v:xx w:xx s:+xx d:+xx      v:xx w:xx s:+xx d:+xx      v:xx w:xx s:+xx d:+xx      v:xx w:xx s:+xx d:+xx      v:xx w:xx s:+xx d:+xx             |
|                                                                                                                                                                                      |
|                                                                                                                                                                                      |
|                                                                                                                                                                                      |
|                                                                                                                                                                                      |
|    (1)       (2)       (3)       (4)       (5)       (6)       (7)       (8)       (9)       (0)                                                                                     |
|  xxxxxx:0  xxxxxx:0  xxxxxx:0  xxxxxx:0  xxxxxx:0  xxxxxx:0  xxxxxx:0  xxxxxx:0  xxxxxx:0  xxxxxx:0                                                                                  |
|______________________________________________________________________________________________________________________________________________________________________________________|
)";
    std::string vis_four = R"(
_____________________________________________________________________________________________________________________________________________________________
|                Potions:                                                                                                                                   |
|  Draw:xx                                                                                                                                                  |
|  Discard:xx    Relics:                                                                                                                                    |
|  Exhausted:xx                                                                                                                                             |
|  Energy:0                                                                                                                                                 |
|                                                                                                         /\                                                |
|          _________                            ____                           /\                        /  \                    __    __                   |
|          |       |                           /    \                         /  \                      /    \                   \ \__/ /                   |
|          |       |                          /      \                       /    \                     \    /                    \    /                    |
|          |       |                          \      /                      /      \                     \  /                     / __ \                    |
|          |_______|                           \____/                      /________\                     \/                     /_/  \_\                   |
|                                                                                                                                                           |
|            xx/xx  [xxx]                      xx/xx  [xx]                  xx/xx  [xx]              xx/xx  [xx]                xx/xx  [xx]                 |
|                                                                                                                                                           |
|   v:xx f:xx w:xx s:+xx d:+xx           v:xx w:xx s:+xx d:+xx      v:xx w:xx s:+xx d:+xx      v:xx w:xx s:+xx d:+xx      v:xx w:xx s:+xx d:+xx             |
|                                                                                                                                                           |
|                                                                                                                                                           |
|                                                                                                                                                           |
|                                                                                                                                                           |
|    (1)       (2)       (3)       (4)       (5)       (6)       (7)       (8)       (9)       (0)                                                          |
|  xxxxxx:0  xxxxxx:0  xxxxxx:0  xxxxxx:0  xxxxxx:0  xxxxxx:0  xxxxxx:0  xxxxxx:0  xxxxxx:0  xxxxxx:0                                                       |
|___________________________________________________________________________________________________________________________________________________________|
)";
    std::string vis_three = R"(
___________________________________________________________________________________________________________________________________
|                Potions:                                                                                                         |
|  Draw:xx                                                                                                                        |
|  Discard:xx    Relics:                                                                                                          |
|  Exhausted:xx                                                                                                                   |
|  Energy:0                                                                                                                       |
|                                                                                                         /\                      |
|          _________                            ____                           /\                        /  \                     |
|          |       |                           /    \                         /  \                      /    \                    |
|          |       |                          /      \                       /    \                     \    /                    |
|          |       |                          \      /                      /      \                     \  /                     |
|          |_______|                           \____/                      /________\                     \/                      |
|                                                                                                                                 |
|            xx/xx  [xxx]                      xx/xx  [xx]                  xx/xx  [xx]              xx/xx  [xx]                  |
|                                                                                                                                 |
|   v:xx f:xx w:xx s:+xx d:+xx           v:xx w:xx s:+xx d:+xx      v:xx w:xx s:+xx d:+xx      v:xx w:xx s:+xx d:+xx              |
|                                                                                                                                 |
|                                                                                                                                 |
|                                                                                                                                 |
|                                                                                                                                 |
|    (1)       (2)       (3)       (4)       (5)       (6)       (7)       (8)       (9)       (0)                                |
|  xxxxxx:0  xxxxxx:0  xxxxxx:0  xxxxxx:0  xxxxxx:0  xxxxxx:0  xxxxxx:0  xxxxxx:0  xxxxxx:0  xxxxxx:0                             |
|_________________________________________________________________________________________________________________________________|
)";
    std::string vis_two = R"(
___________________________________________________________________________________________________________________________________
|                Potions:                                                                                                         |
|  Draw:xx                                                                                                                        |
|  Discard:xx    Relics:                                                                                                          |
|  Exhausted:xx                                                                                                                   |
|  Energy:0                                                                                                                       |
|                                                                                                                                 |
|          _________                            ____                           /\                                                 |
|          |       |                           /    \                         /  \                                                |
|          |       |                          /      \                       /    \                                               |
|          |       |                          \      /                      /      \                                              |
|          |_______|                           \____/                      /________\                                             |
|                                                                                                                                 |
|            xx/xx  [xxx]                      xx/xx  [xx]                  xx/xx  [xx]                                           |
|                                                                                                                                 |
|   v:xx f:xx w:xx s:+xx d:+xx           v:xx w:xx s:+xx d:+xx      v:xx w:xx s:+xx d:+xx                                         |
|                                                                                                                                 |
|                                                                                                                                 |
|                                                                                                                                 |
|                                                                                                                                 |
|    (1)       (2)       (3)       (4)       (5)       (6)       (7)       (8)       (9)       (0)                                |
|  xxxxxx:0  xxxxxx:0  xxxxxx:0  xxxxxx:0  xxxxxx:0  xxxxxx:0  xxxxxx:0  xxxxxx:0  xxxxxx:0  xxxxxx:0                             |
|_________________________________________________________________________________________________________________________________|
)";
    std::string vis_one = R"(
___________________________________________________________________________________________________________________________________
|                Potions:                                                                                                         |
|  Draw:xx                                                                                                                        |
|  Discard:xx    Relics:                                                                                                          |
|  Exhausted:xx                                                                                                                   |
|  Energy:0                                                                                                                       |
|                                                                                                                                 |
|          _________                                        ____                                                                  |
|          |       |                                       /    \                                                                 |
|          |       |                                      /      \                                                                |
|          |       |                                      \      /                                                                |
|          |_______|                                       \____/                                                                 |
|                                                                                                                                 |
|            xx/xx  [xxx]                                   xx/xx  [xx]                                                           |
|                                                                                                                                 |
|   v:xx f:xx w:xx s:+xx d:+xx                    v:xx w:xx s:+xx d:+xx                                                           |
|                                                                                                                                 |
|                                                                                                                                 |
|                                                                                                                                 |
|                                                                                                                                 |
|    (1)       (2)       (3)       (4)       (5)       (6)       (7)       (8)       (9)       (0)                                |
|  xxxxxx:0  xxxxxx:0  xxxxxx:0  xxxxxx:0  xxxxxx:0  xxxxxx:0  xxxxxx:0  xxxxxx:0  xxxxxx:0  xxxxxx:0                             |
|_________________________________________________________________________________________________________________________________|
)";
    string visInUse;
    int linesOfIntent{ 0 };
    int linesOfIntent0{ 0 };
    int linesOfIntent1{ 0 };
    int linesOfIntent2{ 0 };
    int linesOfIntent3{ 0 };
    int linesOfIntent4{ 0 };
    int WIDTH{ 132 }; //107+25
    const int WIDTH1{ 132 };
    const int WIDTH3{ 132 };
    const int WIDTH4{ 158 };
    const int WIDTH5{ 185 };
    const int HEIGHT{ 23 };
    void setvisInUse(aiu AIU)
    {
        AIU_ = AIU;
        switch (AIU_)
        {
        case aiu::one:
            visInUse = vis_one;
            WIDTH = WIDTH1;
            break;
        case aiu::two:
            visInUse = vis_two;
            WIDTH = WIDTH1;
            break;
        case aiu::three:
            visInUse = vis_three;
            WIDTH = WIDTH3;
            break;
        case aiu::four:
            visInUse = vis_four;
            WIDTH = WIDTH4;
            break;
        case aiu::five:
            visInUse = vis_five;
            WIDTH = WIDTH5;
            break;
        }
        linesOfIntent0 = 0; linesOfIntent1 = 0; linesOfIntent2 = 0; linesOfIntent3 = 0; linesOfIntent4 = 0; linesOfIntent = 0;
    }
    void changeTwoDigit(int number, int firstIndex)
    {
        number < 0 ? number *= -1 : number *= 1;
        number = clamp(number, 0, 99);
        int secondIndex = firstIndex + 1;
        if (number < 10) visInUse[firstIndex] = '0';
        else { int i = number / 10; char s = numbers[i]; visInUse[firstIndex] = s; }
        int j = number % 10;
        char k = numbers[j];
        visInUse[secondIndex] = k;
    }
    void changeThreeDigit(int number, int firstIndex)
    {
        int i = number / 100; visInUse[firstIndex] = numbers[i];
        int j = number / 10; visInUse[firstIndex + 1] = numbers[j % 10];
        visInUse[firstIndex + 2] = numbers[number % 10];
    }
    void editEnergy(int number)
    {
        int first{ WIDTH * 5 + 11 };
        changeTwoDigit(number, first);
    }
    void editDrawPile(int number)
    {
        int first{ WIDTH * 2 + 9 };
        changeTwoDigit(number, first);
    }
    void editDiscardPile(int number)
    {
        int first{ WIDTH * 3 + 12 };
        changeTwoDigit(number, first);
    }
    void editExhausted(int number)
    {
        int first{ WIDTH * 4 + 14 };
        changeTwoDigit(number, first);
    }
    void editPlayerHealth(int number)
    {
        int first{ WIDTH * 13 + 14 };
        changeThreeDigit(number, first-1);
    }
    void editPlayerMaxHealth(int number)
    {
        int first{ WIDTH * 13 + 17 };
        changeThreeDigit(number, first);
    }
    void editVulnerablePC(int number)
    {
        int first{ WIDTH * 15 + 7 };
        changeTwoDigit(number, first);
    }
    void editFrailPC(int number)
    {
        int first{ WIDTH * 15 + 12 };
        changeTwoDigit(number, first);
    }
    void editWeakPC(int number)
    {
        int first{ WIDTH * 15 + 17 };
        changeTwoDigit(number, first);
    }
    void editStrengthPC(int number)
    {
        int first{ WIDTH * 15 + 23 };
        if (number < 0)
        {
            visInUse[first - 1] = '-';
            changeTwoDigit(-number, first);
        }
        else
        {
            visInUse[first - 1] = '+';
            changeTwoDigit(number, first);
        }

    }
    void editDexterityPC(int number)
    {
        int first{ WIDTH * 15 + 29 };
        if (number < 0)
        {
            visInUse[first - 1] = '-';
            changeTwoDigit(-number, first);
        }
        else
        {
            visInUse[first - 1] = '+';
            changeTwoDigit(number, first);
        }
    }
    void editBlockPC(int number)
    {
        int first{ WIDTH * 13 + 22 };
        changeThreeDigit(number, first);
    }

    void clearEnemyAltBuffs()
    {
        int first{ WIDTH * 16 + 53 };
        for (int j{ 0 }; j < 40; ++j)
        {
            visInUse[first + j] = ' ';
        }
    }
    void clearPlayerAltBuffs()
    {
        int first{ WIDTH * 16 + 4 };
        for (int j{ 0 }; j < 40; ++j)
        {
            for (int k{ 0 }; k < 3; ++k) visInUse[first + j + k * WIDTH] = ' ';
        }
    }
    void addEnemyAltBuffs(vector<Intents>& vBuffs)
    {
        clearEnemyAltBuffs();
        int first{ WIDTH * 16 + 53 }, j{ 0 }, l{ 0 };
        for (int i{ 0 }; i < vBuffs.size(); ++i)
        {
            if (j > 40) { j = 0; ++l; }
            if (j > 40 && l == 2) throw runtime_error("too many buffs");
            if (vBuffs[i].value == 0 && vBuffs[i].times < 2) continue;
            if (i > 0 && j > 0) { visInUse[first + j + WIDTH * l] = ','; j += 2; }
            if (vBuffs[i].effect.size() + j + 4 > 30) { ++l; j = 0; }
            for (int k{ 0 }; k < vBuffs[i].effect.size(); ++k)
            {
                visInUse[first + j + WIDTH * l] = vBuffs[i].effect[k];
                ++j;
            }
            if (vBuffs[i].value == 1000) { continue; }
            intentsWriting(first, i, j, l, vBuffs);
        }
    }
    void addPCAltBuffs(vector<Intents>& vBuffs)
    {
        clearPlayerAltBuffs();
        int first{ WIDTH * 16 + 4 }, j{ 0 }, l{ 0 };
        for (int i{ 0 }; i < vBuffs.size(); ++i)
        {
            if (j > 40) { j = 0; ++l; }
            if (j > 40 && l == 2) throw runtime_error("too many buffs");
            if (vBuffs[i].value == 0) continue;
            if (i > 0 && j > 0) { visInUse[first + j + WIDTH * l] = ','; j += 2; }
            if (vBuffs[i].effect.size() + j + 5 > 40) { j = 0; ++l; }
            for (int k{ 0 }; k < vBuffs[i].effect.size(); ++k)
            {
                visInUse[first + j + WIDTH * l] = vBuffs[i].effect[k];
                ++j;
            }
            if (vBuffs[i].value == 1000) { j += 0; continue; } // +=2
            visInUse[first + (j++) + WIDTH * l] = ':';
            if (vBuffs[i].value < 0) visInUse[first + (j++) + WIDTH * l] = '-';
            changeTwoDigit(vBuffs[i].value, first + j + WIDTH * l);
            j += 2;
            if (vBuffs[i].effect.size()>4 && vBuffs[i].effect.substr(0,4) == "BOMB")
            {
                visInUse[first + j++ + WIDTH * l] = '|';
                visInUse[first + j++ + WIDTH * l] = numbers[vBuffs[i].times];
            }
        }
    }
    void update() { std::cout << visInUse << endl; }
    void editCardX(const cardHolder& c, int index)
    {
        int top{ WIDTH * 20 + 6 + index * 10 };
        if ((*c).getEnergy() <= static_cast<int>(visInUse[WIDTH * 5 + 11]) && (*c).castable())
        {
            visInUse[top] = '(';
            visInUse[top + 1] = numbers[(index + 1) % 10];
            visInUse[top + 2] = ')';
        }
        else
        {
            visInUse[top] = ' ';
            visInUse[top + 1] = ' ';
            visInUse[top + 2] = ' ';
        }
        int first{ WIDTH * 21 + 4 + index * 10 };
        string sc = (*c).getSixChar();
        for (int i{ 0 }; i < 6; ++i)
        {
            visInUse[first + i] = sc[i];
        }
        visInUse[first + 6] = ':';
        if ((*c).getEnergy() == -1) visInUse[first + 7] = 'X';
        else { visInUse[first + 7] = numbers[(*c).getEnergy()]; }
    }
    void editCardEmpty(int index)
    {
        int top{ WIDTH * 20 + 6 + index * 10 };
        visInUse[top] = ' ';
        visInUse[top + 1] = ' ';
        visInUse[top + 2] = ' ';
        int first{ WIDTH * 21 + 4 + index * 10 };
        for (int i{ 0 }; i < 8; ++i)
        {
            visInUse[first + i] = ' ';
        }
    }
    void editAllHand(cardDeckCH& hand)
    {
        int index{ 0 };
        while (index < hand.size())
        {
            editCardX(hand[index], index);
            ++index;
        }
        while (index < 10)
        {
            editCardEmpty(index++);
        }
    }
    void clearRelics()
    {
        int first{ WIDTH * 3 + 26 };
        for (int i{ 0 }; i < 100; ++i)
        {
            for (int j{ 0 }; j < 3; ++j)
            {
                visInUse[first + i + WIDTH * j] = ' ';
            }
        }
    }
    void editRelics(vector<relic*>& relics)
    {
        clearRelics();
        int first{ WIDTH * 3 + 26 }, j{ 0 };
        for (int i{ 0 }; i < relics.size(); ++i)
        {
            if (j > 85) { j = 0; first += WIDTH; }
            if (i > 0 && j > 0) { visInUse[first + j] = ','; j += 2; }
            string name = relics[i]->getName();
            for (int k{ 0 }; k < name.size(); ++k)
            {
                visInUse[first + j] = name[k];
                ++j;
            }
            int temp = relics[i]->getCounter();
            if (temp != -1)
            {
                temp = clamp(temp, 0, 9);
                visInUse[first + (j++)] = ':';
                visInUse[first + (j++)] = numbers[temp];
            }
        }
    }
    void clearPotions()
    {
        int first{ WIDTH + 27 };
        for (int i{ 0 }; i < 75; ++i)
        {
            visInUse[first + i] = ' ';
        }
    }
    void editPotions(vector<potion*>& potions, int maxPots)
    {
        clearPotions();
        int first{ WIDTH + 27 }, j{ 0 };
        for (int i{ 0 }; i < maxPots; ++i)
        {
            if (i > 0 && j > 0) { visInUse[first + j] = ','; j += 2; }
            string name;
            if (i < potions.size()) name = "p" + to_string(i + 1) + ": " + potions[i]->getName();
            else { name = "Empty"; }
            for (int k{ 0 }; k < name.size(); ++k)
            {
                visInUse[first + j] = name[k];
                ++j;
            }
        }
    }





    void intentsWriting(int& first, int& i, int& j, int& k, vector<Intents>& vBuffs)
    {
        visInUse[first + (j++) + WIDTH * k] = ':';
        if (vBuffs[i].value < 0) visInUse[first + (j++) + WIDTH * k] = '-';
        if (vBuffs[i].value < 100)
        {
            changeTwoDigit(vBuffs[i].value, first + j + WIDTH * k);
            j += 3;
        }
        else
        {
            changeThreeDigit(vBuffs[i].value, first + j + WIDTH * k);
            j += 4;
        }
        if (vBuffs[i].times >= 2)
        {
            visInUse[first + j++ + WIDTH * k] = '|';
            if (vBuffs[i].times <= 99)
            {
                changeTwoDigit(vBuffs[i].times, first + j + WIDTH * k);
                j += 2;
            }
            else
            {
                changeThreeDigit(vBuffs[i].times, first + j + WIDTH * k);
                j += 3;
            }
        }
    }



    void clearEnemyAltBuffs2()
    {
        int first{ WIDTH * 16 + 38 };
        for (int j{ 0 }; j < 25; ++j)
        {
            visInUse[first + j] = ' ';
        }
        for (int j{ 0 }; j < 25; ++j)
        {
            visInUse[first + j + WIDTH] = ' ';
        }
        int second{ WIDTH * 16 + 68 };
        for (int j{ 0 }; j < 25; ++j)
        {
            visInUse[second + j] = ' ';
        }
        for (int j{ 0 }; j < 25; ++j)
        {
            visInUse[second + j + WIDTH] = ' ';
        }
    }
    void addEnemyAltBuffs(vector<Intents>& vBuffs, vector<Intents>& vBuffs2)
    {
        clearEnemyAltBuffs2();
        int first{ WIDTH * 16 + 38 }, j{ 0 }, k{ 0 };
        for (int i{ 0 }; i < vBuffs.size(); ++i)
        {
            if (j > 30) { j = 0; ++k; }
            if (j > 30 && k == 2) throw runtime_error("too many buffs");
            if (vBuffs[i].value == 0 && vBuffs[i].times < 2) continue;
            if (i > 0 && j > 0) { visInUse[first + j + WIDTH * k] = ','; j += 2; }
            if (vBuffs[i].effect.size() + j + 4 > 30) {++k; j = 0; }
            for (int l{ 0 }; l < vBuffs[i].effect.size(); ++l)
            {
                visInUse[first + j + k * WIDTH] = vBuffs[i].effect[l];
                ++j;
            }
            if (vBuffs[i].value == 1000) { continue; }
            intentsWriting(first, i, j, k, vBuffs);
        }
        int second{ WIDTH * 16 + 68 };
        j = 0; k = 0;
        for (int i{ 0 }; i < vBuffs2.size(); ++i)
        {
            if (j > 30) { j = 0; ++k; }
            if (j > 30 && k == 2) throw runtime_error("too many buffs");
            if (vBuffs2[i].value == 0 && vBuffs2[i].times < 2) continue;
            if (i > 0 && j > 0) { visInUse[second + j] = ','; j += 2; }
            if (vBuffs2[i].effect.size() + j + 4 > 30) {++k; j = 0; }
            for (int l{ 0 }; l < vBuffs2[i].effect.size(); ++l)
            {
                visInUse[second + j + k * WIDTH] = vBuffs2[i].effect[l];
                ++j;
            }
            if (vBuffs2[i].value == 1000) { continue; }
            intentsWriting(second, i, j, k, vBuffs2);
        }
    }
    
    void clearEnemyAltBuffs3()
    {
        int first{ WIDTH * 16 + 94 };
        for (int j{ 0 }; j < 25; ++j)
        {
            visInUse[first + j] = ' ';
        }
        for (int j{ 0 }; j < 25; ++j)
        {
            visInUse[first + j + WIDTH] = ' ';
        }

    }
    void addEnemyAltBuffs(vector<Intents>& vBuffs, vector<Intents>& vBuffs2, vector<Intents>& vBuffs3)
    {
        addEnemyAltBuffs(vBuffs, vBuffs2);
        clearEnemyAltBuffs3();
        int first{ WIDTH * 16 + 94 }, j{ 0 }, k{ 0 };
        for (int i{ 0 }; i < vBuffs3.size(); ++i)
        {
            if (j > 30) { j = 0; ++k; }
            if (j > 30 && k == 2) throw runtime_error("too many buffs");
            if (vBuffs3[i].value == 0 && vBuffs3[i].times < 2) continue;
            if (i > 0 && j > 0) { visInUse[first + j + WIDTH * k] = ','; j += 2; }
            if (vBuffs3[i].effect.size() + j + 4 > 30) {++k; j = 0;}
            for (int l{ 0 }; l < vBuffs3[i].effect.size(); ++l)
            {
                visInUse[first + j + k * WIDTH] = vBuffs3[i].effect[l];
                ++j;
            }
            if (vBuffs3[i].value == 1000) { continue; }
            intentsWriting(first, i, j, k, vBuffs3);
        }
    }
    
    void clearEnemyAltBuffs4()
    {
        int first{ WIDTH * 16 + 119 };
        for (int j{ 0 }; j < 25; ++j)
        {
            visInUse[first + j] = ' ';
        }
        for (int j{ 0 }; j < 25; ++j)
        {
            visInUse[first + j + WIDTH] = ' ';
        }

    }
    void addEnemyAltBuffs(vector<Intents>& vBuffs, vector<Intents>& vBuffs2, vector<Intents>& vBuffs3, vector<Intents>& vBuffs4)
    {
        addEnemyAltBuffs(vBuffs, vBuffs2, vBuffs3);
        clearEnemyAltBuffs4();
        int first{ WIDTH * 16 + 119 }, j{ 0 }, k{ 0 };
        for (int i{ 0 }; i < vBuffs4.size(); ++i)
        {
            if (j > 30) { j = 0; ++k; }
            if (j > 30 && k == 2) throw runtime_error("too many buffs");
            if (vBuffs4[i].value == 0 && vBuffs4[i].times < 2) continue;
            if (i > 0 && j > 0) { visInUse[first + j + WIDTH * k] = ','; j += 2; }
            if (vBuffs4[i].effect.size() + j + 4 > 30) {++k;j = 0; }
            for (int l{ 0 }; l < vBuffs4[i].effect.size(); ++l)
            {
                visInUse[first + j + k * WIDTH] = vBuffs4[i].effect[l];
                ++j;
            }
            if (vBuffs4[i].value == 1000) { continue; }
            intentsWriting(first, i, j, k, vBuffs4);
        }
    }
    
    void clearEnemyAltBuffs5()
    {
        int first{ WIDTH * 16 + 146 };
        for (int j{ 0 }; j < 25; ++j)
        {
            visInUse[first + j] = ' ';
        }
        for (int j{ 0 }; j < 25; ++j)
        {
            visInUse[first + j + WIDTH] = ' ';
        }

    }
    void addEnemyAltBuffs(vector<Intents>& vBuffs, vector<Intents>& vBuffs2, vector<Intents>& vBuffs3, vector<Intents>& vBuffs4, vector<Intents>& vBuffs5)
    {
        addEnemyAltBuffs(vBuffs, vBuffs2, vBuffs3, vBuffs4);
        clearEnemyAltBuffs5();
        int first{ WIDTH * 16 + 146 }, j{ 0 }, k{ 0 };
        for (int i{ 0 }; i < vBuffs5.size(); ++i)
        {
            if (j > 30) { j = 0; ++k; }
            if (j > 30 && k == 2) throw runtime_error("too many buffs");
            if (vBuffs5[i].value == 0 && vBuffs5[i].times < 2) continue;
            if (i > 0 && j > 0) { visInUse[first + j + WIDTH * k] = ','; j += 2; }
            if (vBuffs5[i].effect.size() + j + 4 > 30) {++k;j = 0; }
            for (int l{ 0 }; l < vBuffs5[i].effect.size(); ++l)
            {
                visInUse[first + j + k * WIDTH] = vBuffs5[i].effect[l];
                ++j;
            }
            if (vBuffs5[i].value == 1000) { continue; }
            intentsWriting(first, i, j, k, vBuffs5);
            }
    }
    


    void addEnemyIntents(vector<Intents> vIntents, int index, bool goRegardless)
    {
        if (ai && !goRegardless) return;
        int first{0};
        switch (AIU_)
        {
        case aiu::one:
            clearEnemyIntents();
            first = WIDTH * 8 + 72;
            break;
        case aiu::two:
            clearEnemyIntents2(index);
            switch (index)
            {
            case 0: first = WIDTH * 7 + 59; break;
            case 1: first = WIDTH * 7 + 87; break;
            }
            break;
        case aiu::three:
            switch (index)
            {
            case 0: first = WIDTH * 7 + 59; break;
            case 1: first = WIDTH * 7 + 87; break;
            case 2: first = WIDTH * 7 + 112; break;
            }
            break;
        case aiu::four:
            switch (index)
            {
            case 0: first = WIDTH * 7 + 59; break;
            case 1: first = WIDTH * 7 + 87; break;
            case 2: first = WIDTH * 7 + 112; break;
            case 3: first = WIDTH * 7 + 139; break;
            }
            break;
        case aiu::five:
            switch (index)
            {
            case 0: first = WIDTH * 7 + 59; break;
            case 1: first = WIDTH * 7 + 87; break;
            case 2: first = WIDTH * 7 + 112; break;
            case 3: first = WIDTH * 7 + 139; break;
            case 4: first = WIDTH * 7 + 166; break;
            }
            break;
        }
        for (int i{ 0 }; i < vIntents.size(); ++i)
        {
            first += WIDTH;
            int j{ 0 };
            for (j; j < vIntents[i].effect.size(); ++j)
            {
                visInUse[first + j] = vIntents[i].effect[j];
            }
            if (vIntents[i].value == 1000)
            {
                switch (AIU_)
                {
                    case aiu::one: ++linesOfIntent; continue;
                    case aiu::two: switch (index) {
                        case 0: ++linesOfIntent0; continue;
                        case 1: ++linesOfIntent1; continue;
                    }
                    case aiu::three: switch (index) {
                        case 0: ++linesOfIntent0; continue;
                        case 1: ++linesOfIntent1; continue;
                        case 2: ++linesOfIntent2; continue;
                    }
                    case aiu::four: switch (index) {
                        case 0: ++linesOfIntent0; continue;
                        case 1: ++linesOfIntent1; continue;
                        case 2: ++linesOfIntent2; continue;
                        case 3: ++linesOfIntent3; continue;
                    }
                    case aiu::five: switch (index) {
                        case 0: ++linesOfIntent0; continue;
                        case 1: ++linesOfIntent1; continue;
                        case 2: ++linesOfIntent2; continue;
                        case 3: ++linesOfIntent3; continue;
                        case 4: ++linesOfIntent4; continue;
                    }
                }
            }
            visInUse[first + j] = ':';
            if (vIntents[i].effect == "Attack") changeTwoDigit(vIntents[i].adjustedValue, first + j + 1);
            else { changeTwoDigit(vIntents[i].value, first + j + 1); }
            if (vIntents[i].times > 1)
            {
                visInUse[first + j + 3] = 'x';
                changeTwoDigit(vIntents[i].times, first + j + 4);
            }
            switch (AIU_)
            {
                case aiu::one: ++linesOfIntent; continue;
                case aiu::two: switch (index) {
                    case 0: ++linesOfIntent0; continue;
                    case 1: ++linesOfIntent1; continue;
                }
                case aiu::three: switch (index) {
                    case 0: ++linesOfIntent0; continue;
                    case 1: ++linesOfIntent1; continue;
                    case 2: ++linesOfIntent2; continue;
                }
                case aiu::four: switch (index) {
                    case 0: ++linesOfIntent0; continue;
                    case 1: ++linesOfIntent1; continue;
                    case 2: ++linesOfIntent2; continue;
                    case 3: ++linesOfIntent3; continue;
                }
                case aiu::five: switch (index) {
                    case 0: ++linesOfIntent0; continue;
                    case 1: ++linesOfIntent1; continue;
                    case 2: ++linesOfIntent2; continue;
                    case 3: ++linesOfIntent3; continue;
                    case 4: ++linesOfIntent4; continue;
                }
            }
        }
    }
    void clearEnemyIntents()
    {
        int first{ WIDTH * 9 + 72 };
        for (int i{ 0 }; i <= linesOfIntent; ++i)
        {
            for (int j{ 0 }; j < 20; ++j)
            {
                visInUse[first + WIDTH * i + j] = ' ';
            }
        }
        linesOfIntent = 0;
    }
    void clearEnemyIntents2(int ii)
    {
        switch (ii)
        {
        case 0:
        {
            int first{ WIDTH * 8 + 59 };
            for (int i{ 0 }; i <= linesOfIntent0; ++i)
            {
                for (int j{ 0 }; j < 18; ++j)
                {
                    visInUse[first + WIDTH * i + j] = ' ';
                }
            }
            linesOfIntent0 = 0;
            break;
        }
        case 1:
        {
            int second{ WIDTH * 8 + 87 };
            for (int i{ 0 }; i <= linesOfIntent1; ++i)
            {
                for (int j{ 0 }; j < 18; ++j)
                {
                    visInUse[second + WIDTH * i + j] = ' ';
                }
            }
            linesOfIntent1 = 0;
            break;
        }
        }
    }
    void clearEnemyIntents3()
    {
        int first{ WIDTH * 8 + 112 };
        for (int i{ 0 }; i <= linesOfIntent2; ++i)
        {
            for (int j{ 0 }; j < 18; ++j)
            {
                visInUse[first + WIDTH * i + j] = ' ';
            }
        }
        linesOfIntent2 = 0;
    }
    void clearEnemyIntents4()
    {
        int first{ WIDTH * 8 + 139 };
        for (int i{ 0 }; i <= linesOfIntent3; ++i)
        {
            for (int j{ 0 }; j < 18; ++j)
            {
                visInUse[first + WIDTH * i + j] = ' ';
            }
        }
        linesOfIntent3 = 0;
    }
    void clearEnemyIntents5()
    {
        int first{ WIDTH * 8 + 166 };
        for (int i{ 0 }; i <= linesOfIntent4; ++i)
        {
            for (int j{ 0 }; j < 18; ++j)
            {
                visInUse[first + WIDTH * i + j] = ' ';
            }
        }
        linesOfIntent4 = 0;
    }
    void clearEnemyIntents2()
    {
        int first{ WIDTH * 8 + 59 };
        for (int i{ 0 }; i <= linesOfIntent0; ++i)
        {
            for (int j{ 0 }; j < 18; ++j)
            {
                visInUse[first + WIDTH * i + j] = ' ';
            }
        }
        linesOfIntent0 = 0;

        int second{ WIDTH * 8 + 87 };
        for (int i{ 0 }; i <= linesOfIntent1; ++i)
        {
            for (int j{ 0 }; j < 18; ++j)
            {
                visInUse[second + WIDTH * i + j] = ' ';
            }
        }
        linesOfIntent1 = 0;
    }
    void addEnemyIntents(deque<vector<Intents>> vIntents, bool goRegardless)
    {
        if(ai && goRegardless)
        {
            switch (AIU_)
            {
            case aiu::five: clearEnemyIntents5(); // deliberate
            case aiu::four: clearEnemyIntents4(); // deliberate
            case aiu::three: clearEnemyIntents3(); // deliberate
            case aiu::two: clearEnemyIntents2(); break;
            case aiu::one: clearEnemyIntents(); break;
            }
        }
        for (int k{ 0 }; k < vIntents.size(); ++k)
        {
            if (!vIntents[k].empty()) addEnemyIntents(vIntents[k], k,goRegardless);
        }
    }

    void processIntPM(int n, int first)
    {
        if (n < 0)
        {
            visInUse[first - 1] = '-';
            changeTwoDigit(-n, first);
        }
        else
        {
            visInUse[first - 1] = '+';
            changeTwoDigit(n, first);
        }
    }
    void processInt(int n, int first)
    {
        changeTwoDigit(n, first);
    }
    void processIntHP(int n, int first)
    {
        if (n < 100)
        {
            visInUse[first - 1] = ' ';
            changeTwoDigit(n, first);
        }
        else
        {
            changeThreeDigit(n, first - 1);
        }
    }
    void processIntMHP(int n, int first)
    {
        if (n < 100)
        {
            visInUse[first + 2] = ' ';
            changeTwoDigit(n, first);
        }
        else
        {
            changeThreeDigit(n, first);
        }
    }
    void processString(string name, int first)
    {
        for (int i{ 0 }; i < 20; ++i)
        {
            visInUse[first + i] = ' ';
        }
        for (int i{ 0 }; i < name.size(); ++i)
        {
            visInUse[first + i] = name[i];
        }
    }

}
