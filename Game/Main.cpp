#include <iostream>
#ifdef _MSC_VER
#include <CodeAnalysis/Warnings.h> // ALL_CODE_ANALYSIS_WARNINGS
#pragma warning (push)
#pragma warning (disable : ALL_CODE_ANALYSIS_WARNINGS)
#endif
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#ifdef _MSC_VER
#pragma warning (pop)
#endif
#include <string>
#include <functional>

namespace py = pybind11;

#include "card.h"
#include "character.h"
#include "effects_triggers.h"
#include "enemies.h"
#include "events.h"
#include "gamestate.h"
#include "ic_cards.h"
#include "pc_info.h"
#include "relic.h"	
#include "potions.h"	
#include "savestate.h"
#include "visuals.h"

using namespace std;
int callCount{ 0 };
class pythonFunctions
{
private:
    pybind11::module_ MacroModule;
    pybind11::module_ MicroModule;
    pybind11::module_ PredModule;

    //MICRO
    unique_ptr<pybind11::function> takeStateGiveAction;
    unique_ptr<pybind11::function> takeNextStateUpdateNetwork;
    unique_ptr<pybind11::function> callQVALSforSim;
    unique_ptr<pybind11::function> callAllQVALS;
    unique_ptr<pybind11::function> getBestQValIndex;
    unique_ptr<pybind11::function> loadModel_micro;
    unique_ptr<pybind11::function> trainModel_micro;

    //MACRO
    unique_ptr<pybind11::function> takeStatesUpdateNetwork;
    unique_ptr<pybind11::function> callExpectedforSim;
    unique_ptr<pybind11::function> getBestExpectedIndex;
    unique_ptr<pybind11::function> getBestExpectedValue;
    unique_ptr<pybind11::function> loadModel_macro;

    //PRED
    unique_ptr<pybind11::function> updatePred;
    unique_ptr<pybind11::function> callAllPredSingle;
    unique_ptr<pybind11::function> callAllPredBatch;
    unique_ptr<pybind11::function> loadModel_pred;


public:
    pythonFunctions(pybind11::module_ _micro) : MicroModule(_micro)
    {
        takeStateGiveAction = make_unique<pybind11::function>(MicroModule.attr("takeStateGiveAction"));
        takeNextStateUpdateNetwork = make_unique<pybind11::function>(MicroModule.attr("takeNextStateUpdateNetwork"));
        callQVALSforSim = make_unique<pybind11::function>(MicroModule.attr("callQVALSforSim"));
        callAllQVALS = make_unique<pybind11::function>(MicroModule.attr("callAllQVALS"));
        getBestQValIndex = make_unique<pybind11::function>(MicroModule.attr("getBestQValIndex"));
        loadModel_micro = make_unique<pybind11::function>(MicroModule.attr("loadModel"));
        trainModel_micro = make_unique<pybind11::function>(MicroModule.attr("trainModel"));
    }
    void addOther(pybind11::module_ _macro)
    {
        MacroModule = _macro;
        takeStatesUpdateNetwork = make_unique<pybind11::function>(MacroModule.attr("takeStatesUpdateNetwork"));
        callExpectedforSim = make_unique<pybind11::function>(MacroModule.attr("callExpectedforSim"));
        getBestExpectedIndex = make_unique<pybind11::function>(MacroModule.attr("getBestExpectedIndex"));
        getBestExpectedValue = make_unique<pybind11::function>(MacroModule.attr("getBestExpectedValue"));
        loadModel_macro = make_unique<pybind11::function>(MacroModule.attr("loadModel"));
    }
    void addPred(pybind11::module_ _pred)
    {
        PredModule = _pred;
        updatePred = make_unique<pybind11::function>(PredModule.attr("takeStatesUpdateNetwork"));
        callAllPredSingle = make_unique<pybind11::function>(PredModule.attr("callAllPredS"));
        callAllPredBatch = make_unique<pybind11::function>(PredModule.attr("callAllPredB"));
        loadModel_pred = make_unique<pybind11::function>(PredModule.attr("loadModel"));
    }
    pybind11::function& ACTION_MICRO() { ++callCount;  return *takeStateGiveAction.get(); }
    pybind11::function& UPDATE_MICRO() { return *takeNextStateUpdateNetwork.get(); }
    pybind11::function& QSIM_MICRO() { ++callCount; return *callQVALSforSim.get(); }
    pybind11::function& QALL_MICRO() { ++callCount; return *callAllQVALS.get(); }
    pybind11::function& BESTQ_MICRO() { ++callCount; return *getBestQValIndex.get(); }
    pybind11::function& LOAD_MICRO() { return *loadModel_micro.get(); }
    pybind11::function& TRAIN_MICRO() { return *trainModel_micro.get(); }

