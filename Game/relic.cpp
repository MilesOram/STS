#include "relic.h"
#include "effects_triggers.h"
#include "gamestate.h"
#include "pc_info.h"
#include "events.h"
#include "visuals.h"

using namespace std;

vector<int> relic::IDsForNloth{10,15,22,31,32,52,85,88,89,91,95,104,117,120,124,126,132};

void akabeko::execute()
{
    apci.getPlayer().outAtt += 8;
}
void akabeko::fightStartSetup()
{
    firstAttack = true;
    attStart = false;
    observeThis(gm.getAlterBaseADFromPlayer());
}
void akabeko::triggered()
{
    if (attStart && (gm.turnCount > 0 || gm.attackCount > 0))
    {
        firstAttack = false;
        return;
    }
    attStart = true;
    if (firstAttack)
    {
        execute();
    }
}
void anchor::fightStartSetup()
{
    effects::Block(apci.getPlayer(), 12);
}
void ancientTeaSet::onPickUp()
{
    observeThis(gm.getCampfireEntered());
}
void ancientTeaSet::triggered() { rested = true; }
void ancientTeaSet::fightStartSetup()
{
    if (rested)
    {
        rested = false;
        gm.increasePlayerEnergy(2);
    }
}
void artOfWar::execute()
{
    skillOnly = false;
}
void artOfWar::triggered(cardHolder& ch)
{
    if (!skillOnly) return;
    if ((*ch).getType() != cardType::Skill)
    {
        execute();
    }
}
void artOfWar::fightStartSetup()
{
    skillOnly = false;
    observeThis(gm.getCardPlayed());
}
void artOfWar::turnStartSetup()
{
    if (skillOnly)
    {
        gm.increasePlayerEnergy(1);
    }
    skillOnly = true;
}
void bagOfMarbles::fightStartSetup()
{
    for (auto itr{ gm.getAllEnemies().begin() }; itr != gm.getAllEnemies().end(); ++itr)
    {
        effects::Vulnerable((**itr), 1);
    }
}
void bagOfPreparation::fightStartSetup()
{
    apci.drawX(2);
}
void bloodVial::fightStartSetup()
{
    apci.getPlayer().heal(2);
}
void bronzeScales::fightStartSetup()
{
    apci.getPlayer().addThorns(3);
    apci.getPlayer().addAltBuffs(Intents("Thorns", apci.getPlayer().getThorns(), 1));
    apci.getPlayer().observeEvent<thornsTrigger<character>>(gm.getPlayerTakingAttackDamage(), new thornsTrigger<character>(&apci.getPlayer(), gm.getPlayerTakingAttackDamage(), apci.getPlayer().getThorns()), true);
}
void centennialPuzzle::fightStartSetup()
{
    observeThis(gm.getPlayerHealthLoss());
    first = true;
}
void centennialPuzzle::execute()
{
    apci.drawX(3);
}
void centennialPuzzle::triggered()
{
    if (first)
    {
        execute();
        first = false;
        observing_->removeObserver(this);
        observeNull();
    }
}
void ceramicFish::execute()
{
    apci.gainGold(9);
}
void ceramicFish::triggered()
{
    execute();
}
void ceramicFish::onPickUp()
{
    observeThis(gm.getCardAdded());
}
void dreamCatcher::execute()
{
    gm.hallwayReward(true);
}
void dreamCatcher::triggered()
{
    execute();
}
void dreamCatcher::onPickUp()
{
    observeThis(gm.getCampfireRest());
}
void happyFlower::execute()
{
    gm.increasePlayerEnergy(1);
}
void happyFlower::turnStartSetup()
{
    if (!active_) return;
    if (++turnCount == 3)
    {
        turnCount = 0;
        execute();
    }
}
void juzuBracelet::onPickUp()
{
    gamestate::juzuBraceletCheck = true;
}
void juzuBracelet::onDropped()
{
    gamestate::juzuBraceletCheck = false;
}
void lantern::fightStartSetup() { gm.increasePlayerEnergy(1); }
void mawBank::execute() { apci.gainGold(12); }
void mawBank::triggered() { if (!offSwitch) execute(); }
void mawBank::onPickUp()
{
    observeThis(gm.getNewFloorEntered());
    apci.getPlayer().observeEvent<mawBankSwitchTrigger>(gm.getGoldSpent(), new mawBankSwitchTrigger(gm.getGoldSpent()));
}
void mawBank::onDropped()
{
    apci.getPlayer().removeObserver<mawBankSwitchTrigger>();
}
bool mawBank::offSwitch{ false };
void mealTicket::execute() { apci.getPlayer().heal(15); }
void mealTicket::triggered() { execute(); }
void mealTicket::onPickUp()
{
    observeThis(gm.getShopEntered());
}
void nunchuku::execute()
{
    gm.increasePlayerEnergy(1);
}
void nunchuku::fightStartSetup()
{
    observeThis(gm.getAttackPlayed());
}
void nunchuku::triggered()
{
    if (++attackCounter == 10)
    {
        attackCounter = 0;
        execute();
    }
}
void oddlySmoothStone::fightStartSetup()
{
    effects::Dexterity(apci.getPlayer(), 1);
}
omamori::omamori() : relic(relicRarity::Common, "Omamori")
{
    cardCount = &gamestate::omamoriCheck;
}
void omamori::onPickUp()
{
    gamestate::omamoriCheck = 2;
}
void omamori::onDropped()
{
    gamestate::omamoriCheck = 0;
}
void orichalcum::execute()
{
    effects::Block(apci.getPlayer(), 6);
}
void orichalcum::triggered()
{
    if (apci.getPlayer().getBlock() == 0) execute();
}
void orichalcum::fightStartSetup()
{
    observeThis(gm.getPlayerTurnEnd());
}
void penNib::execute()
{
    gm.incremenetDAM();
}
void penNib::fightStartSetup()
{
    observeThis(gm.getAttackPlayed());
    if (gm.getDAM() == 0 && attackCounter == 9) gm.incremenetDAM();
}
void penNib::triggered()
{
    if (++attackCounter == 9)
    {
        execute();
    }
    else if (attackCounter == 10) { gm.decrementDAM(); attackCounter = 0; }
}
void potionBelt::onPickUp()
{
    apci.addPotionSlots(2);
}
void preservedInsect::fightStartSetup()
{
    if (gm.getMapPathEvent() == 'E')
    {
        gm.getAllEnemies()[0]->setHealthPercentage(75);
        if (gm.getAllEnemies().size() > 1) gm.getAllEnemies()[1]->setHealthPercentage(75);
        if (gm.getAllEnemies().size() > 2) gm.getAllEnemies()[2]->setHealthPercentage(75);
    }
}
void regalPillow::triggered()
{
    execute();
}
void regalPillow::execute()
{
    apci.getPlayer().heal(15);
}
void regalPillow::onPickUp()
{
    observeThis(gm.getCampfireRest());
}
void smilingMask::onPickUp()
{
    shop::smilingmaskDiscount = true;
}
void smilingMask::onDropped()
{
    shop::smilingmaskDiscount = false;
}
void strawberry::onPickUp()
{
    apci.getPlayer().gainMaxHP(7);
}
void theBoot::execute()
{
    apci.getPlayer().outAtt = 5;
}
void theBoot::triggered()
{
    if (apci.getPlayer().outAtt >= 1 && apci.getPlayer().outAtt <= 4)
    {
        execute();
    }
}
void theBoot::fightStartSetup()
{
    observeThis(gm.getAlterAdjustedADFromPlayer());
}
void tinyChest::onPickUp()
{
    observeThis(gm.getQuesitonEntered());
}
void tinyChest::triggered()
{
    if (++qCounter == 4)
    {
        qCounter = 0;
        gm.setMP('C');
    }
}
void toyOrnithopter::onPickUp()
{
    observeThis(gm.getPotionUsed());
}
void toyOrnithopter::triggered()
{
    apci.getPlayer().heal(5);
}
void vajra::fightStartSetup()
{
    effects::Strength(apci.getPlayer(), 1);
}
void warPaint::onPickUp()
{
    gm.upgradeRandomFromDeck(Skill);
    gm.upgradeRandomFromDeck(Skill);
}
void whetstone::onPickUp()
{
    gm.upgradeRandomFromDeck(Attack);
    gm.upgradeRandomFromDeck(Attack);
}

