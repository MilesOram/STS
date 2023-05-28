#include "enemies.h"
#include "gamestate.h"


using namespace std;

void jawWorm::chooseAndDeclareIntent()
{
    if (!firstAttackDone && gm.getActNumber() != 3)
    {
        constructIntentions("chomp", { Intents("Attack",11,1) });
        declaredIntent = &jawWorm::chomp;
        firstAttackDone = true;
        return;
    }
    else
    {
        int index = calcIntentOdds({ "bellow","thrash","chomp" }, { 45,30,25 }, { 1,2,1 });
        switch (index)
        {
        case 0:
            constructIntentions("bellow", { Intents("Block",6,1),Intents("Strength",3,1) });
            declaredIntent = &jawWorm::bellow;
            break;
        case 1:
            constructIntentions("thrash", { Intents("Attack",7,1), Intents("Block",5,1) });
            declaredIntent = &jawWorm::thrash;
            break;
        case 2:
            constructIntentions("chomp", { Intents("Attack",11,1) });
            declaredIntent = &jawWorm::chomp;
            break;
        }
    }
}
void jawWorm::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
void jawWorm::chomp()
{
    effects::enemyAdjustedDamage(*this,11);
    // effects::enemyAdjustedDamage(*this, 12);
}
void jawWorm::thrash()
{
    if (!effects::enemyAdjustedDamage(*this, 7))
        effects::Block(*this, 5);
}
void jawWorm::bellow()
{
    effects::Block(*this,6);
    gainStrength(3);
    // effects::Block(*this, 9);
    // gainStrength(5);
}
void jawWorm::startOfFight()
{
    if (gm.getActNumber() == 3)
    {
        bellow();
    }
    nonpc::startOfFight();
}
vector<int> jawWorm::giveIntent()
{
    if (declaredIntent == &jawWorm::chomp) { return { 1,0,0 }; }
    if (declaredIntent == &jawWorm::thrash) { return { 0,1,0 }; }
    if (declaredIntent == &jawWorm::bellow) { return { 0,0,1 }; }
    return { 0,0,0 };
}

void cultist::chooseAndDeclareIntent()
{
    if (!firstAttackDone)
    {
        declaredIntent = &cultist::incantation;
        firstAttackDone = true;
        pastAttacks.push_back("incantation");
        auto temp = { Intents("Ritual",ritualNumber,1) };
        gm.setIntents(temp, scrnIndex);
        return;
    }
    else
    {
        declaredIntent = &cultist::darkStrike;
        pastAttacks.push_back("darkStrike");
        auto temp = { Intents("Attack",6,1) };
        gm.setIntents(temp, scrnIndex);
        return;
    }
}
void cultist::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
void cultist::darkStrike()
{
    effects::enemyAdjustedDamage(*this, 6);
}
void cultist::incantation()
{
    ritual_ += ritualNumber;
    observeEvent<ritualTrigger<character>>(gm.getEnemyTurnEnd(), new ritualTrigger<character>(this, gm.getEnemyTurnEnd()));
    loseStrength(ritualNumber);
    addAltBuffs(Intents("Rit", ritual_, 1));
}
void redLouse::chooseAndDeclareIntent()
{
    if (attackNumber == 0)
    {
        attackNumber = random_int(5, 7);
        shieldNumber = random_int(3, 7);
        addAltBuffs(Intents("SHLD", shieldNumber, 1));
    }
    if ((random_int(1, 4) == 1 || checkLastXAttacks(pastAttacks, 2, "bite")) && !checkLastXAttacks(pastAttacks, 2, "grow"))
    {
        declaredIntent = &redLouse::grow;
        pastAttacks.push_back("grow");
        auto temp = { Intents("Strength",3,1) };
        gm.setIntents(temp, scrnIndex);
        return;
    }
    else
    {
        declaredIntent = &redLouse::bite;
        pastAttacks.push_back("bite");
        auto temp = { Intents("Attack",attackNumber,1) };
        gm.setIntents(temp, scrnIndex);
        return;
    }
}
void redLouse::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
void redLouse::bite()
{
    effects::enemyAdjustedDamage(*this, attackNumber);
}
void redLouse::grow()
{
    effects::Strength(*this, 3);
}
bool redLouse::takeDamage(int amount, character& initiator)
{
    int temp_health{ health_ };
    if (character::takeDamage(amount, initiator)) return true;
    if (shieldNumber == 0) return false;
    amount = temp_health - health_;
    if (amount > 0)
    {
        auto trig = new louseArmourTrigger<character>(this, gm.getAttackPlayed(), shieldNumber);
        auto haltTrig = new haltObservingTriggerCH<character>(this, gm.getAttackPlayed(), trig);
        observeEvent<louseArmourTrigger<character>>(gm.getAttackPlayed(), trig);
        observeEvent<haltObservingTriggerCH<character>>(gm.getAttackPlayed(), haltTrig);
        shieldNumber = 0;
        addAltBuffs(Intents("SHLD", 0, 1));
    }
    return false;
}
void greenLouse::chooseAndDeclareIntent()
{
    if (attackNumber == 0)
    {
        attackNumber = random_int(5, 7);
        shieldNumber = random_int(3, 7);
        addAltBuffs(Intents("SHLD", shieldNumber, 1));
    }
    if ((random_int(1, 4) == 1 || checkLastXAttacks(pastAttacks, 2, "bite")) && !checkLastXAttacks(pastAttacks, 2, "spitWeb"))
    {
        declaredIntent = &greenLouse::spitWeb;
        pastAttacks.push_back("spitWeb");
        auto temp = { Intents("Weak",3,1) };
        gm.setIntents(temp, scrnIndex);
        return;
    }
    else
    {
        declaredIntent = &greenLouse::bite;
        pastAttacks.push_back("bite");
        auto temp = { Intents("Attack",attackNumber,1) };
        gm.setIntents(temp, scrnIndex);
        return;
    }
}
void greenLouse::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
void greenLouse::bite()
{
    effects::enemyAdjustedDamage(*this, attackNumber);
}
void greenLouse::spitWeb()
{
    effects::Weak(apci.getPlayer(), 3);
}
bool greenLouse::takeDamage(int amount, character& initiator)
{
    int temp_health{ health_ };
    if (character::takeDamage(amount, initiator)) return true;
    if (shieldNumber == 0) return false;
    amount = temp_health - health_;
    if (amount > 0)
    {
        auto trig = new louseArmourTrigger<character>(this, gm.getAttackPlayed(), shieldNumber);
        auto haltTrig = new haltObservingTriggerCH<character>(this, gm.getAttackPlayed(), trig);
        observeEvent<louseArmourTrigger<character>>(gm.getAttackPlayed(), trig);
        observeEvent<haltObservingTriggerCH<character>>(gm.getAttackPlayed(), haltTrig);
        shieldNumber = 0;
        addAltBuffs(Intents("SHLD", 0, 1));
    }
    return false;
}
void acidSlimeM::chooseAndDeclareIntent()
{
    int index = calcIntentOdds({ "corrosiveSpit","tackle","lick" }, { 30,40,30 }, { 2,1,2 });
    switch (index)
    {
    case 0:
        constructIntentions("corrosiveSpit", { Intents("Attack",7,1),Intents("Slime",1,1) });
        declaredIntent = &acidSlimeM::corrosiveSpit;
        break;
    case 1:
        constructIntentions("tackle", { Intents("Attack",attackNumber,1) });
        declaredIntent = &acidSlimeM::tackle;
        break;
    case 2:
        constructIntentions("lick", { Intents("Weak",1,1) });
        declaredIntent = &acidSlimeM::lick;
        break;
    }
}
void acidSlimeM::corrosiveSpit()
{
    effects::enemyAdjustedDamage(*this, 7);
    apci.addCardToDiscard(cardHolder(gm.getStatusCardByID("Slime_ID")->clone()));
}
void acidSlimeM::lick()
{
    effects::Weak(apci.getPlayer(), 1);
}
void acidSlimeM::tackle()
{
    effects::enemyAdjustedDamage(*this, attackNumber);
}
void acidSlimeM::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}

void spikeSlimeM::chooseAndDeclareIntent()
{
    int index = calcIntentOdds({ "lick","flameTackle" }, { 70,30 }, { 2,2 });
    switch (index)
    {
    case 0:
        constructIntentions("lick", { Intents("Frail",1,1) });
        declaredIntent = &spikeSlimeM::lick;
        break;
    case 1:
        constructIntentions("flameTackle", { Intents("Attack",attackNumber,1),Intents("Slime",1,1) });
        declaredIntent = &spikeSlimeM::flameTackle;
        break;
    }
}
void spikeSlimeM::lick()
{
    effects::Frail(apci.getPlayer(), 1);
}
void spikeSlimeM::flameTackle()
{
    effects::enemyAdjustedDamage(*this, attackNumber);
    apci.addCardToDiscard(cardHolder(gm.getStatusCardByID("Slime_ID")->clone()));
}
void spikeSlimeM::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}

void acidSlimeS::chooseAndDeclareIntent()
{
    bool t{true};
    if (!firstAttackDone) { t = random_int(0, 1); firstAttackDone = true; }
    else {t = (*(pastAttacks.end() - 1) == "lick");}

    if (t)
    {
        constructIntentions("tackle", { Intents("Attack",attackNumber,1) });
        declaredIntent = &acidSlimeS::tackle;
    }
    else
    {
        constructIntentions("lick", { Intents("Weak",1,1) });
        declaredIntent = &acidSlimeS::lick;
    }
}

void acidSlimeS::lick()
{
    effects::Weak(apci.getPlayer(), 1);
}
void acidSlimeS::tackle()
{
    effects::enemyAdjustedDamage(*this, attackNumber);
}
void acidSlimeS::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}

void spikeSlimeS::tackle()
{
    effects::enemyAdjustedDamage(*this, attackNumber);
}
void spikeSlimeS::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
void spikeSlimeS::chooseAndDeclareIntent()
{
    constructIntentions("tackle", { Intents("Attack",attackNumber,1) });
    declaredIntent = &spikeSlimeS::tackle;
}

void blueSlaver::stab()
{
    effects::enemyAdjustedDamage(*this, attackNumber);
}
void blueSlaver::rake()
{
    effects::enemyAdjustedDamage(*this, 7);
    effects::Weak(apci.getPlayer(), 1);
}
void blueSlaver::chooseAndDeclareIntent()
{
    int index = calcIntentOdds({ "rake","stab" }, { 40,60 }, { 2,2 });
    switch (index)
    {
    case 0:
        constructIntentions("rake", { Intents("Attack",7,1),Intents("Weak",1,1) });
        declaredIntent = &blueSlaver::rake;
        break;
    case 1:
        constructIntentions("stab", { Intents("Attack",attackNumber,1) });
        declaredIntent = &blueSlaver::stab;
        break;
    }
}
void blueSlaver::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}

void redSlaver::stab()
{
    effects::enemyAdjustedDamage(*this, attackNumber);
}
void redSlaver::scrape()
{
    effects::enemyAdjustedDamage(*this, 8);
    effects::Vulnerable(apci.getPlayer(), 1);
}
void redSlaver::entangle()
{
    effects::Entangle(apci.getPlayer(), 1);
}
void redSlaver::chooseAndDeclareIntent()
{
    if (!firstAttackDone)
    {
        constructIntentions("stab", { Intents("Attack",attackNumber,1) });
        declaredIntent = &redSlaver::stab;
        firstAttackDone = true;
    }
    else
    {
        if (!entangled_ && random_int(1, 4) == 1)
        {
            constructIntentions("entangle", { Intents("Entangle",1000,1) });
            declaredIntent = &redSlaver::entangle;
            entangled_ = true;
        }
        else
        {
            if (entangled_)
            {
                int index = calcIntentOdds({ "scrape","stab" }, { 55,45 }, { 2,2 });
                switch (index)
                {
                case 0:
                    constructIntentions("scrape", { Intents("Attack",8,1),Intents("Vulnerable",1,1) });
                    declaredIntent = &redSlaver::scrape;
                    break;
                case 1:
                    constructIntentions("stab", { Intents("Attack",attackNumber,1) });
                    declaredIntent = &redSlaver::stab;
                    break;
                }
            }
            else
            {
                if (*(pastAttacks.end() - 1) == "stab" || (*(pastAttacks.end() - 1) == "scrape" && pastAttacks.size() > 1 && *(pastAttacks.end() - 2) == "stab"))
                {
                    constructIntentions("scrape", { Intents("Attack",8,1),Intents("Vulnerable",1,1) });
                    declaredIntent = &redSlaver::scrape;
                }
                else
                {
                    constructIntentions("stab", { Intents("Attack",attackNumber,1) });
                    declaredIntent = &redSlaver::stab;
                }
            }
        }
    }

}
void redSlaver::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
void fungiBeast::bite()
{
    effects::enemyAdjustedDamage(*this, attackNumber);
}
void fungiBeast::grow()
{
    effects::Strength(*this, 3);
}
void fungiBeast::chooseAndDeclareIntent()
{
    int index = calcIntentOdds({ "bite","grow" }, { 60,40 }, { 2,1 });
    switch (index)
    {
    case 0:
        constructIntentions("bite", { Intents("Attack",attackNumber,1) });
        declaredIntent = &fungiBeast::bite;
        break;
    case 1:
        constructIntentions("grow", { Intents("Strength",3,1) });
        declaredIntent = &fungiBeast::grow;
        break;
    }
}
void fungiBeast::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
void fungiBeast::startOfFight()
{
    addAltBuffs(Intents("Spore Cloud", 2, 1));
    observeEvent<vulnerableTrigger<character>>(gm.getEnemyDied(), new vulnerableTrigger<character>(this, gm.getEnemyDied(), 2));
    nonpc::startOfFight();
}

