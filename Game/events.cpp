#include "events.h"
#include "gamestate.h"
#include "pc_info.h"
#include "character.h"
#include "card.h"
#include "relic.h"
#include "potions.h"
#include <cassert>

using namespace std;

void easyHallway::startEvent()
{
    if(ai) gm.startRandomFight(this);
    else { gm.startRandomFightV(this); }
}
void hardHallway::startEvent()
{
    if (ai) gm.startRandomFight(this);
    else { gm.startRandomFightV(this); }
}
void elites::startEvent()
{
    if (ai) gm.startRandomFight(this);
    else { gm.startRandomFightV(this); }
}
void qSpecial::startEvent()
{
    if (ai) gm.startRandomFight(this);
    else { throw runtime_error("won't happen"); }
}
void bosses::startEvent()
{
    if (ai) gm.startRandomFight(this);
    else { gm.startRandomBossFightV(); }
}
int bosses::selectRandom()
{
    if (bossOptions.empty()) throw runtime_error("no hallway options");
    return random_int(0, static_cast<int>(bossOptions.size()) - 1);
}

void easyHallway::placeRandomForNext(int act_)
{
    if (hallwayOptions.size() > 10) hallwayOptions.clear();
    int i = getRandomIndex(pFight);
    if (lastFight != -1)
    {
        while (i == lastFight)
        {
            i = getRandomIndex(pFight);
        }
    }
    lastFight = i;
    // act 1
    if (act_ == 1)
    {
        gm.fightIndexPRED = i;
        switch (i)
        {
        case 0:
        {
            hallwayOptions.push_back({ "JawWorm_ID" });
            break;
        }
        case 1:
        {
            hallwayOptions.push_back({ "Cultist_ID" });
            break;
        }
        case 2:
        {
            vector<string> rlgl;
            if (random_int(0, 1)) rlgl.push_back("RedLouse_ID");
            else { rlgl.push_back("GreenLouse_ID"); }
            if (random_int(0, 1)) rlgl.push_back("RedLouse_ID");
            else { rlgl.push_back("GreenLouse_ID"); }
            hallwayOptions.push_back(rlgl);
            break;
        }
        case 3:
        {
            vector<string> msss;
            if (random_int(0, 1)) { msss.push_back("AcidSlimeS_ID"); msss.push_back("SpikeSlimeM_ID"); }
            else { msss.push_back("SpikeSlimeS_ID"); msss.push_back("AcidSlimeM_ID"); }
            hallwayOptions.push_back(msss);
            break;
        }
        }
    }
    // act 2
    else if(act_ == 2)
    {
        gm.fightIndexPRED = 21 + i;
        switch (i)
        {
        case 0:
        {
            hallwayOptions.push_back({ "SphericGuardian_ID" });
            break;
        }
        case 1:
        {
            hallwayOptions.push_back({ "Chosen_ID" });
            break;
        }
        case 2:
        {
            hallwayOptions.push_back({ "ShelledParasite_ID" });
            break;
        }
        case 3:
        {
            hallwayOptions.push_back({ "Byrd_ID","Byrd_ID","Byrd_ID" });
            break;
        }
        case 4:
        {
            hallwayOptions.push_back({ "Looter_ID","Mugger_ID" });
            break;
        }
        }
    }
    else
    {
        gm.fightIndexPRED = 43 + i;
        switch (i)
        {
        case 0:
        {
            hallwayOptions.push_back({ "Darkling_ID","Darkling_ID","Darkling_ID" });
            break;
        }
        case 1:
        {
            hallwayOptions.push_back({ "OrbWalker_ID" });
            break;
        }
        case 2:
        {
            vector<string> tempOpts{ "Spiker_ID" , "Repulsor_ID" , "Exploder_ID" ,"Spiker_ID" , "Repulsor_ID" , "Exploder_ID" };
            while (tempOpts.size() > 3)
            {
                tempOpts.erase(tempOpts.begin() + random_int(0, static_cast<int>(tempOpts.size()) - 1));
            }
            hallwayOptions.push_back(tempOpts);
            break;
        }
        }
    }
    
}
void hardHallway::placeRandomForNext(int act_)
{
    if (hallwayOptions.size() > 10) hallwayOptions.clear();
    int i = getRandomIndex(pFight);
    if (lastFight != -1)
    {
        while (i == lastFight)
        {
            i = getRandomIndex(pFight);
        }
    }
    lastFight = i;
    // act 1
    if(act_ == 1)
    {
        gm.fightIndexPRED = 4 + i;
        switch (i)
        {
        case 0:
        {
            vector<string> tempOpts{ "MadGremlin_ID" , "MadGremlin_ID" , "SneakyGremlin_ID" , "SneakyGremlin_ID" ,"FatGremlin_ID" , "FatGremlin_ID" , "ShieldGremlin_ID", "GremlinWizard_ID" };
            vector<string> gg;
            set<int> tempOptIndices;
            while (tempOptIndices.size() < 4)
            {
                tempOptIndices.insert(random_int(0, static_cast<int>(tempOpts.size()) - 1));
            }
            for (auto itr{ tempOptIndices.begin() }; itr != tempOptIndices.end(); ++itr)
            {
                gg.push_back(tempOpts[*itr]);
            }
            hallwayOptions.push_back(gg);
            break;
        }
        case 1:
        {
            vector<string> ls;
            random_int(0, 1) == 1 ? ls = { "AcidSlimeL_ID" } : ls = { "SpikeSlimeL_ID" };
            hallwayOptions.push_back(ls);
            break;
        }
        case 2:
        {
            hallwayOptions.push_back({ "SpikeSlimeS_ID","AcidSlimeS_ID","SpikeSlimeS_ID","AcidSlimeS_ID","SpikeSlimeS_ID" });
            break;
        }
        case 3:
        {
            hallwayOptions.push_back({ "BlueSlaver_ID" });
            break;
        }
        case 4:
        {
            hallwayOptions.push_back({ "RedSlaver_ID" });
            break;
        }
        case 5:
        {
            vector<string> lll;
            if (random_int(0, 1)) { lll.push_back("RedLouse_ID"); }
            else { lll.push_back("GreenLouse_ID"); }
            if (random_int(0, 1)) { lll.push_back("RedLouse_ID"); }
            else { lll.push_back("GreenLouse_ID"); }
            if (random_int(0, 1)) { lll.push_back("RedLouse_ID"); }
            else { lll.push_back("GreenLouse_ID"); }
            hallwayOptions.push_back(lll);
            break;
        }
        case 6:
        {
            hallwayOptions.push_back({ "FungiBeast_ID","FungiBeast_ID" });
            break;
        }
        case 7:
        {
            vector<string> exo;
            int rnd = random_int(1, 4);
            switch (rnd)
            {
            case 1:
                exo.push_back("SpikeSlimeM_ID");
                break;
            case 2:
                exo.push_back("AcidSlimeM_ID");
                break;
            case 3:
                exo.push_back("GreenLouse_ID");
                break;
            case 4:
                exo.push_back("RedLouse_ID");
                break;
            }
            rnd = random_int(1, 4);
            switch (rnd)
            {
            case 1:
                exo.push_back("RedSlaver_ID");
                break;
            case 2:
                exo.push_back("BlueSlaver_ID");
                break;
            case 3:
                exo.push_back("Looter_ID");
                break;
            case 4:
                exo.push_back("Cultist_ID");
                break;
            }
            hallwayOptions.push_back(exo);
            break;
        }
        case 8:
        {
            vector<string> wil;
            int rnd = random_int(1, 2);
            switch (rnd)
            {
            case 1:
                wil.push_back("JawWorm_ID");
                break;
            case 2:
                wil.push_back("FungiBeast_ID");
                break;
            }
            rnd = random_int(1, 4);
            switch (rnd)
            {
            case 1:
                wil.push_back("RedLouse_ID");
                break;
            case 2:
                wil.push_back("GreenLouse_ID");
                break;
            case 3:
                wil.push_back("AcidSlimeM_ID");
                break;
            case 4:
                wil.push_back("SpikeSlimeM_ID");
                break;
            }
            hallwayOptions.push_back(wil);
            break;
        }
        case 9:
        {
            hallwayOptions.push_back({ "Looter_ID" });
            break;
        }
        }
    }
    // act 2
    else if (act_ == 2)
    {
        gm.fightIndexPRED = 26 + i;
        switch (i)
        {
        case 0:
        {
            hallwayOptions.push_back({ "Byrd_ID","Chosen_ID" });
            break;
        }
        case 1:
        {
            hallwayOptions.push_back({ "Cultist_ID","Chosen_ID" });
            break;
        }
        case 2:
        {
            hallwayOptions.push_back({ "Sentry1_ID","SphericGuardian_ID" });
            break;
        }
        case 3:
        {
            hallwayOptions.push_back({ "SnakePlant_ID" });
            break;
        }
        case 4:
        {
            hallwayOptions.push_back({ "Snecko_ID" });
            break;
        }
        case 5:
        {
            hallwayOptions.push_back({ "Centurion_ID","Mystic_ID" });
            break;
        }
        case 6:
        {
            hallwayOptions.push_back({ "Cultist_ID","Cultist_ID","Cultist_ID" });
            break;
        }
        case 7:
        {
            hallwayOptions.push_back({ "ShelledParasite_ID","FungiBeast_ID" });
            break;
        }
        }
    }
    else
    {
        gm.fightIndexPRED = 46 + i;
        switch (i)
        {
        case 0:
        {
            vector<string> tempOpts{ "Spiker_ID" , "Repulsor_ID" , "Exploder_ID" ,"Spiker_ID" , "Repulsor_ID" , "Exploder_ID" };
            while (tempOpts.size() > 4)
            {
                tempOpts.erase(tempOpts.begin() + random_int(0, static_cast<int>(tempOpts.size()) - 1));
            }
            hallwayOptions.push_back(tempOpts);
            break;
        }
        case 1:
        {
            hallwayOptions.push_back({ "TheMaw_ID" });
            break;
        }
        case 2:
        {
            vector<string> tempOpts{ "Spiker_ID" , "Repulsor_ID" , "Exploder_ID", "SphericGuardian_ID" };
            tempOpts.erase(tempOpts.begin() + random_int(0, 2));
            hallwayOptions.push_back(tempOpts);
            break;
        }
        case 3:
        {
            hallwayOptions.push_back({ "Darkling_ID","Darkling_ID","Darkling_ID" });
            break;
        }
        case 4:
        {
            hallwayOptions.push_back({ "WrithingMass_ID" });
            break;
        }
        case 5:
        {
            hallwayOptions.push_back({ "JawWorm_ID","JawWorm_ID","JawWorm_ID" });
            break;
        }
        case 6:
        {
            hallwayOptions.push_back({ "SpireGrowth_ID" });
            break;
        }
        case 7:
        {
            hallwayOptions.push_back({ "Transient_ID" });
            break;
        }
        }
    }
}
void elites::placeRandomForNext(int act_)
{
    int i;
    if (lastElite == -1) { i = random_int(0, 2); }
    else { i = random_intNot(0, 2, lastElite); }
    if (eliteOptions.size() > 10) eliteOptions.clear();
    lastElite = i;
    // act 1
    if (act_ == 1)
    {
        gm.fightIndexPRED = 14 + i;
        switch (i)
        {
        case 0:
        {
            eliteOptions.push_back({ "GremlinNob_ID" });
            break;
        }
        case 1:
        {
            eliteOptions.push_back({ "Lagavulin_ID" });
            break;
        }
        case 2:
        {
            eliteOptions.push_back({ "Sentry1_ID","Sentry2_ID","Sentry1_ID" });
            break;
        }
        }
    }
    // act 2
    else if (act_ == 2)
    {
        gm.fightIndexPRED = 34 + i;
        switch (i)
        {
        case 0:
        {
            eliteOptions.push_back({ "BookOfStabbing_ID" });
            break;
        }
        case 1:
        {
            eliteOptions.push_back({ "BlueSlaver_ID","Taskmaster_ID","RedSlaver_ID" });
            break;
        }
        case 2:
        {
            string gremlin1, gremlin2;
            switch (random_int(0, 4))
            {
            case 0:
                gremlin1 = "MadGremlin_ID";
                break;
            case 1:
                gremlin1 = "SneakyGremlin_ID";
                break;
            case 2:
                gremlin1 = "FatGremlin_ID";
                break;
            case 3:
                gremlin1 = "ShieldGremlin_ID";
                break;
            case 4:
                gremlin1 = "GremlinWizard_ID";
                break;
            }
            switch (random_int(0, 4))
            {
            case 0:
                gremlin2 = "MadGremlin_ID";
                break;
            case 1:
                gremlin2 = "SneakyGremlin_ID";
                break;
            case 2:
                gremlin2 = "FatGremlin_ID";
                break;
            case 3:
                gremlin2 = "ShieldGremlin_ID";
                break;
            case 4:
                gremlin2 = "GremlinWizard_ID";
                break;
            }
            eliteOptions.push_back({ gremlin1,gremlin2,"GremlinLeader_ID"});
            break;
        }
        }
    }
    else if (act_ == 3)
    {
        gm.fightIndexPRED = 54 + i;
        switch (i)
        {
        case 0:
        {
            eliteOptions.push_back({ "GiantHead_ID" });
            break;
        }
        case 1:
        {
            eliteOptions.push_back({ "Nemesis_ID" });
            break;
        }
        case 2:
        {
            eliteOptions.push_back({ "Dagger_ID","Reptomancer_ID","Dagger_ID" });
        }
        }
    }
    else
    {
        gm.fightIndexPRED = 61;
        eliteOptions.push_back({ "SpireShield_ID","SpireSword_ID" });
    }
}
void qSpecial::placeRandomForNext(int act_)
{
    int i;
    i = random_int(0, 2);
    if (specialOptions.size() > 10) specialOptions.clear();
    // act 1
    if (act_ == 1)
    {
        gm.fightIndexPRED = 20;
        specialOptions.push_back({ "FungiBeast_ID","FungiBeast_ID","FungiBeast_ID" });
    }
    // act 2
    else if (act_ == 2)
    {
        gm.fightIndexPRED = 40 + i;
        switch (i)
        {
        case 0:
        {
            specialOptions.push_back({ "BlueSlaver_ID","RedSlaver_ID"});
            break;
        }
        case 1:
        {
            specialOptions.push_back({ "Pointy_ID","Romeo_ID","Bear_ID" });
            break;
        }
        case 2:
        {
            specialOptions.push_back({ "Taskmaster_ID","GremlinNob_ID" });
            break;
        }
        }
    }
    else if (act_ == 3)
    {
        gm.fightIndexPRED = 60;
        specialOptions.push_back({ "OrbWalker_ID","OrbWalker_ID"});
    }
}
void bosses::placeRandomForNext(int act_)
{
    switch (act_)
    {
    case 1:
        gm.fightIndexPRED = 17 + gm.getBossIndex();
        break;
    case 2:
        gm.fightIndexPRED = 37 + gm.getBossIndex();
        break;
    case 3:
        gm.fightIndexPRED = 57 + gm.getBossIndex();
        break;
    case 4:
        gm.fightIndexPRED = 62;
        break;
    }
}
int events::getRandomIndex(vector<float> vI)
{
    float rng = random_float(1, 100);
    float total{ 0.0 };
    for (int i{ 0 }; i < vI.size(); ++i)
    {
        total += vI[i];
        if (rng <= total) return i;
    }
    throw runtime_error("rng from vec");
}