// ########### uncommon ###########
void blueCandle::onPickUp() { card::blueCandleCheck = true; }
void blueCandle::onDropped() { card::blueCandleCheck = false; }
void bottledFlame::onPickUp()
{
    if (!apci.altToBasicCurseBottled(Attack)) return;
    if (ai)
    {
        for (int i{ 0 }; i < apci.getDeck().size(); ++i)
        {
            // whirlwind and immolate
            if (apci.getDeck()[i]->getCardIDNum() == 58 || apci.getDeck()[i]->getCardIDNum() == 69)
            {
                apci.getDeck()[i]->giveBottled();
                return;
            }
        }
        for (int i{ 0 }; i < apci.getDeck().size(); ++i)
        {
            // anger, hemokinesis, uppercut, bludgeon
            if (apci.getDeck()[i]->getCardIDNum() == 5 || apci.getDeck()[i]->getCardIDNum() == 39 ||
                apci.getDeck()[i]->getCardIDNum() == 57 || apci.getDeck()[i]->getCardIDNum() == 61)
            {
                apci.getDeck()[i]->giveBottled();
                return;
            }
        }
    }
    // ai falls through to this if nothing found
    (*gm.chooseCardFromDeck(Attack))->giveBottled();
}
void bottledLightning::onPickUp()
{
    if (!apci.altToBasicCurseBottled(Skill)) return;
    if (ai)
    {
        cardRarity bestCR = cardRarity::Rare;
        while (bestCR!=cardRarity::Common)
        {
            for (int i{ 0 }; i < apci.getDeck().size(); ++i)
            {
                if (apci.getDeck()[i]->getRarity() == bestCR && apci.getDeck()[i]->getType() == Skill)
                {
                    apci.getDeck()[i]->giveBottled();
                    return;
                }
            }
            switch (bestCR)
            {
            case cardRarity::Rare:
                bestCR = cardRarity::Uncommon;
                break;
            case cardRarity::Uncommon:
                bestCR = cardRarity::Common;
                break;
            }
        }
    }
    (*gm.chooseCardFromDeck(Skill))->giveBottled();
}
void bottledTornado::onPickUp()
{
    if (!apci.altToBasicCurseBottled(Power)) return;
    if (ai)
    {
        cardRarity bestCR = cardRarity::Rare;
        while (bestCR != cardRarity::Common)
        {
            for (int i{ 0 }; i < apci.getDeck().size(); ++i)
            {
                if (apci.getDeck()[i]->getRarity() == bestCR && apci.getDeck()[i]->getType() == Power)
                {
                    apci.getDeck()[i]->giveBottled();
                    return;
                }
            }
            switch (bestCR)
            {
            case cardRarity::Rare:
                bestCR = cardRarity::Uncommon;
                break;
            case cardRarity::Uncommon:
                bestCR = cardRarity::Common;
                break;
            }
        }
    }
    (*gm.chooseCardFromDeck(Power))->giveBottled();
}
void darkstonePeriapt::execute()
{
    apci.getPlayer().gainMaxHP(6);
}
void darkstonePeriapt::triggered(cardHolder& ch)
{
    if ((*ch).getType() == Curse) execute();
}
void darkstonePeriapt::onPickUp()
{
    observeThis(gm.getCardAdded());
}
void eternalFeather::execute()
{
    apci.getPlayer().heal(healNumber * 3);
}
void eternalFeather::triggered()
{
    healNumber = static_cast<int>(apci.getDeck().size()) / 5;
    execute();
}
void eternalFeather::onPickUp()
{
    observeThis(gm.getCampfireEntered());
}
void frozenEgg::execute() {}
void frozenEgg::triggered()
{
    if (!apci.getDeck().back()->getUpgraded() && apci.getDeck().back()->getType() == Power)
        apci.getDeck().back()->upgradeThis();
}
void frozenEgg::onPickUp()
{
    observeThis(gm.getCardAdded());
}
void gremlinHorn::triggered()
{
    execute();
}
void gremlinHorn::execute()
{
    apci.drawX(1);
    gm.increasePlayerEnergy(1);
}
void gremlinHorn::fightStartSetup()
{
    observeThis(gm.getEnemyDied());
}
void hornCleat::fightStartSetup()
{
    turnCount = 0;
}
void hornCleat::turnStartSetup()
{
    if (!active_) return;
    if (turnCount == -1) return;
    if (++turnCount == 2) { effects::Block(apci.getPlayer(), 14); turnCount = -1; }
}
void inkBottle::execute()
{
    apci.drawX(1);
}
void inkBottle::triggered()
{
    if (++cardCount == 10)
    {
        cardCount = 0;
        execute();
    }
}
void inkBottle::fightStartSetup()
{
    observeThis(gm.getCardPlayed());
}
void letterOpener::execute()
{
    effects::AOEDamage(5);
}
void letterOpener::fightStartSetup()
{
    observeThis(gm.getSkillPlayed());
}
void letterOpener::turnStartSetup()
{
    skillCounter = 0;
}
void letterOpener::triggered()
{
    if (++skillCounter == 3)
    {
        skillCounter = 0;
        execute();
    }
}
void kunai::execute()
{
    effects::Dexterity(apci.getPlayer(), 1);
}
void kunai::fightStartSetup()
{
    observeThis(gm.getAttackPlayed());
}
void kunai::turnStartSetup()
{
    attackCounter = 0;
}
void kunai::triggered()
{
    if (++attackCounter == 3)
    {
        attackCounter = 0;
        execute();
    }
}
void matryoshka::execute()
{
    relic* relic_ = random_int(1, 4) == 1 ? gm.retrieveUncommonRelic() : gm.retrieveCommonRelic();
    std::cout << "1 to take " << relic_->getName() << ". Or skip" << endl;
    if (request_input({ "1","skip" }) == "skip") { delete relic_; }
    else { apci.addRelic(relic_); }
}
void matryoshka::onPickUp()
{
    observeThis(gm.getChestOpened());
}
void matryoshka::triggered()
{
    if (relicCounter == 0) return;
    if (relicCounter-- > 0) execute();
}
void meatOnTheBone::execute()
{
    if (apci.getPlayer().getHealth() > 0) apci.getPlayer().heal(12);
}
void meatOnTheBone::fightStartSetup()
{
    observeThis(gm.getFightEnd());
}
void meatOnTheBone::triggered()
{
    if (apci.getPlayer().getHealth() * 2 <= apci.getPlayer().getMaxHealth()) execute();
}
void mercuryHourglass::turnStartSetup()
{
    if (!active_) return;
    effects::AOEDamage(3);
}
void moltenEgg::execute() {}
void moltenEgg::triggered()
{
    if (!apci.getDeck().back()->getUpgraded() && apci.getDeck().back()->getType() == Attack)
        apci.getDeck().back()->upgradeThis();
}
void moltenEgg::onPickUp()
{
    observeThis(gm.getCardAdded());
}
void mummifiedHand::fightStartSetup()
{
    observeThis(gm.getPowerPlayed());
}
void mummifiedHand::triggered()
{
    if (!apci.getHand().empty()) execute();
}
void mummifiedHand::execute()
{
    apci.randomHandCostZero();
}
void ornamentalFan::execute()
{
    effects::Block(apci.getPlayer(), 4);
}
void ornamentalFan::fightStartSetup()
{
    observeThis(gm.getAttackPlayed());
}
void ornamentalFan::turnStartSetup()
{
    attackCounter = 0;
}
void ornamentalFan::triggered()
{
    if (++attackCounter == 3)
    {
        attackCounter = 0;
        execute();
    }
}
void pantograph::fightStartSetup()
{
    if (gm.getMapPathEvent() == 'B')
    {
        apci.getPlayer().heal(25);
    }
}
void pear::onPickUp()
{
    apci.getPlayer().gainMaxHP(10);
}
void questionCard::onPickUp()
{
    gamestate::cardRewardNumber += 1;
}
void questionCard::onDropped()
{
    gamestate::cardRewardNumber -= 1;
}
void shuriken::execute()
{
    effects::Strength(apci.getPlayer(), 1);
}
void shuriken::fightStartSetup()
{
    observeThis(gm.getAttackPlayed());
}
void shuriken::turnStartSetup()
{
    attackCounter = 0;
}
void shuriken::triggered()
{
    if (++attackCounter == 3)
    {
        attackCounter = 0;
        execute();
    }
}
void singingBowl::onPickUp()
{
    gamestate::singingBowlCheck = true;
}
void singingBowl::onDropped()
{
    gamestate::singingBowlCheck = false;
}
void strikeDummy::onPickUp()
{
    gamestate::strikeDummyCheck = true;
}
void strikeDummy::onDropped()
{
    gamestate::strikeDummyCheck = false;
}
void sundial::execute()
{
    gm.increasePlayerEnergy(2);
}
void sundial::triggered()
{
    if (++shuffleCounter == 3)
    {
        shuffleCounter = 0;
        execute();
    }
}
void sundial::fightStartSetup()
{
    observeThis(gm.getShuffledDraw());
}
void theCourier::onPickUp()
{
    shop::courierDiscount = true;
}
void theCourier::onDropped()
{
    shop::courierDiscount = false;
}
void toxicEgg::execute() {}
void toxicEgg::triggered()
{
    if (!apci.getDeck().back()->getUpgraded() && apci.getDeck().back()->getType() == Skill)
        apci.getDeck().back()->upgradeThis();
}
void toxicEgg::onPickUp()
{
    observeThis(gm.getCardAdded());
}
void whiteBeastStatue::onPickUp()
{
    gm.setPotionChance(100);
}
void whiteBeastStatue::onDropped()
{
    gm.setPotionChance(10);
}
// ######## rare ########
void birdFacedUrn::fightStartSetup()
{
    observeThis(gm.getPowerPlayed());
}
void birdFacedUrn::execute()
{
    apci.getPlayer().heal(2);
}
void birdFacedUrn::triggered()
{
    execute();
}
void calipers::fightStartSetup()
{
    apci.getPlayer().changeSOTBlocksObs(new calipersTrigger<character>(&apci.getPlayer(), gm.getSOTBlock()));
}
void captainsWheel::fightStartSetup()
{
    turnCount = 0;
}
void captainsWheel::turnStartSetup()
{
    if (!active_) return;
    if (turnCount == -1) return;
    if (++turnCount == 3) { effects::Block(apci.getPlayer(), 18); turnCount = -1; }
}
void deadBranch::fightStartSetup()
{
    observeThis(gm.getCardExhausted());
}
void deadBranch::execute()
{
    apci.addCardToHand(gm.getRandomCard());
}
void deadBranch::triggered()
{
    execute();
}
void duVuDoll::fightStartSetup()
{
    curseCounter = 0;
    for (auto itr{ apci.getDeck().begin() }; itr != apci.getDeck().end(); ++itr)
    {
        if ((*itr)->getType() == Curse) ++curseCounter;
    }
    effects::Strength(apci.getPlayer(), curseCounter);
}
void fossilizedHelix::fightStartSetup()
{
    apci.getPlayer().addBuffer(1);
}
void gamblingChip::turnStartSetup()
{
    if (gm.turnCount == 0)
    {
        if (!ai)
        {
            switch (visuals::AIU_)
            {
            case aiu::five:
                initVisualsFive();
                break;
            case aiu::four:
                initVisualsFour();
                break;
            case aiu::three:
                initVisualsThree();
                break;
            case aiu::two:
                initVisualsTwo();
                break;
            case aiu::one:
                initVisualsOne();
                break;
            }
            std::cout << "Select cards to discard and redraw (eg 1 3 4 8 9)" << endl;
        }
        if (ai) return; // fix later
        set<int> temp = getIndicesForCardBoolFlip(1, static_cast<int>(apci.getHand().size()), static_cast<int>(apci.getHand().size()));
        auto itr{ temp.begin() };
        int i{ 1 };
        while (itr != temp.end())
        {
            apci.sendHandCardToDiscard(apci.getHand()[*itr - i]);
            ++i; ++itr;
        }
        apci.drawX(static_cast<int>(temp.size()));
    }
}
void ginger::fightStartSetup()
{
    observeThis(gm.getApplyingWeak());
}
void ginger::execute()
{
    apci.getPlayer().zeroWeak();
}
void ginger::triggered()
{
    execute();
}
void girya::onDropped()
{
    gamestate::giryaCheck = false;
}
void girya::onPickUp()
{
    gamestate::giryaCheck = true;
    gm.getCampfire()->addOption(campfireChoices(
        []() {string res = "Lift: Gain 1 extra Strength at start of combat"; return res; },
        []() {apci.incrGiryaCounter(); }, []() {return gamestate::giryaCheck && apci.getGiryaCounter() < 3; }));
}
void girya::fightStartSetup()
{
    effects::Strength(apci.getPlayer(), strengthNumber);
}
void iceCream::onPickUp()
{
    gamestate::iceCreamCheck = true;
}
void iceCream::onDropped()
{
    gamestate::iceCreamCheck = false;
}
void incenseBurner::turnStartSetup()
{
    if (!active_) return;
    if (++turnCount == 6)
    {
        turnCount = 0;
        apci.getPlayer().gainIntangible(1);
    }
}
void lizardTail::triggered()
{
    if (apci.getPlayer().getHealth() == 0) execute();
}
void lizardTail::execute()
{
    if (usedUp)
    {
        return;
    }
    usedUp = true;
    std::cout << "Lizard Tail used." << endl;
    apci.getPlayer().healPercentage(50);
    observing_->removeObserver(this);
    observing_ = nullptr;
}
void lizardTail::onPickUp()
{
    observeThis(gm.getPlayerDied());
}
void mango::onPickUp()
{
    apci.getPlayer().gainMaxHP(14);
}
void oldCoin::onPickUp()
{
    apci.gainGold(300);
}
void peacePipe::onDropped()
{
    gamestate::peacePipeCheck = false;
}
void peacePipe::onPickUp()
{
    gamestate::peacePipeCheck = true;
    gm.getCampfire()->addOption(campfireChoices(
        []() {string res = "Smoke: Remove a card from your deck"; return res; },
        []() {gm.removeCardFromDeck(); }, [&]() {return gamestate::peacePipeCheck; }));
}
void pocketwatch::execute()
{
    apci.drawX(3);
}
void pocketwatch::turnStartSetup()
{
    if (!active_) return;
    if (cardCounter <= 3) execute();
    cardCounter = 0;
}
void pocketwatch::fightStartSetup()
{
    observeThis(gm.getCardPlayed());
    cardCounter = 4;
}
void pocketwatch::triggered()
{
    ++cardCounter;
}
void prayerWheel::onPickUp()
{
    gamestate::prayerWheelCheck = true;
}
void prayerWheel::onDropped()
{
    gamestate::prayerWheelCheck = false;
}
void shovel::onDropped()
{
    gamestate::shovelCheck = false;
}
void shovel::onPickUp()
{
    gamestate::shovelCheck = true;
    gm.getCampfire()->addOption(campfireChoices(
        []() {string res = "Dig: Uncover a random relic"; return res; },
        []() {apci.addRelic(gm.retrieveRandomRelic()); }, [&]() {return gamestate::shovelCheck; }));
}
void stoneCalendar::fightStartSetup()
{
    turnCount = 0;
}
void stoneCalendar::turnStartSetup()
{
    if (!active_) return;
    if (++turnCount == 7) observeThis(gm.getPlayerTurnEnd());
}
void stoneCalendar::triggered()
{
    execute();
    observing_->removeObserver(this);
    observeNull();
}
void stoneCalendar::execute()
{
    effects::AOEDamage(52);
}
void threadAndNeedle::fightStartSetup()
{
    apci.getPlayer().addPlatedArmour(4);
}
void torii::execute()
{
    apci.getPlayer().incAtt = 1;
}
void torii::fightStartSetup()
{
    observeThis(gm.getPlayerTakingAttackDamage());
}
void torii::triggered()
{
    if (apci.getPlayer().incAtt <= 5 && apci.getPlayer().incAtt >= 1) execute();
}
void tungsten::onPickUp()
{
    gamestate::tungstenCheck = true;
}
void tungsten::onDropped()
{
    gamestate::tungstenCheck = false;
}
void tungsten::execute()
{
    apci.getPlayer().imminentHPLoss -= 1;
}
void tungsten::fightStartSetup()
{
    observeThis(gm.getPlayerAboutToLoseHealth());
}
void tungsten::triggered()
{
    if (apci.getPlayer().imminentHPLoss >= 1) execute();
}
void turnip::fightStartSetup()
{
    observeThis(gm.getApplyingFrail());
}
void turnip::execute()
{
    apci.getPlayer().zeroFrail();
}
void turnip::triggered()
{
    execute();
}
void unceasingTop::execute()
{
    apci.drawX(1);
}
void unceasingTop::fightStartSetup()
{
    observeThis(gm.getCardPlayed());
}
void unceasingTop::triggered()
{
    if (apci.getHand().empty()) execute();
}
// ####### shop ########
void cauldron::onPickUp()
{
    gm.XPotionSelection(gm.retrieveXRandomPotions(5));
}
void chemicalX::onPickUp()
{
    gamestate::chemicalXCheck = true;
}
void chemicalX::onDropped()
{
    gamestate::chemicalXCheck = false;
}
void clockworkSouvenir::fightStartSetup()
{
    apci.getPlayer().gainArtifactCharge(1);
}
void dollysMirror::onPickUp()
{
    gm.makeCardChoices(aiEventCardChoice::Duplicate);
    apci.addCardToDeck((*gm.chooseCardFromDeck(true))->clone());
}
void frozenEye::onPickUp()
{
    gamestate::frozenEyeCheck = true;
}
void frozenEye::onDropped()
{
    gamestate::frozenEyeCheck = false;
}
void handDrill::fightStartSetup()
{
    observeThis(gm.getBlockBroken());
}
void handDrill::execute()
{
    effects::Vulnerable(*gm.getAllEnemies()[apci.getPlayer().enemyTargetTriggerIndex], 2);
}
void handDrill::triggered()
{
    execute();
}
void leesWaffle::onPickUp()
{
    apci.getPlayer().gainMaxHP(7);
    apci.getPlayer().healToFull();
}
void medKit::onPickUp()
{
    card::medKitCheck = true;
}
void medKit::onDropped()
{
    card::medKitCheck = false;
}
void membershipCard::onPickUp()
{
    shop::membershipDiscount = true;
    if (gm.getMapPathEvent() != 's') return;
    events* event = gm.getEvent();
    shop* shop_ = dynamic_cast<shop*>(event);
    if (shop_ == nullptr) return; // throw runtime_error("impossible");  -- changed forai ---
    shop_->reCalcMC();
}
void membershipCard::onDropped()
{
    shop::membershipDiscount = false;
}
void orrery::onPickUp()
{
    int i{ 0 };
    while (++i <= 5) gm.hallwayReward(true);
}
void orangePellets::execute()
{
    apci.getPlayer().clearDebuffs();
    att = false;
    pwr = false;
    skl = false;
}
void orangePellets::triggered(cardHolder& ch)
{
    switch ((*ch).getType())
    {
    case Attack:
        att = true;
        break;
    case Skill:
        skl = true;
        break;
    case Power:
        pwr = true;
        break;
    }
    if (pwr && att && skl) execute();
}
void orangePellets::turnStartSetup()
{
    if (!active_) return;
    att = false; skl = false; pwr = false;
}
void orangePellets::fightStartSetup()
{
    observeThis(gm.getCardPlayed());
}
void slingOfCourage::fightStartSetup()
{
    if (gm.getMapPathEvent() == 'E') effects::Strength(apci.getPlayer(), 2);
}
void strangeSpoon::onPickUp()
{
    gamestate::strangeSpoonCheck = true;
}
void strangeSpoon::onDropped()
{
    gamestate::strangeSpoonCheck = false;
}
void theAbacus::execute()
{
    effects::Block(apci.getPlayer(), 6);
}
void theAbacus::triggered()
{
    execute();
}
void theAbacus::fightStartSetup()
{
    observeThis(gm.getShuffledDraw());
}
// ####### event ########
void bloodyIdol::triggered()
{
    apci.getPlayer().heal(5);
}
void bloodyIdol::onPickUp()
{
    observeThis(gm.getGoldSpent());
}
void faceOfCleric::triggered()
{
    if(apci.getPlayer().getHealth() != 0) apci.getPlayer().gainMaxHP(1);
}
void faceOfCleric::fightStartSetup()
{
    observeThis(gm.getFightEnd());
}
void goldenIdol::onPickUp()
{
    gamestate::goldenIdolCheck = true;
}
void goldenIdol::onDropped()
{
    gamestate::goldenIdolCheck = false;
}
void gremlinVisage::fightStartSetup()
{
    effects::Weak(apci.getPlayer(), 1);
}
void neowsLament::execute()
{
    gm.getAllEnemies()[0]->setHealth(1);
    if (gm.getAllEnemies().size() > 1) gm.getAllEnemies()[1]->setHealth(1);
    if (gm.getAllEnemies().size() > 2) gm.getAllEnemies()[2]->setHealth(1);
}
void neowsLament::fightStartSetup()
{
    if (fightCount == 0) return;
    if (--fightCount >= 0) execute();
}
void nlothsHungryFace::onPickUp()
{
    chest::nlothCheck = true;
}
void nlothsHungryFace::onDropped()
{
    chest::nlothCheck = false;
}
void oddMushroom::onPickUp()
{
    apci.getPlayer().setVulnerableMulti(1.25);
}
void oddMushroom::onDropped()
{
    apci.getPlayer().setVulnerableMulti(1.5);
}
void ssserpentHead::onPickUp()
{
    observeThis(gm.getQuesitonEntered());
}
void ssserpentHead::triggered()
{
    apci.gainGold(50);
}
void warpedTongs::turnStartSetup()
{
    if (!active_) return;
    effects::upgradeRandomCard();
}
void burningBlood::triggered()
{
    if(apci.getPlayer().getHealth()>0) apci.getPlayer().heal(6);
}
void burningBlood::fightStartSetup()
{
    observeThis(gm.getFightEnd());
}