void looter::mug()
{
    effects::enemyAdjustedDamage(*this, attackNumber);
    goldStolen += apci.getGold() < thieveryNumber ? apci.getGold() : thieveryNumber;
    apci.loseGold(thieveryNumber);
    removeObserverIfFE<thiefTrigger<character>>(gm.getEnemyDied());
    observeEvent<thiefTrigger<character>>(gm.getEnemyDied(), new thiefTrigger<character>(this, gm.getEnemyDied(), goldStolen));
}
void looter::lunge()
{
    effects::enemyAdjustedDamage(*this, 12);
    goldStolen += apci.getGold() < thieveryNumber ? apci.getGold() : thieveryNumber;
    apci.loseGold(thieveryNumber);
    removeObserverIfFE<thiefTrigger<character>>(gm.getEnemyDied());
    observeEvent<thiefTrigger<character>>(gm.getEnemyDied(), new thiefTrigger<character>(this, gm.getEnemyDied(), goldStolen));
}
void looter::startOfFight()
{
    addAltBuffs(Intents("THIEF", thieveryNumber, 1));
    nonpc::startOfFight();
}
void looter::smokeBomb()
{
    effects::Block(*this, 6);
}
void looter::escape()
{
    gm.removeFromCombat(this);
}
void looter::chooseAndDeclareIntent()
{
    if (!firstAttackDone || pastAttacks.size() == 1)
    {
        constructIntentions("mug", { Intents("Attack",attackNumber,1) });
        declaredIntent = &looter::mug;
        firstAttackDone = true;
    }
    else if (checkLastXAttacks(pastAttacks, 2, "mug"))
    {
        if (random_int(0, 1))
        {
            constructIntentions("lunge", { Intents("Attack",12,1) });
            declaredIntent = &looter::lunge;
        }
        else
        {
            constructIntentions("smokeBomb", { Intents("Block",6,1) });
            declaredIntent = &looter::smokeBomb;
        }

    }
    else if (*(pastAttacks.end() - 1) == "lunge")
    {
        constructIntentions("smokeBomb", { Intents("Block",6,1) });
        declaredIntent = &looter::smokeBomb;
    }
    else
    {
        constructIntentions("escape", { Intents("Escaping",1000,1) });
        declaredIntent = &looter::escape;
    }
}
void looter::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
// elites
void gremlinNob::chooseAndDeclareIntent()
{
    if (!firstAttackDone)
    {
        declaredIntent = &gremlinNob::bellow;
        firstAttackDone = true;
        pastAttacks.push_back("bellow");
        auto temp = { Intents("Rage",2,1) };
        gm.setIntents(temp, scrnIndex);
        return;
    }
    else
    {
        //int oneInTwenty;
        if (checkLastXAttacks(pastAttacks, 2, "rush"))
        {

            declaredIntent = &gremlinNob::skullBash;
            auto temp = { Intents("Attack",8,1),Intents("Vulnerable",2,1) };
            gm.setIntents(temp, scrnIndex);
            pastAttacks.push_back("skullBash");
        }
        else
        {
            int oneInThree = random_int(1, 3);
            if (oneInThree == 1)
            {
                declaredIntent = &gremlinNob::rush;
                auto temp = { Intents("Attack",16,1) };
                gm.setIntents(temp, scrnIndex);
                pastAttacks.push_back("rush");
            }
            else
            {
                declaredIntent = &gremlinNob::skullBash;
                auto temp = { Intents("Attack",8,1),Intents("Vulnerable",2,1) };
                gm.setIntents(temp, scrnIndex);
                pastAttacks.push_back("skullBash");
            }
        }
    }
}
void gremlinNob::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
void gremlinNob::bellow()
{
    enrage_ += 2;
    addAltBuffs(Intents("Enrage", enrage_, 1));
    observeEvent<rageTrigger<character>>(gm.getSkillPlayed(), new rageTrigger<character>(this, gm.getSkillPlayed()));
    // enrage_+=3;
}
void gremlinNob::rush()
{
    // effects::enemyAdjustedDamage(*this,14);
    effects::enemyAdjustedDamage(*this, 16);
}
void gremlinNob::skullBash()
{
    // effects::enemyAdjustedDamage(*this,6);
    if (!effects::enemyAdjustedDamage(*this, 8))
        effects::Vulnerable(apci.getPlayer(), 2);
}

void sentry1::beam() { effects::enemyAdjustedDamage(*this, attackNumber); }
void sentry1::bolt() {
    apci.addCardToDiscard(cardHolder(gm.getStatusCardByID("Dazed_ID")->clone()));
    apci.addCardToDiscard(cardHolder(gm.getStatusCardByID("Dazed_ID")->clone()));
}
void sentry1::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
void sentry1::startOfFight() 
{
    gainArtifactCharge(1); 
    nonpc::startOfFight();
}
void sentry2::beam() { effects::enemyAdjustedDamage(*this, attackNumber); }
void sentry2::bolt() {
    apci.addCardToDiscard(cardHolder(gm.getStatusCardByID("Dazed_ID")->clone()));
    apci.addCardToDiscard(cardHolder(gm.getStatusCardByID("Dazed_ID")->clone()));
}
void sentry2::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
void sentry2::startOfFight() 
{
    gainArtifactCharge(1); 
    nonpc::startOfFight();
}
void sentry1::chooseAndDeclareIntent()
{
    if (!firstAttackDone)
    {
        constructIntentions("bolt", { Intents("Dazed",2,1) });
        declaredIntent = &sentry1::bolt;
        firstAttackDone = true;
    }
    else if (*(pastAttacks.end() - 1) == "beam")
    {
        constructIntentions("bolt", { Intents("Dazed",2,1) });
        declaredIntent = &sentry1::bolt;
    }
    else
    {
        constructIntentions("beam", { Intents("Attack",attackNumber,1) });
        declaredIntent = &sentry1::beam;
    }
}
void sentry2::chooseAndDeclareIntent()
{
    if (!firstAttackDone)
    {
        constructIntentions("beam", { Intents("Attack",attackNumber,1) });
        declaredIntent = &sentry2::beam;
        firstAttackDone = true;
    }
    else if (*(pastAttacks.end() - 1) == "beam")
    {
        constructIntentions("bolt", { Intents("Dazed",2,1) });
        declaredIntent = &sentry2::bolt;
    }
    else
    {
        constructIntentions("beam", { Intents("Attack",attackNumber,1) });
        declaredIntent = &sentry2::beam;
    }
}



void lagavulin::chooseAndDeclareIntent()
{
    if (!awoken)
    {
        if (checkLastXAttacks(pastAttacks, 3, "asleep"))
        {
            block_ -= metallicise_;
            wokeUp();
            declaredIntent = &lagavulin::baseAttack;
            pastAttacks.push_back("baseAttack");
            auto temp = { Intents("Attack",20,1) };
            gm.setIntents(temp, scrnIndex);
            return;
        }
        declaredIntent = &lagavulin::asleep;
        pastAttacks.push_back("asleep");
        auto temp = { Intents("Asleep",1000,1) };
        gm.setIntents(temp, scrnIndex);
        return;
    }
    else
    {
        if (checkLastXAttacks(pastAttacks, 2, "baseAttack"))
        {
            declaredIntent = &lagavulin::siphonSoul;
            pastAttacks.push_back("siphonSoul");
            auto temp = { Intents("Lose Dexterity",2,1),Intents("Lose Strength",2,1) };
            gm.setIntents(temp, scrnIndex);
        }
        else
        {
            declaredIntent = &lagavulin::baseAttack;
            pastAttacks.push_back("baseAttack");
            auto temp = { Intents("Attack",20,1) };
            gm.setIntents(temp, scrnIndex);
        }

    }
}
void lagavulin::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
void lagavulin::baseAttack()
{
    effects::enemyAdjustedDamage(*this, 20);
}
void lagavulin::siphonSoul()
{
    effects::Dexterity(apci.getPlayer(), -2);
    effects::Strength(apci.getPlayer(), -2);
}
void lagavulin::asleep() {}
void lagavulin::wokeUp()
{
    awoken = true;
    metallicise_ = 0;
    addAltBuffs(Intents("MTL", 0, 1));
}
bool lagavulin::takeDamage(int amount, character& initiator)
{
    int temp_health{ health_ };
    if (character::takeDamage(amount, initiator)) return true;
    if (awoken) return false;
    amount = temp_health - health_;
    if (amount > 0)
    {
        wokeUp();
        auto temp = { Intents("Awoken",1000,1) };
        gm.setIntents(temp, scrnIndex);
    }
    return false;
}
bool lagavulin::takeDamageNoTrigger(int amount)
{
    int temp_health{ health_ };
    if (character::takeDamageNoTrigger(amount)) return true;
    if (awoken) return false;
    amount = temp_health - health_;
    if (amount > 0)
    {
        wokeUp();
        auto temp = { Intents("Awoken",1000,1) };
        gm.setIntents(temp, scrnIndex);
    }
    return false;
}

void theGuardian::chooseAndDeclareIntent()
{
    if (!firstAttackDone)
    {
        offensiveMode();
        declaredIntent = &theGuardian::chargingUp;
        firstAttackDone = true;
        pastAttacks.push_back("chargingUp");
        auto temp = { Intents("Block",9,1) };
        gm.setIntents(temp, scrnIndex);
        return;
    }
    else if (*(pastAttacks.end() - 1) == "chargingUp")
    {
        pastAttacks.push_back("fierceBash");
        declaredIntent = &theGuardian::fierceBash;
        auto temp = { Intents("Attack",32,1) };
        gm.setIntents(temp, scrnIndex);
    }
    else if (*(pastAttacks.end() - 1) == "fierceBash")
    {
        pastAttacks.push_back("ventSteam");
        declaredIntent = &theGuardian::ventSteam;
        auto temp = { Intents("Vulnerable",2,1),Intents("Weak",2,1) };
        gm.setIntents(temp, scrnIndex);
    }
    else if (*(pastAttacks.end() - 1) == "ventSteam")
    {
        pastAttacks.push_back("whirlwind");
        declaredIntent = &theGuardian::whirlwind;
        auto temp = { Intents("Attack",5,4) };
        gm.setIntents(temp, scrnIndex);
    }
    else if (*(pastAttacks.end() - 1) == "whirlwind")
    {
        pastAttacks.push_back("chargingUp");
        declaredIntent = &theGuardian::chargingUp;
        auto temp = { Intents("Block",9,1) };
        gm.setIntents(temp, scrnIndex);
    }
    else if (*(pastAttacks.end() - 1) == "defensiveMode")
    {
        pastAttacks.push_back("roll");
        declaredIntent = &theGuardian::roll;
        auto temp = { Intents("Attack",9,1) };
        gm.setIntents(temp, scrnIndex);
    }
    else if (*(pastAttacks.end() - 1) == "roll")
    {
        pastAttacks.push_back("twinSlam");
        declaredIntent = &theGuardian::twinSlam;
        auto temp = { Intents("Attack",8,2),Intents("OffensiveMode",1000,1) };
        gm.setIntents(temp, scrnIndex);
    }
    else if (*(pastAttacks.end() - 1) == "twinSlam")
    {
        pastAttacks.push_back("whirlwind");
        declaredIntent = &theGuardian::whirlwind;
        auto temp = { Intents("Attack",5,4) };
        gm.setIntents(temp, scrnIndex);
    }
    else { throw runtime_error("guardian attack error"); }
}
void theGuardian::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
void theGuardian::offensiveMode()
{
    defMode = false;
    addAltBuffs(Intents("Off", 1000, 1));
    addAltBuffs(Intents("Def", 0, 1));
    currentModeShift = modeShift;
    addAltBuffs(Intents("modeShift", currentModeShift, 1));
    addAltBuffs(Intents("sharpHide", 0, 1));
    sharp_hide_ = 0;
    removeObserver<sharpHideTrigger<character>>();
}
void theGuardian::chargingUp()
{
    effects::Block(*this, 9);
}
void theGuardian::fierceBash()
{
    effects::enemyAdjustedDamage(*this, 32);
    //effects::enemyAdjustedDamage(apci.getPlayer(),36);
}
void theGuardian::ventSteam()
{
    effects::Vulnerable(apci.getPlayer(), 2);
    effects::Weak(apci.getPlayer(), 2);
}
void theGuardian::whirlwind()
{
    effects::singleTargetMulti(*this, apci.getPlayer(), 5, 4);
}
void theGuardian::defensiveMode()
{
    defMode = true;
    modeShift += 10;
    addAltBuffs(Intents("Def", 1000, 1));
    addAltBuffs(Intents("Off", 0, 1));
    addAltBuffs(Intents("modeShift", 0, 1));
    effects::Block(*this, 20);
    declaredIntent = &theGuardian::gainSharpHide;
    auto temp = { Intents("sharpHide",3,1) };
    gm.setIntents(temp, scrnIndex);
    pastAttacks.push_back("defensiveMode");
}
void theGuardian::roll()
{
    effects::enemyAdjustedDamage(*this, 9);
    //effects::enemyAdjustedDamage(*this,10);
}
void theGuardian::twinSlam()
{
    if (!effects::singleTargetMulti(*this, apci.getPlayer(), 8, 2)) offensiveMode();
}
void theGuardian::gainSharpHide()
{
    sharp_hide_ = 3;
    observeEvent<sharpHideTrigger<character>>(gm.getAttackPlayed(), new sharpHideTrigger<character>(this, gm.getAttackPlayed()));
    addAltBuffs(Intents("sharpHide", 3, 1));
    //sharp_hide_=4;
}
bool theGuardian::takeDamage(int amount, character& initiator)
{
    int temp_health{ health_ };
    bool died = character::takeDamage(amount, initiator);
    if (died) return true;
    amount = temp_health - health_;
    if (defMode) return false;
    currentModeShift = clamp(currentModeShift - amount, 0, 99);
    if (currentModeShift == 0) defensiveMode();
    else
    {
        addAltBuffs(Intents("modeShift", currentModeShift, 1));
    }
    return false;
}
bool theGuardian::takeDamageNoTrigger(int amount)
{
    int temp_health{ health_ };
    bool died = character::takeDamageNoTrigger(amount);
    if (died) return true;
    amount = temp_health - health_;
    if (defMode) return false;
    currentModeShift = clamp(currentModeShift - amount, 0, 99);
    if (currentModeShift == 0) defensiveMode();
    else
    {
        addAltBuffs(Intents("modeShift", currentModeShift, 1));
    }
    return false;
}