void peaceEvents::prelude()
{
    apci.printDeck();
    std::cout << "POTIONS:" << endl;
    for (int i{ 0 }; i < apci.getPotionSlots(); ++i)
    {
        if (i >= apci.getPotions().size()) { std::cout << "Empty. "; }
        else { std::cout << apci.getPotions()[i]->getName() << ". "; }
    }
    std::cout << endl << "RELICS:" << endl;
    for (int i{ 0 }; i < apci.getRelics().size(); ++i)
    {
        std::cout << apci.getRelics()[i]->getName() << ". ";
    }
    std::cout << endl << endl << "Gold: " << apci.getGold() << ". Health: " << apci.getPlayer().getHealth() << "/" << apci.getPlayer().getMaxHealth() << "." << endl << endl;
    //gm.showPath();
}
int questionMark::storedValue1{ 0 };
int questionMark::storedValue2{ 0 };
int questionMark::storedValue3{ 0 };
int questionMark::storedValue4{ 0 };
void questionMark::startEvent()
{
    if(!ai) prelude();
    gm.callQuesitonEntered();
    std::cout << intro << endl;
    while (true)
    {
        int n{ 1 };
        vector<string> inputs_str;
        for (int j{ 0 }; j < questionOptions.size(); ++j)
        {
            if (questionOptions[j].requirementsCheck())
            {
                std::cout << to_string(n) << ": " << questionOptions[j].textFunction() << "." << endl;
                inputs_str.push_back(to_string(n++));
            }
            else
            {
                std::cout << "Unavailable: " << endl;// questionOptions[j].textFunction() << "." << endl;
                ++n;
            }
        }
        apci.addPotionInputs(inputs_str);
        if (aiDecisionMaker != nullptr)
        {
            aiDecisionMaker();
            /*for (int i{ 0 }; i < gamestate::aiEventActionBuffer.size(); ++i)
            {
                std::cout << gamestate::aiEventActionBuffer[i] << ",  ";
            }
            std::cout << endl;*/
        }
        std::string result{ request_input(inputs_str) };
        if (result[0] == 'p') { apci.potionMenu(static_cast<int>(result[1] - 49)); continue; }
        questionOptions[stoi(result) - 1].eventOutcome();
        break;
    }
}
campfire::campfire() : intro("Campfire")
{
    campfireOptions = {
        campfireChoices([]() {string res = "Rest : Heal " + to_string(apci.getPlayer().getMaxHealth() / 10 * 3) + " HP"; return res; },
                                []() {apci.getPlayer().healPercentage(30); gm.callCampfireRest(); }, []() {return !gamestate::coffeeDripperCheck; }),
        campfireChoices([]() {string res = "Smith : Upgrade a card from your deck"; return res; },
                                []() {gm.upgradeCardFromDeck(true); }, []() {return !gamestate::fusionHammerCheck; })
    };
}
void campfire::startEvent()
{
    if (!ai) prelude();
    gm.callCampfireEntered();
    std::cout << intro << endl;
    if (ai) 
    {
        bool girya_{ false };
        if (gamestate::giryaCheck)
        {
            for (int i{ 2 }; i < campfireOptions.size(); ++i)
            {
                if (campfireOptions[i].textFunction() == "Lift: Gain 1 extra Strength at start of combat" && campfireOptions[i].requirementsCheck())
                {
                    girya_ = true;
                }
            }
        }
        gm.makeCampfireChoices(!gamestate::coffeeDripperCheck, !gamestate::fusionHammerCheck,girya_, gamestate::peacePipeCheck, gamestate::shovelCheck);
        return; 
    }
    while (true)
    {
        int n{ 1 };
        vector<string> inputs_str;
        for (int j{ 0 }; j < campfireOptions.size(); ++j)
        {
            if (campfireOptions[j].requirementsCheck())
            {
                std::cout << to_string(n) << ": " << campfireOptions[j].textFunction() << "." << endl;
                inputs_str.push_back(to_string(n++));
            }
            else
            {
                std::cout << "Unavailable: " << campfireOptions[j].textFunction() << "." << endl;
                ++n;
            }
        }
        apci.addPotionInputs(inputs_str);
        std::string result{ request_input(inputs_str) };
        if (result[0] == 'p') { apci.potionMenu(static_cast<int>(result[1] - 49)); continue; }
        campfireOptions[stoi(result) - 1].eventOutcome();
        break;
    }
}
void campfire::aiCampfireDecision(vector<bool> best_)
{
    if (best_[0])
    {
        campfireOptions[0].eventOutcome();
    }
    else if (best_[1])
    {
        gm.makeCardChoices(aiEventCardChoice::Upgrade);
        campfireOptions[1].eventOutcome();
    }
    else if (best_[2])
    {
        for (int i{ 2 }; i < campfireOptions.size(); ++i)
        {
            if (campfireOptions[i].textFunction() == "Lift: Gain 1 extra Strength at start of combat" && campfireOptions[i].requirementsCheck())
            {
                campfireOptions[i].eventOutcome();
                return;
            }
        }
    }
    else if (best_[3])
    {
        for (int i{ 2 }; i < campfireOptions.size(); ++i)
        {
            if (campfireOptions[i].textFunction() == "Smoke: Remove a card from our deck")
            {
                gm.makeCardChoices(aiEventCardChoice::Remove);
                campfireOptions[1].eventOutcome();
                return;
            }
        }
    }
    else if (best_[4])
    {
        for (int i{ 2 }; i < campfireOptions.size(); ++i)
        {
            if (campfireOptions[i].textFunction() == "Dig: Uncover a random relic")
            {
                campfireOptions[i].eventOutcome();
                return;
            }
        }
    }
    //// choose rest if
    //if (apci.getPlayer().getPercentageHealth() < 0.4 && !gamestate::coffeeDripperCheck)
    //{
    //    campfireOptions[0].eventOutcome();
    //    return;
    //}
    //if (gamestate::giryaCheck)
    //{
    //    for (int i{ 2 }; i < campfireOptions.size(); ++i)
    //    {
    //        if (campfireOptions[i].textFunction() == "Lift: Gain 1 extra Strength at start of combat" && campfireOptions[i].requirementsCheck())
    //        {
    //            campfireOptions[i].eventOutcome();
    //            return;
    //        }
    //    }
    //}
    //// check for good upgrades
    //if(!gamestate::fusionHammerCheck)
    //{
    //    gm.makeCardChoices(aiEventCardChoice::Upgrade);
    //    campfireOptions[1].eventOutcome();
    //    return;
    //}
    //if (apci.getPlayer().getPercentageHealth() < 0.5 && !gamestate::coffeeDripperCheck)
    //{
    //    campfireOptions[0].eventOutcome();
    //    return;
    //}
    //if (gamestate::shovelCheck)
    //{
    //    for (int i{ 2 }; i < campfireOptions.size(); ++i)
    //    {
    //        if (campfireOptions[i].textFunction() == "Dig: Uncover a random relic")
    //        {
    //            campfireOptions[i].eventOutcome();
    //            return;
    //        }
    //    }
    //}
    //if (gamestate::peacePipeCheck)
    //{
    //    for (int i{ 2 }; i < campfireOptions.size(); ++i)
    //    {
    //        if (campfireOptions[i].textFunction() == "Smoke: Remove a card from our deck")
    //        {
    //            gm.makeCardChoices(aiEventCardChoice::Remove);
    //            campfireOptions[1].eventOutcome();
    //            return;
    //        }
    //    }
    //    
    //}
    //// then upgrade unless no available upgrades inwhich case rest unless cant rest then leave
    //

}
void shop::reCalcMC()
{
    for (int j{ 0 }; j < shopOptions.size(); ++j)
    {
        if (shopOptions[j].name_ == "Card Remove" && smilingmaskDiscount) continue;
        shopOptions[j].goldCost /= 2;
    }
}
int shop::calcGold(cardRarity cr)
{
    if (fiftyDiscountCount == 0) fiftyDiscountIndex = random_int(0, 4);
    bool discounted_{false};
    if (fiftyDiscountIndex == fiftyDiscountCount++) discounted_ = true;
    int multi_{ 10 };
    if (membershipDiscount && courierDiscount) multi_ = 4;
    else if (membershipDiscount) multi_ = 5;
    else if (courierDiscount) multi_ = 8;
    switch (cr)
    {
    case cardRarity::Common:
        return static_cast<int>(random_int(45, 55) * (discounted_ ? 0.5f : 1) * multi_ / 10);
    case cardRarity::Uncommon:
        return static_cast<int>(random_int(68, 82) * (discounted_ ? 0.5f : 1) * multi_ / 10);
    case cardRarity::Rare:
        return static_cast<int>(random_int(135, 165) * (discounted_ ? 0.5f : 1) * multi_ / 10);
    }
    throw runtime_error("no gold calc" + cr);
}
int shop::calcGoldColourless(cardRarity cr)
{
    int multi_{ 10 };
    if (membershipDiscount && courierDiscount) multi_ = 4;
    else if (membershipDiscount) multi_ = 5;
    else if (courierDiscount) multi_ = 8;
    switch (cr)
    {
    case cardRarity::Uncommon:
        return random_int(81, 99) * multi_ / 10;
    case cardRarity::Rare:
        return random_int(162, 198) * multi_ / 10;
    }
    throw runtime_error("no gold calc" + cr);
}
int shop::calcGold(cardRarity cr, bool potion)
{
    int multi_{ 10 };
    if (membershipDiscount && courierDiscount) multi_ = 4;
    else if (membershipDiscount) multi_ = 5;
    else if (courierDiscount) multi_ = 8;
    switch (cr)
    {
    case cardRarity::Common:
        return random_int(48, 52) * multi_ / 10;
    case cardRarity::Uncommon:
        return random_int(72, 78) * multi_ / 10;
    case cardRarity::Rare:
        return random_int(95, 105) * multi_ / 10;
    }
    throw runtime_error("no gold calc" + cr);
    potion;
}
int shop::calcGold(relicRarity rr)
{
    int multi_{ 10 };
    if (membershipDiscount && courierDiscount) multi_ = 4;
    else if (membershipDiscount) multi_ = 5;
    else if (courierDiscount) multi_ = 8;
    switch (rr)
    {
    case relicRarity::Common:
        return random_int(143, 157) * multi_ / 10;
    case relicRarity::Uncommon:
        return random_int(238, 262) * multi_ / 10;
    case relicRarity::Rare:
        return random_int(285, 315) * multi_ / 10;
    case relicRarity::Shop:
        return random_int(143, 157) * multi_ / 10;
    }
    throw runtime_error("no gold calc");
}
shop::shop()
{
    attack1 = gm.getCardReward({}, cardType::Attack);
    attack2 = gm.getCardReward({ attack1 }, cardType::Attack);
    skill1 = gm.getCardReward({}, cardType::Skill);
    skill2 = gm.getCardReward({ skill1 }, cardType::Skill);
    power1 = gm.getCardReward({}, cardType::Power);

    clrU1 = gm.getXRandomColourlessCards(1, Uncommon)[0];
    clrR2 = gm.getXRandomColourlessCards(1, Rare)[0];

    // 34 : 48 : 18
    int rr = random_int(1, 100);
    if (rr >= 1 && rr <= 34) relic1 = gm.retrieveCommonRelic();
    else if (rr >= 35 && rr <= 82) relic1 = gm.retrieveUncommonRelic();
    else { relic1 = gm.retrieveRareRelic(); }
    rr = random_int(1, 100);
    if (rr >= 1 && rr <= 34) relic2 = gm.retrieveCommonRelic();
    else if (rr >= 35 && rr <= 82) relic2 = gm.retrieveUncommonRelic();
    else { relic2 = gm.retrieveRareRelic(); }
    relic3 = gm.retrieveShopRelic();

    potion1 = gm.retrieveXRandomPotions(1)[0];
    potion2 = gm.retrieveXRandomPotions(1)[0];
    potion3 = gm.retrieveXRandomPotions(1)[0];

    assignOptions();
}
shop::~shop() { delete relic1; delete relic2; delete relic3; delete potion1; delete potion2; delete potion3; }
void shop::reRollPotion(int index, string _tag)
{
    if (_tag == "p1")
    {
        potion1 = gm.retrieveXRandomPotions(1)[0];
        shopOptions.emplace(shopOptions.begin() + index, potion1->getName(),
            [&]() {apci.addPotion(potion1); potion1 = nullptr; },
            calcGold(potion1->getRarity()), _tag);
    }
    else if (_tag == "p2")
    {
        potion2 = gm.retrieveXRandomPotions(1)[0];
        shopOptions.emplace(shopOptions.begin() + index, potion2->getName(),
            [&]() {apci.addPotion(potion2); potion2 = nullptr; },
            calcGold(potion2->getRarity()), _tag);
    }
    else if (_tag == "p3")
    {
        potion3 = gm.retrieveXRandomPotions(1)[0];
        shopOptions.emplace(shopOptions.begin() + index, potion3->getName(),
            [&]() {apci.addPotion(potion3); potion3 = nullptr; },
            calcGold(potion3->getRarity()), _tag);
    }
}
void shop::reRollRelic(int index,string _tag)
{
    if (_tag == "r1")
    {
        int rr = random_int(1, 100);
        if (rr >= 1 && rr <= 34) relic1 = gm.retrieveCommonRelic();
        else if (rr >= 35 && rr <= 82) relic1 = gm.retrieveUncommonRelic();
        else { relic1 = gm.retrieveRareRelic(); }

        shopOptions.emplace(shopOptions.begin() + index, relic1->getName(),
            [&]() {apci.addRelic(relic1); relic1 = nullptr; },
            calcGold(relic1->getRarity()), _tag);
    }
    else if (_tag == "r2")
    {
        int rr = random_int(1, 100);
        if (rr >= 1 && rr <= 34) relic2 = gm.retrieveCommonRelic();
        else if (rr >= 35 && rr <= 82) relic2 = gm.retrieveUncommonRelic();
        else { relic2 = gm.retrieveRareRelic(); }

        shopOptions.emplace(shopOptions.begin() + index, relic2->getName(),
            [&]() {apci.addRelic(relic2); relic2 = nullptr; },
            calcGold(relic2->getRarity()), _tag);
    }
    else if (_tag == "r3")
    {
        relic3 = gm.retrieveShopRelic();
        shopOptions.emplace(shopOptions.begin() + index, relic3->getName(),
            [&]() {apci.addRelic(relic3); relic3 = nullptr; },
            calcGold(relic3->getRarity()), _tag);
    }
}
void shop::reRollCard(int index, C_DATABASE::iterator& other_, string _tag)
{
    if (_tag == "a1")
    {
        attack1 = gm.getCardReward({ other_ }, Attack);
        shopOptions.emplace(shopOptions.begin() + index, attack1->second->getName(),
            [&]() {apci.addCardToDeck(attack1); },
            calcGold(attack1->second->getRarity()), _tag);
    }
    else if (_tag == "a2")
    {
        attack2 = gm.getCardReward({ other_ }, Attack);
        shopOptions.emplace(shopOptions.begin() + index, attack2->second->getName(),
            [&]() {apci.addCardToDeck(attack2); },
            calcGold(attack2->second->getRarity()), _tag);
    }
    else if (_tag == "s1")
    {
        skill1 = gm.getCardReward({ other_ }, Skill);
        shopOptions.emplace(shopOptions.begin() + index, skill1->second->getName(),
            [&]() {apci.addCardToDeck(skill1); },
            calcGold(skill1->second->getRarity()), _tag);
    }
    else if (_tag == "s2")
    {
        skill2 = gm.getCardReward({ other_ }, Skill);
        shopOptions.emplace(shopOptions.begin() + index, skill2->second->getName(),
            [&]() {apci.addCardToDeck(skill2); },
            calcGold(skill2->second->getRarity()), _tag);
    }
    else if (_tag == "pw")
    {
        power1 = gm.getCardReward({ }, Power);
        shopOptions.emplace(shopOptions.begin() + index, power1->second->getName(),
            [&]() {apci.addCardToDeck(power1); },
            calcGold(power1->second->getRarity()), _tag);
    }
        
}
void shop::reRollColourlessCard(int index, string _tag)
{
    if (_tag == "uc")
    {
        clrU1 = gm.getXRandomColourlessCards(1,Uncommon)[0];
        shopOptions.emplace(shopOptions.begin() + index, clrU1->second->getName(),
            [&]() {apci.addCardToDeck(clrU1); },
            calcGoldColourless(clrU1->second->getRarity()), _tag);
    }
    else if (_tag == "rc")
    {
        clrR2 = gm.getXRandomColourlessCards(1, Rare)[0];
        shopOptions.emplace(shopOptions.begin() + index, clrR2->second->getName(),
            [&]() {apci.addCardToDeck(clrR2); },
            calcGoldColourless(clrR2->second->getRarity()), _tag);
    }
    
}
void shop::assignOptions()
{
    shopOptions.emplace_back(
        attack1->second->getName(),
        [&]() {apci.addCardToDeck(attack1); },
        calcGold(attack1->second->getRarity()), "a1"
    );
    shopOptions.emplace_back(
        attack2->second->getName(),
        [&]() {apci.addCardToDeck(attack2); },
        calcGold(attack2->second->getRarity()), "a2"
    );
    shopOptions.emplace_back(
        skill1->second->getName(),
        [&]() {apci.addCardToDeck(skill1); },
        calcGold(skill1->second->getRarity()), "s1"
    );
    shopOptions.emplace_back(
        skill2->second->getName(),
        [&]() {apci.addCardToDeck(skill2); },
        calcGold(skill2->second->getRarity()), "s2"
    );
    shopOptions.emplace_back(
        power1->second->getName(),
        [&]() {apci.addCardToDeck(power1); },
        calcGold(power1->second->getRarity()), "pw"
    );
    // colourless
    shopOptions.emplace_back(
        clrU1->second->getName(),
        [&]() {apci.addCardToDeck(clrU1); },
        calcGold(clrU1->second->getRarity()), "uc"
    );
    shopOptions.emplace_back(
        clrR2->second->getName(),
        [&]() {apci.addCardToDeck(clrR2); },
        calcGold(clrR2->second->getRarity()), "rc"
    );
    // relics
    shopOptions.emplace_back(
        relic1->getName(),
        [&]() {apci.addRelic(relic1); relic1 = nullptr; },
        calcGold(relic1->getRarity()), "r1"
    );
    shopOptions.emplace_back(
        relic2->getName(),
        [&]() {apci.addRelic(relic2); relic2 = nullptr; },
        calcGold(relic2->getRarity()), "r2"
    );
    shopOptions.emplace_back(
        relic3->getName(),
        [&]() {apci.addRelic(relic3); relic3 = nullptr; },
        calcGold(relic3->getRarity()), "r3"
    );
    // potions
    shopOptions.emplace_back(
        potion1->getName(),
        [&]() {apci.addPotion(potion1); potion1 = nullptr; },
        calcGold(potion1->getRarity()), "p1"
    );
    shopOptions.emplace_back(
        potion2->getName(),
        [&]() {apci.addPotion(potion2); potion2 = nullptr; },
        calcGold(potion2->getRarity()), "p2"
    );
    shopOptions.emplace_back(
        potion3->getName(),
        [&]() {apci.addPotion(potion3); potion3 = nullptr; },
        calcGold(potion3->getRarity()), "p3"
    );
    int multi_{ 10 };
    if (membershipDiscount && courierDiscount) multi_ = 4;
    else if (membershipDiscount) multi_ = 5;
    else if (courierDiscount) multi_ = 8;
    // remove
    shopOptions.emplace_back(
        "Card Remove",
        [&]() {gm.makeCardChoices(aiEventCardChoice::Remove);  gm.removeCardFromDeck(); shop::cardRemove += 25; },
        smilingmaskDiscount ? 50 : cardRemove * multi_ /10, "cr"
    );
}
void shop::startEvent()
{
    if (!ai) prelude();
    gm.callShopEntered();
    std::cout << intro << endl;
    bool firstLoop{ true };
    while (true)
    {
        int n{ 1 };
        vector<string> inputs_str = { "leave" };
        gamestate::aiCardChoices.clear();
        vector<pair<relic*, int>> rel_cost;
        vector<pair<potion*, int>> pot_cost;
        vector<pair<C_DATABASE::iterator, int>> card_cost;
        for (int j{ 0 }; j < shopOptions.size(); ++j)
        {
            if (shopOptions[j].goldCost <= apci.getGold() && (shopOptions[j].tag_[0] != 'p' || (!apci.potionsFull() && !gamestate::sozuCheck || shopOptions[j].tag_[1] == 'w')))
            {
                std::cout << to_string(n) << ": " << shopOptions[j].name_ << " for " << shopOptions[j].goldCost << " gold. " << endl;
                inputs_str.push_back(to_string(n++));
                
            }
            else
            {
                std::cout << "Unavailable: " << shopOptions[j].name_ << " for " << shopOptions[j].goldCost << " gold. " << endl;
                ++n;
            }
            if (shopOptions[j].tag_ == "a1") card_cost.push_back({ attack1,shopOptions[j].goldCost });
            else if (shopOptions[j].tag_ == "a2") card_cost.push_back({ attack2,shopOptions[j].goldCost });
            else if (shopOptions[j].tag_ == "s1") card_cost.push_back({ skill1,shopOptions[j].goldCost });
            else if (shopOptions[j].tag_ == "s2") card_cost.push_back({ skill2,shopOptions[j].goldCost });
            else if (shopOptions[j].tag_ == "pw") card_cost.push_back({ power1,shopOptions[j].goldCost });
            else if (shopOptions[j].tag_ == "uc") card_cost.push_back({ clrU1,shopOptions[j].goldCost });
            else if (shopOptions[j].tag_ == "rc") card_cost.push_back({ clrR2,shopOptions[j].goldCost });
            else if (shopOptions[j].tag_ == "p1" && !apci.potionsFull() && !gamestate::sozuCheck) pot_cost.push_back({ potion1,shopOptions[j].goldCost });
            else if (shopOptions[j].tag_ == "p2" && !apci.potionsFull() && !gamestate::sozuCheck) pot_cost.push_back({ potion2,shopOptions[j].goldCost });
            else if (shopOptions[j].tag_ == "p3" && !apci.potionsFull() && !gamestate::sozuCheck) pot_cost.push_back({ potion3,shopOptions[j].goldCost });
            else if (shopOptions[j].tag_ == "r1") rel_cost.push_back({ relic1,shopOptions[j].goldCost });
            else if (shopOptions[j].tag_ == "r2") rel_cost.push_back({ relic2,shopOptions[j].goldCost });
            else if (shopOptions[j].tag_ == "r3") rel_cost.push_back({ relic3,shopOptions[j].goldCost });
        }
        if (inputs_str.size() == 1) break;
        std::cout << "Or leave" << endl;
        if(!ai) apci.addPotionInputs(inputs_str);
        std::string result;
        if (ai && gamestate::aiSimActionBuffer.empty()) gm.makeShopChoices(rel_cost,pot_cost,card_cost,shopOptions.back().tag_ == "cr" ? shopOptions.back().goldCost : 100000,firstLoop);
        result = request_input(inputs_str);
        std::cout << "RESULT: " << result << endl;
        if (result[0] == 'p') { apci.potionMenu(static_cast<int>(result[1] - 49)); continue; }
        if (result == "leave") break;
        int chosenIndex = stoi(result) - 1;
        apci.spendGold(shopOptions[chosenIndex].goldCost);
        shopOptions[chosenIndex].eventOutcome();
        string thisTag_{ shopOptions[chosenIndex].tag_ };
        shopOptions.erase(shopOptions.begin() + chosenIndex); 
        if (courierDiscount)
        {
            
            if      (thisTag_ == "a1") reRollCard(chosenIndex, attack2, thisTag_);
            else if (thisTag_ == "a2") reRollCard(chosenIndex, attack1, thisTag_);
            else if (thisTag_ == "s1") reRollCard(chosenIndex, skill2, thisTag_);
            else if (thisTag_ == "s2") reRollCard(chosenIndex, skill1, thisTag_);
            else if (thisTag_ == "pw") reRollCard(chosenIndex, attack1, thisTag_); // intentional
            else if (thisTag_ == "uc") reRollColourlessCard(chosenIndex, thisTag_);
            else if (thisTag_ == "rc") reRollColourlessCard(chosenIndex, thisTag_);
            else if (thisTag_ == "p1") reRollPotion(chosenIndex, thisTag_);
            else if (thisTag_ == "p2") reRollPotion(chosenIndex, thisTag_);
            else if (thisTag_ == "p3") reRollPotion(chosenIndex, thisTag_);
            else if (thisTag_ == "r1") reRollRelic(chosenIndex, thisTag_);
            else if (thisTag_ == "r2") reRollRelic(chosenIndex, thisTag_);
            else if (thisTag_ == "r3") reRollRelic(chosenIndex, thisTag_);
        }
        std::cout << "Gold: " << apci.getGold() << endl;
        if (firstLoop && ai)
        {
            gm.makePathChoiceFromNeutral(false);
            firstLoop = false;
        }
    }
}
void shop::handleShopStuff(shopStuff& ss)
{
    if (ss.isSingle())
    {
        if (ss.r1 != nullptr)
        {
            string desiredTag;
            if (ss.r1 == relic1) desiredTag = "r1";
            else if (ss.r1 == relic2) desiredTag = "r2";
            else { desiredTag = "r3"; }
            for (int i{ 0 }; i < shopOptions.size(); ++i)
            {
                if (shopOptions[i].tag_ == desiredTag)
                {
                    std::cout << ss.r1->getName() << " " << i+1 << endl;
                    gamestate::aiEventActionBuffer.push_back(to_string(i+1));
                    return;
                }
            }
        }
        else if (ss.p1 != nullptr)
        {
            string desiredTag;
            if (ss.p1 == potion1) desiredTag = "p1";
            else if (ss.p1 == potion2) desiredTag = "p2";
            else { desiredTag = "p3"; }
            for (int i{ 0 }; i < shopOptions.size(); ++i)
            {
                if (shopOptions[i].tag_ == desiredTag)
                {
                    std::cout << ss.p1->getName() << " " << i + 1 << endl;
                    gamestate::aiEventActionBuffer.push_back(to_string(i + 1));
                    return;
                }
            }
        }
        else if (ss.c1 != gm.getDatabaseEnd())
        {
            string desiredTag;
            if (ss.c1 == attack1) desiredTag = "a1";
            else if (ss.c1 == attack2) desiredTag = "a2";
            else if (ss.c1 == skill1) desiredTag = "s1";
            else if (ss.c1 == skill2) desiredTag = "s2";
            else { desiredTag = "pw"; }
            for (int i{ 0 }; i < shopOptions.size(); ++i)
            {
                if (shopOptions[i].tag_ == desiredTag)
                {
                    std::cout << ss.c1->second->getName() << " " << i + 1 << endl;
                    gamestate::aiEventActionBuffer.push_back(to_string(i + 1));
                    return;
                }
            }
        }
        else if (ss.cc1 != gm.getColourlessDatabaseEnd())
        {
            string desiredTag;
            if (ss.cc1 == clrU1) desiredTag = "uc";
            else { desiredTag = "rc"; }
            for (int i{ 0 }; i < shopOptions.size(); ++i)
            {
                if (shopOptions[i].tag_ == desiredTag)
                {
                    std::cout << ss.cc1->second->getName() << " " << i + 1 << endl;
                    gamestate::aiEventActionBuffer.push_back(to_string(i + 1));
                    return;
                }
            }
        }
        else
        {
            assert(ss.rm);
            std::cout << "remove " << shopOptions.size() << endl;
            gamestate::aiEventActionBuffer.push_back(to_string(shopOptions.size()));
            return;
        }
    }
    else
    {
        int firstIndex{0};
        if (ss.r1)
        {
            string desiredTag;
            if (ss.r1 == relic1) desiredTag = "r1";
            else if (ss.r1 == relic2) desiredTag = "r2";
            else { desiredTag = "r3"; }
            for (int i{ 0 }; i < shopOptions.size(); ++i)
            {
                if (shopOptions[i].tag_ == desiredTag)
                {
                    std::cout << ss.r1->getName() << " " << i + 1 << endl;
                    firstIndex = i+1;
                    break;
                }
            }
        }
        else if (ss.p1)
        {
            string desiredTag;
            if (ss.p1 == potion1) desiredTag = "p1";
            else if (ss.p1 == potion2) desiredTag = "p2";
            else { desiredTag = "p3"; }
            for (int i{ 0 }; i < shopOptions.size(); ++i)
            {
                if (shopOptions[i].tag_ == desiredTag)
                {
                    std::cout << ss.p1->getName() << " " << i + 1 << endl;
                    firstIndex = i+1;
                    break;
                }
            }
        }
        else if (ss.c1 != gm.getDatabaseEnd())
        {
            string desiredTag;
            if (ss.c1 == attack1) desiredTag = "a1";
            else if (ss.c1 == attack2) desiredTag = "a2";
            else if (ss.c1 == skill1) desiredTag = "s1";
            else if (ss.c1 == skill2) desiredTag = "s2";
            else { desiredTag = "pw"; }
            for (int i{ 0 }; i < shopOptions.size(); ++i)
            {
                if (shopOptions[i].tag_ == desiredTag)
                {
                    std::cout << ss.c1->second->getName() << " " << i + 1 << endl;
                    firstIndex = i+1;
                    break;
                }
            }
        }
        else if (ss.cc1 != gm.getColourlessDatabaseEnd())
        {
            string desiredTag;
            if (ss.cc1 == clrU1) desiredTag = "uc";
            else { desiredTag = "rc"; }
            for (int i{ 0 }; i < shopOptions.size(); ++i)
            {
                if (shopOptions[i].tag_ == desiredTag)
                {
                    std::cout << ss.cc1->second->getName() << " " << i + 1 << endl;
                    firstIndex = i+1;
                    break;
                }
            }
        }
        int secondIndex{ 0 };
        if (ss.r2)
        {
            string desiredTag;
            if (ss.r2 == relic1) desiredTag = "r1";
            else if (ss.r2 == relic2) desiredTag = "r2";
            else { desiredTag = "r3"; }
            for (int i{ 0 }; i < shopOptions.size(); ++i)
            {
                if (shopOptions[i].tag_ == desiredTag)
                {
                    std::cout << ss.r2->getName() << " " << i + 1 << endl;
                    secondIndex = i+1;
                    break;
                }
            }
        }
        else if (ss.p2)
        {
            string desiredTag;
            if (ss.p2 == potion1) desiredTag = "p1";
            else if (ss.p2 == potion2) desiredTag = "p2";
            else { desiredTag = "p3"; }
            for (int i{ 0 }; i < shopOptions.size(); ++i)
            {
                if (shopOptions[i].tag_ == desiredTag)
                {
                    std::cout << ss.p2->getName() << " " << i + 1 << endl;
                    secondIndex = i+1;
                    break;
                }
            }
        }
        else if (ss.c2 != gm.getDatabaseEnd())
        {
            string desiredTag;
            if (ss.c2 == attack1) desiredTag = "a1";
            else if (ss.c2 == attack2) desiredTag = "a2";
            else if (ss.c2 == skill1) desiredTag = "s1";
            else if (ss.c2 == skill2) desiredTag = "s2";
            else { desiredTag = "pw"; }
            for (int i{ 0 }; i < shopOptions.size(); ++i)
            {
                if (shopOptions[i].tag_ == desiredTag)
                {
                    std::cout << ss.c2->second->getName() << " " << i + 1 << endl;
                    secondIndex = i+1;
                    break;
                }
            }
        }
        else if (ss.cc2 != gm.getColourlessDatabaseEnd())
        {
            string desiredTag;
            if (ss.cc2 == clrU1) desiredTag = "uc";
            else { desiredTag = "rc"; }
            for (int i{ 0 }; i < shopOptions.size(); ++i)
            {
                if (shopOptions[i].tag_ == desiredTag)
                {
                    std::cout << ss.cc2->second->getName() << " " << i + 1 << endl;
                    secondIndex = i+1;
                    break;
                }
            }
        }
        else
        {
            assert(ss.rm);
            std::cout << "remove " << shopOptions.size() << endl;
            secondIndex = static_cast<int>(shopOptions.size());
        }

        if (firstIndex > secondIndex)
        {
            gamestate::aiSimActionBuffer.push_back(firstIndex);
            gamestate::aiSimActionBuffer.push_back(secondIndex);
        }
        else
        {
            gamestate::aiSimActionBuffer.push_back(secondIndex);
            gamestate::aiSimActionBuffer.push_back(firstIndex);
        }
    }
}

