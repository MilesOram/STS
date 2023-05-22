#include "ic_cards.h"
#include "effects_triggers.h"

using namespace std;

void strike::execute()
{
    nonpc& target = singleTarget();
    effects::playerAdjustedDamage(target, damageNumber);
}
void strike::upgradeThis()
{
    damageNumber = 9;
    sixCharName_ = "Strik+";
    name_ = "Strike+";
    upgraded_ = true;
}
void bash::execute()
{
    nonpc& target = singleTarget();
    if (effects::playerAdjustedDamage(target, damageNumber)) return;
    effects::Vulnerable(target, vulnerableNumber);
}

void bash::upgradeThis()
{
    damageNumber = 10;
    vulnerableNumber = 3;
    sixCharName_ = "Bash+ ";
    name_ = "Bash+";
    upgraded_ = true;
}
void defend::execute()
{
    effects::AdjustedBlock(blockNumber);
}
void defend::upgradeThis()
{
    blockNumber = 8;
    sixCharName_ = "Defnd+";
    name_ = "Defend+";
    upgraded_ = true;
}
void armaments::execute()
{
    effects::AdjustedBlock(blockNumber);
    if (targetAll)
    {
        effects::upgradeCards(apci.getHand());
    }
    else
    {
        bool valid{ false };
        for (int i{ 0 }; i < apci.getHand().size(); ++i)
        {
            if ((*apci.getHand()[i]).getType() == Attack || (*apci.getHand()[i]).getType() == Skill || (*apci.getHand()[i]).getType() == Power)
                valid = true;
        }
        if (!valid) return;
        auto temp = &singleTargetCard({ cardType::Attack,cardType::Power,cardType::Skill });
        effects::upgradeCard(*temp);
    }
}
void armaments::upgradeThis()
{
    blockNumber = 5;
    targetAll = true;
    sixCharName_ = "Armnt+";
    name_ = "Armaments+";
    upgraded_ = true;
}
void anger::execute()
{
    nonpc& target = singleTarget();
    effects::playerAdjustedDamage(target, damageNumber);
    apci.getDiscardPile().push_front(*this);
}
void anger::upgradeThis()
{
    damageNumber = 8;
    sixCharName_ = "Anger+";
    name_ = "Anger+";
    upgraded_ = true;
}
void bodyslam::execute()
{
    nonpc& target = singleTarget();
    effects::playerAdjustedDamage(target, damageNumber + apci.getPlayer().getBlock());
}
void bodyslam::upgradeThis()
{
    energy_ = 0;
    temp_energy_ = 0;
    sixCharName_ = "Bslam+";
    name_ = "Bodyslam+";
    upgraded_ = true;
}
void clash::execute()
{
    nonpc& target = singleTarget();
    effects::playerAdjustedDamage(target, damageNumber);
}
void clash::upgradeThis()
{
    damageNumber = 18;
    sixCharName_ = "Clash+";
    name_ = "Clash+";
    upgraded_ = true;
}
bool clash::castable()
{
    bool temp{ true };
    for (int i{ 0 }; i < apci.getHand().size(); ++i)
    {
        if ((*apci.getHand()[i]).getType() != cardType::Attack) temp = false;
    }
    return temp && card::castable();
}
void clothesline::execute()
{
    nonpc& target = singleTarget();
    if (effects::playerAdjustedDamage(target, damageNumber)) return;
    effects::Weak(target, weakNumber);
}
void clothesline::upgradeThis()
{
    damageNumber = 14;
    weakNumber = 3;
    sixCharName_ = "Cloth+";
    name_ = "Clothesline+";
    upgraded_ = true;
}
void flex::execute()
{
    pc& target = apci.getPlayer();
    effects::Strength(target, strengthNumber);
    target.changeEotStrength(-strengthNumber);
}
void flex::upgradeThis()
{
    strengthNumber = 4;
    sixCharName_ = "Flex+ ";
    name_ = "Flex+";
    upgraded_ = true;
}
void havoc::execute()
{
    if (apci.getDrawSize() == 0) apci.discardToDraw();
    if (apci.getDrawSize() == 0) return;
    apci.addCardToBackOfStack(std::move(apci.getDrawPile()[0]));
    apci.getDrawPile().pop_front();
}
void havoc::playCardByHavoc(cardHolder&& ch)
{
    cardHolder toPlay(ch);
    ++gamestate::nextCardsNoTarget;
    if ((*toPlay).castable())
    {
        if (gamestate::strikeDummyCheck &&
            ((*toPlay).getName().find("strike") != std::string::npos || (*toPlay).getName().find("Strike") != std::string::npos))
        {
            apci.getPlayer().observeEvent<increaseADFromPlayerTrigger>(gm.getAlterBaseADFromPlayer(), new increaseADFromPlayerTrigger(gm.getAlterBaseADFromPlayer(), 3));
        }
        playCard(toPlay);
    }
    --gamestate::nextCardsNoTarget;
    if (!(*toPlay).getExhaust() && (*toPlay).getType() != cardType::Power)
    {
        if (gamestate::strangeSpoonCheck) {
            if (random_int(0, 1)) { gamestate::strangeSpoonLE = true; return; }
            else { gamestate::strangeSpoonLE = false; }
        }
        effects::Exhaust(toPlay);
    }
}
void havoc::upgradeThis()
{
    energy_ = 0;
    temp_energy_ = 0;
    sixCharName_ = "Havoc+";
    name_ = "Havoc+";
    upgraded_ = true;
}
void shrugItOff::execute()
{
    effects::AdjustedBlock(blockNumber);
    apci.drawX(1);
}
void shrugItOff::upgradeThis()
{
    blockNumber = 11;
    sixCharName_ = "Shrug+";
    name_ = "Shrug It Off+";
    upgraded_ = true;
}
void pommelStrike::execute()
{
    nonpc& target = singleTarget();
    effects::playerAdjustedDamage(target, damageNumber);
    apci.drawX(drawNumber);
}
void pommelStrike::upgradeThis()
{
    damageNumber = 10;
    drawNumber = 2;
    sixCharName_ = "PmlSt+";
    name_ = "Pommel Strike+";
    upgraded_ = true;
}
void headbutt::execute()
{
    nonpc& target = singleTarget();
    effects::playerAdjustedDamage(target, damageNumber);
    if (gm.checkAllEnemyDeath())
    {
        if (ai && !gamestate::aiSimActionBuffer.empty() && !gamestate::randomAIChoice && (gamestate::aiSimActionBuffer.size() % 2 != 0 || gamestate::orderedChoiceTree[0].size()==1))
            gamestate::aiSimActionBuffer.pop_front();
        return;
    }
    if (apci.getDiscardSize() == 0) return;

    auto target2 = &singleTargetCard(apci.getDiscardPile());
    cardDeckCH* temp = &apci.getDiscardPile();
    for (int i{ 0 }; i < temp->size(); ++i)
    {
        if (target2 == &(*temp)[i])
        {
            apci.addCardToDrawFront((*temp)[i]);
            temp->erase(temp->begin() + i);
        }
    }
}
void headbutt::upgradeThis()
{
    damageNumber = 12;
    sixCharName_ = "Headb+";
    name_ = "Headbutt+";
    upgraded_ = true;
}
void ironwave::execute()
{
    nonpc& target = singleTarget();
    effects::playerAdjustedDamage(target, damageNumber);
    effects::AdjustedBlock(blockNumber);
}
void ironwave::upgradeThis()
{
    damageNumber = 7;
    blockNumber = 7;
    sixCharName_ = "Ironw+";
    name_ = "Ironwave+";
    upgraded_ = true;
}
void heavyBlade::execute()
{
    nonpc& target = singleTarget();
    effects::playerAdjustedDamage(target, damageNumber + (apci.getPlayer().getTotalStrength() * (strengthMulti - 1)));
}
void heavyBlade::upgradeThis()
{
    strengthMulti = 5;
    sixCharName_ = "HvyBl+";
    name_ = "Heavy Blade+";
    upgraded_ = true;
}
void perfectedStrike::execute()
{
    nonpc& target = singleTarget();
    int damage{ 0 };
    for (int i{ 0 }; i < apci.getDeck().size(); ++i)
    {
        stringstream temp(apci.getDeck()[i]->getName());
        string c;
        while (temp >> c)
        {
            if (c == "strike" || c == "Strike" || c == "strike+" || c == "Strike+") { damage += perStrikeIncrease; break; }
        }
    }
    effects::playerAdjustedDamage(target, damageNumber + damage);
}
void perfectedStrike::upgradeThis()
{
    perStrikeIncrease = 3;
    sixCharName_ = "PrfSt+";
    name_ = "Perfected Strike+";
    upgraded_ = true;
}
void swordBoomerang::execute()
{
    effects::multiRandomAttackDamage(damageNumber, multiAttack);
}
void swordBoomerang::upgradeThis()
{
    multiAttack = 4;
    sixCharName_ = "SBoom+";
    name_ = "Sword Boomerang+";
    upgraded_ = true;
}
void thunderclap::execute()
{
    for (int i{ 0 }; i < gm.getAllEnemies().size(); ++i)
    {
        nonpc& target = *(gm.getAllEnemies()[i]);
        if (effects::playerAdjustedDamage(target, damageNumber)) continue;
        effects::Vulnerable(target, vulnerableNumber);
    }
}
void thunderclap::upgradeThis()
{
    damageNumber = 7;
    sixCharName_ = "Thclp+";
    name_ = "Thunderclap+";
    upgraded_ = true;
}
void trueGrit::execute()
{
    effects::AdjustedBlock(blockNumber);
    try {
        upgraded_ ? effects::exhaustHandCard(singleTargetCard()) :
            effects::exhaustHandCard(randomTargetCard());
    }
    catch (runtime_error) { return; }
}
void trueGrit::upgradeThis()
{
    blockNumber = 9;
    sixCharName_ = "TrueG+";
    name_ = "True Grit+";
    upgraded_ = true;
}
void twinStrike::execute()
{
    nonpc& target = singleTarget();
    effects::singleTargetMulti(apci.getPlayer(), target, damageNumber, multiAttack);
}
void twinStrike::upgradeThis()
{
    damageNumber = 7;
    sixCharName_ = "TwnSt+";
    name_ = "Twin Strike+";
    upgraded_ = true;
}
void warCry::execute()
{
    apci.drawX(drawNumber);
    if (apci.getHand().size() == 0) return;
    cardHolder temp = singleTargetCard();
    cardDeckCH* temp2 = &apci.getHand();
    for (int i{ 0 }; i < temp2->size(); ++i)
    {
        if (temp.getID() == (*temp2)[i].getID())
        {
            apci.addCardToDrawFront((*temp2)[i]);
            apci.removeHandCard(apci.getHand().begin() + i);
            break;
        }
    }
}
void warCry::upgradeThis()
{
    drawNumber = 2;
    sixCharName_ = "WrCry+";
    name_ = "War Cry+";
    upgraded_ = true;
}
void wildStrike::execute()
{
    nonpc& target = singleTarget();
    effects::playerAdjustedDamage(target, damageNumber);
    apci.addCardToDrawRandom(cardHolder(gm.getStatusCardByID("Wound_ID")->clone()));
}
void wildStrike::upgradeThis()
{
    damageNumber = 17;
    sixCharName_ = "WildS+";
    name_ = "Wild Strike+";
    upgraded_ = true;
}
// uncommon