void redSkull::triggered()
{
    auto temp = gm.getPlayerHealthGain();
    if (apci.getPlayer().getHealth() <= apci.getPlayer().getMaxHealth() / 2)
    {
        if (temp == observing_) {}
        else { observing_->removeObserver(this); observeThis(temp); effects::Strength(apci.getPlayer(), 3); }
    }
    else
    {
        if (temp == observing_) { observing_->removeObserver(this); observeThis(gm.getPlayerHealthLoss()); effects::Strength(apci.getPlayer(), -3); }
        else {}
    }
}
void redSkull::fightStartSetup()
{
    if (apci.getPlayer().getHealth() <= apci.getPlayer().getMaxHealth() / 2)
    {
        effects::Strength(apci.getPlayer(), 3);
        observeThis(gm.getPlayerHealthGain());
    }
    else { observeThis(gm.getPlayerHealthLoss()); }
}
void paperPhrog::onPickUp()
{
    nonpc::vulnerableMulti__ = 1.75;
}
void paperPhrog::onDropped()
{
    nonpc::vulnerableMulti__ = 1.5;
}
void selfFormingClay::fightStartSetup()
{
    observeThis(gm.getPlayerHealthLoss());
}
void selfFormingClay::turnStartSetup()
{
    trigCount = 0;
}
void selfFormingClay::triggered()
{
    ++trigCount;
    apci.getPlayer().observeEvent<gainBlockTrigger>(gm.getPlayerTurnStart(), new gainBlockTrigger(gm.getPlayerTurnStart(), 3,true), true);
}