void hexaGhost::calcDivider()
{
    dividerDamage = (apci.getPlayer().getHealth() / 12) + 1;
}
void hexaGhost::chooseAndDeclareIntent()
{
    if (!firstAttackDone)
    {
        declaredIntent = &hexaGhost::activate;
        firstAttackDone = true;
        constructIntentions("activate", { Intents("???",1000,1) });
        return;
    }
    else if (*(pastAttacks.end() - 1) == "activate")
    {
        calcDivider();
        declaredIntent = &hexaGhost::divider;
        constructIntentions("divider", { Intents("Attack",dividerDamage,6) }, { Intents("Flames 6/6",1000,1) });
    }
    else
    {
        switch (attackIndex)
        {
        case 0:
        {
            constructIntentions("sear", { Intents("Attack",6,1),Intents("Burn",1,1) }, { Intents("Flames 6/6",0,1),Intents("Flames 0/6",1000,1) });
            declaredIntent = &hexaGhost::sear;
            ++attackIndex;
            break;
        }
        case 1:
        {
            constructIntentions("tackle", { Intents("Attack",5,2) }, { Intents("Flames 0/6",0,1),Intents("Flames 1/6",1000,1) });
            declaredIntent = &hexaGhost::tackle;
            ++attackIndex;
            break;
        }
        case 2:
        {
            constructIntentions("sear", { Intents("Attack",6,1),Intents("Burn",1,1) }, { Intents("Flames 1/6",0,1),Intents("Flames 2/6",1000,1) });
            declaredIntent = &hexaGhost::sear;
            ++attackIndex;
            break;
        }
        case 3:
        {
            constructIntentions("inflame", { Intents("Strength",2,1),Intents("Block",12,1) }, { Intents("Flames 2/6",0,1),Intents("Flames 3/6",1000,1) });
            declaredIntent = &hexaGhost::inflame;
            ++attackIndex;
            break;
        }
        case 4:
        {
            constructIntentions("tackle", { Intents("Attack",5,2) }, { Intents("Flames 3/6",0,1),Intents("Flames 4/6",1000,1) });
            declaredIntent = &hexaGhost::tackle;
            ++attackIndex;
            break;
        }
        case 5:
        {
            constructIntentions("sear", { Intents("Attack",6,1),Intents("Burn",1,1) }, { Intents("Flames 4/6",0,1),Intents("Flames 5/6",1000,1) });
            declaredIntent = &hexaGhost::sear;
            ++attackIndex;
            break;
        }
        case 6:
        {
            constructIntentions("inferno", { Intents("Attack",2,6),Intents("Burn+",3,1) }, { Intents("Flames 5/6",0,1),Intents("Flames 6/6",1000,1) });
            declaredIntent = &hexaGhost::inferno;
            attackIndex = 0;
            break;
        }
        }
    }
}
void hexaGhost::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
void hexaGhost::activate() {}
void hexaGhost::divider()
{
    effects::singleTargetMulti(*this, apci.getPlayer(), dividerDamage, 6);
}
void hexaGhost::sear()
{
    effects::enemyAdjustedDamage(*this, 6);
    apci.addCardToDiscard(cardHolder(gm.getStatusCardByID("Burn_ID")->clone()));
}
void hexaGhost::tackle()
{
    effects::singleTargetMulti(*this, apci.getPlayer(), 5, 2);
}
void hexaGhost::inflame()
{
    gainStrength(2);
    gainBlock(12);
}
void hexaGhost::inferno()
{
    effects::singleTargetMulti(*this, apci.getPlayer(), 2, 6);
    apci.addCardToDiscard(cardHolder(gm.getStatusCardByID("Burn_ID")->clone()));
    apci.addCardToDiscard(cardHolder(gm.getStatusCardByID("Burn_ID")->clone()));
    apci.addCardToDiscard(cardHolder(gm.getStatusCardByID("Burn_ID")->clone()));
    for (int i{ 0 }; i < apci.getHand().size(); ++i)
    {
        (*apci.getHand()[i]).upgradeStatus();
    }
    for (int i{ 0 }; i < apci.getDiscardSize(); ++i)
    {
        (*apci.getDiscardPile()[i]).upgradeStatus();
    }
    for (int i{ 0 }; i < apci.getDrawSize(); ++i)
    {
        (*apci.getDrawPile()[i]).upgradeStatus();
    }
}

bool madGremlin::isMinion() const { return gm.getMapPathEvent() == 'E'; }
bool fatGremlin::isMinion() const { return gm.getMapPathEvent() == 'E'; }
bool shieldGremlin::isMinion() const { return gm.getMapPathEvent() == 'E'; }
bool sneakyGremlin::isMinion() const { return gm.getMapPathEvent() == 'E'; }
bool gremlinWizard::isMinion() const { return gm.getMapPathEvent() == 'E'; }
void madGremlin::startOfFight()
{
    addAltBuffs(Intents("Angry", 2, 1));
    observeEvent<strengthTrigger<character>>(gm.getEnemyTakingAttackDamage(), new strengthTrigger<character>(this, gm.getEnemyTakingAttackDamage(), 2));
    nonpc::startOfFight();
}
void madGremlin::scratch()
{
    effects::enemyAdjustedDamage(*this, 4);
}
void madGremlin::chooseAndDeclareIntent()
{
    constructIntentions("scratch", { Intents("Attack",4,1) });
    declaredIntent = &madGremlin::scratch;
}
void madGremlin::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}


void fatGremlin::smash()
{
    effects::Weak(apci.getPlayer(), 1);
    effects::enemyAdjustedDamage(*this, 4);
}
void fatGremlin::chooseAndDeclareIntent()
{
    constructIntentions("smash", { Intents("Attack",4,1),Intents("Weak",1,1) });
    declaredIntent = &fatGremlin::smash;
}
void fatGremlin::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}


void shieldGremlin::protect()
{
    if (gm.getAllEnemies().size() == 1) effects::Block(*this, 7);
    else
    {
        int rng = random_intNot(0, static_cast<int>(gm.getAllEnemies().size()) - 1, scrnIndex);
        effects::Block(*gm.getAllEnemies()[rng].get(), 7);
    }
}
void shieldGremlin::shieldBash()
{
    effects::enemyAdjustedDamage(*this, 6);
}
void shieldGremlin::chooseAndDeclareIntent()
{
    if (gm.getAllEnemies().size() >= 1)
    {
        constructIntentions("protect", { Intents("Block",7,1) });
        declaredIntent = &shieldGremlin::protect;
    }
    else
    {
        constructIntentions("shieldBash", { Intents("Attack",6,1) });
        declaredIntent = &shieldGremlin::shieldBash;
    }
}
void shieldGremlin::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}


void sneakyGremlin::puncture()
{
    effects::enemyAdjustedDamage(*this, 9);
}
void sneakyGremlin::chooseAndDeclareIntent()
{
    constructIntentions("puncture", { Intents("Attack",9,1) });
    declaredIntent = &sneakyGremlin::puncture;
}
void sneakyGremlin::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}


void gremlinWizard::charging() {}
void gremlinWizard::ultimateBlast()
{
    effects::enemyAdjustedDamage(*this, 25);
}
void gremlinWizard::chooseAndDeclareIntent()
{
    if (chargeCount > 0)
    {
        constructIntentions("charging", { Intents("???",1000,1) });
        declaredIntent = &gremlinWizard::charging;
        --chargeCount;
    }
    else
    {
        constructIntentions("ultimateBlast", { Intents("Attack",25,1) });
        declaredIntent = &gremlinWizard::ultimateBlast;
        chargeCount = 3;
    }
}
void gremlinWizard::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}


void acidSlimeL::chooseAndDeclareIntent()
{
    int index = calcIntentOdds({ "corrosiveSpit","tackle","lick" }, { 30,40,30 }, { 2,1,2 });
    switch (index)
    {
    case 0:
        constructIntentions("corrosiveSpit", { Intents("Attack",11,1),Intents("Slime",2,1) });
        declaredIntent = &acidSlimeL::corrosiveSpit;
        break;
    case 1:
        constructIntentions("tackle", { Intents("Attack",16,1) });
        declaredIntent = &acidSlimeL::tackle;
        break;
    case 2:
        constructIntentions("lick", { Intents("Weak",2,1) });
        declaredIntent = &acidSlimeL::lick;
        break;
    }
}
void acidSlimeL::corrosiveSpit()
{
    effects::enemyAdjustedDamage(*this, 11);
    apci.addCardToDiscard(cardHolder(gm.getStatusCardByID("Slime_ID")->clone()));
    apci.addCardToDiscard(cardHolder(gm.getStatusCardByID("Slime_ID")->clone()));
}
void acidSlimeL::lick()
{
    effects::Weak(apci.getPlayer(), 2);
}
void acidSlimeL::tackle()
{
    effects::enemyAdjustedDamage(*this, 16);
}
void acidSlimeL::split()
{
    gm.addEnemiesByID("AcidSlimeM_ID", static_cast<int>(gm.getAllEnemies().size()),true);
    gm.getAllEnemies().back()->setHealth(health_);
    gm.getAllEnemies().back()->setMaxHealth(health_);
    gm.addEnemiesByID("AcidSlimeM_ID", static_cast<int>(gm.getAllEnemies().size()),true);
    gm.getAllEnemies().back()->setHealth(health_);
    gm.getAllEnemies().back()->setMaxHealth(health_);
    gm.removeFromCombat(this);
}
void acidSlimeL::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
bool acidSlimeL::takeDamage(int amount, character& initiator)
{
    bool died = character::takeDamage(amount, initiator);
    if (died) return true;
    if (declaredIntent == &acidSlimeL::split) return false;
    if (health_ <= max_health_ / 2)
    {
        constructIntentions("split", { Intents("Split",1000,1) });
        declaredIntent = &acidSlimeL::split;
    }
    return false;
}
bool acidSlimeL::takeDamageNoTrigger(int amount)
{
    bool died = character::takeDamageNoTrigger(amount);
    if (died) return true;
    if (declaredIntent == &acidSlimeL::split) return false;
    if (health_ <= max_health_ / 2)
    {
        constructIntentions("split", { Intents("Split",1000,1) });
        declaredIntent = &acidSlimeL::split;
    }
    return false;
}


void spikeSlimeL::chooseAndDeclareIntent()
{
    int index = calcIntentOdds({ "flameTackle","lick" }, { 30,70 }, { 2,2 });
    switch (index)
    {
    case 0:
        constructIntentions("flameTackle", { Intents("Attack",16,1),Intents("Slime",2,1) });
        declaredIntent = &spikeSlimeL::flameTackle;
        break;
    case 1:
        constructIntentions("lick", { Intents("Weak",2,1) });
        declaredIntent = &spikeSlimeL::lick;
        break;
    }
}
void spikeSlimeL::flameTackle()
{
    effects::enemyAdjustedDamage(*this, 16);
    apci.addCardToDiscard(cardHolder(gm.getStatusCardByID("Slime_ID")->clone()));
    apci.addCardToDiscard(cardHolder(gm.getStatusCardByID("Slime_ID")->clone()));
}
void spikeSlimeL::lick()
{
    effects::Frail(apci.getPlayer(), 2);
}
void spikeSlimeL::split()
{
    gm.addEnemiesByID("SpikeSlimeM_ID", static_cast<int>(gm.getAllEnemies().size()),true);
    gm.getAllEnemies().back()->setHealth(health_);
    gm.getAllEnemies().back()->setMaxHealth(health_);
    gm.addEnemiesByID("SpikeSlimeM_ID", static_cast<int>(gm.getAllEnemies().size()),true);
    gm.getAllEnemies().back()->setHealth(health_);
    gm.getAllEnemies().back()->setMaxHealth(health_);
    gm.removeFromCombat(this);
}
void spikeSlimeL::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
bool spikeSlimeL::takeDamage(int amount, character& initiator)
{
    bool died = character::takeDamage(amount, initiator);
    if (died) return true;
    if (declaredIntent == &spikeSlimeL::split) return false;
    if (health_ <= max_health_ / 2)
    {
        constructIntentions("split", { Intents("Split",1000,1) });
        declaredIntent = &spikeSlimeL::split;
    }
    return false;
}
bool spikeSlimeL::takeDamageNoTrigger(int amount)
{
    bool died = character::takeDamageNoTrigger(amount);
    if (died) return true;
    if (declaredIntent == &spikeSlimeL::split) return false;
    if (health_ <= max_health_ / 2)
    {
        constructIntentions("split", { Intents("Split",1000,1) });
        declaredIntent = &spikeSlimeL::split;
    }
    return false;
}


void slimeBoss::chooseAndDeclareIntent()
{
    if (pastAttacks.size() == 0 || pastAttacks.back() == "slam")
    {
        constructIntentions("goopSpray", { Intents("Slime",3,1) });
        declaredIntent = &slimeBoss::goopSpray;
    }
    else if (pastAttacks.back() == "goopSpray")
    {
        constructIntentions("preparing", { Intents("???",1000,1) });
        declaredIntent = &slimeBoss::preparing;
    }
    else
    {
        constructIntentions("slam", { Intents("Attack",35,1) });
        declaredIntent = &slimeBoss::slam;
    }
}
void slimeBoss::goopSpray()
{
    apci.addCardToDiscard(cardHolder(gm.getStatusCardByID("Slime_ID")->clone()));
    apci.addCardToDiscard(cardHolder(gm.getStatusCardByID("Slime_ID")->clone()));
    apci.addCardToDiscard(cardHolder(gm.getStatusCardByID("Slime_ID")->clone()));
}
void slimeBoss::preparing() {}
void slimeBoss::slam()
{
    effects::enemyAdjustedDamage(*this, 35);
}
void slimeBoss::split()
{
    gm.addEnemiesByID("SpikeSlimeL_ID", static_cast<int>(gm.getAllEnemies().size()),true);
    gm.getAllEnemies().back()->setHealth(health_);
    gm.getAllEnemies().back()->setMaxHealth(health_);
    gm.addEnemiesByID("AcidSlimeL_ID", static_cast<int>(gm.getAllEnemies().size()),true);
    gm.getAllEnemies().back()->setHealth(health_);
    gm.getAllEnemies().back()->setMaxHealth(health_);
    gm.removeFromCombat(this);
}
void slimeBoss::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
bool slimeBoss::takeDamage(int amount, character& initiator)
{
    bool died = character::takeDamage(amount, initiator);
    if (died) return true;
    if (declaredIntent == &slimeBoss::split) return false;
    if (health_ <= max_health_ / 2)
    {
        constructIntentions("split", { Intents("Split",1000,1) });
        declaredIntent = &slimeBoss::split;
    }
    return false;
}
bool slimeBoss::takeDamageNoTrigger(int amount)
{
    bool died = character::takeDamageNoTrigger(amount);
    if (died) return true;
    if (declaredIntent == &slimeBoss::split) return false;
    if (health_ <= max_health_ / 2)
    {
        constructIntentions("split", { Intents("Split",1000,1) });
        declaredIntent = &slimeBoss::split;
    }
    return false;
}

