#include "savestate.h"
#include "gamestate.h"
#include "events.h"
#include "card.h"
#include "pc_info.h"
#include "character.h"
#include "visuals.h"
#include "potions.h"
#include "ic_cards.h"

using namespace std;

void savestate::saveGame()
{
    clearAll();
    player_ = apci.getPlayer().saveClone();
    for (int i{ 0 }; i < apci.getRelics().size(); ++i)
    {
        relics_.push_back(apci.getRelics()[i]->clone());
        if(apci.getRelics()[i]->getObserving()) relics_.back()->observeThis(apci.getRelics()[i]->getObserving());
        relics_.back()->setInactive();
    }
    for (int i{ 0 }; i < apci.getPotions().size(); ++i)
    {
        auto temp = apci.getPotion(i)->clone();
        auto it = dynamic_cast<fairyInABottle*>(temp);
        if (it != nullptr)
        {
            it->setInactive();
        }
        potions_.push_back(temp);
    }
    for (int i{ 0 }; i < gm.getAllEnemies().size(); ++i)
    {
        enemies_.push_back(gm.getAllEnemies()[i]->saveClone());
    }
    for (int i{ 0 }; i < enemies_.size(); ++i)
    {
        if (enemies_[i]->getSOTBlocks())
        {
            enemies_[i]->getSOTBlocks()->setInactive();
            enemies_[i]->getSOTBlocks()->getObserving()->addObserver(enemies_[i]->getSOTBlocks());
        }
    }
    dormants_ = { {},{},{} };
    for (int i{ 0 }; i < gm.getDormantEnemies().size(); ++i)
    {
        for (int j{ 0 }; j < gm.getDormantEnemies()[i].size(); ++j)
        {
            dormants_[i].push_back(gm.getDormantEnemies()[i][j]->saveClone());
        }
    }
    gm_ints_.push_back(gm.getPlayerEnergy());
    gm_ints_.push_back(gm.getXCost());
    gm_ints_.push_back(gm.getDAM());
    gm_ints_.push_back(gm.getActNumber());
    gm_ints_.push_back(gm.cardCount);
    gm_ints_.push_back(gm.attackCount);
    gm_ints_.push_back(gm.skillCount);
    gm_ints_.push_back(gm.powerCount);
    gm_ints_.push_back(gm.turnCount);

    apci_ints.push_back(apci.getGold());
    apci_ints.push_back(apci.getTopCardKnown());
    
    gm_static_bools_.push_back(gamestate::sneckoEyeCheck);
    gm_static_bools_.push_back(gamestate::runicPyramidCheck);
    gm_static_bools_.push_back(gamestate::iceCreamCheck);
    gm_static_bools_.push_back(gamestate::prayerWheelCheck);
    gm_static_bools_.push_back(gamestate::frozenEyeCheck);
    gm_static_bools_.push_back(gamestate::strikeDummyCheck);
    gm_static_bools_.push_back(gamestate::tungstenCheck);
    gm_static_bools_.push_back(gamestate::chemicalXCheck);
    gm_static_bools_.push_back(gamestate::strangeSpoonCheck);
    gm_static_bools_.push_back(gamestate::strangeSpoonLE);
    gm_static_bools_.push_back(gamestate::nextCardSameTarget);
    gm_static_bools_.push_back(gamestate::denyCardPlay);
    gm_static_bools_.push_back(gamestate::dontDupe);

    gm_static_bools_.push_back(gamestate::markOfPainCheck);
    gm_static_bools_.push_back(gamestate::markOfTheBloomCheck);

    gm_static_ints_.push_back(gamestate::nextCardsNoTarget);
    gm_static_ints_.push_back(gamestate::lastAttackedSCRNIndex);

    gm_static_ints_.push_back(gamestate::omamoriCheck);

    gm_intents_ = gm.getAllIntents();

    card_static_bools_.push_back(card::blueCandleCheck);
    card_static_bools_.push_back(card::medKitCheck);
    card_static_bools_.push_back(card::canPlayAttack);
    card_static_bools_.push_back(card::corruptionCheck);

    nonpc_static_floats_.push_back(nonpc::vulnerableMulti__);
    nonpc_static_floats_.push_back(nonpc::weakMulti__);

    pc_static_bools_.push_back(pc::magicFlowerCheck);

    for (int i{ 0 }; i < apci.getHand().size(); ++i)
    {
        cards_hand_.push_back(apci.getHand()[i]);
        for (auto itr{ (*cards_hand_.back()).getObservers().begin() }; itr != (*cards_hand_.back()).getObservers().end(); ++itr)
        {
            (**itr).setInactive();
        }
    }
    for (int i{ 0 }; i < apci.getDrawPile().size(); ++i)
    {
        cards_draw_.push_back(apci.getDrawPile()[i]);
        for (auto itr{ (*cards_draw_.back()).getObservers().begin() }; itr != (*cards_draw_.back()).getObservers().end(); ++itr)
        {
            (**itr).setInactive();
        }
    }
    for (int i{ 0 }; i < apci.getDiscardPile().size(); ++i)
    {
        cards_discard_.push_back(apci.getDiscardPile()[i]);
        for (auto itr{ (*cards_discard_.back()).getObservers().begin() }; itr != (*cards_discard_.back()).getObservers().end(); ++itr)
        {
            (**itr).setInactive();
        }
    }
    for (int i{ 0 }; i < apci.getExhausted().size(); ++i)
    {
        cards_exhaust_.push_back(apci.getExhausted()[i]);
        for (auto itr{ (*cards_exhaust_.back()).getObservers().begin() }; itr != (*cards_exhaust_.back()).getObservers().end(); ++itr)
        {
            (**itr).setInactive();
        }
    }
}
void savestate::loadGame()
{
    for (int i{ 0 }; i < relics_.size(); ++i) relics_[i]->setActive();
    for (int i{ 0 }; i < potions_.size(); ++i) 
    {
        auto it = dynamic_cast<fairyInABottle*>(potions_[i]);
        if (it != nullptr)
        {
            it->setActive();
        }
    }
    for (int i{ 0 }; i < cards_hand_.size(); ++i)
    {
        for (auto itr{ (*cards_hand_[i]).getObservers().begin() }; itr != (*cards_hand_[i]).getObservers().end(); ++itr)
        {
            (**itr).setActive();
        }
    }
    for (int i{ 0 }; i < cards_draw_.size(); ++i)
    {
        for (auto itr{ (*cards_draw_[i]).getObservers().begin() }; itr != (*cards_draw_[i]).getObservers().end(); ++itr)
        {
            (**itr).setActive();
        }
    }
    for (int i{ 0 }; i < cards_discard_.size(); ++i)
    {
        for (auto itr{ (*cards_discard_[i]).getObservers().begin() }; itr != (*cards_discard_[i]).getObservers().end(); ++itr)
        {
            (**itr).setActive();
        }
    }
    for (int i{ 0 }; i < cards_exhaust_.size(); ++i)
    {
        for (auto itr{ (*cards_exhaust_[i]).getObservers().begin() }; itr != (*cards_exhaust_[i]).getObservers().end(); ++itr)
        {
            (**itr).setActive();
        }
    }
    for (auto itr{ player_->getObservers().begin() }; itr != player_->getObservers().end(); ++itr)
    {
        (*itr)->setActive();
    }
    if (player_->getSOTBlocks()) player_->getSOTBlocks()->setActive();
    for (auto n : enemies_)
    {
        for (auto itr{ n->getObservers().begin() }; itr != n->getObservers().end(); ++itr)
        {
            (*itr)->setActive();
        }
        if (n->getSOTBlocks()) n->getSOTBlocks()->setActive();
    }

    gamestate::sneckoEyeCheck = gm_static_bools_[0];
    gamestate::runicPyramidCheck = gm_static_bools_[1];
    gamestate::iceCreamCheck = gm_static_bools_[2];
    gamestate::prayerWheelCheck = gm_static_bools_[3];
    gamestate::frozenEyeCheck = gm_static_bools_[4];
    gamestate::strikeDummyCheck = gm_static_bools_[5];
    gamestate::tungstenCheck = gm_static_bools_[6];
    gamestate::chemicalXCheck = gm_static_bools_[7];
    gamestate::strangeSpoonCheck = gm_static_bools_[8];
    gamestate::strangeSpoonLE = gm_static_bools_[9];
    gamestate::nextCardSameTarget = gm_static_bools_[10];
    gamestate::denyCardPlay = gm_static_bools_[11];
    gamestate::dontDupe = gm_static_bools_[12];

    gamestate::markOfPainCheck = gm_static_bools_[13];
    gamestate::markOfTheBloomCheck = gm_static_bools_[14];

    gamestate::nextCardsNoTarget = gm_static_ints_[0];
    gamestate::lastAttackedSCRNIndex = gm_static_ints_[1];

    gamestate::omamoriCheck = gm_static_ints_[2];

    card::blueCandleCheck = card_static_bools_[0];
    card::medKitCheck = card_static_bools_[1];
    card::canPlayAttack = card_static_bools_[2];
    card::corruptionCheck = card_static_bools_[3];

    nonpc::vulnerableMulti__ = nonpc_static_floats_[0];
    nonpc::weakMulti__ = nonpc_static_floats_[1];

    pc::magicFlowerCheck = pc_static_bools_[0];

    gm.loadEnergy(gm_ints_[0]);
    gm.loadXCost(gm_ints_[1]);
    gm.loadDAM(gm_ints_[2]);
    gm.loadActNumber(gm_ints_[3]);
    gm.loadCounts(gm_ints_[4], gm_ints_[5], gm_ints_[6], gm_ints_[7], gm_ints_[8]);
    gm.loadEnemies(enemies_);
    gm.loadDormantEnemies(dormants_);
    gm.loadIntents(gm_intents_);
    apci.loadVars(apci_ints[0],apci_ints[1]);
    apci.loadNewDiscard(cards_discard_);
    apci.loadNewDraw(cards_draw_);
    apci.loadNewExhaust(cards_exhaust_);
    apci.loadNewHand(cards_hand_);
    apci.loadNewPlayer(player_);
    apci.loadNewPotions(potions_);
    apci.loadNewRelics(relics_);
}
savestate::~savestate()
{
    delete player_;
    for (auto r : relics_) delete r; relics_.clear();
    for (auto p : potions_) delete p; potions_.clear();
    for (auto e : enemies_) delete e; enemies_.clear();
    cards_draw_.clear();
    cards_discard_.clear();
    cards_hand_.clear();
    cards_exhaust_.clear();
    gm_ints_.clear();
    apci_ints.clear();
}