    pybind11::function& UPDATE_MACRO() { return *takeStatesUpdateNetwork.get(); }
    pybind11::function& EXPSIM_MACRO() { ++callCount; return *callExpectedforSim.get(); }
    pybind11::function& BESTEXP_MACRO() { ++callCount; return *getBestExpectedIndex.get(); }
    pybind11::function& BESTEXPVAL_MACRO() { ++callCount; return *getBestExpectedValue.get(); }
    pybind11::function& LOAD_MACRO() { return *loadModel_macro.get(); }

    pybind11::function& UPDATE_PRED() { return *updatePred.get(); }
    pybind11::function& CALL_S_PRED() { ++callCount; return *callAllPredSingle.get(); }
    pybind11::function& CALL_B_PRED() { ++callCount; return *callAllPredBatch.get(); }
    pybind11::function& LOAD_PRED() { return *loadModel_pred.get(); }

};

int callMicroIndex(const std::vector<stateInfo>& vecSINFOS, pythonFunctions& pyFunc_)
{
    vector<vector<vector<int>>> allSimmedSINFO;
    vector<vector<bool>> allSimmedSINFOMasks;
    for (int i{ 0 }; i < vecSINFOS.size(); ++i)
    {
        allSimmedSINFO.push_back({ vecSINFOS[i].getEmbeddingInputsCA(), vecSINFOS[i].getEmbeddingInputsEN(), vecSINFOS[i].getEmbeddingInputsPO(), vecSINFOS[i].getNormalInputs() });
        allSimmedSINFOMasks.push_back(vecSINFOS[i].getMasks());
    }
    return pyFunc_.BESTQ_MICRO()(allSimmedSINFO, allSimmedSINFOMasks).cast<int>();

}
vector<float> callMicroValues(const std::vector<stateInfo>& vecSINFOS, pythonFunctions& pyFunc_)
{
    vector<vector<vector<int>>> allSimmedSINFO;
    vector<vector<bool>> allSimmedSINFOMasks;
    for (int i{ 0 }; i < vecSINFOS.size(); ++i)
    {
        allSimmedSINFO.push_back({ vecSINFOS[i].getEmbeddingInputsCA(), vecSINFOS[i].getEmbeddingInputsEN(), vecSINFOS[i].getEmbeddingInputsPO(), vecSINFOS[i].getNormalInputs() });
        allSimmedSINFOMasks.push_back(vecSINFOS[i].getMasks());
    }
    return pyFunc_.QALL_MICRO()(allSimmedSINFO, allSimmedSINFOMasks).cast<vector<float>>();
}


void showAIDecisions(pythonFunctions& pyFunc_)
{
    //if (ai) return;
    stateInfo SINFO;
    SINFO.createAllVars();
    pyFunc_.ACTION_MICRO()(SINFO.getNormalInputs(), SINFO.getMasks(), SINFO.getEmbeddingInputsCA(), SINFO.getEmbeddingInputsEN(),SINFO.getEmbeddingInputsPO(), true, 27); // 27 prints it
}

