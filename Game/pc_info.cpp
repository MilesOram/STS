#include "pc_info.h"
#include "effects_triggers.h"
#include "gamestate.h"
#include "relic.h"
#include "visuals.h"
#include "potions.h"
#include "savestate.h"
#include "ic_cards.h"

using namespace std;

all_pc_info::~all_pc_info()
{
    for (relic* r : relics) { delete r; } relics.clear();
    for (potion* p : potions) { delete p; } potions.clear();
}

void all_pc_info::printDeck()
{
    std::cout << "DECK: " << endl;
    for (int i{ 0 }; i < deck.size(); ++i)
    {
        std::cout << (*deck[i]).getName();
        if ((i + 1) % 4 == 0 && i != deck.size() - 1) { std::cout << "," << endl; continue; }
        if (i != deck.size() - 1) std::cout << ", ";
    }
    std::cout << "." << endl;
}
void all_pc_info::randomHandCostZero()
{
    if (hand.empty()) return;
    vector<int> potentials;
    for (int i{ 0 }; i < hand.size(); ++i)
    {
        if ((*hand[i]).getType() == Curse || ((*hand[i]).getType() == Status && (*hand[i]).getName() != "Slime")) continue;
        if ((*hand[i]).getEnergy() <= 0) continue;
        if ((*hand[i]).getZeroUntilPlayed()) continue;
        potentials.push_back(i);
    }
    if (potentials.empty()) return;
    makeCostZero(hand[potentials[random_int(0, potentials.size() - 1)]]);
}
void all_pc_info::chooseHandCostZero()
{
    if (hand.empty()) return;
    vector<string> inputs_str;
    vector<int> potentials;
    int j{ 1 };
    for (int i{ 0 }; i < hand.size(); ++i)
    {
        if ((*hand[i]).getType() != Curse && ((*hand[i]).getType() != Status || (*hand[i]).getName() == "Slime") && (*hand[i]).getEnergy() != -1)
        {
            std::cout << j << " for " << (*hand[i]).getName() << ". ";
            potentials.push_back(i);
            inputs_str.push_back(to_string(j++));
        }
    }
    if (potentials.empty()) return;
    std::string result{ request_input(inputs_str) };
    int res = potentials[stoi(result) - 1];
    makeCostZero(hand[res]);
}
void all_pc_info::resetIronclad()
{
    for (int i{ 0 }; i < relics.size(); ++i)
    {
        relics[i]->onDropped();
        delete relics[i];
    }
    relics.clear();
    for (int i{ 0 }; i < potions.size(); ++i)
    {
        delete potions[i];
    }
    potions.clear();
    while (!deck.empty()) { deck.pop_back(); }
    while (!draw_pile.empty()) draw_pile.pop_back();
    while (!hand.empty()) hand.pop_back();
    while (!discard_pile.empty()) discard_pile.pop_back();
    while (!exhausted.empty()) exhausted.pop_back();
    while (!the_stack.empty()) the_stack.pop_back();
}
void all_pc_info::initIronclad()
{
    potionSlots = 3;
    gold_ = 99;
    player = make_unique<pc>(80);
    
    // BASE DECK
    for (int i{0}; i<5; ++i)
    {
        deck.push_back(gm.getCardByID("Strike_ID"));
    }
    for(int i{0};i<4;++i)
    {
        deck.push_back(gm.getCardByID("Defend_ID"));
    }
    deck.push_back(gm.getCardByID("Bash_ID"));
    addRelic(gm.getRelicByID("BurningBlood_ID"));
}
void all_pc_info::changeDeck1()
{
    for (int i{ 0 }; i < relics.size(); ++i)
    {
        relics[i]->onDropped();
        delete relics[i];
    }
    relics.clear();
    while (!deck.empty()) {deck.pop_back();}
    for (int i{ 0 }; i < 4; ++i)
    {
        deck.push_back(gm.getCardByID("Strike_ID"));
    }
    for (int i{ 0 }; i < 3; ++i)
    {
        deck.push_back(gm.getCardByID("ShrugItOff_ID"));
        (*(deck.end() - 1))->upgradeThis();
    }
    deck.push_back(gm.getCardByID("Bash_ID"));
    (*(deck.end() - 1))->upgradeThis();
    deck.push_back(gm.getCardByID("Corruption_ID"));
    (*(deck.end() - 1))->upgradeThis();
    deck.push_back(gm.getCardByID("FeelNoPain_ID"));
    deck.push_back(gm.getCardByID("DarkEmbrace_ID"));
    (*(deck.end() - 1))->upgradeThis();
    deck.push_back(gm.getCardByID("BurningPact_ID"));
    deck.push_back(gm.getCardByID("Hemokinesis_ID"));
    (*(deck.end() - 1))->upgradeThis();
    deck.push_back(gm.getCardByID("BloodForBlood_ID"));
    (*(deck.end() - 1))->upgradeThis();
    deck.push_back(gm.getCardByID("RecklessCharge_ID"));
    deck.push_back(gm.getCardByID("Offering_ID"));
}
void all_pc_info::changeDeck2()
{
    for (int i{ 0 }; i < relics.size(); ++i)
    {
        relics[i]->onDropped();
        delete relics[i];
    }
    relics.clear();
    while (!deck.empty()) {deck.pop_back();}
    for (int i{ 0 }; i < 5; ++i)
    {
        deck.push_back(gm.getCardByID("Strike_ID"));
    }
    (*(deck.end() - 1))->upgradeThis();
    for (int i{ 0 }; i < 4; ++i)
    {
        deck.push_back(gm.getCardByID("Defend_ID"));
    }
    deck.push_back(gm.getCardByID("Bash_ID"));
    deck.push_back(gm.getCardByID("Ironwave_ID"));
    deck.push_back(gm.getCardByID("Anger_ID"));
    deck.push_back(gm.getCardByID("TrueGrit_ID"));
    (*(deck.end() - 1))->upgradeThis();
    deck.push_back(gm.getCardByID("Armaments_ID"));
    deck.push_back(gm.getCardByID("BattleTrance_ID"));
    deck.push_back(gm.getColourlessCardByID("Panache_ID"));
    deck.push_back(gm.getColourlessCardByID("SecretWeapon_ID"));
    apci.addRelic(gm.getRelicByID("BurningBlood_ID"));
}
void all_pc_info::changeDeck3()
{
    for (int i{ 0 }; i < relics.size(); ++i)
    {
        relics[i]->onDropped();
        delete relics[i];
    }
    relics.clear();
    while (!deck.empty()) {deck.pop_back();}
    for (int i{ 0 }; i < 4; ++i)
    {
        deck.push_back(gm.getCardByID("Strike_ID"));
    }
    (*(deck.end() - 1))->upgradeThis();
    for (int i{ 0 }; i < 3; ++i)
    {
        deck.push_back(gm.getCardByID("Defend_ID"));
    }
    deck.push_back(gm.getCardByID("Bash_ID"));
    (*(deck.end() - 1))->upgradeThis();
    deck.push_back(gm.getCardByID("BloodForBlood_ID"));
    deck.push_back(gm.getCardByID("Impervious_ID"));
    deck.push_back(gm.getCardByID("FeelNoPain_ID"));
    deck.push_back(gm.getCardByID("Shockwave_ID"));
    deck.push_back(gm.getCardByID("BurningPact_ID"));
    deck.push_back(gm.getCardByID("DemonForm_ID"));
    deck.push_back(gm.getColourlessCardByID("SadisticNature_ID"));
    deck.push_back(gm.getColourlessCardByID("PanicButton_ID"));
}
void all_pc_info::makeStageDeck(curriculumStage cs)
{
    for (int i{ 0 }; i < relics.size(); ++i)
    {
        relics[i]->onDropped();
        delete relics[i];
    }
    relics.clear();
    for (int i{ 0 }; i < potions.size(); ++i)
    {
        delete potions[i];
    }
    potions.clear();
    while (!deck.empty()) { deck.pop_back(); }
    potionSlots = 3;
    gold_ = 100;
    switch (cs)
    {
    case curriculumStage::Stage1:
    {
        makeDeck({ "Strike","Strike","Strike","Strike","Strike","Defend","Defend","Defend","Defend","Bash" });
        if (random_int(0, 1)) addPotion(gm.retrieveXRandomPotions(1)[0]);
        if (random_int(0, 1)) addPotion(gm.retrieveXRandomPotions(1)[0]);
        int rng = random_int(1, 4);
        if (rng >= 1)
        {
            if (random_int(0, 1))
            {
                auto UCs = gm.getUncommonCards();
                apci.addCardToDeck(gm.getCardByID(UCs[random_int(0, UCs.size() - 1)]));
            }
            else
            {
                auto CCs = gm.getCommonCards();
                apci.addCardToDeck(gm.getCardByID(CCs[random_int(0, CCs.size() - 1)]));
            }
        }
        if (rng >= 2)
        {
            if (random_int(0, 1))
            {
                auto UCs = gm.getUncommonCards();
                apci.addCardToDeck(gm.getCardByID(UCs[random_int(0, UCs.size() - 1)]));
            }
            else
            {
                auto CCs = gm.getCommonCards();
                apci.addCardToDeck(gm.getCardByID(CCs[random_int(0, CCs.size() - 1)]));
            }
        }
        break;
    }
    case curriculumStage::Stage2:
        switch (random_int(0, 5))
        {
        case 0: // strength
            std::cout << "Strength: ---vvv---" << endl;
            makeDeck({ "Strike","Strike","Strike","Strike","Strike","Defend","Defend","Defend+","Defend","Bash",
                "Inflame","SpotWeakness","LimitBreak","TwinStrike","DemonForm","Pummel","Impervious","SwordBoomerang"});
            if (random_int(0, 7)) addRelic(gm.getRelicByID("Vajra_ID"));
            break;
        case 1: // standard exhaust
            std::cout << "Exhaust: ---vvv---" << endl;
            makeDeck({ "Strike","Strike+","Strike","Strike","Strike","Defend","Defend","Defend","Defend","Bash",
                "FeelNoPain","TrueGrit+","SecondWind","FiendFire","DarkEmbrace","SeeingRed","SeverSoul","Sentinel"});
            if (random_int(0, 7)) addRelic(gm.getRelicByID("LetterOpener_ID"));
            break;
        case 2: // block
            std::cout << "Block: ---vvv---" << endl;
            makeDeck({ "Strike","Strike","Strike","Strike","Strike","Defend+","Defend","Defend","Defend","Bash",
                "Bodyslam","Bodyslam+","Entrench","Juggernaut","Barricade+","SeeingRed","ShrugItOff" });
            if (random_int(0, 7)) addRelic(gm.getRelicByID("OddlySmoothStone_ID"));
            if (random_int(0, 7)) addRelic(gm.getRelicByID("MummifiedHand_ID"));

            break;
        case 3: // corruption
            std::cout << "Corruption: ---vvv---" << endl;
            makeDeck({ "Strike","Strike","Strike","Strike","Strike","Defend","Defend","Defend","Defend","Bash",
                "Corruption","Havoc","GhostlyArmor","Shockwave","Anger","ShrugItOff","TrueGrit","Whirlwind"});
            if (random_int(0, 7)) addRelic(gm.getRelicByID("DeadBranch_ID"));
            break;
        case 4: // status
            std::cout << "Status: ---vvv---" << endl;
            makeDeck({ "Strike","Strike","Strike","Strike","Strike","Defend","Defend","Defend","Defend","Bash",
                "Evolve","FireBreathing","FeelNoPain","PowerThrough","PowerThrough","WildStrike","BattleTrance"});
            if (random_int(0, 7)) addRelic(gm.getRelicByID("MedKit_ID"));
            break;
        case 5: // infinite
            std::cout << "Infinte: ---vvv---" << endl;
            makeDeck({ "BurningPact","PommelStrike+","PommelStrike+","TrueGrit","Bash","Thunderclap","Dropkick","Dropkick"});
            if (random_int(0, 5)) addRelic(gm.getRelicByID("Sundial_ID"));
            if (random_int(0, 5)) addRelic(gm.getRelicByID("BagOfPreparation_ID"));
            break;
        }
        if (random_int(0, 1)) addPotion(gm.retrieveXRandomPotions(1)[0]);
        removeCard(deck.begin() + random_int(0, deck.size() - 1));
        if (random_int(0, 1))
        {
            auto UCs = gm.getUncommonCards();
            apci.addCardToDeck(gm.getCardByID(UCs[random_int(0, UCs.size() - 1)]));
        }
        else
        {
            auto RCs = gm.getRareCards();
            apci.addCardToDeck(gm.getCardByID(RCs[random_int(0, RCs.size() - 1)]));
        }
        break;
    case curriculumStage::Stage3:
        switch (random_int(0, 6))
        {
        case 0: // strength
            std::cout << "Strength: ---vvv---" << endl;
            makeDeck({ "Strike","Strike","Strike","Strike","Strike","Defend","Defend","Defend+","Defend","Bash",
                "Inflame","SpotWeakness","LimitBreak","TwinStrike","DemonForm","Pummel","Impervious","SwordBoomerang" });
            if (random_int(0, 7)) addRelic(gm.getRelicByID("Vajra_ID"));
            break;
        case 1: // standard exhaust
            std::cout << "Exhaust: ---vvv---" << endl;
            makeDeck({ "Strike","Strike+","Strike","Strike","Strike","Defend","Defend","Defend","Defend","Bash",
                "FeelNoPain","TrueGrit+","SecondWind","FiendFire","DarkEmbrace","SeeingRed","SeverSoul","Sentinel" });
            if (random_int(0, 7)) addRelic(gm.getRelicByID("LetterOpener_ID"));
            break;
        case 2: // block
            std::cout << "Block: ---vvv---" << endl;
            makeDeck({ "Strike","Strike","Strike","Strike","Strike","Defend+","Defend","Defend","Defend","Bash",
                "Bodyslam","Bodyslam+","Entrench","Juggernaut","Barricade+","SeeingRed","ShrugItOff" });
            if (random_int(0, 7)) addRelic(gm.getRelicByID("OddlySmoothStone_ID"));
            if (random_int(0, 7)) addRelic(gm.getRelicByID("MummifiedHand_ID"));

            break;
        case 3: // corruption
            std::cout << "Corruption: ---vvv---" << endl;
            makeDeck({ "Strike","Strike","Strike","Strike","Strike","Defend","Defend","Defend","Defend","Bash",
                "Corruption","Havoc","GhostlyArmor","Shockwave","Anger","ShrugItOff","TrueGrit","Whirlwind" });
            if (random_int(0, 7)) addRelic(gm.getRelicByID("DeadBranch_ID"));
            break;
        case 4: // status
            std::cout << "Status: ---vvv---" << endl;
            makeDeck({ "Strike","Strike","Strike","Strike","Strike","Defend","Defend","Defend","Defend","Bash",
                "Evolve","FireBreathing","FeelNoPain","PowerThrough","PowerThrough","WildStrike","BattleTrance"});
            if (random_int(0, 7)) addRelic(gm.getRelicByID("MedKit_ID"));
            break;
        case 5: // infinite
            std::cout << "Infinte: ---vvv---" << endl;
            makeDeck({ "BurningPact","PommelStrike+","PommelStrike+","TrueGrit","Bash","Thunderclap","Dropkick","Dropkick" });
            if (random_int(0, 5)) addRelic(gm.getRelicByID("Sundial_ID"));
            if (random_int(0, 5)) addRelic(gm.getRelicByID("BagOfPreparation_ID"));
            break;
        case 6: // life loss
            std::cout << "Life Loss: ---vvv---" << endl;
            makeDeck({ "Strike","Strike","Strike","Strike","Strike","Defend","Defend","Defend","Defend","Bash",
                "Brutality","Hemokinesis","Whirlwind","Rupture","Rupture","Combust","Metallicize","Offering" });
            if (random_int(0, 7)) addRelic(gm.getRelicByID("Orichalcum_ID"));
            if (random_int(0, 7)) addRelic(gm.getRelicByID("Necronomicon_ID"));
            if (random_int(0, 7)) addRelic(gm.getRelicByID("BlueCandle_ID"));
            break;
        }
        removeCard(deck.begin() + random_int(0, deck.size() - 1));
        if (random_int(0, 1)) addPotion(gm.retrieveXRandomPotions(1)[0]);
        if (random_int(0, 1)) addPotion(gm.retrieveXRandomPotions(1)[0]);
        if (random_int(0, 1)) apci.addCardToDeck(gm.getXRandomColourlessCards(1)[0]->second->cloneU());
        break;
    }
}
void all_pc_info::changeDeck(int i)
{
    for (int n{ 0 }; n < relics.size(); ++n)
    {
        relics[n]->onDropped();
        delete relics[n];
    }
    relics.clear();
    for (int j{ 0 }; j < potions.size(); ++j)
    {
        delete potions[j];
    }
    potions.clear();
    while (!deck.empty()) { deck.pop_back(); }
    gold_ = 100;
    potionSlots = 3;
    switch (i)
    {
    case 0:
        addRelic(gm.getRelicByID("Akabeko_ID"));
        addRelic(gm.getRelicByID("Orichalcum_ID"));
        addRelic(gm.getRelicByID("BronzeScales_ID"));
        addRelic(gm.getRelicByID("UnceasingTop_ID"));
        makeDeck({
            "Strike","Defend","Defend","Bash","Finesse1+","BattleTrance+","Anger+","ShrugItOff","Impervious+",
            "RecklessCharge","Shockwave+","InfernalBlade+","FeelNoPain+","Offering+","BattleTrance+","Impervious+",
            "Immolate+","Rage+","Inflame","Evolve+","Feed+","DarkEmbrace+","BurningPact+","WarCry+","Finesse1+","Purity1"
            });
        break;
    case 1:
        addRelic(gm.getRelicByID("Kunai_ID"));
        addRelic(gm.getRelicByID("PhilosophersStone_ID"));
        addRelic(gm.getRelicByID("HornCleat_ID"));
        addRelic(gm.getRelicByID("Shuriken_ID"));
        addRelic(gm.getRelicByID("DeadBranch_ID"));
        makeDeck({
            "Strike","Strike","Defend","Defend","GhostlyArmor","Sentinel+","Bash","SpotWeakness","Anger","Immolate","ShrugItOff",
            "Reaper","ShrugItOff","Evolve","Shockwave","BurningPact","Bloodletting","Headbutt"
            });
        break;
    case 2:
        addRelic(gm.getRelicByID("Akabeko_ID"));
        addRelic(gm.getRelicByID("Vajra_ID"));
        addRelic(gm.getRelicByID("CoffeeDripper_ID"));
        addRelic(gm.getRelicByID("MeatOnTheBone_ID"));
        addRelic(gm.getRelicByID("CharonsAshes_ID"));
        addRelic(gm.getRelicByID("DuVuDoll_ID"));
        addRelic(gm.getRelicByID("BagOfMarbles_ID"));
        makeDeck({
            "Strike","Strike","Strike","Defend","Defend","Defend","Defend","Bash","Shockwave","Pummel","BattleTrance",
            "ShrugItOff","Offering","Disarm","Rage","DualWield","ShrugItOff","SeeingRed","Clumsy3","Havoc+","DoubleTap",
            "LimitBreak+","LimitBreak"
            });
        break;
    case 3:
        addRelic(gm.getRelicByID("PhilosophersStone_ID"));
        addRelic(gm.getRelicByID("Anchor_ID"));
        addRelic(gm.getRelicByID("Lantern_ID"));
        addRelic(gm.getRelicByID("Brimstone_ID"));
        addRelic(gm.getRelicByID("SlaversCollar_ID"));
        addRelic(gm.getRelicByID("TheBoot_ID"));
        addRelic(gm.getRelicByID("PreservedInsect_ID"));
        addRelic(gm.getRelicByID("SelfFormingClay_ID"));
        makeDeck({
            "Strike","Strike","Strike","Strike","Defend","Defend","Defend+","Defend","Bash","PommelStrike+",
            "Disarm+","Pummel+","Anger+","Carnage+","BurningPact","FlashOfSteel1+","Whirlwind+","ShrugItOff+",
            "PommelStrike+","ShrugItOff","Disarm+","Clothesline+","DarkEmbrace+","Corruption","GhostlyArmor","Sentinel+"
            });
        break;
    case 4:
        addRelic(gm.getRelicByID("Sundial_ID"));
        addRelic(gm.getRelicByID("CaptainsWheel_ID"));
        addRelic(gm.getRelicByID("LetterOpener_ID"));
        addRelic(gm.getRelicByID("TheBoot_ID"));
        addRelic(gm.getRelicByID("CentennialPuzzle_ID"));
        makeDeck({
            "Strike","Defend","Defend","Bash","Apotheosis1+","SecondWind","Dropkick","Dropkick","TrueGrit+",
            "RecklessCharge","Shockwave+","InfernalBlade+","FeelNoPain+","Offering+","BattleTrance+","FiendFire+",
            "PommelStrike+","PommelStrike+","FeelNoPain","Metallicize","GhostlyArmor","Sentinel+"
            });
        break;
    case 5:
        addRelic(gm.getRelicByID("OrnamentalFan_ID"));
        addRelic(gm.getRelicByID("SneckoEye_ID"));
        addRelic(gm.getRelicByID("HornCleat_ID"));
        addRelic(gm.getRelicByID("MummifiedHand_ID"));
        addRelic(gm.getRelicByID("FossilizedHelix_ID"));
        addRelic(gm.getRelicByID("LizardTail_ID"));
        makeDeck({
            "Strike","Strike","Defend","Defend","Defend","Defend","Bash","SpotWeakness","Anger","Carnage","ShrugItOff",
            "Reaper","ShrugItOff","Evolve","Shockwave+","BurningPact","Bloodletting","WildStrike","DemonForm",
            "Barricade","Entrench"
            });
        break;
    case 6:
        addRelic(gm.getRelicByID("StoneCalendar_ID"));
        addRelic(gm.getRelicByID("ThreadAndNeedle_ID"));
        addRelic(gm.getRelicByID("Torii_ID"));
        addRelic(gm.getRelicByID("MeatOnTheBone_ID"));
        addRelic(gm.getRelicByID("CharonsAshes_ID"));
        addRelic(gm.getRelicByID("Brimstone_ID"));
        addRelic(gm.getRelicByID("CoffeeDripper_ID"));
        addRelic(gm.getRelicByID("ChemicalX_ID"));
        makeDeck({
            "Strike","Strike","Defend","Defend","Bash","Shockwave","Pummel","BattleTrance","Havoc","Uppercut","Discovery1+",
            "ShrugItOff","Offering","Whirlwind","Rage","DualWield","PanicButton1","SeeingRed","Clumsy3","Havoc+","DoubleTap"
            });
        break;
    case 7:
        addRelic(gm.getRelicByID("FusionHammer_ID"));
        addRelic(gm.getRelicByID("Tungsten_ID"));
        addRelic(gm.getRelicByID("InkBottle_ID"));
        addRelic(gm.getRelicByID("GremlinHorn_ID"));
        addRelic(gm.getRelicByID("BirdFacedUrn_ID"));
        makeDeck({
            "Strike","Strike","Strike","Strike","Defend","Defend","Defend+","Defend","Bash","PommelStrike+",
            "Disarm+","Pummel+","Anger+","Carnage+","BurningPact","FlashOfSteel1+","Whirlwind+","ShrugItOff+",
            "Juggernaut+","ShrugItOff","Disarm+","Clothesline+","DarkEmbrace+","Corruption","Violence1"
            });
        break;
    case 8:
        addRelic(gm.getRelicByID("MedKit_ID"));
        addRelic(gm.getRelicByID("HandDrill_ID"));
        addRelic(gm.getRelicByID("BustedCrown_ID"));
        addRelic(gm.getRelicByID("ChampionsBelt_ID"));
        addRelic(gm.getRelicByID("PaperPhrog_ID"));
        makeDeck({
            "Strike","Strike","Strike","Defend","Defend+","Defend","Bash","PommelStrike+",
            "Disarm+","Pummel+","Anger+","Carnage+","BurningPact","Chrysalis1+","ShrugItOff+",
            "Apparition2+","Apparition2+","Apparition2+","Blind1","TheBomb1+"
            });
        break;
    case 9:
        addRelic(gm.getRelicByID("BlueCandle_ID"));
        addRelic(gm.getRelicByID("Necronomicon_ID"));
        addRelic(gm.getRelicByID("Ectoplasm_ID"));
        addRelic(gm.getRelicByID("Calipers_ID"));
        addRelic(gm.getRelicByID("TheAbacus_ID"));
        addRelic(gm.getRelicByID("Nunchuku_ID"));
        makeDeck({
            "Strike","Strike","Strike","Defend","Defend+","Defend","Bash","PommelStrike+",
            "Disarm+","Pummel+","Anger+","Carnage+","BurningPact","Chrysalis1+","ShrugItOff+",
            "Apparition2+","Apparition2+","Apparition2+","Blind1","Rupture+","Rupture+","FeelNoPain"
            });
        break;
    case 10:
        addRelic(gm.getRelicByID("BurningBlood_ID"));
        addRelic(gm.getRelicByID("Vajra_ID"));
        addRelic(gm.getRelicByID("BagOfMarbles_ID"));
        addRelic(gm.getRelicByID("Akabeko_ID"));
        addRelic(gm.getRelicByID("CursedKey_ID"));
        addRelic(gm.getRelicByID("TheAbacus_ID"));
        addRelic(gm.getRelicByID("IceCream_ID"));
        addRelic(gm.getRelicByID("PotionBelt_ID"));
        addRelic(gm.getRelicByID("ArtOfWar_ID"));
        makeDeck({
            "Clumsy3","Clumsy3","Bash+","Offering+","Offering+","BurningPact+","BloodForBlood","Armaments+",
            "FeelNoPain+","Impervious+","DualWield+","FeelNoPain+","BurningPact+","Shockwave+","PowerThrough+",
            "Dropkick+","DarkEmbrace+","DarkEmbrace+","Exhume+","Impervious+","Entrench+","FiendFire","Combust",
            "BurningPact+","Impervious+","Bloodletting+","Finesse1+"
            });
        if (random_int(0, 1)) addPotion(gm.getPotionByID("StrengthPotion_ID"));
        if (random_int(0, 1)) addPotion(gm.getPotionByID("LiquidMemories_ID"));
        if (random_int(0, 1)) addPotion(gm.getPotionByID("WeakPotion_ID"));
        break;
    case 11:
        addRelic(gm.getRelicByID("BurningBlood_ID"));
        addRelic(gm.getRelicByID("BloodVial_ID"));
        addRelic(gm.getRelicByID("BronzeScales_ID"));
        addRelic(gm.getRelicByID("BagOfPreparation_ID"));
        addRelic(gm.getRelicByID("ClockworkSouvenir_ID"));
        addRelic(gm.getRelicByID("CoffeeDripper_ID"));
        addRelic(gm.getRelicByID("DeadBranch_ID"));
        addRelic(gm.getRelicByID("UnceasingTop_ID"));
        addRelic(gm.getRelicByID("HornCleat_ID"));
        addRelic(gm.getRelicByID("MedKit_ID"));
        addRelic(gm.getRelicByID("CharonsAshes_ID"));
        addRelic(gm.getRelicByID("Lantern_ID"));
        addRelic(gm.getRelicByID("Anchor_ID"));
        addRelic(gm.getRelicByID("Akabeko_ID"));
        addRelic(gm.getRelicByID("Pantograph_ID"));
        makeDeck({
            "Clumsy3","Strike","Strike","Defend","Defend","Defend","Defend","Bash+","Bludgeon+",
            "TrueGrit+","PommelStrike+","ShrugItOff","Rampage","Dropkick+","Impervious+","Reaper+",
            "Thunderclap+","Whirlwind+","Clothesline+","FeelNoPain+","Feed+","Barricade+","Disarm+",
            "BurningPact","Offering","Sentinel","Havoc+","Combust","DarkEmbrace","Inflame","Evolve",
            "Metallicize","DarkEmbrace","FireBreathing","Corruption"
            });
        if (random_int(0, 1)) addPotion(gm.retrieveXRandomPotions(1)[0]);
        break;
    case 12:
        addRelic(gm.getRelicByID("BurningBlood_ID"));
        addRelic(gm.getRelicByID("TheBoot_ID"));
        addRelic(gm.getRelicByID("Turnip_ID"));
        addRelic(gm.getRelicByID("OrnamentalFan_ID"));
        addRelic(gm.getRelicByID("LetterOpener_ID"));
        addRelic(gm.getRelicByID("RunicPyramid_ID"));
        addRelic(gm.getRelicByID("Shuriken_ID"));
        addRelic(gm.getRelicByID("MeatOnTheBone_ID"));
        addRelic(gm.getRelicByID("OrangePellets_ID"));
        addRelic(gm.getRelicByID("Toolbox_ID"));
        makeDeck({
            "Dropkick","Uppercut+","Ironwave","Impervious","Armaments+","FeelNoPain+","Anger","Bash",
            "Disarm","Havoc","SwordBoomerang","Impervious","Bloodletting","Flex+","Bloodletting+","Apparition2","Apparition2",
            "Apparition2","ShrugItOff","DarkEmbrace+","Disarm","Pummel+","PommelStrike+","Headbutt+","Apparition2",
            "TrueGrit+","Corruption","Sentinel","Hemokinesis+","DemonForm+","Clothesline","RecklessCharge",
            "SeeingRed","Flex","BurningPact","SpotWeakness","Armaments+","ShrugItOff","WarCry","DarkEmbrace+",
            "Rage","FiendFire","ShrugItOff+","BurningPact"
            });
        if (random_int(0, 1)) addPotion(gm.retrieveXRandomPotions(1)[0]);
        break;
    case 13:
        addRelic(gm.getRelicByID("BurningBlood_ID"));
        addRelic(gm.getRelicByID("Akabeko_ID"));
        addRelic(gm.getRelicByID("Vajra_ID"));
        addRelic(gm.getRelicByID("BustedCrown_ID"));
        addRelic(gm.getRelicByID("DarkstonePeriapt_ID"));
        addRelic(gm.getRelicByID("MeatOnTheBone_ID"));
        addRelic(gm.getRelicByID("CharonsAshes_ID"));
        addRelic(gm.getRelicByID("DuVuDoll_ID"));
        addRelic(gm.getRelicByID("BagOfMarbles_ID"));
        addRelic(gm.getRelicByID("PhilosophersStone_ID"));
        addRelic(gm.getRelicByID("HornCleat_ID"));
        addRelic(gm.getRelicByID("Pocketwatch_ID"));
        addRelic(gm.getRelicByID("MummifiedHand_ID"));
        addRelic(gm.getRelicByID("CaptainsWheel_ID"));
        makeDeck({
            "Defend","Defend","Defend","Defend","Bash","Pummel+","Shockwave","Anger","Clumsy3",
            "Pummel","PommelStrike+","BattleTrance","ShrugItOff","Whirlwind+","DarkEmbrace+","Inflame+",
            "LimitBreak+","Offering+","Dropkick+","Disarm+","PommelStrike+","Rage","DualWield","Feed+",
            "ShrugItOff","SeeingRed","Reaper+","Evolve","DarkEmbrace+","WarCry","TrueGrit+","Uppercut+"
            });
        if (random_int(0, 1)) addPotion(gm.retrieveXRandomPotions(1)[0]);
        break;
    }
    if (random_int(0, 1)) addPotion(gm.retrieveXRandomPotions(1)[0]);
}
void all_pc_info::generateRandomDeck(deckGen dG)
{
    for (int i{ 0 }; i < relics.size(); ++i)
    {
        relics[i]->onDropped();
        delete relics[i];
    }
    relics.clear();
    for (int i{ 0 }; i < potions.size(); ++i)
    {
        delete potions[i];
    }
    potions.clear();
    while (!deck.empty()) { deck.pop_back(); }
    potionSlots = 3;
    gold_ = 100;
    player->setHealth(80);
    player->setMaxHealth(80);
    int deckSize{ 0 };
    int upgradedCards{ 0 };
    int relicsNumber{ 0 };
    int potionsNumber{ 0 };
    int colourlessCards{ 0 };
    int healthNumber{ 0 };
    int rareCards{ 0 };
    bool bossRelic{ false };
    bool bossRelic2{ false };
    switch (dG)
    {
    case deckGen::EH1:
        deckSize =          random_int(9, 13);
        upgradedCards =     random_int(0, 2);
        relicsNumber =      random_int(0, 1);
        potionsNumber =     random_int(0, 1);
        colourlessCards =   random_int(0, 1);
        healthNumber =      random_int(60, 80);
        rareCards =         random_int(0, 0);
        break;
    case deckGen::HH1:
        deckSize = random_int(11, 17);
        upgradedCards = random_int(1,2);
        relicsNumber = random_int(1,2);
        potionsNumber = random_int(0,1);
        colourlessCards = random_int(0,1);
        healthNumber = random_int(50, 80);
        rareCards = random_int(0, 1);
        break;
    case deckGen::QS1:
    case deckGen::EL1:
        deckSize = random_int(16,21);
        upgradedCards = random_int(1,4);
        relicsNumber = random_int(3,5);
        potionsNumber = random_int(0, 2);
        colourlessCards = random_int(0,2);
        healthNumber = random_int(50, 80);
        rareCards = random_int(0, 1);
        break;
    case deckGen::BO1:
        deckSize = random_int(15,22);
        upgradedCards = random_int(1, 5);
        relicsNumber = random_int(2,5);
        potionsNumber = random_int(0,3);
        colourlessCards = random_int(0,1);
        healthNumber = random_int(50, 80);
        rareCards = random_int(1, 2);
        break;
    case deckGen::EH2:
        deckSize = random_int(15,22);
        upgradedCards = random_int(3,6);
        relicsNumber = random_int(2,6);
        potionsNumber = random_int(0, 2);
        colourlessCards = random_int(0, 2);
        healthNumber = random_int(60, 80);
        rareCards = random_int(1,3);
        bossRelic = true;
        break;
    case deckGen::HH2:
        deckSize = random_int(18,25);
        upgradedCards = random_int(3,10);
        relicsNumber = random_int(3,8);
        potionsNumber = random_int(0,3);
        colourlessCards = random_int(0,3);
        healthNumber = random_int(50, 80);
        rareCards = random_int(1,4);
        bossRelic = true;
        break;
    case deckGen::QS2:
    case deckGen::EL2:
        if (random_int(0, 1))
        {
            changeDeck(random_int(0, 9));
            return;
        }
        deckSize = random_int(18, 27);
        upgradedCards = random_int(3, 10);
        relicsNumber = random_int(3, 8);
        potionsNumber = random_int(0, 3);
        colourlessCards = random_int(0, 3);
        healthNumber = random_int(50, 80);
        rareCards = random_int(1, 4);
        bossRelic = true;
        break;
    case deckGen::BO2:
        deckSize = random_int(18, 29);
        upgradedCards = random_int(6, 12);
        relicsNumber = random_int(6, 12);
        potionsNumber = random_int(1, 3);
        colourlessCards = random_int(0, 3);
        healthNumber = random_int(50, 80);
        rareCards = random_int(2, 5);
        bossRelic = true;
        break;
    case deckGen::EH3:
        deckSize = random_int(18, 29);
        upgradedCards = random_int(11, 17);
        relicsNumber = random_int(8, 15);
        potionsNumber = random_int(2, 3);
        colourlessCards = random_int(2, 3);
        healthNumber = random_int(60, 80);
        rareCards = random_int(1,3);
        bossRelic = true;
        bossRelic2 = true;
        break;
    case deckGen::HH3:
        deckSize = random_int(25, 35);
        upgradedCards = random_int(11, 17);
        relicsNumber = random_int(8, 15);
        potionsNumber = random_int(2, 3);
        colourlessCards = random_int(2, 3);
        healthNumber = random_int(50, 80);
        rareCards = random_int(3,6);
        bossRelic = true;
        bossRelic2 = true;
        break;
    case deckGen::QS3:
    case deckGen::EL3:
        deckSize = random_int(25, 35);
        upgradedCards = random_int(11, 17);
        relicsNumber = random_int(8, 15);
        potionsNumber = random_int(2, 3);
        colourlessCards = random_int(2, 3);
        healthNumber = random_int(50, 80);
        rareCards = random_int(3, 6);
        bossRelic = true;
        bossRelic2 = true;
        break;
    case deckGen::BO3:
        if (random_int(0, 2)) { changeDeck(random_int(10, 13)); return; }
        deckSize = random_int(25, 35);
        upgradedCards = random_int(11, 17);
        relicsNumber = random_int(8, 15);
        potionsNumber = random_int(2, 3);
        colourlessCards = random_int(2, 3);
        healthNumber = random_int(50, 80);
        rareCards = random_int(3, 6);
        bossRelic = true;
        bossRelic2 = true;
        break;
    case deckGen::EL4:
    case deckGen::BO4:
        if (random_int(0, 2)) { changeDeck(random_int(10, 13)); return; }
        deckSize = random_int(25, 37);
        upgradedCards = random_int(15, 24);
        relicsNumber = random_int(11, 15);
        potionsNumber = random_int(2, 3);
        colourlessCards = random_int(2, 3);
        healthNumber = random_int(50, 80);
        rareCards = random_int(3, 6);
        bossRelic = true;
        bossRelic2 = true;
        break;
    }
    //if(random_int(0,1)) player->setHealth(healthNumber);
    int defends = random_int(2, 4);
    int strikes = random_int(3, 5);
    // 1 in 20 to have 5 bites
    if (bossRelic && random_int(1, 20) == 20)
    {
        for (int i{ 0 }; i < 5; ++i) apci.addCardToDeck(gm.getSpecialCardByID("Bite_ID"));
    }
    else
    {
        for (int i{ 0 }; i < strikes; ++i) apci.addCardToDeck(gm.getCardByID("Strike_ID"));
    }
    for (int i{ 0 }; i < defends; ++i) apci.addCardToDeck(gm.getCardByID("Defend_ID"));
    apci.addCardToDeck(gm.getCardByID("Bash_ID"));
    while (deck.size() < deckSize)
    {
        while (rareCards > 0)
        {
            auto rCs = gm.getRareCards();
            apci.addCardToDeck(gm.getCardByID(rCs[random_int(0,rCs.size()-1)]));
            --rareCards;
        }
        while (colourlessCards > 0)
        {
            apci.addCardToDeck(gm.getXRandomColourlessCards(1)[0]->second->cloneU());
            --colourlessCards;
        }
        if (random_int(0, 1))
        {
            auto UCs = gm.getUncommonCards();
            apci.addCardToDeck(gm.getCardByID(UCs[random_int(0, UCs.size() - 1)]));
        }
        else
        {
            auto CCs = gm.getCommonCards();
            apci.addCardToDeck(gm.getCardByID(CCs[random_int(0, CCs.size() - 1)]));
        }
    }
    // 1 in 20 to have 5 appartions
    if (bossRelic && random_int(1, 20) == 20)
    {
        for (int i{ 0 }; i < 5; ++i) apci.addCardToDeck(gm.getSpecialCardByID("Apparition_ID"));
    }
    vector<int> upgrades(deck.size());
    for (int i{ 0 }; i < upgrades.size(); ++i) upgrades[i] = i;
    while (upgrades.size() > upgradedCards)
    {
        upgrades.erase(upgrades.begin() + random_int(0, upgrades.size() - 1));
    }
    for (int i{ 0 }; i < upgrades.size(); ++i)
    {
        deck[upgrades[i]]->upgradeThis();
    }
    if (bossRelic)
    {
        auto tempRelic = gm.getBossRelic();
        addRelic(tempRelic);
        if (bossRelic2) addRelic(gm.getBossRelic(tempRelic));
    }
    while (relicsNumber > 0)
    {
        int rngrelic = random_int(0, 4);//4
        if (rngrelic >= 2) rngrelic = random_int(0, 4);//4
        switch (rngrelic)
        {
        case 0:
            addRelic(gm.getCommonRelic());
            //addRelic(gm.retrieveCommonRelic());
            break;
        case 1:
            addRelic(gm.getUncommonRelic());
            //addRelic(gm.retrieveUncommonRelic());
            break;
        case 2:
            addRelic(gm.getRareRelic());
            //addRelic(gm.retrieveRareRelic());
            break;
        case 3:
            addRelic(gm.getShopRelic());
            //addRelic(gm.retrieveShopRelic());
            break;
        case 4:
            addRelic(gm.getEventRelic());
            break;
        }
        --relicsNumber;
    }
    while (potionsNumber > 0)
    {
        addPotion(gm.retrieveXRandomPotions(1)[0]);
        --potionsNumber;
    }
    if (random_int(0, 5) == 5) addCardToDeck(gm.getCurse());
    if (random_int(0, 8) == 8)
    {
        while (apci.getPotions().size() < apci.getPotionSlots())
        {
            apci.addPotion(gm.retrieveXRandomPotions(1)[0]);
        }
    }
    for (int i{ 0 }; i < relics.size(); ++i)
    {
        relics[i]->randomizeCounter();
    }
    player->setHealth(player->getHealth() - random_int(1, 20));
}
void all_pc_info::makeDeck(vector<string> ids)
{
    for (int i{ 0 }; i < ids.size(); ++i)
    {
        if (ids[i].back() == '+')
        {
            ids[i].pop_back();
            if (ids[i].back() == '1' || ids[i].back() == '2' || ids[i].back() == '3')
            {
                char database = ids[i].back();
                ids[i].pop_back();
                ids[i].append("_ID");
                switch (database)
                {
                case '1':
                    deck.push_back(gm.getColourlessCardByID(ids[i]));
                    deck.back()->upgradeThis();
                    break;
                case '2':
                    deck.push_back(gm.getSpecialCardByID(ids[i]));
                    deck.back()->upgradeThis();
                    break;
                case '3':
                    deck.push_back(gm.getCurseCardByID(ids[i]));
                    deck.back()->upgradeThis();
                    break;
                }
            }
            else
            {
                ids[i].append("_ID");
                deck.push_back(gm.getCardByID(ids[i]));
                deck.back()->upgradeThis();
            }
                
        }
        else
        {
            if (ids[i].back() == '1' || ids[i].back() == '2' || ids[i].back() == '3')
            {
                char database = ids[i].back();
                ids[i].pop_back();
                ids[i].append("_ID");
                switch (database)
                {
                case '1':
                    deck.push_back(gm.getColourlessCardByID(ids[i]));
                    break;
                case '2':
                    deck.push_back(gm.getSpecialCardByID(ids[i]));
                    break;
                case '3':
                    deck.push_back(gm.getCurseCardByID(ids[i]));
                    break;
                }
            }
            else
            {
                ids[i].append("_ID");
                deck.push_back(gm.getCardByID(ids[i]));
            }

        }
    }
}

