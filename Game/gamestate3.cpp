#include "gamestate.h"
#include "enemies.h"
#include "ic_cards.h"

using namespace std;

// ############# cards ##################
void gamestate::makeCardDatabase()
{
    // card database
    card_database.emplace("Strike_ID", make_unique<strike>(false));
    card_database.emplace("Defend_ID", make_unique<defend>(false));
    card_database.emplace("Bash_ID", make_unique<bash>(false));
    card_database.emplace("Armaments_ID", make_unique<armaments>(false));
    card_database.emplace("Anger_ID", make_unique<anger>(false));
    card_database.emplace("Bodyslam_ID", make_unique<bodyslam>(false));
    card_database.emplace("Clash_ID", make_unique<clash>(false));
    card_database.emplace("Cleave_ID", make_unique<cleave>(false));
    card_database.emplace("Clothesline_ID", make_unique<clothesline>(false));
    card_database.emplace("Flex_ID", make_unique<flex>(false));
    card_database.emplace("Headbutt_ID", make_unique<headbutt>(false));
    card_database.emplace("HeavyBlade_ID", make_unique<heavyBlade>(false));
    card_database.emplace("Havoc_ID", make_unique<havoc>(false));
    card_database.emplace("Ironwave_ID", make_unique<ironwave>(false));
    card_database.emplace("PerfectedStrike_ID", make_unique<perfectedStrike>(false));
    card_database.emplace("PommelStrike_ID", make_unique<pommelStrike>(false));
    card_database.emplace("ShrugItOff_ID", make_unique<shrugItOff>(false));
    card_database.emplace("SwordBoomerang_ID", make_unique<swordBoomerang>(false));
    card_database.emplace("Thunderclap_ID", make_unique<thunderclap>(false));
    card_database.emplace("TrueGrit_ID", make_unique<trueGrit>(false));
    card_database.emplace("TwinStrike_ID", make_unique<twinStrike>(false));
    card_database.emplace("WarCry_ID", make_unique<warCry>(false));
    card_database.emplace("WildStrike_ID", make_unique<wildStrike>(false));

    card_database.emplace("BattleTrance_ID", make_unique<battleTrance>(false));
    card_database.emplace("BloodForBlood_ID", make_unique<bloodForBlood>(false));
    card_database.emplace("Bloodletting_ID", make_unique<bloodletting>(false));
    card_database.emplace("BurningPact_ID", make_unique<burningPact>(false));
    card_database.emplace("Carnage_ID", make_unique<carnage>(false));
    card_database.emplace("Combust_ID", make_unique<combust>(false));
    card_database.emplace("DarkEmbrace_ID", make_unique<darkEmbrace>(false));
    card_database.emplace("Disarm_ID", make_unique<disarm>(false));
    card_database.emplace("Dropkick_ID", make_unique<dropkick>(false));
    card_database.emplace("DualWield_ID", make_unique<dualWield>(false));
    card_database.emplace("Entrench_ID", make_unique<entrench>(false));
    card_database.emplace("Evolve_ID", make_unique<evolve>(false));
    card_database.emplace("FeelNoPain_ID", make_unique<feelNoPain>(false));
    card_database.emplace("FireBreathing_ID", make_unique<fireBreathing>(false));
    card_database.emplace("FlameBarrier_ID", make_unique<flameBarrier>(false));
    card_database.emplace("GhostlyArmor_ID", make_unique<ghostlyArmor>(false));
    card_database.emplace("Hemokinesis_ID", make_unique<hemokinesis>(false));
    card_database.emplace("InfernalBlade_ID", make_unique<infernalBlade>(false));
    card_database.emplace("Inflame_ID", make_unique<inflame>(false));
    card_database.emplace("Intimidate_ID", make_unique<intimidate>(false));
    card_database.emplace("Metallicize_ID", make_unique<metallicize>(false));
    card_database.emplace("PowerThrough_ID", make_unique<powerThrough>(false));
    card_database.emplace("Pummel_ID", make_unique<pummel>(false));
    card_database.emplace("Rage_ID", make_unique<ragee>(false));
    card_database.emplace("Rampage_ID", make_unique<rampage>(false));
    card_database.emplace("RecklessCharge_ID", make_unique<recklessCharge>(false));
    card_database.emplace("Rupture_ID", make_unique<rupture>(false));
    card_database.emplace("SearingBlow_ID", make_unique<searingBlow>(false));
    card_database.emplace("SecondWind_ID", make_unique<secondWind>(false));
    card_database.emplace("SeeingRed_ID", make_unique<seeingRed>(false));
    card_database.emplace("Sentinel_ID", make_unique<sentinel>(false));
    card_database.emplace("SeverSoul_ID", make_unique<severSoul>(false));
    card_database.emplace("Shockwave_ID", make_unique<shockwave>(false));
    card_database.emplace("SpotWeakness_ID", make_unique<spotWeakness>(false));
    card_database.emplace("Uppercut_ID", make_unique<uppercut>(false));
    card_database.emplace("Whirlwind_ID", make_unique<whirlwind>(false));

    card_database.emplace("Barricade_ID", make_unique<barricade>(false));
    card_database.emplace("Berserk_ID", make_unique<berserk>(false));
    card_database.emplace("Bludgeon_ID", make_unique<bludgeon>(false));
    card_database.emplace("Brutality_ID", make_unique<brutality>(false));
    card_database.emplace("Corruption_ID", make_unique<corruption>(false));
    card_database.emplace("DemonForm_ID", make_unique<demonForm>(false));
    card_database.emplace("DoubleTap_ID", make_unique<doubleTap>(false));
    card_database.emplace("Exhume_ID", make_unique<exhume>(false));
    card_database.emplace("Feed_ID", make_unique<feed>(false));
    card_database.emplace("FiendFire_ID", make_unique<fiendFire>(false));
    card_database.emplace("Immolate_ID", make_unique<immolate>(false));
    card_database.emplace("Impervious_ID", make_unique<impervious>(false));
    card_database.emplace("Juggernaut_ID", make_unique<juggernaut>(false));
    card_database.emplace("LimitBreak_ID", make_unique<limitBreak>(false));
    card_database.emplace("Reaper_ID", make_unique<reaper>(false));
    card_database.emplace("Offering_ID", make_unique<offering>(false));

    card_database_curse.emplace("Regret_ID", make_unique<regret>());
    card_database_curse.emplace("Injury_ID", make_unique<injury>());
    card_database_curse.emplace("Pain_ID", make_unique<pain>());
    card_database_curse.emplace("Doubt_ID", make_unique<doubt>());
    card_database_curse.emplace("Parasite_ID", make_unique<parasite>());
    card_database_curse.emplace("Necronomicurse_ID", make_unique<necronomicurse>());
    card_database_curse.emplace("Clumsy_ID", make_unique<clumsy>());
    card_database_curse.emplace("CurseOfTheBell_ID", make_unique<curseOfTheBell>());
    card_database_curse.emplace("Decay_ID", make_unique<IC_CARDS_H::decay>());
    card_database_curse.emplace("Normality_ID", make_unique<normality>());
    card_database_curse.emplace("Writhe_ID", make_unique<writhe>());
    card_database_curse.emplace("Shame_ID", make_unique<shame>());

    card_database_status.emplace("Burn_ID", make_unique<burn>());
    card_database_status.emplace("Dazed_ID", make_unique<dazed>());
    card_database_status.emplace("Slime_ID", make_unique<slime>());
    card_database_status.emplace("Wound_ID", make_unique<wound>());
    card_database_status.emplace("Void_ID", make_unique<voidx>());

    //special
    card_database_spcl.emplace("Jax_ID", make_unique<jax>(false));
    card_database_spcl.emplace("Bite_ID", make_unique<bite>(false));
    card_database_spcl.emplace("Apparition_ID", make_unique<apparition>(false));
    card_database_spcl.emplace("RitualDagger_ID", make_unique<ritualDagger>(false));

    //colourless
    card_database_clr.emplace("BandageUp_ID", make_unique<bandageUp>(false));
    card_database_clr.emplace("Blind_ID", make_unique<blind>(false));
    card_database_clr.emplace("DarkShackles_ID", make_unique<darkShackles>(false));
    card_database_clr.emplace("DeepBreath_ID", make_unique<deepBreath>(false)); // shuffle dis + draw
    card_database_clr.emplace("Discovery_ID", make_unique<discovery>(false)); // choice 3 costs 0
    card_database_clr.emplace("DramaticEntrance_ID", make_unique<dramaticEntrance>(false)); 
    card_database_clr.emplace("Enlightenment_ID", make_unique<enlightenment>(false)); // hand costs 1
    card_database_clr.emplace("Finesse_ID", make_unique<finesse>(false));
    card_database_clr.emplace("FlashOfSteel_ID", make_unique<flashOfSteel>(false));
    card_database_clr.emplace("Forethought_ID", make_unique<forethought>(false)); // hand card(s) bottom of lib costs 0 
    card_database_clr.emplace("GoodInstincts_ID", make_unique<goodInstincts>(false));
    card_database_clr.emplace("Impatience_ID", make_unique<impatience>(false));
    card_database_clr.emplace("JackOfAllTrades_ID", make_unique<jackOfAllTrades>(false)); // rng colourless to hand
    card_database_clr.emplace("Madness_ID", make_unique<madness>(false));
    card_database_clr.emplace("MindBlast_ID", make_unique<mindBlast>(false));
    card_database_clr.emplace("Panacea_ID", make_unique<panacea>(false));
    card_database_clr.emplace("PanicButton_ID", make_unique<panicButton>(false));
    card_database_clr.emplace("Purity_ID", make_unique<purity>(false));
    card_database_clr.emplace("SwiftStrike_ID", make_unique<swiftStrike>(false));
    card_database_clr.emplace("Trip_ID", make_unique<trip>(false));

    card_database_clr.emplace("Apotheosis_ID", make_unique<apotheosis>(false));
    card_database_clr.emplace("Chrysalis_ID", make_unique<chrysalis>(false)); // 3 skills drawp cost 0
    card_database_clr.emplace("HandOfGreed_ID", make_unique<handOfGreed>(false));
    card_database_clr.emplace("Magnetism_ID", make_unique<magnetism>(false)); // rng colourless sot
    card_database_clr.emplace("MasterOfStrategy_ID", make_unique<masterOfStrategy>(false));
    card_database_clr.emplace("Mayhem_ID", make_unique<mayhem>(false));
    card_database_clr.emplace("Metamorphisis_ID", make_unique<metamorphisis>(false)); // 3 attacks drawp cost 0
    card_database_clr.emplace("Panache_ID", make_unique<panache>(false));
    card_database_clr.emplace("SadisticNature_ID", make_unique<sadisticNature>(false));
    card_database_clr.emplace("SecretTechnique_ID", make_unique<secretTechnique>(false)); // put skill
    card_database_clr.emplace("SecretWeapon_ID", make_unique<secretWeapon>(false)); // put attack
    card_database_clr.emplace("TheBomb_ID", make_unique<theBomb>(false));
    card_database_clr.emplace("ThinkingAhead_ID", make_unique<thinkingAhead>(false)); // warcry+
    card_database_clr.emplace("Transmutation_ID", make_unique<transmutation>(false)); // add x colourless cost 0 turn
    card_database_clr.emplace("Violence_ID", make_unique<violence>(false)); // 3 rng atts from drawp

}
unique_ptr<card> gamestate::getCardByID(const string& s)
{
    auto res = card_database.find(s);
    if(res!=card_database.end()) 
    {
        return res->second->cloneU();
    }
    else{throw runtime_error("cannot find card");}
}
unique_ptr<card> gamestate::getSpecialCardByID(const string& s)
{
    auto res = card_database_spcl.find(s);
    if (res != card_database_spcl.end())
    {
        return res->second->cloneU();
    }
    else { throw runtime_error("cannot find card"); }
}
unique_ptr<card> gamestate::getColourlessCardByID(const string& s)
{
    auto res = card_database_clr.find(s);
    if (res != card_database_clr.end())
    {
        return res->second->cloneU();
    }
    else { throw runtime_error("cannot find card"); }
}
unique_ptr<card> gamestate::getCurseCardByID(const string& s)
{
    auto res = card_database_curse.find(s);
    if (res != card_database_curse.end())
    {
        return res->second->cloneU();
    }
    else { throw runtime_error("cannot find card"); }
}
unique_ptr<card> gamestate::getStatusCardByID(const string& s)
{
    auto res = card_database_status.find(s);
    if (res != card_database_status.end())
    {
        return res->second->cloneU();
    }
    else { throw runtime_error("cannot find card"); }
}
card* gamestate::getCardPByID(const string& s)
{
    auto res = card_database.find(s);
    if(res!=card_database.end()) 
    {
        return res->second->clone();
    }
    else{throw runtime_error("cannot find card");}
}