void championsBelt::fightStartSetup()
{
    observeThis(gm.getApplyingVulnerable());
}
void championsBelt::triggered(character& enemy)
{
    effects::Weak(enemy, 1);
}
void charonsAshes::fightStartSetup()
{
    observeThis(gm.getCardExhausted());
}
void charonsAshes::triggered()
{
    effects::AOEDamage(3);
}
void magicFlower::onPickUp()
{
    pc::magicFlowerCheck = true;
}
void magicFlower::onDropped()
{
    pc::magicFlowerCheck = false;
}
void brimstone::turnStartSetup()
{
    if (!active_) return;
    effects::Strength(apci.getPlayer(), 2);
    for_each(gm.getAllEnemies().begin(), gm.getAllEnemies().end(),
        [](unique_ptr<nonpc>& e) {effects::Strength(*e, 1); });
}
void astrolabe::onPickUp()
{
    vector<deque<unique_ptr<card>>::iterator> toTransformAndUpgrade;
    int x{3};
    while(--x>=0)
    {
        if(!ai) std::cout << "Choose " << x+1 << " more cards to transform." << endl;
        auto temp = gm.chooseCardFromDeck();
        if(find(toTransformAndUpgrade.begin(),toTransformAndUpgrade.end(), temp) != toTransformAndUpgrade.end()) { ++x; continue; }
        toTransformAndUpgrade.push_back(temp);
    }
    vector<pair<int, deque<unique_ptr<card>>::iterator>> orderedITR(3);
    for(int i{0}; i<3; ++i)
    {
        orderedITR[i].second = toTransformAndUpgrade[i];
        deque<unique_ptr<card>>::iterator temp = toTransformAndUpgrade[i];
        while (temp != apci.getDeck().end()) { ++orderedITR[i].first; ++temp; }
    }
    sort(orderedITR.begin(), orderedITR.end(), [](const pair<int, deque<unique_ptr<card>>::iterator>& p1, const pair<int, deque<unique_ptr<card>>::iterator>& p2)
        {
            return p1.first < p2.first;
        });
    for(int i{0}; i<3; ++i)
    {
        gm.transformCard(apci.getDeck().end() - orderedITR[i].first);
        apci.getDeck().back()->upgradeThis();
    }
}
blackStar::blackStar() : relic(relicRarity::Boss, "Black Star"), bossRelic(3, 1, 0, 1, []() {return apci.getPlayer().getHealth() > 50; }) {}