void battleTrance::execute()
{
    apci.drawX(drawNumber);
    apci.getPlayer().noDrawDebuff();
}
void battleTrance::upgradeThis()
{
    drawNumber = 4;
    sixCharName_ = "BTrnc+";
    name_ = "Battle Trance+";
    upgraded_ = true;
}
void bloodForBlood::execute()
{
    nonpc& target = singleTarget();
    effects::playerAdjustedDamage(target, damageNumber);
}
void bloodForBlood::upgradeThis()
{
    damageNumber = 22;
    energy_ = 3;
    temp_energy_ = 3;
    sixCharName_ = "BldFB+";
    name_ = "Blood For Blood+";
    upgraded_ = true;
}
void bloodForBlood::fightStart(bool midFight)
{
    observeEvent<incrementCostTrigger<card>>(gm.getPlayerHealthLoss(), new incrementCostTrigger<card>(this, gm.getPlayerHealthLoss()));
    card::fightStart(midFight);
}
int bloodForBlood::getEnergy() const 
{
    if (zeroUntilPlayed) { return 0; }
    else if (oneUntilPlayed) { return clamp(temp_energy_ - lessEnergy, 0, 1); }
    else { return clamp(temp_energy_ - lessEnergy, 0, 9); }
}

void bloodletting::execute()
{
    apci.getPlayer().loseHealth(3);
    gm.increasePlayerEnergy(energyNumber);
}
void bloodletting::upgradeThis()
{
    energyNumber = 3;
    sixCharName_ = "Bldlt+";
    name_ = "Bloodletting+";
    upgraded_ = true;
}
void burningPact::execute()
{
    if (!apci.getHand().empty())
    {
        auto target = singleTargetCard();
        effects::exhaustHandCard(target);
    }
    apci.drawX(drawNumber);
}
void burningPact::upgradeThis()
{
    drawNumber = 3;
    sixCharName_ = "BrnPc+";
    name_ = "Burning Pact+";
    upgraded_ = true;
}
void carnage::execute()
{
    nonpc& target = singleTarget();
    effects::playerAdjustedDamage(target, damageNumber);
}
void carnage::upgradeThis()
{
    damageNumber = 28;
    sixCharName_ = "Carng+";
    name_ = "Carnage+";
    upgraded_ = true;
}
void combust::execute()
{
    apci.getPlayer().addCMBbuff(damageNumber);
}
void combust::upgradeThis()
{
    damageNumber = 7;
    sixCharName_ = "Cmbst+";
    name_ = "Combust+";
    upgraded_ = true;
}
void darkEmbrace::execute()
{
    apci.getPlayer().addDRKbuff(1);
}
void darkEmbrace::upgradeThis()
{
    energy_ = 1;
    temp_energy_ = 1;
    sixCharName_ = "DrkEm+";
    name_ = "Dark Embrace+";
    upgraded_ = true;
}
void disarm::execute()
{
    nonpc& target = singleTarget();
    effects::Strength(target, -strengthNumber);
}
void disarm::upgradeThis()
{
    strengthNumber = 3;
    sixCharName_ = "Disar+";
    name_ = "Disarm+";
    upgraded_ = true;
}
void dropkick::execute()
{
    nonpc& target = singleTarget();
    bool vul = target.getVulnerable();
    effects::playerAdjustedDamage(target, damageNumber);
    if (vul)
    {
        apci.drawX(1);
        gm.increasePlayerEnergy(1);
    }
}
void dropkick::upgradeThis()
{
    damageNumber = 8;
    sixCharName_ = "DropK+";
    name_ = "Dropkick+";
    upgraded_ = true;
}
void dualWield::execute()
{
    if (apci.numberInHand({Attack,Power}) == 0) return;
    auto target = singleTargetCard({ cardType::Attack,cardType::Power });
    for (int i{ 0 }; i < copyNumber; ++i)
    {
        apci.addCardToHand(target);
    }
}
void dualWield::upgradeThis()
{
    copyNumber = 2;
    sixCharName_ = "DualW+";
    name_ = "Dual Wield+";
    upgraded_ = true;
}
void entrench::execute()
{
    effects::Block(apci.getPlayer(), apci.getPlayer().getBlock());
}
void entrench::upgradeThis()
{
    energy_ = 1;
    temp_energy_ = 1;
    sixCharName_ = "Entrn+";
    name_ = "Entrench+";
    upgraded_ = true;
}
void evolve::execute()
{
    apci.getPlayer().addEVObuff(drawNumber);
}
void evolve::upgradeThis()
{
    drawNumber = 2;
    sixCharName_ = "Evolv+";
    name_ = "Evolve+";
    upgraded_ = true;
}
void feelNoPain::execute()
{
    apci.getPlayer().addFNPbuff(blockNumber);
}
void feelNoPain::upgradeThis()
{
    blockNumber = 4;
    sixCharName_ = "FNoP+ ";
    name_ = "Feel No Pain+";
    upgraded_ = true;
}
void fireBreathing::execute()
{
    apci.getPlayer().addFRBbuff(damageNumber);
}
void fireBreathing::upgradeThis()
{
    damageNumber = 10;
    sixCharName_ = "FireB+";
    name_ = "Fire Breathing+";
    upgraded_ = true;
}
void flameBarrier::execute()
{
    effects::AdjustedBlock(blockNumber);
    apci.getPlayer().addFlameBar(damageNumber);
}
void flameBarrier::upgradeThis()
{
    blockNumber = 16;
    damageNumber = 6;
    sixCharName_ = "FlamB+";
    name_ = "Flame Barrier+";
    upgraded_ = true;
}
void ghostlyArmor::execute()
{
    effects::AdjustedBlock(blockNumber);
}
void ghostlyArmor::upgradeThis()
{
    blockNumber = 13;
    sixCharName_ = "GhstA+";
    name_ = "Ghost Armor+";
    upgraded_ = true;
}
void hemokinesis::execute()
{
    nonpc& target = singleTarget();
    apci.getPlayer().loseHealth(2);
    if (effects::playerAdjustedDamage(target, damageNumber)) return;
}
void hemokinesis::upgradeThis()
{
    damageNumber = 20;
    sixCharName_ = "Hemok+";
    name_ = "Hemokinesis+";
    upgraded_ = true;
}
void infernalBlade::execute()
{
    auto options = gm.getXRandomCardsOfType(1, Attack);
    auto c = cardHolder(options[0]->second->clone());
    apci.makeCostZero(c);
    (*c).fightStart(true);
    apci.addCardToHand(c);
}
void infernalBlade::upgradeThis()
{
    energy_ = 0;
    temp_energy_ = 0;
    sixCharName_ = "InfBl+";
    name_ = "Infernal Blade+";
    upgraded_ = true;
}
void inflame::execute()
{
    effects::Strength(apci.getPlayer(), strengthNumber);
}
void inflame::upgradeThis()
{
    strengthNumber = 3;
    sixCharName_ = "Inflm+";
    name_ = "Inflame+";
    upgraded_ = true;
}
void intimidate::execute()
{
    for (auto itr{ gm.getAllEnemies().begin() }; itr != gm.getAllEnemies().end(); ++itr)
    {
        effects::Weak((**itr), weakNumber);
    }
}
void intimidate::upgradeThis()
{
    weakNumber = 2;
    sixCharName_ = "Intim+";
    name_ = "Intimidate+";
    upgraded_ = true;
}
void metallicize::execute()
{
    apci.getPlayer().addMTL(blockNumber);
    apci.getPlayer().addAltBuffs(Intents("MTL", apci.getPlayer().getMetalliciseBuff(), 1));
}
void metallicize::upgradeThis()
{
    blockNumber = 4;
    sixCharName_ = "Metal+";
    name_ = "Metallicize+";
    upgraded_ = true;
}
void powerThrough::execute()
{
    effects::AdjustedBlock(blockNumber);
    apci.addCardToHand(cardHolder(gm.getStatusCardByID("Wound_ID")->clone()));
    apci.addCardToHand(cardHolder(gm.getStatusCardByID("Wound_ID")->clone()));
}
void powerThrough::upgradeThis()
{
    blockNumber = 20;
    sixCharName_ = "PowrT+";
    name_ = "Power Through+";
    upgraded_ = true;
}
void pummel::execute()
{
    nonpc& target = singleTarget();
    effects::singleTargetMulti(apci.getPlayer(), target, damageNumber, multi);
}
void pummel::upgradeThis()
{
    multi = 5;
    sixCharName_ = "Pumml+";
    name_ = "Pummel+";
    upgraded_ = true;
}
void ragee::execute()
{
    apci.getPlayer().addRage(blockNumber);
    auto blockTrig = new gainBlockTrigger(gm.getAttackPlayed(), blockNumber);
    auto haltTrig = new haltObservingTriggerCH<character>(&apci.getPlayer(), blockTrig);
    apci.getPlayer().observeEvent<gainBlockTrigger>(gm.getAttackPlayed(), blockTrig, true);
    apci.getPlayer().observeEvent<haltObservingTriggerCH<character>>(gm.getPlayerTurnEnd(), haltTrig, true);
}
void ragee::upgradeThis()
{
    blockNumber = 5;
    sixCharName_ = "Rage+ ";
    name_ = "Rage+";
    upgraded_ = true;
}
void rampage::execute()
{
    nonpc& target = singleTarget();
    effects::playerAdjustedDamage(target, damageNumber);
    damageNumber += dIncrease;
}
void rampage::upgradeThis()
{
    dIncrease = 8;
    sixCharName_ = "Rampg+";
    name_ = "Rampage+";
    upgraded_ = true;
}
void recklessCharge::execute()
{
    nonpc& target = singleTarget();
    effects::playerAdjustedDamage(target, damageNumber);
    apci.addCardToDrawRandom(cardHolder(gm.getStatusCardByID("Dazed_ID")->clone()));
}
void recklessCharge::upgradeThis()
{
    damageNumber = 10;
    sixCharName_ = "ReckC+";
    name_ = "Reckless Charge+";
    upgraded_ = true;
}
void rupture::execute()
{
    apci.getPlayer().addRUPbuff(strengthNumber);
    apci.getPlayer().observeEvent<strengthTrigger<character>>(gm.getPlayerHealthLoss(), new strengthTrigger<character>(&apci.getPlayer(), gm.getPlayerHealthLoss(), strengthNumber), true);
    apci.getPlayer().addAltBuffs(Intents("RUP", apci.getPlayer().getRuptureBuff(), 1));
}
void rupture::upgradeThis()
{
    strengthNumber = 2;
    sixCharName_ = "Ruptr+";
    name_ = "Rupture+";
    upgraded_ = true;
}
void searingBlow::execute()
{
    nonpc& target = singleTarget();
    effects::playerAdjustedDamage(target, damageNumber);
}
void searingBlow::upgradeThis()
{
    ++upgradeCount;
    damageNumber += 4 + incr;
    incr += 2;
    sixCharName_ = "Sear+" + to_string(upgradeCount);
    name_ = "Searing Blow+" + to_string(upgradeCount);
}
void secondWind::execute()
{
    int nonAttackCount{ 0 };
    cardDeckCH tempStack;
    for (int i = static_cast<int>(apci.getHand().size()) - 1; i >= 0; --i)
    {
        if ((*apci.getHand()[i]).getType() != cardType::Attack)
        {
            ++nonAttackCount;
            tempStack.push_back(apci.getHand()[i]);
            apci.removeHandCard(apci.getHand().begin() + i);
        }
    }
    for (int i{ 0 }; i < tempStack.size(); ++i)
    {
        effects::Exhaust(tempStack[i]);
    }
    nonAttackCount *= blockNumber;
    effects::AdjustedBlock(nonAttackCount);
}
void secondWind::upgradeThis()
{
    blockNumber = 7;
    sixCharName_ = "SWind+";
    name_ = "Second Wind+";
    upgraded_ = true;
}
void seeingRed::execute()
{
    gm.increasePlayerEnergy(2);
}
void seeingRed::upgradeThis()
{
    energy_ = 0;
    temp_energy_ = 0;
    sixCharName_ = "SeeRd+";
    name_ = "Seeing Red+";
    upgraded_ = true;
}
void sentinel::execute()
{
    effects::AdjustedBlock(blockNumber);
}
void sentinel::upgradeThis()
{
    energyNumber = 3;
    sixCharName_ = "Sentl+";
    name_ = "Sentinel+";
    upgraded_ = true;
}
void sentinel::fightStart(bool midFight)
{
    observeEvent<gainEnergyTrigger2<card>>(gm.getCardExhausted(), new gainEnergyTrigger2<card>(this, gm.getCardExhausted(), energyNumber));
    card::fightStart(midFight);
}
void severSoul::execute()
{
    for (int i = static_cast<int>(apci.getHand().size()) - 1; i >= 0; --i)
    {
        if ((*apci.getHand()[i]).getType() != cardType::Attack)
        {
            effects::exhaustHandCard(apci.getHand()[i]);
        }
    }
    nonpc& target = singleTarget();
    effects::playerAdjustedDamage(target, damageNumber);
}
void severSoul::upgradeThis()
{
    damageNumber = 22;
    sixCharName_ = "SevrS+";
    name_ = "Sever Soul+";
    upgraded_ = true;
}
void shockwave::execute()
{
    for (auto itr{ gm.getAllEnemies().begin() }; itr != gm.getAllEnemies().end(); ++itr)
    {
        effects::Weak((**itr), weakNumber);
        effects::Vulnerable((**itr), vulnerableNumber);
    }
}
void shockwave::upgradeThis()
{
    weakNumber = 5;
    vulnerableNumber = 5;
    sixCharName_ = "ShkWv+";
    name_ = "Shockwave+";
    upgraded_ = true;
}
void spotWeakness::execute()
{
    if (ai)
    {
        if (gm.getAllEnemies().empty()) return;
        if (gamestate::nextCardsNoTarget > 0)
        {
            return;
        }
        else if (gamestate::nextCardSameTarget)
        {
            int j = gamestate::lastAttackedSCRNIndex;
            for (int i{ 0 }; i < gm.getIntents(j).size(); ++i)
            {
                if (gm.getIntents(j)[i].effect == "Attack")
                {
                    effects::Strength(apci.getPlayer(), strengthNumber);
                    gamestate::lastAttackedSCRNIndex = j;
                    break;
                }
            }
            return;
        }
        else
        {
            for (int j{ 0 }; j < gm.getAllEnemies().size(); ++j)
            {
                for (int i{ 0 }; i < gm.getIntents(j).size(); ++i)
                {
                    if (gm.getIntents(j)[i].effect == "Attack")
                    {
                        effects::Strength(apci.getPlayer(), strengthNumber);
                        gamestate::lastAttackedSCRNIndex = j;
                        break;
                    }
                }
            }
        }
        return;
    }
    nonpc& target = singleTarget();
    auto intents_ = gm.getIntents(target.getSCRNIndex());
    for (auto itr{ intents_.begin() }; itr != intents_.end(); ++itr)
    {
        if (itr->effect == "Attack") { effects::Strength(apci.getPlayer(), strengthNumber); break; }
    }
}
void spotWeakness::upgradeThis()
{
    strengthNumber = 4;
    sixCharName_ = "SpotW+";
    name_ = "Spot Weakness+";
    upgraded_ = true;
}
void uppercut::execute()
{
    nonpc& target = singleTarget();
    if (effects::playerAdjustedDamage(target, damageNumber)) return;
    effects::Weak(target, weakNumber);
    effects::Vulnerable(target, vulnerableNumber);
}
void uppercut::upgradeThis()
{
    damageNumber = 13;
    vulnerableNumber = 2;
    weakNumber = 2;
    sixCharName_ = "Upper+";
    name_ = "Uppercut+";
    upgraded_ = true;
}
void whirlwind::execute()
{
    if (multiX == 0) { multiX = gm.getXCost(false); }
    effects::multiAOEAttackDamage(damageNumber, multiX);
    if(!gamestate::dontDupe) observeEvent<resetXMultiTrigger<card>>(gm.getAttackPlayed(), new resetXMultiTrigger<card>(this, gm.getAttackPlayed()));
}
void whirlwind::upgradeThis()
{
    damageNumber = 8;
    sixCharName_ = "Whirl+";
    name_ = "Whirlwind+";
    upgraded_ = true;
}