// ############# enemies ##################
void gamestate::makeEnemyDatabase()
{
    // enemy database
    enemy_database.emplace("JawWorm_ID", make_unique<jawWorm>());
    enemy_database.emplace("Cultist_ID", make_unique<cultist>());
    enemy_database.emplace("RedLouse_ID", make_unique<redLouse>());
    enemy_database.emplace("GreenLouse_ID", make_unique<greenLouse>());

    enemy_database.emplace("AcidSlimeM_ID", make_unique<acidSlimeM>());
    enemy_database.emplace("SpikeSlimeM_ID", make_unique<spikeSlimeM>());
    enemy_database.emplace("AcidSlimeS_ID", make_unique<acidSlimeS>());
    enemy_database.emplace("SpikeSlimeS_ID", make_unique<spikeSlimeS>());
    enemy_database.emplace("AcidSlimeL_ID", make_unique<acidSlimeL>());
    enemy_database.emplace("SpikeSlimeL_ID", make_unique<spikeSlimeL>());

    enemy_database.emplace("BlueSlaver_ID", make_unique<blueSlaver>());
    enemy_database.emplace("RedSlaver_ID", make_unique<redSlaver>());
    enemy_database.emplace("FungiBeast_ID", make_unique<fungiBeast>());
    enemy_database.emplace("Looter_ID", make_unique<looter>());

    enemy_database.emplace("MadGremlin_ID", make_unique<madGremlin>());
    enemy_database.emplace("FatGremlin_ID", make_unique<fatGremlin>());
    enemy_database.emplace("SneakyGremlin_ID", make_unique<sneakyGremlin>());
    enemy_database.emplace("ShieldGremlin_ID", make_unique<shieldGremlin>());
    enemy_database.emplace("GremlinWizard_ID", make_unique<gremlinWizard>());

    enemy_database.emplace("GremlinNob_ID", make_unique<gremlinNob>());
    enemy_database.emplace("Lagavulin_ID", make_unique<lagavulin>());
    enemy_database.emplace("Sentry1_ID", make_unique<sentry1>());
    enemy_database.emplace("Sentry2_ID", make_unique<sentry2>());

    enemy_database.emplace("TheGuardian_ID", make_unique<theGuardian>());
    enemy_database.emplace("HexaGhost_ID", make_unique<hexaGhost>());
    enemy_database.emplace("SlimeBoss_ID", make_unique<slimeBoss>());

    // ACT 2
    enemy_database.emplace("SphericGuardian_ID", make_unique<sphericGuardian>());
    enemy_database.emplace("Chosen_ID", make_unique<theChosen>());
    enemy_database.emplace("ShelledParasite_ID", make_unique<shelledParasite>());
    enemy_database.emplace("Byrd_ID", make_unique<byrd>());
    enemy_database.emplace("Mugger_ID", make_unique<mugger>());

    enemy_database.emplace("SnakePlant_ID", make_unique<snakePlant>());
    enemy_database.emplace("Snecko_ID", make_unique<snecko>());
    enemy_database.emplace("Centurion_ID", make_unique<centurion>());
    enemy_database.emplace("Mystic_ID", make_unique<mystic>());

    enemy_database.emplace("BookOfStabbing_ID", make_unique<bookOfStabbing>());
    enemy_database.emplace("Taskmaster_ID", make_unique<taskmaster>());
    enemy_database.emplace("GremlinLeader_ID", make_unique<gremlinLeader>());

    enemy_database.emplace("TheChamp_ID", make_unique<theChamp>());
    enemy_database.emplace("TheCollector_ID", make_unique<theCollector>());
    enemy_database.emplace("TorchHead_ID", make_unique<torchHead>());
    enemy_database.emplace("BronzeAutomaton_ID", make_unique<bronzeAutomaton>());
    enemy_database.emplace("BronzeOrb_ID", make_unique<bronzeOrb>());

    enemy_database.emplace("Pointy_ID", make_unique<pointy>());
    enemy_database.emplace("Romeo_ID", make_unique<romeo>());
    enemy_database.emplace("Bear_ID", make_unique<bear>());

    // ACT 3
    enemy_database.emplace("Darkling_ID", make_unique<darkling>());
    enemy_database.emplace("OrbWalker_ID", make_unique<orbWalker>());
    enemy_database.emplace("Spiker_ID", make_unique<spiker>());
    enemy_database.emplace("Exploder_ID", make_unique<exploder>());
    enemy_database.emplace("Repulsor_ID", make_unique<repulsor>());

    enemy_database.emplace("TheMaw_ID", make_unique<theMaw>());
    enemy_database.emplace("SpireGrowth_ID", make_unique<spireGrowth>());
    enemy_database.emplace("Transient_ID", make_unique<transient>());
    enemy_database.emplace("WrithingMass_ID", make_unique<writhingMass>());

    enemy_database.emplace("GiantHead_ID", make_unique<giantHead>());
    enemy_database.emplace("Nemesis_ID", make_unique<nemesis>());
    enemy_database.emplace("Reptomancer_ID", make_unique<reptomancer>());
    enemy_database.emplace("Dagger_ID", make_unique<dagger>());

    enemy_database.emplace("Donu_ID", make_unique<donu>());
    enemy_database.emplace("Deca_ID", make_unique<deca>());
    enemy_database.emplace("AwakenedOne_ID", make_unique<awakenedOne>());
    enemy_database.emplace("TimeEater_ID", make_unique<timeEater>());

    // ACT 4
    enemy_database.emplace("SpireShield_ID", make_unique<spireShield>());
    enemy_database.emplace("SpireSword_ID", make_unique<spireSword>());

    enemy_database.emplace("CorruptHeart_ID", make_unique<corruptHeart>());
}
unique_ptr<nonpc> gamestate::getEnemyByID(const string& s)
{
    auto res = enemy_database.find(s);
    if(res!=enemy_database.end()) 
    {
        return res->second->clone();
    }
    else{throw runtime_error("cannot find enemy");}
}
string gamestate::getHighestHealthEnemyName()
{
    string res;
    int highest{ 0 };
    for (int i{ 0 }; i < enemies_.size(); ++i)
    {
        if (enemies_[i]->getHealth() > highest) {res = enemies_[i]->getName(); highest = enemies_[i]->getHealth();}
    }
    return res;
}
bool gamestate::checkEnemyDeath()
{
    int i{0};
    bool onlyMinions{ true };
    while(i<enemies_.size())
    {
        if (enemies_[i]->getHealth()==0) { return true; }
        if (!enemies_[i]->isMinion()) { onlyMinions = false; }
        ++i;
    }
    return onlyMinions;
}
void gamestate::handleEnemyDeath()
{
    int i{0};
    while(i<enemies_.size())
    {
        if(enemies_[i]->getHealth()==0)
        {
            //intents_[i].clear();
            intents_.erase(intents_.begin() + i);
            intents_.push_back({});
            enemies_.erase(enemies_.begin()+i);
            --i;
        }
        for (int j{ 0 }; j < enemies_.size(); ++j)
        {
            enemies_[j]->setSCRNindex(j);
        }
        //reshuffleIntents();
        if (enemies_.size() != 0) resetVisuals();
        ++i;
    }
    if (checkAllEnemyDeath()) enemies_.clear();
}
bool gamestate::checkAllEnemyDeath()
{
    if (getMapPathEvent() == 'B' && !dormant_enemies_[0].empty()) return false;
    int i{ 0 };
    while (i < enemies_.size())
    {
        if (enemies_[i]->getHealth() != 0 && !enemies_[i]->isMinion())
        {
            return false;
        }
        ++i;
    }
    return true;
}
void gamestate::startEnemyTurn()
{
    vector<int> uniqueIDS_;
    int num_enemy_ = static_cast<int>(enemies_.size());
    for (int i{ 0 }; i < enemies_.size(); ++i)
    {
        uniqueIDS_.push_back(enemies_[i]->getUniqueID());
    }
    // all enemies go once, dont go if dead, repeat to allow for reshuffles but not forcing no intents to go as cant
    int x{ 5 };
    while (--x >= 0)
    {
        for (int i{ 0 }; i < num_enemy_; ++i)
        {
            for (int j{ 0 }; j < uniqueIDS_.size(); ++j)
            {
                if (uniqueIDS_[j] == enemies_[i]->getUniqueID() && enemies_[i]->getHealth() > 0)
                {
                    uniqueIDS_.erase(uniqueIDS_.begin() + j);
                    if (enemies_[i]->getPastAttacks().size() != 0) enemies_[i]->EnactIntent();
                    if (enemies_.size() < num_enemy_) { --num_enemy_; --i; }
                }
            }
        }
    }
}