bool chest::nlothCheck{ false };
void chest::startEvent()
{
    if (!ai) prelude();
    gamestate::randomAIChoice = false;
    switch (size_)
    {
    case chestSize::Small:
        std::cout << "Small chest: 1 to Open. Or skip" << endl;
        if (!ai && request_input({ "1","skip" }) == "skip") { break; }
        else
        {
            gm.callChestOpened();
            if (nlothCheck) { nlothCheck = false; std::cout << "Chest empty." << endl; return; }
            relic* relic_ = random_int(1, 4) == 1 ? gm.retrieveUncommonRelic() : gm.retrieveCommonRelic();
            std::cout << "1 to take " << relic_->getName() << ". Or skip" << endl;
            if (request_input({ "1","skip" }) == "skip" && !ai) { delete relic_; break; }
            else
            {
                apci.addRelic(relic_);
                if (random_int(1, 2) == 1) apci.gainGold(random_int(23, 27));
            }
        }
        break;
    case chestSize::Medium:
        std::cout << "Medium chest: 1 to Open. Or skip" << endl;
        if (!ai && request_input({ "1","skip" }) == "skip") { break; }
        else
        {
            gm.callChestOpened();
            if (nlothCheck) { nlothCheck = false; std::cout << "Chest empty." << endl; return; }
            int res = random_int(1, 100);
            relic* relic_;
            if (res >= 1 && res <= 35) relic_ = gm.retrieveCommonRelic();
            else if (res >= 36 && res <= 85) relic_ = gm.retrieveUncommonRelic();
            else { relic_ = gm.retrieveRareRelic(); }
            std::cout << "1 to take " << relic_->getName() << ". Or skip" << endl;
            if (request_input({ "1","skip" }) == "skip" && !ai) { delete relic_; break; }
            else
            {
                apci.addRelic(relic_);
                if (random_int(1, 100) <= 35) apci.gainGold(random_int(45, 55));
            }
        }
        break;
    case chestSize::Large:
        std::cout << "Large chest: 1 to Open. Or skip" << endl;
        if (!ai && request_input({ "1","skip" }) == "skip") { break; }
        else
        {
            gm.callChestOpened();
            if (nlothCheck) { nlothCheck = false; std::cout << "Chest empty." << endl; return; }
            relic* relic_ = random_int(1, 4) == 1 ? gm.retrieveRareRelic() : gm.retrieveUncommonRelic();
            std::cout << "1 to take " << relic_->getName() << ". Or skip" << endl;
            if (request_input({ "1","skip" }) == "skip" && !ai) { delete relic_; break; }
            else
            {
                apci.addRelic(relic_);
                if (random_int(1, 2) == 1) apci.gainGold(random_int(68, 82));
            }
        }
        break;
    case chestSize::Boss:
        std::cout << "Boss chest: 1 to Open. Or skip" << endl;
        if (!ai && request_input({ "1","skip" }) == "skip") { break; }
        else
        {
            vector<relic*> relics_;
            relics_.push_back(gm.retrieveBossRelic());
            relics_.push_back(gm.retrieveBossRelic());
            relics_.push_back(gm.retrieveBossRelic());
            std::cout << "1 to take " << relics_[0]->getName() << ". ";
            std::cout << "2 to take " << relics_[1]->getName() << ". ";
            std::cout << "3 to take " << relics_[2]->getName() << ". ";
            std::cout << "Or skip" << endl;
            if (ai)
            {
                gm.makeBossRelicChoices(relics_);
                //gamestate::aiEventActionBuffer.push_back(to_string(apci.compareBossRelics(relics_, gm.getActNumber()) + 1));
            }
            string res = request_input({ "1","2","3","skip" });
            if (res != "skip")
            {
                int intRes = stoi(res);
                apci.addRelic(relics_[intRes - 1]);
                relics_[intRes - 1] = nullptr;
            }
            for (int i{ 0 }; i < 3; ++i)
            {
                delete relics_[i];
            }
        }
        break;
    }
}

