#include "gamestate.h"
#include "pc_info.h"
#include "potions.h"
#include "relic.h"
#include "character.h"
#include "card.h"
#include "events.h"

using namespace std;

// ############# potions ##################
void gamestate::makePotionDatabase()
{
    potion_database.emplace("AncientPot_ID", make_unique<ancientPotion>());
    potion_database.emplace("AttackPot_ID", make_unique<attackPotion>());
    potion_database.emplace("BlessingOfTheForge_ID", make_unique<blessingOFPotion>());
    potion_database.emplace("BlockPot_ID", make_unique<blockPotion>());
    potion_database.emplace("BloodPot_ID", make_unique<bloodPotion>());
    potion_database.emplace("ColourlessPot_ID", make_unique<colourlessPotion>());
    potion_database.emplace("CultistPot_ID", make_unique<cultistPotion>());
    potion_database.emplace("DexterityPot_ID", make_unique<dexterityPotion>());
    potion_database.emplace("DistilledChaos_ID", make_unique<distilledChaos>());
    potion_database.emplace("DuplicationPot_ID", make_unique<duplicationPotion>());
    potion_database.emplace("Elixir_ID", make_unique<elixir>());
    potion_database.emplace("EnergyPot_ID", make_unique<energyPotion>());
    potion_database.emplace("EntropicBrew_ID", make_unique<entropicBrew>());
    potion_database.emplace("EssenceOfSteel_ID", make_unique<essenceOfSteel>());
    potion_database.emplace("ExplosivePotion_ID", make_unique<explosivePotion>());
    potion_database.emplace("FairyInABottle_ID", make_unique<fairyInABottle>());
    potion_database.emplace("FearPotion_ID", make_unique<fearPotion>());
    potion_database.emplace("FirePotion_ID", make_unique<firePotion>());
    potion_database.emplace("FlexPotion_ID", make_unique<flexPotion>());
    potion_database.emplace("FruitJuice_ID", make_unique<fruitJuice>());
    potion_database.emplace("GamblersBrew_ID", make_unique<gamblersBrew>());
    potion_database.emplace("HeartOfIron_ID", make_unique<heartOfIron>());
    potion_database.emplace("LiquidBronze_ID", make_unique<liquidBronze>());
    potion_database.emplace("LiquidMemories_ID", make_unique<liquidMemories>());
    potion_database.emplace("PowerPotion_ID", make_unique<powerPotion>());
    potion_database.emplace("RegenPotion_ID", make_unique<regenPotion>());
    potion_database.emplace("SkillPotion_ID", make_unique<skillPotion>());
    //potion_database.emplace("SmokeBomb_ID", make_unique<smokeBomb>());
    potion_database.emplace("SneckoOil_ID", make_unique<sneckoOil>());
    potion_database.emplace("SpeedPotion_ID", make_unique<speedPotion>());
    potion_database.emplace("StrengthPotion_ID", make_unique<strengthPotion>());
    potion_database.emplace("SwiftPotion_ID", make_unique<swiftPotion>());
    potion_database.emplace("WeakPotion_ID", make_unique<weakPotion>());
}
potion* gamestate::retrieveCommonPotion()
{
    if (commonPotiondeck.size() == 0) throw runtime_error("no cPotions");
    return commonPotiondeck[random_int(0, commonPotiondeck.size() - 1)]->clone();
}
potion* gamestate::retrieveUncommonPotion()
{
    if (uncommonPotiondeck.size() == 0) throw runtime_error("no uPotions");
    return uncommonPotiondeck[random_int(0, uncommonPotiondeck.size() - 1)]->clone();
}
potion* gamestate::retrieveRarePotion()
{
    if (rarePotiondeck.size() == 0) throw runtime_error("no rPotions");
    return rarePotiondeck[random_int(0, rarePotiondeck.size() - 1)]->clone();
}
void gamestate::makePotionStacks()
{
    commonPotiondeck.clear();
    uncommonPotiondeck.clear();
    rarePotiondeck.clear();
    for (auto itr{ potion_database.begin() }; itr != potion_database.end(); ++itr)
    {
        switch (itr->second->getRarity())
        {
        case cardRarity::Common:
            commonPotiondeck.push_back(itr->second->cloneU());
            break;
        case cardRarity::Uncommon:
            uncommonPotiondeck.push_back(itr->second->cloneU());
            break;
        case cardRarity::Rare:
            rarePotiondeck.push_back(itr->second->cloneU());
            break;
        }
    }
    shufflePotions(commonPotiondeck, static_cast<int>(commonPotiondeck.size()) - 1);
    shufflePotions(uncommonPotiondeck, static_cast<int>(uncommonPotiondeck.size()) - 1);
    shufflePotions(rarePotiondeck, static_cast<int>(rarePotiondeck.size()) - 1);
}
potion* gamestate::getPotionByID(const string& s)
{
    auto res = potion_database.find(s);
    if (res != potion_database.end())
    {
        return res->second->clone();
    }
    else { throw runtime_error("cannot find potion"); }
}
vector<potion*> gamestate::retrieveXRandomPotions(int x)
{
    vector<potion*> pots;
    while (x-- != 0)
    {
        int res = random_int(1, 100);
        if (res >= 1 && res <= common_pc_potion) { pots.push_back(gm.retrieveCommonPotion()); }
        else if (res >= common_pc_potion + 1 && res <= common_pc_potion + uncommon_pc_potion) { pots.push_back(gm.retrieveUncommonPotion()); }
        else { pots.push_back(gm.retrieveRarePotion()); }
    }
    return pots;
}
void gamestate::XPotionSelection(vector<potion*> pots)
{
    if (gamestate::sozuCheck)
    {
        for (int i{0}; i<pots.size(); ++i)
        {
            delete pots[i];
        }
        return;
    }
    while (true)
    {
        if (ai) { apci.aiPotionChoice(pots, apci.getPotionSlots()); return; }
        else 
        {
            vector<string> validInputs = { "skip" };
            for (int i{ 1 }; i <= pots.size(); ++i)
            {
                if (!ai) std::cout << i << " for " << pots[i - 1]->getName() << "." << endl;
                validInputs.push_back(to_string(i));
            }
            if (!ai) std::cout << "Or skip.";
            apci.addPotionInputs(validInputs);
            string result = request_input(validInputs);
            if (result[0] == 'p') { apci.potionMenu(static_cast<int>(result[1]) - 49); continue; }
            if (result == "skip") { return; }
            if (!apci.addPotion(pots[stoi(result) - 1])) continue;
            else { pots.erase(pots.begin() + stoi(result) - 1); }
            if (pots.size() == 0) return;
        }
    }
}
void gamestate::potionReward()
{
    if (gamestate::sozuCheck) return;
    if (random_int(1, 100) <= current_potion_pc)
    {
        potion* pot;
        int res = random_int(1, 100);
        if (res >= 1 && res <= common_pc_potion) { pot = gm.retrieveCommonPotion(); }
        else if (res >= common_pc_potion + 1 && res <= common_pc_potion + uncommon_pc_potion) { pot = gm.retrieveUncommonPotion(); }
        else { pot = gm.retrieveRarePotion(); }
        if (ai && apci.getPotions().size() < apci.getPotionSlots())
        {
            apci.addPotion(pot);
        }
        else if (ai)
        {
            vector<potion*> opts = apci.getPotions();
            opts.push_back(pot);
            apci.aiPotionChoice(opts, apci.getPotionSlots());
        }
        else
        {
            while (true)
            {
                vector<string> validInputs = { "1","skip" };
                if (!ai) std::cout << pot->getName() << " : 1 to take. Or skip" << endl;
                apci.addPotionInputs(validInputs);
                string result = request_input(validInputs);
                if (result[0] == 'p') { apci.potionMenu(static_cast<int>(result[1]) - 49); continue; }
                if (result == "1") { if (!apci.addPotion(pot)) continue; }
                break;
            }
        }
        if (current_potion_pc != 100) current_potion_pc -= 10;
    }
    else { current_potion_pc = clamp(current_potion_pc + 10, 0, 100); }
}
void gamestate::prelude()
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
    //showPath();
}

