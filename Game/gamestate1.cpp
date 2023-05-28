#include "gamestate.h"
#include "events.h"
#include "card.h"
#include "pc_info.h"
#include "character.h"
#include "visuals.h"

using namespace std;

std::string request_input(std::vector<std::string> valid_inputs)
{
    if (ai)
    {
        if (gamestate::randomAIChoice)
        {
            return valid_inputs[random_int(0, static_cast<int>(valid_inputs.size()) - 1)];
        }
        else if (!gamestate::aiEventActionBuffer.empty())
        {
            if (find(valid_inputs.begin(), valid_inputs.end(), gamestate::aiEventActionBuffer[0]) == valid_inputs.end())
            {
                int a = 9 + 10; a;
            }
            std::string res = gamestate::aiEventActionBuffer[0];
            //cout << "Choice: " << res << endl;
            gamestate::aiEventActionBuffer.pop_front();
            return res;
        }
        else
        {
            if (!gamestate::aiSimActionBuffer.empty())
            {
                std::string res = to_string(gamestate::aiSimActionBuffer[0]);
                gamestate::aiSimActionBuffer.pop_front();
                if (gamestate::orderedChoiceTree.empty()) return res;
                if (gamestate::orderedChoiceTree[0].size() == 2)
                { 
                    gamestate::randomAIChoice = gamestate::aiSimActionBuffer.size() % 2 == 0;
                }
                else { gamestate::randomAIChoice = true; }
                return res;
            }
            else
            {
                // don't want to use this currently but once everything checked then fine as catch all
                gamestate::randomAIChoice = true;
                return valid_inputs[random_int(0, static_cast<int>(valid_inputs.size()) - 1)];
            }
        }
    }
    else
    {
        std::string input{ "repeat" };
        while (true)
        {
            std::getline(std::cin, input);
            for (auto itr = valid_inputs.begin(); itr < valid_inputs.end(); itr++) { if (input == *itr)return input; }
        }
    }
}
namespace adjusted
{
    int calcEnemyDamage(const nonpc& enemy_, const int& amount_)
    {
        int amount = clamp(amount_ + enemy_.getTotalStrength(), 0, 10000);
        if (enemy_.getWeak()) amount = static_cast<int>(amount * enemy_.getWeakMulti());
        amount = static_cast<int>(amount * ((apci.getPlayer().getVulnerable() ? apci.getPlayer().getVulnerableMulti() : 1) + (enemy_.getBackAttack() ? 0.5 : 0)));
        if (apci.getPlayer().getIntangible() > 0 && amount > 0) amount = 1;
        return amount;
    }
}
gamestate::gamestate()
{
    for (int i{ 0 }; i < 6; ++i) intents_.push_back({});
    persistentFE = vector<focusedEvent*>(11, nullptr);
    temporaryFE = vector<focusedEvent*>(31, nullptr);
    dormant_enemies_ = vector<vector<unique_ptr<nonpc>>>(3);
}
void gamestate::makeDatabase()
{
    makeCardDatabase();
    makeEnemyDatabase();
    makeRelicDatabase();
    makePotionDatabase();
}
void gamestate::cycleAct()
{
    if (actNumber == 1)
    {
        makeEnemyEventsA2();
        actNumber = 2;
    }
    else if(actNumber == 2)
    {
        //makeEnemyEvents();
        //actNumber = 1;
        makeEnemyEventsA3();
        actNumber = 3;
    }
    else if(actNumber == 3)
    {
        makeEnemyEvents();
        actNumber = 1;
        //makeEnemyEventsA4();
        //actNumber = 4;
    }
    else if(actNumber == 4)
    {
        makeEnemyEvents();
        actNumber = 1;
    }
}
void gamestate::decreasePlayerEnergy(int i)
{
    if (i == -1) { xCost_ = playerEnergy_; playerEnergy_ = 0; return; }
    playerEnergy_ -= i;
    if (playerEnergy_ < 0) throw runtime_error("negative energy");
}
void gamestate::resetFocusedEvents()
{
    for (auto a : temporaryFE) { delete a; }
    temporaryFE = vector<focusedEvent*>(31, nullptr);
}
void gamestate::clearEvents()
{
    for (auto a : questionEvents) {delete a;}
    questionEvents.clear();
    for (auto a : persistentFE) { delete a; }
    persistentFE = vector<focusedEvent*>(11, nullptr);
    delete events_eh; events_eh = nullptr;
    delete events_hh; events_hh = nullptr;
    delete events_el; events_el = nullptr;
    delete events_bo; events_bo = nullptr;
    delete events_ca; events_ca = nullptr;
    delete events_qs; events_qs = nullptr;
}
void gamestate::addCheatCard()
{
    string s;
    int i;
    cin >> s >> i;
    try {
        auto temp = getCardPByID(s);
        if (i < 1 || i>9) throw runtime_error("caught");
        auto temp2 = cardHolder(temp);
        while (i--) apci.addCardToHand(temp2);
    }
    catch (...) { std::cout << "invalid card."; }
}
vector<string> gamestate::getCommonCards() const
{
    return commonCards;
}
vector<string> gamestate::getUncommonCards() const
{
    return uncommonCards;
}
vector<string> gamestate::getRareCards() const
{
    auto temp = rareCards;
    temp.push_back("Feed_ID");
    temp.push_back("Reaper_ID");
    return temp;
}
void gamestate::showBasicPath()
{
    for (int i{ 0 }; i < mapPath.size(); ++i)
    {
        std::cout << mapPath[i] << ". ";
    }
    std::cout << endl;
}

void gamestate::showPath()
{
    for (int i{ 0 }; i < mapPath.size(); ++i)
    {
        string toprint;
        if (i == mapPathIndex) toprint = "## HERE ##";
        else
        {
            switch (mapPath[i])
            {
            case 'N':
                toprint = "Neow";
                break;
            case 'e':
                toprint = "Hallway Fight";
                break;
            case 'E':
                toprint = "Elite Fight";
                break;
            case 'B':
                toprint = "Boss Fight";
                break;
            case 'c':
                toprint = "Campfire";
                break;
            case 's':
                toprint = "Shop";
                break;
            case 'C':
                toprint = "Chest";
                break;
            case '?':
                toprint = "Random Event";
                break;
            }
        }
        std::cout << toprint;
        if (i != mapPath.size() - 1) std::cout << " -> ";
    }
    std::cout << "." << endl << endl;
}
void gamestate::setPlayerEnergySOT(int i)
{
    if (iceCreamCheck) { playerEnergy_ = turnCount == 0 ? i : (i + playerEnergy_); }
    else { playerEnergy_ = i; }
    if (playerEnergy_ < 0) throw runtime_error("negative energy");
}
void gamestate::reshuffleIntents()
{
    int i{ 0 }, emptied{ 0 };
    while (i + emptied < intents_.size())
    {
        if (intents_[i].empty())
        {
            intents_.erase(intents_.begin() + i);
            intents_.push_back({});
            ++emptied;
        }
        else { ++i; }
    }
}
int gamestate::getTotalIncomingDamage()
{
    int sum{ 0 };
    for (int i{ 0 }; i < intents_.size(); ++i)
    {
        for (int j{ 0 }; j < intents_[i].size(); ++j)
        {
            if (intents_[i][j].effect == "Attack") sum +=adjusted::calcEnemyDamage(*(gm.getAllEnemies()[i]), intents_[i][j].value);
        }
    }
    return sum;
}
void gamestate::adjustAllIntentsAttacks()
{
    for (int i{ 0 }; i < intents_.size(); ++i)
    {
        for (int j{ 0 }; j < intents_[i].size(); ++j)
        {
            if (intents_[i][j].effect == "Attack") intents_[i][j].adjustedValue = adjusted::calcEnemyDamage(*(gm.getAllEnemies()[i]), intents_[i][j].value);
        }
    }
}
void gamestate::resetVisuals()
{
    int newSize = static_cast<int>(enemies_.size());
    switch (newSize)
    {
    case 1:
        visuals::setvisInUse(aiu::one);
        break;
    case 2:
        visuals::setvisInUse(aiu::two);
        break;
    case 3:
        visuals::setvisInUse(aiu::three);
        break;
    case 4:
        visuals::setvisInUse(aiu::four);
        break;
    case 5:
        visuals::setvisInUse(aiu::five);
        break;
    }
}
void gamestate::removeFromCombat(nonpc* enemy_)
{
    //for (auto itr{ enemies_.begin() }; itr != enemies_.end(); ++itr)
    //{
    //    if (enemy_ == itr->get())
    //    {
    //        enemies_.erase(itr);
    //        break;
    //    }
    //}
    for (int i{ 0 }; i < enemies_.size(); ++i)
    {
        if (enemy_ == enemies_[i].get())
        {
            enemies_.erase(enemies_.begin() + i);
            intents_.erase(intents_.begin() + i);
            intents_.push_back({});
            break;
        }
    }
    for (int i{ 0 }; i < enemies_.size(); ++i)
    {
        enemies_[i]->setSCRNindex(i);
    }
    if (enemies_.size() != 0) resetVisuals();
    //reshuffleIntents();
}
void gamestate::makeEvents()
{
    makeEnemyEvents();
    makeQuestionEvents();
}
void gamestate::setBossIndex() { bossIndexThisAct = events_bo->selectRandom(); }
void gamestate::makeEnemyEvents()
{
    easyDone = 0;
    if (events_eh) delete events_eh;
    if (events_hh) delete events_hh;
    if (events_el) delete events_el;
    if (events_bo) delete events_bo;

    events_eh = new easyHallway;
    events_eh->setPFight({ 25.0f,25.0f,25.0f,25.01f });
    events_hh = new hardHallway;
    events_hh->setPFight({ 6.25f, 12.5f, 6.25f, 12.5f, 6.25f, 12.5f, 12.5f, 9.375f, 9.375f, 12.51f });
    events_el = new elites;
    events_bo = new bosses;
    if (ai) events_qs = new qSpecial;
    if(!events_ca) events_ca = new campfire;

    // boss
    events_bo->bossOptions.push_back({ "TheGuardian_ID" });
    events_bo->bossOptions.push_back({ "HexaGhost_ID" });
    events_bo->bossOptions.push_back({ "SlimeBoss_ID" });
}
void gamestate::makeEnemyEventsA2()
{
    easyDone = 0;
    if (events_eh) delete events_eh;
    if (events_hh) delete events_hh;
    if (events_el) delete events_el;
    if (events_bo) delete events_bo;

    events_eh = new easyHallway; // 5
    events_eh->setPFight({ 20.0f,20.0f,20.0f,20.0f,20.01f });
    events_hh = new hardHallway; // 8
    events_hh->setPFight({ 7.0f, 10.0f, 7.0f, 21.0f, 14.0f, 21.0f, 10.0f, 10.01f });
    events_el = new elites; // 3
    events_bo = new bosses;
    if (ai) events_qs = new qSpecial;

    events_bo->bossOptions.push_back({ "TheCollector_ID" });
    events_bo->bossOptions.push_back({ "TheChamp_ID" });
    events_bo->bossOptions.push_back({ "BronzeAutomaton_ID" });
}
void gamestate::makeEnemyEventsA3()
{
    easyDone = 0;
    if (events_eh) delete events_eh;
    if (events_hh) delete events_hh;
    if (events_el) delete events_el;
    if (events_bo) delete events_bo;

    events_eh = new easyHallway; // 3
    events_eh->setPFight({ 33.34f,33.34f,33.34f });
    events_hh = new hardHallway; // 8
    events_hh->setPFight({ 12.5f,12.5f,12.5f,12.5f,12.5f,12.5f,12.5f,12.51f });
    events_el = new elites; // 3
    events_bo = new bosses;
    if (ai) events_qs = new qSpecial;

    events_bo->bossOptions.push_back({ "Deca_ID","Donu_ID" });
    events_bo->bossOptions.push_back({ "TimeEater_ID" });
    events_bo->bossOptions.push_back({ "Cultist_ID","Cultist_ID","AwakenedOne_ID" });
}
void gamestate::makeEnemyEventsA4()
{
    if (events_eh) delete events_eh;
    if (events_hh) delete events_hh;
    if (events_el) delete events_el;
    if (events_bo) delete events_bo;

    events_eh = nullptr;
    events_hh = nullptr;
    events_el = new elites;
    events_bo = new bosses;

    events_bo->bossOptions.push_back({ "CorruptHeart_ID" });
}