void all_pc_info::masterDeckMaker(curriculumStage stage_, int e)
{
    gamestate::randomAIChoice = true;
    switch (stage_)
    {
    case curriculumStage::Stage1:
        gm.StartGame(0);
        apci.makeStageDeck(stage_);
        break;
    case curriculumStage::Stage2:
        gm.StartGame(random_int(0, 1));
        apci.makeStageDeck(stage_);
        break;
    case curriculumStage::Stage3:
        gm.StartGame(random_int(0, 4));
        //apci.makeStageDeck(stage);
        apci.generateRandomDeck(gm.makeDGEnum());
        break;
    case curriculumStage::Stage4:
        if (e % 50 == 0) gm.cycleAct();
        gm.StartGame(3);
        apci.generateRandomDeck(gm.makeDGEnum());
        break;
    case curriculumStage::Stage5:
        if (e % 50 == 0) gm.cycleAct();
        gm.StartGame();
        apci.generateRandomDeck(gm.makeDGEnum());
        //apci.changeDeck(random_int(10,12));
        break;
    case curriculumStage::Stage6:
        //if (e % 25 == 0) gm.cycleAct();
        while (gm.getActNumber() != 4) gm.cycleAct();
        gm.StartGame(3);
        if (gm.getActNumber() >= 3 && (gm.getMapPathEvent() == 'E' || gm.getMapPathEvent() == 'B')) apci.changeDeck(random_int(10, 13));
        else { apci.generateRandomDeck(gm.makeDGEnum()); }
        break;
    }
    gamestate::randomAIChoice = false;
}