int callMacroIndexCONSTPRED(const vector<macroStateInfo>& MSIs, pythonFunctions& pyFunc_)
{
    if (MSIs.size() == 0) return 0;
    if (gm.getRandomMacroThisFloor()) return random_int(0, MSIs.size() - 1);

    vector<vector<int>> pathVars;
    vector<vector<float>> pathPreds(MSIs.size(), callPredSINGLE(MSIs[0], pyFunc_));
    for (int i{ 0 }; i < MSIs.size(); ++i)
    {
        pathVars.push_back(MSIs[i].getNormalInputs());
    }
    return pyFunc_.BESTEXP_MACRO()(pathVars, pathPreds).cast<int>();
}
int callMacroIndexREROLLPRED(const vector<macroStateInfo>& MSIs, pythonFunctions& pyFunc_)
{
    if (MSIs.size() == 0) return 0;
    if (gm.getRandomMacroThisFloor()) return random_int(0, MSIs.size() - 1);

    vector<vector<int>> pathVars;
    vector<vector<float>> pathPreds(callPredBATCH(MSIs, pyFunc_));
    for (int i{ 0 }; i < MSIs.size(); ++i)
    {
        pathVars.push_back(MSIs[i].getNormalInputs());
    }
    return pyFunc_.BESTEXP_MACRO()(pathVars, pathPreds).cast<int>();
}
float callMacroSINGLE(const macroStateInfo& MSI, pythonFunctions& pyFunc_)
{
    if (gm.getRandomMacroThisFloor()) return random_float(0.0f, 50.0f);

    return pyFunc_.EXPSIM_MACRO()(MSI.getNormalInputs(),callPredSINGLE(MSI,pyFunc_)).cast<float>();
}
vector<float> callPredSINGLE(const macroStateInfo& MSI, pythonFunctions& pyFunc_)
{
    return pyFunc_.CALL_S_PRED()(MSI.getPredInputs()).cast<vector<float>>();
}
vector<vector<float>> callPredBATCH(const vector<macroStateInfo>& MSIs, pythonFunctions& pyFunc_)
{
    vector<vector<int>> pathPreds;
    for (int i{ 0 }; i < MSIs.size(); ++i)
    {
        pathPreds.push_back(MSIs[i].getPredInputs());
    }
    return pyFunc_.CALL_B_PRED()(pathPreds).cast<vector<vector<float>>>();
}
float callMacroValueCONSTPRED(const vector<macroStateInfo>& MSIs, pythonFunctions& pyFunc_)
{
    if (MSIs.size() == 0) return 0;
    if (gm.getRandomMacroThisFloor()) return random_float(0.0f, 50.0f);

    vector<vector<int>> pathVars;
    vector<vector<float>> pathPreds(MSIs.size(),callPredSINGLE(MSIs[0],pyFunc_));
    for (int i{ 0 }; i < MSIs.size(); ++i)
    {
        pathVars.push_back(MSIs[i].getNormalInputs());
    }
    return pyFunc_.BESTEXPVAL_MACRO()(pathVars, pathPreds).cast<float>();
}
float callMacroValueREROLLPRED(const vector<macroStateInfo>& MSIs, pythonFunctions& pyFunc_)
{
    if (MSIs.size() == 0) return 0;
    if (gm.getRandomMacroThisFloor()) return random_float(0.0f, 50.0f);

    vector<vector<int>> pathVars;
    vector<vector<float>> pathPreds(callPredBATCH(MSIs,pyFunc_));
    for (int i{ 0 }; i < MSIs.size(); ++i)
    {
        pathVars.push_back(MSIs[i].getNormalInputs());
    }
    return pyFunc_.BESTEXPVAL_MACRO()(pathVars, pathPreds).cast<float>();
}

bool ai{ true };
bool fullRuns{ false };
all_pc_info apci;
gamestate gm;
savestate sv;
float rewardMultiplier{ 0.04f };