// ############# relics ##################
void gamestate::makeRelicDatabase()
{
    // relic database
    relic_database.emplace("Akabeko_ID", make_unique<akabeko>());
    relic_database.emplace("Anchor_ID", make_unique<anchor>());
    relic_database.emplace("AncientTeaSet_ID", make_unique<ancientTeaSet>());//
    relic_database.emplace("ArtOfWar_ID", make_unique<artOfWar>());
    relic_database.emplace("BagOfMarbles_ID", make_unique<bagOfMarbles>());
    relic_database.emplace("BagOfPreparation_ID", make_unique<bagOfPreparation>());
    relic_database.emplace("BloodVial_ID", make_unique<bloodVial>());
    relic_database.emplace("BronzeScales_ID", make_unique<bronzeScales>());
    relic_database.emplace("CentennialPuzzle_ID", make_unique<centennialPuzzle>());
    relic_database.emplace("CeramicFish_ID", make_unique<ceramicFish>());//
    relic_database.emplace("DreamCatcher_ID", make_unique<dreamCatcher>());//
    relic_database.emplace("HappyFlower_ID", make_unique<happyFlower>());
    relic_database.emplace("JuzuBracelet_ID", make_unique<juzuBracelet>());//
    relic_database.emplace("Lantern_ID", make_unique<lantern>());
    relic_database.emplace("MawBank_ID", make_unique<mawBank>());//
    relic_database.emplace("MealTicket_ID", make_unique<mealTicket>());//
    relic_database.emplace("Nunchuku_ID", make_unique<nunchuku>());
    relic_database.emplace("OddlySmoothStone_ID", make_unique<oddlySmoothStone>());
    relic_database.emplace("Omamori_ID", make_unique<omamori>());//
    relic_database.emplace("Orichalcum_ID", make_unique<orichalcum>());
    relic_database.emplace("PenNib_ID", make_unique<penNib>());
    relic_database.emplace("PotionBelt_ID", make_unique<potionBelt>());
    relic_database.emplace("PreservedInsect_ID", make_unique<preservedInsect>());
    relic_database.emplace("RegalPillow_ID", make_unique<regalPillow>());//
    relic_database.emplace("SmilingMask_ID", make_unique<smilingMask>());//
    relic_database.emplace("Strawberry_ID", make_unique<strawberry>());
    relic_database.emplace("TheBoot_ID", make_unique<theBoot>());
    relic_database.emplace("TinyChest_ID", make_unique<tinyChest>());//
    relic_database.emplace("ToyOrnithopter_ID", make_unique<toyOrnithopter>());
    relic_database.emplace("Vajra_ID", make_unique<vajra>());
    relic_database.emplace("WarPaint_ID", make_unique<warPaint>());//
    relic_database.emplace("Whetstone_ID", make_unique<whetstone>());//

    relic_database.emplace("BlueCandle_ID", make_unique<blueCandle>());
    relic_database.emplace("BottledFlame_ID", make_unique<bottledFlame>());//
    relic_database.emplace("BottledLightning_ID", make_unique<bottledLightning>());//
    relic_database.emplace("BottledTornado_ID", make_unique<bottledTornado>());//
    relic_database.emplace("DarkstonePeriapt_ID", make_unique<darkstonePeriapt>());//
    relic_database.emplace("EternalFeather_ID", make_unique<eternalFeather>());//
    relic_database.emplace("FrozenEgg_ID", make_unique<frozenEgg>());//
    relic_database.emplace("GremlinHorn_ID", make_unique<gremlinHorn>());
    relic_database.emplace("HornCleat_ID", make_unique<hornCleat>());
    relic_database.emplace("InkBottle_ID", make_unique<inkBottle>());
    relic_database.emplace("Kunai_ID", make_unique<kunai>());
    relic_database.emplace("LetterOpener_ID", make_unique<letterOpener>());
    relic_database.emplace("Matryoshka_ID", make_unique<matryoshka>());//
    relic_database.emplace("MeatOnTheBone_ID", make_unique<meatOnTheBone>());
    relic_database.emplace("MercuryHourglass_ID", make_unique<mercuryHourglass>());
    relic_database.emplace("MoltenEgg_ID", make_unique<moltenEgg>());//
    relic_database.emplace("MummifiedHand_ID", make_unique<mummifiedHand>());
    relic_database.emplace("OrnamentalFan_ID", make_unique<ornamentalFan>());
    relic_database.emplace("Pantograph_ID", make_unique<pantograph>());//
    relic_database.emplace("Pear_ID", make_unique<pear>());
    relic_database.emplace("QuestionCard_ID", make_unique<questionCard>());//
    relic_database.emplace("Shuriken_ID", make_unique<shuriken>());
    relic_database.emplace("SingingBowl_ID", make_unique<singingBowl>());//
    relic_database.emplace("StrikeDummy_ID", make_unique<strikeDummy>());
    relic_database.emplace("Sundial_ID", make_unique<sundial>());
    relic_database.emplace("TheCourier_ID", make_unique<theCourier>());//
    relic_database.emplace("ToxicEgg_ID", make_unique<toxicEgg>());//
    relic_database.emplace("WhiteBeastStatue_ID", make_unique<whiteBeastStatue>());//

    relic_database.emplace("BirdFacedUrn_ID", make_unique<birdFacedUrn>());
    relic_database.emplace("CaptainsWheel_ID", make_unique<captainsWheel>());
    relic_database.emplace("Calipers_ID", make_unique<calipers>());
    relic_database.emplace("DeadBranch_ID", make_unique<deadBranch>());
    relic_database.emplace("DuVuDoll_ID", make_unique<duVuDoll>());
    relic_database.emplace("FossilizedHelix_ID", make_unique<fossilizedHelix>());
    relic_database.emplace("GamblingChip_ID", make_unique<gamblingChip>());//
    relic_database.emplace("Ginger_ID", make_unique<ginger>());
    relic_database.emplace("Girya_ID", make_unique<girya>());//
    relic_database.emplace("IceCream_ID", make_unique<iceCream>());
    relic_database.emplace("IncenseBurner_ID", make_unique<incenseBurner>());
    relic_database.emplace("LizardTail_ID", make_unique<lizardTail>());
    relic_database.emplace("Mango_ID", make_unique<mango>());
    relic_database.emplace("OldCoin_ID", make_unique<oldCoin>());//
    relic_database.emplace("PeacePipe_ID", make_unique<peacePipe>());//
    relic_database.emplace("Pocketwatch_ID", make_unique<pocketwatch>());
    relic_database.emplace("PrayerWheel_ID", make_unique<prayerWheel>());//
    relic_database.emplace("Shovel_ID", make_unique<shovel>());//
    relic_database.emplace("StoneCalendar_ID", make_unique<stoneCalendar>());
    relic_database.emplace("ThreadAndNeedle_ID", make_unique<threadAndNeedle>());
    relic_database.emplace("Torii_ID", make_unique<torii>());
    relic_database.emplace("Tungsten_ID", make_unique<tungsten>());
    relic_database.emplace("Turnip_ID", make_unique<turnip>());
    relic_database.emplace("UnceasingTop_ID", make_unique<unceasingTop>());
    relic_database.emplace("WingBoots_ID", make_unique<wingBoots>());//
    
    relic_database.emplace("Cauldron_ID", make_unique<cauldron>());//
    relic_database.emplace("ChemicalX_ID", make_unique<chemicalX>());
    relic_database.emplace("ClockworkSouvenir_ID", make_unique<clockworkSouvenir>());
    relic_database.emplace("DollysMirror_ID", make_unique<dollysMirror>());//
    relic_database.emplace("FrozenEye_ID", make_unique<frozenEye>());//
    relic_database.emplace("HandDrill_ID", make_unique<handDrill>());
    relic_database.emplace("LeesWaffle_ID", make_unique<leesWaffle>());
    relic_database.emplace("MedKit_ID", make_unique<medKit>());
    if (fullRuns) relic_database.emplace("MembershipCard_ID", make_unique<membershipCard>());//
    relic_database.emplace("OrangePellets_ID", make_unique<orangePellets>());
    relic_database.emplace("Orrery_ID", make_unique<orrery>());//
    relic_database.emplace("SlingOfCourage_ID", make_unique<slingOfCourage>());
    relic_database.emplace("StrangeSpoon_ID", make_unique<strangeSpoon>());
    relic_database.emplace("TheAbacus_ID", make_unique<theAbacus>());
    relic_database.emplace("Toolbox_ID", make_unique<toolbox>());//

    relic_database.emplace("BloodyIdol_ID", make_unique<bloodyIdol>());
    relic_database.emplace("CultistHeadpiece_ID", make_unique<cultistHeadpiece>());//
    relic_database.emplace("FaceOfCleric_ID", make_unique<faceOfCleric>());
    relic_database.emplace("GoldenIdol_ID", make_unique<goldenIdol>());//
    relic_database.emplace("GremlinVisage_ID", make_unique<gremlinVisage>());
    relic_database.emplace("NeowsLament_ID", make_unique<neowsLament>());//
    relic_database.emplace("NlothsHungryFace_ID", make_unique<nlothsHungryFace>());//
    relic_database.emplace("OddMushroom_ID", make_unique<oddMushroom>());
    relic_database.emplace("SsserpentHead_ID", make_unique<ssserpentHead>());//
    relic_database.emplace("WarpedTongs_ID", make_unique<warpedTongs>());

    relic_database.emplace("BurningBlood_ID", make_unique<burningBlood>());

    relic_database.emplace("RedSkull_ID", make_unique<redSkull>());
    relic_database.emplace("PaperPhrog_ID", make_unique<paperPhrog>());
    relic_database.emplace("SelfFormingClay_ID", make_unique<selfFormingClay>());
    relic_database.emplace("ChampionsBelt_ID", make_unique<championsBelt>());
    relic_database.emplace("CharonsAshes_ID", make_unique<charonsAshes>());
    relic_database.emplace("MagicFlower_ID", make_unique<magicFlower>());
    relic_database.emplace("Brimstone_ID", make_unique<brimstone>());

    relic_database.emplace("Astrolabe_ID", make_unique<astrolabe>());//
    if(fullRuns) relic_database.emplace("BlackStar_ID", make_unique<blackStar>());//
    relic_database.emplace("BustedCrown_ID", make_unique<bustedCrown>());
    if(fullRuns) relic_database.emplace("CallingBell_ID", make_unique<callingBell>());//
    relic_database.emplace("CoffeeDripper_ID", make_unique<coffeeDripper>());
    relic_database.emplace("CursedKey_ID", make_unique<cursedKey>());
    relic_database.emplace("Ectoplasm_ID", make_unique<ectoplasm>());
    relic_database.emplace("EmptyCage_ID", make_unique<emptyCage>());//
    relic_database.emplace("FusionHammer_ID", make_unique<fusionHammer>());
    relic_database.emplace("PandorasBox_ID", make_unique<pandorasBox>());
    relic_database.emplace("PhilosophersStone_ID", make_unique<philosophersStone>());
    relic_database.emplace("RunicPyramid_ID", make_unique<runicPyramid>());
    relic_database.emplace("SlaversCollar_ID", make_unique<slaversCollar>());
    relic_database.emplace("SneckoEye_ID", make_unique<sneckoEye>());
    relic_database.emplace("Sozu_ID", make_unique<sozu>());
    relic_database.emplace("TinyHouse_ID", make_unique<tinyHouse>());//
    relic_database.emplace("VelvetChoker_ID", make_unique<velvetChoker>());

    relic_database.emplace("BlackBlood_ID", make_unique<blackBlood>());//
    relic_database.emplace("MarkOfPain_ID", make_unique<markOfPain>());//
    relic_database.emplace("RunicCube_ID", make_unique<runicCube>());//

    relic_database.emplace("MutagenicStrength_ID", make_unique<mutagenicStrength>());
    relic_database.emplace("RedMask_ID", make_unique<redMask>());
    relic_database.emplace("Enchridion_ID", make_unique<enchridion>());
    relic_database.emplace("NilrysCodex_ID", make_unique<nilrysCodex>());//
    relic_database.emplace("Necronomicon_ID", make_unique<necronomicon>());
    relic_database.emplace("NlothsGift_ID", make_unique<nlothsGift>());//
    relic_database.emplace("MarkOfTheBloom_ID", make_unique<markOfTheBloom>());//
}
relic* gamestate::retrieveCommonRelic()
{
    if (commonRelicdeck.size() == 0) throw runtime_error("no relics");
    while (getMapPathEvent() == 's' && (commonRelicdeck.front()->getName() == "Maw Bank" || commonRelicdeck.front()->getName() == "Smiling Mask"))
    {
        commonRelicdeck.push_back(std::move(commonRelicdeck.front()));
        commonRelicdeck.pop_front();
    }
    auto temp = commonRelicdeck[0]->clone();
    commonRelicdeck.pop_front();
    return temp;
    
}
relic* gamestate::retrieveUncommonRelic()
{
    if (uncommonRelicdeck.size() == 0) throw runtime_error("no relics");
    while (getMapPathEvent() == 's' && uncommonRelicdeck.front()->getName() == "The Courier")
    {
        uncommonRelicdeck.push_back(std::move(uncommonRelicdeck.front()));
        uncommonRelicdeck.pop_front();
    }
    auto temp = uncommonRelicdeck[0]->clone();
    uncommonRelicdeck.pop_front();
    return temp;
}
relic* gamestate::retrieveRareRelic()
{
    if (rareRelicdeck.size() == 0) throw runtime_error("no relics");
    while (getMapPathEvent() == 's' && rareRelicdeck.front()->getName() == "Old Coin")
    {
        rareRelicdeck.push_back(std::move(rareRelicdeck.front()));
        rareRelicdeck.pop_front();
    }
    auto temp = rareRelicdeck[0]->clone();
    rareRelicdeck.pop_front();
    return temp;
}
relic* gamestate::retrieveBossRelic()
{
    if (bossRelicdeck.size() == 0) throw runtime_error("no relics");
    if (bossRelicdeck.front()->getName() == "Black Blood" && !apci.checkForRelic("Burning Blood"))
    {
        bossRelicdeck.erase(bossRelicdeck.begin());
    }
    if (actNumber == 2 && bossRelicdeck.front()->getName() == "Ectoplasm")
    {
        auto temp = bossRelicdeck[1]->clone();
        bossRelicdeck.erase(bossRelicdeck.begin() + 1);
        return temp;
    }
    else
    {
        auto temp = bossRelicdeck[0]->clone();
        bossRelicdeck.pop_front();
        return temp;
    }
}
relic* gamestate::retrieveShopRelic()
{
    if (shopRelicdeck.size() == 0) throw runtime_error("no relics");
    auto temp = shopRelicdeck[0]->clone();
    shopRelicdeck.pop_front();
    return temp;
}
relic* gamestate::getCommonRelic()
{
    int rng = random_int(0, commonRelicdeck.size() - 1);
    bool repeat{ true };
    while (repeat)
    {
        repeat = false;
        for (int i{ 0 }; i < apci.getRelics().size(); ++i)
        {
            if (commonRelicdeck[rng]->getName() == apci.getRelics()[i]->getName())
            {
                rng = random_intNot(0, commonRelicdeck.size() - 1, rng);
                repeat = true;
                break;
            }
        }
    }
    return commonRelicdeck[rng]->clone();
}
relic* gamestate::getUncommonRelic()
{
    int rng = random_int(0, uncommonRelicdeck.size() - 1);
    bool repeat{ true };
    while (repeat)
    {
        repeat = false;
        for (int i{ 0 }; i < apci.getRelics().size(); ++i)
        {
            if (uncommonRelicdeck[rng]->getName() == apci.getRelics()[i]->getName())
            {
                rng = random_intNot(0, uncommonRelicdeck.size() - 1, rng);
                repeat = true;
                break;
            }
        }
    }
    return uncommonRelicdeck[rng]->clone();
}
relic* gamestate::getRareRelic()
{
    int rng = random_int(0, rareRelicdeck.size() - 1);
    bool repeat{ true };
    while (repeat)
    {
        repeat = false;
        for (int i{ 0 }; i < apci.getRelics().size(); ++i)
        {
            if (rareRelicdeck[rng]->getName() == apci.getRelics()[i]->getName())
            {
                rng = random_intNot(0, rareRelicdeck.size() - 1, rng);
                repeat = true;
                break;
            }
        }
    }
    return rareRelicdeck[rng]->clone();
}
relic* gamestate::getBossRelic()
{
    return bossRelicdeck[random_int(0, bossRelicdeck.size() - 1)]->clone();
}
relic* gamestate::getBossRelic(relic* other)
{
    int index{ 0 };
    for (int i{ 0 }; i < bossRelicdeck.size(); ++i)
    {
        if (bossRelicdeck[i]->getRelicIDNum() == other->getRelicIDNum()) { index = i; break; }
    }
    return bossRelicdeck[random_intNot(0, bossRelicdeck.size() - 1,index)]->clone();
}
relic* gamestate::getShopRelic()
{
    int rng = random_int(0, shopRelicdeck.size() - 1);
    bool repeat{ true };
    while (repeat)
    {
        repeat = false;
        for (int i{ 0 }; i < apci.getRelics().size(); ++i)
        {
            if (shopRelicdeck[rng]->getName() == apci.getRelics()[i]->getName())
            {
                rng = random_intNot(0, shopRelicdeck.size() - 1, rng);
                repeat = true;
                break;
            }
        }
    }
    return shopRelicdeck[rng]->clone();
}
relic* gamestate::getEventRelic()
{
    int rng = random_int(0, eventRelicdeck.size() - 1);
    bool repeat{ true };
    while (repeat)
    {
        repeat = false;
        for (int i{ 0 }; i < apci.getRelics().size(); ++i)
        {
            if (eventRelicdeck[rng]->getName() == apci.getRelics()[i]->getName())
            {
                rng = random_intNot(0, eventRelicdeck.size() - 1, rng);
                repeat = true;
                break;
            }
        }
    }
    return eventRelicdeck[rng]->clone();
}
void gamestate::makeRelicStacks()
{
    commonRelicdeck.clear();
    uncommonRelicdeck.clear();
    rareRelicdeck.clear();
    bossRelicdeck.clear();
    shopRelicdeck.clear();
    eventRelicdeck.clear();
    for (auto itr{ relic_database.begin() }; itr != relic_database.end(); ++itr)
    {
        switch (itr->second->getRarity())
        {
        case relicRarity::Common:
            commonRelicdeck.push_back(itr->second->cloneU());
            break;
        case relicRarity::Uncommon:
            uncommonRelicdeck.push_back(itr->second->cloneU());
            break;
        case relicRarity::Rare:
            rareRelicdeck.push_back(itr->second->cloneU());
            break;
        case relicRarity::Boss:
            bossRelicdeck.push_back(itr->second->cloneU());
            break;
        case relicRarity::Shop:
            shopRelicdeck.push_back(itr->second->cloneU());
            break;
        case relicRarity::Event:
            eventRelicdeck.push_back(itr->second->cloneU());
            break;
        }
    }
    shuffleRelics(commonRelicdeck, static_cast<int>(commonRelicdeck.size()) - 1);
    shuffleRelics(uncommonRelicdeck, static_cast<int>(uncommonRelicdeck.size()) - 1);
    shuffleRelics(rareRelicdeck, static_cast<int>(rareRelicdeck.size()) - 1);
    shuffleRelics(bossRelicdeck, static_cast<int>(bossRelicdeck.size()) - 1);
    shuffleRelics(shopRelicdeck, static_cast<int>(shopRelicdeck.size()) - 1);
    shuffleRelics(eventRelicdeck, static_cast<int>(eventRelicdeck.size()) - 1);
}
relic* gamestate::getRelicByID(const string& s)
{
    auto res = relic_database.find(s);
    if (res != relic_database.end())
    {
        return res->second->clone();
    }
    else { throw runtime_error("cannot find relic"); }
}