void byrd::chooseAndDeclareIntent()
{
    if(flying>0) flying = 3;
    addAltBuffs({Intents("Flying",flying,1)});
    if(!firstAttackDone)
    {
        if(random_int(1,8) >= 6)
        {
            constructIntentions("caw", { Intents("Strength",1,1) });
            declaredIntent = &byrd::caw;
        }
        else
        {
            constructIntentions("peck", { Intents("Attack",1,5) });
            declaredIntent = &byrd::peck;
        }
        firstAttackDone = true;
    }
    else
    {
        if(flying == 0)
        {
            if ( pastAttacks.back() == "stunned")
            {
                constructIntentions("headbutt", { Intents("Attack",3,1) });
                declaredIntent = &byrd::headbutt;
            }
            else if (pastAttacks.back() == "headbutt")
            {
                constructIntentions("fly", { Intents("Fly",1000,1) });
                declaredIntent = &byrd::fly;
            }
        }
        else
        {
            int index = calcIntentOdds({ "peck","caw","swoop" }, { 50,30,20 }, { 2,1,1 });
            switch (index)
            {
            case 0:
                constructIntentions("peck", { Intents("Attack",1,5) });
                declaredIntent = &byrd::peck;
                break;
            case 1:
                constructIntentions("caw", { Intents("Strength",1,1) });
                declaredIntent = &byrd::caw;
                break;
            case 2:
                constructIntentions("swoop", { Intents("Attack",12,1) });
                declaredIntent = &byrd::swoop;
                break;
            }
        }
    }
}
void byrd::caw()
{
    effects::Strength(*this,1);
}
void byrd::peck() 
{
    effects::singleTargetMulti(*this, apci.getPlayer(), 1, 5);
}
void byrd::swoop()
{
    effects::enemyAdjustedDamage(*this, 12);
}
void byrd::fly()
{
    flying=3;
    addAltBuffs({Intents("Flying",flying,1)});
}
void byrd::headbutt()
{
    effects::enemyAdjustedDamage(*this, 3);
}
void byrd::stunned() {}
void byrd::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
bool byrd::takeDamage(int amount, character& initiator)
{
    if(flying>0) amount -= amount/2;
    bool died = character::takeDamage(amount, initiator);
    if (died) return true;
    if (declaredIntent == &byrd::stunned) return false;
    flying = clamp(flying-1,0,9);
    addAltBuffs({Intents("Flying",flying,1)});
    if (flying == 0)
    {
        constructIntentions("stunned", { Intents("Stunned",1000,1) });
        declaredIntent = &byrd::stunned;
    }
    return false;
}


void mugger::mug()
{
    effects::enemyAdjustedDamage(*this, 10);
    goldStolen += apci.getGold() < thieveryNumber ? apci.getGold() : thieveryNumber;
    apci.loseGold(thieveryNumber);
    removeObserverIfFE<thiefTrigger<character>>(gm.getEnemyDied());
    observeEvent<thiefTrigger<character>>(gm.getEnemyDied(), new thiefTrigger<character>(this, gm.getEnemyDied(), goldStolen));
}
void mugger::lunge()
{
    effects::enemyAdjustedDamage(*this, 16);
    goldStolen += apci.getGold() < thieveryNumber ? apci.getGold() : thieveryNumber;
    apci.loseGold(thieveryNumber);
    removeObserverIfFE<thiefTrigger<character>>(gm.getEnemyDied());
    observeEvent<thiefTrigger<character>>(gm.getEnemyDied(), new thiefTrigger<character>(this, gm.getEnemyDied(), goldStolen));
}
void mugger::startOfFight()
{
    addAltBuffs(Intents("THIEF", thieveryNumber, 1));
    nonpc::startOfFight();
}
void mugger::smokeBomb()
{
    effects::Block(*this, 11);
}
void mugger::escape()
{
    gm.removeFromCombat(this);
}
void mugger::chooseAndDeclareIntent()
{
    if (!firstAttackDone || pastAttacks.size() == 1)
    {
        constructIntentions("mug", { Intents("Attack",10,1) });
        declaredIntent = &mugger::mug;
        firstAttackDone = true;
    }
    else if (checkLastXAttacks(pastAttacks, 2, "mug"))
    {
        if (random_int(0, 1))
        {
            constructIntentions("lunge", { Intents("Attack",16,1) });
            declaredIntent = &mugger::lunge;
        }
        else
        {
            constructIntentions("smokeBomb", { Intents("Block",11,1) });
            declaredIntent = &mugger::smokeBomb;
        }

    }
    else if (*(pastAttacks.end() - 1) == "lunge")
    {
        constructIntentions("smokeBomb", { Intents("Block",11,1) });
        declaredIntent = &mugger::smokeBomb;
    }
    else
    {
        constructIntentions("escape", { Intents("Escaping",1000,1) });
        declaredIntent = &mugger::escape;
    }
}
void mugger::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}


void shelledParasite::suck()
{
    int healthPool = apci.getPlayer().getHealth();
    effects::enemyAdjustedDamage(*this, 10);
    healthPool -= apci.getPlayer().getHealth();
    if (healthPool > 0 && health_!=0) heal(healthPool);
}
void shelledParasite::fell()
{
    effects::Frail(apci.getPlayer(),2);
    effects::enemyAdjustedDamage(*this, 18);
}
void shelledParasite::doubleStrike()
{
    effects::singleTargetMulti(*this, apci.getPlayer(), 6, 2);
}
void shelledParasite::chooseAndDeclareIntent()
{
    if (!firstAttackDone)
    {
        if(random_int(0,1)==1)
        {
            constructIntentions("suck", { Intents("Attack",10,1), Intents("Life Steal",1000,1)});
            declaredIntent = &shelledParasite::suck;
        }
        else
        {
            constructIntentions("doubleStrike", { Intents("Attack",6,2) });
            declaredIntent = &shelledParasite::doubleStrike;
        }
        firstAttackDone = true;
    }
    else
    {
        int index = calcIntentOdds({ "doubleStrike","suck","fell" }, { 40,40,20 }, { 2,2,1 });
        switch (index)
        {
        case 0:
            constructIntentions("doubleStrike", { Intents("Attack",6,2) });
            declaredIntent = &shelledParasite::doubleStrike;
            break;
        case 1:
            constructIntentions("suck", { Intents("Attack",10,1), Intents("Life Steal",1000,1)});
            declaredIntent = &shelledParasite::suck;
            break;
        case 2:
            constructIntentions("fell", { Intents("Attack",18,1), Intents("Frail",2,1) });
            declaredIntent = &shelledParasite::fell;
            break;
        }
    }
}
void shelledParasite::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
bool shelledParasite::takeDamage(int amount, character& initiator)
{
    bool died = character::takeDamage(amount, initiator);
    if (died) return true;
    if (declaredIntent == &shelledParasite::stunned) return false;
    if (plated_armour_ == 0)
    {
        constructIntentions("stunned", { Intents("Stunned",1000,1) });
        declaredIntent = &shelledParasite::stunned;
    }
    return false;
}
bool shelledParasite::takeDamageNoTrigger(int amount)
{
    bool died = character::takeDamageNoTrigger(amount);
    if (died) return true;
    if (declaredIntent == &shelledParasite::stunned) return false;
    if (plated_armour_ == 0)
    {
        constructIntentions("stunned", { Intents("Stunned",1000,1) });
        declaredIntent = &shelledParasite::stunned;
    }
    return false;
}
void shelledParasite::startOfFight()
{
    addPlatedArmour(14);
    block_ += 14;
    nonpc::startOfFight();
}


void theChosen::poke()
{
    effects::singleTargetMulti(*this, apci.getPlayer(), 5, 2);
}
void theChosen::zap()
{
    effects::enemyAdjustedDamage(*this, 18);
}
void theChosen::debilitate()
{
    if(effects::enemyAdjustedDamage(*this, 10)) return;
    effects::Vulnerable(apci.getPlayer(), 2);
}
void theChosen::drain()
{
    effects::Weak(apci.getPlayer(), 3);
    effects::Strength(*this, 3);
}
void theChosen::hex()
{
    apci.getPlayer().applyHexed();
}
void theChosen::chooseAndDeclareIntent()
{
    if (!firstAttackDone)
    {
        constructIntentions("poke", { Intents("Attack",5,2) });
        declaredIntent = &theChosen::poke;
        firstAttackDone = true;
    }
    else if (*(pastAttacks.end() - 1) == "poke" && pastAttacks.size() == 1)
    {
        constructIntentions("hex", { Intents("Hex",1000,1) });
        declaredIntent = &theChosen::hex;
    }
    else
    {
        if (pastAttacks.size() % 2 == 0)
        {
            if (random_int(0, 1))
            {
                constructIntentions("debilitate", { Intents("Attack",10,1), Intents("Vulnerable",2,1)});
                declaredIntent = &theChosen::debilitate;
            }
            else
            {
                constructIntentions("drain", { Intents("Strength",3,1), Intents("Weak",3,1) });
                declaredIntent = &theChosen::drain;
            }
        }
        else
        {
            if (random_int(1, 10) <= 6)
            {
                constructIntentions("poke", { Intents("Attack",5,2) });
                declaredIntent = &theChosen::poke;
            }
            else
            {
                constructIntentions("zap", { Intents("Attack",18,1) });
                declaredIntent = &theChosen::zap;
            }
        }
    }
}
void theChosen::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}


void sphericGuardian::slam()
{
    effects::singleTargetMulti(*this, apci.getPlayer(), 10, 2);
}
void sphericGuardian::activate()
{
    effects::Block(*this, 25);
}
void sphericGuardian::harden()
{
    if(effects::enemyAdjustedDamage(*this, 10)) return;
    effects::Block(*this, 15);
}
void sphericGuardian::attdeb()
{
    if(effects::enemyAdjustedDamage(*this, 10)) return;
    effects::Frail(apci.getPlayer(), 5);
}
void sphericGuardian::chooseAndDeclareIntent()
{
    if (!firstAttackDone)
    {
        constructIntentions("activate", { Intents("Block",25,1) });
        declaredIntent = &sphericGuardian::activate;
        firstAttackDone = true;
    }
    else if(pastAttacks.back() == "activate")
    {
        constructIntentions("attdeb", { Intents("Attack",10,1), Intents("Frail",5,1) });
        declaredIntent = &sphericGuardian::attdeb;
    }
    else if(pastAttacks.back() == "attdeb")
    {
        if(random_int(0,1) == 1)
        {
            constructIntentions("harden", { Intents("Attack",10,1), Intents("Block",15,1) });
            declaredIntent = &sphericGuardian::harden;
        }
        else
        {
            constructIntentions("slam", { Intents("Attack",10,2) });
            declaredIntent = &sphericGuardian::slam;
        }
    }
    else if(pastAttacks.back() == "harden")
    {
        constructIntentions("slam", { Intents("Attack",10,2) });
        declaredIntent = &sphericGuardian::slam;
    }
    else
    {
        constructIntentions("harden", { Intents("Attack",10,1), Intents("Block",15,1) });
        declaredIntent = &sphericGuardian::harden;
    }
}
void sphericGuardian::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
void sphericGuardian::startOfFight()
{
    block_ = 40;
    nonpc::startOfFight();
    gainArtifactCharge(3);
    sotBlockObs->getObserving()->removeObserver(sotBlockObs);
    delete sotBlockObs;
    sotBlockObs = new barricadeTrigger<character>(this, gm.getEnemyTurnStart());
    gm.getEnemyTurnStart()->addObserver(sotBlockObs);
    addAltBuffs({ Intents("BAR",1000,1) });
}

void snakePlant::chomp()
{
    effects::singleTargetMulti(*this, apci.getPlayer(), 7, 3);
}
void snakePlant::enfeeblingSpores()
{
    effects::Weak(apci.getPlayer(), 2);
    effects::Frail(apci.getPlayer(), 2);
}
void snakePlant::chooseAndDeclareIntent()
{
    int index = calcIntentOdds({ "chomp","enfeeblingSpores" }, { 65,35 }, { 2,1 });
    switch (index)
    {
    case 0:
        constructIntentions("chomp", { Intents("Attack",7,3) });
        declaredIntent = &snakePlant::chomp;
        break;
    case 1:
        constructIntentions("enfeeblingSpores", { Intents("Weak",2,1), Intents("Frail",2,1) });
        declaredIntent = &snakePlant::enfeeblingSpores;
        break;
    }
}
void snakePlant::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
void snakePlant::startOfFight()
{
    sofMalleable(3);
    nonpc::startOfFight();
}


void snecko::bite()
{
    effects::enemyAdjustedDamage(*this, 15);
}
void snecko::tailWhip()
{
    if (effects::enemyAdjustedDamage(*this, 8)) return;
    effects::Vulnerable(apci.getPlayer(), 2);
}
void snecko::perplexingGlare()
{
    apci.getPlayer().applyConfused();
}
void snecko::chooseAndDeclareIntent()
{
    if (!firstAttackDone)
    {
        constructIntentions("perplexingGlare", { Intents("Confuse",1000,1) });
        declaredIntent = &snecko::perplexingGlare;
        firstAttackDone = true;
    }
    else
    {
        int index = calcIntentOdds({ "bite","tailWhip" }, { 60,40 }, { 2,10 });
        switch (index)
        {
        case 0:
            constructIntentions("bite", { Intents("Attack",15,1) });
            declaredIntent = &snecko::bite;
            break;
        case 1:
            constructIntentions("tailWhip", { Intents("Attack",8,1), Intents("Vulnerable",2,1) });
            declaredIntent = &snecko::tailWhip;
            break;
        }
    }
}
void snecko::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}