vector<int> all_pc_info::deckForStats()
{
    vector<int> res;
    for (int i{ 0 }; i < deck.size(); ++i)
    {
        res.push_back((deck[i]->getCardIDNum() - 1) * 2 + (deck[i]->getUpgraded() ? 1 : 0));
    }
    return res;
}
vector<int> all_pc_info::relicForStats()
{
    vector<int> res;
    for (int i{ 0 }; i < relics.size(); ++i)
    {
        res.push_back(relics[i]->getRelicIDNum());
    }
    return res;
}
vector<int> all_pc_info::potionsForStats()
{
    vector<int> res;
    for (int i{ 0 }; i < potions.size(); ++i)
    {
        res.push_back(potions[i]->getPotionIDNum());
    }
    return res;
}

bool all_pc_info::fineForCurse()
{
    return checkForRelic("Blue Candle") || checkForRelic("Du-Vu Doll") || checkForRelic("Darkstone Periapt") || gamestate::omamoriCheck>0 || node::checkForC('s', gm.getCurrentNode()->nextNodes);
}
bool all_pc_info::checkForCurse()
{
    for (int i{ 0 }; i < deck.size(); ++i)
    {
        if (deck[i]->getType() == Curse && deck[i]->getName()!= "Necronomicurse" && deck[i]->getName() != "Curse Of The Bell") return true;
    }
    return false;
}
int all_pc_info::cursesInDeck()
{
    int res{ 0 };
    for (int i{ 0 }; i < deck.size(); ++i)
    {
        if (deck[i]->getType() == Curse) ++res;
    }
    return res;
}