int gamestate::chooseCardFromDraw(cardType ct)
{
    vector<string> validInputs;
    for (int i{ 1 }; i < apci.getDrawSize()+1; ++i)
    {
        if ((*apci.getDrawPile()[i - 1]).getType() != ct) continue;
        if(!ai) std::cout << i << " for " << (*apci.getDrawPile()[i - 1]).getName() << ". ";
        validInputs.push_back(to_string(i));
    }
    if (validInputs.empty()) return -1;

    string inputChoice = request_input(validInputs);
    int index = stoi(inputChoice) - 1;
    return index;
}

void gamestate::chooseFromThree(vector<C_DATABASE::iterator> options, bool skipAllowed, bool zeroCost)
{
    if (ai)
    {
        if (aiCardBuffer.empty()) aiCardBuffer = options;
        int cho;
        if (randomAIChoice) cho = random_int(0, skipAllowed ? 3 : 2);
        else 
        {
            cho = aiSimActionBuffer[0]; 
            aiSimActionBuffer.pop_front();
        }
        if (cho == 3) return;
        auto c = cardHolder(aiCardBuffer[cho]->second->clone());
        
        if (zeroCost) apci.makeCostZero(c);
        (*c).fightStart(true);
        apci.addCardToHand(c);
        randomAIChoice = true;
    }
    else
    {
        vector<string> validInputs;
        if (skipAllowed)
        {
            validInputs.push_back("skip");
            std::cout << "skip or ";
        }
        for (int i{ 1 }; i < options.size() + 1; ++i)
        {
            std::cout << i << " for " << options[i - 1]->second->getName() << ". ";
            validInputs.push_back(to_string(i));
        }
        string inputChoice = request_input(validInputs);
        if (inputChoice == "skip") { return; }
        auto c = cardHolder(options[stoi(inputChoice) - 1]->second->clone());
        if(zeroCost) apci.makeCostZero(c);
        (*c).fightStart(true);
        apci.addCardToHand(c);
    }
    
}
bool gamestate::checkIfBanned()
{
    if (apci.getPlayer().getJuggernautBuff() > 0) return true;
    if (apci.getPlayer().getEvolveBuff() > 0) return true;
    if (apci.getPlayer().getFireBreathingBuff() > 0) return true;

    for (int i{ 0 }; i < enemies_.size(); ++i)
    {
        if (enemies_[i]->getName() == "Bronze Orb") return true;
    }
    return apci.checkForRelic("Gremlin Horn") || apci.checkForRelic("Centennial Puzzle");
}

