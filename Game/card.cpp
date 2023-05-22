#include "card.h"
#include "pc_info.h"
#include "character.h"
#include "gamestate.h"
#include "effects_triggers.h"

using namespace std;

bool card::blueCandleCheck{ false };
bool card::medKitCheck{ false };
bool card::canPlayAttack{ true };
bool card::corruptionCheck{ false };
map<int, int> card::tierList{
    {1,1},{2,4},{3,2},{4,4},{5,5},{6,2},{7,1},{8,2},{9,1},{10,4},
    {11,3},{12,1},{13,3},{14,3},{15,4},{16,4},{17,3},{18,3},{19,3},{20,3},
    {21,4},{22,4},{23,5},{24,5},{25,4},{26,5},{27,3},{28,4},{29,5},{30,5},
    {31,4},{32,3},{33,2},{34,3},{35,5},{36,3},{37,5},{38,2},{39,5},{40,3},
    {41,3},{42,2},{43,3},{44,5},{45,3},{46,4},{47,2},{48,4},{49,1},{50,1},
    {51,5},{52,4},{53,4},{54,3},{55,5},{56,3},{57,4},{58,5},{59,2},{60,2},
    {61,2},{62,3},{63,5},{64,2},{65,3},{66,4},{67,5},{68,5},{69,5},{70,5},
    {71,3},{72,2},{73,5},{74,4},{75,0},{76,0},{77,0},{78,0},{79,0},{80,0},
    {81,0},{82,0},{83,0},{84,0},{85,0},{86,0}
};

// 5 anger,battletrance,bloodforblood,burningpact,corruption,darkembrace,diarm,
// 5 exhume,feed,feelnopain,fiendfire,hemo,immolate,impervious,ofering,powerthorugh
// 5 secondwind, showckwave,whirlind
//
// 4 armaments,bash,bloodletting,combust,droplick,evole,flamebarrier,havoc,
// 4 pommelstrike,ragee,reaper,recklesscharge,seeingred,sentinel,shruhitoff,
// 4 uppercut,warcry,wildstrike
//
// 3 brutality,carnage,defend,doubletap,dualwield,firebreathing,headbutt,infenalblade,inflame
// 3 ironwave,juggernat,metalliceize,perfectedstrike,pummel,seversoul,spotweakness,swordboomerang
// 3 thunderclap,truegrit,twinstrke,
// 
// 2 barricade,berserk,bodyslam,clotheslinmdemonform,entrenchm,ghostlyarmor,
// 2 intimidate,limitbreak,rampage,strike,bludgeon,
// 
// 1 clash,flex,heavybalde,rupture,cleave,seraingblow

void card::clonedObservers(observer_set obss)
{
    if (!observers_.empty()) throw runtime_error("shouldn't have observers yet");
    observers_ = obss;
}
card* card::safeClone()
{
    card* res = clone();
    if (observers_.empty()) return res;
    for (auto itr{ observers_.begin() }; itr != observers_.end(); ++itr)
    {
        auto temp = (**itr).clone(res);
        if (temp) res->observers_.insert(temp);
    }
    return res;
}
void card::fightStart(bool midFight)
{
    if (midFight) return;
    if (bottled_)
    {
        apci.sendCardFromDrawToHand(this);
        apci.getPlayer().incrementBottledCount();
    }
    else if (innate_)
    {
        apci.putDrawPileCardAtFront(this);
    }
}
int card::getEnergy() const 
{
    if (corruptionCheck && type_ == Skill) return clamp(temp_energy_, -1, 0);
    else if (zeroUntilPlayed) { return clamp(temp_energy_, -1, 0); }
    else if (oneUntilPlayed) { return clamp(temp_energy_, -1, 1); }
    else { return clamp(temp_energy_, -1, 9); }
}
bool card::castable()
{
    if (gm.getAllEnemies().empty() && (getCombatChoices() == combatChoices::EnemyTarget 
        || getCombatChoices() == combatChoices::Headbutt)) return false;
    bool res{ true };
    if (type_ == Attack) res = canPlayAttack;
    return res && !gamestate::denyCardPlay;
}