void barricade::execute()
{
    apci.getPlayer().addAltBuffs({ Intents("BAR",1000,1) });
    apci.getPlayer().changeSOTBlocksObs(new barricadeTrigger<character>(&apci.getPlayer(), gm.getSOTBlock()));
}
void barricade::upgradeThis()
{
    energy_ = 2;
    temp_energy_ = 2;
    sixCharName_ = "Barri+";
    name_ = "Barricade+";
    upgraded_ = true;
}
void berserk::execute()
{
    effects::Vulnerable(apci.getPlayer(), vulnerableNumber);
    apci.getPlayer().addBSKbuff(1);
    apci.getPlayer().observeEvent<gainEnergyTrigger>(gm.getPlayerTurnStart(), new gainEnergyTrigger(), true);
    apci.getPlayer().addAltBuffs(Intents("BSK", apci.getPlayer().getBerserkBuff(), 1));
}
void berserk::upgradeThis()
{
    vulnerableNumber = 1;
    sixCharName_ = "Bersk+";
    name_ = "Berserk+";
    upgraded_ = true;
}
void bludgeon::execute()
{
    nonpc& target = singleTarget();
    effects::playerAdjustedDamage(target, damageNumber);
}
void bludgeon::upgradeThis()
{
    damageNumber = 42;
    sixCharName_ = "Bldgn+";
    name_ = "Bludgeon+";
    upgraded_ = true;
}
void brutality::execute()
{
    apci.getPlayer().addBRTbuff(1);
}
void brutality::upgradeThis()
{
    innate_ = true;
    sixCharName_ = "Bruta+";
    name_ = "Brutality+";
    upgraded_ = true;
}
void corruption::execute()
{
    card::corruptionCheck = true;
    apci.getPlayer().addCORbuff(1);
    apci.getPlayer().addAltBuffs(Intents("COR", apci.getPlayer().getCorruptionBuff(), 1));
}
void corruption::upgradeThis()
{
    energy_ = 2;
    temp_energy_ = 2;
    sixCharName_ = "Corrp+";
    name_ = "Corruption+";
    upgraded_ = true;
}
void demonForm::execute()
{
    apci.getPlayer().observeEvent<strengthTrigger<character>>(gm.getPlayerTurnStart(), new strengthTrigger<character>(&apci.getPlayer(), gm.getPlayerTurnStart(), strengthNumber), true);
    apci.getPlayer().addDMNbuff(strengthNumber);
    apci.getPlayer().addAltBuffs(Intents("DMN", apci.getPlayer().getDemonFormBuff(), 1));
}
void demonForm::upgradeThis()
{
    strengthNumber = 3;
    sixCharName_ = "Demon+";
    name_ = "Demon Form+";
    upgraded_ = true;
}
void doubleTap::execute()
{
    apci.getPlayer().addDUPDoubleTap(doublePlayNumber);
}
void doubleTap::upgradeThis()
{
    doublePlayNumber = 2;
    sixCharName_ = "Doubl+";
    name_ = "Double Tap+";
    upgraded_ = true;
}
void exhume::execute()
{
    if (apci.getExhausted().size() == 0) return;
    auto target = &singleTargetCard(apci.getExhausted());
    apci.sendCardFromToHand(apci.getExhausted(), *target);
}
void exhume::upgradeThis()
{
    energy_ = 0;
    temp_energy_ = 0;
    sixCharName_ = "Exhum+";
    name_ = "Exhume+";
    upgraded_ = true;
}
void feed::execute()
{
    nonpc& target = singleTarget();
    auto feedTrig = new maxHPTrigger(gm.getEnemyFatal(), healthNumber);
    auto haltTrig = new haltObservingTriggerCH<character>(&apci.getPlayer(),gm.getCardPlayed(), feedTrig,1);
    apci.getPlayer().observeEvent<maxHPTrigger>(gm.getEnemyFatal(), feedTrig, true);
    apci.getPlayer().observeEvent<haltObservingTriggerCH<character>>(gm.getCardPlayed(), haltTrig, true);
    effects::playerAdjustedDamage(target, damageNumber);
}
void feed::upgradeThis()
{
    damageNumber = 12;
    healthNumber = 4;
    sixCharName_ = "Feed+ ";
    name_ = "Feed+";
    upgraded_ = true;
}
void fiendFire::execute()
{
    nonpc& target = singleTarget();
    int n = static_cast<int>(apci.getHand().size());
    effects::exhaustHand();
    effects::singleTargetMulti(apci.getPlayer(), target, damageNumber, n);
}
void fiendFire::upgradeThis()
{
    damageNumber = 10;
    sixCharName_ = "Fiend+";
    name_ = "Fiend Fire+";
    upgraded_ = true;
}
void immolate::execute()
{
    effects::AOEAttackDamge(damageNumber);
    apci.addCardToDiscard(cardHolder(gm.getStatusCardByID("Burn_ID")->clone()));
}
void immolate::upgradeThis()
{
    damageNumber = 28;
    sixCharName_ = "Immol+";
    name_ = "Immolate+";
    upgraded_ = true;
}
void impervious::execute()
{
    effects::AdjustedBlock(blockNumber);
}
void impervious::upgradeThis()
{
    blockNumber = 40;
    sixCharName_ = "Imper+";
    name_ = "Impervious+";
    upgraded_ = true;
}
void juggernaut::execute()
{
    apci.getPlayer().addJGRbuff(damageNumber);
}
void juggernaut::upgradeThis()
{
    damageNumber = 7;
    sixCharName_ = "Juggr+";
    name_ = "Juggernaut+";
    upgraded_ = true;
}
void limitBreak::execute()
{
    if (apci.getPlayer().getTotalStrength() <= 0) return;
    apci.getPlayer().gainStrength(apci.getPlayer().getTotalStrength());
}
void limitBreak::upgradeThis()
{
    exhaust_ = false;
    sixCharName_ = "LimtB+";
    name_ = "Limit Break+";
    upgraded_ = true;
}
void offering::execute()
{
    apci.getPlayer().loseHealth(lifeLoss);
    gm.increasePlayerEnergy(energyGain);
    apci.drawX(drawNumber);
}
void offering::upgradeThis()
{
    drawNumber = 5;
    sixCharName_ = "Offri+";
    name_ = "Offering+";
    upgraded_ = true;
}
void reaper::execute()
{
    int healthPool{ 0 };
    for (int i{ 0 }; i < gm.getAllEnemies().size(); ++i)
    {
        healthPool += gm.getAllEnemies()[i]->getHealth();
    }
    effects::AOEAttackDamge(damageNumber);
    for (int i{ 0 }; i < gm.getAllEnemies().size(); ++i)
    {
        healthPool -= gm.getAllEnemies()[i]->getHealth();
    }
    if (healthPool > 0) apci.getPlayer().heal(healthPool);
}
void reaper::upgradeThis()
{
    damageNumber = 5;
    sixCharName_ = "Reapr+";
    name_ = "Reaper+";
    upgraded_ = true;
}