void gamestate::actionChoicesForSim(combatChoices actionChoices, int action)
{
    if (action < 263)
    {
        bool passedAction{ false };
        switch (actionChoices)
        {
        case combatChoices::Hand: // req inp
            for (int i{ 0 }; i < apci.getHand().size() - 1; ++i)
            {
                if (((((*apci.getHand()[i]).getCardIDNum() - 1) * 2 + ((*apci.getHand()[i]).getUpgraded() ? 1 : 0)) == action) && !passedAction) { --i; passedAction = true; continue; }
                aiSimActionBuffer.push_back(i + 1); // 0 1 2 3
                orderedChoiceTree.push_back({ i + 1 });
            }
            break;
        case combatChoices::HandDW: // req inp
        {
            vector<cardType> cts = { Attack,Power };
            for (int i{ 0 }; i < apci.getHand().size() - 1; ++i)
            {
                if (((((*apci.getHand()[i]).getCardIDNum() - 1) * 2 + ((*apci.getHand()[i]).getUpgraded() ? 1 : 0)) == action) && !passedAction) { --i; passedAction = true; continue; }
                if (find(cts.begin(), cts.end(), (*apci.getHand()[i]).getType()) != cts.end())
                {
                    aiSimActionBuffer.push_back(i + 1); // 0 1 2 3
                    orderedChoiceTree.push_back({ i + 1 });
                }
            }
            break;
        }
        case combatChoices::HandAR: // req inp
        {
            vector<cardType> cts = { Attack,Power,Skill };
            for (int i{ 0 }; i < apci.getHand().size() - 1; ++i)
            {
                if (((((*apci.getHand()[i]).getCardIDNum() - 1) * 2 + ((*apci.getHand()[i]).getUpgraded() ? 1 : 0)) == action) && !passedAction) { --i; passedAction = true; continue; }
                if (find(cts.begin(), cts.end(), (*apci.getHand()[i]).getType()) != cts.end())
                {
                    aiSimActionBuffer.push_back(i + 1); // 0 1 2 3
                    orderedChoiceTree.push_back({ i + 1 });
                }
            }
            break;
        }
        case combatChoices::HandWC: // req inp
        {
            if (apci.getPlayer().getDUPPotion() > 0 || gm.checkIfBanned()) { gamestate::randomAIChoice = true; return; }
            int x = static_cast<int>(apci.getHand().size()) - 1;
            if (apci.getDiscardSize() + apci.getDrawSize() > 0 && !apci.getPlayer().getNoDraw())
            {
                ++x;
            }
            if (x > 10) x = 10;
            for (int i{ 0 }; i < x; ++i)
            {
                aiSimActionBuffer.push_back(i + 1); // 0 1 2 3
                orderedChoiceTree.push_back({ i + 1 });
            }
            break;
        }
        case combatChoices::HandTA: // req inp
        {
            if (apci.getPlayer().getDUPPotion() > 0 || gm.checkIfBanned()) { gamestate::randomAIChoice = true; return; }
            int x = static_cast<int>(apci.getHand().size()) - 1;
            if (apci.getDiscardSize() + apci.getDrawSize() > 1 && !apci.getPlayer().getNoDraw())
            {
                x+=2;
            }
            else if (apci.getDiscardSize() + apci.getDrawSize() == 1 && !apci.getPlayer().getNoDraw())
            {
                x += 1;
            }
            if (x > 10) x = 10;
            for (int i{ 0 }; i < x; ++i)
            {
                aiSimActionBuffer.push_back(i + 1); // 0 1 2 3
                orderedChoiceTree.push_back({ i + 1 });
            }
            break;
        }
        case combatChoices::HandMulti3: // other
        {
            int perms{ 0 };
            int s = static_cast<int>(apci.getHand().size()) - 1;
            if (action == -1) ++s;
            int maxChoose = s < 3 ? s : 3;
            for (int j{ 0 }; j < maxChoose; ++j)
            {
                perms += NchooseR(s, j);
            }
            for (int i{ 0 }; i < perms; ++i)
            {
                aiSimActionBuffer.push_back(i);
                orderedChoiceTree.push_back({ i });
            }
            break;
        }
        case combatChoices::HandMulti5: // other
        {
            int perms{ 0 };
            int s = static_cast<int>(apci.getHand().size()) - 1;
            if (action == -1) ++s;
            int maxChoose = s < 5 ? s : 5;
            for (int j{ 0 }; j < maxChoose; ++j)
            {
                perms += NchooseR(s, j);
            }
            for (int i{ 0 }; i < perms; ++i)
            {
                aiSimActionBuffer.push_back(i);
                orderedChoiceTree.push_back({ i });
            }
            break;
        }
        case combatChoices::HandMultiAll: // other
        {
            int perms{ 0 };
            int s = static_cast<int>(apci.getHand().size());
            if (action == -1) ++s;
            int maxChoose = s;
            for (int j{ 0 }; j < maxChoose; ++j)
            {
                perms += NchooseR(s, j);
            }
            for (int i{ 0 }; i < perms; ++i)
            {
                aiSimActionBuffer.push_back(i);
                orderedChoiceTree.push_back({ i });
            }
            break;
        }
        case combatChoices::EnemyTarget: // req inp
            for (int i{ 0 }; i < gm.getAllEnemies().size(); ++i)
            {
                aiSimActionBuffer.push_back(i + 1);
                orderedChoiceTree.push_back({ i + 1 });
            }
            break;
        case combatChoices::Exhaust: // req inp
            for (int i{ 0 }; i < apci.getExhausted().size(); ++i)
            {
                aiSimActionBuffer.push_back(i + 1);
                orderedChoiceTree.push_back({ i + 1 });
            }
            break;
        case combatChoices::Headbutt: // req inp
            if (apci.getPlayer().getDUPDoubleTap() > 0 || apci.getPlayer().getDUPPotion() > 0 || apci.checkForRelic("Necronomicon") || gm.checkIfBanned()) { gamestate::randomAIChoice = true; return; }
            for (int j{ 0 }; j < gm.getAllEnemies().size(); ++j)
            {
                if (apci.getDiscardSize() == 0)
                {
                    aiSimActionBuffer.push_back(j + 1);
                    orderedChoiceTree.push_back({ j + 1 });
                }
                else
                {
                    for (int i{ 0 }; i < apci.getDiscardSize(); ++i)
                    {
                        if (gm.getAllEnemies().size() > 1 && apci.getDiscardSize() > 1)
                        {
                            aiSimActionBuffer.push_back(j + 1);
                            aiSimActionBuffer.push_back(i + 1);
                            orderedChoiceTree.push_back({ j + 1,i + 1 });
                        }
                        else if (gm.getAllEnemies().size() == 1 && apci.getDiscardSize() > 1)
                        {
                            aiSimActionBuffer.push_back(i + 1);
                            orderedChoiceTree.push_back({ i + 1 });
                        }
                        else if (gm.getAllEnemies().size() > 1 && apci.getDiscardSize() <= 1)
                        {
                            aiSimActionBuffer.push_back(j + 1);
                            orderedChoiceTree.push_back({ j + 1 });
                        }
                    }
                }

            }
            break;
        case combatChoices::Choice3Skip: // other
            for (int i{ 0 }; i < 4; ++i)
            {
                aiSimActionBuffer.push_back(i);
                orderedChoiceTree.push_back({ i });
            }
            break;
        case combatChoices::Choice3NoSkip: // other
            for (int i{ 0 }; i < 3; ++i)
            {
                aiSimActionBuffer.push_back(i);
                orderedChoiceTree.push_back({ i });
            }
            break;
        case combatChoices::DrawAtt: // req inp
            for (int i{ 0 }; i < apci.getDrawSize(); ++i)
            {
                if ((*apci.getDrawPile()[i]).getType() == Attack)
                {
                    aiSimActionBuffer.push_back(i + 1);
                    orderedChoiceTree.push_back({ i + 1 });
                }
            }
            break;
        case combatChoices::DrawSkl: // req inp
            for (int i{ 0 }; i < apci.getDrawSize(); ++i)
            {
                if ((*apci.getDrawPile()[i]).getType() == Skill)
                {
                    aiSimActionBuffer.push_back(i + 1);
                    orderedChoiceTree.push_back({ i + 1 });
                }
            }
            break;
        case combatChoices::Havoc:
            if (gm.checkIfBanned()) { gamestate::randomAIChoice = true; return; }
            break;
        }
    }
    else
    {
        for (int i{ 0 }; i < apci.getPotions().size(); ++i)
        {
            if (apci.getPotion(i)->getPotionIDNum() + 262 == action)
            {
                potionUse pU = apci.getPotion(i)->getPotionUsage();
                switch (pU)
                {
                case potionUse::Enemy:
                    if (gm.getAllEnemies().size() > 1)
                    {
                        for (int j{ 0 }; j < gm.getAllEnemies().size(); ++j)
                        {
                            aiSimActionBuffer.push_back(j + 1);
                            orderedChoiceTree.push_back({ j + 1 });
                        }
                    }
                    break;
                case potionUse::SelfCombat:
                    switch (apci.getPotion(i)->getCombatChoices())
                    {
                    case combatChoices::Choice3Skip:
                        for (int j{ 0 }; j < 4; ++j)
                        {
                            aiSimActionBuffer.push_back( j );
                            orderedChoiceTree.push_back({ j });
                        }
                        break;
                    case combatChoices::Discard:
                        if (apci.getDiscardSize() > 1)
                        {
                            for (int j{ 0 }; j < apci.getDiscardSize(); ++j)
                            {
                                aiSimActionBuffer.push_back(j + 1);
                                orderedChoiceTree.push_back({ j + 1 });
                            }
                        }
                        break;
                    case combatChoices::HandMultiAll:
                    {
                        int perms{ 0 };
                        int s = static_cast<int>(apci.getHand().size());
                        if (action == -1) ++s;
                        int maxChoose = s;
                        for (int j{ 0 }; j < maxChoose; ++j)
                        {
                            perms += NchooseR(s, j);
                        }
                        for (int k{ 0 }; k < perms; ++k)
                        {
                            aiSimActionBuffer.push_back(k);
                            orderedChoiceTree.push_back({ k });
                        }
                        break;
                    }
                    case combatChoices::Havoc:
                        if (gm.checkIfBanned()) { gamestate::randomAIChoice = true; return; }
                        break;
                    }
                    break;
                case potionUse::SelfAny:
                    break;
                }
                break;
            }
        }
    }
}