void blackStar::fightStartSetup()
{
    if(gm.getMapPathEvent() == 'E') observeThis(gm.getFightEnd());
}
void blackStar::triggered()
{
    gm.eliteRelicReward();
}
void bustedCrown::onPickUp()
{
    gamestate::cardRewardNumber -= 2;
    apci.getPlayer().incrementStartEnergy();
}
void bustedCrown::onDropped()
{
    gamestate::cardRewardNumber += 2;
    apci.getPlayer().decrementStartEnergy();
}
void callingBell::onPickUp()
{
    apci.addCardToDeck(gm.getCurseCardByID("CurseOfTheBell_ID"));
    apci.addRelic(gm.retrieveRandomRelic());
    apci.addRelic(gm.retrieveRandomRelic());
    apci.addRelic(gm.retrieveRandomRelic());
}
void coffeeDripper::onPickUp()
{
    apci.getPlayer().incrementStartEnergy();
    gamestate::coffeeDripperCheck = true;
}
void coffeeDripper::onDropped()
{
    apci.getPlayer().decrementStartEnergy();
    gamestate::coffeeDripperCheck = false;
}
cursedKey::cursedKey() : relic(relicRarity::Boss, "Cursed Key"), bossRelic(2, 4, 1, 1, []() { return apci.fineForCurse(); }) {}