void savestate::clearAll()
{
    player_ = nullptr;
    relics_.clear();
    potions_.clear();
    enemies_.clear();
    cards_draw_.clear();
    cards_discard_.clear();
    cards_hand_.clear();
    cards_exhaust_.clear();
    gm_ints_.clear();
    gm_static_ints_.clear();
    gm_static_bools_.clear();
    gm_intents_.clear();
    apci_ints.clear();
    card_static_bools_.clear();
}



void macro_savestate::saveGame()
{
    clearAll();
    player_ = apci.getPlayer().saveClone();
    for (int i{ 0 }; i < apci.getRelics().size(); ++i)
    {
        relics_.push_back(apci.getRelics()[i]->clone());
        if (apci.getRelics()[i]->getObserving()) relics_.back()->observeThis(apci.getRelics()[i]->getObserving());
        relics_.back()->setInactive();
    }
    for (int i{ 0 }; i < apci.getPotions().size(); ++i)
    {
        auto temp = apci.getPotion(i)->clone();
        auto it = dynamic_cast<fairyInABottle*>(temp);
        if (it != nullptr)
        {
            it->setInactive();
        }
        potions_.push_back(temp);
    }

    apci_ints.push_back(apci.getGold());
    apci_ints.push_back(apci.getTopCardKnown());

    gm_static_bools_.push_back(gamestate::sneckoEyeCheck);
    gm_static_bools_.push_back(gamestate::runicPyramidCheck);
    gm_static_bools_.push_back(gamestate::iceCreamCheck);
    gm_static_bools_.push_back(gamestate::prayerWheelCheck);
    gm_static_bools_.push_back(gamestate::frozenEyeCheck);
    gm_static_bools_.push_back(gamestate::strikeDummyCheck);
    gm_static_bools_.push_back(gamestate::tungstenCheck);
    gm_static_bools_.push_back(gamestate::chemicalXCheck);
    gm_static_bools_.push_back(gamestate::strangeSpoonCheck);
    gm_static_bools_.push_back(gamestate::strangeSpoonLE);
    gm_static_bools_.push_back(gamestate::nextCardSameTarget);
    gm_static_bools_.push_back(gamestate::denyCardPlay);
    gm_static_bools_.push_back(gamestate::dontDupe);

    card_static_bools_.push_back(card::blueCandleCheck);
    card_static_bools_.push_back(card::medKitCheck);
    card_static_bools_.push_back(card::canPlayAttack);
    card_static_bools_.push_back(card::corruptionCheck);

    for (int i{ 0 }; i < apci.getDeck().size(); ++i)
    {
        deck_.push_back(apci.getDeck()[i]->cloneU());
    }
}
void macro_savestate::loadGame()
{
    for (int i{ 0 }; i < relics_.size(); ++i) relics_[i]->setActive();
    for (int i{ 0 }; i < potions_.size(); ++i)
    {
        auto it = dynamic_cast<fairyInABottle*>(potions_[i]);
        if (it != nullptr)
        {
            it->setActive();
        }
    }
    for (auto itr{ player_->getObservers().begin() }; itr != player_->getObservers().end(); ++itr)
    {
        (*itr)->setActive();
    }

    gamestate::sneckoEyeCheck = gm_static_bools_[0];
    gamestate::runicPyramidCheck = gm_static_bools_[1];
    gamestate::iceCreamCheck = gm_static_bools_[2];
    gamestate::prayerWheelCheck = gm_static_bools_[3];
    gamestate::frozenEyeCheck = gm_static_bools_[4];
    gamestate::strikeDummyCheck = gm_static_bools_[5];
    gamestate::tungstenCheck = gm_static_bools_[6];
    gamestate::chemicalXCheck = gm_static_bools_[7];
    gamestate::strangeSpoonCheck = gm_static_bools_[8];
    gamestate::strangeSpoonLE = gm_static_bools_[9];
    gamestate::nextCardSameTarget = gm_static_bools_[10];
    gamestate::denyCardPlay = gm_static_bools_[11];
    gamestate::dontDupe = gm_static_bools_[12];

    card::blueCandleCheck = card_static_bools_[0];
    card::medKitCheck = card_static_bools_[1];
    card::canPlayAttack = card_static_bools_[2];
    card::corruptionCheck = card_static_bools_[3];

    apci.loadVars(apci_ints[0],apci_ints[2]);
    apci.loadNewPlayer(player_);
    apci.loadNewPotions(potions_);
    apci.loadNewRelics(relics_);
}
macro_savestate::~macro_savestate()
{
    delete player_;
    for (auto r : relics_) delete r; relics_.clear();
    for (auto p : potions_) delete p; potions_.clear();
    deck_.clear();
}

void macro_savestate::clearAll()
{
    player_ = nullptr;
    relics_.clear();
    potions_.clear();
    deck_.clear();
    gm_static_bools_.clear();
    apci_ints.clear();
    card_static_bools_.clear();
}