void all_pc_info::readyDeckForFight()
{
    if (gamestate::sneckoEyeCheck) player->applyConfused();
    if (gamestate::markOfPainCheck) {
        draw_pile.push_front(cardHolder(gm.getStatusCardByID("Wound_ID")->clone()));
        draw_pile.push_front(cardHolder(gm.getStatusCardByID("Wound_ID")->clone()));
    }
    for(int i{0};i<deck.size();++i)
    {
        //cardHolder temp();
        draw_pile.push_front(*deck[i]);
    }
    shuffleDeck(draw_pile,false);
}
void all_pc_info::cardsStartOfFight()
{
    for(int i{0};i<draw_pile.size();++i)
    {
        (*draw_pile[i]).fightStart();
    }
}
void all_pc_info::addRelic(relic* r)
{
    //cout << "Obtained " << r->getName() << endl;
    relics.push_back(r);
    r->onPickUp();
}
void all_pc_info::loseRelicByIndex(int i)
{
    std::cout << "Losing " << relics[i]->getName() << endl;
    relics[i]->onDropped();
    delete relics[i];
    relics.erase(relics.begin() + i);
}
void all_pc_info::removeRelicByName(string s)
{
    for (int i{ 0 }; i < relics.size(); ++i)
    {
        if (relics[i]->getName() == s) 
        {
            relics[i]->onDropped();
            delete relics[i];
            relics.erase(relics.begin() + i); 
            break; 
        }
    }
}
bool all_pc_info::checkForEntropic()
{
    for (int i{ 0 }; i < potions.size(); ++i)
    {
        if (potions[i]->getName() == "Entropic Brew") return true;
    }
    return false;
}
bool all_pc_info::checkForRelic(string s)
{
    for (int i{ 0 }; i < relics.size(); ++i)
    {
        if (relics[i]->getName() == s) return true;
    }
    return false;
}
bool all_pc_info::addPotion(potion* p)
{
    if (potionSlots == potions.size()) { return false; }
    potions.push_back(p);
    p->pickUp();
    return true;
}
void all_pc_info::losePotion(int i)
{
    if (i >= potions.size()) throw runtime_error("index error losing pot");
    delete potions[i];
    potions.erase(potions.begin() + i);
}
void all_pc_info::removeSmokeBomb()
{
    for (int i{ 0 }; i < potions.size(); ++i)
    {
        if (potions[i]->getName() == "Smoke Bomb")
        {
            delete potions[i];
            potions.erase(potions.begin() + i);
            break;
        }
    }
}
void all_pc_info::removeFIAB()
{
    for (int i{ 0 }; i < potions.size(); ++i)
    {
        if (potions[i]->getName() == "Fairy In A Bottle")
        {
            potions.erase(potions.begin() + i);
            break;
        }
    }
}
void all_pc_info::potionMenu(int index)
{
    if (index < 0 || index >= potions.size()) throw runtime_error("potmenu");
    potion* pot = potions[index];
    if (pot == nullptr) return;
    if (ai && pot->canUse())
    {
        potions.erase(potions.begin() + index);
        pot->execute();
        gm.callPotionUsed();
        delete pot;
    }
    else
    {
        string first;
        first = (pot->canUse() ? "1" : "Unavailable");
        std::cout << pot->getName() << ": " << endl;
        std::cout << first << " to use" << ". 2 to discard potion. Or exit." << endl;
        string res;
        if (first == "1") res = request_input({ "1","2","exit" });
        else { res = request_input({ "2","exit","" }); }
        if (res == "1")
        {
            potions.erase(potions.begin() + index);
            pot->execute();
            gm.callPotionUsed();
            delete pot;
        }
        else if (res == "2") { potions.erase(potions.begin() + index); delete pot; }
        else {}
    }
}
void all_pc_info::relicsStartOfFight() 
{
    auto temp = relics;
    sort(temp.begin(), temp.end(), [&](const relic* a, const relic* b) {return a->getTriggerPrio() < b->getTriggerPrio(); });
    for (auto r : temp)
    {
        if(r->getActive()) r->fightStartSetup();
    }
}
void all_pc_info::relicsStartOfTurn() 
{
    auto temp = relics;
    sort(temp.begin(), temp.end(), [&](const relic* a, const relic* b) {return a->getTriggerPrio() < b->getTriggerPrio(); });
    for (auto r : temp)
    {
        if (r->getActive()) r->turnStartSetup();
    }
}