void cursedKey::onPickUp()
{
    apci.getPlayer().incrementStartEnergy();
    observeThis(gm.getChestOpened());
}
void cursedKey::onDropped()
{
    apci.getPlayer().decrementStartEnergy();
}
void cursedKey::triggered()
{
    apci.addCardToDeck(gm.getCurse());
}
void ectoplasm::onPickUp()
{
    apci.getPlayer().incrementStartEnergy();
    gamestate::ectoplasmCheck = true;
}
void ectoplasm::onDropped()
{
    apci.getPlayer().decrementStartEnergy();
    gamestate::ectoplasmCheck = false;
}
void emptyCage::onPickUp()
{
    gm.removeCardFromDeck();
    gm.removeCardFromDeck();
}
void fusionHammer::onPickUp()
{
    apci.getPlayer().incrementStartEnergy();
    gamestate::fusionHammerCheck = true;
}
void fusionHammer::onDropped()
{
    apci.getPlayer().decrementStartEnergy();
    gamestate::fusionHammerCheck = false;
}
void pandorasBox::onPickUp()
{
    auto toTrans = find_if(apci.getDeck().begin(), apci.getDeck().end(), [](const unique_ptr<card>& c_) {return c_->getCardIDNum() == 1 || c_->getCardIDNum() == 3; });
    while(toTrans!= apci.getDeck().end())
    {
        gm.transformCard(toTrans);
        toTrans = find_if(apci.getDeck().begin(), apci.getDeck().end(), [](const unique_ptr<card>& c_) {return c_->getCardIDNum() == 1 || c_->getCardIDNum() == 3; });
    }
}
void philosophersStone::onPickUp()
{
    apci.getPlayer().incrementStartEnergy();
}
void philosophersStone::onDropped()
{
    apci.getPlayer().decrementStartEnergy();
}
void philosophersStone::fightStartSetup()
{
    for_each(gm.getAllEnemies().begin(), gm.getAllEnemies().end(), [](unique_ptr<nonpc>& e) { e->gainStrength(1); });
}
runicPyramid::runicPyramid() : relic(relicRarity::Boss, "Runic Pyramid"), bossRelic(5, 5, 0, 1, []() {return !apci.checkForRelic("Snecko Eye"); }) {}