void neowEvent::startEvent()
{
    firstRNG = random_int(0, 10);
    secondRNG = random_intNot(0, 10,firstRNG);
    thirdGoodRNG = random_int(0, 6);
    
    if (thirdGoodRNG == 2) thirdBadRNG = random_int(0,2);
    else if (thirdGoodRNG == 6) thirdBadRNG = random_int(1,3);
    else { thirdBadRNG = random_int(0, 3); }

    printOptions();
    if (ai) gm.makeNeowChoices(firstRNG, secondRNG, thirdGoodRNG, thirdBadRNG);
    choiceOutcomes(stoi(request_input({ "1","2","3","4" })));
}
void neowEvent::printOptions()
{
    std::cout << "1: ";
    switch (firstRNG)
    {
    case 0:
        std::cout << "Gain 8 Max HP" << endl; break;
    case 1:
        std::cout << "Obtain Neow's Lament" << endl; break;
    case 2:
        std::cout << "Remove Card From Deck" << endl; break;
    case 3:
        std::cout << "Transform Card From Deck" << endl; break;
    case 4:
        std::cout << "Upgrade Card From Deck" << endl; break;
    case 5:
        std::cout << "Choose Card Reward" << endl; break;
    case 6:
        std::cout << "Choose Colourless Card Reward" << endl; break;
    case 7:
        std::cout << "Random Rare Card" << endl; break;
    case 8:
        std::cout << "Obtain Common Relic" << endl; break;
    case 9:
        std::cout << "Gain 100 Gold" << endl; break;
    case 10:
        std::cout << "Obtain 3 Potions" << endl; break;
    }
    std::cout << "2: ";
    switch (secondRNG)
    {
    case 0:
        std::cout << "Gain 8 Max HP" << endl; break;
    case 1:
        std::cout << "Obtain Neow's Lament" << endl; break;
    case 2:
        std::cout << "Remove Card From Deck" << endl; break;
    case 3:
        std::cout << "Transform Card From Deck" << endl; break;
    case 4:
        std::cout << "Upgrade Card From Deck" << endl; break;
    case 5:
        std::cout << "Choose Card Reward" << endl; break;
    case 6:
        std::cout << "Choose Colourless Card Reward" << endl; break;
    case 7:
        std::cout << "Random Rare Card" << endl; break;
    case 8:
        std::cout << "Obtain Common Relic" << endl; break;
    case 9:
        std::cout << "Gain 100 Gold" << endl; break;
    case 10:
        std::cout << "Obtain 3 Potions" << endl; break;
    }
    // disadv
    std::cout << "3: ";
    switch (thirdBadRNG)
    {
    case 0:
        std::cout << "Lose 8 Max HP - ";
        break;
    case 1:
        std::cout << "Lose 24 HP - ";
        break;
    case 2:
        std::cout << "Curse - Random - ";
        break;
    case 3:
        std::cout << "Lose all Gold - ";
        break;
    }
    // adv
    switch (thirdGoodRNG)
    {
    case 0:
        std::cout << "Remove 2 cards from deck" << endl;
        break;
    case 1:
        std::cout << "Transform 2 cards from deck" << endl;
        break;
    case 2:
        std::cout << "Gain 250 Gold" << endl;
        break;
    case 3:
        std::cout << "Choose Rare Card Reward" << endl;
        break;
    case 4:
        std::cout << "Choose Colourless Rare Card Reward" << endl;
        break;
    case 5:
        std::cout << "Obtain Rare Relic" << endl;
        break;
    case 6:
        std::cout << "Gain 16 Max HP" << endl;
        break;
    }
    std::cout << "4: Lose Burning Blood - Obtain Random Boss Relic" << endl;
}
void neowEvent::choiceOutcomes(int choice)
{
    int rng = secondRNG;
    switch (choice)
    {
    case 0:// intentional
        rng = firstRNG; // intentional
    case 1:// intentional
        switch (rng)
        {
        case 0:
            apci.getPlayer().gainMaxHP(8);
            break;
        case 1:
            apci.addRelic(gm.getRelicByID("NeowsLament_ID")->clone());
            break;
        case 2:
            if (ai) gm.makeCardChoices(aiEventCardChoice::Remove);
            gm.removeCardFromDeck();
            break;
        case 3:
            if (ai) gm.makeCardChoices(aiEventCardChoice::Transform);
            gm.transformCardFromDeck();
            break;
        case 4:
            if (ai) gm.makeCardChoices(aiEventCardChoice::Upgrade);
            gm.upgradeCardFromDeck();
            break;
        case 5:
            gm.hallwayReward(false);
            break;
        case 6:
            gm.colourlessReward();
            break;
        case 7:
        {
            vector<string> opts = gm.getRareCards();
            opts.push_back("Feed_ID");
            opts.push_back("Reaper_ID");
            apci.addCardToDeck(gm.getCardByID(opts[random_int(0, static_cast<int>(opts.size()) - 1)]));
            break;
        }
        case 8:
            apci.addRelic(gm.retrieveCommonRelic());
            break;
        case 9:
            apci.gainGold(100);
            break;
        case 10:
            gm.XPotionSelection(gm.retrieveXRandomPotions(3));
            break;
        }
        break;
        // disadv
    case 2:
        switch (thirdBadRNG)
        {
        case 0:
            apci.getPlayer().loseMaxHP(8);
            break;
        case 1:
            apci.getPlayer().setHealth(56);
            break;
        case 2:
            apci.addCardToDeck(gm.getCurse());
            break;
        case 3:
            apci.loseGold(apci.getGold());
            break;
        }
        // adv
        switch (thirdGoodRNG)
        {
        case 0:
            if (ai) gm.makeCardChoices(aiEventCardChoice::Remove);
            gm.removeCardFromDeck();
            if (ai) gm.makeCardChoices(aiEventCardChoice::Remove);
            gm.removeCardFromDeck();
            break;
        case 1:
            if (ai) gm.makeCardChoices(aiEventCardChoice::Transform2);
            gm.transformTwoFromDeck();
            break;
        case 2:
            apci.gainGold(250);
            break;
        case 3:
            gm.rareCardReward();
            break;
        case 4:
            gm.colourlessReward(cardRarity::Rare);
            break;
        case 5:
            apci.addRelic(gm.retrieveRareRelic());
            break;
        case 6:
            apci.getPlayer().gainMaxHP(16);
            break;
        }
        break;
    case 3:
        apci.removeRelicByName("Burning Blood");
        apci.addRelic(gm.retrieveBossRelic());
        break;
    }

    vector<int> opCH(24);
    switch (choice)
    {
    case 0:
        ++opCH[firstRNG];
        break;
    case 1:
        ++opCH[secondRNG];
        break;
    case 2:
        ++opCH[thirdBadRNG + 11];
        ++opCH[thirdGoodRNG + 15];
        break;
    case 3:
        ++opCH[22];
        break;
    }
    gm.setOpeningChoice(opCH);
}


bool shop::membershipDiscount{ false };
bool shop::courierDiscount{ false };
bool shop::smilingmaskDiscount{ false };
int shop::cardRemove{ 75 };
