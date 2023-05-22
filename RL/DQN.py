#from keras.backend import identity
#from keras.engine import input_layer
#from numpy.random.mtrand import random_integers
import random
from re import I
from threading import active_count
import numpy as np
import tensorflow as tf
import os
#from keras import layers, losses, Model, optimizers, initializers, models, activations
from keras import layers, losses, optimizers, models

state_size = 1325 # 857 1283
action_size = 296 # 263 + 33
batch_size = 64 # 32/64/128/256
output_dir = './model/sts'
update_after_actions = 20 # 5/10/20/30  
update_target_network = 1000 # 200/500

cname = ["Strike","Bash","Defend","Armaments","Anger","Bodyslam","Clash","Clothesline","Flex","Havoc","Headbutt",
"HeavyBlade","Ironwave","PerfectedStrike","PommelStrike","ShrugItOff","SwordBoomerang","Thunderclap","TrueGrit",
"TwinStrike","WarCry","WildStrike","BattleTrance","BloodForBlood","Bloodletting","BurningPact","Carnage","Combust","DarkEmbrace",
"Disarm","Dropkick","DualWield","Entrench","Evolve","FeelNoPain","FireBreathing","FlameBarrier",
"GhostlyArmor","Hemokinesis","InfernalBlade","Inflame","Intimidate","Metallicize","PowerThrough","Pummel","Rage",
"Rampage","RecklessCharge","Rupture","SearingBlow","SecondWind","SeeingRed","Sentinel","SeverSoul",
"Shockwave","SpotWeakness","Uppercut","Whirlwind","Barricade","Berserk","Bludgeon","Brutality","Corruption",
"DemonForm","DoubleTap","Exhume","Feed","FiendFire","Immolate","Impervious","Juggernaut","LimitBreak","Offering",
"Reaper","Injury","Regret","Pain","Doubt","Parasite","Necronomicure","Clumsy","CurseOfTheBell","Decay",
"Normality","Writhe","Shame","Burn","Dazed","Slime","Wound",
"Bite","Jax","Apparition","RitualDagger","BandageUp","Blind","DarkShackles","DeepBreath","Discovery","DramaticEntrance",
"Enlightenment","Finesse","FlashOfSteel","Forethought","GoodInstincts","Impatience","JackOfAllTrades","Madness","MindBlast",
"Panacea","PanicButton","Purity","SwiftStrike","Trip","Apotheosis","Chrysalis","HandOfGreed","Magnetism","MasterOfStrategy",
"Mayhem","Metamorphosis","Panache","SadisticNature","SecretTechnique","SecretWeapon","TheBomb","ThinkingAhead",
"Transmutation","Violence","Cleave","Void","End",
"AncientPot","AttackPot","BlessingOTF","BlockPot","BloodPot","ColourlessPot","CultistPot","DexterityPot","DistilledChaos","DuplicationPot",
"Elixir","EnergyPot","EntropicBrew","EssenceOS","ExplosivePot","FairyIAB","FearPot","FirePot","FlexPot","FruitJuice","GamblersBrew",
"HeartOfIron","LiquidBronze","LiquidMemories","PowerPot","RegenPot","SkillPot","SmokeBomb","SneckoOil","SpeedPot","StrengthPot",
"SwiftPot","WeakPot"]

if not os.path.exists(output_dir):
    os.makedirs(output_dir)