void runicPyramid::onPickUp()
{
    gamestate::runicPyramidCheck = true;
}
void runicPyramid::onDropped()
{
    gamestate::runicPyramidCheck = false;
}
void slaversCollar::turnStartSetup()
{
    if (gm.getMapPathEvent() == 'E' || gm.getMapPathEvent() == 'B') gm.increasePlayerEnergy(1);
}
sneckoEye::sneckoEye() : relic(relicRarity::Boss, "Snecko Eye"), bossRelic(3, 3, 0, 1, []() {return apci.isSneckoGood(); }) { }

void sneckoEye::onPickUp()
{
    apci.getPlayer().increaseToDrawSOTPerma(2);
    gamestate::sneckoEyeCheck = true;
}
void sneckoEye::onDropped()
{
    apci.getPlayer().decreaseToDrawSOTPerma(2);
    gamestate::sneckoEyeCheck = false;
}
void sozu::onPickUp()
{
    apci.getPlayer().incrementStartEnergy();
    gamestate::sozuCheck = true;
}
void sozu::onDropped()
{
    apci.getPlayer().decrementStartEnergy();
    gamestate::sozuCheck = false;
}
void tinyHouse::onPickUp()
{
    gm.potionReward();
    apci.gainGold(50);
    apci.getPlayer().gainMaxHP(5);
    gm.hallwayReward(true);
    gm.upgradeRandomFromDeck();
}
void velvetChoker::onPickUp()
{
    apci.getPlayer().incrementStartEnergy();
}
void velvetChoker::onDropped()
{
    apci.getPlayer().decrementStartEnergy();
}
void velvetChoker::triggered()
{
    if (++cardCounter >= 6) gamestate::denyCardPlay = true;
}
void velvetChoker::turnStartSetup()
{
    gamestate::denyCardPlay = false;
    cardCounter = 0;
}
void velvetChoker::fightStartSetup()
{
    observeThis(gm.getCardPlayed());
}