void gamestate::qEventMatchAndKeep(vector<card*>& known_, const vector<string>& letters_, int first_)
{
    if (first_ == -1)
    {
        int starter_{ 0 };
        while (known_[starter_] != nullptr) ++starter_;
        if (starter_ > 2)
        {
            bool canTakeSeen{ false };
            for (int i{ 0 }; i < starter_-1; ++i)
            {
                if (known_[i] != nullptr && known_[i]->getCardIDNum() == known_[starter_-1]->getCardIDNum())
                {
                    canTakeSeen = true;
                    break;
                }
            }
            if (canTakeSeen)
            {
                makePathChoiceFromNeutral(false);
                macroStateInfo msi_opt(msi);
                int index = (known_[starter_ - 1]->getCardIDNum() - 1) * 2;
                switch (known_[starter_ - 1]->getType())
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
                ++msi_opt.deckCountPerCard[index];
                if (apci.checkForRelic("Ceramic Fish")) msi_opt.playerVars[10] += 9;
                switch (known_[starter_ - 1]->getType())
                {
                case Attack:
                    ++msi_opt.playerVars[3];
                    break;
                case Skill:
                    ++msi_opt.playerVars[4];
                    break;
                case Power:
                    ++msi_opt.playerVars[5];
                    break;
                }
                float exp = callMacroSINGLE(msi_opt, getPyFunc());
                float skip = callMacroSINGLE(msi, getPyFunc());

                if (skip > exp)
                {
                    aiEventActionBuffer.push_back(letters_[starter_]);
                    return;
                }
                else
                {
                    aiEventActionBuffer.push_back(letters_[starter_ - 1]);
                    return;
                }
            }
        }
        aiEventActionBuffer.push_back(letters_[starter_]);
    }
    else
    {
        if (first_ == 0)
        {
            aiEventActionBuffer.push_back(letters_[first_+1]);
            return;
        }

        //makePathChoiceFromNeutral(false);
        macroStateInfo msi_opt(msi);
        int index = (known_[first_]->getCardIDNum() - 1) * 2;
        switch (known_[first_]->getType())
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
        ++msi_opt.deckCountPerCard[index];
        if (apci.checkForRelic("Ceramic Fish")) msi_opt.playerVars[10] += 9;
        switch (known_[first_]->getType())
        {
        case Attack:
            ++msi_opt.playerVars[3];
            break;
        case Skill:
            ++msi_opt.playerVars[4];
            break;
        case Power:
            ++msi_opt.playerVars[5];
            break;
        }
        float exp = callMacroSINGLE(msi_opt, getPyFunc());
        float skip = callMacroSINGLE(msi, getPyFunc());

        if (skip > exp)
        {
            for (int i{ 0 }; i < known_.size(); ++i)
            {
                if (i == first_) continue;
                if (known_[i] != nullptr && known_[i]->getCardIDNum() == known_[first_]->getCardIDNum())
                {
                    aiEventActionBuffer.push_back(letters_[first_ + 1]);
                    return;
                }
            }
            for (int i{ 0 }; i < known_.size(); ++i)
            {
                if (known_[i] != nullptr && known_[i]->getCardIDNum() != known_[first_]->getCardIDNum())
                {
                    aiEventActionBuffer.push_back(letters_[i]);
                    return;
                }
            }
            aiEventActionBuffer.push_back(letters_[first_+1]);
        }
        else
        {
            for (int i{ 0 }; i < known_.size(); ++i)
            {
                if (i == first_) continue;
                if (known_[i] != nullptr && known_[i]->getCardIDNum() == known_[first_]->getCardIDNum())
                {
                    aiEventActionBuffer.push_back(letters_[i]);
                    return;
                }
            }
            aiEventActionBuffer.push_back(letters_[first_ + 1]);
        }
    }
}