// more messy than i would like, originally was just three lamba funcs for each event choice: text to display, choice outcome and requirements met
// certain events are arguably too long to sensibly use lambdas
// each event also needs a name, acts it can be present in, if its a shrine or not, prerequisites for it showing up, would maybe have been better off making a class and functions for each
void gamestate::makeQuestionEvents()
{
    questionEvents.push_back
    (
        new  questionMark(0,"Bonfire Spirits", 
            {
                questionChoices([]() {string res = "Offer : Offer a card to the spirits"; return res; },
                                []()
                                {
                                    auto temp = gm.chooseCardFromDeck();
                                    switch ((*temp)->getRarity())
                                    {
                                        case cardRarity::Other:
                                            break;
                                        case cardRarity::Common:
                                            if ((*temp)->getName() == "Strike" || (*temp)->getName() == "Defend" || (*temp)->getName() == "Bash") {}
                                            else { apci.getPlayer().heal(5); }
                                            break;
                                        case cardRarity::Uncommon:
                                            apci.getPlayer().healToFull();
                                            break;
                                        case cardRarity::Rare:
                                            apci.getPlayer().healToFull();
                                            apci.getPlayer().gainMaxHP(10);
                                            break;
                                    }
                                    apci.removeCard(temp,true);
                                },
                                []() {return true; }),
            }, []() {gamestate::aiEventActionBuffer.push_back("1"); gm.makeCardChoices(aiEventCardChoice::Remove); }, true)
    );
    questionEvents.push_back
    (
        new  questionMark(0,"Divine Fountain", 
            []() { return apci.checkForCurse(); },
            {
                questionChoices([]() {string res = "Drink : Remove all curses from your deck"; return res; },
                                []() 
                    {
                        while (apci.checkForCurse())
                        {
                            for (int i{ 0 }; i < apci.getDeck().size(); ++i)
                            {
                                if (apci.getDeck()[i]->getType() == Curse)
                                {
                                    apci.removeCard(apci.getDeck().begin() + i);
                                    break;
                                }
                            }
                        }
                    }, []() {return true; }),
                questionChoices([]() {string res = "Leave : Nothing happens"; return res; },
                                []() {}, []() {return true; })
            }, []() { if(apci.checkForRelic("Du-Vu Doll")) gamestate::aiEventActionBuffer.push_back("2");
                        else { gamestate::aiEventActionBuffer.push_back("1"); } }, true)
    );
    questionEvents.push_back
    (
        new  questionMark(0,"Duplicator",
            {
                questionChoices([]() {string res = "Pray : Duplicate a card in your deck"; return res; },
                                []() {auto temp = gm.chooseCardFromDeck(true); apci.addCardToDeck(temp->get()); }, []() {return true; }),
                questionChoices([]() {string res = "Leave : Nothing happens"; return res; },
                                []() {}, []() {return true; })
            }, []() {gamestate::aiEventActionBuffer.push_back("1"); gm.makeCardChoices(aiEventCardChoice::Duplicate); }, true)
    );
    questionEvents.push_back
    (
        new  questionMark(0,"Golden Shrine", 
            {
                questionChoices([]() {string res = "Pray : Gain 100 Gold"; return res; },
                                []() {apci.gainGold(100); }, []() {return true; }),
                questionChoices([]() {string res = "Desecrate : Gain 275 Gold. Become Cursed - Regret"; return res; },
                                []() {apci.gainGold(275); apci.addCardToDeck(gm.getCurseCardByID("Regret_ID")); }, []() {return true; }),
                questionChoices([]() {string res = "Leave : Nothing happens"; return res; },
                                []() {}, []() {return true; })
            }, []() { 
                if(apci.fineForCurse()) gamestate::aiEventActionBuffer.push_back("2");
                else { gamestate::aiEventActionBuffer.push_back("1"); } }
        )
    );
    questionEvents.push_back
    (
        new  questionMark(0,"Lab", 
            {
                questionChoices([]() {string res = "Search : Obtain 3 random potions"; return res; },
                                []() {gm.XPotionSelection(gm.retrieveXRandomPotions(3)); }, []() {return true; })
            },
            []() {gamestate::aiEventActionBuffer.push_back("1"); },
            true)
    );
    questionEvents.push_back
    (
        new  questionMark(0,"Match and Keep", 
            {
                questionChoices([]() {string res = "Play : Match the cards to keep them"; return res; },
                                []() {
                                        vector<card*> cards;
                                        cards.push_back(gm.getCardByID("Bash_ID")->clone());
                                        cards.push_back(gm.getCardOfRarity(cardRarity::Common));
                                        cards.push_back(gm.getCardOfRarity(cardRarity::Uncommon));
                                        cards.push_back(gm.getCardOfRarity(cardRarity::Rare));
                                        cards.push_back(gm.getXRandomColourlessCards(1)[0]->second->clone());
                                        cards.push_back(gm.getCurse());
                                        for (int i{0}; i < 6; ++i) { cards.push_back(cards[i]->clone()); }

                                        std::random_device rd; std::mt19937 g(rd());
                                        std::shuffle(cards.begin(), cards.end(), g);

                                        int counter{6};
                                        vector<string> opts{"A","B","C","D","E","F","G","H","I","J","K","L"};
                                        vector<card*> knownCards(12,nullptr);
                                        while (--counter)
                                        {
                                            auto tempOpts = opts;
                                            std::cout << "Tries remaining: " << counter << endl;
                                            std::cout << "A B C D" << endl;
                                            std::cout << "E F G H" << endl;
                                            std::cout << "I J K L" << endl;
                                            gm.qEventMatchAndKeep(knownCards, opts);
                                            string res = request_input(opts);
                                            int firstName{0}, secondName{0};
                                            for (int i{0}; i < opts.size(); ++i)
                                            {
                                                if (opts[i] == res)
                                                {
                                                    firstName = i;
                                                    knownCards[firstName] = cards[firstName];
                                                    std::cout << res << " = " << cards[firstName]->getName() << endl;
                                                    tempOpts.erase(tempOpts.begin() + i);
                                                    break;
                                                }
                                            }
                                            gm.qEventMatchAndKeep(knownCards, opts, firstName);
                                            res = request_input(tempOpts);
                                            for (int i{0}; i < tempOpts.size(); ++i)
                                            {
                                                if (tempOpts[i] == res)
                                                {
                                                    secondName = i >= firstName ? i + 1 : i;
                                                    knownCards[secondName] = cards[secondName];
                                                    std::cout << res << " = " << cards[secondName]->getName() << endl;
                                                    tempOpts.erase(tempOpts.begin() + i);
                                                    break;
                                                }
                                            }
                                            if (cards[firstName]->getName() == cards[secondName]->getName())
                                            {
                                                opts = tempOpts;
                                                std::cout << "Matched!" << endl << endl;
                                                apci.addCardToDeck(cards[firstName]);
                                                delete cards[firstName];
                                                delete cards[secondName];
                                                if (firstName > secondName)
                                                {
                                                    cards.erase(cards.begin() + firstName);
                                                    cards.erase(cards.begin() + secondName);
                                                    knownCards.erase(knownCards.begin() + firstName);
                                                    knownCards.erase(knownCards.begin() + secondName);
                                                }
                                                else
                                                {
                                                    cards.erase(cards.begin() + secondName);
                                                    cards.erase(cards.begin() + firstName);
                                                    knownCards.erase(knownCards.begin() + secondName);
                                                    knownCards.erase(knownCards.begin() + firstName);
                                                }
                                            }
                                        }
                                        while (!cards.empty())
                                        {
                                            delete cards.back();
                                            cards.pop_back();
                                        }
                                    },
                                []() {return true; })
            }, []() {gamestate::aiEventActionBuffer.push_back("1"); },
            true)
    );
    questionEvents.push_back
    (
        new  questionMark(0,"Ominous Forge", 
            {
                questionChoices([]() {string res = "Forge : Upgrade a card from your deck"; return res; },
                                []() {gm.upgradeCardFromDeck(); }, []() {return true; }),
                questionChoices([]() {string res = "Rummage : Obtain Warped Tongs. Becom Curse - Pain"; return res; },
                                []() {apci.addRelic(gm.getRelicByID("WarpedTongs_ID")); apci.addCardToDeck(gm.getCurseCardByID("Pain_ID")); }, []() {return true; }),
                questionChoices([]() {string res = "Leave : Nothing happens"; return res; },
                                []() {}, []() {return true; })
            }, []() { gm.qEventOminousForge(); }
                    //{if (apci.fineForCurse()) gamestate::aiEventActionBuffer.push_back("2");
                    //else { gamestate::aiEventActionBuffer.push_back("1"); gm.makeCardChoices(aiEventCardChoice::Upgrade);} }
        )
    );
    questionEvents.push_back
    (
        new  questionMark(0,"Purifier", 
            {
                questionChoices([]() {string res = "Pray : Remove a card from your deck"; return res; },
                                []() {gm.removeCardFromDeck(); }, []() {return true; }),
                questionChoices([]() {string res = "Leave : Nothing happens"; return res; },
                                []() {}, []() {return true; })
            }, []() {gamestate::aiEventActionBuffer.push_back("1"); gm.makeCardChoices(aiEventCardChoice::Remove); },
            true)
    );
    questionEvents.push_back
    (
        new  questionMark(0,"Transmogrifier", 
            {
                questionChoices([]() {string res = "Pray : Transform a card from your deck"; return res; },
                                []() {gm.transformCardFromDeck(); }, []() {return true; }),
                questionChoices([]() {string res = "Leave : Nothing happens"; return res; },
                                []() {}, []() {return true; })
            }, []() {gamestate::aiEventActionBuffer.push_back("1"); gm.makeCardChoices(aiEventCardChoice::Transform); },
            true)
    );
    questionEvents.push_back
    (
        new  questionMark(0,"Upgrade Shrine", 
            {
                questionChoices([]() {string res = "Pray : Upgrade a card from your deck"; return res; },
                                []() {gm.upgradeCardFromDeck(); }, []() {return true; }),
                questionChoices([]() {string res = "Leave:  Nothing happens"; return res; },
                                []() {}, []() {return true; })
            }, []() {gamestate::aiEventActionBuffer.push_back("1"); gm.makeCardChoices(aiEventCardChoice::Upgrade); },
            true)
    );
    questionEvents.push_back
    (
        new  questionMark(0,"The Woman in Blue", 
            []() { return apci.getGold() >= 50; },
            {
                questionChoices([]() {string res = "Buy 1 Potion : 20 Gold"; return res; },
                                []() {apci.spendGold(20); gm.XPotionSelection(gm.retrieveXRandomPotions(1)); }, []() {return apci.getGold() > 20; }),
                questionChoices([]() {string res = "Buy 2 Potion : 30 Gold"; return res; },
                                []() {apci.spendGold(30); gm.XPotionSelection(gm.retrieveXRandomPotions(2)); }, []() {return apci.getGold() > 30; }),
                questionChoices([]() {string res = "Buy 3 Potion : 40 Gold"; return res; },
                                []() {apci.spendGold(40); gm.XPotionSelection(gm.retrieveXRandomPotions(3)); }, []() {return apci.getGold() > 40; }),
                questionChoices([]() {string res = "Leave : Nothing happens"; return res; },
                                []() {}, []() {return true; })
            }, []() { gamestate::aiEventActionBuffer.push_back("3");},
            true)
    );
    questionEvents.push_back
    (
        new  questionMark(0,"Wheel of Change", 
            {
                questionChoices([]() {string res = "Play : Spin the Wheel"; questionMark::storedValue1 = random_int(1,6); return res; },
                                []() {
                                        switch (questionMark::storedValue1)
                                        {
                                        case 1:
                                            std::cout << "Prize: Receive a large amount of gold!" << endl;
                                            apci.gainGold(100);
                                            break;
                                        case 2:
                                            std::cout << "Prize: Obtain a random relic!" << endl;
                                            apci.addRelic(gm.retrieveRandomRelic());
                                            break;
                                        case 3:
                                            std::cout << "Prize: Heal to full!" << endl;
                                            apci.getPlayer().healToFull();
                                            break;
                                        case 4:
                                            std::cout << "Prize: Become Cursed - Decay..." << endl;
                                            apci.addCardToDeck(gm.getCurseCardByID("Decay_ID"));
                                            break;
                                        case 5:
                                            std::cout << "Prize: Remove a card from your deck!" << endl;
                                            gm.removeCardFromDeck();
                                            break;
                                        case 6:
                                            std::cout << "Prize: Take 10% of your max HP as damage..." << endl;
                                            apci.getPlayer().eventLosePercentageHP(10);
                                            break;
                                        default:
                                            break;
                                        }
                                    }, []() {return true; })
            }, []() {gamestate::aiEventActionBuffer.push_back("1"); if(questionMark::storedValue1==5) gm.makeCardChoices(aiEventCardChoice::Remove); },
            true)
    );
    questionEvents.push_back
    (                                           
        new  questionMark(0, "We Meet Again", []() {return apci.getPotions().size() > 0 || apci.getGold() >= 50 || apci.altToBasicCurseBottled(); },
            {
                questionChoices([]() {
                                    questionMark::storedValue1 = random_int(0,static_cast<int>(apci.getPotions().size()) - 1);
                                    string res = "Give Potion : Lose " + apci.getPotion(questionMark::storedValue1)->getName() + ", receive a relic";
                                    return res;},
                                []() { apci.losePotion(questionMark::storedValue1); apci.addRelic(gm.retrieveRandomRelic()); },
                                []() { return apci.getPotions().size() > 0; }),
                questionChoices([]() {
                                    questionMark::storedValue2 = random_int(50,apci.getGold());
                                    string res = "Give Gold : Lose " + to_string(questionMark::storedValue2) + " Gold, receive a relic";
                                    return res; },
                                []() {apci.loseGold(questionMark::storedValue2); apci.addRelic(gm.retrieveRandomRelic()); }, []() {return apci.getGold() >= 50; }),
                questionChoices([]() {
                                        vector<int> validOpts;
                                        for (int i{ 0 }; i < apci.getDeck().size(); ++i)
                                        {
                                            if (apci.getDeck()[i]->getCardIDNum() == 1 || apci.getDeck()[i]->getCardIDNum() == 2 || apci.getDeck()[i]->getCardIDNum() == 3 ||
                                                apci.getDeck()[i]->getType() == Curse || apci.getDeck()[i]->getBottled()) continue;
                                            validOpts.push_back(i);
                                        }
                                        questionMark::storedValue3 = validOpts[random_int(0, static_cast<int>(validOpts.size()) - 1)];
                                        string res = "Give Card : Lose " + apci.getDeck()[questionMark::storedValue3]->getName() + ", receive a relic";
                                        return res; },
                                []() {apci.removeCard(apci.getDeck().begin() + questionMark::storedValue3); apci.addRelic(gm.retrieveRandomRelic()); },
                                []() {return apci.altToBasicCurseBottled();}),
                questionChoices([]() {string res = "Leave : Nothing happens"; return res; },
                                []() {}, []() {return true; })
            }, []() {
                        if(questionMark::storedValue2 >= 50 && questionMark::storedValue2 < 100) gamestate::aiEventActionBuffer.push_back("2"); 
                        else if(questionMark::storedValue1 >= 0 && apci.getPotion(questionMark::storedValue1)->getRank() <= 2 ) gamestate::aiEventActionBuffer.push_back("1");
                        else { gamestate::aiEventActionBuffer.push_back("4"); }
                    }, true)
    );
    questionEvents.push_back
    (
        new  questionMark(1,"Big Fish", 
            {
                questionChoices([]() {return "Banana : Heal " + to_string(apci.getPlayer().getMaxHealth() / 3) + " HP"; },
                                []() {apci.getPlayer().healPercentage(33); }, []() {return true; }),
                questionChoices([]() {string res = "Donut : Gain 5 Max HP"; return res; }, []() {apci.getPlayer().gainMaxHP(5); }, []() {return true; }),
                questionChoices([]() {string res = "Box : Receive a Relic. Become Cursed - Regret"; return res; },
                                []() {apci.addCardToDeck(gm.getCurseCardByID("Regret_ID")); apci.addRelic(gm.retrieveRandomRelic()); }, []() {return true; })
            }, []() {if(apci.getPlayer().getHealth() < 35) gamestate::aiEventActionBuffer.push_back("1"); 
                    else if(apci.fineForCurse()) gamestate::aiEventActionBuffer.push_back("3");
                    else {gamestate::aiEventActionBuffer.push_back("2");}})
    );
    questionEvents.push_back
    (
        new  questionMark(1,"The Cleric", 
            []() { return apci.getGold() >= 35; },
            {
                questionChoices([]() {string res = "Heal : Lose 35 Gold. Heal 25% Max HP"; return res; },
                                []() {apci.getPlayer().healPercentage(25); apci.loseGold(35); }, []() {return apci.getGold() >= 35; }),
                questionChoices([]() {string res = "Purify : Lose 50 Gold. Remove a card from your deck"; return res; },
                                []() {apci.loseGold(50); gm.removeCardFromDeck(); }, []() {return apci.getGold() >= 50; }),
                questionChoices([]() {string res = "Leave : Nothing happens"; return res; }, []() {}, []() {return true; })
            }, []() {if (apci.getGold() >= 50) gm.qEventTheCleric();
                else if (apci.getPlayer().getPercentageHealth() < 0.9) gamestate::aiEventActionBuffer.push_back("1");
                else { gamestate::aiEventActionBuffer.push_back("3"); }}
                    //{if (apci.getPlayer().getPercentageHealth() < 0.6) gamestate::aiEventActionBuffer.push_back("1");
                    //else if(apci.getGold() >= 50) { gamestate::aiEventActionBuffer.push_back("2"); gm.makeCardChoices(aiEventCardChoice::Remove);}
                    //else { gamestate::aiEventActionBuffer.push_back("3"); }}
        )
    );
    questionEvents.push_back
    (
        new  questionMark(12,"Face Trader", 
            {
                questionChoices([]() {return "Trade : Gain 75 gold. Lose " + to_string(apci.getPlayer().getMaxHealth() * 10 / 100) + " Max HP"; },
                                []() {apci.getPlayer().losePercentageMaxHP(10); apci.gainGold(75); }, []() {return true; }),
                questionChoices([]() {string res = "Touch : 50% Good Mask. 50% Bad Mask"; return res; },
                                []() {
                                            int rng = random_int(1,5);
                                            switch (rng)
                                            {
                                                case 1:
                                                    apci.addRelic(gm.getRelicByID("CultistHeadpiece_ID"));
                                                    break;
                                                case 2:
                                                    apci.addRelic(gm.getRelicByID("FaceOfCleric_ID"));
                                                    break;
                                                case 3:
                                                    apci.addRelic(gm.getRelicByID("GremlinVisage_ID"));
                                                    break;
                                                case 4:
                                                    apci.addRelic(gm.getRelicByID("NlothsHungryFace_ID"));
                                                    break;
                                                case 5:
                                                    apci.addRelic(gm.getRelicByID("SsserpentHead_ID"));
                                                    break;
                                            }
                                        },
                                        []() {return true; }),
                questionChoices([]() {string res = "Leave : Nothing happens"; return res; }, []() {}, []() {return true; })
            }, []() {if (apci.getPlayer().getPercentageHealth() > 0.8) gamestate::aiEventActionBuffer.push_back("1");
                    else { gamestate::aiEventActionBuffer.push_back("2"); }}, true)
    );
    questionEvents.push_back
    (
        new  questionMark(1,"Golden Shrine", 
            {
                questionChoices([]() {string res = "Outrun : Become Cursed - Injury"; return res; },
                                        []() {apci.addRelic(gm.getRelicByID("GoldenIdol_ID")); apci.addCardToDeck(gm.getCurseCardByID("Injury_ID")); }, []() {return true; }),
                questionChoices([]() {return "Smash : Lose " + to_string(apci.getPlayer().getMaxHealth() * 25 / 100) + " HP"; },
                                        []() {apci.addRelic(gm.getRelicByID("GoldenIdol_ID")); apci.getPlayer().eventLosePercentageHP(25); }, []() {return true; }),
                questionChoices([]() {return "Hide : Lose " + to_string(apci.getPlayer().getMaxHealth() * 8 / 100) + " Max HP"; },
                                        []() {apci.addRelic(gm.getRelicByID("GoldenIdol_ID")); apci.getPlayer().losePercentageMaxHP(8); }, []() {return true; })
            }, []() { if(apci.getPlayer().getHealth() > 75) gamestate::aiEventActionBuffer.push_back("3"); 
                        else { gamestate::aiEventActionBuffer.push_back("3"); } }, true)
    );
    questionEvents.push_back
    (
        new  questionMark(1,"Hypnotyzing Coloured Mushrooms", 
            []() { return gm.getFloorIndex() >= 7; },
            {
                questionChoices([]() {string res = "Stomp : Anger the Mushrooms"; return res; },
                                        []() {gm.startMushroomFight(); },[]() {return true; }),
                questionChoices([]() {string res = "Eat : Heal 25% HP. Become Cursed - Parasite"; return res; },
                                        []() {apci.getPlayer().healPercentage(25); apci.addCardToDeck(gm.getCurseCardByID("Parasite_ID")); }, []() {return true; })
            }, []()  {if ( apci.getPlayer().getPercentageHealth() < 0.65 ) gamestate::aiEventActionBuffer.push_back("2");
                    else { gamestate::aiEventActionBuffer.push_back("1"); }})
    );
    questionEvents.push_back
    (
        new  questionMark(1,"Living Wall", 
            {
                questionChoices([]() {string res = "Forget : Remove a card from your deck"; return res; },
                                        []() {gm.removeCardFromDeck(); }, []() {return true; }),
                questionChoices([]() {string res = "Change : Transform a card from your deck"; return res; },
                                        []() {gm.transformCardFromDeck(); }, []() {return true; }),
                questionChoices([]() {string res = "Grow : Upgrade a card from your deck"; return res; },
                                        []() {gm.upgradeCardFromDeck(true); }, []() {return true; })
            }, []() {if (apci.checkForCurse()) { gamestate::aiEventActionBuffer.push_back("1"); gm.makeCardChoices(aiEventCardChoice::Remove); }
                    else { gamestate::aiEventActionBuffer.push_back("3"); gm.makeCardChoices(aiEventCardChoice::Upgrade); }}
        )
    );
    questionEvents.push_back
    (
        new  questionMark(1,"The Ssssserpent",
            {
                questionChoices([]() {string res = "Agree : Gain 175 Gold. Become Cursed - Doubt"; return res; },
                                        []() {apci.gainGold(175); apci.addCardToDeck(gm.getCurseCardByID("Doubt_ID")); }, []() {return true; }),
                questionChoices([]() {string res = "Disagree : Leave"; return res; },
                                        []() {}, []() {return true; })
            }, []() { if (apci.fineForCurse()) gamestate::aiEventActionBuffer.push_back("1");
            else { gamestate::aiEventActionBuffer.push_back("2"); }})
    );
    questionEvents.push_back
    (
        new  questionMark(1,"Shining Light", 
            {
                questionChoices([]() {string res = "Enter : Upgrade 2 random cards. Take 20% of your max HP in damage"; return res; },
                                        []() {gm.upgradeRandomFromDeck(); gm.upgradeRandomFromDeck(); apci.getPlayer().eventLosePercentageHP(20); }, []() {return true; }),
                questionChoices([]() {string res = "Leave : Nothing happens"; return res; },
                                        []() {}, []() {return true; })
            }, []() { if (apci.getPlayer().getPercentageHealth() > 0.55) gamestate::aiEventActionBuffer.push_back("1");
            else { gamestate::aiEventActionBuffer.push_back("2"); }})
    );
    questionEvents.push_back
    (
        new  questionMark(1,"World of Goop", // 
            {
                questionChoices([]() {string res = "Gather Gold : Gain 75 Gold. Lose 11 HP"; return res; },
                                        []() {apci.gainGold(75); apci.getPlayer().eventLoseHP(11); }, []() {return true; }),
                questionChoices([]() {string res = "Leave : Lose some gold"; return res; },
                                        []() {apci.loseGold(random_int(20,50)); }, []() {return true; })
            }, []() { if (apci.getPlayer().getHealth() > 50) gamestate::aiEventActionBuffer.push_back("1");
                    else { gamestate::aiEventActionBuffer.push_back("2"); }}
        )
    );
    questionEvents.push_back
    (
        new  questionMark(1,"Wing Statue", 
            {
                questionChoices([]() {string res = "Pray : Remove a card from your deck. Lose 7 HP"; return res; },
                                        []() {gm.removeCardFromDeck(); }, []() {return true; }),
                questionChoices([]() {string res = "Destroy : Gain 50-80 Gold"; return res; },
                                        []() {apci.gainGold(random_int(50,80)); }, []() {return true; })
            }, []() {gm.qEventWingStatue(); }
                //{ if (apci.getPlayer().getHealth() > 20 && apci.checkForCurse()) { gamestate::aiEventActionBuffer.push_back("1"); gamestate::aiEventActionBuffer.push_back("1"); gm.makeCardChoices(aiEventCardChoice::Remove);}
                //    else { gamestate::aiEventActionBuffer.push_back("2"); }}
        )
    );
    questionEvents.push_back
    (
        new  questionMark(1,"Scrap Ooze", 
            {
                questionChoices([]() {string res = "Reach Inside : Lose 3 HP. 25% chance for relic"; return res; },
                                []() {
                                            int hpLoss{3}, relicChance{25};
                                            apci.getPlayer().eventLoseHP(hpLoss);
                                            if (random_int(1,100) <= relicChance)
                                            {
                                                relic* temp;
                                                temp = random_int(0,1) ? gm.retrieveCommonRelic() : gm.retrieveUncommonRelic();
                                                gamestate::aiEventActionBuffer.clear();
                                                apci.addRelic(temp);
                                                return;
                                            }
                                            ++hpLoss; relicChance += 10;
                                            while (true)
                                            {
                                                std::cout << "1: Reach Inside : Lose " << hpLoss << " HP. " << relicChance << "% chance for relic" << endl;
                                                std::cout << "2: Leave : Nothing happens" << endl;
                                                if (request_input({"1","2"}) == "2") break;
                                                else
                                                {
                                                    apci.getPlayer().eventLoseHP(hpLoss);
                                                    if (random_int(1,100) <= relicChance)
                                                    {
                                                        relic* temp;
                                                        temp = random_int(0,1) ? gm.retrieveCommonRelic() : gm.retrieveUncommonRelic();
                                                        gamestate::aiEventActionBuffer.clear();
                                                        apci.addRelic(temp);
                                                        break;
                                                    }
                                                    ++hpLoss; relicChance += 10;
                                                }
                                            }
                                        },
                                []() {return true; }),
                questionChoices([]() {string res = "Leave : Nothing happens"; return res; },
                                        []() {}, []() {return true; })
            }, []() { if (apci.getPlayer().getHealth() < 32) gamestate::aiEventActionBuffer.push_back("2");
                    else { gamestate::aiEventActionBuffer.push_back("1");
                            if(apci.getPlayer().getHealth() > 35) gamestate::aiEventActionBuffer.push_back("1");
                            if(apci.getPlayer().getHealth() > 39) gamestate::aiEventActionBuffer.push_back("1");
                            if(apci.getPlayer().getHealth() > 45) gamestate::aiEventActionBuffer.push_back("1");
                            if(apci.getPlayer().getHealth() > 52) gamestate::aiEventActionBuffer.push_back("1");
                            gamestate::aiEventActionBuffer.push_back("2");}}
        )
    );
    questionEvents.push_back
    (
        new  questionMark(1, "Dead Adventurer", 
            []() { return gm.getFloorIndex() >= 7; },
            {
                questionChoices([]() {
                                            questionMark::storedValue1 = random_int(0,2);
                                            string res = "Search: Find loot. 25% chance for elite to return (";
                                            switch (questionMark::storedValue1)
                                            {
                                            case 0:
                                                res += "Gremlin Nob)";
                                                break;
                                            case 1:
                                                res += "Lagavulin)";
                                                break;
                                            case 2:
                                                res += "Sentries)";
                                                break;
                                            }
                                            return res; },
                                []() {
                                            int eliteChance{25};
                                            vector<int> loot_{ 0,1,2 };
                                            if (random_int(1,100) <= eliteChance)
                                            {
                                                gm.startDeadAdventurerFight();
                                            }
                                            else
                                            {
                                                eliteChance += 25;
                                                int index = random_int(0, static_cast<int>(loot_.size()) - 1);
                                                switch (loot_[index])
                                                {
                                                case 0:
                                                    apci.gainGold(30);
                                                    loot_.erase(loot_.begin() + index);
                                                    break;
                                                case 1:
                                                    std::cout << "Nothing..." << endl;
                                                    loot_.erase(loot_.begin() + index);
                                                    break;
                                                case 2:
                                                    apci.addRelic(gm.retrieveRandomRelic());
                                                    loot_.erase(loot_.begin() + index);
                                                    break;
                                                }

                                                while (eliteChance < 100)
                                                {
                                                    std::cout << "1: Search: Find loot. " << eliteChance << "% chance for elite to return" << endl;
                                                    std::cout << "2: Escape : End the search and resume your journey" << endl;
                                                    if (questionMark::storedValue1 == 1) gamestate::aiEventActionBuffer.push_back("2");
                                                    else if(eliteChance > 50) gamestate::aiEventActionBuffer.push_back("2");
                                                    else { gamestate::aiEventActionBuffer.push_back("1"); }
                                                    if (request_input({"1","2"}) == "2") return;
                                                    else
                                                    {
                                                        if (random_int(1, 100) <= eliteChance)
                                                        {
                                                            gm.startDeadAdventurerFight();
                                                            break;
                                                        }
                                                        else
                                                        {
                                                            eliteChance += 25;
                                                            index = random_int(0, static_cast<int>(loot_.size()) - 1);
                                                            switch (loot_[index])
                                                            {
                                                            case 0:
                                                                apci.gainGold(30);
                                                                loot_.erase(loot_.begin() + index);
                                                                break;
                                                            case 1:
                                                                std::cout << "Nothing..." << endl;
                                                                loot_.erase(loot_.begin() + index);
                                                                break;
                                                            case 2:
                                                                apci.addRelic(gm.retrieveRandomRelic());
                                                                loot_.erase(loot_.begin() + index);
                                                                break;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                            if (loot_.empty()) return;
                                            int goldReward = random_int(25, 35);
                                            apci.gainGold(gamestate::goldenIdolCheck ? goldReward : goldReward * 5 / 4);
                                            while (!loot_.empty())
                                            {
                                                switch (loot_[0])
                                                {
                                                case 0:
                                                    apci.gainGold(30);
                                                    loot_.erase(loot_.begin());
                                                    break;
                                                case 1:
                                                    loot_.erase(loot_.begin());
                                                    break;
                                                case 2:
                                                    apci.addRelic(gm.retrieveRandomRelic());
                                                    loot_.erase(loot_.begin());
                                                    break;
                                                }
                                            }
                                        },
                                []() {return true; }),
                questionChoices([]() {string res = "Escape : End the search and resume your journey"; return res; },
                                        []() {}, []() {return true; })
            }, []() { if(apci.getPlayer().getPercentageHealth() > 0.7) gamestate::aiEventActionBuffer.push_back("1");
                    else {gamestate::aiEventActionBuffer.push_back("2");} }
        )
    );
}
void gamestate::makeQuestionEventsA2()
{
    for (int i = static_cast<int>(questionEvents.size()) - 1 ; i >= 0; --i)
    {
        if (questionEvents[i]->getAct() == 1)
        {
            delete questionEvents[i];
            questionEvents.erase(questionEvents.begin() + i);
        }
    }
    questionEvents.push_back
    (
        new  questionMark(2,"Ancient Writing", 
            {
                questionChoices([]() {string res = "Elegance : Remove a card from your deck"; return res; },
                                []() { gm.removeCardFromDeck(); }, []() {return true; }),
                questionChoices([]() {string res = "Simplicity : Upgrade all Strikes and Defends"; return res; },
                                []() {for (int i{0}; i < apci.getDeck().size(); ++i) {
                                if (apci.getDeck()[i]->getName() == "Strike" || apci.getDeck()[i]->getName() == "Defend")
                                apci.getDeck()[i]->upgradeThis();}}, []() {return true; })
            }, []() { if(!apci.checkForRelic("Pandora's Box")) gamestate::aiEventActionBuffer.push_back("2");
            else { gamestate::aiEventActionBuffer.push_back("1"); gm.makeCardChoices(aiEventCardChoice::Remove); }}
        )
    );
    questionEvents.push_back
    (
        new  questionMark(2,"Augmentation", 
            {
                questionChoices([]() {string res = "Test J.A.X : Obtain a Jax"; return res; },
                                []() { apci.addCardToDeck(gm.getSpecialCardByID("Jax_ID")); }, []() {return true; }),
                questionChoices([]() {string res = "Become Test Subject : Choose and Transform 2 Cards in your deck"; return res; },
                                []() { gm.transformTwoFromDeck(); }, []() {return true; }),
                questionChoices([]() {string res = "Ingest Mutagens : Obtain Mutagenic Strength"; return res; },
                                []() { apci.addRelic(gm.getRelicByID("MutagenicStrength_ID")); }, []() {return true; })
            }, []() {  gm.qEvent1(); }
        )
    );
    questionEvents.push_back
    (
        new  questionMark(2, "The Colosseum", 
            []() { return gm.getFloorIndex() >= 26; },
            {
                questionChoices([]() {string res = "Fight : You have no choice"; return res; },
                    []() 
                    {
                        gm.startColosseumFight(); 
                        if (ai)
                        {
                            if (apci.getPlayer().getPercentageHealth() < 0.3) return;
                        }
                        else
                        {
                            std::cout << "1: COWARDICE : Escape" << endl;
                            std::cout << "2: VICTORY : A fight with many rewards" << endl;
                            if (request_input({ "1","2" }) == "1") return;
                        }
                        gm.startColosseumFight2();
                    },
                    []() {return true; })
            }, []() { gamestate::aiEventActionBuffer.push_back("1"); }
        )
    );
    questionEvents.push_back
    (
        new  questionMark(2, "Council of Ghosts", 
            {
                questionChoices([]() {string res = "Accept : Recieve 5 Apparitions. Lose 50% Max HP"; return res; },
                                []() {int x{5}; while (--x >= 0) apci.addCardToDeck(gm.getSpecialCardByID("Apparition_ID"));
                                        apci.getPlayer().losePercentageMaxHP(50); }, []() {return true; }),
                questionChoices([]() {string res = "Refuse : Nothing happens"; return res; },
                                []() {}, []() {return true; }),
            }, []() { gm.qEventApparitions(); }
        )
    );
    questionEvents.push_back
    (
        new  questionMark(2, "Cursed Tome", 
            {
                questionChoices([]() {string res = "Read : .."; return res; },
                                []() 
                                {
                                    string s;
                                    std::cout << "Continue : Lose 1 HP.";
                                    if(!ai) cin >> s; 
                                    apci.getPlayer().eventLoseHP(1);
                                    std::cout << "Continue : Lose 2 HP.";
                                    if (!ai) cin >> s;
                                    apci.getPlayer().eventLoseHP(2);
                                    std::cout << "Continue : Lose 3 HP.";
                                    if (!ai) cin >> s;
                                    apci.getPlayer().eventLoseHP(3);

                                    relic* book{ nullptr };
                                    int rng = random_int(1, 3);
                                    switch (rng)
                                    {
                                    case 1:
                                        book = gm.getRelicByID("Enchridion_ID");
                                        break;
                                    case 2:
                                        book = gm.getRelicByID("Necronomicon_ID");
                                        break;
                                    case 3:
                                        book = gm.getRelicByID("NilrysCodex_ID");
                                        break;
                                    }

                                    std::cout << "1. Take : Obtain " << book->getName() << " and lose 10 HP." << endl;
                                    std::cout << "2. Stop :  Lose 3 HP." << endl;
                                    string res = request_input({ "1","2" });
                                    if (res == "1")
                                    {
                                        apci.addRelic(book);
                                        apci.getPlayer().eventLoseHP(10);
                                    }
                                    else
                                    {
                                        apci.getPlayer().eventLoseHP(3);
                                    }
                                }, []() {return true; }),
                questionChoices([]() {string res = "Leave : Nothing happens"; return res; },
                                []() {}, []() {return true; }),
            }, []() {   if (apci.getPlayer().getHealth() > 45) gm.qEvent2();
                        else { gamestate::aiEventActionBuffer.push_back("2"); }}
        )
    );
    questionEvents.push_back
    (                                   
        new  questionMark(23, "Designer In-Spire", []() { return apci.getGold() >= 75; },
            {
                questionChoices([]() 
                    {
                        questionMark::storedValue1 = random_int(0,1);
                        string res;
                        if (questionMark::storedValue1 == 0) { res = "Adjustments : Lose 40 Gold. Upgrade a card"; }
                        else { res = "Adjustments : Lose 40 Gold. Upgrade 2 random cards"; }
                        return res;
                    },[]()
                    { 
                        if (questionMark::storedValue1 == 0) { apci.loseGold(40); gm.upgradeCardFromDeck(); }
                        else {
                            apci.loseGold(40);
                            gm.upgradeRandomFromDeck();
                            gm.upgradeRandomFromDeck();
                        }
                    },[]() { return apci.getGold() >= 40; }),
                questionChoices([]() 
                    {
                        questionMark::storedValue2 = random_int(0,1);
                        string res;
                        if (questionMark::storedValue2 == 0) { res = "Clean up : Lose 60 Gold. Remove a card"; }
                        else { res = "Clean up : Lose 60 Gold. Transform 2 random cards"; }
                        return res; 
                    },[]()
                    {
                        if (questionMark::storedValue2 == 0) { apci.loseGold(60); gm.removeCardFromDeck(); }
                        else {
                            apci.loseGold(60);
                            gm.transformTwoFromDeck();
                        }
                    }, []() {return apci.getGold() >= 60; }),
                questionChoices([]() {string res = "Full Service : Lose 90 Gold. Remove a card then upgrade a random card"; return res; },
                                []() {apci.loseGold(90); gm.removeCardFromDeck(); gm.upgradeRandomFromDeck(); }, []() {return apci.getGold() >= 90; }),
                questionChoices([]() {string res = "Punch : Lose 3 HP"; return res; },
                                []() { apci.getPlayer().eventLoseHP(3); }, []() {return true; }),
            }, []() { if (apci.getGold() >= 90)
                    {
                        gamestate::aiEventActionBuffer.push_back("3");
                        gm.makeCardChoices(aiEventCardChoice::Remove);
                    }
                    else if (apci.getGold() >= 60 && apci.checkForCurse() && questionMark::storedValue2 == 0)
                    {
                        gamestate::aiEventActionBuffer.push_back("2");
                        gm.makeCardChoices(aiEventCardChoice::Remove);
                    }
                    else if (apci.getGold() >= 40)
                    {
                        gamestate::aiEventActionBuffer.push_back("1");
                        if (questionMark::storedValue1 == 0) gm.makeCardChoices(aiEventCardChoice::Upgrade);
                    }
                    else { gamestate::aiEventActionBuffer.push_back("4"); }}
        )
    );
    questionEvents.push_back
    (
        new  questionMark(2,"Forgotten Altar", 
            {
                questionChoices([]() {string res = "Offer Golden Idol : Lose Golden Idol. Gain Bloody Idol"; return res; },
                                []() { apci.removeRelicByName("Golden Idol"); apci.addRelic(gm.getRelicByID("BloodyIdol_ID")); }, []() { return apci.checkForRelic("Golden Idol"); }),
                questionChoices([]() {string res = "Sacrifice : Gain 5 Max HP.Lose 25 % of Max HP"; return res; },
                                []() {apci.getPlayer().gainMaxHP(5); apci.getPlayer().eventLosePercentageHP(25);}, []() {return true; }),
                questionChoices([]() {string res = "Desecrate : Become Cursed - Decay"; return res; },
                                []() {apci.addCardToDeck(gm.getCurseCardByID("Decay_ID")); }, []() {return true; }),
            }, []() { if(apci.checkForRelic("Golden Idol")) gamestate::aiEventActionBuffer.push_back("1");
            else if(apci.getPlayer().getHealth() >= 50 && !gamestate::omamoriCheck) gamestate::aiEventActionBuffer.push_back("2");
            else { gamestate::aiEventActionBuffer.push_back("2"); } }
        )
    );
    questionEvents.push_back
    (                           
        new  questionMark(2, "Knowing Skull", []() {return apci.getPlayer().getHealth() >= 12; },
            {
                questionChoices([]() { string res = "Approach : .."; return res; },[]()
                    {
                        int hpLoss = apci.getPlayer().getHealth() / 10;
                        questionMark::storedValue1 = hpLoss >= 6 ? hpLoss : 6;
                        questionMark::storedValue2 = questionMark::storedValue1;
                        questionMark::storedValue3 = questionMark::storedValue1;
                        questionMark::storedValue4 = questionMark::storedValue1;
                        vector<questionChoices> loopOptions =
                        {
                        questionChoices([]() {string res = "Riches? : Obtain 90 Gold. Lose " + to_string(questionMark::storedValue1) + " HP"; return res; },
                                        []() { apci.gainGold(90); apci.getPlayer().eventLoseHP(questionMark::storedValue1); ++questionMark::storedValue1; },
                                        []() {return (questionMark::storedValue1 + questionMark::storedValue4) < apci.getPlayer().getHealth(); }),
                        questionChoices([]() {string res = "Success? : Obtain a random Uncommon Colorless card. Lose " + to_string(questionMark::storedValue2) + " HP"; return res; },
                                        []() { apci.addCardToDeck(gm.getXRandomColourlessCards(1,Uncommon)[0]); apci.getPlayer().eventLoseHP(questionMark::storedValue2); ++questionMark::storedValue2; },
                                        []() {return (questionMark::storedValue2 + questionMark::storedValue4) < apci.getPlayer().getHealth(); }),
                        questionChoices([]() {string res = "A Pick Me Up? : Obtain a Potion. Lose " + to_string(questionMark::storedValue3) + " HP"; return res; },
                                        []() { gm.potionReward(); apci.getPlayer().eventLoseHP(questionMark::storedValue3); ++questionMark::storedValue3; },
                                        []() {return (questionMark::storedValue3 + questionMark::storedValue4) < apci.getPlayer().getHealth(); }),
                        questionChoices([]() {string res = "How Do I Leave? : Leave. Lose " + to_string(questionMark::storedValue4) + " HP"; return res; },
                                        []() { apci.getPlayer().eventLoseHP(questionMark::storedValue4); },
                                        []() {return true; })
                        };
                        while (true)
                        {
                            int n{ 1 };
                            vector<string> inputs_str;
                            for (int j{ 0 }; j < loopOptions.size(); ++j)
                            {
                                if (loopOptions[j].requirementsCheck())
                                {
                                    std::cout << to_string(n) << ": " << loopOptions[j].textFunction() << "." << endl;
                                    inputs_str.push_back(to_string(n++));
                                }
                                else
                                {
                                    std::cout << "Unavailable: " << endl;// questionOptions[j].textFunction() << "." << endl;
                                    ++n;
                                }
                            }
                            apci.addPotionInputs(inputs_str);
                            std::string result{ request_input(inputs_str) };
                            if (result[0] == 'p') { apci.potionMenu(static_cast<int>(result[1] - 49)); continue; }
                            loopOptions[stoi(result) - 1].eventOutcome();
                            if (stoi(result) - 1 == 3) break;
                        }
                    }, []() {return true; }),
            }, []() { gamestate::aiEventActionBuffer.push_back("1");
                      if (apci.getPlayer().getHealth() > 51) gamestate::aiEventActionBuffer.push_back("1");
                      if (apci.getPlayer().getHealth() > 58)
                      {
                          if(!apci.potionsFull()) gamestate::aiEventActionBuffer.push_back("3");
                          else { gamestate::aiEventActionBuffer.push_back("1"); }
                      }
                      if (apci.getPlayer().getHealth() > 66) gamestate::aiEventActionBuffer.push_back("1");
                      gamestate::aiEventActionBuffer.push_back("4"); }, true)
    );
    questionEvents.push_back
    (               // either
        new  questionMark(2,"The Joust", []() {return apci.getGold() >= 50; },
            {
                questionChoices([]() {string res = "Murderer : Bet 50 Gold - 70% to win 100 Gold"; return res; },
                                []() { apci.loseGold(50); if (random_int(1,10) <= 7) apci.gainGold(100); }, []() {return true; }),
                questionChoices([]() {string res = "Murderer : Bet 50 Gold - 30% to win 250 Gold"; return res; },
                                []() { apci.loseGold(50); if (random_int(1,10) <= 3) apci.gainGold(250); }, []() {return true; }),
            }, []() { gamestate::aiEventActionBuffer.push_back("1"); }, true)
    );
    questionEvents.push_back
    (                   // if < 20% HP 2 else 1 and choose
        new  questionMark(2, "The Library",
            {
                questionChoices([]() {string res = "Read : Choose 1 of 20 cards to add to your deck"; return res; },
                                []() { gm.libraryCardOpts(); }, []() {return true; }),
                questionChoices([]() {string res = "Sleep : Heal 33% of your max HP"; return res; },
                                []() { apci.getPlayer().healPercentage(33); }, []() {return true; }),
            }, []() {if(apci.getPlayer().getPercentageHealth() < 0.6 ) gamestate::aiEventActionBuffer.push_back("2");
                    else { gamestate::aiEventActionBuffer.push_back("1"); } }
        )
    );
    questionEvents.push_back
    (                   // 1 if curse ??
        new  questionMark(2,"Old Beggar", []() {return apci.getGold() >= 75; },
            {
                questionChoices([]() {string res = "Offer Gold : Lose 75 Gold. Remove a card from your deck"; return res; },
                                []() { apci.loseGold(75); gm.removeCardFromDeck(); }, []() {return apci.getGold() >= 75; }),
                questionChoices([]() {string res = "Leave : Nothing happens"; return res; },
                                []() {}, []() {return true; }),
            }, []() { gamestate::aiEventActionBuffer.push_back("1"); gm.makeCardChoices(aiEventCardChoice::Remove); }
        )
    );
    questionEvents.push_back
    (
        new  questionMark(2,"The Mausoleum", // 2
            {
                questionChoices([]() {string res = "Open Coffin : Obtain a relic. 50 % to Become Cursed - Writhe"; return res; },
                                []() { apci.addRelic(gm.retrieveRandomRelic()); if (random_int(0,1)) apci.addCardToDeck(gm.getCurseCardByID("Writhe_ID")); }, []() {return true; }),
                questionChoices([]() {string res = "Leave : Nothing happens"; return res; },
                                []() {}, []() {return true; }),
            }, []() { gamestate::aiEventActionBuffer.push_back("1"); }
        )
    );
    questionEvents.push_back
    (
        new  questionMark(2, "Masked Bandits", // sim fight if good 2
            {
                questionChoices([]() {string res = "Pay : Lose all your Gold. Escape"; return res; },
                                []() {apci.loseGold(apci.getGold()); }, []() {return true; }),
                questionChoices([]() {string res = "Fight : Stand your ground"; return res; },
                                []() {gm.startBanditFight(); }, []() {return true; }),
            }, []() { if( apci.getPlayer().getPercentageHealth() > 0.6) gamestate::aiEventActionBuffer.push_back("2"); 
            else { gamestate::aiEventActionBuffer.push_back("1"); }}
        )
    );
    questionEvents.push_back
    (
        new  questionMark(2, "The Nest", // 1
            {
                questionChoices([]() {string res = "Smash and Grab : Obtain 99 Gold"; return res; },
                                []() { apci.gainGold(99); }, []() {return true; }),
                questionChoices([]() {string res = "Stay in line : Obtain Ritual Dagger. Lose 6 HP"; return res; },
                                []() {apci.addCardToDeck(gm.getSpecialCardByID("RitualDagger_ID")); apci.getPlayer().eventLoseHP(6); }, []() {return true; }),
            }, []() { gamestate::aiEventActionBuffer.push_back("1"); }
        )
    );
    questionEvents.push_back
    (
        new  questionMark(2, "Nloth", []() {return apci.getRelics().size() >= 2; }, // complicated, probably just leave
            {
                questionChoices([]() 
                    {
                        questionMark::storedValue1 = random_int(0,static_cast<int>(apci.getRelics().size()) - 1);
                        string res = "Offer : Lose " + apci.getRelics()[questionMark::storedValue1]->getName() + ". Gain Nloth's Gift";
                        return res; 
                    },
                                []() { apci.loseRelicByIndex(questionMark::storedValue1); apci.addRelic(gm.getRelicByID("NlothsGift_ID")); }, []() {return true; }),
                questionChoices([]()
                    {
                        questionMark::storedValue2 = random_intNot(0,static_cast<int>(apci.getRelics().size()) - 1,questionMark::storedValue1);
                        string res = "Offer : Lose " + apci.getRelics()[questionMark::storedValue2]->getName() + ". Gain Nloth's Gift";
                        return res;
                    },
                                []() { apci.loseRelicByIndex(questionMark::storedValue2); apci.addRelic(gm.getRelicByID("NlothsGift_ID")); }, []() {return true; }),
                questionChoices([]() {string res = "Leave : Nothing happens"; return res; },
                                []() {}, []() { return true; }),
            }, []() { 
                        if (find(relic::IDsForNloth.begin(), relic::IDsForNloth.end(),apci.getRelics()[questionMark::storedValue1]->getRelicIDNum())!= relic::IDsForNloth.end()) gamestate::aiEventActionBuffer.push_back("1"); 
                        else if (find(relic::IDsForNloth.begin(), relic::IDsForNloth.end(), apci.getRelics()[questionMark::storedValue2]->getRelicIDNum()) != relic::IDsForNloth.end()) gamestate::aiEventActionBuffer.push_back("2");
                        else { gamestate::aiEventActionBuffer.push_back("3"); }
                    }, true)
    );
    questionEvents.push_back
    (
        new  questionMark(2,"Pleading Vagrant", // 1 if gold, else 3
            {
                questionChoices([]() {string res = "Give 85 Gold : Obtain a random Relic"; return res; },
                                []() { apci.loseGold(85); apci.addRelic(gm.retrieveRandomRelic()); }, []() {return apci.getGold() >= 85; }),
                questionChoices([]() {string res = "Rob : Obtain a random Relic. Become Cursed - Shame"; return res; },
                                []() { apci.addCardToDeck(gm.getCurseCardByID("Shame_ID")); apci.addRelic(gm.retrieveRandomRelic()); }, []() {return true; }),
                questionChoices([]() {string res = "Leave : Nothing happens"; return res; },
                                []() {}, []() {return true; }),
            }, []() { if (apci.getGold() >= 85) gamestate::aiEventActionBuffer.push_back("1");
            else if (apci.fineForCurse()) { gamestate::aiEventActionBuffer.push_back("2"); }
            else { gamestate::aiEventActionBuffer.push_back("3"); }}
        )
    );
    questionEvents.push_back
    (
        new  questionMark(2,"Vampires", // complicated
            {
                questionChoices([]() {string res = "Offer Blood Vial : Lose Blood Vial. Remove all Strikes. Receive 5 Bites"; return res; },
                                []()    {
                                            apci.removeRelicByName("Blood Vial"); 
                                            auto toTrans = find_if(apci.getDeck().begin(), apci.getDeck().end(), [](const unique_ptr<card>& c_) {return c_->getCardIDNum() == 1; });
                                            while (toTrans != apci.getDeck().end())
                                            {
                                                apci.removeCard(toTrans,true);
                                                toTrans = find_if(apci.getDeck().begin(), apci.getDeck().end(), [](const unique_ptr<card>& c_) {return c_->getCardIDNum() == 1; });
                                            }
                                            int x{ 5 };
                                            while (--x >= 0)
                                            {
                                                apci.addCardToDeck(gm.getSpecialCardByID("Bite_ID"));
                                            }
                                        }, []() { return apci.checkForRelic("Blood Vial"); }),
                questionChoices([]() {string res = "Accept : Remove all Strikes. Receive 5 Bites. Lose 30% Max HP"; return res; },
                                []() {      auto toTrans = find_if(apci.getDeck().begin(), apci.getDeck().end(), [](const unique_ptr<card>& c_) {return c_->getCardIDNum() == 1; });
                                            while (toTrans != apci.getDeck().end())
                                            {
                                                apci.removeCard(toTrans,true);
                                                toTrans = find_if(apci.getDeck().begin(), apci.getDeck().end(), [](const unique_ptr<card>& c_) {return c_->getCardIDNum() == 1; });
                                            }
                                            int x{ 5 };
                                            while (--x >= 0)
                                            {
                                                apci.addCardToDeck(gm.getSpecialCardByID("Bite_ID"));
                                            }
                                            apci.getPlayer().losePercentageMaxHP(30);
                    }, []() {return true; }),
                questionChoices([]() {string res = "Refuse : Nothing happens"; return res; },
                                []() {}, []() {return true; }),
            }, []() { if(apci.checkForRelic("Blood Vial")) gamestate::aiEventActionBuffer.push_back("1");
                      else { gm.qEvent3(); }}
        )
    );
}
void gamestate::makeQuestionEventsA3()
{
    for (int i = static_cast<int>(questionEvents.size()) - 1; i >= 0; --i)
    {
        if (questionEvents[i]->getAct() == 2 || questionEvents[i]->getAct() == 12)
        {
            delete questionEvents[i];
            questionEvents.erase(questionEvents.begin() + i);
        }
    }
    questionEvents.push_back
    (
        new  questionMark(3, "Falling", 
            []() { return apci.getDeck().size()>=3 && apci.altToBasicCurseBottled(); },
            {
                questionChoices([]() {
                                        vector<int> validOpts;
                                        for (int i{ 0 }; i < apci.getDeck().size(); ++i)
                                        {
                                            if (apci.getDeck()[i]->getType() != Skill || apci.getDeck()[i]->getBottled()) continue;
                                            validOpts.push_back(i);
                                        }
                                        questionMark::storedValue1 = validOpts[random_int(0, static_cast<int>(validOpts.size()) - 1)];
                                        string res = "Land : Lose " + apci.getDeck()[questionMark::storedValue1]->getName();
                                        return res; },
                                []() {  apci.removeCard(apci.getDeck().begin() + questionMark::storedValue1); },
                                []() {  for (int i{ 0 }; i < apci.getDeck().size(); ++i)
                                        {
                                            if (apci.getDeck()[i]->getType() == Skill && !apci.getDeck()[i]->getBottled()) return true;
                                        }
                                        return false;
                                     }),
                questionChoices([]() {
                                        vector<int> validOpts;
                                        for (int i{ 0 }; i < apci.getDeck().size(); ++i)
                                        {
                                            if (apci.getDeck()[i]->getType() != Power || apci.getDeck()[i]->getBottled()) continue;
                                            validOpts.push_back(i);
                                        }
                                        questionMark::storedValue2 = validOpts[random_int(0, static_cast<int>(validOpts.size()) - 1)];
                                        string res = "Channel : Lose " + apci.getDeck()[questionMark::storedValue2]->getName();
                                        return res; },
                                []() {  apci.removeCard(apci.getDeck().begin() + questionMark::storedValue2); },
                                []() {  for (int i{ 0 }; i < apci.getDeck().size(); ++i)
                                        {
                                            if (apci.getDeck()[i]->getType() == Power && !apci.getDeck()[i]->getBottled()) return true;
                                        }
                                        return false;
                                     }),
                questionChoices([]() {
                                        vector<int> validOpts;
                                        for (int i{ 0 }; i < apci.getDeck().size(); ++i)
                                        {
                                            if (apci.getDeck()[i]->getType() != Attack || apci.getDeck()[i]->getBottled()) continue;
                                            validOpts.push_back(i);
                                        }
                                        questionMark::storedValue3 = validOpts[random_int(0, static_cast<int>(validOpts.size()) - 1)];
                                        string res = "Strike : Lose " + apci.getDeck()[questionMark::storedValue3]->getName();
                                        return res; },
                                []() {  apci.removeCard(apci.getDeck().begin() + questionMark::storedValue3); apci.addRelic(gm.retrieveRandomRelic()); },
                                []() {  for (int i{ 0 }; i < apci.getDeck().size(); ++i)
                                        {
                                            if (apci.getDeck()[i]->getType() == Attack && !apci.getDeck()[i]->getBottled()) return true;
                                        }
                                        return false;
                                     })
            }, []() {gm.qEventFalling(); })
    );
    questionEvents.push_back
    (
        new  questionMark(3, "Mind Bloom",
            {
                questionChoices([]() {  string res = "I am War : Fight a Boss from Act 1. Obtain a Rare Relic and 50 Gold"; return res; },
                                []() {  gm.startAct1BossFight(); },
                                []() {  return true; }),
                questionChoices([]() {  string res = "I am Awake : Upgrade all cards. Obtain Mark of the Bloom"; return res; },
                                []() {  for (int i{0}; i < apci.getDeck().size(); ++i)
                                        {
                                            if(apci.getDeck()[i]->getUpgraded() || apci.getDeck()[i]->getType() == Curse) continue;
                                            apci.getDeck()[i]->upgradeThis();
                                        }
                                        apci.addRelic(gm.getRelicByID("MarkOfTheBloom_ID"));
                                     },
                                []() {  return true; }),
                questionChoices([]() {  
                                        string res;
                                        if (gm.getFloorIndex() >= 41) res = "I am Healthy : Heal to full. Become Cursed - Doubt";
                                        else { res = "I am Rich : Gain 999 Gold. Become Cursed - 2 Normality"; }
                                        return res; 
                                     },
                                []() {
                                        if (gm.getFloorIndex() >= 41)
                                        {
                                            apci.getPlayer().healToFull();
                                            apci.addCardToDeck(gm.getCurseCardByID("Doubt_ID"));
                                        }
                                        else
                                        {
                                            apci.gainGold(999);
                                            apci.addCardToDeck(gm.getCurseCardByID("Normality_ID"));
                                            apci.addCardToDeck(gm.getCurseCardByID("Normality_ID"));
                                        }
                                     },
                                []() {  return true; }),
            }, []() { if (apci.getDeck().size() > 30 && apci.getPlayer().getHealth() > 80) gamestate::aiEventActionBuffer.push_back("2");
                    else if(apci.getPlayer().getPercentageHealth() < 0.2) gamestate::aiEventActionBuffer.push_back("3"); 
                    else { gamestate::aiEventActionBuffer.push_back("1"); }})
    );
    questionEvents.push_back
    (
        new  questionMark(3, "The Moai Head",
            []() { return apci.checkForRelic("Golden Idol") || apci.getPlayer().getHealth() < apci.getPlayer().getMaxHealth() /2; },
            {
                questionChoices([]() {  string res = "Jump Inside : Heal to full. Lose 13% Max HP"; return res; },
                                []() {  apci.getPlayer().healToFull(); apci.getPlayer().losePercentageMaxHP(13); },
                                []() {  return true; }),
                questionChoices([]() {  string res = "Offer Golden Idol : Gain 333 Gold. Lose Golden Idol"; return res; },
                                []() {  apci.removeRelicByName("Golden Idol"); apci.gainGold(333); },
                                []() {  return apci.checkForRelic("Golden Idol"); }),
                questionChoices([]() {  string res = "Leave : Nothing happens"; return res; },
                                []() {}, []() { return true; })
            }, []() {
                if(apci.checkForRelic("Golden Idol")) gamestate::aiEventActionBuffer.push_back("2");
                else if (apci.getPlayer().getHealth() < 50) { gamestate::aiEventActionBuffer.push_back("1"); }
                else { gamestate::aiEventActionBuffer.push_back("3"); } 
            })
    );
    questionEvents.push_back
    (
        new  questionMark(3, "Sensory Stone",
            {
                questionChoices([]() {  string res = "Recall : Add 1 Colourless Card to your deck"; return res; },
                                []() {  gm.colourlessReward(); },
                                []() {  return true; }),
                questionChoices([]() {  string res = "Recall : Add 2 Colourless Card to your deck. Lose 5 HP"; return res; },
                                []() {  apci.getPlayer().eventLoseHP(5); gm.colourlessReward(); gm.colourlessReward(); },
                                []() {  return true; }),
                questionChoices([]() {  string res = "Recall : Add 3 Colourless Card to your deck. Lose 10 HP"; return res; },
                                []() {  apci.getPlayer().eventLoseHP(10); gm.colourlessReward(); gm.colourlessReward(); gm.colourlessReward(); },
                                []() {  return true; })
            }, []() {gamestate::aiEventActionBuffer.push_back("1"); })
    );
    questionEvents.push_back
    (
        new  questionMark(3, "Tomb of Lord Red Mask",
            {
                questionChoices([]() {  
                                        string res;
                                        if(apci.checkForRelic("Red Mask")) res = "Don the Red Mask : Gain 222 Gold";
                                        else { res = "Offer Gold : Lose all Gold. Obtain the Red Mask"; }
                                        return res;
                                     },
                                []() {  if (apci.checkForRelic("Red Mask")) { apci.removeRelicByName("Red Mask"); apci.gainGold(222); }
                                        else { apci.loseGold(apci.getGold()); apci.addRelic(gm.getRelicByID("RedMask_ID")); }
                                     },
                                []() {  return true; }),
                questionChoices([]() {  string res = "Leave : Nothing happens"; return res; }, []() {}, []() { return true; })
            }, []() {
                        if (apci.checkForRelic("Red Mask")) gamestate::aiEventActionBuffer.push_back("1");
                        else if (apci.getGold() < 50) { gamestate::aiEventActionBuffer.push_back("1"); }
                        else { gamestate::aiEventActionBuffer.push_back("2"); } 
                    })
    );
    questionEvents.push_back
    (
        new  questionMark(3, "Mysterious Sphere",
            {
                questionChoices([]() {  string res = "Open Sphere : Fight two Orb Walkers. Obtain a Rare Relic"; return res;},
                                []() {  gm.startOrbFight(); },
                                []() {  return true; }),
                questionChoices([]() {  string res = "Leave : Nothing happens"; return res; }, []() {}, []() { return true; })
            }, []() {if (apci.getPlayer().getPercentageHealth() < 0.55) gamestate::aiEventActionBuffer.push_back("2");
            else { gamestate::aiEventActionBuffer.push_back("1"); } })
    );
    questionEvents.push_back
    (
        new  questionMark(3, "Winding Halls",
            {
                questionChoices([]() {  string res = "Embrace Madness : Receive 2 Madness. Lose 13% Max HP"; return res; },
                                []() {  apci.addCardToDeck(gm.getColourlessCardByID("Madness_ID"));
                                        apci.addCardToDeck(gm.getColourlessCardByID("Madness_ID"));
                                        apci.getPlayer().losePercentageMaxHP(13);
                                     },
                                []() {  return true; }),
                questionChoices([]() {  string res = "Press On : Become Cursed - Writhe. Heal 25% of Max HP."; return res; },
                                []() {  apci.addCardToDeck(gm.getCurseCardByID("Writhe_ID")); apci.getPlayer().healPercentage(25); }, []() { return true; }),
                questionChoices([]() {  string res = "Retrace Your Steps : Lose 5% Max HP"; return res; },
                                []() {  apci.getPlayer().eventLosePercentageHP(5); }, []() { return true; })
            }, []() {if (apci.getPlayer().getPercentageHealth() < 0.25) gamestate::aiEventActionBuffer.push_back("2");
            else { gamestate::aiEventActionBuffer.push_back("3"); } })
    );
    questionEvents.push_back
    (
        new  questionMark(3, "Secret Portal",
            {
                questionChoices([]() {  string res = "Immediately travel to boss"; return res; },
                                []() {  gm.teleportToBoss(); },[]() {  return true; }),
                questionChoices([]() {  string res = "Leave : Nothing happens"; return res; }, []() {}, []() { return true; })
            }, []() {gamestate::aiEventActionBuffer.push_back("2"); }, true)
    );
}

events* gamestate::makeNextEvent(char c)
{
    gm.callNewFloorEntered();
    switch (c)
    {
    case 'N':
        return new neowEvent;
    case 'e':
        if ((easyDone >= 3 && actNumber == 1 )
            || (easyDone >= 2 && actNumber == 2)
            || (easyDone >= 2 && actNumber == 3)) return events_hh;
        else { ++easyDone; return events_eh; }
    case 'E':
        return events_el;
    case 'B':
        return events_bo;
    case 'c':
        return events_ca;
    case 's':
        return new shop;
    case 'C':
        return new chest;
    case '?':
        if (juzuBraceletCheck) enemyChance = 0;
        gm.callQuesitonEntered();
        if (mapPath[mapPathIndex] == 'C') return new chest;
        if (tinyChestCheck) { if (random_int(1, 4) == 1) return new chest; }
        char res = calcOdds({ 'e','C','s','?' }, { enemyChance,chestChance,shopChance,100 - enemyChance - shopChance - chestChance }, { 10,10,1,10 }, mapPathIndex);
        switch (res)
        {
        case 'e':
            chestChance += 2;
            shopChance += 3;
            enemyChance = 10;
            mapPath[mapPathIndex] = 'e';
            if ((easyDone >= 3 && actNumber == 1)
                || (easyDone >= 2 && actNumber == 2)
                || (easyDone >= 2 && actNumber == 3)) return events_hh;
            else { ++easyDone; return events_eh; }
        case 'C':
            enemyChance += 10;
            shopChance += 3;
            chestChance = 2;
            mapPath[mapPathIndex] = 'C';
            return new chest;
        case 's':
            enemyChance += 10;
            chestChance += 2;
            shopChance = 3;
            mapPath[mapPathIndex] = 's';
            return new shop;
        case '?':
            enemyChance += 10;
            shopChance += 3; 
            chestChance += 2;
            mapPath[mapPathIndex] = '?';
            return chooseNextQuestionEvent();
        case ' ':
            enemyChance += 10;
            shopChance += 3;
            chestChance += 2;
            mapPath[mapPathIndex] = '?';
            return chooseNextQuestionEvent();
        }
    }
    throw runtime_error("event error");
}
questionMark* gamestate::chooseNextQuestionEvent()
{
    questionMark::storedValue1 = -1;
    questionMark::storedValue2 = -1;
    questionMark::storedValue3 = -1;
    questionMark::storedValue4 = -1;
    bool _shrine = random_int(1, 100) <= shrineChance;
    vector<questionMark*> potentials_;
    for (int i{ 0 }; i < questionEvents.size(); ++i)
    {
        if (questionEvents[i]->enterRequirement() && questionEvents[i]->getShrine() == _shrine) potentials_.push_back(questionEvents[i]);
    }
    if (potentials_.size() == 0)
    {
        _shrine = !_shrine;
        for (int i{ 0 }; i < questionEvents.size(); ++i)
        {
            if (questionEvents[i]->enterRequirement() && questionEvents[i]->getShrine() == _shrine) potentials_.push_back(questionEvents[i]);
        }
    }
    questionMark* ev_ = potentials_[random_int(0, static_cast<int>(potentials_.size()) - 1)];
    for (int i{ 0 }; i < questionEvents.size(); ++i)
    {
        if (ev_ == questionEvents[i]) questionEvents.erase(questionEvents.begin() + i);
    }
    return ev_;
}
relic* gamestate::retrieveRandomRelic()
{
    int rng = random_int(0, 2);
    switch (rng)
    {
    case 0: return gm.retrieveCommonRelic();
    case 1: return gm.retrieveUncommonRelic();
    case 2: return gm.retrieveRareRelic();
    }
    throw runtime_error("retranrel");
}
card* gamestate::getCurse()
{
    return gm.getCurseCardByID(curseCards[random_int(0, static_cast<int>(curseCards.size()) - 1)])->clone();
}
card* gamestate::getCardOfRarity(cardRarity cr)
{
    switch (cr)
    {
    case cardRarity::Common: return gm.getCardByID(commonCards[random_int(0, static_cast<int>(commonCards.size()) - 1)])->clone();
    case cardRarity::Uncommon: return gm.getCardByID(uncommonCards[random_int(0, static_cast<int>(uncommonCards.size()) - 1)])->clone();
    case cardRarity::Rare: return gm.getCardByID(rareCards[random_int(0, static_cast<int>(rareCards.size()) - 1)])->clone();
    }
    throw runtime_error("gcardofrar");
}
C_DATABASE::iterator gamestate::getCardReward(vector<C_DATABASE::iterator> notIncluding)
{
    cardRarity chosenRarity;
    vector<C_DATABASE::iterator> cardOptions;
    cardOptions.clear();
    int tempCommon{ common_pc }, tempUncommon{ uncommon_pc }, tempRare{ rare_pc };
    if (nlothsGiftCheck) tempRare *= 3;
    tempRare += rare_offset;
    if (tempRare < 0)
    {
        tempUncommon += tempRare;
        tempRare = 0;
    }
    tempCommon = 100 - tempUncommon - tempRare;
    int res = random_int(1, 100);

    if (res >= 1 && res <= tempCommon)
    {
        chosenRarity = cardRarity::Common;
        rare_offset += 1;
    }
    else if (res >= tempCommon + 1 && res <= tempCommon + tempUncommon)
    {
        chosenRarity = cardRarity::Uncommon;
    }
    else { chosenRarity = cardRarity::Rare; rare_offset = -5; }

    for (auto itr{ card_database.begin() }; itr != card_database.end(); ++itr)
    {
        if (chosenRarity == itr->second->getRarity())
        {
            if (chosenRarity == cardRarity::Common && (itr->second->getName() == "Bash" ||
                itr->second->getName() == "Strike" ||
                itr->second->getName() == "Defend")) continue;
            if (std::find(notIncluding.begin(), notIncluding.end(), itr) != notIncluding.end()) continue;
            cardOptions.push_back(itr);
        }
    }
    return cardOptions[random_int(0, static_cast<int>(cardOptions.size()) - 1)];
}
C_DATABASE::iterator gamestate::getCardReward(vector<C_DATABASE::iterator> notIncluding, cardType ct)
{
    cardRarity chosenRarity;
    vector<C_DATABASE::iterator> cardOptions;
    cardOptions.clear();
    int res = random_int(1, 100);

    if (res >= 1 && res <= common_pc) chosenRarity = cardRarity::Common;
    else if (res >= common_pc + 1 && res <= common_pc + uncommon_pc) chosenRarity = cardRarity::Uncommon;
    else { chosenRarity = cardRarity::Rare; }

    if (ct == cardType::Power && chosenRarity == cardRarity::Common) { chosenRarity = random_int(1, 3) == 1 ? cardRarity::Rare : cardRarity::Uncommon; }
    for (auto itr{ card_database.begin() }; itr != card_database.end(); ++itr)
    {
        if (chosenRarity == itr->second->getRarity())
        {
            if (ct != cardType::Status) { if (itr->second->getType() != ct) continue; }
            if (chosenRarity == cardRarity::Common && (itr->second->getName() == "Bash" ||
                itr->second->getName() == "Strike" ||
                itr->second->getName() == "Defend")) continue;
            if (std::find(notIncluding.begin(), notIncluding.end(), itr) != notIncluding.end()) continue;
            cardOptions.push_back(itr);
        }
    }
    return cardOptions[random_int(0, static_cast<int>(cardOptions.size()) - 1)];
}
C_DATABASE::iterator gamestate::getColourlessCardReward(vector<C_DATABASE::iterator> notIncluding, cardRarity cr)
{
    vector<C_DATABASE::iterator> cardOptions;
    cardOptions.clear();
    for (auto itr{ card_database_clr.begin() }; itr != card_database_clr.end(); ++itr)
    {
        if (std::find(notIncluding.begin(), notIncluding.end(), itr) != notIncluding.end()) continue;
        if (cr != cardRarity::Other && cr != itr->second->getRarity()) continue;
        cardOptions.push_back(itr);
    }
    return cardOptions[random_int(0, static_cast<int>(cardOptions.size()) - 1)];
}
void gamestate::colourlessReward(cardRarity cr)
{
    aiCardChoices.clear();
    vector<bool> upgradedCards;
    for (int i{ 0 }; i < cardRewardNumber; ++i)
    {
        upgradedCards.push_back(random_int(1, 100) <= upgraded_pc);
    }
    while (true)
    {
        vector<decltype(card_database.begin())> cardOptions;
        vector<decltype(card_database.begin())> finalOptions;
        for (int n{ 0 }; n < cardRewardNumber; ++n)
        {
            finalOptions.push_back(getColourlessCardReward(finalOptions,cr));
        }
        vector<string> validInputs{ "skip" };
        std::cout << "skip or ";
        for (int i{ 1 }; i < finalOptions.size() + 1; ++i)
        {
            validInputs.push_back(to_string(i));
            if (!ai)
            {
                std::cout << i << " for " << finalOptions[i - 1]->second->getName();
                if (upgradedCards[i - 1]) std::cout << "+";
                std::cout << " (" << finalOptions[i - 1]->second->getRarity() << ")" << ". ";
            }
            else
            {
                aiCardChoices.push_back(finalOptions[i - 1]);
            }
        }
        if (!ai) apci.addPotionInputs(validInputs);
        if (ai) gm.makeCardChoices(aiEventCardChoice::Add);
        string inputChoice = request_input(validInputs);
        if (inputChoice[0] == 'p') { apci.potionMenu(static_cast<int>(inputChoice[1]) - 49); continue; }
        if (inputChoice == "skip") { if (singingBowlCheck) { apci.getPlayer().gainMaxHP(2); } return; }
        for (int i{ 1 }; i < validInputs.size(); ++i)
        {
            if (validInputs[i] == inputChoice)
            {
                apci.addCardToDeck(finalOptions[i - 1], upgradedCards[i - 1]);
                break;
            }
        }
        break;
    }
}
void gamestate::hallwayReward()
{
    int goldReward = random_int(10, 20);
    apci.gainGold(goldenIdolCheck ? goldReward : goldReward * 5 / 4);
    if(!ai) prelude();
    potionReward();
    hallwayReward(true);
    if (prayerWheelCheck) hallwayReward(true);
}
void gamestate::hallwayReward(bool nogold)
{
    aiCardChoices.clear();
    vector<bool> upgradedCards;
    for (int i{ 0 }; i < cardRewardNumber; ++i)
    {
        upgradedCards.push_back(random_int(1, 100) <= upgraded_pc);
    }
    while (true)
    {
        vector<decltype(card_database.begin())> cardOptions;
        vector<decltype(card_database.begin())> finalOptions;
        for (int n{ 0 }; n < cardRewardNumber; ++n)
        {
            finalOptions.push_back(getCardReward(finalOptions));
        }
        vector<string> validInputs{ "skip" };
        if (!ai) std::cout << "skip or ";
        for (int i{ 1 }; i < finalOptions.size() + 1; ++i)
        {
            if (!ai)
            {
                std::cout << i << " for " << finalOptions[i - 1]->second->getName();
                if (upgradedCards[i - 1]) std::cout << "+";
                std::cout << " (" << finalOptions[i - 1]->second->getRarity() << ")" << ". ";
            }
            else
            {
                /*cout << i << " for " << finalOptions[i - 1]->second->getName();
                if (upgradedCards[i - 1]) std::cout << "+";
                std::cout << " (" << finalOptions[i - 1]->second->getRarity() << ")" << ". ";*/
                aiCardChoices.push_back(finalOptions[i - 1]);
            }
            validInputs.push_back(to_string(i));
        }
        if (!ai) apci.addPotionInputs(validInputs);
        if (ai) gm.makeCardChoices(aiEventCardChoice::Add);
        string inputChoice = request_input(validInputs);
        if (inputChoice[0] == 'p') { apci.potionMenu(static_cast<int>(inputChoice[1]) - 49); continue; }
        if (inputChoice == "skip") { if (singingBowlCheck) { apci.getPlayer().gainMaxHP(2); } return; }
        for (int i{ 1 }; i < validInputs.size(); ++i)
        {
            if (validInputs[i] == inputChoice)
            {
                apci.addCardToDeck(finalOptions[i - 1],upgradedCards[i-1]);
                break;
            }
        }
        break;
    }
    nogold;
}
void gamestate::eliteRelicReward()
{
    apci.addRelic(gm.retrieveRandomRelic());
}
void gamestate::eliteReward()
{
    int goldReward = random_int(25, 35);
    apci.gainGold(goldenIdolCheck ? goldReward : goldReward * 5 / 4);
    if(!ai) prelude();
    potionReward();
    eliteRelicReward();
    eliteReward(true);
    if (prayerWheelCheck) eliteReward(true);
}
void gamestate::eliteReward(bool nogold)
{
    aiCardChoices.clear();
    cardRarity chosenRarity;
    vector<decltype(card_database.begin())> cardOptions;
    vector<decltype(card_database.begin())> finalOptions;
    for (int n{ 0 }; n < cardRewardNumber; ++n)
    {
        cardOptions.clear();

        int tempCommon{ common_pc_elite }, tempUncommon{ uncommon_pc_elite }, tempRare{ rare_pc_elite };
        if (nlothsGiftCheck) tempRare *= 3;
        tempRare += rare_offset;
        if (tempRare < 0)
        {
            tempUncommon += tempRare;
            tempRare = 0;
        }
        tempCommon = 100 - tempUncommon - tempRare;
        int res = random_int(1, 100);

        if (res >= 1 && res <= tempCommon)
        {
            chosenRarity = cardRarity::Common;
            rare_offset += 1;
        }
        else if (res >= tempCommon + 1 && res <= tempCommon + tempUncommon)
        {
            chosenRarity = cardRarity::Uncommon;
        }
        else { chosenRarity = cardRarity::Rare; rare_offset = -5; }


        for (auto itr{ card_database.begin() }; itr != card_database.end(); ++itr)
        {
            if (chosenRarity == itr->second->getRarity())
            {
                if (chosenRarity == cardRarity::Common && (itr->second->getName() == "Bash" ||
                    itr->second->getName() == "Strike" ||
                    itr->second->getName() == "Defend")) continue;
                if (std::find(finalOptions.begin(), finalOptions.end(), itr) != finalOptions.end()) continue;
                cardOptions.push_back(itr);
            }
        }
        finalOptions.push_back(cardOptions[random_int(0, static_cast<int>(cardOptions.size()) - 1)]);
    }
    vector<bool> upgradedCards;
    for (int i{ 0 }; i < cardRewardNumber; ++i)
    {
        upgradedCards.push_back(random_int(1, 100) <= upgraded_pc);
    } 
    while (true)
    {
        vector<string> validInputs{ "skip" };
        std::cout << "skip or ";
        for (int i{ 1 }; i < finalOptions.size() + 1; ++i)
        {
            if (!ai)
            {
                std::cout << i << " for " << finalOptions[i - 1]->second->getName();
                if (upgradedCards[i - 1]) std::cout << "+";
                std::cout << " (" << finalOptions[i - 1]->second->getRarity() << ")" << ". ";
            }
            else
            {
                /*cout << i << " for " << finalOptions[i - 1]->second->getName();
                if (upgradedCards[i - 1]) std::cout << "+";
                std::cout << " (" << finalOptions[i - 1]->second->getRarity() << ")" << ". ";*/
                aiCardChoices.push_back(finalOptions[i - 1]);
            }
            validInputs.push_back(to_string(i));
        }
        if(!ai)apci.addPotionInputs(validInputs);
        if (ai) gm.makeCardChoices(aiEventCardChoice::Add);
        string inputChoice = request_input(validInputs);
        if (inputChoice[0] == 'p') { apci.potionMenu(static_cast<int>(inputChoice[1]) - 49); continue; }
        if (inputChoice == "skip") return;
        for (int i{ 1 }; i < validInputs.size(); ++i)
        {
            if (validInputs[i] == inputChoice)
            {
                apci.addCardToDeck(finalOptions[i - 1], upgradedCards[i - 1]);
                break;
            }
        }
        break;
    }
    nogold;
}
cardHolder gamestate::getRandomAttack()
{
    auto itr = card_database.begin();
    while (true)
    {
        int index = random_int(0, static_cast<int>(card_database.size()) - 1);
        itr = card_database.begin();
        for (int i{ 0 }; i < index; ++i) ++itr;
        if (itr->second->getType() != cardType::Attack || itr->second->getName() == "Feed" || itr->second->getName() == "Reaper") continue;
        break;
    }
    return cardHolder(itr->second->clone());
}
cardHolder gamestate::getRandomPower()
{
    auto itr = card_database.begin();
    while (true)
    {
        int index = random_int(0, static_cast<int>(card_database.size()) - 1);
        itr = card_database.begin();
        for (int i{ 0 }; i < index; ++i) ++itr;
        if (itr->second->getType() != cardType::Power) continue;
        break;
    }
    return cardHolder(itr->second->clone());
}
vector<C_DATABASE::iterator> gamestate::getXRandomCardsOfType(int x, cardType ct)
{
    vector<C_DATABASE::iterator> cardOptions, res;
    for (auto itr{ card_database.begin() }; itr != card_database.end(); ++itr)
    {
        if (ct == itr->second->getType() || ct == Status)
        {
            if (itr->second->getName() == "Bash" || itr->second->getName() == "Strike" || itr->second->getName() == "Reaper" ||
                itr->second->getName() == "Feed" || itr->second->getName() == "Defend") continue;
            if (itr->second->getType() == Status) continue;
            cardOptions.push_back(itr);
        }
    }
    for (int i{ 0 }; i < x; ++i)
    {
        int rng = random_int(0, static_cast<int>(cardOptions.size()) - 1);
        res.push_back(cardOptions[rng]);
        cardOptions.erase(cardOptions.begin() + rng);
    }
    return res;
}
vector<C_DATABASE::iterator> gamestate::getXRandomColourlessCards(int x, cardRarity cr)
{
    vector<C_DATABASE::iterator> cardOptions, res;
    for (auto itr{ card_database_clr.begin() }; itr != card_database_clr.end(); ++itr)
    {
        if(itr->second->getRarity() == cr || cr == cardRarity::Other) cardOptions.push_back(itr);
    }
    for (int i{ 0 }; i < x; ++i)
    {
        int rng = random_int(0, static_cast<int>(cardOptions.size()) - 1);
        res.push_back(cardOptions[rng]);
        if (x <= 5) cardOptions.erase(cardOptions.begin() + rng);
    }
    return res;
}
void gamestate::rareCardReward()
{
    aiCardChoices.clear();
    vector<bool> upgradedCards;
    for (int i{ 0 }; i < cardRewardNumber; ++i)
    {
        upgradedCards.push_back(random_int(1, 100) <= upgraded_pc);
    }
    while (true)
    {
        cardRarity chosenRarity = Rare;
        vector<C_DATABASE::iterator> cardOptions, notIncluding;
        while (cardOptions.size() < cardRewardNumber)
        {
            notIncluding.clear();
            for (auto itr{ card_database.begin() }; itr != card_database.end(); ++itr)
            {
                if (chosenRarity == itr->second->getRarity())
                {
                    if (std::find(cardOptions.begin(), cardOptions.end(), itr) != cardOptions.end()) continue;
                    notIncluding.push_back(itr);
                }
            }
            cardOptions.push_back(notIncluding[random_int(0, static_cast<int>(notIncluding.size()) - 1)]);
        }
        vector<string> validInputs{ "skip" };
        std::cout << "Select a card to add to your deck or skip: " << endl;
        for (int i{ 1 }; i < cardOptions.size() + 1; ++i)
        {
            if (!ai)
            {
                std::cout << i << " for " << cardOptions[i - 1]->second->getName();
                if (upgradedCards[i - 1]) std::cout << "+";
                std::cout << " (" << cardOptions[i - 1]->second->getRarity() << ")" << ". ";
            }
            else
            {
                aiCardChoices.push_back(cardOptions[i - 1]);
            }
            validInputs.push_back(to_string(i));
        }
        if (!ai) apci.addPotionInputs(validInputs);
        if (ai) gm.makeCardChoices(aiEventCardChoice::Add);
        string inpRes = request_input(validInputs);
        if (inpRes == "skip") { if (singingBowlCheck) { apci.getPlayer().gainMaxHP(2); } return; }
        if (inpRes[0] == 'p') { apci.potionMenu(static_cast<int>(inpRes[1]) - 49); continue; }
        for (int i{ 1 }; i < validInputs.size(); ++i)
        {
            if (validInputs[i] == inpRes)
            {
                apci.addCardToDeck(cardOptions[i - 1], upgradedCards[i - 1]);
                break;
            }
        }
        break;
    }
}
void gamestate::libraryCardOpts()
{
    cardRarity chosenRarity;
    vector<C_DATABASE::iterator> cardOptions, notIncluding;
    cardOptions.clear();
    int tempCommon{ common_pc }, tempUncommon{ uncommon_pc }, tempRare{ rare_pc };
    tempRare += rare_offset;
    if (tempRare < 0)
    {
        tempUncommon += tempRare;
        tempRare = 0;
    }
    tempCommon = 100 - tempUncommon - tempRare;
    
    while (cardOptions.size() < 20)
    {
        int res = random_int(1, 100);

        if (res >= 1 && res <= tempCommon)
        {
            chosenRarity = cardRarity::Common;
        }
        else if (res >= tempCommon + 1 && res <= tempCommon + tempUncommon)
        {
            chosenRarity = cardRarity::Uncommon;
        }
        else { chosenRarity = cardRarity::Rare; }

        notIncluding.clear();
        for (auto itr{ card_database.begin() }; itr != card_database.end(); ++itr)
        {
            if (chosenRarity == itr->second->getRarity())
            {
                if (chosenRarity == cardRarity::Common && (itr->second->getName() == "Bash" ||
                    itr->second->getName() == "Strike" ||
                    itr->second->getName() == "Defend") || itr->second->getType() == Status) continue;
                if (std::find(cardOptions.begin(), cardOptions.end(), itr) != cardOptions.end()) continue;
                notIncluding.push_back(itr);
            }
        }
        cardOptions.push_back(notIncluding[random_int(0, static_cast<int>(notIncluding.size()) - 1)]);
    }
    vector<string> validInputs;
    std::cout << "Select a card to add to your deck: " << endl;
    for (int i{ 0 }; i < cardOptions.size(); ++i)
    {
        if(!ai) std::cout << i + 1 << " for " << cardOptions[i]->second->getName() << ". ";
        else
        {
            aiCardChoices.push_back(cardOptions[i]);
        }
        validInputs.push_back(to_string(i+1));

    }
    if (ai) gm.makeCardChoices(aiEventCardChoice::Add);
    if (aiEventActionBuffer.front() == "skip") aiEventActionBuffer.front() = "1";
    int res2 = stoi(request_input(validInputs)) - 1;
    apci.addCardToDeck(cardOptions[res2]);
}

cardHolder gamestate::getRandomCard()
{
    auto itr = card_database.begin();
    while (true)
    {
        int index = random_int(0, static_cast<int>(card_database.size()) - 1);
        itr = card_database.begin();
        for (int i{ 0 }; i < index; ++i) ++itr;
        if (itr->second->getType() == cardType::Status || itr->second->getName() == "Feed" || itr->second->getName() == "Reaper") continue;
        break;
    }
    return cardHolder(itr->second->clone());
}
void gamestate::transformCard(deque<unique_ptr<card>>::iterator chosenCard, bool print)
{
    switch ((*chosenCard)->getClass())
    {
    case cardClass::IC:
        break;
    case cardClass::Colourless:
        transformColourless(chosenCard, print);
        return;
        break;
    case cardClass::Curse:
        transformCurse(chosenCard, print);
        return;
        break;
    }
    auto itr = card_database.begin();
    while (true)
    {
        int index = random_int(0, static_cast<int>(card_database.size()) - 1);
        itr = card_database.begin();
        for (int i{ 0 }; i < index; ++i) ++itr;
        if ((itr->second->getRarity() == cardRarity::Common && (itr->second->getName() == "Bash" ||
            itr->second->getName() == "Strike" || itr->second->getName() == "Defend"))) continue;
        if (itr->second->getName() == (*chosenCard)->getName()) continue;
        break;
    }
    if (print) std::cout << (*chosenCard)->getName() << " was transformed into " << itr->second->getName() << endl;
    apci.removeCard(chosenCard,print);
    apci.addCardToDeck(itr);
}
void gamestate::transformColourless(deque<unique_ptr<card>>::iterator chosenCard, bool print)
{
    auto itr = card_database_clr.begin();
    while (true)
    {
        int index = random_int(0, static_cast<int>(card_database_clr.size()) - 1);
        itr = card_database_clr.begin();
        for (int i{ 0 }; i < index; ++i) ++itr;
        if (itr->second->getName() == (*chosenCard)->getName()) continue;
        break;
    }
    if (print) std::cout << (*chosenCard)->getName() << " was transformed into " << itr->second->getName() << endl;
    apci.removeCard(chosenCard, print);
    apci.addCardToDeck(itr);
}
void gamestate::transformCurse(deque<unique_ptr<card>>::iterator chosenCard, bool print)
{
    auto itr = card_database_curse.begin();
    while (true)
    {
        int index = random_int(0, static_cast<int>(card_database_curse.size()) - 1);
        itr = card_database_curse.begin();
        for (int i{ 0 }; i < index; ++i) ++itr;
        if (itr->first == "Necronomicurse_ID" || itr->first == "CurseOfTheBell_ID") continue;
        if (itr->second->getName() == (*chosenCard)->getName()) continue;
        break;
    }
    if (print) std::cout << (*chosenCard)->getName() << " was transformed into " << itr->second->getName() << endl;
    apci.removeCard(chosenCard, print);
    apci.addCardToDeck(itr);
}
void gamestate::upgradeCardFromDeck(bool print)
{
    cardDeck::iterator chosen;
    while (true)
    {
        chosen = chooseCardFromDeck(true);
        if ((*chosen)->getType() == cardType::Curse) continue;
        if (!(*chosen)->getUpgraded()) break;
    }
    (*chosen)->upgradeThis();
    if (print && !ai) std::cout << (*chosen)->getName() << " was upgraded." << endl;
}
void gamestate::upgradeRandomFromDeck()
{
    vector<cardDeck::iterator> opts;
    auto itr = apci.getDeck().begin();
    while (itr != apci.getDeck().end())
    {
        if ((*itr)->getType() == cardType::Curse || (*itr)->getUpgraded()) { ++itr; continue; }
        opts.push_back(itr);
        ++itr;
    }
    if (opts.empty()) return;
    int ind = random_int(0, static_cast<int>(opts.size()) - 1);
    (*opts[ind])->upgradeThis();
    if (!ai) std::cout << (*opts[ind])->getName() << " was upgraded." << endl;
}
void gamestate::upgradeRandomFromDeck(cardType ct)
{
    vector<cardDeck::iterator> opts;
    auto itr = apci.getDeck().begin();
    while (itr != apci.getDeck().end())
    {
        if ((*itr)->getType() != ct || (*itr)->getUpgraded()) { ++itr; continue; }
        opts.push_back(itr);
        ++itr;
    }
    if (opts.empty()) return;
    int ind = random_int(0, static_cast<int>(opts.size()) - 1);
    (*opts[ind])->upgradeThis();
    if (!ai) std::cout << (*opts[ind])->getName() << " was upgraded." << endl;
}
deque<unique_ptr<card>>::iterator gamestate::chooseCardFromDeck(bool bottledAllowed)
{
    vector<string> validInputs{};
    for (int i{ 1 }; i < apci.getDeck().size() + 1; ++i)
    {
        if (apci.getDeck()[i - 1]->getName() == "Curse of the Bell" || apci.getDeck()[i - 1]->getName() == "Necronomicurse" || 
            (!bottledAllowed && apci.getDeck()[i - 1]->getBottled())) continue;
        if (!ai) std::cout << i << " for " << apci.getDeck()[i - 1]->getName() << " (" << apci.getDeck()[i - 1]->getRarity() << "). ";
        validInputs.push_back(to_string(i));
    }
    string inputChoice = request_input(validInputs);
    int index = stoi(inputChoice) - 1;
    return apci.getDeck().begin() + index;
}
deque<unique_ptr<card>>::iterator gamestate::chooseCardFromDeck(cardType ct, bool bottledAllowed)
{
    vector<string> validInputs{};
    for (int i{ 1 }; i < apci.getDeck().size() + 1; ++i)
    {
        if (ct != apci.getDeck()[i - 1]->getType() || (!bottledAllowed && apci.getDeck()[i - 1]->getBottled())) continue;
        if(!ai) std::cout << i << " for " << apci.getDeck()[i - 1]->getName() << " (" << apci.getDeck()[i - 1]->getRarity() << "). ";
        validInputs.push_back(to_string(i));
    }
    string inputChoice = request_input(validInputs);
    int index = stoi(inputChoice) - 1;
    return apci.getDeck().begin() + index;
}
void gamestate::transformCardFromDeck()
{
    auto chosen = chooseCardFromDeck();
    transformCard(chosen, true);
}
void gamestate::transformTwoFromDeck()
{
    auto chosen = chooseCardFromDeck();
    transformCard(chosen, true);
    chosen = chooseCardFromDeck();
    transformCard(chosen, true);
}
void gamestate::removeCardFromDeck()
{
    auto chosen = chooseCardFromDeck();
    apci.removeCard(chosen, true);
}

void gamestate::endFight()
{
    while (!enemies_.empty()) enemies_.pop_back();
    while (!dormant_enemies_.empty()) dormant_enemies_.pop_back();
    dormant_enemies_ = vector<vector<unique_ptr<nonpc>>>(3);
    for (int i{ 0 }; i < intents_.size(); ++i)
    {
        intents_[i].clear();
    }
    gm.callFightEnd();
    gm.resetFocusedEvents();
    apci.endFight();
    turnCount = 0;
    doubleAttackMod = 0;
    nextCardsNoTarget = 0;
    nextCardSameTarget = false;
    denyCardPlay = false;
    clockEnd = false;
    lastAttackedSCRNIndex = -1;
    dontDupe = false;
    aiCardChoices.clear();
}

void gamestate::startMushroomFight()
{
    gm.fightIndexPRED = 20;
    visuals::setvisInUse(aiu::three);
    mapPath[mapPathIndex] = 'e';
    addEnemiesByID("FungiBeast_ID",0,true);
    addEnemiesByID("FungiBeast_ID",1,true);
    addEnemiesByID("FungiBeast_ID",2,true);
    try { if (ai) { apci.startTurn(); outerTurnLoopAI(getPyFunc()); } else { outerTurnLoop(); } }
    catch (SmokeBombError) { apci.removeSmokeBomb(); mapPath[mapPathIndex] = '?'; throw SmokeBombError("escaped"); }
    if(apci.getPlayer().getHealth() != 0)
    {
        std::cout<<"Victory..."<<endl;
    }
    endFight();
    hallwayReward();
    apci.addRelic(gm.getRelicByID("OddMushroom_ID"));
    mapPath[mapPathIndex] = '?';
}
void gamestate::startDeadAdventurerFight()
{
    switch (questionMark::storedValue1)
    {
    case 0:
        gm.fightIndexPRED = 14;
        addEnemiesByID("GremlinNob_ID",0,true);
        visuals::setvisInUse(aiu::one);
        break;
    case 1:
        gm.fightIndexPRED = 15;
        addEnemiesByID("Lagavulin_ID",0,true);
        visuals::setvisInUse(aiu::one);
        break;
    case 2:
        gm.fightIndexPRED = 16;
        addEnemiesByID("Sentry1_ID", 0, true);
        addEnemiesByID("Sentry2_ID", 1, true);
        addEnemiesByID("Sentry1_ID" ,2, true);
        visuals::setvisInUse(aiu::three);
        break;
    }
    mapPath[mapPathIndex] = 'E';
    if (questionMark::storedValue1 == 1)
    {
        enemies_[0]->addPastAttack("asleep");
        enemies_[0]->addPastAttack("asleep");
        enemies_[0]->addPastAttack("asleep");
    }
    try { if (ai) { apci.startTurn(); outerTurnLoopAI(getPyFunc()); } else { outerTurnLoop(); } }
    catch (SmokeBombError) { apci.removeSmokeBomb(); mapPath[mapPathIndex] = '?'; throw SmokeBombError("escaped"); }
    if (apci.getPlayer().getHealth() != 0)
    {
        std::cout << "Victory..." << endl;
    }
    endFight();
    mapPath[mapPathIndex] = '?';
}
void gamestate::startBanditFight()
{
    gm.fightIndexPRED = 41;
    visuals::setvisInUse(aiu::three);
    mapPath[mapPathIndex] = 'e';
    addEnemiesByID("Pointy_ID", 0, true);
    addEnemiesByID("Romeo_ID", 1, true);
    addEnemiesByID("Bear_ID", 2, true);
    try { if (ai) { apci.startTurn(); outerTurnLoopAI(getPyFunc()); } else { outerTurnLoop(); } }
    catch (SmokeBombError) { apci.removeSmokeBomb(); mapPath[mapPathIndex] = '?'; throw SmokeBombError("escaped"); }
    if (apci.getPlayer().getHealth() != 0)
    {
        std::cout << "Victory..." << endl;
    }
    endFight();
    hallwayReward();
    apci.addRelic(gm.getRelicByID("RedMask_ID"));
    mapPath[mapPathIndex] = '?';
}
void gamestate::startOrbFight()
{
    gm.fightIndexPRED = 60;
    visuals::setvisInUse(aiu::two);
    mapPath[mapPathIndex] = 'e';
    addEnemiesByID("OrbWalker_ID", 0, true);
    addEnemiesByID("OrbWalker_ID", 1, true);
    try { if (ai) { apci.startTurn(); outerTurnLoopAI(getPyFunc()); } else { outerTurnLoop(); } }
    catch (SmokeBombError) { apci.removeSmokeBomb(); mapPath[mapPathIndex] = '?'; throw SmokeBombError("escaped"); }
    if (apci.getPlayer().getHealth() != 0)
    {
        std::cout << "Victory..." << endl;
    }
    endFight();
    hallwayReward();
    apci.addRelic(gm.retrieveRareRelic());
    mapPath[mapPathIndex] = '?';
}
void gamestate::startAct1BossFight()
{
    mapPath[mapPathIndex] = 'B';
    int rng = random_int(0, 2);
    switch (rng)
    {
    case 0:
        gm.fightIndexPRED = 19;
        addEnemiesByID("SlimeBoss_ID", 0, true);
        break;
    case 1:
        gm.fightIndexPRED = 17;
        addEnemiesByID("TheGuardian_ID", 0, true);
        break;
    case 2:
        gm.fightIndexPRED = 18;
        addEnemiesByID("HexaGhost_ID", 0, true);
        break;
    }
    resetVisuals();
    try { if (ai) { apci.startTurn(); outerTurnLoopAI(getPyFunc()); } else { outerTurnLoop(); } }
    catch (SmokeBombError) { apci.removeSmokeBomb(); mapPath[mapPathIndex] = '?'; throw SmokeBombError("escaped"); }
    if (apci.getPlayer().getHealth() != 0)
    {
        std::cout << "Victory..." << endl;
    }
    endFight();
    int goldReward = random_int(40, 50);
    apci.gainGold(goldenIdolCheck ? goldReward : goldReward * 5 / 4);
    prelude();
    apci.addRelic(gm.retrieveRareRelic());
    potionReward();
    hallwayReward(true);
    if (prayerWheelCheck) hallwayReward(true);
    mapPath[mapPathIndex] = '?';
}
void gamestate::startColosseumFight()
{
    gm.fightIndexPRED = 40;
    visuals::setvisInUse(aiu::two);
    mapPath[mapPathIndex] = 'E';
    addEnemiesByID("BlueSlaver_ID", 0, true);
    addEnemiesByID("RedSlaver_ID", 1, true);
    try { if (ai) { apci.startTurn(); outerTurnLoopAI(getPyFunc()); } else { outerTurnLoop(); } }
    catch (SmokeBombError) { apci.removeSmokeBomb(); return; }
    if (apci.getPlayer().getHealth() != 0)
    {
        std::cout << "Victory..." << endl;
    }
    endFight();
}
void gamestate::startColosseumFight2()
{
    gm.fightIndexPRED = 42;
    visuals::setvisInUse(aiu::two);
    addEnemiesByID("Taskmaster_ID", 0, true);
    addEnemiesByID("GremlinNob_ID", 1, true);
    try { if (ai) { apci.startTurn(); outerTurnLoopAI(getPyFunc()); } else { outerTurnLoop(); } }
    catch (SmokeBombError) { apci.removeSmokeBomb(); mapPath[mapPathIndex] = '?'; throw SmokeBombError("escaped"); }
    if (apci.getPlayer().getHealth() != 0)
    {
        std::cout << "Victory..." << endl;
    }
    endFight();
    apci.gainGold(100);
    apci.addRelic(gm.retrieveUncommonRelic());
    apci.addRelic(gm.retrieveRareRelic());
    hallwayReward(true);
    mapPath[mapPathIndex] = '?';
}
void gamestate::playNextEvent() { nextEvent->startEvent(); }
void gamestate::outerTurnLoop()
{
    while (true)
    {
        apci.startTurnV();

        if (checkEnemyDeath()) { handleEnemyDeath(); if (checkAllEnemyDeath()) break; }

        startEnemyTurn();
        if (apci.getPlayer().getHealth() == 0)
        {
            std::cout << "Defeat..." << endl;
            break;
        }
        if (checkEnemyDeath()) { handleEnemyDeath(); if (checkAllEnemyDeath()) break; }
    }
}

void gamestate::startRandomFight(events* e)
{
    e->placeRandomForNext(actNumber);
    vector<string> names_ = e->namesForFight(bossIndexThisAct);
    if (names_.empty())
    {
        int a = 9 + 10; a;
    }
    for (int i{ 0 }; i < names_.size(); ++i)
    {
        addEnemiesByID(names_[i], i);
    }
    if (enemies_.empty())
    {
        throw runtime_error("noenemies");
    }
    resetVisuals();

    apci.startTurn();
}
void gamestate::startRandomFightV(events* e)
{
    e->placeRandomForNext(actNumber);
    vector<string> names_ = e->namesForFight();

    for (int i{ 0 }; i < names_.size(); ++i)
    {
        addEnemiesByID(names_[i], i);
    }
    resetVisuals();
    outerTurnLoop();
    if (apci.getPlayer().getHealth() != 0) std::cout << "Victory..." << endl;
    endFight();
    auto it = dynamic_cast<elites*>(e);
    if (it == nullptr)
    {
        gm.hallwayReward();
    }
    else
    {
        gm.eliteRelicReward();
        gm.eliteReward();
    }
}
void gamestate::AIFightEnd()
{
    endFight();
    auto it = dynamic_cast<elites*>(nextEvent);
    if (it == nullptr)
    {
        auto it2 = dynamic_cast<easyHallway*>(nextEvent);
        if (it2 == nullptr)
        {
            auto it3 = dynamic_cast<hardHallway*>(nextEvent);
            if(it3 != nullptr ) gm.hallwayReward();
        }
        else{ gm.hallwayReward(); }
    }
    else
    {
        gm.eliteReward();
    }
}
void gamestate::startRandomBossFight()
{
    apci.endFight();
    apci.getPlayer().healToFull();
    enemies_.clear();
    addBossEnemies(0, 0);
    apci.getPlayer().sotCleanup();
    for_each(getAllEnemies().begin(), getAllEnemies().end(), [](unique_ptr<nonpc>& e) {e->sotCleanup(); });
    apci.drawX(apci.getPlayer().getDrawSOT());
    setPlayerEnergySOT(apci.getPlayer().getEnergy());
    for_each(getAllEnemies().begin(), getAllEnemies().end(), [](unique_ptr<nonpc>& e) {e->chooseAndDeclareIntent(); });
    if (gm.turnCount == 0)
    {
        for_each(getAllEnemies().begin(), getAllEnemies().end(), [](unique_ptr<nonpc>& e) {e->startOfFight(); });
        apci.getPlayer().startOfFight();
        apci.cardsStartOfFight();
    }
}
void gamestate::startRandomBossFightV()
{
    int index = bossIndexThisAct;
    if (events_bo->bossOptions[index].size() == 1)
    {
        visuals::setvisInUse(aiu::one);
        addBossEnemies(index, 0);
    }
    else if (events_bo->bossOptions[index].size() == 2)
    {
        visuals::setvisInUse(aiu::two);
        addBossEnemies(index, 0);
        addBossEnemies(index, 1);
    }
    else if (events_bo->bossOptions[index].size() == 3)
    {
        visuals::setvisInUse(aiu::three);
        addBossEnemies(index, 0);
        addBossEnemies(index, 1);
        addBossEnemies(index, 2);
    }
    else { throw runtime_error("not one or two"); }
    outerTurnLoop();
    if (apci.getPlayer().getHealth() != 0)
    {
        std::cout << "Victory..." << endl;
    }
    endFight();
}
char gamestate::calcOdds(vector<char> events_, vector<int> percentages, vector<int> maxInARow, int index)
{
    if(percentages.size()!=maxInARow.size() || percentages.empty()) throw runtime_error("calc intent bad 1");
    int oneInHundred{0};
    vector<int> cumulative = {percentages[0]};
    for(int j{0};j<percentages.size();++j)
    {
        if(j!=0) cumulative.push_back(percentages[j] + cumulative[j-1]);
    }
    if(*(cumulative.end()-1)!=100) throw runtime_error("calc intent bad 2");
    int i{0}; 
    while(i!=percentages.size() && oneInHundred==0)
    {
        if(checkLastX(mapPath,maxInARow[i],events_[i],index))
        {
            if(i==0) oneInHundred = random_int(percentages[i]+1,100);
            else if(i==percentages.size()-1) oneInHundred = random_int(1, 100-percentages[i]);
            else
            {
                int temp = random_int(1,100-percentages[i]);
                if(temp>cumulative[i]-percentages[i]) oneInHundred = temp+percentages[i];
                else{oneInHundred = temp;}
            }
        }
        ++i;
    }
    if(oneInHundred==0) oneInHundred = random_int(1,100);
    for(int j{0}; j<cumulative.size(); ++j)
    {
        if(oneInHundred <= cumulative[j]) 
        {
            if( (mapPath[index-1]=='E' && events_[j]=='E') ||(mapPath[index-1]=='c' && events_[j]=='c' ))
            {
                int g{4};
                g=3;
            }
            return events_[j];
        }
    }
    throw runtime_error("bad calc intent 3");
}
void gamestate::generateMapPath()
{
    int eCount{0}, qCount{0}, sinceElite{0};
    mapPath[0] = 'N';
    mapPath[1] = 'e';
    for(int i{2};i<5;++i)
    {
        int rng = random_int(0,1);
        if(rng==0)
        {
            if(eCount<=qCount) mapPath[i] = 'e';
            else
            {
                if(random_int(0,eCount-qCount)>=2) mapPath[i] = 'e';
                else{mapPath[i] = '?';}
            }
        }
        else
        {
            if(qCount<=eCount) mapPath[i] = '?';
            else
            {
                if(random_int(0,qCount-eCount)>=2) mapPath[i] = '?';
                else{mapPath[i] = 'e';}
            }
        }
        mapPath[i]=='e' ? ++eCount : ++qCount;
    }
    for(int i{5};i<9;++i)
    {
        if(random_int(0,sinceElite) >= 1)
        {
            mapPath[i] = 'E';
            sinceElite=0;
        }
        else
        {
            mapPath[i] = calcOdds({'e','?','s','E','c'},{50,20,10,10,10},{3,3,1,1,1},i);
            if(mapPath[i] == 'E') sinceElite=0;
            else{++sinceElite;}
        }
        
    }
    mapPath[9] = 'C';
    for(int i{10};i<15;++i)
    {
        if(random_int(0,sinceElite) >= 2)
        {
            mapPath[i] = 'E';
            sinceElite=0;
        }
        mapPath[i] = calcOdds({'e','?','s','E','c'},{38,25,12,10,15},{3,3,1,1,1},i);
    }
    mapPath[15] = random_int(0,1) ? '?' : 'e';
    mapPath[16] = 'c';
    mapPath[17] = 'B';
}
void gamestate::teleportToBoss()
{
    currentFloorIndex = 49;
    currentWidthIndex = mapGen->getNode(16)->prevNodes[0]->widthIndex;
    mapPathIndex = 49;
}
void gamestate::advanceEvent()
{
    //unused
    if(firstMapItr) firstMapItr = false;
    else {++mapPathIndex;}
    if(nextEvent && mapPathIndex!=0 && (mapPath[mapPathIndex-1] == 'N' ||mapPath[mapPathIndex-1] == 's' 
    || mapPath[mapPathIndex-1] == 'C' ||mapPath[mapPathIndex-1] == '?')) delete nextEvent;
    nextEvent = makeNextEvent(mapPath[mapPathIndex]);
}
void gamestate::StartGame()
{
    int rng = random_int(0, 4);
    if (rng >= 2) rng = random_int(0, 4);
    if (rng == 4) rng = random_int(0, 4);
    if (actNumber == 4) rng = random_int(2, 3);
    switch (rng)
    {
    case 0:
        nextEvent = events_eh;
        setMP('e');
        break;
    case 1:
        nextEvent = events_hh;
        setMP('e');
        break;
    case 2:
        nextEvent = events_el;
        setMP('E');
        break;
    case 3:
        nextEvent = events_bo;
        setMP('B');
        break;
    case 4:
        nextEvent = events_qs;
        setMP('e');
        break;
    }
}
void gamestate::StartGame(int rng)
{
    if (rng == 4) rng = random_int(0, 4);
    if (actNumber == 4) rng = random_int(2, 3);
    switch (rng)
    {
    case 0:
        nextEvent = events_eh;
        setMP('e');
        break;
    case 1:
        nextEvent = events_hh;
        setMP('e');
        break;
    case 2:
        nextEvent = events_el;
        setMP('E');
        break;
    case 3:
        nextEvent = events_bo;
        setMP('B');
        break;
    case 4:
        nextEvent = events_qs;
        setMP('e');
        break;
    }
}
void gamestate::StartGameV()
{
    try
    {
        setBossIndex();
        while (currentFloorIndex != 50)
        {
            if(currentFloorIndex == 16) Act1to2();
            else if (currentFloorIndex == 33) Act2to3();
            
            advanceRealEvent();
            try { nextEvent->startEvent(); }
            catch (SmokeBombError) { apci.removeSmokeBomb(); }
        }
        Act3to4();
        startAct4();
        std::cout << "Act 4 Complete! Results: " << endl << endl;
        prelude();
    }
    catch (PlayerDiedError)
    {
        std::cout << "You lose" << endl;
    }
}
void gamestate::Act1to2()
{
    rareCardReward();
    int goldReward = random_int(95, 105);
    apci.gainGold(goldenIdolCheck ? goldReward * 5 / 4 : goldReward);
    potionReward();
    mapPathIndex = 17;

    apci.getPlayer().healToFull();
    nextEvent = new chest(true);
    nextEvent->startEvent();
    delete nextEvent;
    nextEvent = nullptr;

    actNumber = 2;
    currentFloorIndex = 17;

    msi.createAllVars();
    storedMSIs.push_back(msi);

    genRealMap();
    upgraded_pc = 25;

    makeEnemyEventsA2();
    makeQuestionEventsA2();

    setBossIndex();
}
void gamestate::Act2to3()
{
    rareCardReward();
    int goldReward = random_int(95, 105);
    apci.gainGold(goldenIdolCheck ? goldReward * 5 / 4 : goldReward);
    potionReward();
    mapPathIndex = 34;

    apci.getPlayer().healToFull();
    nextEvent = new chest(true);
    nextEvent->startEvent();
    delete nextEvent;
    nextEvent = nullptr;

    actNumber = 3;
    currentFloorIndex = 34;

    msi.createAllVars();
    storedMSIs.push_back(msi);

    genRealMap();
    upgraded_pc = 50;

    makeEnemyEventsA3();
    makeQuestionEventsA3();

    setBossIndex();
}
void gamestate::Act3to4()
{
    mapPathIndex = 50;
    actNumber = 4;
    currentFloorIndex = 50;
    apci.getPlayer().healToFull();
    makeEnemyEventsA4();
}
void gamestate::startAct4()
{
    gamestate::aiCardChoices.clear();
    gamestate::aiEventActionBuffer.clear();
    gamestate::randomAIChoice = false;

    bossIndexThisAct = 0;
    mapPath[mapPathIndex] = 'c';
    nextEvent = events_ca;
    nextEvent->startEvent();

    gamestate::aiCardChoices.clear();
    gamestate::aiEventActionBuffer.clear();
    gamestate::randomAIChoice = false;

    msi.createAllVars();
    storedMSIs.push_back(msi);
    mapPath[mapPathIndex] = 's';
    nextEvent = new shop;
    nextEvent->startEvent();

    gamestate::aiCardChoices.clear();
    gamestate::aiEventActionBuffer.clear();
    gamestate::randomAIChoice = false;

    msi.createAllVars();
    storedMSIs.push_back(msi);
    delete nextEvent;
    mapPath[mapPathIndex] = 'E';
    nextEvent = events_el;
    try { nextEvent->startEvent(); outerTurnLoopAI(getPyFunc()); }
    catch (SmokeBombError) { apci.removeSmokeBomb(); }

    gamestate::aiCardChoices.clear();
    gamestate::aiEventActionBuffer.clear();
    gamestate::randomAIChoice = false;

    msi.createAllVars();
    storedMSIs.push_back(msi);
    mapPath[mapPathIndex] = 'B';
    nextEvent = events_bo;
    nextEvent->startEvent();
    outerTurnLoopAI(getPyFunc());

}
void gamestate::ResetRun()
{
    int startingPoint = 1;
    //int startingPoint = random_int(0, 2);

    apci.resetIronclad();
    actNumber = 1;
    mapPathIndex = 0; currentFloorIndex = 0; currentWidthIndex = 0;
    storedMSIs.clear();
    firstMapItr = true;
    enemies_.clear();

    dormant_enemies_.clear();
    dormant_enemies_ = vector<vector<unique_ptr<nonpc>>>(3);

    intents_.clear();
    for (int i{ 0 }; i < 6; ++i) intents_.push_back({});

    resetFocusedEvents();
    clearEvents();

    makeEvents();
    makeRelicStacks();
    makePotionStacks();
    
    genRealMap();

    if (startingPoint <= 1) apci.initIronclad();
    else if (startingPoint >= 2)
    {
        firstMapItr = false;
        mapPathIndex = 15; currentFloorIndex = 15; currentWidthIndex = mapGen->getNode(16)->prevNodes[0]->widthIndex;
        apci.generateRandomDeck(deckGen::BO1);
        apci.addRelic(gm.getRelicByID("BurningBlood_ID"));
        apci.getPlayer().setHealth(random_int(30, apci.getPlayer().getMaxHealth()));
        msi.createAllVars();
        msi.mapRepZeroes();
        storedMSIs.push_back(msi);
    }
    nextEvent = nullptr;

    shrineChance        = 25;
    enemyChance         = 10;
    chestChance         = 2;
    shopChance          = 3;
    upgraded_pc         = 0;
    common_pc           = 60;
    uncommon_pc         = 37;
    rare_pc             = 3;
    rare_offset         = -5;
    common_pc_elite     = 50;
    uncommon_pc_elite   = 40;
    rare_pc_elite       = 10;
    common_pc_potion    = 65;
    uncommon_pc_potion  = 25;
    rare_pc_potion      = 10;
    current_potion_pc   = 40;
    easyDone            = 0;
    //easyDone            = 3;
    shop::cardRemove = 75;
    aiSimActionBuffer.clear(); aiSimPotionBuffer.clear(); aiEventActionBuffer.clear(); aiCardBuffer.clear(); aiCardChoices.clear();

}
void gamestate::thinOutMSIs()
{
    int i{ 0 }, j{ 0 };
    size_t n{ storedMSIs.size() };
    while (i < storedMSIs.size())
    {
        if (random_int(1, n + 40) > j + 50) {
            storedMSIs.erase(storedMSIs.begin() + i);
            ++j;
        }
        else {
            ++i;
        }
    }
}
deckGen gamestate::makeDGEnum()
{
    //return static_cast<deckGen>(random_int(0, 14));
    if (actNumber == 1)
    {
        if (nextEvent == events_eh) return deckGen::EH1;
        if (nextEvent == events_hh) return deckGen::HH1;
        if (nextEvent == events_el) return deckGen::EL1;
        if (nextEvent == events_bo) return deckGen::BO1;
        if (nextEvent == events_qs) return deckGen::QS1;
    }
    else if(actNumber == 2)
    {
        if (nextEvent == events_eh) return deckGen::EH2;
        if (nextEvent == events_hh) return deckGen::HH2;
        if (nextEvent == events_el) return deckGen::EL2;
        if (nextEvent == events_bo) return deckGen::BO2;
        if (nextEvent == events_qs) return deckGen::QS2;
    }
    else if(actNumber == 3)
    {
        if (nextEvent == events_eh) return deckGen::EH3;
        if (nextEvent == events_hh) return deckGen::HH3;
        if (nextEvent == events_el) return deckGen::EL3;
        if (nextEvent == events_bo) return deckGen::BO3;
        if (nextEvent == events_qs) return deckGen::QS3;
    }
    else
    {
        if (nextEvent == events_el) return deckGen::EL4;
        if (nextEvent == events_bo) return deckGen::BO4;
    }
    throw runtime_error("dg enum");
}
void gamestate::addEnemyFrontByIDMidFight(string _name)
{
    enemies_.push_front(getEnemyByID(_name));
    enemies_.front()->sotCleanup();
    enemies_.front()->startOfFight();
    intents_.push_front({});
    intents_.pop_back();
    for (int i{ 0 }; i < enemies_.size(); ++i)
    {
        enemies_[i]->setSCRNindex(i);
    }
}
void gamestate::addEnemiesByID(string _name, int i, bool midFight)
{
    enemies_.push_back(getEnemyByID(_name));
    enemies_[i]->setSCRNindex(i);
    enemies_[i]->sotCleanup();
    // enemies_[i]->chooseAndDeclareIntent();
    enemies_[i]->startOfFight();
    midFight;
}
void gamestate::addEnemiesByID(string _name, int i)
{
    enemies_.push_back(getEnemyByID(_name));
    enemies_[i]->setSCRNindex(i);
}
void gamestate::addEasyEnemies(int index, int i)
{
    enemies_.push_back(getEnemyByID(events_eh->hallwayOptions[index][i]));
    enemies_[i]->setSCRNindex(i);
}
void gamestate::addHardEnemies(int index, int i)
{
    enemies_.push_back(getEnemyByID(events_hh->hallwayOptions[index][i]));
    enemies_[i]->setSCRNindex(i);
}
void gamestate::addEliteEnemies(int index, int i)
{
    enemies_.push_back(getEnemyByID(events_el->eliteOptions[index][i]));
    enemies_[i]->setSCRNindex(i);
}
void gamestate::addBossEnemies(int index, int i)
{
    enemies_.push_back(getEnemyByID(events_bo->bossOptions[index][i]));
    enemies_[i]->setSCRNindex(i);
}

void gamestate::makeCardChoices(aiEventCardChoice aiE, bool storeBest)
{
    switch (aiE)
    {
    case aiEventCardChoice::Add:
    {
        msi.createAllVars();
        vector<macroStateInfo> msi_opts(aiCardChoices.size()+1,msi);
        for (int i{ 0 }; i < aiCardChoices.size(); ++i)
        {
            int index = (aiCardChoices[i]->second->getCardIDNum() - 1) * 2;
            if (aiCardChoices[i]->second->getUpgraded())
            {
                ++index;
            }
            else
            {
                switch (aiCardChoices[i]->second->getType())
                {
                case Attack:
                    if (apci.checkForRelic("Molten Egg")) ++index;
                    break;
                case Skill:
                    if (apci.checkForRelic("Toxic Egg")) ++index;
                    break;
                case Power:
                    if (apci.checkForRelic("Frozen Egg")) ++index;
                    break;
                }
            }
            ++msi_opts[i].deckCountPerCard[index];
            if (apci.checkForRelic("Ceramic Fish")) msi_opts[i].playerVars[10] += 9;
            switch (aiCardChoices[i]->second->getType())
            {
            case Attack:
                ++msi_opts[i].playerVars[3];
                break;
            case Skill:
                ++msi_opts[i].playerVars[4];
                break;
            case Power:
                ++msi_opts[i].playerVars[5];
                break;
            }
        }
        if (gamestate::singingBowlCheck) { msi_opts.back().playerVars[0] += 2; msi_opts.back().playerVars[1] += 2; }
        // call on all msis and return best index
        int bestIndex = callMacroIndexREROLLPRED(msi_opts, getPyFunc()) + 1;
        //if (random_int(1, 20) == 20) bestIndex = random_int(0, msi_opts.size() - 1) + 1;
        //cout << bestIndex << endl;
        //if(!random_int(0,3)) bestIndex = tierListAddChoice();
        aiEventActionBuffer.push_back(bestIndex == (aiCardChoices.size() + 1) ? "skip" : to_string(bestIndex));
        break;
    }
    case aiEventCardChoice::Remove:
    {
        if (apci.checkForCurse() && !apci.checkForRelic("Blue Candle") && !apci.checkForRelic("Du-Vu Doll"))
        {
            for (int i{ 0 }; i < apci.getDeck().size(); ++i)
            {
                if (apci.getDeck()[i]->getType() == Curse && apci.getDeck()[i]->getName() != "Necronomicurse" && apci.getDeck()[i]->getName() != "Curse Of The Bell")
                {
                    aiEventActionBuffer.push_back(to_string(i + 1));
                    break;
                }
            }
        }
        else
        {
            msi.createAllVars();
            vector<macroStateInfo> msi_opts(apci.validRemoveCount(), msi);
            vector<int> deckIndices(msi_opts.size());
            int j{ 0 };
            for (int i{ 0 }; i < apci.getDeck().size(); ++i)
            {
                if (apci.getDeck()[i]->getBottled() || apci.getDeck()[i]->getCardIDNum() == 80 || apci.getDeck()[i]->getCardIDNum() == 82) { continue; }
                int index = (apci.getDeck()[i]->getCardIDNum() - 1) * 2;
                if (apci.getDeck()[i]->getUpgraded()) ++index;
                deckIndices[j] = i;
                --msi_opts[j++].deckCountPerCard[index];
            }
            // call on all msis and return best index
            int bestIndex = deckIndices[callMacroIndexREROLLPRED(msi_opts, getPyFunc())] + 1;
            //if (!random_int(0, 2)) bestIndex = tierListRemoveChoice();
            aiEventActionBuffer.push_back(to_string(bestIndex));
        }
        break;
    }
    case aiEventCardChoice::Duplicate:
    {
        msi.createAllVars();
        vector<macroStateInfo> msi_opts(apci.validDuplicateCount(), msi);
        vector<int> deckIndices(msi_opts.size());
        int j{ 0 };
        for (int i{ 0 }; i < apci.getDeck().size(); ++i)
        {
            if (apci.getDeck()[i]->getType() == Curse) { continue; }
            int index = (apci.getDeck()[i]->getCardIDNum() - 1) * 2;
            if (apci.getDeck()[i]->getUpgraded())
            {
                ++index;
            }
            else
            {
                switch (apci.getDeck()[i]->getType())
                {
                case Attack:
                    if (apci.checkForRelic("Molten Egg")) ++index;
                    break;
                case Skill:
                    if (apci.checkForRelic("Toxic Egg")) ++index;
                    break;
                case Power:
                    if (apci.checkForRelic("Frozen Egg")) ++index;
                    break;
                }
            }
            deckIndices[j] = i;
            ++msi_opts[j++].deckCountPerCard[index];
        }
        // call on all msis and return best index
        int bestIndex = deckIndices[callMacroIndexREROLLPRED(msi_opts, getPyFunc())] + 1;
        aiEventActionBuffer.push_back(to_string(bestIndex));
        break;
    }
    case aiEventCardChoice::Upgrade:
    {
        msi.createAllVars();
        vector<macroStateInfo> msi_opts(apci.validDuplicateCount(), msi);
        vector<int> deckIndices(msi_opts.size());
        int j{ 0 };
        for (int i{ 0 }; i < apci.getDeck().size(); ++i)
        {
            if (apci.getDeck()[i]->getUpgraded() || apci.getDeck()[i]->getType() == Curse) { continue; }
            int index = (apci.getDeck()[i]->getCardIDNum() - 1) * 2;
            deckIndices[j] = i;
            ++msi_opts[j].deckCountPerCard[index+1];
            --msi_opts[j++].deckCountPerCard[index];
        }
        // call on all msis and return best index
        int bestIndex = deckIndices[callMacroIndexREROLLPRED(msi_opts, getPyFunc())] + 1;
        //if (!random_int(0, 3)) bestIndex = tierListUpgradeChoice();
        aiEventActionBuffer.push_back(to_string(bestIndex));
        break;
    }
    case aiEventCardChoice::Transform:
    {
        msi.createAllVars();
        vector<macroStateInfo> msi_opts(apci.validTransformCount(), msi);
        vector<int> deckIndices(msi_opts.size());
        int j{ 0 };
        for (int i{ 0 }; i < apci.getDeck().size(); ++i)
        {
            if (apci.getDeck()[i]->getBottled() || apci.getDeck()[i]->getType() == Curse) { continue; }
            int index = (apci.getDeck()[i]->getCardIDNum() - 1) * 2;
            if (apci.getDeck()[i]->getUpgraded()) ++index;
            deckIndices[j] = i;
            --msi_opts[j++].deckCountPerCard[index];
        }
        // call on all msis and return best index

        int bestIndex = deckIndices[callMacroIndexREROLLPRED(msi_opts, getPyFunc())] + 1;
        aiEventActionBuffer.push_back(to_string(bestIndex));
        break;
    }
    case aiEventCardChoice::Transform2:
    {
        msi.createAllVars();
        vector<macroStateInfo> msi_opts(apci.validTransformCount(), msi);
        vector<int> deckIndices(msi_opts.size());
        macroStateInfo temp_msi = msi;
        int j{ 0 };
        for (int k{ 0 }; k < apci.getDeck().size(); ++k)
        {
            if (apci.getDeck()[k]->getBottled() || apci.getDeck()[k]->getType() == Curse) { continue; }
            int index = (apci.getDeck()[k]->getCardIDNum() - 1) * 2;
            if (apci.getDeck()[k]->getUpgraded()) ++index;
            deckIndices[j] = k;
            --msi_opts[j++].deckCountPerCard[index];
        }
        int firstRemove = deckIndices[callMacroIndexREROLLPRED(msi_opts, getPyFunc())];
        --temp_msi.deckCountPerCard[(apci.getDeck()[firstRemove]->getCardIDNum() - 1) * 2 + apci.getDeck()[firstRemove]->getUpgraded()];

        msi_opts = vector<macroStateInfo>(msi_opts.size() - 1, temp_msi);
        deckIndices = vector<int>(msi_opts.size());
        j = 0;
        for (int k{ 0 }; k < apci.getDeck().size(); ++k)
        {
            if (firstRemove == k) continue;
            if (apci.getDeck()[k]->getBottled() || apci.getDeck()[k]->getType() == Curse) { continue; }
            int index = (apci.getDeck()[k]->getCardIDNum() - 1) * 2;
            if (apci.getDeck()[k]->getUpgraded()) ++index;
            deckIndices[j] = k;
            --msi_opts[j++].deckCountPerCard[index];
        }
        int secondRemove = deckIndices[callMacroIndexREROLLPRED(msi_opts, getPyFunc())];

        if (firstRemove > secondRemove)
        {
            aiEventActionBuffer.push_back(to_string(firstRemove+1));
            aiEventActionBuffer.push_back(to_string(secondRemove+1));
        }
        else
        {
            aiEventActionBuffer.push_back(to_string(secondRemove+1));
            aiEventActionBuffer.push_back(to_string(firstRemove+1));
        }
        break;
    }
    }
    storeBest;
}
void gamestate::makeShopChoices(vector<pair<relic*, int>> relics_, vector<pair<potion*, int>> potions_, vector<pair<C_DATABASE::iterator, int>> cards_, int remove_,bool check_map_)
{
    // first buy membership if there - always good
    // then remove if can and have curse and curse bad - allows ? decision making predicated on guaranteed removal at shop
    // how to makefaster? not sure, well whats currently happening, going through adding and doing const pred fro each pair
    // then rerolls for remove, each action goes throguh all mappaths, could do everthing together, but huge memory req

    // some ideas, macro should eventually know if having the gold is worth more than whatevers being bought, esp w/ maprep
    // cant go full depth, im not doing bool flip all over again
    // sensible depth seems to be all pair perms,or just single if not enough gold... so....
    int gold_ = apci.getGold();
    for (int i{ 0 }; i < relics_.size(); ++i)
    {
        // side note, late in run this could be a bad purchase, but only in the case where barely any gold, can if that
        // out later if needed, 
        if (relics_[i].first->getRelicIDNum() == 93 && relics_[i].second < gold_)
        {
            // buy and go again
            shopStuff perm;
            perm.r1 = relics_[i].first;
            static_cast<shop*>(nextEvent)->handleShopStuff(perm);
            return;
        }
    }
    if (apci.checkForCurse() && !apci.checkForRelic("Blue Candle") && !apci.checkForRelic("Du-Vu Doll") && gold_>=remove_)
    {
        // remove
        shopStuff perm;
        perm.rm = true;
        static_cast<shop*>(nextEvent)->handleShopStuff(perm);
        return;
    }
    for (int i{ 0 }; i < relics_.size(); ++i)
    {
        // always orrery, reasoning is its really good and hard to sim, even if sometimes other stuff is better, in general
        // its probably going to be worth, can change later
        if (relics_[i].first->getRelicIDNum() == 95 && relics_[i].second < gold_)
        {
            // buy and go again
            shopStuff perm;
            perm.r1 = relics_[i].first;
            static_cast<shop*>(nextEvent)->handleShopStuff(perm);
            return;
        }
    }
    int temp_gold = gold_;
    msi.createAllVars();
    vector<macroStateInfo> paths_;
    mapGen->discreteMapRep(getCurrentNode(), msi, paths_);
    // first item, second item, best val
    vector<shopStuff> perms;
    for (int i{ 0 }; i < relics_.size(); ++i)
    {
        if (relics_[i].second > temp_gold) continue;

        temp_gold = gold_;
        temp_gold -= relics_[i].second;
        macroStateInfo first_temp_msi = msi;
        addToRelicPairChoice(relics_[i].first, first_temp_msi);
        first_temp_msi.playerVars[10] = temp_gold;
        // single
        if (true)
        {
            shopStuff perm;
            if (check_map_)
            {
                for (int k{ 0 }; k < paths_.size(); ++k)
                {
                    paths_[k].copyNonMapRep(first_temp_msi);
                }
                perm.expval = callMacroValueCONSTPRED(paths_, getPyFunc());
            }
            else
            {
                perm.expval = callMacroSINGLE(first_temp_msi, getPyFunc());
            }
            //if (random_int(1, 30) == 30) perm.expval = random_float(10.0f, 50.0f);
            perm.r1 = relics_[i].first;
            //std::cout << perm.r1->getName() << ": " << perm.expval << endl;
            perms.push_back(perm);
        }
        for (int j{ 0 }; j < relics_.size(); ++j)
        {
            if (j <= i || relics_[j].second > temp_gold) continue;


            macroStateInfo temp_msi = first_temp_msi;
            addToRelicPairChoice(relics_[j].first, temp_msi);
            temp_msi.playerVars[10] -= relics_[j].second;
            shopStuff perm;
            if (check_map_)
            {
                for (int k{ 0 }; k < paths_.size(); ++k)
                {
                    paths_[k].copyNonMapRep(temp_msi);
                }
                perm.expval = callMacroValueCONSTPRED(paths_, getPyFunc());
            }
            else
            {
                perm.expval = callMacroSINGLE(temp_msi, getPyFunc());
            }
            //if (random_int(1, 30) == 30) perm.expval = random_float(10.0f, 50.0f);
            perm.r1 = relics_[i].first;
            perm.r2 = relics_[j].first;
            //std::cout << perm.r1->getName() << " and " << perm.r2->getName() << ": " << perm.expval << endl;
            perms.push_back(perm);
        }
        for (int j{ 0 }; j < potions_.size(); ++j)
        {
            if (potions_[j].second > temp_gold) continue;


            macroStateInfo temp_msi = first_temp_msi;
            ++temp_msi.potCounts[potions_[j].first->getPotionIDNum()];
            temp_msi.playerVars[10] -= potions_[j].second;
            shopStuff perm;
            if (check_map_)
            {
                for (int k{ 0 }; k < paths_.size(); ++k)
                {
                    paths_[k].copyNonMapRep(temp_msi);
                }
                perm.expval = callMacroValueCONSTPRED(paths_, getPyFunc());
            }
            else
            {
                perm.expval = callMacroSINGLE(temp_msi, getPyFunc());
            }
            //if (random_int(1, 30) == 30) perm.expval = random_float(10.0f, 50.0f);
            perm.r1 = relics_[i].first;
            perm.p2 = potions_[j].first;
            //std::cout << perm.r1->getName() << " and " << perm.p2->getName() << ": " << perm.expval << endl;
            perms.push_back(perm);
        }
        for (int j{ 0 }; j < cards_.size(); ++j)
        {
            if (cards_[j].second > temp_gold) continue;

            macroStateInfo temp_msi = first_temp_msi;
            int index_ = getIndexForAIAdd(cards_[j].first);
            ++temp_msi.deckCountPerCard[index_];
            temp_msi.playerVars[10] -= cards_[j].second;
            shopStuff perm;
            if (check_map_)
            {
                for (int k{ 0 }; k < paths_.size(); ++k)
                {
                    paths_[k].copyNonMapRep(temp_msi);
                }
                perm.expval = callMacroValueCONSTPRED(paths_, getPyFunc());
            }
            else
            {
                perm.expval = callMacroSINGLE(temp_msi, getPyFunc());
            }
            perm.r1 = relics_[i].first;
            if (cards_[j].first->second->getClass() == cardClass::Colourless) 
            {
                perm.cc2 = cards_[j].first; 
                //std::cout << perm.r1->getName() << " and " << perm.cc2->second->getName() << ": " << perm.expval << endl;
            }
            else 
            {
                perm.c2 = cards_[j].first; 
                //std::cout << perm.r1->getName() << " and " << perm.c2->second->getName() << ": " << perm.expval << endl;
            }
            //if (random_int(1, 30) == 30) perm.expval = random_float(10.0f, 50.0f);
            perms.push_back(perm);
        }
        if (remove_ <= temp_gold)
        {
            macroStateInfo temp_msi = first_temp_msi;
            
            temp_msi.playerVars[10] -= remove_;
            vector<macroStateInfo> msi_opts(apci.validRemoveCount(), temp_msi);
            vector<int> deckIndices(msi_opts.size());
            int j{ 0 };
            for (int k{ 0 }; k < apci.getDeck().size(); ++k)
            {
                if (apci.getDeck()[k]->getBottled() || apci.getDeck()[k]->getCardIDNum() == 80 || apci.getDeck()[k]->getCardIDNum() == 82) { continue; }
                int index = (apci.getDeck()[k]->getCardIDNum() - 1) * 2;
                if (apci.getDeck()[k]->getUpgraded()) ++index;
                deckIndices[j] = k;
                --msi_opts[j++].deckCountPerCard[index];
            }
            // call on all msis and return best index
            shopStuff perm;
            perm.r1 = relics_[i].first;
            perm.rm = true;
            perm.expval = 0;
            //perm.expval = callMacroValueREROLLPRED(msi_opts, getPyFunc());
            //std::cout << perm.r1->getName() << " and remove: " << perm.expval << endl;
            perms.push_back(perm);
        }
        temp_gold = gold_;
    }
    for (int i{ 0 }; i < potions_.size(); ++i)
    {
        if (potions_[i].second > temp_gold) continue;

        temp_gold = gold_;
        temp_gold -= potions_[i].second;
        macroStateInfo first_temp_msi = msi;
        ++first_temp_msi.potCounts[potions_[i].first->getPotionIDNum()];
        first_temp_msi.playerVars[10] = temp_gold;
        // single
        if (true)
        {
            shopStuff perm;
            if (check_map_)
            {
                for (int k{ 0 }; k < paths_.size(); ++k)
                {
                    paths_[k].copyNonMapRep(first_temp_msi);
                }
                perm.expval = callMacroValueCONSTPRED(paths_, getPyFunc());
            }
            else
            {
                perm.expval = callMacroSINGLE(first_temp_msi, getPyFunc());
            }
            perm.p1 = potions_[i].first;
            //std::cout << perm.p1->getName() << ": " << perm.expval << endl;
            perms.push_back(perm);
        }
        for (int j{ 0 }; j < potions_.size(); ++j)
        {
            if ( j <= i || potions_[j].second > temp_gold || apci.getPotionSlots() - apci.getPotions().size() == 1) continue;


            macroStateInfo temp_msi = first_temp_msi;
            ++temp_msi.potCounts[potions_[j].first->getPotionIDNum()];
            temp_msi.playerVars[10] -= potions_[j].second;
            shopStuff perm;
            if (check_map_)
            {
                for (int k{ 0 }; k < paths_.size(); ++k)
                {
                    paths_[k].copyNonMapRep(temp_msi);
                }
                perm.expval = callMacroValueCONSTPRED(paths_, getPyFunc());
            }
            else
            {
                perm.expval = callMacroSINGLE(temp_msi, getPyFunc());
            }
            perm.p1 = potions_[i].first;
            perm.p2 = potions_[j].first;
            //std::cout << perm.p1->getName() << " and " << perm.p2->getName() << ": " << perm.expval << endl;
            perms.push_back(perm);
        }
        for (int j{ 0 }; j < cards_.size(); ++j)
        {
            if (cards_[j].second > temp_gold) continue;


            macroStateInfo temp_msi = first_temp_msi;
            int index_ = getIndexForAIAdd(cards_[j].first);
            ++temp_msi.deckCountPerCard[index_];
            temp_msi.playerVars[10] -= cards_[j].second;
            shopStuff perm;
            if (check_map_)
            {
                for (int k{ 0 }; k < paths_.size(); ++k)
                {
                    paths_[k].copyNonMapRep(temp_msi);
                }
                perm.expval = callMacroValueCONSTPRED(paths_, getPyFunc());
            }
            else
            {
                perm.expval = callMacroSINGLE(temp_msi, getPyFunc());
            }
            perm.p1 = potions_[i].first;
            if (cards_[j].first->second->getClass() == cardClass::Colourless)
            {
                perm.cc2 = cards_[j].first; 
                //std::cout << perm.p1->getName() << " and " << perm.cc2->second->getName() << ": " << perm.expval << endl;
            }
            else 
            {
                perm.c2 = cards_[j].first;
                //std::cout << perm.p1->getName() << " and " << perm.c2->second->getName() << ": " << perm.expval << endl;
            }
            perms.push_back(perm);
        }
        if (remove_ <= temp_gold)
        {
            macroStateInfo temp_msi = first_temp_msi;

            temp_msi.playerVars[10] -= remove_;
            vector<macroStateInfo> msi_opts(apci.validRemoveCount(), temp_msi);
            vector<int> deckIndices(msi_opts.size());
            int j{ 0 };
            for (int k{ 0 }; k < apci.getDeck().size(); ++k)
            {
                if (apci.getDeck()[k]->getBottled() || apci.getDeck()[k]->getCardIDNum() == 80 || apci.getDeck()[k]->getCardIDNum() == 82) { continue; }
                int index = (apci.getDeck()[k]->getCardIDNum() - 1) * 2;
                if (apci.getDeck()[k]->getUpgraded()) ++index;
                deckIndices[j] = k;
                --msi_opts[j++].deckCountPerCard[index];
            }
            // call on all msis and return best index
            shopStuff perm;
            perm.p1 = potions_[i].first;
            perm.rm = true;
            perm.expval = 0;
            //perm.expval = callMacroValueREROLLPRED(msi_opts, getPyFunc());
            //std::cout << perm.p1->getName() << " and remove: " << perm.expval << endl;
            perms.push_back(perm);
        }
        temp_gold = gold_;
    }
    for (int i{ 0 }; i < cards_.size(); ++i)
    {
        if ( cards_[i].second > temp_gold) continue;

        temp_gold = gold_;
        int first_index_ = getIndexForAIAdd(cards_[i].first);
        temp_gold -= cards_[i].second;
        macroStateInfo first_temp_msi = msi;
        ++first_temp_msi.deckCountPerCard[first_index_];
        first_temp_msi.playerVars[10] = temp_gold;
        // single
        if (true)
        {
            shopStuff perm;
            if (check_map_)
            {
                for (int k{ 0 }; k < paths_.size(); ++k)
                {
                    paths_[k].copyNonMapRep(first_temp_msi);
                }
                perm.expval = callMacroValueCONSTPRED(paths_, getPyFunc());
            }
            else
            {
                perm.expval = callMacroSINGLE(first_temp_msi, getPyFunc());
            }
            if (cards_[i].first->second->getClass() == cardClass::Colourless) 
            {
                perm.cc1 = cards_[i].first; 
                //std::cout << perm.cc1->second->getName() << ": " << perm.expval << endl;
            }
            else
            {
                perm.c1 = cards_[i].first; 
                //std::cout << perm.c1->second->getName() << ": " << perm.expval << endl;
            }
            perms.push_back(perm);
        }
        for (int j{ 0 }; j < cards_.size(); ++j)
        {
            if (j <= i || cards_[j].second > temp_gold) continue;


            macroStateInfo temp_msi = first_temp_msi;
            int index_ = getIndexForAIAdd(cards_[j].first);
            ++temp_msi.deckCountPerCard[index_];
            temp_msi.playerVars[10] -= cards_[j].second;
            shopStuff perm;
            if (check_map_)
            {
                for (int k{ 0 }; k < paths_.size(); ++k)
                {
                    paths_[k].copyNonMapRep(temp_msi);
                }
                perm.expval = callMacroValueCONSTPRED(paths_, getPyFunc());
            }
            else
            {
                perm.expval = callMacroSINGLE(temp_msi, getPyFunc());
            }
            if (cards_[i].first->second->getClass() == cardClass::Colourless) 
            {
                perm.cc1 = cards_[i].first;
                //std::cout << perm.cc1->second->getName();
            }
            else 
            {
                perm.c1 = cards_[i].first;
                //std::cout << perm.c1->second->getName();
            }
            if (cards_[j].first->second->getClass() == cardClass::Colourless) 
            {
                perm.cc2 = cards_[j].first;
                //std::cout << " and " << perm.cc2->second->getName() << ": " << perm.expval << endl;
            }
            else
            {
                perm.c2 = cards_[j].first; 
                //std::cout << " and " << perm.c2->second->getName() << ": " << perm.expval << endl;
            }
            perms.push_back(perm);
        }
        if (remove_ <= temp_gold)
        {
            macroStateInfo temp_msi = first_temp_msi;

            temp_msi.playerVars[10] -= remove_;
            vector<macroStateInfo> msi_opts(apci.validRemoveCount(), temp_msi);
            vector<int> deckIndices(msi_opts.size());
            int j{ 0 };
            for (int k{ 0 }; k < apci.getDeck().size(); ++k)
            {
                if (apci.getDeck()[k]->getBottled() || apci.getDeck()[k]->getCardIDNum() == 80 || apci.getDeck()[k]->getCardIDNum() == 82) { continue; }
                int index = (apci.getDeck()[k]->getCardIDNum() - 1) * 2;
                if (apci.getDeck()[k]->getUpgraded()) ++index;
                deckIndices[j] = k;
                --msi_opts[j++].deckCountPerCard[index];
            }
            // call on all msis and return best index
            shopStuff perm;
            //perm.expval = callMacroValueREROLLPRED(msi_opts, getPyFunc());
            perm.expval = 0;
            if (cards_[i].first->second->getClass() == cardClass::Colourless) 
            {
                perm.cc1 = cards_[i].first; 
                //std::cout << perm.cc1->second->getName() << " and remove: " << perm.expval << endl;
            }
            else 
            {
                perm.c1 = cards_[i].first;
                //std::cout << perm.c1->second->getName() << " and remove: " << perm.expval << endl;
            }
            perm.rm = true;
            perms.push_back(perm);
        }
        temp_gold = gold_;
    }
    if (remove_ <= gold_)
    {
        //single remove
        temp_gold -= remove_;
        macroStateInfo temp_msi = msi;
        temp_msi.playerVars[10] = temp_gold;

        vector<macroStateInfo> msi_opts(apci.validRemoveCount(), temp_msi);
        vector<int> deckIndices(msi_opts.size());
        int j{ 0 };
        for (int i{ 0 }; i < apci.getDeck().size(); ++i)
        {
            if (apci.getDeck()[i]->getBottled() || apci.getDeck()[i]->getCardIDNum() == 80 || apci.getDeck()[i]->getCardIDNum() == 82) { continue; }
            int index = (apci.getDeck()[i]->getCardIDNum() - 1) * 2;
            if (apci.getDeck()[i]->getUpgraded()) ++index;
            deckIndices[j] = i;
            --msi_opts[j++].deckCountPerCard[index];
        }
        // call on all msis and return best index
        shopStuff perm;
        perm.rm = true;
        perm.expval = 0;
        //perm.expval = callMacroValueREROLLPRED(msi_opts, getPyFunc());
        //std::cout << "remove: " << perm.expval << endl;
        perms.push_back(perm);
    }
    // leave
    for (int k{ 0 }; k < paths_.size(); ++k)
    {
        paths_[k].copyNonMapRep(msi);
    }
    shopStuff perm;
    perm.expval = callMacroValueCONSTPRED(paths_, getPyFunc());
    perms.push_back(perm);
    //
    pair<int, float> bestPerm{0,0.0f};
    for (int i{ 0 }; i < perms.size(); ++i)
    {
        if (perms[i].expval > bestPerm.second)
        {
            bestPerm = { i, perms[i].expval };
            if (i == perms.size() - 1)
            {
                aiEventActionBuffer.push_back("leave");
                return;
            }
        }
    }
    static_cast<shop*>(nextEvent)->handleShopStuff(perms[bestPerm.first]);
}
void gamestate::makeCampfireChoices(bool rest_, bool upgrade_, bool girya_, bool pipe_, bool shovel_)
{
    msi.createAllVars();
    vector<macroStateInfo> paths_;
    mapGen->discreteMapRep(getCurrentNode(), msi, paths_);
    vector<float> bestCH(5);
    if (rest_)
    {
        macroStateInfo temp_msi = msi;
        int newHP = apci.getPlayer().getHealth() + (apci.getPlayer().getHealth() * 30 / 100);
        if (apci.checkForRelic("Regal Pillow")) newHP += 15;
        temp_msi.playerVars[0] = clamp(newHP, 0, apci.getPlayer().getMaxHealth());

        for (int k{ 0 }; k < paths_.size(); ++k)
        {
            paths_[k].copyNonMapRep(temp_msi);
        }
        bestCH[0] = callMacroValueCONSTPRED(paths_, getPyFunc());
    }
    if (upgrade_)
    {
        int j{ 0 };
        vector<macroStateInfo> msi_opts(apci.validUpgradeCount(), msi);
        for (int i{ 0 }; i < apci.getDeck().size(); ++i)
        {
            if (apci.getDeck()[i]->getUpgraded() || apci.getDeck()[i]->getType() == Curse) { continue; }
            int index = (apci.getDeck()[i]->getCardIDNum() - 1) * 2;
            ++msi_opts[j].deckCountPerCard[index + 1];
            --msi_opts[j].deckCountPerCard[index];

            for (int k{ 0 }; k < paths_.size(); ++k)
            {
                paths_[k].copyNonMapRep(msi_opts[j]);
            }
            float potential = callMacroValueCONSTPRED(paths_, getPyFunc());
            if (potential > bestCH[1]) bestCH[1] = potential;
            ++j;
        }
    }
    if (girya_)
    {
        macroStateInfo temp_msi = msi;
        ++temp_msi.relicCounters[68];
        for (int k{ 0 }; k < paths_.size(); ++k)
        {
            paths_[k].copyNonMapRep(temp_msi);
        }
        bestCH[2] = callMacroValueCONSTPRED(paths_, getPyFunc());
    }
    if (pipe_)
    {
        int j{ 0 };
        vector<macroStateInfo> msi_opts(apci.validRemoveCount(), msi);
        for (int i{ 0 }; i < apci.getDeck().size(); ++i)
        {
            if (apci.getDeck()[i]->getBottled() || apci.getDeck()[i]->getCardIDNum() == 80 || apci.getDeck()[i]->getCardIDNum() == 82) { continue; }
            int index = (apci.getDeck()[i]->getCardIDNum() - 1) * 2;
            if (apci.getDeck()[i]->getUpgraded()) ++index;
            --msi_opts[j].deckCountPerCard[index];
            for (int k{ 0 }; k < paths_.size(); ++k)
            {
                paths_[k].copyNonMapRep(msi_opts[j]);
            }
            float potential = callMacroValueCONSTPRED(paths_, getPyFunc());
            if (potential > bestCH[3]) bestCH[3] = potential;
            ++j;
        }
    }
    if (shovel_)
    {
        vector<int> takenID;
        for (int i{ 0 }; i < apci.getRelics().size(); ++i)
        {
            takenID.push_back(apci.getRelics()[i]->getRelicIDNum());
        }
        vector<int> optsRels;
        rngRelicAddID(takenID, optsRels, 10);
        vector<macroStateInfo> msi_opts(10, msi);

        vector<float> potentials(10);
        for (int i{ 0 }; i < optsRels.size(); ++i)
        {
            addToRelicPairChoice(optsRels[i], msi_opts[i]);
            for (int k{ 0 }; k < paths_.size(); ++k)
            {
                paths_[k].copyNonMapRep(msi_opts[i]);
            }
            float potential = callMacroValueCONSTPRED(paths_, getPyFunc());
            potentials[i] = potential;
        }
        for (int i{ 0 }; i < potentials.size(); ++i)
        {
            bestCH[4] += potentials[i];
        }
        bestCH[4] /= potentials.size();
    }
    float best_{ 0.0 };
    for (int i{ 0 }; i < bestCH.size(); ++i)
    {
        if (bestCH[i] > best_) best_ = bestCH[i];
    }
    vector<bool> res(5,false);
    for (int i{ 0 }; i < bestCH.size(); ++i)
    {
        if (bestCH[i] == best_) res[i] = true;
    }
    static_cast<campfire*>(nextEvent)->aiCampfireDecision(res);
}
void gamestate::makeNeowChoices(int first_, int second_, int thirdG_, int thirdB_)
{
    msi.createAllVars();
    vector<macroStateInfo> paths_;
    mapGen->discreteMapRep(getCurrentNode(), msi, paths_);
    vector<int> opCH(24);
    vector<float> potentials(4);
    for (int i{ 0 }; i < 4; ++i)
    {
        switch (i)
        {
        case 0:
            ++opCH[first_];
            for (int k{ 0 }; k < paths_.size(); ++k)
            {
                paths_[k].openingChoice = opCH;
            }
            potentials[i] = callMacroValueCONSTPRED(paths_, getPyFunc());
            --opCH[first_];
            break;
        case 1:
            ++opCH[second_];
            for (int k{ 0 }; k < paths_.size(); ++k)
            {
                paths_[k].openingChoice = opCH;
            }
            potentials[i] = callMacroValueCONSTPRED(paths_, getPyFunc());
            --opCH[second_];
            break;
        case 2:
            ++opCH[thirdB_ + 11];
            ++opCH[thirdG_ + 15];
            for (int k{ 0 }; k < paths_.size(); ++k)
            {
                paths_[k].openingChoice = opCH;
            }
            potentials[i] = callMacroValueCONSTPRED(paths_, getPyFunc());
            --opCH[thirdB_ + 11];
            --opCH[thirdG_ + 15];
            break;
        case 3:
            ++opCH[22];
            for (int k{ 0 }; k < paths_.size(); ++k)
            {
                paths_[k].openingChoice = opCH;
            }
            potentials[i] = callMacroValueCONSTPRED(paths_, getPyFunc());
            --opCH[22];
            break;
        }
    }
    pair<float, int> best{0.0f,0};
    for (int i{ 0 }; i < potentials.size(); ++i)
    {
        if (potentials[i] > best.first) { best.first = potentials[i]; best.second = i; }
    }
    aiEventActionBuffer.push_back(to_string(best.second + 1));
    msi.mapRepZeroes();
}

void gamestate::makeBossRelicChoices(vector<relic*> relicOptions)
{
    msi.createAllVars();
    msi.mapRepZeroes();
    vector<float> bestOpts(3);
    for (int i{ 0 }; i < 3; ++i)
    {
        macroStateInfo temp_msi = msi;
        if (dynamic_cast<bossRelic*>(relicOptions[i])->getEnergyRelic()) ++temp_msi.playerVars[2];
        ++temp_msi.relicPresent[relicOptions[i]->getRelicIDNum() - 1];
        switch (relicOptions[i]->getRelicIDNum())
        {
        case 117: // astrolabe
        {
            int firstRemove{ 0 };
            for (int k{ 0 }; k < apci.getDeck().size(); ++k)
            {
                if (apci.getDeck()[k]->getBottled() || apci.getDeck()[k]->getCardIDNum() == 80 || apci.getDeck()[k]->getCardIDNum() == 82) { continue; }
                int index = (apci.getDeck()[k]->getCardIDNum() - 1) * 2;
                --temp_msi.deckCountPerCard[index];
                float potential = callMacroSINGLE(temp_msi, getPyFunc());
                if (potential > bestOpts[i])
                {
                    bestOpts[i] = potential;
                    firstRemove = k;
                }
                ++temp_msi.deckCountPerCard[index];
            }
            --temp_msi.deckCountPerCard[(apci.getDeck()[firstRemove]->getCardIDNum() - 1) * 2];
            bestOpts[i] = 0;
            int secondRemove{ 0 };
            for (int k{ 0 }; k < apci.getDeck().size(); ++k)
            {
                if (firstRemove == k) continue;
                if (apci.getDeck()[k]->getBottled() || apci.getDeck()[k]->getType() == Curse) { continue; }
                int index = (apci.getDeck()[k]->getCardIDNum() - 1) * 2;
                --temp_msi.deckCountPerCard[index];

                float potential = callMacroSINGLE(temp_msi, getPyFunc());
                if (potential > bestOpts[i])
                {
                    bestOpts[i] = potential;
                    secondRemove = k;
                }
                ++temp_msi.deckCountPerCard[index];
            }
            --temp_msi.deckCountPerCard[(apci.getDeck()[secondRemove]->getCardIDNum() - 1) * 2];
            bestOpts[i] = 0;
            int thirdRemove{ 0 };
            for (int k{ 0 }; k < apci.getDeck().size(); ++k)
            {
                if (firstRemove == k || secondRemove == k) continue;
                if (apci.getDeck()[k]->getBottled() || apci.getDeck()[k]->getType() == Curse) { continue; }
                int index = (apci.getDeck()[k]->getCardIDNum() - 1) * 2;
                --temp_msi.deckCountPerCard[index];

                float potential = callMacroSINGLE(temp_msi, getPyFunc());
                if (potential > bestOpts[i])
                {
                    bestOpts[i] = potential;
                    thirdRemove = k;
                }
                ++temp_msi.deckCountPerCard[index];
            }
            --temp_msi.deckCountPerCard[(apci.getDeck()[thirdRemove]->getCardIDNum() - 1) * 2];
            vector<float> potentials(10);
            vector<macroStateInfo> msi_opts(10, temp_msi);
            for (int j{ 0 }; j < 10; ++j)
            {
                vector<int> optsCard;
                rngCardAddID(optsCard, 3);
                for (int k{ 0 }; k < optsCard.size(); ++k)
                {
                    ++msi_opts[j].deckCountPerCard[optsCard[k] - 1];
                }
                potentials[j] = callMacroSINGLE(msi_opts[j], getPyFunc());
            }
            for (int j{ 0 }; j < potentials.size(); ++j)
            {
                bestOpts[i] += potentials[j];
            }
            bestOpts[i] /= potentials.size();
            break;
        }
        case 120: // calling bell
        {
            vector<int> takenID;
            for (int j{ 0 }; j < apci.getRelics().size(); ++j)
            {
                takenID.push_back(apci.getRelics()[j]->getRelicIDNum());
            }
            vector<int> optsRels;
            ++temp_msi.deckCountPerCard[162];
            vector<macroStateInfo> msi_opts(10,temp_msi);
            vector<float> potentials(10);
            rngRelicAddID(takenID, optsRels, 30);
            for (int j{ 0 }; j < 10; ++j)
            {
                addToRelicPairChoice(optsRels[j], msi_opts[j]);
                addToRelicPairChoice(optsRels[j+10], msi_opts[j]);
                addToRelicPairChoice(optsRels[j+20], msi_opts[j]);
                float potential = callMacroSINGLE(msi_opts[j], getPyFunc());
                potentials.push_back(potential);
            }
            for (int j{ 0 }; j < potentials.size(); ++j)
            {
                bestOpts[i] += potentials[j];
            }
            bestOpts[i] /= potentials.size();
            break;
        }
        case 124: // empty cage
        {
            int firstRemove{ 0 };
            for (int k{ 0 }; k < apci.getDeck().size(); ++k)
            {
                if (apci.getDeck()[k]->getBottled() || apci.getDeck()[k]->getCardIDNum() == 80 || apci.getDeck()[k]->getCardIDNum() == 82) { continue; }
                int index = (apci.getDeck()[k]->getCardIDNum() - 1) * 2;
                --temp_msi.deckCountPerCard[index];
                float potential = callMacroSINGLE(temp_msi, getPyFunc());
                if (potential > bestOpts[i])
                {
                    bestOpts[i] = potential;
                    firstRemove = k;
                }
                ++temp_msi.deckCountPerCard[index];
            }
            --temp_msi.deckCountPerCard[(apci.getDeck()[firstRemove]->getCardIDNum() - 1) * 2];
            bestOpts[i] = 0;
            for (int k{ 0 }; k < apci.getDeck().size(); ++k)
            {
                if (firstRemove == k) continue;
                if (apci.getDeck()[k]->getBottled() || apci.getDeck()[k]->getCardIDNum() == 80 || apci.getDeck()[k]->getCardIDNum() == 82) { continue; }
                int index = (apci.getDeck()[k]->getCardIDNum() - 1) * 2;
                --temp_msi.deckCountPerCard[index];

                float potential = callMacroSINGLE(temp_msi, getPyFunc());
                if (potential > bestOpts[i])
                {
                    bestOpts[i] = potential;
                }
                ++temp_msi.deckCountPerCard[index];
            }
            break;
        }
        case 126: // pandoras box
        {
            vector<float> potentials(10);
            vector<macroStateInfo> msi_opts(10, temp_msi);
            for (int j{ 0 }; j < 10; ++j)
            {
                vector<int> optsCard;
                int length_{ 0 };
                for (int k{ 0 }; k < apci.getDeck().size(); ++k)
                {
                    if (apci.getDeck()[k]->getCardIDNum() == 1 || apci.getDeck()[k]->getCardIDNum() == 3) ++length_;
                }
                rngCardAddID(optsCard, length_);
                for (int k{ 0 }; k < optsCard.size(); ++k)
                {
                    ++msi_opts[j].deckCountPerCard[optsCard[k] - 1];
                }
                potentials[j] = callMacroSINGLE(msi_opts[j], getPyFunc());
            }
            for (int j{ 0 }; j < potentials.size(); ++j)
            {
                bestOpts[i] += potentials[j];
            }
            bestOpts[i] /= potentials.size();
            break;
        }
        case 130: // snecko
        {
            temp_msi.playerVars[6] += 2;
            bestOpts[i] = callMacroSINGLE(temp_msi, getPyFunc());
            break;
        }
        case 132: // tiny house
        {
            if (!apci.potionsFull()) ++temp_msi.potCounts[random_int(0, 33)];
            temp_msi.playerVars[10] += 50;
            temp_msi.playerVars[0] += 5;
            temp_msi.playerVars[1] += 5;

            int index = apci.randomUpgradeID();
            --temp_msi.deckCountPerCard[(apci.getDeck()[index]->getCardIDNum() - 1) * 2];
            ++temp_msi.deckCountPerCard[(apci.getDeck()[index]->getCardIDNum() - 1) * 2 + 1];
            vector<int> cardOpts;
            rngCardAddID(cardOpts, 1);
            ++temp_msi.deckCountPerCard[(cardOpts[0] - 1) * 2];
            bestOpts[i] = callMacroSINGLE(temp_msi, getPyFunc());
            break;
        }
        default:
            bestOpts[i] = callMacroSINGLE(temp_msi, getPyFunc());
            break;
        }
    }
    int best_{ 0 };
    for (int i{ 0 }; i < bestOpts.size(); ++i)
    {
        if (bestOpts[i] > best_) best_ = i;
    }
    aiEventActionBuffer.push_back(to_string(best_ + 1));
}
int gamestate::getIndexForAIAdd(C_DATABASE::iterator _card)
{
    int index_ = _card->second->getCardIDNum();
    if (_card->second->getUpgraded())
    {
        ++index_;
    }
    else
    {
        switch (_card->second->getType())
        {
        case Attack:
            if (apci.checkForRelic("Molten Egg")) ++index_;
            break;
        case Skill:
            if (apci.checkForRelic("Toxic Egg")) ++index_;
            break;
        case Power:
            if (apci.checkForRelic("Frozen Egg")) ++index_;
            break;
        }
    }
    return index_;
}

void gamestate::addToRelicPairChoice(relic* other_relic, macroStateInfo& temp_msi)
{
    int r_id = other_relic->getRelicIDNum();
    addToRelicPairChoice(r_id, temp_msi);
}
void gamestate::addToRelicPairChoice(int r_id, macroStateInfo& temp_msi)
{
    ++temp_msi.relicPresent[r_id - 1];
    switch (r_id)
    {
    case 26: // strawbery
        temp_msi.playerVars[0] += 7;
        temp_msi.playerVars[1] += 7;
        break;
    case 52: // pear
        temp_msi.playerVars[0] += 10;
        temp_msi.playerVars[1] += 10;
        break;
    case 73: // mango
        temp_msi.playerVars[0] += 14;
        temp_msi.playerVars[1] += 14;
        break;
    case 91: // waffle
        temp_msi.playerVars[1] += 7;
        temp_msi.playerVars[0] = temp_msi.playerVars[1];
        break;
    case 31: // war paint
    {
        vector<int> rng_upgrades = apci.twoRandomSkillIDs();
        for (int j{ 0 }; j < rng_upgrades.size(); ++j)
        {
            --temp_msi.deckCountPerCard[rng_upgrades[j]];
            ++temp_msi.deckCountPerCard[rng_upgrades[j] + 1];
        }
        break;
    }
    case 32: // whetstone
    {
        vector<int> rng_upgrades = apci.twoRandomAttackIDs();
        for (int j{ 0 }; j < rng_upgrades.size(); ++j)
        {
            --temp_msi.deckCountPerCard[rng_upgrades[j]];
            ++temp_msi.deckCountPerCard[rng_upgrades[j] + 1];
        }
        break;
    }
    case 88: // cauldron
    {
        int to_add = apci.getPotionSlots() - static_cast<int>(apci.getPotions().size());
        while (to_add-- > 0)
        {
            ++temp_msi.potCounts[random_int(0, temp_msi.potCounts.size() - 1)];
        }
        break;
    }
    }
}
void gamestate::makePathChoiceFromNeutral(bool store)
{
    if (actNumber == 4)
    {
        msi.createAllVars();
        msi.mapRepZeroes();
        if (store) storedMSIs.push_back(msi);
        return;
    }
    msi.createAllVars();
    vector<macroStateInfo> paths_;
    map<int,int> directions_ = mapGen->discreteMapRep(getCurrentNode(), msi, paths_,true);
    int bestMSI = callMacroIndexCONSTPRED(paths_, getPyFunc());
    if(store) aiEventActionBuffer.push_back(to_string(directions_[bestMSI]+1));
    msi = paths_[bestMSI];
    if(store) storedMSIs.push_back(msi);
}
int gamestate::tierListAddChoice()
{
    pair<int, int> best = { 0,0 };
    for (int i{ 0 }; i < aiCardChoices.size(); ++i)
    {
        if (card::tierList[aiCardChoices[i]->second->getCardIDNum()] > best.first) best = { card::tierList[aiCardChoices[i]->second->getCardIDNum()] , i };
    }
    return best.second + 1;
}
int gamestate::tierListRemoveChoice()
{
    pair<int, int> worst = { 10,0 };
    for (int i{ 0 }; i < apci.getDeck().size(); ++i)
    {
        if (apci.getDeck()[i]->getBottled() || apci.getDeck()[i]->getCardIDNum() == 80 || apci.getDeck()[i]->getCardIDNum() == 82 || apci.getDeck()[i]->getClass() == cardClass::Colourless) continue;
        if (card::tierList[apci.getDeck()[i]->getCardIDNum()] < worst.first) worst = { card::tierList[apci.getDeck()[i]->getCardIDNum()] , i };
    }
    return worst.second + 1;
}
int gamestate::tierListUpgradeChoice()
{
    pair<int, int> best = { 0,0 };
    for (int i = static_cast<int>(apci.getDeck().size()) - 1; i >= 0; --i)
    {
        if (apci.getDeck()[i]->getUpgraded() || apci.getDeck()[i]->getCardIDNum() == Curse || apci.getDeck()[i]->getClass() == cardClass::Colourless) continue;
        if (card::tierList[apci.getDeck()[i]->getCardIDNum()] > best.first) best = { card::tierList[apci.getDeck()[i]->getCardIDNum()] , i };
    }
    return best.second + 1;
}


focusedEvent* gamestate::getCardDrawn()     {if(!temporaryFE[0]){temporaryFE[0] = new focusedEvent("fe: cardDrawn");} return temporaryFE[0];}
focusedEvent* gamestate::getCardPlayed()    {if(!temporaryFE[1]){temporaryFE[1] = new focusedEvent("fe: cardPlayed");}return temporaryFE[1];}
focusedEvent* gamestate::getAttackPlayed()  { getCardPlayed(); if(!temporaryFE[2]){temporaryFE[2] = new focusedEvent("fe: attackPlayed");}return temporaryFE[2];}
focusedEvent* gamestate::getSkillPlayed()   { getCardPlayed(); if(!temporaryFE[3]){temporaryFE[3] = new focusedEvent("fe: skillPlayed");}return temporaryFE[3];}
focusedEvent* gamestate::getPowerPlayed()   { getCardPlayed(); if(!temporaryFE[4]){temporaryFE[4] = new focusedEvent("fe: powerPlayed");}return temporaryFE[4];}
focusedEvent* gamestate::getCardExhausted() {if(!temporaryFE[5]){temporaryFE[5] = new focusedEvent("fe: cardExhausted");}return temporaryFE[5];}

focusedEvent* gamestate::getPlayerTurnStart()   {if(!temporaryFE[6]){temporaryFE[6] = new focusedEvent("fe: playerTurnStart");}return temporaryFE[6];}
focusedEvent* gamestate::getPlayerTurnEnd()     {if(!temporaryFE[7]){temporaryFE[7] = new focusedEvent("fe: playerTurnEnd");}return temporaryFE[7];}
focusedEvent* gamestate::getEnemyTurnStart()    {if(!temporaryFE[8]){temporaryFE[8] = new focusedEvent("fe: enemyTurnStart");}return temporaryFE[8];}
focusedEvent* gamestate::getEnemyTurnEnd()      {if(!temporaryFE[9]){temporaryFE[9] = new focusedEvent("fe: enemyTurnEnd");}return temporaryFE[9];}

focusedEvent* gamestate::getPlayerAboutToLoseHealth()   {if(!temporaryFE[10]){temporaryFE[10] = new focusedEvent("fe: playerAboutToLoseHealth");}return temporaryFE[10];}
focusedEvent* gamestate::getPlayerHealthGain()          {if(!temporaryFE[11]){temporaryFE[11] = new focusedEvent("fe: playerHealthGain");}return temporaryFE[11];}
focusedEvent* gamestate::getPlayerHealthLoss()          {if(!temporaryFE[12]){temporaryFE[12] = new focusedEvent("fe: playerHealthLoss");}return temporaryFE[12];}
focusedEvent* gamestate::getPlayerTakingAttackDamage()  {if(!temporaryFE[13]){temporaryFE[13] = new focusedEvent("fe: playerTakingAttackDamage");}return temporaryFE[13];}
focusedEvent* gamestate::getEnemyHealthLoss()           {if(!temporaryFE[14]){temporaryFE[14] = new focusedEvent("fe: enemyHealthLoss");}return temporaryFE[14];}
focusedEvent* gamestate::getEnemyTakingAttackDamage()   {if(!temporaryFE[15]){temporaryFE[15] = new focusedEvent("fe: enemyTakingAttackDamage");}return temporaryFE[15];}

focusedEvent* gamestate::getAlterBaseADFromPlayer()     {if(!temporaryFE[16]){temporaryFE[16] = new focusedEvent("fe: alterBaseADFromPlayer");} return temporaryFE[16];}
focusedEvent* gamestate::getAlterBaseADFromEnemy()      {if(!temporaryFE[17]){temporaryFE[17] = new focusedEvent("fe: alterBaseADFromEnemy");} return temporaryFE[17];}
focusedEvent* gamestate::getAlterAdjustedADFromPlayer() {if(!temporaryFE[18]){temporaryFE[18] = new focusedEvent("fe: alterAdjustedADFromPlayer");} return temporaryFE[18];}
focusedEvent* gamestate::getAlterAdjustedADFromEnemy()  {if(!temporaryFE[19]){temporaryFE[19] = new focusedEvent("fe: alterAdjustedADFromEnemy");} return temporaryFE[19];}

focusedEvent* gamestate::getGainBlock()         {if(!temporaryFE[20]){temporaryFE[20] = new focusedEvent("fe: gainBlock");}return temporaryFE[20];}
focusedEvent* gamestate::getApplyingVulnerable(){if(!temporaryFE[21]){temporaryFE[21] = new focusedEvent("fe: applyingVulnerable");}return temporaryFE[21];}
focusedEvent* gamestate::getApplyingFrail()     {if(!temporaryFE[22]){temporaryFE[22] = new focusedEvent("fe: applyingFrail");}return temporaryFE[22];}
focusedEvent* gamestate::getApplyingWeak()      {if(!temporaryFE[23]){temporaryFE[23] = new focusedEvent("fe: applyingWeak");}return temporaryFE[23];}
focusedEvent* gamestate::getApplyingDebuff()    {if(!temporaryFE[24]){temporaryFE[24] = new focusedEvent("fe: applyingDebuff");}return temporaryFE[24];}

focusedEvent* gamestate::getBlockBroken()   {if(!temporaryFE[25]){temporaryFE[25] = new focusedEvent("fe: blockBroken");}return temporaryFE[25];}
focusedEvent* gamestate::getSOTBlock()      {if(!temporaryFE[26]){temporaryFE[26] = new focusedEvent("fe: sotBlock");}return temporaryFE[26];}
focusedEvent* gamestate::getShuffledDraw()  {if(!temporaryFE[27]){temporaryFE[27] = new focusedEvent("fe: shuffledDraw");}return temporaryFE[27];}
focusedEvent* gamestate::getFightEnd()      {if(!temporaryFE[28]){temporaryFE[28] = new focusedEvent("fe: fightEnd");}return temporaryFE[28];}
focusedEvent* gamestate::getEnemyDied()     {if(!temporaryFE[29]){temporaryFE[29] = new focusedEvent("fe: enemyDied");}return temporaryFE[29];}
focusedEvent* gamestate::getEnemyFatal()     {if(!temporaryFE[30]){temporaryFE[30] = new focusedEvent("fe: enemyFatal");}return temporaryFE[30];}

focusedEvent* gamestate::getNewFloorEntered()   {if(!persistentFE[0]){persistentFE[0] = new focusedEvent("fe: newFloorEntered");}return persistentFE[0];}
focusedEvent* gamestate::getCampfireEntered()   {if(!persistentFE[1]){persistentFE[1] = new focusedEvent("fe: campfireEntered");}return persistentFE[1];}
focusedEvent* gamestate::getCampfireRest()      {if(!persistentFE[2]){persistentFE[2] = new focusedEvent("fe: campfireRest");}return persistentFE[2];}
focusedEvent* gamestate::getShopEntered()       {if(!persistentFE[3]){persistentFE[3] = new focusedEvent("fe: shopEntered");}return persistentFE[3];}
focusedEvent* gamestate::getQuesitonEntered()   {if(!persistentFE[4]){persistentFE[4] = new focusedEvent("fe: quesitonEntered");}return persistentFE[4];}
focusedEvent* gamestate::getChestOpened()       {if(!persistentFE[5]){persistentFE[5] = new focusedEvent("fe: chestOpened");}return persistentFE[5];}
focusedEvent* gamestate::getCardAdded()         {if(!persistentFE[6]){persistentFE[6] = new focusedEvent("fe: cardAdded");}return persistentFE[6];}
focusedEvent* gamestate::getGoldSpent()         {if(!persistentFE[7]){persistentFE[7] = new focusedEvent("fe: goldSpent");}return persistentFE[7];}
focusedEvent* gamestate::getGoldGained()        {if(!persistentFE[8]){persistentFE[8] = new focusedEvent("fe: goldGained");}return persistentFE[8];}
focusedEvent* gamestate::getPotionUsed()        {if(!persistentFE[9]){persistentFE[9] = new focusedEvent("fe: potionUsed");}return persistentFE[9];}
focusedEvent* gamestate::getPlayerDied()        {if(!persistentFE[10]){persistentFE[10] = new focusedEvent("fe: playerDied");}return persistentFE[10];}

void gamestate::callCardDrawn(cardHolder& ch){if(temporaryFE[0]) temporaryFE[0]->notify(ch);}
void gamestate::callCardPlayed(cardHolder& ch)
{
    ++cardCount;
    if (!temporaryFE[1]) return;
    observer_set currentTriggers = temporaryFE[1]->getObservers();
    observer_set::iterator obs;
    switch((*ch).getType())
    {
        case cardType::Attack :
        {
            ++attackCount;
            if (temporaryFE[2]) 
            {
                observer_set currentTriggersAtt = temporaryFE[2]->getObservers();
                for (auto itr{ currentTriggersAtt.begin() }; itr != currentTriggersAtt.end(); ++itr)
                {
                    currentTriggers.insert(*itr);
                }
            }
            obs = currentTriggers.begin();
            while (obs != currentTriggers.end())
            {
                (*obs)->triggeredACT(ch);
                ++obs;
            }
            break;
        }
        case cardType::Skill :
        {
            ++skillCount;
            if (temporaryFE[3])
            {
                observer_set currentTriggersSkl = temporaryFE[3]->getObservers();
                for (auto itr{ currentTriggersSkl.begin() }; itr != currentTriggersSkl.end(); ++itr)
                {
                    currentTriggers.insert(*itr);
                }
            }
            obs = currentTriggers.begin();
            while (obs != currentTriggers.end())
            {
                (*obs)->triggeredACT(ch);
                ++obs;
            }
            break;
        }
        case cardType::Power :
        {
            ++powerCount;
            if (temporaryFE[4])
            {
                observer_set currentTriggersPwr = temporaryFE[4]->getObservers();
                for (auto itr{ currentTriggersPwr.begin() }; itr != currentTriggersPwr.end(); ++itr)
                {
                    currentTriggers.insert(*itr);
                }
            }
            obs = currentTriggers.begin();
            while (obs != currentTriggers.end())
            {
                (*obs)->triggeredACT(ch);
                ++obs;
            }
            break;
        }
    }
}
void gamestate::callCardExhausted(cardHolder& ch){if(temporaryFE[5]) temporaryFE[5]->notify(ch);}

void gamestate::callPlayerTurnStart()   {if(temporaryFE[6]) temporaryFE[6]->notify();}
void gamestate::callPlayerTurnEnd()     {if(temporaryFE[7]) temporaryFE[7]->notify();}
void gamestate::callEnemyTurnStart()    {if(temporaryFE[8]) temporaryFE[8]->notify();}
void gamestate::callEnemyTurnEnd()      {if(temporaryFE[9]) temporaryFE[9]->notify();}

void gamestate::callPlayerAboutToLoseHealth()   {if(temporaryFE[10]) temporaryFE[10]->notify();}
void gamestate::callPlayerHealthGain()          {if(temporaryFE[11]) temporaryFE[11]->notify();}
void gamestate::callPlayerHealthLoss()          {if(temporaryFE[12]) temporaryFE[12]->notify();}
void gamestate::callPlayerTakingAttackDamage()  {if(temporaryFE[13]) temporaryFE[13]->notify();}
void gamestate::callEnemyHealthLoss()           {if(temporaryFE[14]) temporaryFE[14]->notify();}
void gamestate::callEnemyTakingAttackDamage()   {if(temporaryFE[15]) temporaryFE[15]->notify();}

void gamestate::callPlayerTakingAttackDamage(character& enemy)  {if(temporaryFE[13]) temporaryFE[13]->notify(enemy);}
void gamestate::callEnemyTakingAttackDamage(character& enemy)   {if(temporaryFE[15]) temporaryFE[15]->notify(enemy);}

void gamestate::callAlterBaseADFromPlayer()     {if(temporaryFE[16])temporaryFE[16]->notify();}
void gamestate::callAlterBaseADFromEnemy()      {if(temporaryFE[17])temporaryFE[17]->notify();}
void gamestate::callAlterAdjustedADFromPlayer() {if(temporaryFE[18])temporaryFE[18]->notify();}
void gamestate::callAlterAdjustedADFromEnemy()  {if(temporaryFE[19])temporaryFE[19]->notify();}

void gamestate::callGainBlock()                         {if(temporaryFE[20]) temporaryFE[20]->notify();}
void gamestate::callApplyingVulnerable(character& enemy){if(temporaryFE[21]) temporaryFE[21]->notify(enemy); callApplyingDebuff(enemy);}
void gamestate::callApplyingFrail()                     {if(temporaryFE[22]) temporaryFE[22]->notify();}
void gamestate::callApplyingWeak(character& enemy)      {if(temporaryFE[23]) temporaryFE[23]->notify(enemy); callApplyingDebuff(enemy);}
void gamestate::callApplyingDebuff(character& enemy)    {if(temporaryFE[24]) temporaryFE[24]->notify(enemy);}

void gamestate::callBlockBroken()           {if(temporaryFE[25]) temporaryFE[25]->notify();}
void gamestate::callSOTBlock()              {if(temporaryFE[26]) temporaryFE[26]->notify();}
void gamestate::callShuffledDraw()          {if(temporaryFE[27]) temporaryFE[27]->notify();}
void gamestate::callFightEnd()              {if(temporaryFE[28]) temporaryFE[28]->notify();}
void gamestate::callEnemyDied(character& c) {if(temporaryFE[29]) temporaryFE[29]->notify(c);}
void gamestate::callEnemyFatal(character& c) {if(temporaryFE[30]) temporaryFE[30]->notify(c);}

void gamestate::callNewFloorEntered()           {if(persistentFE[0]) persistentFE[0]->notify();}
void gamestate::callCampfireEntered()           {if(persistentFE[1]) persistentFE[1]->notify();}
void gamestate::callCampfireRest()              {if(persistentFE[2]) persistentFE[2]->notify();}
void gamestate::callShopEntered()               {if(persistentFE[3]) persistentFE[3]->notify();}
void gamestate::callQuesitonEntered()           {if(persistentFE[4]) persistentFE[4]->notify();}
void gamestate::callChestOpened()               {if(persistentFE[5]) persistentFE[5]->notify();}
void gamestate::callCardAdded(cardHolder& ch)   {if(persistentFE[6]) persistentFE[6]->notify(ch);}
void gamestate::callGoldSpent()                 {if(persistentFE[7]) persistentFE[7]->notify();}
void gamestate::callGoldGained()                {if(persistentFE[8]) persistentFE[8]->notify();}
void gamestate::callPotionUsed()                {if(persistentFE[9]) persistentFE[9]->notify();}
void gamestate::callPlayerDied()                {if(persistentFE[10]) persistentFE[10]->notify();}



gen::gen()
{
    start = new node(0, 0, 'N');
    for (int i{ 0 }; i < 15; ++i)
    {
        vector<node*> _floor;
        for (int j{ 0 }; j < 7; ++j)
        {
            _floor.push_back(new node(i + 1, j, 'x'));
        }
        nodeMap.push_back(_floor);
    }
    end = new node(16, 0, 'B');
}

bool gen::makeValid()
{
    static int t;

    for (int i{ 0 }; i < nodeMap.size(); ++i)
    {
        for (int j{ 0 }; j < nodeMap[i].size(); ++j)
        {
            if (nodeMap[i][j]->name == 'x') continue;
            if (nodeMap[i][j]->name == ' ' && !nodeMap[i][j]->nextNodes.empty() && !nodeMap[i][j]->prevNodes.empty()) return false;
            set<char> names_;
            for (auto k : nodeMap[i][j]->nextNodes)
            {
                if (nodeMap[i][j]->name == 's' && k->name == 's')
                {
                    if (random_int(0, 1)) nodeMap[i][j]->setFirstType(0);
                    else { k->setFirstType(0); }
                    return false;
                }
                if (nodeMap[i][j]->name == 'c' && k->name == 'c')
                {
                    if (random_int(0, 1)) nodeMap[i][j]->setFirstType(0);
                    else { k->setFirstType(0); }
                    return false;
                }
                if (nodeMap[i][j]->name == 'E' && k->name == 'E')
                {
                    if (random_int(0, 1)) nodeMap[i][j]->setFirstType(0);
                    else { k->setFirstType(0); }
                    return false;
                }
                if (names_.count(k->name) != 0)
                {
                    nodeMap[i][j]->nextNodes[random_int(0, nodeMap[i][j]->nextNodes.size() - 1)]->setFirstType(0);
                    return false;
                }
                if (i < 13 && k->name != 'C') names_.insert(k->name);
            }
        }
    }
    return true;
}
bool gen::checkValid()
{
    for (int i{ 0 }; i < nodeMap.size(); ++i)
    {
        for (int j{ 0 }; j < nodeMap[i].size(); ++j)
        {
            if (nodeMap[i][j]->name == 'x') continue;
            if (nodeMap[i][j]->name == ' ' && !nodeMap[i][j]->nextNodes.empty() && !nodeMap[i][j]->prevNodes.empty()) return false;
            set<char> names_;
            for (auto k : nodeMap[i][j]->nextNodes)
            {
                if (nodeMap[i][j]->name == 's' && k->name == 's')
                {
                    return false;
                }
                if (nodeMap[i][j]->name == 'E' && k->name == 'E')
                {
                    return false;
                }
                if (nodeMap[i][j]->name == 'c' && k->name == 'c')
                {
                    return false;
                }
                if (names_.count(k->name) != 0)
                {
                    return false;
                }
                if (i < 13 && k->name != 'C') names_.insert(k->name);
            }
        }
    }
    return true;
}

bool node::checkForC(char c, vector<node*> vn)
{
    for (int i{ 0 }; i < vn.size(); ++i)
    {
        if (vn[i]->name == c) return true;
    }
    return false;
}
bool node::checkForRepeat(char c, vector<node*> vn)
{
    int x{ 0 };
    for (int i{ 0 }; i < vn.size(); ++i)
    {
        if (vn[i]->name == c) ++x;
    }
    return x > 1;
}
// return 0 == good for all, -1== left bad, 1== right bad, 10== both bad 
int gen::checkGoodForCross(int fl, int wi)
{
    if (lastUsedWidthIndexOfFloor.find(fl) == lastUsedWidthIndexOfFloor.end())
    {
        lastUsedWidthIndexOfFloor[fl] = vector<pair<int, int>>({});
        return 0;
    }
    else
    {
        int res{ 0 };
        for (int i{ 0 }; i < lastUsedWidthIndexOfFloor[fl].size(); ++i)
        {
            if (lastUsedWidthIndexOfFloor[fl][i].first == wi - 1 && lastUsedWidthIndexOfFloor[fl][i].second == wi)
            {
                res = res == 0 || res == -1 ? -1 : 10;
                if (res == 10) return res;
            }
            else if (lastUsedWidthIndexOfFloor[fl][i].first == wi + 1 && lastUsedWidthIndexOfFloor[fl][i].second == wi)
            {
                res = res == 0 || res == 1 ? 1 : 10;
                if (res == 10) return res;
            }
        }
        return res;
    }
}
bool node::setFirstType(int depth)
{
    if (depth > 100)
    {
        return false;
    }
    if (prevNodes.size() == 0) { name = ' '; return true; }
    if (floorIndex == 1) { name = 'e'; return true; }
    if (floorIndex == 9) { name = 'C'; return true; }
    if (floorIndex == 15) { name = 'c'; return true; }
    int rng;
    while (true)
    {
        rng = random_int(1, 100);
        if (rng <= 43) name = 'e'; //45
        else if (rng <= 67) name = '?'; //67
        else if (rng <= 82) name = 'E'; //83
        else if (rng <= 95) name = 'c'; //95
        else { name = 's'; }
        if (floorIndex < 6 && (name == 'E' || name == 'c')) continue;
        if (name == 'E' && checkForC('E', prevNodes)) continue;
        if (name == 's' && (checkForC('s', prevNodes) || (floorIndex < 6 && nextNodes.size() == 3))) continue;
        if (name == 'c' && (checkForC('c', prevNodes) || floorIndex == 14)) continue;
        break;
    }
    if (floorIndex < 6 && checkForC('s', prevNodes))
    {
        bool repeats_{ false };
        for (auto i : prevNodes)
        {
            if (checkForRepeat('e', i->nextNodes) || checkForRepeat('?', i->nextNodes)) { repeats_ = true; }
        }
        if (repeats_)
        {
            char star_{ ' ' };
            node* holdThis{ nullptr };
            if (prevNodes.size() == 3)
            {
                for (int i{ 0 }; i < prevNodes.size(); ++i)
                {
                    if (prevNodes[i]->widthIndex == widthIndex) { holdThis = prevNodes[i]; prevNodes.erase(prevNodes.begin() + i); break; }
                }
            }
            if (prevNodes.size() == 2)
            {
                node* shopNode{ nullptr };
                for (auto i : prevNodes) // i=-1
                {
                    if (i->name == 's')
                    {
                        for (auto j : i->nextNodes) // j=0
                        {
                            if (j != this) star_ = j->name;
                        }
                        name = star_ == '?' ? 'e' : '?';
                        shopNode = i;
                    }
                }
                if (star_ == ' ')
                {
                    for (auto i : prevNodes) // i=-1
                    {
                        if (i->name != 's')
                        {
                            if (i->nextNodes.size() == 2)
                            {
                                name = i->name == 'e' ? '?' : 'e';
                            }
                            else if (i->nextNodes.size() == 3)
                            {
                                i->nextNodes[1]->name = 's';
                                if (checkForC('s', i->nextNodes[1]->nextNodes))
                                {
                                    for (auto j : i->nextNodes[1]->nextNodes)
                                    {
                                        if (j->name == 's')
                                        {
                                            if (!checkForC('?', i->nextNodes[1]->nextNodes)) j->name = '?';
                                            else if (!checkForC('e', i->nextNodes[1]->nextNodes)) j->name = 'e';
                                            else if (!checkForC('c', i->nextNodes[1]->nextNodes)) j->name = 'c';
                                            else { j->name = 'E'; }
                                        }
                                    }
                                }
                            }
                            else
                            {
                                if (!setFirstType(depth + 1))
                                {
                                    return false;
                                }
                            }
                            break;
                        }
                    }
                    if (holdThis != nullptr) prevNodes.insert(prevNodes.begin() + 1, holdThis);
                    return true;
                }
                for (auto i : prevNodes) // i=-1
                {
                    if (i != shopNode)
                    {
                        if (i->nextNodes.size() == 2)
                        {
                            for (auto j : i->nextNodes) // j=0
                            {
                                if (star_ == ' ')
                                {
                                    star_ = '?';
                                }
                                if (j != this)
                                {
                                    j->name = star_;
                                }
                            }
                        }
                        if (i->nextNodes.size() == 3)
                        {
                            bool done_{ false };
                            for (auto j : i->nextNodes) // j=0
                            {
                                if (j != this && done_)
                                {
                                    j->name = 's';
                                    for (auto k : j->nextNodes) // k=1
                                    {
                                        if (k->name == 's')
                                        {
                                            if (!k->setFirstType(depth + 1))
                                            {
                                                return false;
                                            }
                                        }
                                    }
                                    break;
                                }
                                if (j != this && !done_) { j->name = star_ == '?' ? '?' : 'e'; done_ = true; }
                            }
                        }
                    }
                }
            }
            if (holdThis != nullptr) prevNodes.insert(prevNodes.begin() + 1, holdThis);

        }
    }
    for (int i{ 0 }; i < prevNodes.size(); ++i)
    {
        if (checkForRepeat(name, prevNodes[i]->nextNodes))
        {
            if (!setFirstType(depth + 1))
            {
                return false;
            }
            i = 0;
        }
    }
    return true;
}
node* gen::giveNextRNGNode(node* current)
{
    if (current->floorIndex == 1) { current->addPrevNode(start); start->addNextNode(current); }
    if (current->floorIndex == 15)
    {
        current->name = 'c';
        current->addNextNode(end);
        end->addPrevNode(current);
        return end;
    }
    if (current->nextNodes.size() == 2 && current->floorIndex < 6)
    {
        for (auto i : current->nextNodes)
        {
            if (i->prevNodes.size() == 3)
            {
                return current->nextNodes[random_int(0, 1)];
            }
        }
    }
    int rng{ 0 };
    if (current->name == 's' && current->nextNodes.size() == 2 && current->floorIndex < 6)
    {
        return current->nextNodes[random_int(0, 1)];
    }
    else if ((current->floorIndex == 13 || current->floorIndex == 14) && current->nextNodes.size() == 2)
    {
        return current->nextNodes[random_int(0, 1)];
    }
    else if (current->nextNodes.size() == 2 && current->floorIndex < 6 && current->prevNodes.size() == 3 &&
        find_if(current->prevNodes.begin(), current->prevNodes.end(), [&](const node* n) {return n->nextNodes.size() == 3; }) != current->prevNodes.end())
    {
        return current->nextNodes[random_int(0, 1)];
    }

    else if (current->widthIndex == 0)
    {
        rng = random_int(0, 1);
        int temp = checkGoodForCross(current->floorIndex, current->widthIndex);
        rng = temp == 1 ? 0 : random_int(0, 1);
    }
    else if (current->widthIndex == 6)
    {
        int temp = checkGoodForCross(current->floorIndex, current->widthIndex);
        rng = temp == -1 ? 6 : random_int(5, 6);
    }
    else
    {
        int temp = checkGoodForCross(current->floorIndex, current->widthIndex);
        if (temp == 0) rng = random_int(current->widthIndex - 1, current->widthIndex + 1);
        else if (temp == -1) rng = random_int(current->widthIndex, current->widthIndex + 1);
        else if (temp == 1) rng = random_int(current->widthIndex - 1, current->widthIndex);
        else if (temp == 10) rng = current->widthIndex;
    }
    // would be +1 but index thing		
    node* nextNode = nodeMap[current->floorIndex][rng];
    if (!current->checkN(nextNode)) current->addNextNode(nextNode);

    if (node::checkForC('s', nextNode->prevNodes) && current->nextNodes.size() == 3)
    {
        for (int i{ 0 }; i < current->nextNodes.size(); ++i)
        {
            if (current->nextNodes[i] == nextNode)
            {
                current->nextNodes.erase(current->nextNodes.begin() + i);
                break;
            }
        }
        return giveNextRNGNode(current);
    }

    if (!nextNode->checkP(current)) nextNode->addPrevNode(current);
    pair<int, int> a(current->widthIndex, rng);
    lastUsedWidthIndexOfFloor[current->floorIndex].push_back(a);
    if (!current->setFirstType(0))
    {
        //current->name = current->prevNodes.back()->nextNodes.back()->name;
        //if(!current->prevNodes.back()->nextNodes.back()->setFirstType(0)) 
        //{
        while (!makeValid());
        //return nullptr;
    //}
    }
    return nextNode;
}
bool gen::fillMap()
{
    vector<int> starters = makeStartIndices();
    for (auto i : starters)
    {
        node* starter = nodeMap[0][i];
        while (starter != end)
        {
            starter = giveNextRNGNode(starter);
            if (starter == nullptr)
            {
                return false;
            }
        }
    }
    node* starter = nodeMap[0][starters[random_int(0, 1)]];
    while (starter != end)
    {
        starter = giveNextRNGNode(starter);
        if (starter == nullptr)
        {
            return false;
        }
    }
    starter = nodeMap[0][starters[random_int(2, 3)]];
    while (starter != end)
    {
        starter = giveNextRNGNode(starter);
        if (starter == nullptr)
        {
            return false;
        }
    }
    for (int i{ 0 }; i < nodeMap.size(); ++i)
    {
        for (int j{ 0 }; j < nodeMap[i].size(); ++j)
        {
            if (nodeMap[i][j]->name == ' ')
            {
                if (!nodeMap[i][j]->setFirstType(0)) return false;
            }
        }
    }
    return true;
}
vector<int> gen::makeStartIndices()
{
    vector<int> res{ 0,1,2,4,5,6 };
    while (res.size() != 4)
    {
        res.erase(res.begin() + random_int(0, res.size() - 1));
    }
    return res;
}
void gen::showMap(int f, int w, int act_, string bossname)
{
    int actDispl{ 0 };
    switch (act_)
    {
    case 1:
        actDispl = 0;
        break;
    case 2:
        actDispl = 17;
        break;
    case 3:
        actDispl = 34;
        break;
    }
    for (int x{ 0 }; x < 3; ++x) bossname.pop_back();
    // visual map formatting, don't try to figure out
    std::cout;
    std::cout << endl << "Boss: ----- " << bossname << " -----" << endl << "    ";
    for (int j{ 0 }; j < 7; ++j)
    {
        if (nodeMap[14][j]->nextNodes.size() == 0) { std::cout << "    "; continue; }
        char line_;
        if (j < 3)
        {
            line_ = '/';
            std::cout << "  " << line_ << " ";
        }
        else if (j > 3)
        {
            line_ = '\\';
            std::cout << "" << line_ << "  ";
        }
        else
        {
            line_ = '|';
            std::cout << " " << line_ << "  ";
        }
    }
    std::cout << endl;
    for (int i{ 14 }; i >= 0; --i)
    {
        i + actDispl > 8 ? std::cout << i + 1 + actDispl << ": " : std::cout << i + 1 + actDispl << ":  ";
        for (int j{ 0 }; j < 7; ++j)
        {
            if (nodeMap[i][j]->nextNodes.size() == 0) { nodeMap[i][j]->name = ' '; std::cout << "    "; continue; }
            if (nodeMap[i][j]->floorIndex == f && nodeMap[i][j]->widthIndex == w) std::cout << ">" << nodeMap[i][j]->name << "< ";
            else { std::cout << " " << nodeMap[i][j]->name << "  "; }
        }
        std::cout << endl;
        if (i == 0) break;
        int lastI{ 0 };
        std::cout << "    ";
        for (int j{ 0 }; j < 7; ++j)
        {
            vector<char> c = { ' ',' ',' ' };
            for (int k{ 0 }; k < nodeMap[i][j]->prevNodes.size(); ++k)
            {
                int t = nodeMap[i][j]->widthIndex;
                int index = nodeMap[i][j]->prevNodes[k]->widthIndex - t;
                switch (index)
                {
                case -1:
                    c[0] = '/';
                    break;
                case 0:
                    c[1] = '|';
                    break;
                case 1:
                    c[2] = '\\';
                    break;
                }
            }
            int temp = lastI;
            lastI = 0;
            if (temp == 2)
            {
                if (c[1] != ' ') { std::cout << " " << c[1]; lastI = 1; }
                else { std::cout << "  "; }
                if (c[2] != ' ') { std::cout << " " << c[2]; lastI = 2; }
                else { std::cout << " "; }
                if (c == vector<char>{' ', ' ', ' '}) lastI = 0;
            }
            else if (temp == 1)
            {
                if (c[0] != ' ') { std::cout << c[0]; lastI = 0; }
                else { std::cout << " "; }
                if (c[1] != ' ') { std::cout << " " << c[1]; lastI = 1; }
                else { std::cout << "  "; }
                if (c[2] != ' ') { std::cout << " " << c[2]; lastI = 2; }
                else { std::cout << " "; }
                if (c == vector<char>{' ', ' ', ' '}) lastI = 0;
            }
            else if (temp == 0)
            {
                if (j == 0)
                {
                    if (c[1] != ' ') { std::cout << " " << c[1]; lastI = 1; }
                    else { std::cout << "  "; }
                    if (c[2] != ' ') { std::cout << " " << c[2]; lastI = 2; }
                    else { std::cout << " "; }
                }
                else
                {
                    if (c[0] != ' ') { std::cout << c[0]; lastI = 0; }
                    else { std::cout << " "; }
                    if (c[1] != ' ') { std::cout << " " << c[1]; lastI = 1; }
                    else { std::cout << "  "; }
                    if (c[2] != ' ') { std::cout << " " << c[2]; lastI = 2; }
                    else { std::cout << " "; }
                }
            }

        }
        std::cout << endl;
    }
}
node* gen::getNode(int f, int w)
{
    if (f == 0) return start;
    if (f == 16) return end;

    return nodeMap[f - 1][w];
}

gen::~gen() 
{
    delete start; 
    delete end;
    for (int i{ 0 }; i < nodeMap.size(); ++i)
    {
        for (int j{ 0 }; j < nodeMap[0].size(); ++j) delete nodeMap[i][j];
    }
}

void gamestate::genRealMap()
{
    if (mapGen) delete mapGen;
    mapGen = new gen;
    mapGen->fillMap();
    while (!mapGen->makeValid());
    if (!mapGen->checkValid())
    {
        delete mapGen;
        mapGen = new gen;
    };
    //mapGen->showMap();
}
void gamestate::advanceRealEvent()
{
    if (random_int(1, 30) == 31) randomMacroThisFloor = true;
    else { randomMacroThisFloor = false; }
    if (firstMapItr)
    {
        firstMapItr = false;
        nextEvent = makeNextEvent('N');
        return;
    }
    ++mapPathIndex;
    int mapDispl = ((actNumber - 1) * 17);
    if(!ai) mapGen->showMap(currentFloorIndex - mapDispl, currentWidthIndex,actNumber, events_bo->getBossName(bossIndexThisAct));
    vector<string> inputNumbers;
    vector<node*> next_floors_;
    if(!ai) std::cout << "Choose the next room (ordered left to right as on map)" << endl;
    if (gamestate::wingBootsCheck > 0 && !ai)
    {
        for (int i{ 0 }; i < 7; ++i)
        {
            if (mapGen->getNode(currentFloorIndex - mapDispl, i)->name == ' ') continue;
            next_floors_.push_back(mapGen->getNode(currentFloorIndex - mapDispl, i));
        }
    }
    else
    {
        node* curr = mapGen->getNode(currentFloorIndex - mapDispl, currentWidthIndex);
        next_floors_ = curr->nextNodes;
    }
    sort(next_floors_.begin(), next_floors_.end(), [&](const node* a, const node* b) {return a->widthIndex < b->widthIndex; });
    for (int i{ 1 }; i <= next_floors_.size(); ++i)
    {
        inputNumbers.push_back(to_string(i));
        if(!ai) std::cout << i << ": for " << next_floors_[i - 1]->name << ". ";
    }
    if (aiEventActionBuffer.empty()) makePathChoiceFromNeutral();
    string s = request_input(inputNumbers);
    int i = stoi(s) - 1;
    if (nextEvent && (mapPath[mapPathIndex - 1] == 'N' || mapPath[mapPathIndex - 1] == 's' || mapPath[mapPathIndex - 1] == 'C' || mapPath[mapPathIndex - 1] == '?'))
    {
        delete nextEvent;
        nextEvent = nullptr;
    }

    if (gamestate::wingBootsCheck > 0 && !ai)
    {
        bool mWB{ true };
        for (auto ff : next_floors_[i]->prevNodes)
        {
            if (ff->widthIndex == currentWidthIndex) mWB = false;
        }
        if (mWB) --gamestate::wingBootsCheck;
    }

    mapPath[mapPathIndex] = next_floors_[i]->name;
    currentFloorIndex = next_floors_[i]->floorIndex + mapDispl;
    currentWidthIndex = next_floors_[i]->widthIndex;

    nextEvent = makeNextEvent(next_floors_[i]->name);
}

map<int, int> gen::discreteMapRep(node* startingPoint, const macroStateInfo& _msi, vector<macroStateInfo>& MSIs, bool _return)
{
    if (!_return && !gm.getCachedPaths().empty())
    {
        MSIs = gm.getCachedPaths();
        return {};
    }
    vector<vector<char>> allPaths{ {} };
    vector<vector<int>> directions{ {} };
    map<int, int> pathFirstChoice;

    recursivePathing(allPaths, directions, startingPoint, 0);

    MSIs = vector<macroStateInfo>(allPaths.size(), _msi);

    for (int i{ 0 }; i < allPaths.size(); ++i)
    {
        if(!allPaths.empty()) pathFirstChoice[i] = directions[i][0];
        MSIs[i].nextFloor =   { 0,0,0,0,0,0 };
        MSIs[i].secondFloor = { 0,0,0,0,0,0 };
        MSIs[i].thirdFloor =  { 0,0,0,0,0,0 };
        int greed{ 0 }, safe{ 0 }, tillCampfire{ 0 }, tillShop{ 0 };
        float expectedRelics{ 0 }, expectedPotions{ 0 }, expectedCards{ 0 }, expectedUpgrades{ 0 };
        bool campfireNextFive{ 0 }, shopEarly{ 0 }, shopLate{ 0 };
        int eCount{ 0 }, ECount{ 0 }, qCount{ 0 }, cCount{ 0 }, CCount{ 0 }, sCount{ 0 };
        for (int j{ 0 }; j < allPaths[i].size(); ++j)
        {
            switch (allPaths[i][j])
            {
            case 'e':
                if (j < 3) {
                    switch (j) {
                        case 0: MSIs[i].nextFloor = { 1,0,0,0,0,0 }; break;
                        case 1: MSIs[i].secondFloor = { 1,0,0,0,0,0 }; break;
                        case 2: MSIs[i].thirdFloor = { 1,0,0,0,0,0 }; break;
                    }
                }
                ++eCount;
                greed += 2;
                safe -= gm.getEasyDone() + eCount >= 2 ? 2 : 1;
                expectedCards += 1;
                expectedPotions += (gm.current_potion_pc / 100.0f);
                break;
            case 'E':
                if (j < 3) {
                    switch (j) {
                        case 0: MSIs[i].nextFloor = { 0,1,0,0,0,0 }; break;
                        case 1: MSIs[i].secondFloor = { 0,1,0,0,0,0 }; break;
                        case 2: MSIs[i].thirdFloor = { 0,1,0,0,0,0 }; break;
                    }
                }
                ++ECount;
                greed += 4;
                safe -= 3;
                expectedCards += 1;
                expectedRelics += 1;
                expectedPotions += (gm.current_potion_pc / 100.0f);
                break;
            case '?':
                if (j < 3) {
                    switch (j) {
                        case 0: MSIs[i].nextFloor = { 0,0,1,0,0,0 }; break;
                        case 1: MSIs[i].secondFloor = { 0,0,1,0,0,0 }; break;
                        case 2: MSIs[i].thirdFloor = { 0,0,1,0,0,0 }; break;
                    }
                }
                ++qCount;
                greed += 1;
                safe += 1;
                if (gm.getEasyDone() + eCount >= 2) --safe;
                expectedRelics += 0.2f;
                expectedCards += 0.5f;
                expectedUpgrades += 0.35f;
                expectedPotions += (gm.current_potion_pc / 200.0f);
                break;
            case 'C':
                if (j < 3) {
                    switch (j) {
                        case 0: MSIs[i].nextFloor = { 0,0,0,1,0,0 }; break;
                        case 1: MSIs[i].secondFloor = { 0,0,0,1,0,0 }; break;
                        case 2: MSIs[i].thirdFloor = { 0,0,0,1,0,0 }; break;
                    }
                }
                ++CCount;
                break;
            case 'c':
                if (j < 3) {
                    switch (j) {
                        case 0: MSIs[i].nextFloor = { 0,0,0,0,1,0 }; break;
                        case 1: MSIs[i].secondFloor = { 0,0,0,0,1,0 }; break;
                        case 2: MSIs[i].thirdFloor = { 0,0,0,0,1,0 }; break;
                    }
                }
                ++cCount;
                greed -= 1;
                safe += 3;
                expectedUpgrades += 0.65f;
                if (tillCampfire == 0) {
                    tillCampfire = i;
                }
                break;
            case 's':
                if (j < 3) {
                    switch (j) {
                        case 0: MSIs[i].nextFloor = { 0,0,0,0,0,1 }; break;
                        case 1: MSIs[i].secondFloor = { 0,0,0,0,0,1 }; break;
                        case 2: MSIs[i].thirdFloor = { 0,0,0,0,0,1 }; break;
                    }
                }
                ++sCount;
                greed -= 2;
                safe += 2;
                if (tillShop == 0) {
                    tillShop = i;
                    greed += 3;
                    expectedRelics += 0.5f;
                    expectedCards += 0.5f;
                    expectedPotions += 0.5f;
                }
                break;
            }
        }
        MSIs[i].mapRepVars = { greed,safe,tillCampfire,tillShop,static_cast<int>(expectedRelics), static_cast<int>(expectedPotions),
            static_cast<int>(expectedCards), static_cast<int>(expectedUpgrades),campfireNextFive, shopEarly, shopLate,
            eCount, ECount, qCount, cCount, CCount, sCount };

    }
    for (int i{ 0 }; i < allPaths.size(); ++i)
    {
        if (allPaths[i].size() > 14)
        {
            MSIs[i].mapRepVars.insert(MSIs[i].mapRepVars.end(), { 10,10,10,10,10,10 });
            continue;
        }
        vector<bool> validBranchPaths(allPaths.size(),1);
        validBranchPaths[i] = false;
        int earlySafe{ 0 }, earlyGreedy{ 0 }, midSafe{ 0 }, midGreedy{ 0 }, lateSafe{ 0 }, lateGreedy{ 0 };
        
        for (int j{ 0 }; j < allPaths[i].size(); ++j)
        {
            for (int k{ 0 }; k < allPaths.size(); ++k)
            {
                if (!validBranchPaths[k]) continue;
                if (directions[i][j] != directions[k][j])
                {
                    validBranchPaths[k] = false;
                    if (j < 3)
                    {
                        if (MSIs[k].mapRepVars[1] > MSIs[i].mapRepVars[1]) ++earlySafe;
                        if (MSIs[k].mapRepVars[0] > MSIs[i].mapRepVars[0]) ++earlyGreedy;
                    }
                    else if (j < 7)
                    {
                        if (MSIs[k].mapRepVars[1] > MSIs[i].mapRepVars[1]) ++midSafe;
                        if (MSIs[k].mapRepVars[0] > MSIs[i].mapRepVars[0]) ++midGreedy;
                    }
                    else
                    {
                        if (MSIs[k].mapRepVars[1] > MSIs[i].mapRepVars[1]) ++lateSafe;
                        if (MSIs[k].mapRepVars[0] > MSIs[i].mapRepVars[0]) ++lateGreedy;
                    }
                }
            }
            
        }
        MSIs[i].mapRepVars.insert(MSIs[i].mapRepVars.end(), {clamp(earlySafe,0,10), clamp(earlyGreedy,0,10), clamp(midSafe,0,10), clamp(midGreedy,0,10), clamp(lateSafe,0,10), clamp(lateGreedy,0,10) });
    }
    gm.setCachedPaths(MSIs);
    return pathFirstChoice;
}

void gen::recursivePathing(vector<vector<char>>& paths, vector<vector<int>>& directions, node* current, int recurseLevel)
{
    if (current->nextNodes[0] == end)
    {
        if (recurseLevel == 0)
        {
            paths = { {'B'} };
            directions = { {0} };
        }
        return;
    }
    for (int i{ 0 }; i < current->nextNodes.size(); ++i)
    {
        if (i != 0)
        {
            paths.push_back(paths.back());
            directions.push_back(directions.back());
            while (paths.back().size() > recurseLevel)
            {
                paths.back().pop_back();
                directions.back().pop_back();
            }
        }
        paths.back().push_back(current->nextNodes[i]->name);
        directions.back().push_back(i);
        recursivePathing(paths, directions, current->nextNodes[i], recurseLevel + 1);
    }
}

// static initializing
deque<int> gamestate::aiSimActionBuffer{};
deque<string> gamestate::aiSimPotionBuffer{};
deque<string> gamestate::aiEventActionBuffer{};
vector<C_DATABASE::iterator> gamestate::aiCardBuffer{};
vector<C_DATABASE::iterator> gamestate::aiCardChoices{};
vector<vector<int>> gamestate::orderedChoiceTree{};

int gamestate::cardRewardNumber{ 3 };
bool gamestate::ectoplasmCheck{ false };
bool gamestate::coffeeDripperCheck{ false };
bool gamestate::fusionHammerCheck{ false };
bool gamestate::sneckoEyeCheck{ false };
bool gamestate::markOfPainCheck{ false };
bool gamestate::runicPyramidCheck{ false };
bool gamestate::sozuCheck{ false };
bool gamestate::singingBowlCheck{ false };
bool gamestate::peacePipeCheck{ false };
bool gamestate::giryaCheck{ false };
bool gamestate::shovelCheck{ false };
bool gamestate::iceCreamCheck{ false };
bool gamestate::prayerWheelCheck{ false };
bool gamestate::frozenEyeCheck{ false };
bool gamestate::goldenIdolCheck{ false };
bool gamestate::nlothsGiftCheck{ false };
bool gamestate::strikeDummyCheck{ false };
bool gamestate::juzuBraceletCheck{ false };
bool gamestate::tinyChestCheck{ false };
bool gamestate::tungstenCheck{ false };
bool gamestate::chemicalXCheck{ false };
bool gamestate::strangeSpoonCheck{ false };
bool gamestate::strangeSpoonLE{ false };
bool gamestate::nextCardSameTarget{ false };
bool gamestate::denyCardPlay{ false };
bool gamestate::clockEnd{ false };
bool gamestate::dontDupe{ false };
bool gamestate::randomAIChoice{ false };
int gamestate::nextCardsNoTarget{ 0 };
int gamestate::lastAttackedSCRNIndex{ 0 };
int gamestate::wingBootsCheck{ 0 };
int gamestate::omamoriCheck{ 0 };
bool gamestate::markOfTheBloomCheck{ false };
bool gamestate::sacredBarkCheck{ false };
bool gamestate::runicDomeCheck{ false };