// curses

bool regret::endStepDiscardPrio()
{
    int life = static_cast<int>(apci.getHand().size());
    apci.getPlayer().loseHealth(life);
    return true;
}
void pain::fightStart(bool midFight)
{
    observeEvent<painLoseLifeTrigger<card>>(gm.getCardPlayed(), new painLoseLifeTrigger<card>(this, gm.getCardPlayed()));
    card::fightStart(midFight);
}
bool IC_CARDS_H::decay::endStepDiscard()
{
    apci.getPlayer().takeDamageNoTrigger(2);
    return true;
}
bool doubt::endStepDiscard()
{
    apci.getPlayer().applyWeak(1);
    return true;
}
void necronomicurse::fightStart(bool midFight)
{
    observeEvent<necroCurseTrigger<card>>(gm.getCardExhausted(), new necroCurseTrigger<card>(this, gm.getCardExhausted()));
    card::fightStart(midFight);
}
void normality::fightStart(bool midFight)
{
    card::fightStart(midFight);
}
bool shame::endStepDiscard()
{
    apci.getPlayer().applyFrail(1);
    return true;
}
void injury::execute() { if (card::blueCandleCheck) apci.getPlayer().loseHealth(1); }
void regret::execute() { if (card::blueCandleCheck) apci.getPlayer().loseHealth(1); }
void pain::execute() { if (card::blueCandleCheck) apci.getPlayer().loseHealth(1); }
void doubt::execute() { if (card::blueCandleCheck) apci.getPlayer().loseHealth(1); }
void parasite::execute() { if (card::blueCandleCheck) apci.getPlayer().loseHealth(1); }
void necronomicurse::execute() { if (card::blueCandleCheck) apci.getPlayer().loseHealth(1); }
void clumsy::execute() { if (card::blueCandleCheck) apci.getPlayer().loseHealth(1); }
void curseOfTheBell::execute() { if (card::blueCandleCheck) apci.getPlayer().loseHealth(1); }
void IC_CARDS_H::decay::execute() { if (card::blueCandleCheck) apci.getPlayer().loseHealth(1); }
void normality::execute() { if (card::blueCandleCheck) apci.getPlayer().loseHealth(1); }
void writhe::execute() { if (card::blueCandleCheck) apci.getPlayer().loseHealth(1); }
void shame::execute() { if (card::blueCandleCheck) apci.getPlayer().loseHealth(1); }