cardDeckCH& all_pc_info::shuffleDeck(cardDeckCH& deck_,bool trig)
{
    if(trig) gm.callShuffledDraw();
    topCardOfDeckKnown = draw_pile.size() == 1;
    return shuffleDeck(deck_,static_cast<int>(deck_.size()-1));
}
cardDeckCH& all_pc_info::shuffleDeck(cardDeckCH& deck_, int i)
{
    if(i==0) return deck_;
    shuffleDeck(deck_,i-1);
    int k = random_int(0,i);
    swap(deck_[k],deck_[i]);
    return deck_;
}
bool all_pc_info::checkCardInHand(const string& s)
{
    for (int i{ 0 }; i < hand.size(); ++i)
    {
        if ((*hand[i]).getName() == s) return true;
    }
    return false;
}
bool all_pc_info::checkCardInHand(card* c)
{
    for (int i{ 0 }; i < hand.size(); ++i)
    {
        if (hand[i].getCardPointer() == c) return true;
    }
    return false;
}
void all_pc_info::sendHandCardToDiscard(cardHolder& ch)
{
    for(auto itr{hand.begin()};itr!=hand.end();++itr)
    {
        if (ch == (*itr)) { discard_pile.push_front(std::move(*itr)); hand.erase(itr); break; }
    }
}
void all_pc_info::sendCardFromToHand(cardDeckCH& a, cardHolder& ch)
{
    for(int i{0};i<a.size();++i)
    {
        if(ch==(a[i]))
        {
            if (hand.size() == 10) { discard_pile.push_back(std::move(a[i])); }
            else { hand.push_back(std::move(a[i])); }
            a.erase(a.begin()+i);
            break;
        }
    }
}
void all_pc_info::sendCardFromDrawToHand(card* c)
{
    for(auto itr{draw_pile.begin()};itr!=draw_pile.end();++itr)
    {
        if(c==(*itr).getCardPointer()) 
        {
            if(hand.size()==10) {discard_pile.push_back(std::move(*itr));}
            else{hand.push_back(std::move(*itr));}
            draw_pile.erase(itr);
            break;
        }
    }
}
void all_pc_info::addCardToBackOfStack(cardHolder&& ch)
{
    the_stack.push_back(ch);
}
bool all_pc_info::altToBasicCurseBottled()
{
    for (int i{ 0 }; i < deck.size(); ++i)
    {
        if (deck[i]->getCardIDNum() == 1 || deck[i]->getCardIDNum() == 2 || deck[i]->getCardIDNum() == 3 ||
            deck[i]->getType() == Curse || deck[i]->getBottled()) continue;
        return true;
    }
    return false;
}
bool all_pc_info::altToBasicCurseBottled(cardType ct)
{
    for (int i{ 0 }; i < deck.size(); ++i)
    {
        if (deck[i]->getCardIDNum() == 1 || deck[i]->getCardIDNum() == 2 || deck[i]->getCardIDNum() == 3 ||
            deck[i]->getType() == Curse || deck[i]->getBottled() || deck[i]->getType() != ct) continue;
        return true;
    }
    return false;
}
void all_pc_info::addPotionInputs(vector<string>& validInps)
{
    for (int i{ 1 }; i <= potions.size(); ++i)
    {
        validInps.push_back("p" + to_string(i));
    }
}
int all_pc_info::numberInHand(vector<cardType> cts)
{
    int valid{ false };
    for (int i{ 0 }; i < hand.size(); ++i)
    {
        if (find(cts.begin(), cts.end(), (*hand[i]).getType()) != cts.end()) ++valid;
    }
    return valid;
}
int all_pc_info::numberInDraw(vector<cardType> cts)
{
    int valid{ false };
    for (int i{ 0 }; i < draw_pile.size(); ++i)
    {
        if (find(cts.begin(), cts.end(), (*draw_pile[i]).getType()) != cts.end()) ++valid;
    }
    return valid;
}
combatChoices all_pc_info::needForSim(combatChoices choice)
{
    if (
            (choice == combatChoices::EnemyTarget && gm.getAllEnemies().size() > 1) ||
            (choice == combatChoices::Exhaust && exhausted.size() > 1) ||
            (choice == combatChoices::HandAR && numberInHand({ Attack,Power,Skill }) > 1) ||
            (choice == combatChoices::HandDW && numberInHand({ Attack,Power }) > 1) ||
            (choice == combatChoices::HandWC && ((player->getNoDraw() && hand.size() > 1) || (hand.size() > 0 && draw_pile.size() + discard_pile.size() > 0))) ||
            (choice == combatChoices::HandTA && ((player->getNoDraw() && hand.size() > 1) || (hand.size() + draw_pile.size() + discard_pile.size() > 1))) ||
            (choice == combatChoices::Hand && hand.size() > 1) ||
            (choice == combatChoices::HandMulti3 && hand.size() > 0) ||
            (choice == combatChoices::HandMulti5 && hand.size() > 0) ||
            (choice == combatChoices::HandMultiAll && hand.size() > 0) ||
            (choice == combatChoices::Headbutt && (gm.getAllEnemies().size() > 1 || discard_pile.size() > 1)) ||
            (choice == combatChoices::Choice3NoSkip) ||
            (choice == combatChoices::Choice3Skip) ||
            (choice == combatChoices::DrawAtt && numberInDraw({ Attack }) > 0) ||
            (choice == combatChoices::DrawSkl && numberInDraw({ Skill }) > 0) ||
            (choice == combatChoices::Havoc)
        ) { return choice; }

    else { return combatChoices::None; }
}
combatChoices all_pc_info::needForSim(int action)
{
    if (action == 262) return combatChoices::None;
    if (action > 262) return combatChoices::Potion;
    vector<int> optionsByIndex;
    for (int i{ 0 }; i < hand.size(); ++i)
    {
        if ((((*hand[i]).getCardIDNum() - 1) * 2 + ((*hand[i]).getUpgraded() ? 1 : 0)) == action) optionsByIndex.push_back(i);
    }
    if (optionsByIndex.size() == 0) { printCardDeck(hand); throw runtime_error("..."); }
    if (optionsByIndex.size() == 1) action = optionsByIndex[0];
    else { sort(optionsByIndex.begin(), optionsByIndex.end(), [&](int index1, int index2) {return (*hand[index1]).getEnergy() < (*hand[index2]).getEnergy(); }); action = optionsByIndex[0]; }
    int i = action;
    combatChoices choice = (*hand[i]).getCombatChoices();
    if  ( 
            (*hand[i]).getEnergy() <= gm.getPlayerEnergy() && (*hand[i]).castable() &&
            (
                (choice == combatChoices::EnemyTarget && gm.getAllEnemies().size() > 1) ||
                (choice == combatChoices::Exhaust && exhausted.size() > 1) ||
                (choice == combatChoices::HandAR && numberInHand({ Attack,Power,Skill }) > 2) ||
                (choice == combatChoices::HandDW && numberInHand({Attack,Power}) > 1) ||
                (choice == combatChoices::HandWC && ((player->getNoDraw() && hand.size() > 2) || (!player->getNoDraw() && hand.size() > 1 && draw_pile.size() + discard_pile.size() > 0))) ||
                (choice == combatChoices::HandTA && ((player->getNoDraw() && hand.size() > 2) || (!player->getNoDraw() && hand.size() + draw_pile.size() + discard_pile.size() > 2))) ||
                (choice == combatChoices::Hand && hand.size() > 2) ||
                (choice == combatChoices::HandMulti3 && hand.size() > 1) ||
                (choice == combatChoices::HandMulti5 && hand.size() > 1) ||
                (choice == combatChoices::HandMultiAll && hand.size() > 1) ||
                (choice == combatChoices::Headbutt && (gm.getAllEnemies().size() > 1 || discard_pile.size() > 1)) ||
                (choice == combatChoices::Choice3NoSkip) ||
                (choice == combatChoices::Choice3Skip) ||
                (choice == combatChoices::DrawAtt && numberInDraw({Attack}) > 0) ||
                (choice == combatChoices::DrawSkl && numberInDraw({Skill}) > 0) ||
                (choice == combatChoices::Havoc)
            )
        ) {return (*hand[i]).getCombatChoices();}

    else { return combatChoices::None; }
}
bool all_pc_info::turnLoop()
{
    gm.getXCost(true);
    vector<string> validInputs = { "end","draw","dis","ex","deck","add","sim","k"};
    bool already = gamestate::denyCardPlay;
    if (gm.cardCount >= 3 && checkCardInHand("Normality") ) { gamestate::denyCardPlay = true; }
    for (int i{ 0 }; i < hand.size(); ++i)
    {
        if ((*hand[i]).getEnergy() <= gm.getPlayerEnergy() && (*hand[i]).castable()) validInputs.push_back(to_string((i + 1) % 10));
    }
    if (!already) gamestate::denyCardPlay = false;
    addPotionInputs(validInputs);
    showAIDecisions(gm.getPyFunc());
    string result = request_input(validInputs);
    if (result == "end") return false;
    if (result == "draw") { printCardDeck(draw_pile); return true; }
    if (result == "dis") { printCardDeck(discard_pile); return true; }
    if (result == "ex") { printCardDeck(exhausted); return true; }
    if (result == "deck") { printDeck(); return true; }
    if (result == "add") { gm.addCheatCard(); return true; }
    if (result == "sim") { sim = true; sv.saveGame(); return true; }
    if (result == "k") { gm.endFight(); return false; }
    if (result[0] == 'p') 
    {
        apci.potionMenu(static_cast<int>(result[1]) - 49);
    }
    else
    {
        int res = stoi(result) - 1;
        if (res == -1) res = 9;
        // play card
        cardHolder temp(std::move(hand[res]));
        hand.erase(hand.begin() + res);
        gm.decreasePlayerEnergy((*temp).getEnergy());
        playCard(temp);
        if (!(*temp).getExhaust() && (*temp).getType() != cardType::Power || ((*temp).getExhaust() && gamestate::strangeSpoonCheck && gamestate::strangeSpoonLE))
        {
            discard_pile.push_front(std::move(temp));
        }
    }
    while (!the_stack.empty())
    {
        havoc::playCardByHavoc(std::move(the_stack.front()));
        the_stack.pop_front();
    }
    visuals::editAllHand(hand);
    gm.getXCost(true);
    if (gamestate::clockEnd) { gamestate::clockEnd = false; gamestate::denyCardPlay = false; return false; }
    return true;
}
void all_pc_info::turnLoop(int action)
{   
    gm.getXCost(true);
    if (action == 262) // 4
    {
        gamestate::randomAIChoice = true;
        player->eotCleanup();
        for_each(gm.getAllEnemies().begin(), gm.getAllEnemies().end(), [](unique_ptr<nonpc>& e) {e->eotCleanup(); });
        endTurn();
        try {
            gm.startEnemyTurn();
        }
        catch (PlayerDiedError) { return; }
        if (gm.checkEnemyDeath()) { gm.handleEnemyDeath(); if (gm.checkAllEnemyDeath()) return; }
        startTurn();
        gamestate::randomAIChoice = false;
        return;
    }
    else if (action > 262)
    {
        for (int i{ 0 }; i < apci.getPotions().size(); ++i)
        {
            if (apci.getPotion(i)->getPotionIDNum() + 262 == action)
            {
                apci.potionMenu(i);
                break;
            }
        }
    }
    else
    {
        vector<int> optionsByIndex;
        for (int i{ 0 }; i < hand.size(); ++i)
        {
            if ((((*hand[i]).getCardIDNum() - 1) * 2 + ((*hand[i]).getUpgraded() ? 1 : 0)) == action) optionsByIndex.push_back(i);
        }
        if (optionsByIndex.size() == 0) { printCardDeck(hand); throw runtime_error("..."); }
        if (optionsByIndex.size() == 1) action = optionsByIndex[0];
        else { sort(optionsByIndex.begin(), optionsByIndex.end(), [&](int index1, int index2) {return (*hand[index1]).getEnergy() < (*hand[index2]).getEnergy(); }); action = optionsByIndex[0]; }

        cardHolder temp(std::move(hand[action]));
        hand.erase(hand.begin() + action);
        gm.decreasePlayerEnergy((*temp).getEnergy());
        playCard(temp);
        if (!(*temp).getExhaust() && (*temp).getType() != cardType::Power || ((*temp).getExhaust() && gamestate::strangeSpoonCheck && gamestate::strangeSpoonLE))
        {
            discard_pile.push_front(std::move(temp));
        }
    }
    bool already = gamestate::randomAIChoice;
    while (!the_stack.empty())
    {
        // first time round simming, make all buffers here, subsequent passes skip this and use simbuffer, or rng
        if (gamestate::aiSimActionBuffer.empty() && !gamestate::randomAIChoice)
        {
            for (int j{ 0 }; j < the_stack.size(); ++j)
            {
                combatChoices actionChoices = apci.needForSim((*the_stack[j]).getCombatChoices());
                if (actionChoices != combatChoices::EnemyTarget && actionChoices != combatChoices::None)
                {
                    if (actionChoices == combatChoices::Headbutt)
                    {
                        if (discard_pile.size() > 1)
                        {
                            for (int i{ 0 }; i < apci.getDiscardSize(); ++i)
                            {
                                gamestate::aiSimActionBuffer.push_back(i + 1);
                                gamestate::orderedChoiceTree.push_back({ i + 1 });
                            }
                        }
                    }
                    else { gm.actionChoicesForSim((*the_stack[j]).getCombatChoices(), -1); }
                }
                if (!gamestate::aiSimActionBuffer.empty()) break;
            }
            
        }
        havoc::playCardByHavoc(std::move(the_stack.front()));
        the_stack.pop_front();
        if (the_stack.size() > 0) gamestate::randomAIChoice = true;
    }
    gamestate::randomAIChoice = already;
    gm.getXCost(true);
    if (gamestate::clockEnd) 
    {
        gamestate::clockEnd = false;
        gamestate::denyCardPlay = false;
        gamestate::randomAIChoice = true;
        player->eotCleanup();
        for_each(gm.getAllEnemies().begin(), gm.getAllEnemies().end(), [](unique_ptr<nonpc>& e) {e->eotCleanup(); });
        endTurn();
        try {
            gm.startEnemyTurn();
        }
        catch (PlayerDiedError) { return; }
        if (gm.checkEnemyDeath()) { gm.handleEnemyDeath(); if (gm.checkAllEnemyDeath()) return; }
        startTurn();
        gamestate::randomAIChoice = false;
    }

}
void all_pc_info::startTurn()
{
    if (gm.turnCount == 0) readyDeckForFight();

    gm.reAddDormant();
    player->sotCleanup();
    for_each(gm.getAllEnemies().begin(), gm.getAllEnemies().end(), [](unique_ptr<nonpc>& e) {e->sotCleanup(); });
    if (gm.turnCount != 0) gm.callEnemyTurnEnd();
    if (gm.turnCount == 0) cardsStartOfFight();
    drawX(player->getDrawSOT() - (player->getDrawReduction() > 0 ? 1 : 0));
    gm.setPlayerEnergySOT(player->getEnergy());
    for_each(gm.getAllEnemies().begin(), gm.getAllEnemies().end(), [](unique_ptr<nonpc>& e) {e->chooseAndDeclareIntent(); });
    if (gm.turnCount == 0)
    {
        for_each(gm.getAllEnemies().begin(), gm.getAllEnemies().end(), [](unique_ptr<nonpc>& e) {e->startOfFight(); });
        player->startOfFight();
        relicsStartOfFight();
    }
    relicsStartOfTurn();
    if (gm.checkEnemyDeath()) gm.handleEnemyDeath();
}
void all_pc_info::startTurnV()
{
    startTurn();
    initVisuals();
    while (turnLoop())
    {
        if (sim && simCount > 0) { sv.loadGame(); --simCount; std::cout << "Sim Begins (" << simCount << ")" << endl; sv.saveGame();  continue; }
        if (sim && simCount == 0) { sim = false; simCount = 5; std::cout << "Sim Over" << endl; sv.clearAll(); }
        if (gm.checkEnemyDeath()) { gm.handleEnemyDeath(); if (gm.checkAllEnemyDeath()) return; }
        initVisuals();
    }
    player->eotCleanup();
    for_each(gm.getAllEnemies().begin(), gm.getAllEnemies().end(), [](unique_ptr<nonpc>& e) {e->eotCleanup(); });
    endTurn();
}
void all_pc_info::initVisuals()
{
    int newSize = static_cast<int>(gm.getAllEnemies().size());
    switch (newSize)
    {
    case 1:
        visuals::setvisInUse(aiu::one);
        initVisualsOne();
        break;
    case 2:
        visuals::setvisInUse(aiu::two);
        initVisualsTwo();
        break;
    case 3:
        visuals::setvisInUse(aiu::three);
        initVisualsThree();
        break;
    case 4:
        visuals::setvisInUse(aiu::four);
        initVisualsFour();
        break;
    case 5:
        visuals::setvisInUse(aiu::five);
        initVisualsFive();
        break;
    }
}
void all_pc_info::drawX(int x)
{
    if(player->getNoDraw()) return;
    while(--x>=0)
    {
        if(hand.size()==10) break;
        if(draw_pile.size()==0) discardToDraw();
        if(draw_pile.size()==0) break;
        hand.push_back(std::move(draw_pile[0]));
        draw_pile.pop_front();
        gm.callCardDrawn(hand.back());
    }
    topCardOfDeckKnown = draw_pile.size() == 1;
    if(!ai) visuals::editAllHand(hand);
}
void all_pc_info::discardToDraw()
{
    if(discard_pile.size()==0) return;
    while(discard_pile.size()>0)
    {
        draw_pile.push_front(std::move(discard_pile[0]));
        discard_pile.pop_front();
    }
    shuffleDeck(draw_pile,true);
    if (!ai) std::cout<<"Shuffled discard to draw."<<endl;
}
void all_pc_info::reshuffleDrawAndHand()
{
    int handSize = static_cast<int>(hand.size());
    if (hand.size() == 0) return;
    while (hand.size() > 0)
    {
        //  keep bottled
        draw_pile.push_front(std::move(hand[0]));
        hand.pop_front();
    }
    shuffleDeck(draw_pile, false);
    drawX(handSize);
}
void all_pc_info::putDrawPileCardAtFront(card* c)
{
    for (int i{ 0 }; i < draw_pile.size(); ++i)
    {
        if (draw_pile[i].getCardPointer() == c)
        {
            cardHolder temp(std::move(draw_pile[i]));
            draw_pile.erase(draw_pile.begin() + i);
            draw_pile.push_front(std::move(temp));
        }
    }
}
void all_pc_info::addCardToDrawFront(cardHolder ch)
{
    if (!ai) std::cout<<(*ch).getName() <<" put on top of draw pile."<<endl;
    draw_pile.push_front(ch);
    topCardOfDeckKnown = draw_pile.size() > 0;
}
void all_pc_info::addCardToDrawBack(cardHolder ch)
{
    if (!ai) std::cout << (*ch).getName() << " put on bottom of draw pile." << endl;
    draw_pile.push_back(ch);
}
void all_pc_info::addCardToDrawRandom(cardHolder ch)
{
    if (!ai) std::cout<<(*ch).getName() <<" shuffled into draw pile."<<endl;
    if (draw_pile.empty())
    {
        draw_pile.push_back(ch);
        topCardOfDeckKnown = true;
    }
    else 
    {
        draw_pile.insert(draw_pile.begin() + random_int(0, draw_pile.size() - 1), ch); 
        topCardOfDeckKnown = false;
    }
}
void all_pc_info::addCardToDiscard(cardHolder ch)
{
    if (!ai) std::cout<<(*ch).getName() <<" put in discard pile."<<endl;
    discard_pile.push_back(ch);
}
void all_pc_info::addCardToExhaust(cardHolder ch)
{
    if (!ai) std::cout<<(*ch).getName() <<" exhausted."<<endl;
    exhausted.push_back(ch);
}
void all_pc_info::addCardToHand(cardHolder ch)
{
    if (hand.size() == 10) { discard_pile.push_back(ch); if (!ai) std::cout << (*ch).getName() << " discarded." << endl; }
    else { hand.push_back(ch); if (!ai) std::cout << (*ch).getName() << " added to hand." << endl; }
}
void all_pc_info::addCardToDeck(card* card_)
{
    if (gamestate::omamoriCheck > 0 && card_->getType() == Curse)
    {
        --gamestate::omamoriCheck;
        if (!ai) std::cout << "Omamori charge used." << endl; return;
    }
    deck.push_back(unique_ptr<card>(card_->clone()));
    if(!ai) std::cout<<card_->getName() <<" added to deck."<<endl;
    cardHolder tempForTrigger(card_->clone());
    gm.callCardAdded(tempForTrigger);
}
void all_pc_info::addCardToDeck(std::unique_ptr<card> dtbCard)
{
    if (gamestate::omamoriCheck > 0 && dtbCard->getType() == Curse) 
    {
        --gamestate::omamoriCheck; 
        if (!ai) std::cout << "Omamori charge used." << endl; return;
    }
    deck.push_back(dtbCard->cloneU());
    if (!ai) std::cout<<dtbCard->getName() <<" added to deck."<<endl;
    cardHolder tempForTrigger(dtbCard->clone());
    gm.callCardAdded(tempForTrigger);
}
void all_pc_info::addCardToDeck(std::map<std::string, std::unique_ptr<card>>::iterator& dtbCard, bool upgrade_)
{
    if (gamestate::omamoriCheck > 0 && dtbCard->second->getType() == Curse)
    {
        --gamestate::omamoriCheck;
        if(!ai) std::cout << "Omamori charge used." << endl; return;
    }
    deck.push_back(dtbCard->second->cloneU());
    if (upgrade_) deck.back()->upgradeThis();
    if (!ai) std::cout<<dtbCard->second->getName() <<" added to deck."<<endl;
    cardHolder tempForTrigger(dtbCard->second->clone());
    gm.callCardAdded(tempForTrigger);
}
void all_pc_info::removeCard(const deque<unique_ptr<card>>::iterator& deckCard, bool print)
{
    if(print && !ai) std::cout<< (*deckCard)->getName() << " was removed."<< endl;
    if ((*deckCard)->getName() == "Parasite" && print)
    {
        apci.getPlayer().loseMaxHP(3);
    }
    deck.erase(deckCard);
}
void all_pc_info::gainGold(int i) 
{
    if (gamestate::ectoplasmCheck) return;
    gold_ += i;
    if (!ai) std::cout << "Gain " << i << " Gold." << endl;
    gm.callGoldGained();
}
void all_pc_info::loseGold(int i) 
{
    gold_ = clamp(gold_ - i, 0, 999);
    if (!ai) std::cout << "Lost " << i << " Gold. " << gold_ << " remaining." << endl;
}
void all_pc_info::endTurn()
{
    int handEnd = static_cast<int>(hand.size());
    for(int i{0};i<handEnd;++i)
    {
        if((*hand[i]).endStepDiscardPrio())
        {
            sendHandCardToDiscard(hand[i]);
            --i;
            --handEnd;
        }
    }
    for(int i{0};i<handEnd;++i)
    {
        if((*hand[i]).getEthereal())
        {
            effects::exhaustHandCard(hand[i]);
            --i;
            --handEnd;
        }
    }
    //handEnd = hand.size();
    for(int i{0};i<handEnd;++i)
    {
        if ((*hand[i]).endStepDiscard())
        {
            sendHandCardToDiscard(hand[i]);
            --i;
            --handEnd;
        }

    }
    if (!gamestate::runicPyramidCheck)
    {
        while (handEnd > 0)
        {
            discard_pile.push_front(hand[0]);
            hand.pop_front();
            --handEnd;
        }
    }
    
    ++gm.turnCount;
}
void all_pc_info::spendGold(int i)
{
    if (i > gold_) throw runtime_error("not enough gold");
    gold_ -= i;
    gm.callGoldSpent();
}
void all_pc_info::makeCostZero(cardHolder& ch)
{
    if ((*ch).getEnergy() == -1) return;
    auto t1 = new zeroCostTrigger<card>(ch.getCardPointer(), gm.getPlayerTurnEnd());
    auto t2 = new zeroCostTrigger<card>(ch.getCardPointer(), gm.getCardPlayed());
    (*ch).observeEvent<zeroCostTrigger<card>>(gm.getPlayerTurnEnd(), t1);
    (*ch).observeEvent<zeroCostTrigger<card>>(gm.getCardPlayed(), t2);
    (*ch).setZeroUntilPlayed(true);
}
void all_pc_info::makeCostZero(card* c)
{
    if (c->getEnergy() == -1) return;
    auto t1 = new zeroCostTrigger<card>(c, gm.getPlayerTurnEnd());
    auto t2 = new zeroCostTrigger<card>(c, gm.getCardPlayed());
    c->observeEvent<zeroCostTrigger<card>>(gm.getPlayerTurnEnd(), t1);
    c->observeEvent<zeroCostTrigger<card>>(gm.getCardPlayed(), t2);
    c->setZeroUntilPlayed(true);
}
void all_pc_info::makeCostOne(cardHolder& ch)
{
    if ((*ch).getEnergy() <= 1) return;
    auto t1 = new oneCostTrigger<card>(ch.getCardPointer(), gm.getPlayerTurnEnd());
    auto t2 = new oneCostTrigger<card>(ch.getCardPointer(), gm.getCardPlayed());
    (*ch).observeEvent<oneCostTrigger<card>>(gm.getPlayerTurnEnd(), t1);
    (*ch).observeEvent<oneCostTrigger<card>>(gm.getCardPlayed(), t2);
    (*ch).setOneUntilPlayed(true);
}
void all_pc_info::makeCostOne(card* c)
{
    if (c->getEnergy() <= 1) return;
    auto t1 = new oneCostTrigger<card>(c, gm.getPlayerTurnEnd());
    auto t2 = new oneCostTrigger<card>(c, gm.getCardPlayed());
    c->observeEvent<oneCostTrigger<card>>(gm.getPlayerTurnEnd(), t1);
    c->observeEvent<oneCostTrigger<card>>(gm.getCardPlayed(), t2);
    c->setOneUntilPlayed(true);
}
void all_pc_info::endFight()
{
    while (!draw_pile.empty()) draw_pile.pop_back();
    while (!hand.empty()) hand.pop_back();
    while (!discard_pile.empty()) discard_pile.pop_back();
    while (!exhausted.empty()) exhausted.pop_back();
    while (!the_stack.empty()) the_stack.pop_back();
    player->endFight();
}
void all_pc_info::aiPotionChoice(vector<potion*> options, int maxHeld)
{
    vector<int> res;
    int currentRank{ 5 };
    while (true)
    {
        for (int i{ 0 }; i < options.size(); ++i)
        {
            if (options[i]->getRank() == currentRank)
            {
                res.push_back(i);
            }
        }
        if (res.size()  >= maxHeld || res.size() == options.size()) break;
        --currentRank;
    }
    while (res.size() > maxHeld)
    {
        int currentWorstIndex{ 0 };
        for (int i{ 0 }; i < res.size(); ++i)
        {
            if (options[res[currentWorstIndex]]->getRank() < options[res[i]]->getRank()) continue;
            if (options[res[currentWorstIndex]]->getSituational() < options[res[i]]->getSituational()) continue;
            if (options[res[currentWorstIndex]]->getReliable() < options[res[i]]->getReliable()) continue;
            currentWorstIndex = i;
        }
        res.erase(res.begin() + currentWorstIndex);

    }
    potions.clear();
    for (int i{ 0 }; i < options.size(); ++i)
    {
        if (find(res.begin(), res.end(), i) == res.end()) delete options[i];
        else
        {
            potions.push_back(options[i]);
            options[i]->pickUp();
        }
    }
}
int all_pc_info::getGiryaCounter() const 
{
    for (int i{ 0 }; i < relics.size(); ++i)
    {
        if (relics[i]->getName() == "Girya")
        {
            return relics[i]->getCounter();
        }
    }
    return 0;
}
void all_pc_info::incrGiryaCounter()  
{
    for (int i{ 0 }; i < relics.size(); ++i)
    {
        if (relics[i]->getName() == "Girya")
        {
            static_cast<girya*>(relics[i])->incrStr();
            break;
        }
    }
}
bool all_pc_info::isEnergyRelicGood()
{
    if (gm.getPlayerEnergy() > 3) return false;
    float total_energy{ 0 }, zero_costs{ 0 };
    for (int i{ 0 }; i < deck.size(); ++i)
    {
        if (deck[i]->getEnergy() == 0) ++zero_costs;
        if (deck[i]->getCardIDNum() == 25 || deck[i]->getCardIDNum() == 52) total_energy -= 1.5f;
        else { total_energy += deck[i]->getEnergy(); }
    }
    if (zero_costs > 3) return false;
    float av_energy = static_cast<float>(total_energy) / static_cast<float>(deck.size());
    if (checkForRelic("Lantern")) av_energy -= 0.2f;
    if (checkForRelic("Happy Flower")) av_energy -= 0.2f;
    return av_energy > 1.4;
}
bool all_pc_info::isSneckoGood()
{
    if (checkForRelic("Runic Pyramid")) return false;
    float total_energy{ 0 }, zero_costs{ 0 };
    for (int i{ 0 }; i < deck.size(); ++i)
    {
        if (deck[i]->getEnergy() == 0) ++zero_costs;
        if (deck[i]->getCardIDNum() == 25 || deck[i]->getCardIDNum() == 52) total_energy -= 1.5f;
        else { total_energy += deck[i]->getEnergy(); }
    }
    if (zero_costs > 1) return false;
    float av_energy = static_cast<float>(total_energy) / static_cast<float>(deck.size());
    if (checkForRelic("Lantern")) av_energy -= 0.2f;
    if (checkForRelic("Happy Flower")) av_energy -= 0.2f;
    return av_energy > 1.4;
}
bool all_pc_info::couldHavePowered()
{
    for (int i{ 0 }; i < hand.size(); ++i)
    {
        // if could have played a power (not corruption) and chose to end turn, return true
        if ((*hand[i]).getType() == Power && (*hand[i]).castable() && (*hand[i]).getEnergy() <= gm.getPlayerEnergy() 
            && (*hand[i]).getCardIDNum() != 63 && (*hand[i]).getCardIDNum() != 60) return true;
    }
    return false;
}
bool all_pc_info::couldHavePlayedAnother()
{
    for (int i{ 0 }; i < hand.size(); ++i)
    {
        // if could have played a power (not corruption) and chose to end turn, return true
        if ((*hand[i]).castable() && (*hand[i]).getEnergy() <= gm.getPlayerEnergy() 
            && (*hand[i]).getCardIDNum() != 63 && (*hand[i]).getCardIDNum() != 60) return true;
    }
    return false;
}
bool all_pc_info::fineToBlockMore(int _action)
{
    // incoming damage
    if (gm.getTotalIncomingDamage() > player->getBlock()) return true;
    if (_action == 6 || _action == 7 || _action == 24 || _action == 25 || _action == 30 || _action == 31 || _action == 74 || _action == 75) return true;
    if (player->getRage() || player->getJuggernautBuff() || player->getFeelNoPainBuff() || player->getBarricadeBuff() || checkForRelic("Calipers") || checkForRelic("Ornamental Fan")) return true;
    bool bodyslamCheck{ false };
    for (int i{ 0 }; i < hand.size(); ++i)
    {
        if ((*hand[i]).getCardIDNum() == 6) { bodyslamCheck = true; break; }
    }
    if (bodyslamCheck) return true;
    for (int i{ 0 }; i < draw_pile.size(); ++i)
    {
        if ((*draw_pile[i]).getCardIDNum() == 6) { bodyslamCheck = true; break; }
    }
    if (bodyslamCheck) return true;

    return false;
}
int all_pc_info::validRemoveCount()
{
    int res{ 0 };
    for (int i{ 0 }; i < deck.size(); ++i)
    {
        if (deck[i]->getBottled() || deck[i]->getCardIDNum() == 80 || deck[i]->getCardIDNum() == 82) continue;
        ++res;
    }
    return res;
}
int all_pc_info::validTransformCount()
{
    int res{ 0 };
    for (int i{ 0 }; i < deck.size(); ++i)
    {
        if (deck[i]->getBottled() || deck[i]->getType() == Curse) continue;
        ++res;
    }
    return res;
}
int all_pc_info::validDuplicateCount()
{
    int res{ 0 };
    for (int i{ 0 }; i < deck.size(); ++i)
    {
        if (deck[i]->getType() == Curse) continue;
        ++res;
    }
    return res;
}
int all_pc_info::validUpgradeCount()
{
    int res{ 0 };
    for (int i{ 0 }; i < deck.size(); ++i)
    {
        if (deck[i]->getType() == Curse || deck[i]->getUpgraded()) continue;
        ++res;
    }
    return res;
}