void gamestate::qEventOminousForge()
{
    msi.createAllVars();
    vector<macroStateInfo> paths_;
    mapGen->discreteMapRep(getCurrentNode(), msi, paths_);
    vector<float> bestCH(2);
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

    macroStateInfo msi_opt(msi);
    ++msi_opt.deckCountPerCard[152];
    if (!apci.checkForRelic("Omamori"))
    {
        if (apci.checkForRelic("Darkstone Periapt"))
        {
            msi_opt.playerVars[0] += 6;
            msi_opt.playerVars[1] += 6;
        }
        ++msi_opt.relicPresent[107];
    }
    for (int k{ 0 }; k < paths_.size(); ++k)
    {
        paths_[k].copyNonMapRep(msi_opt);
    }
    bestCH[0] = callMacroValueCONSTPRED(paths_, getPyFunc());
    if (bestCH[0] > bestCH[1])
    {
        aiEventActionBuffer.push_back("2");
    }
    else
    {
        aiEventActionBuffer.push_back("1");
    }
}
void gamestate::qEventTheCleric()
{
    msi.createAllVars();
    vector<macroStateInfo> paths_;
    mapGen->discreteMapRep(getCurrentNode(), msi, paths_);
    vector<float> bestCH(2);
    int j{ 0 };
    vector<macroStateInfo> msi_opts(apci.validRemoveCount(), msi);
    for (int i{ 0 }; i < apci.getDeck().size(); ++i)
    {
        if (apci.getDeck()[i]->getBottled() || apci.getDeck()[i]->getCardIDNum() == 80 || apci.getDeck()[i]->getCardIDNum() == 82) { continue; }
        int index = (apci.getDeck()[i]->getCardIDNum() - 1) * 2;
        if (apci.getDeck()[i]->getUpgraded()) ++index;
        --msi_opts[j].deckCountPerCard[index];
        msi_opts[j].playerVars[10] -= 50;
        for (int k{ 0 }; k < paths_.size(); ++k)
        {
            paths_[k].copyNonMapRep(msi_opts[j]);
        }
        float potential = callMacroValueCONSTPRED(paths_, getPyFunc());
        if (potential > bestCH[1]) bestCH[1] = potential;
        ++j;
    }

    macroStateInfo msi_opt(msi);
    msi_opt.playerVars[10] -= 35;
    msi_opt.playerVars[0] = clamp(msi_opt.playerVars[0] + (msi_opt.playerVars[0]*25/100),0, msi_opt.playerVars[1]);

    for (int k{ 0 }; k < paths_.size(); ++k)
    {
        paths_[k].copyNonMapRep(msi_opt);
    }
    bestCH[0] = callMacroValueCONSTPRED(paths_, getPyFunc());
    if (bestCH[0] > bestCH[1])
    {
        aiEventActionBuffer.push_back("1");
    }
    else
    {
        aiEventActionBuffer.push_back("2");
    }
}
void gamestate::qEventWingStatue()
{
    msi.createAllVars();
    vector<macroStateInfo> paths_;
    mapGen->discreteMapRep(getCurrentNode(), msi, paths_);
    vector<float> bestCH(2);
    int j{ 0 };
    vector<macroStateInfo> msi_opts(apci.validRemoveCount(), msi);
    for (int i{ 0 }; i < apci.getDeck().size(); ++i)
    {
        if (apci.getDeck()[i]->getBottled() || apci.getDeck()[i]->getCardIDNum() == 80 || apci.getDeck()[i]->getCardIDNum() == 82) { continue; }
        int index = (apci.getDeck()[i]->getCardIDNum() - 1) * 2;
        if (apci.getDeck()[i]->getUpgraded()) ++index;
        --msi_opts[j].deckCountPerCard[index];
        msi_opts[j].playerVars[0] -= 7;
        for (int k{ 0 }; k < paths_.size(); ++k)
        {
            paths_[k].copyNonMapRep(msi_opts[j]);
        }
        float potential = callMacroValueCONSTPRED(paths_, getPyFunc());
        if (potential > bestCH[1]) bestCH[1] = potential;
        ++j;
    }

    macroStateInfo msi_opt(msi);
    msi_opt.playerVars[10] += 65;

    for (int k{ 0 }; k < paths_.size(); ++k)
    {
        paths_[k].copyNonMapRep(msi_opt);
    }
    bestCH[0] = callMacroValueCONSTPRED(paths_, getPyFunc());
    if (bestCH[0] > bestCH[1])
    {
        aiEventActionBuffer.push_back("2");
    }
    else
    {
        aiEventActionBuffer.push_back("1");
    }
}
void gamestate::qEventApparitions()
{
    msi.createAllVars();
    vector<macroStateInfo> paths_;
    mapGen->discreteMapRep(getCurrentNode(), msi, paths_);
    vector<float> bestCH(2);

    macroStateInfo msi_opt(msi);
    for (int k{ 0 }; k < paths_.size(); ++k)
    {
        paths_[k].copyNonMapRep(msi_opt);
    }
    bestCH[0] = callMacroValueCONSTPRED(paths_, getPyFunc());

    msi_opt.playerVars[1] = apci.getPlayer().getMaxHealth() / 2;
    msi_opt.playerVars[0] = clamp(apci.getPlayer().getHealth(),1, apci.getPlayer().getMaxHealth() / 2);
    if (apci.checkForRelic("Toxic Egg")) msi_opt.deckCountPerCard[185] = 5;
    else { msi_opt.deckCountPerCard[184] = 5; }

    for (int k{ 0 }; k < paths_.size(); ++k)
    {
        paths_[k].copyNonMapRep(msi_opt);
    }
    bestCH[1] = callMacroValueCONSTPRED(paths_, getPyFunc());
    if (bestCH[0] > bestCH[1])
    {
        aiEventActionBuffer.push_back("2");
    }
    else
    {
        aiEventActionBuffer.push_back("1");
    }
}
void gamestate::qEventFalling()
{
    msi.createAllVars();
    vector<macroStateInfo> paths_;
    mapGen->discreteMapRep(getCurrentNode(), msi, paths_);
    vector<float> bestCH(3);

    macroStateInfo msi_opt(msi); 
    // skill
    int index = (apci.getDeck()[questionMark::storedValue1]->getCardIDNum() - 1) * 2 + apci.getDeck()[questionMark::storedValue1]->getUpgraded();
    --msi_opt.deckCountPerCard[index];
    for (int k{ 0 }; k < paths_.size(); ++k)
    {
        paths_[k].copyNonMapRep(msi_opt);
    }
    bestCH[0] = callMacroValueCONSTPRED(paths_, getPyFunc());
    ++msi_opt.deckCountPerCard[index];

    // power
    if (questionMark::storedValue2 >= 0)
    {
        index = (apci.getDeck()[questionMark::storedValue2]->getCardIDNum() - 1) * 2 + apci.getDeck()[questionMark::storedValue2]->getUpgraded();
        --msi_opt.deckCountPerCard[index];
        for (int k{ 0 }; k < paths_.size(); ++k)
        {
            paths_[k].copyNonMapRep(msi_opt);
        }
        bestCH[1] = callMacroValueCONSTPRED(paths_, getPyFunc());
        ++msi_opt.deckCountPerCard[index];
    }
    else
    {
        bestCH[1] = 0;
    }

    // attack
    index = (apci.getDeck()[questionMark::storedValue3]->getCardIDNum() - 1) * 2 + apci.getDeck()[questionMark::storedValue3]->getUpgraded();
    --msi_opt.deckCountPerCard[index];
    for (int k{ 0 }; k < paths_.size(); ++k)
    {
        paths_[k].copyNonMapRep(msi_opt);
    }
    bestCH[2] = callMacroValueCONSTPRED(paths_, getPyFunc());
    ++msi_opt.deckCountPerCard[index];


    if (bestCH[0] > bestCH[1] && bestCH[0] > bestCH[2])
    {
        aiEventActionBuffer.push_back("1");
    }
    else if(bestCH[1] > bestCH[2])
    {
        aiEventActionBuffer.push_back("2");
    }
    else
    {
        aiEventActionBuffer.push_back("3");
    }
}