void centurion::slash()
{
    effects::enemyAdjustedDamage(*this, 12);
}
void centurion::fury()
{
    effects::singleTargetMulti(*this, apci.getPlayer(), 6, 3);
}
void centurion::defend()
{
    if (gm.getAllEnemies().size() > 1 && gm.getAllEnemies()[1]->getName() == "Mystic")
    {
        effects::Block(*gm.getAllEnemies()[1], 15);
    }
    else
    {
        mysticGone = true;
        effects::Block(*this, 15);
    }
}
void centurion::chooseAndDeclareIntent()
{
    if (!mysticGone)
    {
        int index = calcIntentOdds({ "slash","defend" }, { 35,65 }, { 10,10 });
        switch (index)
        {
        case 0:
            constructIntentions("slash", { Intents("Attack",12,1) });
            declaredIntent = &centurion::slash;
            break;
        case 1:
            constructIntentions("defend", { Intents("Block",15,1) });
            declaredIntent = &centurion::defend;
            break;
        }
    }
    else
    {
        int index = calcIntentOdds({ "slash","fury" }, { 35,65 }, { 10,10 });
        switch (index)
        {
        case 0:
            constructIntentions("slash", { Intents("Attack",12,1) });
            declaredIntent = &centurion::slash;
            break;
        case 1:
            constructIntentions("fury", { Intents("Attack",6,3) });
            declaredIntent = &centurion::fury;
            break;
        }
    }
    
}
void centurion::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}


void mystic::heal__()
{
    for_each(gm.getAllEnemies().begin(), gm.getAllEnemies().end(), [](unique_ptr<nonpc>& e) {e->heal(16); });
}
void mystic::buff()
{
    for_each(gm.getAllEnemies().begin(), gm.getAllEnemies().end(), [](unique_ptr<nonpc>& e) {e->gainStrength(2); });
}
void mystic::attdeb()
{
    if (effects::enemyAdjustedDamage(*this, 8)) return;
    effects::Frail(apci.getPlayer(), 2);
}
void mystic::chooseAndDeclareIntent()
{
    bool _heal_{ false };
    for (int i{ 0 }; i < gm.getAllEnemies().size(); ++i)
    {
        if (gm.getAllEnemies()[i]->getMaxHealth() - gm.getAllEnemies()[i]->getHealth() >= 16) _heal_ = true;
    }
    if (_heal_ && !checkLastXAttacks(pastAttacks,2,"heal"))
    {
        constructIntentions("heal", { Intents("Heal",16,1) });
        declaredIntent = &mystic::heal__;
    }
    else
    {
        int index = calcIntentOdds({ "buff","attdeb" }, { 40,60 }, { 2,2 });
        switch (index)
        {
        case 0:
            constructIntentions("buff", { Intents("Strength",2,1) });
            declaredIntent = &mystic::buff;
            break;
        case 1:
            constructIntentions("attdeb", { Intents("Attack",8,1), Intents("Frail",2,1) });
            declaredIntent = &mystic::attdeb;
            break;
        }
    }
}
void mystic::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}


void bookOfStabbing::multiStab()
{
    auto trig = new painStabTrigger(gm.getPlayerHealthLoss());
    observeEvent<painStabTrigger>(gm.getPlayerHealthLoss(), trig);
    effects::singleTargetMulti(*this, apci.getPlayer(), 6, multiCount);
    ++multiCount;
    removeObserver(trig);
}
void bookOfStabbing::singleStab()
{
    int plHP = apci.getPlayer().getHealth();
    effects::enemyAdjustedDamage(*this, 21);
    if(apci.getPlayer().getHealth() < plHP) apci.addCardToDiscard(cardHolder(gm.getStatusCardByID("Wound_ID")->clone()));
}
void bookOfStabbing::chooseAndDeclareIntent()
{
    int index = calcIntentOdds({ "multiStab","singleStab" }, { 85,15 }, { 2,1 });
    switch (index)
    {
    case 0:
        constructIntentions("multiStab", { Intents("Attack",6,multiCount) });
        declaredIntent = &bookOfStabbing::multiStab;
        break;
    case 1:
        constructIntentions("singleStab", { Intents("Attack",21,1) });
        declaredIntent = &bookOfStabbing::singleStab;
        break;
    }
}
void bookOfStabbing::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
void bookOfStabbing::startOfFight()
{
    painful_stabs_ = true;
    addAltBuffs(Intents("PainStab", 1000, 1));
    nonpc::startOfFight();
}


void gremlinLeader::encourage()
{
    for_each(gm.getAllEnemies().begin(), gm.getAllEnemies().end(), [&](unique_ptr<nonpc>& e)
        {e->gainStrength(3); if (e->getName() != this->getName()) e->gainBlock(6); });
}
void gremlinLeader::rally()
{
    vector<string> gremlinIDs{"MadGremlin_ID","FatGremlin_ID","SneakyGremlin_ID","ShieldGremlin_ID","GremlinWizard_ID"};
    gm.addEnemyFrontByIDMidFight(gremlinIDs[random_int(0, static_cast<int>(gremlinIDs.size()) - 1)]);
    gm.addEnemyFrontByIDMidFight(gremlinIDs[random_int(0, static_cast<int>(gremlinIDs.size()) - 1)]);
}
void gremlinLeader::stab()
{
    effects::singleTargetMulti(*this, apci.getPlayer(), 6, 3);
}
void gremlinLeader::chooseAndDeclareIntent()
{
    if (gm.getAllEnemies().size() == 1)
    {
        int index = calcIntentOdds({ "rally","stab" }, { 75,25 }, { 1,1 });
        switch (index)
        {
        case 0:
            constructIntentions("rally", { Intents("Rally",2,1) });
            declaredIntent = &gremlinLeader::rally;
            break; 
        case 1:
            constructIntentions("stab", { Intents("Attack",6,3) });
            declaredIntent = &gremlinLeader::stab;
            break;
        }
    }
    else if (gm.getAllEnemies().size() == 2)
    {
        if (pastAttacks.back() == "encourage")
        {
            int index = calcIntentOdds({ "stab","rally" }, { 50,50 }, { 1,1 });
            switch (index)
            {
            case 0:
                constructIntentions("stab", { Intents("Attack",6,3) });
                declaredIntent = &gremlinLeader::stab;
                break;
            case 1:
                constructIntentions("rally", { Intents("Rally",2,1) });
                declaredIntent = &gremlinLeader::rally;
                break;
            }
        }
        else
        {
            int index = calcIntentOdds({ "rally","encourage" }, { 63,37 }, { 1,1 });
            switch (index)
            {
            case 0:
                constructIntentions("rally", { Intents("Rally",2,1) });
                declaredIntent = &gremlinLeader::rally;
                break;
            case 1:
                constructIntentions("encourage", { Intents("Strength",3,1), Intents("Block OTR",6,1) });
                declaredIntent = &gremlinLeader::encourage;
                break;
            }
        }
    }
    else
    {
        int index = calcIntentOdds({ "stab","encourage" }, { 34,66 }, { 1,1 });
        switch (index)
        {
        case 0:
            constructIntentions("stab", { Intents("Attack",6,3) });
            declaredIntent = &gremlinLeader::stab;
            break;
        case 1:
            constructIntentions("encourage", { Intents("Strength",3,1), Intents("Block OTR",6,1) });
            declaredIntent = &gremlinLeader::encourage;
            break;
        }
    }
}
void gremlinLeader::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}


void taskmaster::scouringWhip()
{
    if(effects::enemyAdjustedDamage(*this, 7)) return;
    apci.addCardToDiscard(cardHolder(gm.getStatusCardByID("Wound_ID")->clone()));
}
void taskmaster::chooseAndDeclareIntent()
{
    constructIntentions("scouringWhip", { Intents("Attack",7,1), Intents("Wound",1,1) });
    declaredIntent = &taskmaster::scouringWhip;
}
void taskmaster::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}


void theChamp::defensiveStance()
{
    effects::Block(*this, 15);
    metallicise_ += 5;
    addAltBuffs(Intents("MTL", metallicise_, 1));
}
void theChamp::faceSlap()
{
    if(effects::enemyAdjustedDamage(*this, 12)) return;
    effects::Vulnerable(apci.getPlayer(), 2);
    effects::Frail(apci.getPlayer(), 2);
}
void theChamp::taunt()
{
    effects::Vulnerable(apci.getPlayer(), 2);
    effects::Weak(apci.getPlayer(), 2);
}
void theChamp::heavySlash()
{
    effects::enemyAdjustedDamage(*this, 16);
}
void theChamp::gloat()
{
    effects::Strength(*this, 2);
}
void theChamp::execute()
{
    effects::singleTargetMulti(*this, apci.getPlayer(), 10, 2);
}
void theChamp::anger()
{
    clearDebuffs();
    gainStrength(6);
}
void theChamp::chooseAndDeclareIntent()
{
    if (health_ <= max_health_ / 2 && !phaseTwo)
    {
        constructIntentions("anger", { Intents("ClearDebuff",1000,1), Intents("Strength",6,1) });
        declaredIntent = &theChamp::anger;
        phaseTwo = true;
    }
    else if (pastAttacks.size() > 0 && pastAttacks.back() == "anger")
    {
        constructIntentions("execute", { Intents("Attack",10,2) });
        declaredIntent = &theChamp::execute;
    }
    else if (pastAttacks.size() > 3 && *(pastAttacks.end() - 3) == "execute")
    {
        constructIntentions("execute", { Intents("Attack",10,2) });
        declaredIntent = &theChamp::execute;
    }
    else if (pastAttacks.size() % 4 == 0)
    {
        constructIntentions("taunt", { Intents("Vulnerable",2,1), Intents("Weak",2,1) });
        declaredIntent = &theChamp::taunt;
    }
    else
    {
        vector<int> percentages_{ 15,15,25,45 };
        if (pastAttacks.back() == "defensiveStance" || defStanceCount==2) { percentages_[0] = 0; percentages_[1] = 30; }
        else if (pastAttacks.back() == "gloat") { percentages_[1] = 0; percentages_[2] = 40; }
        else if (pastAttacks.back() == "faceSlap") { percentages_[2] = 0; percentages_[3] = 70; }
        else if (pastAttacks.back() == "heavySlash") { percentages_[3] = 0; percentages_[2] = 70; }
        int index = calcIntentOdds({ "defensiveStance","gloat","faceSlap","heavySlash" }, percentages_, { 1,1,1,1 });
        switch (index)
        {
        case 0:
            constructIntentions("defensiveStance", { Intents("Block",15,1), Intents("Metallicise",5,1) });
            declaredIntent = &theChamp::defensiveStance;
            ++defStanceCount;
            break;
        case 1:
            constructIntentions("gloat", { Intents("Strength",2,1) });
            declaredIntent = &theChamp::gloat;
            break;
        case 2:
            constructIntentions("faceSlap", { Intents("Attack",12,1), Intents("Vulnerable",2,1), Intents("Frail",2,1) });
            declaredIntent = &theChamp::faceSlap;
            break;
        case 3:
            constructIntentions("heavySlash", { Intents("Attack",16,1) });
            declaredIntent = &theChamp::heavySlash;
            break;
        }
    }
}
void theChamp::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}


void theCollector::buff()
{
    for_each(gm.getAllEnemies().begin(), gm.getAllEnemies().end(), [&](unique_ptr<nonpc>& e) {e->gainStrength(3);});  
    effects::Block(*this, 15);
}
void theCollector::fireball()
{
    effects::enemyAdjustedDamage(*this, 18);
}
void theCollector::megaDebuff()
{
    effects::Weak(apci.getPlayer(), 3);
    effects::Vulnerable(apci.getPlayer(), 3);
    effects::Frail(apci.getPlayer(), 3);
}
void theCollector::spawn()
{
    while (gm.getAllEnemies().size() < 3)
    {
        gm.addEnemyFrontByIDMidFight("TorchHead_ID");
    }
}
void theCollector::chooseAndDeclareIntent()
{
    if (!firstAttackDone)
    {
        constructIntentions("spawn", { Intents("Spawning",1000,1) });
        declaredIntent = &theCollector::spawn;
        firstAttackDone = true;
    }
    else if (pastAttacks.size() == 3)
    {
        constructIntentions("megaDebuff", { Intents("Weak",3,1), Intents("Vulnerable",3,1),Intents("Frail",3,1)});
        declaredIntent = &theCollector::megaDebuff;
    }
    else if (gm.getAllEnemies().size() == 3)
    {
        int index = calcIntentOdds({ "fireball","buff" }, {70,30}, { 2,1 });
        switch (index)
        {
        case 0:
            constructIntentions("fireball", { Intents("Attack",18,1) });
            declaredIntent = &theCollector::fireball;
            break;
        case 1:
            constructIntentions("buff", { Intents("StrengthAll",3,1), Intents("Block",15,1) });
            declaredIntent = &theCollector::buff;
            break;
        }
    }
    else if (gm.getAllEnemies().size() == 1 || gm.getAllEnemies().size() == 2)
    {
        int index = calcIntentOdds({ "spawn","fireball","buff" }, { 25,45,30 }, { 1,2,1 });
        switch (index)
        {
        case 0:
            constructIntentions("spawn", { Intents("Spawning",1000,1) });
            declaredIntent = &theCollector::spawn;
            break;
        case 1:
            constructIntentions("fireball", { Intents("Attack",18,1) });
            declaredIntent = &theCollector::fireball;
            break;
        case 2:
            constructIntentions("buff", { Intents("StrengthAll",3,1), Intents("Block",15,1) });
            declaredIntent = &theCollector::buff;
            break;
        }
    }
    
}
void theCollector::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}