int all_pc_info::compareBossRelics(vector<relic*>& opts, int act_)
{
    bool nrgGood = isEnergyRelicGood();
    vector<int> rankings(opts.size());
    for (int i{ 0 }; i< opts.size(); ++i)
    {
        auto it = dynamic_cast<bossRelic*>(opts[i]);
        rankings[i] = act_ == 1 ? it->getAct1Rank() : it->getAct2Rank();
        if (it->getEnergyRelic() && nrgGood) rankings[i] += 2;
        if (it->getUnique()) rankings[i] *= it->uniqueCheck();
    }
    pair<int,int> best_index{ 0,0 };

    for (int i{ 0 }; i < rankings.size(); ++i)
    {
        if (rankings[i] > best_index.first) best_index = { rankings[i], i };
    }
    return best_index.second;
}

vector<int> all_pc_info::twoRandomSkillIDs()
{
    vector<int> res;
    for (int i{ 0 }; i < deck.size(); ++i)
    {
        if (find(res.begin(), res.end(), deck[i]->getCardIDNum()) != res.end()) continue;
        if (deck[i]->getType() == Skill && !deck[i]->getUpgraded()) res.push_back(deck[i]->getCardIDNum());
    }
    while (res.size() > 2)
    {
        res.erase(res.begin() + random_int(0, res.size() - 1));
    }
    return res;
}
vector<int> all_pc_info::twoRandomAttackIDs()
{
    vector<int> res;
    for (int i{ 0 }; i < deck.size(); ++i)
    {
        if (find(res.begin(), res.end(), deck[i]->getCardIDNum()) != res.end()) continue;
        if (deck[i]->getType() == Attack && !deck[i]->getUpgraded()) res.push_back(deck[i]->getCardIDNum());
    }
    while (res.size() > 2)
    {
        res.erase(res.begin() + random_int(0, res.size() - 1));
    }
    return res;
}
int all_pc_info::randomUpgradeID()
{
    vector<int> res;
    for (int i{ 0 }; i < deck.size(); ++i)
    {
        if (!deck[i]->getUpgraded() && deck[i]->getType() != Curse) res.push_back(i);
    }
    if (res.size() == 0) return 0;
    return res[random_int(0,res.size()-1)];
}