// statuses
bool burn::endStepDiscard()
{
    apci.getPlayer().takeDamageNoTrigger(damageNumber);
    return true;
}
void burn::upgradeStatus()
{
    damageNumber = 4;
    sixCharName_ = "Burn +";
    name_ = "Burn+";
    upgraded_ = true;
}
void voidx::fightStart(bool midFight)
{
    observeEvent<voidTrigger<card>>(gm.getCardDrawn(), new voidTrigger<card>(this, gm.getCardDrawn()));
    card::fightStart(midFight);
}
// colourless

void bite::execute()
{
    nonpc& target = singleTarget();
    effects::playerAdjustedDamage(target, damageNumber);
    apci.getPlayer().heal(healNumber);
}
void bite::upgradeThis()
{
    damageNumber = 8;
    healNumber = 3;
    sixCharName_ = "Bite +";
    name_ = "Bite+";
    upgraded_ = true;
}

void jax::execute()
{
    apci.getPlayer().loseHealth(hpNumber);
    effects::Strength(apci.getPlayer(), strengthNumber);
}
void jax::upgradeThis()
{
    strengthNumber = 3;
    sixCharName_ = "Jax + ";
    name_ = "Jax+";
    upgraded_ = true;
}

void apparition::execute()
{
    apci.getPlayer().gainIntangible(1);
}
void apparition::upgradeThis()
{
    ethereal_ = false;
    sixCharName_ = "Appar+";
    name_ = "Apparition+";
    upgraded_ = true;
}