void outerTurnLoopAI(pythonFunctions& pyFunc)
{
    int startHP = apci.getPlayer().getHealth();
    vector<unique_ptr<potion>> holdPots;
    if (apci.getPotions().size() != 0 && !apci.checkForEntropic())
    {
        bool useOne = apci.checkForRelic("White Beast Statue") || (apci.potionsFull() && gm.getPotionChance() > 60);
        vector<int> IDsToUse = apci.bestPotionsInFight(useOne);
        for (int i{ 0 }; i < apci.getPotions().size(); ++i)
        {
            if (apci.getPotion(i)->getName() == "Fairy In A Bottle") continue;
            if (find(IDsToUse.begin(), IDsToUse.end(), apci.getPotion(i)->getPotionIDNum()) == IDsToUse.end())
            {
                holdPots.push_back(unique_ptr<potion>(apci.getPotion(i)));
                apci.getPotions().erase(apci.getPotions().begin() + i);
                --i;
            }
            else
            {
                IDsToUse.erase(find(IDsToUse.begin(), IDsToUse.end(), apci.getPotion(i)->getPotionIDNum()));
            }
        }
    }
    bool showThisFight = random_int(1, 50) == 50;
    while (true)
    {
        //apci.initVisuals();
        //showAIDecisions(gm.getPyFunc());
        if (gm.getMapPathEvent() == 'B' && gm.getActNumber()>1 || showThisFight)
        {
            apci.initVisuals();
            if(random_int(1,10)==10) showAIDecisions(gm.getPyFunc());
        }
        stateInfo SINFO;
        SINFO.createAllVars();
        auto currentState = SINFO.getNormalInputs();
        auto currentStateCA = SINFO.getEmbeddingInputsCA();
        auto currentStateEN = SINFO.getEmbeddingInputsEN();
        auto currentStatePO = SINFO.getEmbeddingInputsPO();
        auto currentMasks = SINFO.getMasks();
        if (!holdPots.empty() && pyFunc.QSIM_MICRO()(currentStateCA, currentStateEN, currentStatePO, currentState, currentMasks).cast<float>() < 1.5f)
        {
            for (int i{ 0 }; i < holdPots.size(); ++i)
            {
                apci.getPotions().push_back(holdPots[i].release());
            }
            holdPots.clear();
        }

        vector<bool> nextMasks(296, true);
        int action;
        action = pyFunc.ACTION_MICRO()(currentState, currentMasks, currentStateCA, currentStateEN, currentStatePO, false, 0).cast<int>();
        float reward{ 0 };
        bool done{ false };

        auto nextState = SINFO.getNormalInputs();
        auto nextStateCA = SINFO.getEmbeddingInputsCA();
        auto nextStateEN = SINFO.getEmbeddingInputsEN();
        auto nextStatePO = SINFO.getEmbeddingInputsPO();
        //int oldHealth = apci.getPlayer().getHealth();
        int oldMaxHP = apci.getPlayer().getMaxHealth();
        int oldBlock = apci.getPlayer().getBlock();
        int oldEnergy = gm.getPlayerEnergy();
        int oldGold = apci.getGold();
        int oldCurses = apci.cursesInDeck();

        combatChoices actionChoices = apci.needForSim(action);
        if (actionChoices != combatChoices::None && action != 110 && action != 111)
        {

            gamestate::aiSimActionBuffer.clear();
            gamestate::aiCardBuffer.clear();
            gamestate::orderedChoiceTree.clear();

            gm.actionChoicesForSim(actionChoices, action);

            sv.saveGame();
            vector<stateInfo> vecSINFOS;
            while (!gamestate::aiSimActionBuffer.empty())
            {
                gamestate::randomAIChoice = false;
                sv.loadGame();
                sv.saveGame();
                try { apci.turnLoop(action); }
                catch (PlayerDiedError) { if (gamestate::orderedChoiceTree[0].size() == 2 && gamestate::aiSimActionBuffer.size() % 2 != 0) gamestate::aiSimActionBuffer.pop_front(); }
                if (gm.checkEnemyDeath()) gm.handleEnemyDeath();
                vecSINFOS.emplace_back(stateInfo());
                vecSINFOS.back().createAllVars(false);
                if (vecSINFOS.size() > gamestate::orderedChoiceTree.size())
                {
                    vecSINFOS.clear();
                    gamestate::orderedChoiceTree.clear();
                    gamestate::aiSimActionBuffer.clear();
                    break;
                }
            }
            sv.loadGame();
            sv.clearAll();
            vector<float> Q_VALS;
            vector<vector<vector<int>>> allSimmedSINFO;
            vector<vector<bool>> allSimmedSINFOMasks;
            //float maximumQ{ -1000 };
            for (int i{ 0 }; i < vecSINFOS.size(); ++i)
            {
                allSimmedSINFO.push_back({ vecSINFOS[i].getEmbeddingInputsCA(), vecSINFOS[i].getEmbeddingInputsEN(), vecSINFOS[i].getEmbeddingInputsPO(), vecSINFOS[i].getNormalInputs() });
                allSimmedSINFOMasks.push_back(vecSINFOS[i].getMasks());
            }
            if (gamestate::orderedChoiceTree.size() != 0)
            {
                int maximumQIndex{ 0 };
                maximumQIndex = pyFunc.BESTQ_MICRO()(allSimmedSINFO, allSimmedSINFOMasks).cast<int>();
                for (int i{ 0 }; i < gamestate::orderedChoiceTree[maximumQIndex].size(); ++i)
                {
                    gamestate::aiSimActionBuffer.push_back(gamestate::orderedChoiceTree[maximumQIndex][i]);
                }
            }
        }
        forcedAIActions(action, reward);

        try { apci.turnLoop(action); }
        catch (PlayerDiedError) {}

        gamestate::aiSimActionBuffer.clear();
        gamestate::aiCardBuffer.clear();
        gamestate::orderedChoiceTree.clear();

        if (gm.checkEnemyDeath()) gm.handleEnemyDeath();

        if (gm.turnCount > 100) apci.getPlayer().setHealth(0);

        if (apci.getPlayer().getHealth() == 0) { throw PlayerDiedError("died"); }
        else if (gm.checkAllEnemyDeath())
        {
            done = true;
            reward = static_cast<float>(apci.getPlayer().getHealth() / 10);
            if (gm.getMapPathEvent() == 'B') reward += 1;
            SINFO.enemyVars[0] = 0;
            for (int i{ 0 }; i < apci.getRelics().size(); ++i)
            {
                reward += apci.getRelics()[i]->getCounterReward() * rewardMultiplier;
            }
            reward += static_cast<float>(apci.getGold() - oldGold) / 50;
            reward += static_cast<float>(apci.getPlayer().getMaxHealth() - oldMaxHP) / 6;
        }
        else
        {
            SINFO.createAllVars();
            if (apci.cursesInDeck() > oldCurses) reward -= 1.5f;
            reward += static_cast<float>(apci.getGold() - oldGold) / 50;
            reward += static_cast<float>(apci.getPlayer().getMaxHealth() - oldMaxHP) / 6;
            if (apci.getPlayer().getBlock() > oldBlock && gm.getPlayerEnergy() > oldEnergy && !apci.fineToBlockMore(action)) reward -= 0.4f;
            nextState = SINFO.getNormalInputs();
            nextStateCA = SINFO.getEmbeddingInputsCA();
            nextStateEN = SINFO.getEmbeddingInputsEN();
            nextStatePO = SINFO.getEmbeddingInputsPO();
            nextMasks = SINFO.getMasks();
        }
        if (gm.getMapPathEvent() == 'E' || gm.getMapPathEvent() == 'B' || gm.getActNumber() > 1 || random_int(1,5)==5)
        {
            //pyFunc.UPDATE_MICRO()(currentState, currentStateCA, currentStateEN, currentStatePO, action, reward, nextState, nextStateCA, nextStateEN, nextStatePO, done, nextMasks, 1);
        }
        if (done)
        {
            break;
        }
    }
    if (!holdPots.empty())
    {
        for (int i{ 0 }; i < holdPots.size(); ++i)
        {
            apci.getPotions().push_back(holdPots[i].release());
        }
    }
    startHP -= apci.getPlayer().getHealth();
    //pyFunc.UPDATE_PRED()(gm.mostRecentMSI().getPredInputs(), startHP, gm.fightIndexPRED);
}