void mutagenicStrength::fightStartSetup()
{
    effects::Strength(apci.getPlayer(), 3);
    apci.getPlayer().changeEotStrength(-3);
}

void redMask::fightStartSetup()
{
    for (auto itr{ gm.getAllEnemies().begin() }; itr != gm.getAllEnemies().end(); ++itr)
    {
        effects::Weak((**itr), 1);
    }
}

void enchridion::fightStartSetup()
{
    cardHolder cc(gm.getRandomPower());
    apci.makeCostZero(cc);
    apci.addCardToHand(cc);
}
void nilrysCodex::fightStartSetup()
{
    observeThis(gm.getPlayerTurnEnd());
}
void nilrysCodex::triggered()
{
    auto cs = gm.getXRandomCardsOfType(3,Status);
    cardHolder c1 = cs[0]->second->clone();
    cardHolder c2 = cs[1]->second->clone();
    cardHolder c3 = cs[2]->second->clone();
    if (!ai) std::cout << "1 for " << (*c1).getName() << ", 2 for " << (*c2).getName() << ", 3 for " << (*c3).getName() << endl;
    int res = stoi(request_input({ "1","2","3" }));
    switch (res)
    {
    case 1:
        apci.addCardToDrawRandom(c1);
        break;
    case 2:
        apci.addCardToDrawRandom(c2);
        break;
    case 3:
        apci.addCardToDrawRandom(c3);
        break;
    }
}
void necronomicon::onPickUp()
{
    apci.addCardToDeck(gm.getCurseCardByID("Necronomicurse_ID"));
}
void necronomicon::onDropped()
{
    auto necro = find_if(apci.getDeck().begin(), apci.getDeck().end(), [](const unique_ptr<card>& c) {return c->getName() == "Necronomicurse"; });
    if (necro == apci.getDeck().end()) return;
    apci.removeCard(necro);
}
void necronomicon::triggered(cardHolder& ch)
{
    if (!thisTurn && !gamestate::dontDupe)
    {
        int eCost = (*ch).getEnergy();
        if (eCost == -1) eCost = gm.getXCost(false);
        if (eCost < 2) return;
        thisTurn = true;
        playCardDup(ch);
    }
}
void necronomicon::turnStartSetup()
{
    thisTurn = false;
}
void necronomicon::fightStartSetup()
{
    observeThis(gm.getAttackPlayed());
}

wingBoots::wingBoots() : relic(relicRarity::Rare, "Wing Boots")
{
    flyCount = &gamestate::wingBootsCheck;
}
void wingBoots::onPickUp()
{
    gamestate::wingBootsCheck = 3;
}
void wingBoots::onDropped()
{
    gamestate::wingBootsCheck = 0;
}
void toolbox::fightStartSetup()
{
    observeThis(gm.getPlayerTurnEnd());
}
void toolbox::triggered()
{
    gm.getXRandomCardsOfType(3, Attack);
}

void nlothsGift::onPickUp()
{
    gamestate::nlothsGiftCheck = true;
}
void nlothsGift::onDropped()
{
    gamestate::nlothsGiftCheck = false;
}

void markOfTheBloom::onPickUp()
{
    gamestate::markOfTheBloomCheck = true;
}
void markOfTheBloom::onDropped()
{
    gamestate::markOfTheBloomCheck = false;
}

void blackBlood::triggered()
{
    if (apci.getPlayer().getHealth() > 0) apci.getPlayer().heal(12);
}
void blackBlood::fightStartSetup()
{
    observeThis(gm.getFightEnd());
}
void blackBlood::onPickUp()
{
    if(apci.checkForRelic("Burning Blood")) apci.removeRelicByName("Burning Blood");
}

void markOfPain::onPickUp()
{
    apci.getPlayer().incrementStartEnergy();
    gamestate::markOfPainCheck = true;
}
void markOfPain::onDropped()
{
    apci.getPlayer().decrementStartEnergy();
    gamestate::markOfPainCheck = false;
}

void runicCube::triggered()
{
    apci.drawX(1);
}
void runicCube::fightStartSetup()
{
    observeThis(gm.getPlayerHealthLoss());
}

void sacredBark::triggered()
{
     
}
void sacredBark::fightStartSetup()
{
     
}

void runicDome::triggered()
{
     
}
void runicDome::fightStartSetup()
{
     
}