void gamestate::qEvent1()
{
    msi.createAllVars();
    vector<macroStateInfo> paths_;
    mapGen->discreteMapRep(getCurrentNode(), msi, paths_);
    vector<float> bestCH(2);
    vector<macroStateInfo> msi_opts(2, msi);
    ++msi_opts[0].deckCountPerCard[182]; // 92-1 * 2
    ++msi_opts[1].relicPresent[133]; // 134 - 1
    int best = callMacroIndexREROLLPRED(msi_opts, getPyFunc());
    if (best == 1) ++best;
    aiEventActionBuffer.push_back(to_string(best+1));
}
void gamestate::qEvent2()
{
    msi.createAllVars();
    vector<macroStateInfo> paths_;
    mapGen->discreteMapRep(getCurrentNode(), msi, paths_);
    vector<float> bestCH(4);
    vector<macroStateInfo> msi_opts(4, msi);
    for (int i{ 0 }; i < 3; ++i) msi_opts[i].playerVars[0] -= 16;
    ++msi_opts[0].relicPresent[135]; // 136 - 1
    ++msi_opts[1].relicPresent[136]; // 137 - 1
    ++msi_opts[2].relicPresent[137]; // 138 - 1
    ++msi_opts[2].deckCountPerCard[158]; // 80-1 * 2
    int best = callMacroIndexREROLLPRED(msi_opts, getPyFunc());
    if (best == 3)
    {
        aiEventActionBuffer.push_back("2");
    }
    else
    {
        aiEventActionBuffer.push_back("1");
        aiEventActionBuffer.push_back("1");
    }
}
void gamestate::qEvent3()
{
    msi.createAllVars();
    vector<macroStateInfo> paths_;
    mapGen->discreteMapRep(getCurrentNode(), msi, paths_);
    vector<float> bestCH(2);

    macroStateInfo msi_opt(msi);
    for (int k{ 0 }; k < paths_.size(); ++k)
    {
        paths_[k].copyNonMapRep(msi_opt);
    }
    bestCH[0] = callMacroValueCONSTPRED(paths_, getPyFunc());

    msi_opt.playerVars[1] = apci.getPlayer().getMaxHealth() * 70 / 100;
    msi_opt.playerVars[0] = clamp(apci.getPlayer().getHealth(), 1, msi_opt.playerVars[1]);
    if (apci.checkForRelic("Molten Egg")) msi_opt.deckCountPerCard[181] = 5;
    else { msi_opt.deckCountPerCard[180] = 5; }
    msi_opt.deckCountPerCard[0] = 0;
    msi_opt.deckCountPerCard[1] = 0;
    for (int k{ 0 }; k < paths_.size(); ++k)
    {
        paths_[k].copyNonMapRep(msi_opt);
    }
    bestCH[1] = callMacroValueCONSTPRED(paths_, getPyFunc());
    if (bestCH[0] > bestCH[1])
    {
        aiEventActionBuffer.push_back("3");
    }
    else
    {
        aiEventActionBuffer.push_back("2");
    }
}