void ritualDagger::execute()
{
    nonpc& target = singleTarget();

    auto ritPermTrig = new ritualDaggerFatalPermTrigger(gm.getEnemyFatal(),damageNumber);
    auto haltTrig = new haltObservingTriggerCH<character>(&apci.getPlayer(), gm.getCardPlayed(), ritPermTrig);
    apci.getPlayer().observeEvent<ritualDaggerFatalPermTrigger>(gm.getEnemyFatal(), ritPermTrig, true);
    apci.getPlayer().observeEvent<haltObservingTriggerCH<character>>(gm.getCardPlayed(), haltTrig, true);

    auto ritTrig = new ritualDaggerFatalTrigger<card>(this,gm.getEnemyFatal());
    auto haltTrig2 = new haltObservingTriggerCH<character>(&apci.getPlayer(), gm.getCardPlayed(), ritTrig);
    apci.getPlayer().observeEvent<ritualDaggerFatalTrigger<card>>(gm.getEnemyFatal(), ritTrig, true);
    apci.getPlayer().observeEvent<haltObservingTriggerCH<character>>(gm.getCardPlayed(), haltTrig2, true);

    effects::playerAdjustedDamage(target, damageNumber);
}
void ritualDagger::upgradeThis()
{
    damageIncr = 5;
    sixCharName_ = "RitDg+";
    name_ = "Ritual Dagger+";
    upgraded_ = true;
}


void bandageUp::execute()
{
    apci.getPlayer().heal(healNumber);
}
void bandageUp::upgradeThis()
{
    healNumber = 6;
    sixCharName_ = "Bandg+";
    name_ = "Bandage Up+";
    upgraded_ = true;
}

void blind::execute()
{
    if (upgraded_)
    {
        for (auto itr{ gm.getAllEnemies().begin() }; itr != gm.getAllEnemies().end(); ++itr)
        {
            effects::Weak((**itr), 2);
        }
    }
    else
    {
        nonpc& target = singleTarget();
        effects::Weak(target, 2);
    }
}
void blind::upgradeThis()
{
    pickChoice = combatChoices::None;
    sixCharName_ = "Blind+";
    name_ = "Blind+";
    upgraded_ = true;
}

void darkShackles::execute()
{
    nonpc& target = singleTarget();
    if (target.getArtifact())
    {
        effects::Strength(target, -strengthNumber);
    }
    else
    {
        effects::Strength(target, -strengthNumber);
        target.changeEotStrength(strengthNumber);
    }
}
void darkShackles::upgradeThis()
{
    strengthNumber = 15;
    sixCharName_ = "DkShk+";
    name_ = "Dark Shackles+";
    upgraded_ = true;
}

void deepBreath::execute()
{
    apci.discardToDraw();
    apci.drawX(drawNumber);
}
void deepBreath::upgradeThis()
{
    drawNumber = 2;
    sixCharName_ = "DeepB+";
    name_ = "Deep Breath+";
    upgraded_ = true;
}

void discovery::execute()
{
    auto options = gm.getXRandomCardsOfType(3);
    gm.chooseFromThree(options,false);
}
void discovery::upgradeThis()
{
    exhaust_ = false;
    sixCharName_ = "Dscvr+";
    name_ = "Deep Breath+";
    upgraded_ = true;
}

void dramaticEntrance::execute()
{
    effects::AOEAttackDamge(damageNumber);
}
void dramaticEntrance::upgradeThis()
{
    damageNumber = 12;
    sixCharName_ = "DrmaE+";
    name_ = "Dramatic Entrance+";
    upgraded_ = true;
}

void enlightenment::execute()
{
    if (upgraded_)
    {
        for (int i{ 0 }; i < apci.getHand().size(); ++i)
        {
            if ((*apci.getHand()[i]).getZeroUntilPlayed() || (*apci.getHand()[i]).getEnergy() <= 1) continue;
            (*apci.getHand()[i]).changeTempEnergy(1);
        }
    }
    else
    {
        for (int i{ 0 }; i < apci.getHand().size(); ++i)
        {
            if ((*apci.getHand()[i]).getZeroUntilPlayed() || (*apci.getHand()[i]).getEnergy() <= 1) continue;
            apci.makeCostOne(apci.getHand()[i]);
        }
    }
}
void enlightenment::upgradeThis()
{
    sixCharName_ = "Enlgh+";
    name_ = "Enlightenment+";
    upgraded_ = true;
}