void gamestate::addDormantEnemy(const nonpc& original, int turns)
{
    dormant_enemies_[turns].push_back(original.clone());
}
void gamestate::reAddDormant()
{
    for (int i{ 0 }; i < dormant_enemies_[0].size(); ++i)
    {
        enemies_.push_back(dormant_enemies_[0][i]->clone());
        enemies_.back()->setSCRNindex(static_cast<int>(enemies_.size())-1);
        enemies_.back()->sotCleanup();
        enemies_.back()->returnFromDormant();
    }
    dormant_enemies_.erase(dormant_enemies_.begin());
    dormant_enemies_.push_back({});
}

void gamestate::loadEnemies(vector<nonpc*> vn)
{
    while (!enemies_.empty()) enemies_.pop_back();
    for (auto n : vn)
    {
        enemies_.push_back(unique_ptr<nonpc>(n));
    }
}
void gamestate::loadDormantEnemies(vector<vector<nonpc*>> vn)
{
    while (!dormant_enemies_.empty()) dormant_enemies_.pop_back();
    dormant_enemies_ = vector<vector<unique_ptr<nonpc>>>(3);
    for (int i{ 0 }; i < vn.size(); ++i)
    {
        for (int j{ 0 }; j < vn[i].size(); ++j)
        {
            dormant_enemies_[i].push_back(vn[i][j]->clone());
        }
    }
}
node* gamestate::getCurrentNode() 
{
    if (actNumber != 4) return mapGen->getNode(currentFloorIndex - ((actNumber - 1) * 17), currentWidthIndex);
    else 
    {
        return mapGen->getNode(16);
    }
}
int gamestate::getFloorIndex() const 
{
    if (actNumber != 4) return currentFloorIndex;
    else 
    {
        if (mapPath[mapPathIndex] == 'c') return 51;
        if (mapPath[mapPathIndex] == 's') return 52;
        if (mapPath[mapPathIndex] == 'E') return 53;
        if (mapPath[mapPathIndex] == 'B') return 54;
    }
}