int main()
{
    py::scoped_interpreter guard{};
    pythonFunctions pyFunc(py::module::import("scripts.DQN"));
    pyFunc.addOther(py::module::import("scripts.NN"));
    pyFunc.addPred(py::module::import("scripts.PRED"));
    gm.makeDatabase();
    gm.makeEvents();
    gm.makeRelicStacks();
    gm.makePotionStacks();
    gm.genRealMap();
    
    apci.initIronclad();

    if (!ai)
    {
        gm.StartGameV();
        return 0;
    }

    const int n_episodes{ 30003 };
    bool predTraining{ false };
    if (fullRuns)
    {
        pyFunc.LOAD_MICRO()();
        pyFunc.LOAD_MACRO()();
        pyFunc.LOAD_PRED()();
        gm.holdPyFunc(pyFunc);
        vector<pair<int,int>> endFloors;
        //gm.ResetRun();
        for (int e{ 0 }; e < n_episodes; ++e)
        {
            try
            {
                int currentFloor = gm.getFloorIndex();
                if (currentFloor > 33)
                {
                    int a = 9 + 10; a;
                }
                gm.setBossIndex();
                while (currentFloor != 50)
                {
                    gm.clearCachedPaths();
                    gamestate::aiCardChoices.clear();
                    gamestate::aiEventActionBuffer.clear();
                    gamestate::randomAIChoice = false;
                    if (currentFloor == 16) { gm.Act1to2(); currentFloor = gm.getFloorIndex(); continue;}
                    else if (currentFloor == 33) { gm.Act2to3(); currentFloor = gm.getFloorIndex(); continue; }
                    gm.advanceRealEvent();
                    currentFloor = gm.getFloorIndex();
                    if (currentFloor > 0)
                    {
                        std::cout << "Floor: " << currentFloor << ", HP: " << apci.getPlayer().getHealth() << "/" << apci.getPlayer().getMaxHealth();
                        std::cout << ", Type: " << gm.getMapPathEvent();
                        std::cout << ", Expected: " << std::setprecision(4) << callMacroSINGLE(gm.mostRecentMSI(), pyFunc) << endl;
                    }
                    try {
                        gm.playNextEvent();
                        if (gm.getMapPathEvent() == 'e' || gm.getMapPathEvent() == 'E' || gm.getMapPathEvent() == 'B')
                        {
                            outerTurnLoopAI(pyFunc);
                            gm.AIFightEnd();
                        }
                    }
                    catch (SmokeBombError) { apci.removeSmokeBomb(); }
                }
                gm.Act3to4();
                gm.startAct4();
                std::cout << "Act 4 Complete! Results: " << endl << endl;
                gm.prelude();
                throw PlayerDiedError(" ");
            }
            catch (PlayerDiedError)
            {
                gm.endFight();
                std::cout << "You lose" << endl;
                gm.prelude();
                std::cout <<"Calls: "<< callCount << endl;
                callCount = 0;
                gm.setRandomMacroThisFloor(false);
                gm.thinOutMSIs();
                vector<vector<int>> MSIs;
                vector<vector<float>> PREDs(callPredBATCH(gm.MSIsForUpdate(), pyFunc));
                for (int i{ 0 }; i < gm.MSIsForUpdate().size(); ++i)
                {
                    MSIs.push_back(gm.MSIsForUpdate()[i].getNormalInputs());
                }
                endFloors.push_back({ e, gm.getFloorIndex() });
                if (e % 250 == 0)
                {
                    ofstream file;
                    file.open("./model/RUN_PERFORMANCE.txt", ios::app);
                    for (int j{ 0 }; j < endFloors.size(); ++j)
                    {
                        file << endFloors[j].first << " " << endFloors[j].second << "\n";
                    }
                    file.close();
                    endFloors.clear();
                }
                pyFunc.UPDATE_MACRO()(MSIs, gm.getFloorIndex(), PREDs);
                //pyFunc.TRAIN_MICRO()(e);
                gm.ResetRun();
            }
        }
    }
    else
    {
        if (predTraining) pyFunc.LOAD_PRED()();
        pyFunc.LOAD_MICRO()();
        pyFunc.LOAD_MACRO()();
        gm.holdPyFunc(pyFunc);
        gm.zeroMSI();
        stateInfo SINFO;
        macroStateInfo PRED_INFO;
        vector<int> endPLHealths;
        map<string, vector<int>> endPLHealthsByFight;
        vector<statistics> stats;
        bool firstPass{ true };
        bool humanFeedback{ false };
        try {
            gm.StartGame();
            for (int e{ 0 }; e < n_episodes; ++e)
            {
                apci.masterDeckMaker(curriculumStage::Stage5,e);
                gamestate::aiEventActionBuffer.clear();
                //statistics currentStats(gm.makeDGEnum(), {apci.deckForStats()}, { apci.relicForStats() }, { apci.potionsForStats() }, apci.getDeck().size(), apci.getPlayer().getHealth());
                gm.setBossIndex();
                gm.playNextEvent();
                if (predTraining) PRED_INFO.createPredVars();
                string fightName = gm.getHighestHealthEnemyName();
                string takeControl = "";
                if ((gm.getActNumber() > 2 || gm.getMapPathEvent() == 'B') && humanFeedback)
                {
                    apci.initVisuals();
                    cin >> takeControl;
                }
                for (int time{ 0 }; time < 5000; ++time)
                {
                    gamestate::aiEventActionBuffer.clear();

                    SINFO.createAllVars();
                    auto currentState = SINFO.getNormalInputs();
                    auto currentStateCA = SINFO.getEmbeddingInputsCA();
                    auto currentStateEN = SINFO.getEmbeddingInputsEN();
                    auto currentStatePO = SINFO.getEmbeddingInputsPO();
                    auto currentMasks = SINFO.getMasks();

                    vector<bool> nextMasks(296, true);
                    int action;

                    if (e % 20 == 19) apci.initVisuals();
                    if (takeControl == "y")
                    {
                        apci.initVisuals();
                        while (true)
                        {
                            vector<string> validInputs = { "end" };
                            bool already = gamestate::denyCardPlay;
                            if (gm.cardCount >= 3 && apci.checkCardInHand("Normality")) { gamestate::denyCardPlay = true; }
                            for (int i{ 0 }; i < apci.getHand().size(); ++i)
                            {
                                if ((*apci.getHand()[i]).getEnergy() <= gm.getPlayerEnergy() && (*apci.getHand()[i]).castable()) validInputs.push_back(to_string((i + 1) % 10));
                            }
                            if (!already) gamestate::denyCardPlay = false;
                            apci.addPotionInputs(validInputs);
                            showAIDecisions(pyFunc);
                            ai = false;
                            string result = request_input(validInputs);
                            ai = true;
                            if (result == "0") result = "10";
                            if (result == "end") { action = 262; break; }
                            else if (result[0] == 'p')
                            {
                                action = apci.getPotion(static_cast<int>(result[1]) - 49)->getPotionIDNum() + 262;
                                break;
                            }
                            else { action = ((*apci.getHand()[stoi(result) - 1]).getCardIDNum() - 1) * 2 + ((*apci.getHand()[stoi(result) - 1]).getUpgraded() ? 1 : 0); break; }
                        }
                    }
                    else
                    {
                        action = pyFunc.ACTION_MICRO()(currentState, currentMasks, currentStateCA, currentStateEN, currentStatePO, firstPass, e).cast<int>();
                    }
                    if (firstPass) firstPass = false;
                    float reward{ 0 };
                    bool done{ false };

                    auto nextState = SINFO.getNormalInputs();
                    auto nextStateCA = SINFO.getEmbeddingInputsCA();
                    auto nextStateEN = SINFO.getEmbeddingInputsEN();
                    auto nextStatePO = SINFO.getEmbeddingInputsPO();
                    //int oldHealth = apci.getPlayer().getHealth();
                    int oldMaxHP = apci.getPlayer().getMaxHealth();
                    int oldBlock = apci.getPlayer().getBlock();
                    int oldEnergy = gm.getPlayerEnergy();
                    int oldGold = apci.getGold();
                    int oldCurses = apci.cursesInDeck();

                    combatChoices actionChoices = apci.needForSim(action);
                    if (actionChoices != combatChoices::None && action != 110 && action != 111)
                    {

                        gamestate::aiSimActionBuffer.clear();
                        gamestate::aiCardBuffer.clear();
                        gamestate::orderedChoiceTree.clear();

                        gm.actionChoicesForSim(actionChoices, action);

                        sv.saveGame();
                        vector<stateInfo> vecSINFOS;
                        while (!gamestate::aiSimActionBuffer.empty())
                        {
                            gamestate::randomAIChoice = false;
                            sv.loadGame();
                            sv.saveGame();
                            try { apci.turnLoop(action); }
                            catch (PlayerDiedError) { if (gamestate::orderedChoiceTree[0].size() == 2 && gamestate::aiSimActionBuffer.size() % 2 != 0) gamestate::aiSimActionBuffer.pop_front(); }
                            if (gm.checkEnemyDeath()) gm.handleEnemyDeath();
                            vecSINFOS.emplace_back(stateInfo());
                            vecSINFOS.back().createAllVars(false);
                            if (vecSINFOS.size() > gamestate::orderedChoiceTree.size())
                            {
                                vecSINFOS.clear();
                                gamestate::orderedChoiceTree.clear();
                                gamestate::aiSimActionBuffer.clear();
                                break;
                            }
                        }
                        sv.loadGame();
                        sv.clearAll();
                        vector<float> Q_VALS;
                        vector<vector<vector<int>>> allSimmedSINFO;
                        vector<vector<bool>> allSimmedSINFOMasks;
                        //float maximumQ{ -1000 };
                        for (int i{ 0 }; i < vecSINFOS.size(); ++i)
                        {
                            allSimmedSINFO.push_back({ vecSINFOS[i].getEmbeddingInputsCA(), vecSINFOS[i].getEmbeddingInputsEN(), vecSINFOS[i].getEmbeddingInputsPO(), vecSINFOS[i].getNormalInputs() });
                            allSimmedSINFOMasks.push_back(vecSINFOS[i].getMasks());
                        }
                        if (gamestate::orderedChoiceTree.size() != 0)
                        {
                            int maximumQIndex{ 0 };
                            maximumQIndex = pyFunc.BESTQ_MICRO()(allSimmedSINFO, allSimmedSINFOMasks).cast<int>();
                            for (int i{ 0 }; i < gamestate::orderedChoiceTree[maximumQIndex].size(); ++i)
                            {
                                gamestate::aiSimActionBuffer.push_back(gamestate::orderedChoiceTree[maximumQIndex][i]);
                            }
                        }
                    }
                    forcedAIActions(action, reward);

                    try { apci.turnLoop(action); }
                    catch (PlayerDiedError) {}

                    gamestate::aiSimActionBuffer.clear();
                    gamestate::aiCardBuffer.clear();
                    gamestate::orderedChoiceTree.clear();

                    if (gm.checkEnemyDeath()) gm.handleEnemyDeath();

                    if (apci.getPlayer().getHealth() == 0) { done = true; }
                    else if (gm.checkAllEnemyDeath())
                    {
                        done = true;
                        reward = static_cast<float>(apci.getPlayer().getHealth() / 10);
                        if (gm.getMapPathEvent() == 'B') reward += 1;
                        SINFO.enemyVars[0] = 0;
                        for (int i{ 0 }; i < apci.getRelics().size(); ++i)
                        {
                            reward += apci.getRelics()[i]->getCounterReward() * rewardMultiplier;
                        }
                        reward += static_cast<float>(apci.getGold() - oldGold) / 50;
                        reward += static_cast<float>(apci.getPlayer().getMaxHealth() - oldMaxHP) / 6;
                    }
                    else
                    {
                        SINFO.createAllVars();
                        if (apci.cursesInDeck() > oldCurses) reward -= 1.5f;
                        reward += static_cast<float>(apci.getGold() - oldGold) / 50;
                        reward += static_cast<float>(apci.getPlayer().getMaxHealth() - oldMaxHP) / 6;
                        if (apci.getPlayer().getBlock() > oldBlock && gm.getPlayerEnergy() < oldEnergy && !apci.fineToBlockMore(action)) reward -= 0.6f;
                        nextState = SINFO.getNormalInputs();
                        nextStateCA = SINFO.getEmbeddingInputsCA();
                        nextStateEN = SINFO.getEmbeddingInputsEN();
                        nextStatePO = SINFO.getEmbeddingInputsPO();
                        nextMasks = SINFO.getMasks();
                    }
                    if (gm.turnCount > 25) { reward = 0; done = true; }
                    pyFunc.UPDATE_MICRO()(currentState, currentStateCA, currentStateEN, currentStatePO, action, reward, nextState, nextStateCA, nextStateEN, nextStatePO, done, nextMasks, e);

                    if (done)
                    {
                        if (predTraining)
                        {
                            int hpLoss = apci.getPlayer().getMaxHealth() - apci.getPlayer().getHealth();
                            if (apci.getPlayer().getHealth() == 0) hpLoss = 100;
                            pyFunc.UPDATE_PRED()(PRED_INFO.getPredInputs(), hpLoss, gm.fightIndexPRED);
                        }
                        int nmeHP{ 0 };
                        if (gm.getAllEnemies().size() != 0)
                        {
                            for (int i{ 0 }; i < gm.getAllEnemies().size(); ++i)
                            {
                                if (gm.getAllEnemies()[i]->getHealth() > nmeHP) nmeHP = gm.getAllEnemies()[i]->getHealth();
                            }
                        }
                        gm.endFight();
                        std::cout << "episode:" << e << "/" << n_episodes << "      playerHealth:" << apci.getPlayer().getHealth() << "      " << fightName << ":" << nmeHP << std::endl;
                        endPLHealths.push_back(apci.getPlayer().getHealth());
                        endPLHealthsByFight[fightName].push_back(apci.getPlayer().getHealth());
                        //currentStats.HP = apci.getPlayer().getHealth();
                        int hprng = random_int(75, 85);
                        apci.getPlayer().setMaxHealth(hprng);
                        apci.getPlayer().setHealth(hprng - random_int(0,20));

                        //stats.push_back(currentStats);
                        /*if (stats.size() % 1000 == 0)
                        {
                            for (int i{ 0 }; i < stats.size(); ++i)
                            {
                                ofstream file;
                                switch (stats[i].dg)
                                {
                                case deckGen::EH1:
                                    file.open("./model/sts/statsEH1.txt", ios::app);
                                    break;
                                case deckGen::HH1:
                                    file.open("./model/sts/statsHH1.txt", ios::app);
                                    break;
                                case deckGen::EL1:
                                    file.open("./model/sts/statsEL1.txt", ios::app);
                                    break;
                                case deckGen::BO1:
                                    file.open("./model/sts/statsBO1.txt", ios::app);
                                    break;
                                case deckGen::EH2:
                                    file.open("./model/sts/statsEH2.txt", ios::app);
                                    break;
                                case deckGen::HH2:
                                    file.open("./model/sts/statsHH2.txt", ios::app);
                                    break;
                                case deckGen::EL2:
                                    file.open("./model/sts/statsEL2.txt", ios::app);
                                    break;
                                case deckGen::BO2:
                                    file.open("./model/sts/statsBO2.txt", ios::app);
                                    break;
                                }
                                for (int j{ 0 }; j < stats[i].deckIDs.size(); ++j)
                                {
                                    file << stats[i].deckIDs[j] << " ";
                                }
                                file << "\n";
                                for (int j{ 0 }; j < stats[i].relicIDs.size(); ++j)
                                {
                                    file << stats[i].relicIDs[j] << " ";
                                }
                                file << "\n";
                                for (int j{ 0 }; j < stats[i].potionIDs.size(); ++j)
                                {
                                    file << stats[i].potionIDs[j] << " ";
                                }
                                file << "\n";
                                file << stats[i].deckSize << " " << stats[i].HP;
                                file << "\n";
                                file.close();
                            }
                            stats.clear();
                        }*/
                        break;
                    }
                }
            }
        }
        catch (py::error_already_set& e) {
            std::cout << e.what() << std::endl;
        }
        ofstream myfile2;
        myfile2.open("./model/sts/FINALHPs.txt");
        for (int i{ 0 }; i < endPLHealths.size(); ++i)
        {
            myfile2 << endPLHealths[i] << " " << i << "\n";
        }
        myfile2.close();
        for (auto itr{ endPLHealthsByFight.begin() }; itr != endPLHealthsByFight.end(); ++itr)
        {
            ofstream myfile3;
            myfile3.open("./model/sts/EndHealths_" + itr->first + ".txt");
            for (int i{ 0 }; i < itr->second.size(); ++i)
            {
                myfile3 << itr->second[i] << " " << i << "\n";
            }
        }
    }
    return 0;
}