void finesse::execute()
{
    effects::AdjustedBlock(blockNumber);
    apci.drawX(1);
}
void finesse::upgradeThis()
{
    blockNumber = 4;
    sixCharName_ = "Finss+";
    name_ = "Finesse+";
    upgraded_ = true;
}

void flashOfSteel::execute()
{
    nonpc& target = singleTarget();
    effects::playerAdjustedDamage(target, damageNumber);
    apci.drawX(1);
}
void flashOfSteel::upgradeThis()
{
    damageNumber = 6;
    sixCharName_ = "FlshS+";
    name_ = "Flash Of Steel+";
    upgraded_ = true;
}

void forethought::execute()
{
    if (upgraded_)
    {
        if (apci.getHand().size() == 0) return;
        if(!ai) std::cout << "Select cards to place on bottom of draw. 0 until played (eg 1 3 4 8 9)" << endl;
        set<int> temp = getIndicesForCardBoolFlip(1, static_cast<int>(apci.getHand().size()), static_cast<int>(apci.getHand().size()));
        auto itr{ temp.begin() };
        int i{ 1 };
        while (itr != temp.end())
        {
            apci.makeCostZero(apci.getHand()[*itr-i]);
            apci.addCardToDrawBack(apci.getHand()[*itr-i]);
            apci.removeHandCard(apci.getHand().begin() + (*itr - i));
            ++i; ++itr;
        }
    }
    else
    {
        if (apci.getHand().size() == 0) return;
        if (!ai) std::cout << "Select card to place on bottom of draw. 0 until played." << endl;
        cardHolder temp = singleTargetCard();
        cardDeckCH* temp2 = &apci.getHand();
        for (int i{ 0 }; i < temp2->size(); ++i)
        {
            if (temp.getID() == (*temp2)[i].getID())
            {
                apci.makeCostZero(( * temp2)[i]);
                apci.addCardToDrawBack((*temp2)[i]);
                apci.removeHandCard(apci.getHand().begin() + i);
                break;
            }
        }
    }
}
void forethought::upgradeThis()
{
    pickChoice = combatChoices::HandMultiAll;
    sixCharName_ = "Foret+";
    name_ = "Forethought+";
    upgraded_ = true;
}

void goodInstincts::execute()
{
    effects::AdjustedBlock(blockNumber);
}
void goodInstincts::upgradeThis()
{
    blockNumber = 9;
    sixCharName_ = "GoodI+";
    name_ = "Good Instincts+";
    upgraded_ = true;
}

void impatience::execute()
{
    bool res{ true };
    for (int i{ 0 }; i < apci.getHand().size(); ++i)
    {
        if ((*apci.getHand()[i]).getType() == Attack) { res = false; break;}
    }
    if(res) apci.drawX(drawNumber);
}
void impatience::upgradeThis()
{
    drawNumber = 3;
    sixCharName_ = "Impat+";
    name_ = "Impatience+";
    upgraded_ = true;
}

void jackOfAllTrades::execute()
{
    auto c_s = gm.getXRandomColourlessCards(cardNumber);
    for (int i{ 0 }; i < c_s.size(); ++i)
    {
        apci.addCardToHand(c_s[i]->second->clone());
    }
}
void jackOfAllTrades::upgradeThis()
{
    cardNumber = 2;
    sixCharName_ = "JkOAT+";
    name_ = "Jack Of All Trades+";
    upgraded_ = true;
}

void madness::execute()
{
    if (apci.getHand().empty()) return;
    vector<int> validOpts;
    for (int i{ 0 }; i < apci.getHand().size(); ++i)
    {
        if ((*apci.getHand()[i]).getBaseEnergy() <= 0) continue;
        validOpts.push_back(i);
    }
    if (validOpts.empty()) return;
    int rng = validOpts[random_int(0, validOpts.size() - 1)];
    (*apci.getHand()[rng]).changeTempEnergy(0);
}
void madness::upgradeThis()
{
    energy_ = 0;
    temp_energy_ = 0;
    sixCharName_ = "Madns+";
    name_ = "Madness+";
    upgraded_ = true;
}

void mindBlast::execute()
{
    nonpc& target = singleTarget();
    effects::playerAdjustedDamage(target, apci.getDrawSize());
}
void mindBlast::upgradeThis()
{
    energy_ = 1;
    temp_energy_ = 1;
    sixCharName_ = "MindB+";
    name_ = "Mind Blast+";
    upgraded_ = true;
}

void panacea::execute()
{
    apci.getPlayer().gainArtifactCharge(artifactNumber);
}
void panacea::upgradeThis()
{
    artifactNumber = 2;
    sixCharName_ = "Pncea+";
    name_ = "Panacea+";
    upgraded_ = true;
}

void panicButton::execute()
{
    effects::AdjustedBlock(blockNumber);
    apci.getPlayer().noBlockDebuff(2);
}
void panicButton::upgradeThis()
{
    blockNumber = 40;
    sixCharName_ = "PncBt+";
    name_ = "Panic Button+";
    upgraded_ = true;
}

void purity::execute()
{
    if(!ai) std::cout << "Select up to " << cardNumber << " cards to exhaust(eg 1 3 4)" << endl;
    set<int> temp = getIndicesForCardBoolFlip(1, static_cast<int>(apci.getHand().size()),cardNumber);
    vector<cardHolder> temps;
    auto itr{ temp.begin() };
    int i{ 1 };
    while (itr != temp.end())
    {
        temps.push_back(apci.getHand()[*itr - i]);
        apci.removeHandCard(apci.getHand().begin() + (*itr - i));
        ++i; ++itr;
    }
    for (int j{ 0 }; j < temp.size(); ++j)
    {
        effects::Exhaust(*temps[j]);
    }
}
void purity::upgradeThis()
{
    cardNumber = 5;
    sixCharName_ = "Purty+";
    name_ = "Purity+";
    upgraded_ = true;
}

void swiftStrike::execute()
{
    nonpc& target = singleTarget();
    effects::playerAdjustedDamage(target, damageNumber);
}
void swiftStrike::upgradeThis()
{
    damageNumber = 10;
    sixCharName_ = "SwftS+";
    name_ = "Swift Strike+";
    upgraded_ = true;
}

void trip::execute()
{
    if (upgraded_)
    {
        for (auto itr{ gm.getAllEnemies().begin() }; itr != gm.getAllEnemies().end(); ++itr)
        {
            effects::Vulnerable((**itr), 2);
        }
    }
    else
    {
        nonpc& target = singleTarget();
        effects::Vulnerable(target, 2);
    }
}
void trip::upgradeThis()
{
    pickChoice = combatChoices::None;
    sixCharName_ = "Trip +";
    name_ = "Trip+";
    upgraded_ = true;
}

void apotheosis::execute()
{
    effects::upgradeCards(apci.getDrawPile());
    effects::upgradeCards(apci.getHand());
    effects::upgradeCards(apci.getDiscardPile());
    effects::upgradeCards(apci.getExhausted());
}
void apotheosis::upgradeThis()
{
    temp_energy_ = 1;
    energy_ = 1;
    sixCharName_ = "Apoth+";
    name_ = "Apotheosis+";
    upgraded_ = true;
}