void torchHead::tackle()
{
    effects::enemyAdjustedDamage(*this, 7);
}
void torchHead::chooseAndDeclareIntent()
{
    constructIntentions("tackle", { Intents("Attack",7,1) });
    declaredIntent = &torchHead::tackle;
}
void torchHead::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}

void bronzeAutomaton::boost()
{
    gainStrength(3);
    effects::Block(*this, 9);
}
void bronzeAutomaton::flail()
{
    effects::singleTargetMulti(*this, apci.getPlayer(), 7, 2);
}
void bronzeAutomaton::hyperBeam()
{
    effects::enemyAdjustedDamage(*this, 45);
}
void bronzeAutomaton::spawnOrbs()
{
    gm.addEnemiesByID("BronzeOrb_ID", static_cast<int>(gm.getAllEnemies().size()), true);
    gm.addEnemiesByID("BronzeOrb_ID", static_cast<int>(gm.getAllEnemies().size()), true);
}
void bronzeAutomaton::chooseAndDeclareIntent()
{
    if (!firstAttackDone)
    {
        constructIntentions("spawnOrbs", { Intents("Spawning",1000,1) });
        declaredIntent = &bronzeAutomaton::spawnOrbs;
        firstAttackDone = true;
    }
    else if (pastAttacks.back() == "spawnOrbs" || pastAttacks.back() == "stunned" || 
        (pastAttacks.back() == "boost" && ( * (pastAttacks.end() - 3) == "spawnOrbs" || *(pastAttacks.end() - 3) == "stunned")))
    {
        constructIntentions("flail", { Intents("Attack",7,2) });
        declaredIntent = &bronzeAutomaton::flail;
    }
    else if (pastAttacks.back() == "flail")
    {
        constructIntentions("boost", { Intents("Strength",3,1), Intents("Block",9,1) });
        declaredIntent = &bronzeAutomaton::boost;
    }
    else if (pastAttacks.back() == "boost" && *(pastAttacks.end() - 3) == "boost")
    {
        constructIntentions("hyperBeam", { Intents("Attack",45,1) });
        declaredIntent = &bronzeAutomaton::hyperBeam;
    }
    else if (pastAttacks.back() == "hyperBeam")
    {
        constructIntentions("stunned", { Intents("Stunned",1000,1) });
        declaredIntent = &bronzeAutomaton::stunned;
    }
}
void bronzeAutomaton::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
void bronzeAutomaton::startOfFight()
{
    gainArtifactCharge(3);
    nonpc::startOfFight();
}

void bronzeOrb::stasis()
{
    card* toSteal;
    vector<int> rareIndicesDR, uncommonIndicesDR;
    vector<int> rareIndicesDS, uncommonIndicesDS;
    for (int i{ 0 }; i < apci.getDiscardSize(); ++i)
    {
        switch ((*apci.getDiscardPile()[i]).getRarity())
        {
        case Rare:
            rareIndicesDS.push_back(i);
            break;
        case Uncommon:
            uncommonIndicesDS.push_back(i);
            break;
        }
    }
    for (int i{ 0 }; i < apci.getDrawSize(); ++i)
    {
        switch ((*apci.getDrawPile()[i]).getRarity())
        {
        case Rare:
            rareIndicesDR.push_back(i);
            break;
        case Uncommon:
            uncommonIndicesDR.push_back(i);
            break;
        }
    }
    if (!rareIndicesDR.empty())
    {
        int rng = random_int(0, static_cast<int>(rareIndicesDR.size()) - 1);
        toSteal = (*apci.getDrawPile()[rareIndicesDR[rng]]).clone();
        apci.removeDrawCard(apci.getDrawPile().begin() + rareIndicesDR[rng]);
    }
    else if (!uncommonIndicesDR.empty())
    {
        int rng = random_int(0, static_cast<int>(uncommonIndicesDR.size()) - 1);
        toSteal = (*apci.getDrawPile()[uncommonIndicesDR[rng]]).clone();
        apci.removeDrawCard(apci.getDrawPile().begin() + uncommonIndicesDR[rng]);
    }
    else if (!apci.getDrawPile().empty())
    {
        int rng = random_int(0, static_cast<int>(apci.getDrawSize()) - 1);
        toSteal = (*apci.getDrawPile()[rng]).clone();
        apci.removeDrawCard(apci.getDrawPile().begin() + rng);
    }
    else if (!rareIndicesDS.empty())
    {
        int rng = random_int(0, static_cast<int>(rareIndicesDS.size()) - 1);
        toSteal = (*apci.getDiscardPile()[rareIndicesDS[rng]]).clone();
        apci.removeDiscardCard(apci.getDiscardPile().begin() + rareIndicesDS[rng]);
    }
    else if (!uncommonIndicesDS.empty())
    {
        int rng = random_int(0, static_cast<int>(uncommonIndicesDS.size()) - 1);
        toSteal = (*apci.getDiscardPile()[uncommonIndicesDS[rng]]).clone();
        apci.removeDiscardCard(apci.getDiscardPile().begin() + uncommonIndicesDS[rng]);
    }
    else if (!apci.getDiscardPile().empty())
    {
        int rng = random_int(0, static_cast<int>(apci.getDiscardSize()) - 1);
        toSteal = (*apci.getDiscardPile()[rng]).clone();
        apci.removeDiscardCard(apci.getDiscardPile().begin() + rng);
    }
    else
    {
        return;
    }
    for (auto itr{ toSteal->getObservers().begin() }; itr != toSteal->getObservers().end(); ++itr)
    {
        (**itr).setInactive();
    }
    addAltBuffs(Intents(toSteal->getName(), 1000, 1));
    observeEvent<returnStolenTrigger<character>>(gm.getEnemyDied(), new returnStolenTrigger<character>(this,gm.getEnemyDied(),toSteal));
}
void bronzeOrb::beam()
{
    effects::enemyAdjustedDamage(*this, 8);
}
void bronzeOrb::supportBeam()
{
    effects::Block(*gm.getAllEnemies()[0], 12);
}
void bronzeOrb::chooseAndDeclareIntent()
{
    if (!stasised)
    {
        int index = calcIntentOdds({ "stasis","beam","supportBeam" }, { 75,8,17 }, { 2,2,2 });
        switch (index)
        {
        case 0:
            constructIntentions("stasis", { Intents("Steal Card",1000,1) });
            declaredIntent = &bronzeOrb::stasis;
            stasised = true;
            break;
        case 1:
            constructIntentions("beam", { Intents("Attack",8,1) });
            declaredIntent = &bronzeOrb::beam;
            break;
        case 2:
            constructIntentions("supportBeam", { Intents("Block",12,1) });
            declaredIntent = &bronzeOrb::supportBeam;
            break;
        }
    }
    else
    {
        int index = calcIntentOdds({ "beam","supportBeam" }, { 30,70 }, { 2,2 });
        switch (index)
        {
        case 0:
            constructIntentions("beam", { Intents("Attack",8,1) });
            declaredIntent = &bronzeOrb::beam;
            break;
        case 1:
            constructIntentions("supportBeam", { Intents("Block",12,1) });
            declaredIntent = &bronzeOrb::supportBeam;
            break;
        }
    }
}
void bronzeOrb::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}


void pointy::attack()
{
    effects::singleTargetMulti(*this, apci.getPlayer(), 5, 2);
}
void pointy::chooseAndDeclareIntent()
{
    constructIntentions("attack", { Intents("Attack",12,1) });
    declaredIntent = &pointy::attack;
}
void pointy::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}

void romeo::agonizingSlash()
{
    effects::enemyAdjustedDamage(*this, 10);
    effects::Weak(*this, 2);
}
void romeo::crossSlash()
{
    effects::enemyAdjustedDamage(*this, 15);
}
void romeo::chooseAndDeclareIntent()
{
    if (pastAttacks.size() == 0)
    {
        constructIntentions("mock", { Intents("???",1000,1) });
        declaredIntent = &romeo::mock;
    }
    else if ( pastAttacks.back() == "mock" || pastAttacks.back() == "crossSlash")
    {
        constructIntentions("agonizingSlash", { Intents("Attack",10,1), Intents("Weak",2,1) });
        declaredIntent = &romeo::agonizingSlash;
    }
    else if (pastAttacks.back() == "agonizingSlash" )
    {
        constructIntentions("crossSlash", { Intents("Attack",15,1) });
        declaredIntent = &romeo::crossSlash;
    }
}
void romeo::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}



void bear::bearHug()
{
    effects::Dexterity(apci.getPlayer(), -2);
}
void bear::lunge()
{
    effects::enemyAdjustedDamage(*this, 9);
    effects::Block(*this, 9);
}
void bear::maul()
{
    effects::enemyAdjustedDamage(*this, 18);
}
void bear::chooseAndDeclareIntent()
{
    if (pastAttacks.size() == 0)
    {
        constructIntentions("bearHug", { Intents("Lose Dexterity",2,1) });
        declaredIntent = &bear::bearHug;
    }
    else if (pastAttacks.back() == "bearHug" || pastAttacks.back() == "maul")
    {
        constructIntentions("lunge", { Intents("Attack",9,1), Intents("Block",9,1) });
        declaredIntent = &bear::lunge;
    }
    else if (pastAttacks.back() == "lunge")
    {
        constructIntentions("maul", { Intents("Attack",18,1) });
        declaredIntent = &bear::maul;
    }
}
void bear::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}

void orbWalker::laser()
{
    effects::enemyAdjustedDamage(*this, 10);
    apci.addCardToDiscard(cardHolder(gm.getStatusCardByID("Burn_ID")->clone()));
}
void orbWalker::claw()
{
    effects::enemyAdjustedDamage(*this, 15);
}
void orbWalker::chooseAndDeclareIntent()
{
    int index = calcIntentOdds({ "laser","claw" }, { 60,40 }, { 2,2 });
    switch (index)
    {
    case 0:
        constructIntentions("laser", { Intents("Attack",10,1),Intents("Burn",1,1) });
        declaredIntent = &orbWalker::laser;
        break;
    case 1:
        constructIntentions("claw", { Intents("Attack",15,1) });
        declaredIntent = &orbWalker::claw;
        break;
    }
    
}
void orbWalker::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
void orbWalker::startOfFight()
{
    ritual_ += strengthNumber;
    observeEvent<ritualTrigger<character>>(gm.getEnemyTurnEnd(), new ritualTrigger<character>(this, gm.getEnemyTurnEnd()));
    addAltBuffs(Intents("Rit", ritual_, 1));
    nonpc::startOfFight();
}

void spiker::cut()
{
    effects::enemyAdjustedDamage(*this, 7);
}
void spiker::spike()
{
    ++spikeCount;
    removeObserver<thornsTrigger<character>>();
    addThorns(2);
    addAltBuffs(Intents("Thorns", thorns_, 1));
    observeEvent<thornsTrigger<character>>(gm.getEnemyTakingAttackDamage(), new thornsTrigger<character>(this, gm.getEnemyTakingAttackDamage(), thorns_), true);
}
void spiker::chooseAndDeclareIntent()
{
    if (spikeCount >= 6)
    {
        constructIntentions("cut", { Intents("Attack",7,1) });
        declaredIntent = &spiker::cut;
        return;
    }
    int index = calcIntentOdds({ "cut","spike" }, { 50,50 }, { 1,1000 });
    switch (index)
    {
    case 0:
        constructIntentions("cut", { Intents("Attack",7,1) });
        declaredIntent = &spiker::cut;
        break;
    case 1:
        constructIntentions("spike", { Intents("Spikes",2,1) });
        declaredIntent = &spiker::spike;
        break;
    }

}
void spiker::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
void spiker::startOfFight()
{
    addThorns(thornsNumber);
    addAltBuffs(Intents("Thorns", thorns_, 1));
    observeEvent<thornsTrigger<character>>(gm.getEnemyTakingAttackDamage(), new thornsTrigger<character>(this, gm.getEnemyTakingAttackDamage(), thorns_), true);
    nonpc::startOfFight();
}

void exploder::slam()
{
    effects::enemyAdjustedDamage(*this, 9);
}
void exploder::explode()
{
    apci.getPlayer().takeDamageNoTrigger(30);
    takeDamageNoTrigger(1000);
}
void exploder::chooseAndDeclareIntent()
{
    if (pastAttacks.size() <= 1)
    {
        constructIntentions("slam", { Intents("Attack",9,1) });
        declaredIntent = &exploder::slam;
    }
    else
    {
        constructIntentions("explode", { Intents("???",30,1) });
        declaredIntent = &exploder::explode;
    }
}
void exploder::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
void exploder::startOfFight()
{
    nonpc::startOfFight();
    addAltBuffs(Intents("Explode", 30, 1));
}

void repulsor::bashh()
{
    effects::enemyAdjustedDamage(*this, 11);
}
void repulsor::repulse()
{
    apci.addCardToDrawRandom(cardHolder(gm.getStatusCardByID("Dazed_ID")->clone()));
}
void repulsor::chooseAndDeclareIntent()
{
    int index = calcIntentOdds({ "bashh","repulse" }, { 20,80 }, { 1,1000 });
    switch (index)
    {
    case 0:
        constructIntentions("bashh", { Intents("Attack",11,1) });
        declaredIntent = &repulsor::bashh;
        break;
    case 1:
        constructIntentions("repulse", { Intents("Dazed",2,1) });
        declaredIntent = &repulsor::repulse;
        break;
    }
}
void repulsor::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}