vector<int> all_pc_info::bestPotionsInFight(bool onlyUseOne)
{
    vector<int> potIDs(potions.size());
    for (int i{ 0 }; i < potions.size(); ++i)
    {
        potIDs[i] = potions[i]->getPotionIDNum();
    }
    size_t s = static_cast<size_t>(pow(2, static_cast<double>(potions.size())));
    vector<vector<int>> perms(s, potIDs);

    int index{ 0 };
    for (int i{ 0 }; i < 2; ++i)
    {
        for (int j{ 0 }; j < 2; ++j)
        {
            for (int k{ 0 }; k < 2; ++k)
            {
                for (int l{ 0 }; l < 2; ++l)
                {
                    for (int m{ 0 }; m < 2; ++m)
                    {
                        if (i == 0 && potions.size() >= 1) perms[index][0] = 0;
                        if (j == 0 && potions.size() >= 2) perms[index][1] = 0;
                        if (k == 0 && potions.size() >= 3) perms[index][2] = 0;
                        if (l == 0 && potions.size() >= 4) perms[index][3] = 0;
                        if (m == 0 && potions.size() >= 5) perms[index][4] = 0;
                        ++index;
                        if (potions.size() < 5) break;
                    }
                    if (potions.size() < 4) break;
                }
                if (potions.size() < 3) break;
            }
            if (potions.size() < 2) break;
        }
    }
    for (int i{ 0 }; i < perms.size(); ++i)
    {
        while (perms[i].size() < 5)
        {
            perms[i].push_back(0);
        }
    }
    vector<stateInfo> tempSINFOs(perms.size(), stateInfo(true));
    for (int i{ 0 }; i < perms.size(); ++i)
    {
        tempSINFOs[i].potionIDs = perms[i];
        for (int j{ 0 }; j < perms[i].size(); ++j)
        {
            if (perms[i][j] == 0) continue;
            tempSINFOs[i].masksForHandCards[perms[i][j] + 262] = apci.getPotion(j)->canUse();
        }
    }
    vector<float> qVals = callMicroValues(tempSINFOs,gm.getPyFunc());
    // if fine without potions, use none
    if (qVals.back() > (static_cast<float>(apci.getPlayer().getHealth()) / 10.0f) - 2 && !onlyUseOne) return perms.front();
    // if you're low, use all
    if (qVals.back() < 1.5) return perms.back();
    // if they make no difference use none
    if (qVals.back() - qVals.front() < 1 && !onlyUseOne) return perms.front();
    // find some balance of picking the best qval with the fewest potions.
    pair<int, float> index_best{0,0.0f};
    for (int i{ 0 }; i < perms.size(); ++i)
    {
        if (onlyUseOne)
        {
            if (i > potions.size()) break;
            if (qVals[i] > index_best.second)
            {
                index_best.first = i;
                index_best.second = qVals[i];
            }
        }
        else { if (qVals[i] - qVals.front() > 2) return perms[i]; }
    }
    if (onlyUseOne)
    {
        return perms[index_best.first];
    }
    if (qVals.front() > 4) return perms.front();
    else { return perms.back(); }
    
}