void chrysalis::execute()
{
    auto c_s = gm.getXRandomCardsOfType(cardNumber, Skill);
    for (int i{ 0 }; i < cardNumber; ++i)
    {
        cardHolder temp(c_s[i]->second->clone());
        (*temp).fightStart(true);
        (*temp).changeTempEnergy(0);
        apci.addCardToDrawRandom(temp);
    }
}
void chrysalis::upgradeThis()
{
    cardNumber = 5;
    sixCharName_ = "Chrys+";
    name_ = "Chrysalis+";
    upgraded_ = true;
}

void handOfGreed::execute()
{
    nonpc& target = singleTarget();
    auto greedTrig = new goldTrigger(gm.getEnemyFatal(), bothNumber);
    auto haltTrig = new haltObservingTriggerCH<character>(&apci.getPlayer(), gm.getCardPlayed(), greedTrig,1);
    apci.getPlayer().observeEvent<goldTrigger>(gm.getEnemyFatal(), greedTrig, true);
    apci.getPlayer().observeEvent<haltObservingTriggerCH<character>>(gm.getCardPlayed(), haltTrig, true);
    effects::playerAdjustedDamage(target, bothNumber);
}
void handOfGreed::upgradeThis()
{
    bothNumber = 25;
    sixCharName_ = "HOGrd+";
    name_ = "Hand Of Greed+";
    upgraded_ = true;
}

void magnetism::execute()
{
    apci.getPlayer().addMAGbuff(1);
}
void magnetism::upgradeThis()
{
    energy_ = 1;
    temp_energy_ = 1;
    sixCharName_ = "Magnt+";
    name_ = "Magnetism+";
    upgraded_ = true;
}

void masterOfStrategy::execute()
{
    apci.drawX(drawNumber);
}
void masterOfStrategy::upgradeThis()
{
    drawNumber = 4;
    sixCharName_ = "MasSt+";
    name_ = "Master Of Strategy+";
    upgraded_ = true;
}

void mayhem::execute()
{
    apci.getPlayer().addMAYbuff(1);
}
void mayhem::upgradeThis()
{
    energy_ = 1;
    temp_energy_ = 1;
    sixCharName_ = "Mayhm+";
    name_ = "Mayhem+";
    upgraded_ = true;
}

void metamorphisis::execute()
{
    auto c_s = gm.getXRandomCardsOfType(cardNumber, Attack);
    for (int i{ 0 }; i < cardNumber; ++i)
    {
        cardHolder temp(c_s[i]->second->clone());
        (*temp).fightStart(true);
        (*temp).changeTempEnergy(0);
        apci.addCardToDrawRandom(temp);
    }
}
void metamorphisis::upgradeThis()
{
    cardNumber = 5;
    sixCharName_ = "Metam+";
    name_ = "Metamorphisis+";
    upgraded_ = true;
}

void panache::execute()
{
    apci.getPlayer().addPANbuff(damageNumber);
}
void panache::upgradeThis()
{
    damageNumber = 14;
    sixCharName_ = "Panch+";
    name_ = "Panache+";
    upgraded_ = true;
}

void sadisticNature::execute()
{
    apci.getPlayer().addSADbuff(damageNumber);
}
void sadisticNature::upgradeThis()
{
    damageNumber = 7;
    sixCharName_ = "SadNa+";
    name_ = "Sadistic Nature+";
    upgraded_ = true;
}

void secretTechnique::execute()
{
    if (apci.getDrawSize() == 0) return;
    int index = gm.chooseCardFromDraw(Skill);
    if (index == -1) return;
    apci.addCardToHand(apci.getDrawPile()[index]);
    apci.removeDrawCard(apci.getDrawPile().begin() + index);
}
void secretTechnique::upgradeThis()
{
    exhaust_ = false;
    sixCharName_ = "SecTe+";
    name_ = "Secret Technique+";
    upgraded_ = true;
}

void secretWeapon::execute()
{
    if (apci.getDrawSize() == 0) return;
    int index = gm.chooseCardFromDraw(Attack);
    if (index == -1) return;
    apci.addCardToHand(apci.getDrawPile()[index]);
    apci.removeDrawCard(apci.getDrawPile().begin() + index);
}
void secretWeapon::upgradeThis()
{
    exhaust_ = false;
    sixCharName_ = "SecWe+";
    name_ = "Secret Weapon+";
    upgraded_ = true;
}

void theBomb::execute()
{
    apci.getPlayer().applyBomb(damageNumber, 3);
}
void theBomb::upgradeThis()
{
    damageNumber = 50;
    sixCharName_ = "TheBm+";
    name_ = "The Bomb+";
    upgraded_ = true;
}

void thinkingAhead::execute()
{
    apci.drawX(2);
    if (apci.getHand().size() == 0) return;
    cardHolder temp = singleTargetCard();
    cardDeckCH* temp2 = &apci.getHand();
    for (int i{ 0 }; i < temp2->size(); ++i)
    {
        if (temp.getID() == (*temp2)[i].getID())
        {
            apci.addCardToDrawFront((*temp2)[i]);
            apci.removeHandCard(apci.getHand().begin() + i);
            break;
        }
    }
}
void thinkingAhead::upgradeThis()
{
    exhaust_ = false;
    sixCharName_ = "ThAhd+";
    name_ = "Thinking Ahead+";
    upgraded_ = true;
}

void transmutation::execute()
{
    if (multiX == 0) { multiX = gm.getXCost(false); }
    observeEvent<resetXMultiTrigger<card>>(gm.getSkillPlayed(), new resetXMultiTrigger<card>(this, gm.getSkillPlayed()));

    auto c_s = gm.getXRandomColourlessCards(multiX);
    for (int i{ 0 }; i < multiX; ++i)
    {
        cardHolder temp(c_s[i]->second->clone());
        if (upgraded_) (*temp).upgradeThis();
        (*temp).fightStart(true);
        apci.makeCostZero(temp);
        apci.addCardToDrawRandom(temp);
    }
}
void transmutation::upgradeThis()
{
    sixCharName_ = "Trnsm+";
    name_ = "Transmutation+";
    upgraded_ = true;
}

void violence::execute()
{
    int x{ cardNumber };
    while (--x >= 0)
    {
        vector<int> validOpts;
        for (int i{ 0 }; i < apci.getDrawSize(); ++i)
        {
            if ((*apci.getDrawPile()[i]).getType() != Attack) continue;
            validOpts.push_back(i);
        }
        if (validOpts.size() == 0) break;
        int rng = random_int(0, validOpts.size() - 1);
        apci.addCardToHand(apci.getDrawPile()[validOpts[rng]]);
        validOpts.erase(validOpts.begin() + rng);
    }
}
void violence::upgradeThis()
{
    cardNumber = 4;
    sixCharName_ = "Violn+";
    name_ = "Violence+";
    upgraded_ = true;
}

void cleave::execute()
{
    effects::AOEAttackDamge(damageNumber);
}
void cleave::upgradeThis()
{
    damageNumber = 11;
    sixCharName_ = "Cleav+";
    name_ = "Cleave+";
    upgraded_ = true;
}