void theMaw::roar()
{
    effects::Weak(apci.getPlayer(), 3);
    effects::Frail(apci.getPlayer(), 3);
}
void theMaw::drool()
{
    effects::Strength(*this, 3);
}
void theMaw::slam()
{
    effects::enemyAdjustedDamage(*this, 25);
}
void theMaw::nom()
{
    int t = gm.turnCount / 2;
    effects::singleTargetMulti(*this, apci.getPlayer(), 5, t);
}
void theMaw::chooseAndDeclareIntent()
{
    if (pastAttacks.empty())
    {
        constructIntentions("roar", { Intents("Frail",3,1), Intents("Weak",3,1) });
        declaredIntent = &theMaw::roar;
    }
    else if (pastAttacks.back() == "roar" || pastAttacks.back() == "drool")
    {
        if (random_int(0, 1))
        {
            constructIntentions("slam", { Intents("Attack",25,1) });
            declaredIntent = &theMaw::slam;
        }
        else
        {
            constructIntentions("nom", { Intents("Attack",5,(gm.turnCount+1) / 2) });
            declaredIntent = &theMaw::nom;
        }
    }
    else if (pastAttacks.back() == "slam")
    {
        if (random_int(0, 1))
        {
            constructIntentions("drool", { Intents("Strength",3,1) });
            declaredIntent = &theMaw::drool;
        }
        else
        {
            constructIntentions("nom", { Intents("Attack",5,(gm.turnCount + 1) / 2) });
            declaredIntent = &theMaw::nom;
        }
    }
    else
    {
        constructIntentions("drool", { Intents("Strength",3,1) });
        declaredIntent = &theMaw::drool;
    }
}
void theMaw::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}

void spireGrowth::quickTackle()
{
    effects::enemyAdjustedDamage(*this, 16);
}
void spireGrowth::smash()
{
    effects::enemyAdjustedDamage(*this, 22);
}
void spireGrowth::constrict()
{
    apci.getPlayer().applyConstricted(10);
}
void spireGrowth::chooseAndDeclareIntent()
{
    if (apci.getPlayer().getConstricted() > 0 || (pastAttacks.size()>0 && pastAttacks.back() == "constrict"))
    {
        int index = calcIntentOdds({ "quickTackle","smash" }, { 50,50 }, { 2,2 });
        switch (index)
        {
        case 0:
            constructIntentions("quickTackle", { Intents("Attack",16,1) });
            declaredIntent = &spireGrowth::quickTackle;
            break;
        case 1:
            constructIntentions("smash", { Intents("Attack",22,1) });
            declaredIntent = &spireGrowth::smash;
            break;
        }
    }
    else
    {
        int index = calcIntentOdds({ "quickTackle","constrict" }, { 50,50 }, { 2,1 });
        switch (index)
        {
        case 0:
            constructIntentions("quickTackle", { Intents("Attack",16,1) });
            declaredIntent = &spireGrowth::quickTackle;
            break;
        case 1:
            constructIntentions("constrict", { Intents("Constrict",10,1) });
            declaredIntent = &spireGrowth::constrict;
            break;
        }
    }
    
}
void spireGrowth::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}

void transient::attack()
{
    effects::enemyAdjustedDamage(*this, 30 + 10*(gm.turnCount - 1));
    if (gm.turnCount >= 5) takeDamageNoTrigger(1000);
}
void transient::chooseAndDeclareIntent()
{
    constructIntentions("attack", { Intents("Attack",30 + 10*(gm.turnCount),1)});
    declaredIntent = &transient::attack;
}
void transient::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
bool transient::takeDamage(int amount, character& initiator)
{
    int temp_health{ health_ };
    if (character::takeDamage(amount, initiator)) return true;
    amount = temp_health - health_;
    if (amount > 0)
    {
        changeEotStrength(amount);
        loseStrength(amount);
    }
    return false;
}
bool transient::takeDamageNoTrigger(int amount)
{
    int temp_health{ health_ };
    if (character::takeDamageNoTrigger(amount)) return true;
    amount = temp_health - health_;
    if (amount > 0)
    {
        changeEotStrength(amount);
        loseStrength(amount);
    }
    return false;
}

void writhingMass::implant()
{
    apci.addCardToDeck(gm.getCurseCardByID("Parasite_ID")->clone());
    implanted = true;
}
void writhingMass::flail()
{
    effects::enemyAdjustedDamage(*this,15);
    effects::Block(*this, 16);
}
void writhingMass::whither()
{
    effects::enemyAdjustedDamage(*this, 10);
    effects::Weak(apci.getPlayer(), 2);
    effects::Vulnerable(apci.getPlayer(), 2);
}
void writhingMass::multiStrike()
{
    effects::singleTargetMulti(*this, apci.getPlayer(), 7, 3);
}
void writhingMass::strongStrike()
{
    effects::enemyAdjustedDamage(*this, 32);
}
void writhingMass::chooseAndDeclareIntent()
{
    if (pastAttacks.empty())
    {
        switch (random_int(0, 3))
        {
        case 0:
            constructIntentions("flail", { Intents("Attack",15,1), Intents("Block",16,1) });
            declaredIntent = &writhingMass::flail;
            break;
        case 1:
            constructIntentions("whither", { Intents("Attack",10,1), Intents("Weak",2,1), Intents("Vulnerable",2,1) });
            declaredIntent = &writhingMass::whither;
            break;
        case 2:
            constructIntentions("multiStrike", { Intents("Attack",7,3) });
            declaredIntent = &writhingMass::multiStrike;
            break;
        case 3:
            constructIntentions("strongStrike", { Intents("Attack",32,1) });
            declaredIntent = &writhingMass::strongStrike;
            break;
        }
    }
    else
    {
        vector<string> names = { "implant", "flail", "whither", "multiStrike", "strongStrike" };
        vector<int> pcs = { 10,40,60,90,100 };
        int rng{ 0 };
        bool repeat{ true };
        while(repeat)
        {
            rng = random_int(1, 100);
            if (implanted && rng <= 10) continue;
            if (rng <= 10 && pastAttacks.back() != "implant") break;
            if (rng > 10 && pastAttacks.back() == "implant") break;
            for (int i{ 1 }; i < names.size(); ++i)
            {
                if (names[i] == pastAttacks.back() && (rng > pcs[i] || rng <= pcs[i - 1]))
                {
                    repeat = false;
                    break;
                }
            }
        }
        int index{ 0 };
        for (int i{ 0 }; i < pcs.size(); ++i)
        {
            if (rng <= pcs[i]) { index = i; break; }
        }
        switch (index)
        {
        case 0:
            constructIntentions("implant", { Intents("Curse",1000,1) });
            declaredIntent = &writhingMass::implant;
            break;
        case 1:
            constructIntentions("flail", { Intents("Attack",15,1), Intents("Block",16,1) });
            declaredIntent = &writhingMass::flail;
            break;
        case 2:
            constructIntentions("whither", { Intents("Attack",10,1), Intents("Weak",2,1), Intents("Vulnerable",2,1) });
            declaredIntent = &writhingMass::whither;
            break;
        case 3:
            constructIntentions("multiStrike", { Intents("Attack",7,3) });
            declaredIntent = &writhingMass::multiStrike;
            break;
        case 4:
            constructIntentions("strongStrike", { Intents("Attack",32,1) });
            declaredIntent = &writhingMass::strongStrike;
            break;
        }
    }
}
void writhingMass::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
bool writhingMass::takeDamage(int amount, character& initiator)
{
    int temp_health{ health_ };
    if (character::takeDamage(amount, initiator)) return true;
    amount = temp_health - health_;
    if (amount > 0)
    {
        chooseAndDeclareIntent();
    }
    return false;
}
void writhingMass::startOfFight()
{
    sofMalleable(3);
    nonpc::startOfFight();
}

void giantHead::count()
{
    effects::enemyAdjustedDamage(*this, 13);
}
void giantHead::glare()
{
    effects::Weak(apci.getPlayer(), 1);
}
void giantHead::itsTime()
{
    effects::enemyAdjustedDamage(*this, 30 + augment);
    if (augment <= 25) augment += 5;
}
void giantHead::chooseAndDeclareIntent()
{
    if (gm.turnCount < 5)
    {
        int index = calcIntentOdds({ "count","glare" }, { 50,50 }, { 2,2 });
        switch (index)
        {
        case 0:
            constructIntentions("count", { Intents("Attack",13,1) });
            declaredIntent = &giantHead::count;
            break;
        case 1:
            constructIntentions("glare", { Intents("Weak",1,1) });
            declaredIntent = &giantHead::glare;
            break;
        }
    }
    else
    {
        constructIntentions("itsTime", { Intents("Attack",30 + augment,1) });
        declaredIntent = &giantHead::itsTime;
    }
}
void giantHead::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
void giantHead::startOfFight()
{
    observeEvent<slowTrigger<character>>(gm.getCardPlayed(), new slowTrigger<character>(this, gm.getCardPlayed()));
    nonpc::startOfFight();
}

void nemesis::debuff()
{
    apci.addCardToDiscard(cardHolder(gm.getStatusCardByID("Burn_ID")->clone()));
    apci.addCardToDiscard(cardHolder(gm.getStatusCardByID("Burn_ID")->clone()));
    apci.addCardToDiscard(cardHolder(gm.getStatusCardByID("Burn_ID")->clone()));
    if (gm.turnCount % 2 == 1) gainIntangible(2);
}
void nemesis::attack()
{
    effects::singleTargetMulti(*this, apci.getPlayer(), 6, 3);
    if (gm.turnCount % 2 == 1) gainIntangible(2);
}
void nemesis::scythe()
{
    effects::enemyAdjustedDamage(*this, 45);
    if (gm.turnCount % 2 == 1) gainIntangible(2);
}
void nemesis::chooseAndDeclareIntent()
{
    if (pastAttacks.empty())
    {
        if (random_int(0, 1))
        {
            constructIntentions("debuff", { Intents("Burn",3,1) });
            declaredIntent = &nemesis::debuff;
        }
        else
        {
            constructIntentions("attack", { Intents("Attack",6,3) });
            declaredIntent = &nemesis::attack;
        }
    }
    else
    {
        int index = calcIntentOdds({ "debuff","attack","scythe" }, { 35,35,30 }, { 1,2,1 });
        switch (index)
        {
        case 0:
            constructIntentions("debuff", { Intents("Burn",3,1) });
            declaredIntent = &nemesis::debuff;
            break;
        case 1:
            constructIntentions("attack", { Intents("Attack",6,3) });
            declaredIntent = &nemesis::attack;
            break;
        case 2:
            constructIntentions("scythe", { Intents("Attack",45,1) });
            declaredIntent = &nemesis::scythe;
            break;
        }
    }
}
void nemesis::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}

void reptomancer::summon()
{
    gm.addEnemiesByID("Dagger_ID", static_cast<int>(gm.getAllEnemies().size()), true);
}
void reptomancer::snakeStrike()
{
    effects::singleTargetMulti(*this, apci.getPlayer(), 13, 2);
    effects::Weak(apci.getPlayer(), 1);
}
void reptomancer::bigBite()
{
    effects::enemyAdjustedDamage(*this, 30);
}
void reptomancer::chooseAndDeclareIntent()
{
    if (pastAttacks.empty())
    {
        constructIntentions("summon", { Intents("Summon",1,1) });
        declaredIntent = &reptomancer::summon;
    }
    else if (gm.getAllEnemies().size() == 5)
    {
        int index = calcIntentOdds({ "snakeStrike","bigBite" }, { 67,33 }, { 1,1 });
        switch (index)
        {
        case 0:
            constructIntentions("snakeStrike", { Intents("Attack",6,3) });
            declaredIntent = &reptomancer::snakeStrike;
            break;
        case 1:
            constructIntentions("bigBite", { Intents("Attack",45,1) });
            declaredIntent = &reptomancer::bigBite;
            break;
        }
    }
    else
    {
        int index = calcIntentOdds({ "summon","snakeStrike","bigBite" }, { 34,33,33 }, { 2,1,1 });
        switch (index)
        {
        case 0:
            constructIntentions("summon", { Intents("Summon",1,1) });
            declaredIntent = &reptomancer::summon;
            break;
        case 1:
            constructIntentions("snakeStrike", { Intents("Attack",6,3) });
            declaredIntent = &reptomancer::snakeStrike;
            break;
        case 2:
            constructIntentions("bigBite", { Intents("Attack",45,1) });
            declaredIntent = &reptomancer::bigBite;
            break;
        }
    }
}
void reptomancer::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}

void dagger::stab()
{
    effects::enemyAdjustedDamage(*this, 9);
    apci.addCardToDiscard(cardHolder(gm.getStatusCardByID("Wound_ID")->clone()));
}
void dagger::explode()
{
    effects::enemyAdjustedDamage(*this, 25);
    takeDamageNoTrigger(1000);
}
void dagger::chooseAndDeclareIntent()
{
    if (pastAttacks.empty())
    {
        constructIntentions("stab", { Intents("Attack",9,1) });
        declaredIntent = &dagger::stab;
    }
    else
    {
        constructIntentions("explode", { Intents("Attack",25,1) });
        declaredIntent = &dagger::explode;
    }
}
void dagger::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}

void darkling::nip()
{
    effects::enemyAdjustedDamage(*this, attackNumber);
}
void darkling::chomp()
{
    effects::singleTargetMulti(*this, apci.getPlayer(), 8, 2);
}
void darkling::harden()
{
    effects::Block(*this, 12);
}
void darkling::chooseAndDeclareIntent()
{
    if (attackNumber == 0)
    {
        attackNumber = random_int(7, 11);
        middleOne = scrnIndex == 1;
    }
    if (pastAttacks.size() == 0 || middleOne)
    {
        if (random_int(0, 1))
        {
            constructIntentions("nip", { Intents("Attack",attackNumber,1) });
            declaredIntent = &darkling::nip;
        }
        else
        {
            constructIntentions("harden", { Intents("Block",12,1) });
            declaredIntent = &darkling::harden;
        }
    }
    else
    {
        int index = calcIntentOdds({ "nip","chomp","harden" }, { 30,40,30 }, { 2,1,1 });
        switch (index)
        {
        case 0:
            constructIntentions("nip", { Intents("Attack",attackNumber,1) });
            declaredIntent = &darkling::nip;
            break;
        case 1:
            constructIntentions("chomp", { Intents("Attack",8,2) });
            declaredIntent = &darkling::chomp;
            break;
        case 2:
            constructIntentions("harden", { Intents("Block",12,1) });
            declaredIntent = &darkling::harden;
            break;
        }
    }
}
void darkling::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}