class DQNAgent:
    def __init__(self,_state_size,_action_size):
        self.state_size = _state_size
        self.action_size = _action_size
        self.memory = []

        self.gamma = 0.985 # 0.98
        self.epsilon = 0.7
        #self.epsilon = 0.0
        self.epsilon_decay = 0.9999 #  ---  0.9994 is goodfor 5000, 0.9992 for 3000
        self.epsilon_min = 0.2
        #self.epsilon_min = 0.0
        self.learning_rate = 0.00005 

        self.model = self._build_model()
        self.target_model = self._build_model()
        self.initial_learning_rate = 0.003
        self.lr_schedule = tf.keras.optimizers.schedules.ExponentialDecay(
            self.initial_learning_rate,
            decay_steps=150,
            decay_rate=0.95,
            staircase=True)
        self.optimizer = optimizers.Adam(learning_rate=self.learning_rate)
        self.loss_function = losses.MeanSquaredError()
        self.last_e_trained = 0

    def _build_model(self):
        embed_size = 80 # 50/100
        embed_size2 = 50 # 50/100 
        card_input = layers.Input(shape=(11,))
        card_embed = layers.Embedding(132, embed_size, input_length=11)(card_input)
        card_embed = layers.Flatten()(card_embed)
        enemy_input = layers.Input(shape=(15,))
        enemy_embed = layers.Embedding(195, embed_size, input_length=15)(enemy_input)
        enemy_embed = layers.Flatten()(enemy_embed)
        pot_input = layers.Input(shape=(5,))
        pot_embed = layers.Embedding(34, embed_size2, input_length=5)(pot_input)
        pot_embed = layers.Flatten()(pot_embed)
        main_input = layers.Input(shape=(self.state_size,))
        
        merged = layers.Concatenate(axis=-1)([card_embed, enemy_embed, pot_embed, main_input])
        layer0 = layers.Dropout(0.003)(merged) # 0.2
        dense1 = layers.Dense(3072, input_dim=(self.state_size+26*embed_size + embed_size2*5), activation='relu')(layer0) # 3072
        dense2 = layers.Dense(2048, activation='relu')(dense1) # 2048
        output = layers.Dense(self.action_size, activation='linear')(dense2)

        model = models.Model(inputs=[card_input, enemy_input, pot_input, main_input], outputs=output)
        model.summary()

        return model

    def remember(self, state, action, reward, next_state, done, masks_next):
        if(len(self.memory)) > 30000: # 500/15000/25000
           self.memory = self.memory[1000:] # 2000
        self.memory.append((state,action,reward,next_state,done, masks_next))

    def act(self, card_state, enemy_state, pot_state, current_state, masks,e):
        card_state = np.reshape( card_state, (1, len(card_state)))
        enemy_state = np.reshape( enemy_state, (1, len(enemy_state)))
        pot_state = np.reshape( pot_state, (1, len(pot_state)))

        current_state = np.reshape( current_state, (1, len(current_state)))

        act_values = self.target_model.__call__([card_state, enemy_state, pot_state, current_state])
        if e % 28 == 27:
            textprint = ""
            for i in range(len(masks)):
                if(masks[i]):
                    if i > 262:
                        textprint += cname[i-131]
                    else:
                        textprint += cname[(i // 2)]
                        if i % 2 == 1:
                            textprint += "+"
                    textprint += ":" + str(np.array(act_values[0][i])) + ", " 
            print(textprint)
        altRandom = False
        if altRandom:
            if np.random.rand() <= self.epsilon:
                #arr = [3.65,3.22,1.76,3.61,2.81]
                # custom explore vs exploit, similar to softmax but not linear scaling (pow3) 
                arr=[]
                indices = []
                for i in range(len(masks)):
                    if(masks[i]):
                        arr.append(np.array(act_values[0][i]))
                        indices.append([np.array(act_values[0][i]),i])
                arr.sort(reverse=True)
                for i in range(len(indices)):
                    for j in range(len(arr)):
                        if indices[i][0] == arr[j]:
                            indices[i][0] = j
                            break
                s = arr[0]
                for i in range(len(arr)):
                    arr[i] = s - arr[i] +1
                    arr[i] = 1/(arr[i]**2)
                a=[0]
                arr.extend(a)
                res=[0] * len(arr)
                for i in range(len(arr)-2,-1,-1):
                    for j in range(len(arr)):
                        if j<i:
                            res[j] += ( arr[i] - arr[i+1] ) / (i+1)
                choice = np.random.rand()
                total = 0
                for i in range(len(arr)):
                    total+=arr[i]
                    if total >= choice:
                        for it in indices:
                            if it[0] == arr[i]:
                                return it[1];
            else:
                #temp = []
                #for i in range(len(masks)):
                #    if(masks[i]):
                #        temp.append(i)
                #return temp[random.randrange(len(temp))]
                temp = []
                for i in range(len(masks)):
                    temp.append(-2000 if not masks[i] else act_values[0][i])
                return np.argmax(temp)
        else:
            if np.random.rand() <= self.epsilon:
                temp = []
                for i in range(len(masks)):
                    if(masks[i]):
                        temp.append(i)
                return temp[random.randrange(len(temp))]
            else:
                temp = []
                for i in range(len(masks)):
                    temp.append(-2000 if not masks[i] else act_values[0][i])
                return np.argmax(temp)

    def replay(self, batch_size):
        minibatch = random.sample(self.memory, batch_size)

        card_state_sample =   np.array([minibatch[i][0][0] for i in range(batch_size)])
        enemy_state_sample =  np.array([minibatch[i][0][1] for i in range(batch_size)])
        pot_state_sample =    np.array([minibatch[i][0][2] for i in range(batch_size)])
        main_state_sample =   np.array([minibatch[i][0][3] for i in range(batch_size)])

        card_state_next_sample =   np.array([minibatch[i][3][0] for i in range(batch_size)])
        enemy_state_next_sample =  np.array([minibatch[i][3][1] for i in range(batch_size)])
        pot_state_next_sample =    np.array([minibatch[i][3][2] for i in range(batch_size)])
        main_state_next_sample =   np.array([minibatch[i][3][3] for i in range(batch_size)])

        rewards_sample =    [minibatch[i][2] for i in range(batch_size)]
        action_sample =     [minibatch[i][1] for i in range(batch_size)]
        done_sample =       tf.convert_to_tensor([float(minibatch[i][4]) for i in range(batch_size)])
        masks_next_sample = [minibatch[i][5] for i in range(batch_size)]  
        
        future_rewards = self.target_model.predict([card_state_next_sample,enemy_state_next_sample,pot_state_next_sample,main_state_next_sample])

        for i in range(batch_size):
            for j in range(len(future_rewards[0])):
                if not masks_next_sample[i][j]:
                    future_rewards[i][j] = 0
        updated_q_values = [rewards_sample[i] + self.gamma * np.amax(future_rewards[i]) for i in range(batch_size)]
        for n in range(batch_size):
            if(done_sample[n]):
                updated_q_values[n] = rewards_sample[n]

        masks = tf.one_hot(action_sample, action_size)
        with tf.GradientTape() as tape:
            q_values = self.model([card_state_sample,enemy_state_sample,pot_state_sample,main_state_sample])
            q_action = tf.reduce_sum(tf.multiply(q_values, masks), axis=1)
            loss = self.loss_function(updated_q_values, q_action)
            print(loss)

        grads = tape.gradient(loss, self.model.trainable_variables)
        self.optimizer.apply_gradients(zip(grads, self.model.trainable_variables))

        if self.epsilon > self.epsilon_min:
            self.epsilon *= self.epsilon_decay
        #if self.epsilon < 0.009:
         #   self.epsilon = 0.1
    def load(self, name):
        self.model.load_weights(name)
        self.target_model.load_weights(name)

    def save(self, name):
        self.model.save_weights(name)


agent = DQNAgent(state_size, action_size)

def takeStateGiveAction(current_state, masks, card_state, enemy_state, pot_state, firstPass,e):
    #if firstPass:
    #    agent.load(".\model\stsL.hdf5")
    return agent.act(card_state, enemy_state, pot_state, current_state,masks,e) 

def takeNextStateUpdateNetwork(old_state,card_state, enemy_state, pot_state, action, reward, next_state, next_state_card, next_state_enemy, next_state_pot, done, masks_next, e):
    agent.remember([card_state, enemy_state, pot_state, old_state], action, reward, [next_state_card, next_state_enemy, next_state_pot, next_state], done, masks_next)

    if len(agent.memory) > 1000 and e % update_after_actions == 0 and e != agent.last_e_trained:
        print("e: {:.2}".format(agent.epsilon))
        agent.last_e_trained = e
        for _ in range(10):
            agent.replay(batch_size)

    if e % update_target_network == 0:
        agent.target_model.set_weights(agent.model.get_weights())

    if e % 2000 == 0:
        agent.save(output_dir + "MICRO" + '{:04d}'.format(e) + ".hdf5")
def getEpsilon():
    return round(agent.epsilon,4)

def callQVALSforSim(card_state, enemy_state, pot_state, current_state, masks):
    card_state = np.reshape( card_state, (1, len(card_state)))
    enemy_state = np.reshape( enemy_state, (1, len(enemy_state)))
    pot_state = np.reshape( pot_state, (1, len(pot_state)))
    current_state = np.reshape( current_state, (1, len(current_state)))
        
    act_values = agent.target_model.__call__([card_state, enemy_state, pot_state, current_state])
    temp = []
    for i in range(len(masks)):
        temp.append(-2000 if not masks[i] else act_values[0][i])
    return np.amax(temp)

def callAllQVALS(states, masks):

    card_state = np.array([states[i][0] for i in range(len(states))])
    enemy_state = np.array([states[i][1] for i in range(len(states))])
    pot_state = np.array([states[i][2] for i in range(len(states))])
    current_state = np.array([states[i][3] for i in range(len(states))])
    masks = np.array(masks)
        
    act_values = agent.target_model.__call__([card_state, enemy_state, pot_state, current_state])
    res = []
    for n in range(len(masks)):
        temp = []
        for i in range(len(masks[0])):
            temp.append(-2000 if not masks[n][i] else act_values[n][i])
        res.append(np.amax(temp))
    return res

def getBestQValIndex(states, masks):

    card_state = np.array([states[i][0] for i in range(len(states))])
    enemy_state = np.array([states[i][1] for i in range(len(states))])
    pot_state = np.array([states[i][2] for i in range(len(states))])
    current_state = np.array([states[i][3] for i in range(len(states))])

    act_values = agent.target_model.__call__([card_state, enemy_state, pot_state, current_state])

    res = [[0]*296 for i in range(len(states))]
    for i in range(len(masks)):
        for j in range(len(masks[0])):
            if not masks[i][j]:
                res[i][j] = -2000
            else:
                res[i][j] = act_values[i][j]
    
    return np.argmax(np.average(res[i]) for i in range(len(states)))
    #return np.argmax(np.amax(res[i]) for i in range(len(states)))

def loadModel():
    agent.load(".\model\stsMICRO.hdf5")
    

def trainModel(e):
    if len(agent.memory) > 2000 and e % update_after_actions == 0:
        print("e: {:.2}".format(agent.epsilon))
        for _ in range(50):
            agent.replay(batch_size)
    if e % update_target_network == 0:
        agent.target_model.set_weights(agent.model.get_weights())

    if e % 2000 == 0:
        agent.save(output_dir + "MICRO" + '{:04d}'.format(e) + ".hdf5")