void donu::circleOfPower()
{
    for_each(gm.getAllEnemies().begin(), gm.getAllEnemies().end(), [](unique_ptr<nonpc>& e) {e->gainStrength(3); });
}
void donu::beam()
{
    effects::singleTargetMulti(*this,apci.getPlayer(), 10,2);
}
void donu::chooseAndDeclareIntent()
{
    if (pastAttacks.size() % 2 ==0)
    {
        constructIntentions("circleOfPower", { Intents("StrengthAll",3,1) });
        declaredIntent = &donu::circleOfPower;
    }
    else
    {
        constructIntentions("beam", { Intents("Attack",10,2) });
        declaredIntent = &donu::beam;
    }
}
void donu::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
void donu::startOfFight()
{
    gainArtifactCharge(2);
    nonpc::startOfFight();
}

void deca::squareOfProtection()
{
    for_each(gm.getAllEnemies().begin(), gm.getAllEnemies().end(), [](unique_ptr<nonpc>& e) {e->gainBlock(16); });
}
void deca::beam()
{
    effects::singleTargetMulti(*this, apci.getPlayer(), 10, 2);
    apci.addCardToDiscard(cardHolder(gm.getStatusCardByID("Dazed_ID")->clone()));
    apci.addCardToDiscard(cardHolder(gm.getStatusCardByID("Dazed_ID")->clone()));

}
void deca::chooseAndDeclareIntent()
{
    if (pastAttacks.size() % 2 == 0)
    {
        constructIntentions("beam", { Intents("Attack",10,2) });
        declaredIntent = &deca::beam; 
    }
    else
    {
        constructIntentions("squareOfProtection", { Intents("BlockAll",16,1) });
        declaredIntent = &deca::squareOfProtection;
    }
}
void deca::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
void deca::startOfFight()
{
    gainArtifactCharge(2);
    nonpc::startOfFight();
}

void awakenedOne::slash()
{
    effects::enemyAdjustedDamage(*this, 20);
}
void awakenedOne::soulStrike()
{
    effects::singleTargetMulti(*this, apci.getPlayer(), 6, 4);
}
void awakenedOne::darkEcho()
{
    effects::enemyAdjustedDamage(*this, 40);
}
void awakenedOne::sludge()
{
    effects::enemyAdjustedDamage(*this, 18);
    apci.addCardToDrawRandom(cardHolder(gm.getStatusCardByID("Void_ID")->clone()));
}
void awakenedOne::tackle()
{
    effects::singleTargetMulti(*this, apci.getPlayer(), 10, 3);
}
void setAOstored(int val)
{
    awakenedOne::storedStrength = val;
}
void awakenedOne::chooseAndDeclareIntent()
{
    if (!awakened)
    {
        if (pastAttacks.size() == 0)
        {
            constructIntentions("slash", { Intents("Attack",20,1) });
            declaredIntent = &awakenedOne::slash;
        }
        else
        {
            int index = calcIntentOdds({ "slash","soulStrike" }, { 75,25 }, { 2,1 });
            switch (index)
            {
            case 0:
                constructIntentions("slash", { Intents("Attack",20,1) });
                declaredIntent = &awakenedOne::slash;
                break;
            case 1:
                constructIntentions("soulStrike", { Intents("Attack",6,4) });
                declaredIntent = &awakenedOne::soulStrike;
                break;
            }
        }
    }
    else
    {
        if (pastAttacks.size() == 0)
        {
            constructIntentions("darkEcho", { Intents("Attack",40,1) });
            declaredIntent = &awakenedOne::darkEcho;
        }
        else
        {
            int index = calcIntentOdds({ "sludge","tackle" }, { 50,50 }, { 2,2 });
            switch (index)
            {
            case 0:
                constructIntentions("sludge", { Intents("Attack",18,1), Intents("Void",1,1) });
                declaredIntent = &awakenedOne::sludge;
                break;
            case 1:
                constructIntentions("tackle", { Intents("Attack",10,3) });
                declaredIntent = &awakenedOne::tackle;
                break;
            }
        }
    }
}
void awakenedOne::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
void awakenedOne::startOfFight()
{
    curiosity_ = 1;
    addAltBuffs(Intents("CUR", 1, 1));
    addRegeneration(10);
    observeEvent<curiosityTrigger<character>>(gm.getPowerPlayed(), new curiosityTrigger<character>(this, gm.getPowerPlayed(), 1));
    observeEvent<awakenedDeathTrigger<character>>(gm.getEnemyDied(), new awakenedDeathTrigger<character>(this, gm.getEnemyDied()));
    nonpc::startOfFight();
}
void awakenedOne::returnFromDormant() 
{
    awakened = true;
    strength_ = storedStrength;
    storedStrength = 0;
    dormant_ = false;
}
int awakenedOne::storedStrength{ 0 };


void timeEater::reverberate()
{
    effects::singleTargetMulti(*this, apci.getPlayer(), 7, 3);
}
void timeEater::headSlam()
{
    effects::enemyAdjustedDamage(*this, 26);
    apci.getPlayer().applyDrawReduction(2);
}
void timeEater::ripple()
{
    effects::Block(*this, 20);
    effects::Vulnerable(apci.getPlayer(), 1);
    effects::Weak(apci.getPlayer(), 1);
}
void timeEater::haste()
{
    clearDebuffs();
    if (health_< max_health_/2) heal(max_health_ / 2 - health_);
    alreadyHalf = true;
}
void timeEater::chooseAndDeclareIntent()
{
    if (health_ <= max_health_/2 && !alreadyHalf)
    {
        constructIntentions("haste", { Intents("Debuff",1000,1), Intents("Heal%",50,1) });
        declaredIntent = &timeEater::haste;
    }
    else
    {
        int index = calcIntentOdds({ "ripple","reverberate","headSlam"}, {20,45,35}, {1,1,2});
        switch (index)
        {
        case 0:
            constructIntentions("ripple", { Intents("Block",20,1),Intents("Vulnerable",1,1),Intents("Weak",1,1), });
            declaredIntent = &timeEater::ripple;
            break;
        case 1:
            constructIntentions("reverberate", { Intents("Attack",7,3) });
            declaredIntent = &timeEater::reverberate;
            break;
        case 2:
            constructIntentions("headSlam", { Intents("Attack",26,1),Intents("DrawRed",2,1) });
            declaredIntent = &timeEater::headSlam;
            break;
        }
    }
}
void timeEater::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
void timeEater::startOfFight()
{
    observeEvent<clockTrigger<character>>(gm.getCardPlayed(), new clockTrigger<character>(this, gm.getCardPlayed()));
    nonpc::startOfFight();
}

void spireShield::bashh()
{
    effects::enemyAdjustedDamage(*this, 12);
    effects::Strength(apci.getPlayer(), -1);
}
void spireShield::fortify()
{
    for_each(gm.getAllEnemies().begin(), gm.getAllEnemies().end(), [](unique_ptr<nonpc>& e) {e->gainBlock(30); });
}
void spireShield::smash()
{
    effects::enemyAdjustedDamage(*this, 34);
    effects::Block(*this, adjusted::calcEnemyDamage(*this, 34));
}
void spireShield::chooseAndDeclareIntent()
{
    if (pastAttacks.size() % 3 == 0)
    {
        if (random_int(0, 1))
        {
            constructIntentions("bashh", { Intents("Attack",12,1), Intents("LoseSTR",1,1) });
            declaredIntent = &spireShield::bashh;
        }
        else
        {
            constructIntentions("fortify", { Intents("BlockAll",30,1) });
            declaredIntent = &spireShield::fortify;
        }
    }
    else if (pastAttacks.size() % 3 == 1)
    {
        if (pastAttacks.back() == "bashh")
        {
            constructIntentions("fortify", { Intents("BlockAll",30,1) });
            declaredIntent = &spireShield::fortify;
        }
        else
        {
            constructIntentions("bashh", { Intents("Attack",12,1), Intents("LoseSTR",1,1) });
            declaredIntent = &spireShield::bashh;
        }
    }
    else
    {
        constructIntentions("smash", { Intents("Attack",34,1), Intents("Block",adjusted::calcEnemyDamage(*this, 34),1) });
        declaredIntent = &spireShield::smash;
    }
}
void spireShield::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
void spireShield::startOfFight()
{
    observeEvent<backTrigger<character>>(gm.getCardPlayed(), new backTrigger<character>(this, gm.getCardPlayed()));
    gainArtifactCharge(1);
    nonpc::startOfFight();
}

void spireSword::burnStrike()
{
    effects::singleTargetMulti(*this,apci.getPlayer(),5,2);
    apci.addCardToDrawFront(cardHolder(gm.getStatusCardByID("Burn_ID")->clone()));
    apci.addCardToDrawFront(cardHolder(gm.getStatusCardByID("Burn_ID")->clone()));
}
void spireSword::piercer()
{
    for_each(gm.getAllEnemies().begin(), gm.getAllEnemies().end(), [](unique_ptr<nonpc>& e) {e->gainStrength(2); });
}
void spireSword::skewer()
{
    effects::singleTargetMulti(*this, apci.getPlayer(), 10, 3);
}
void spireSword::chooseAndDeclareIntent()
{
    if (pastAttacks.size() == 0)
    {
        constructIntentions("burnStrike", { Intents("Attack",5,2), Intents("Burn",2,1) });
        declaredIntent = &spireSword::burnStrike;
    }
    else if (pastAttacks.size() % 3 == 1)
    {
        constructIntentions("skewer", { Intents("Attack",10,3) });
        declaredIntent = &spireSword::skewer;
    }
    else if (pastAttacks.size() % 3 == 2)
    {
        if (random_int(0, 1))
        {
            constructIntentions("burnStrike", { Intents("Attack",5,2), Intents("Burn",2,1) });
            declaredIntent = &spireSword::burnStrike;
        }
        else
        {
            constructIntentions("piercer", { Intents("STRAll",2,1) });
            declaredIntent = &spireSword::piercer;
        }
    }
    else
    {
        if (pastAttacks.back() == "piercer")
        {
            constructIntentions("burnStrike", { Intents("Attack",5,2), Intents("Burn",2,1) });
            declaredIntent = &spireSword::burnStrike;
        }
        else
        {
            constructIntentions("piercer", { Intents("STRAll",2,1) });
            declaredIntent = &spireSword::piercer;
        }
    }
}
void spireSword::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
void spireSword::startOfFight()
{
    observeEvent<backTrigger<character>>(gm.getCardPlayed(), new backTrigger<character>(this, gm.getCardPlayed()));
    gainArtifactCharge(1);
    nonpc::startOfFight();
}

void corruptHeart::debilitate()
{
    effects::Vulnerable(apci.getPlayer(), 2);
    effects::Weak(apci.getPlayer(), 2);
    effects::Frail(apci.getPlayer(), 2);
    apci.addCardToDrawRandom(cardHolder(gm.getStatusCardByID("Dazed_ID")->clone()));
    apci.addCardToDrawRandom(cardHolder(gm.getStatusCardByID("Slime_ID")->clone()));
    apci.addCardToDrawRandom(cardHolder(gm.getStatusCardByID("Wound_ID")->clone()));
    apci.addCardToDrawRandom(cardHolder(gm.getStatusCardByID("Burn_ID")->clone()));
    apci.addCardToDrawRandom(cardHolder(gm.getStatusCardByID("Void_ID")->clone()));
}
void corruptHeart::bloodShots()
{
    if (painful_stabs_)
    {
        auto trig = new painStabTrigger(gm.getPlayerHealthLoss());
        observeEvent<painStabTrigger>(gm.getPlayerHealthLoss(), trig);
        effects::singleTargetMulti(*this, apci.getPlayer(), 2, 12);
        removeObserver(trig);
    }
    else
    {
        effects::singleTargetMulti(*this, apci.getPlayer(), 2, 12);
    }
}
void corruptHeart::echo()
{
    if (painful_stabs_)
    {
        auto trig = new painStabTrigger(gm.getPlayerHealthLoss());
        observeEvent<painStabTrigger>(gm.getPlayerHealthLoss(), trig);
        effects::enemyAdjustedDamage(*this, 40);
        removeObserver(trig);
    }
    else
    {
        effects::enemyAdjustedDamage(*this, 40);
    }
}
void corruptHeart::buff()
{
    m_strength_ = 0;
    gainStrength(2);
    switch (buffCount)
    {
    case 0:
        gainArtifactCharge(2);
        ++buffCount;
        break;
    case 1:
        addBeatOfDeath(1);
        ++buffCount;
        break;
    case 2:
        painful_stabs_ = true;
        addAltBuffs(Intents("PainStab", 1, 1));
        ++buffCount;
        break;
    case 3:
        gainStrength(10);
        ++buffCount;
        break;
    case 4:
        gainStrength(50);
        break;
    }
}
void corruptHeart::chooseAndDeclareIntent()
{
    if (pastAttacks.size() == 0)
    {
        constructIntentions("debilitate", { Intents("Vulnerable",2,1), Intents("Weak",2,1), Intents("Frail",2,1), Intents("Status",5,1)});
        declaredIntent = &corruptHeart::debilitate;
    }
    else if (pastAttacks.size() % 3 == 1)
    {
        if (random_int(0, 1))
        {
            constructIntentions("echo", { Intents("Attack",40,1) });
            declaredIntent = &corruptHeart::echo;
        }
        else
        {
            constructIntentions("bloodShots", { Intents("Attack",2,12) });
            declaredIntent = &corruptHeart::bloodShots;
        }
    }
    else if (pastAttacks.size() % 3 == 2)
    {
        if (pastAttacks.back() == "echo")
        {
            constructIntentions("bloodShots", { Intents("Attack",2,12) });
            declaredIntent = &corruptHeart::bloodShots;
        }
        else
        {
            constructIntentions("echo", { Intents("Attack",40,1) });
            declaredIntent = &corruptHeart::echo;
        }
    }
    else if (pastAttacks.size() % 3 == 0) 
    {
        constructIntentions("buff", { Intents("Strength",2,1), Intents("Buff",1000,1) });
        declaredIntent = &corruptHeart::buff;
    }
}
void corruptHeart::EnactIntent()
{
    if (declaredIntent == nullptr) throw runtime_error("no stored intent");
    (*this.*declaredIntent)();
}
void corruptHeart::startOfFight()
{
    addBeatOfDeath(1);
    addInvincible(300);
    nonpc::startOfFight();
}
