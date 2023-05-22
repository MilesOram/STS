import random
import numpy as np
import tensorflow as tf
import os
from keras import layers, losses, optimizers, models

state_size = 602 #660
action_size = 126 # (4,10,3,3,1) (5,8,3,3,3) (3,8,3,3,1) (1,1) x2
batch_size = 64
output_dir = './model/sts'
update_after_actions = 20 
update_target_network = 2000


class DQNAgent:
    def __init__(self,_state_size,_action_size):
        self.state_size = _state_size
        self.action_size = _action_size
        self.memory = []

        self.learning_rate = 0.0001

        self.model = self._build_model()
        self.count = 0

    def _build_model(self):
        main_input = layers.Input(shape=(self.state_size,))
        dense1 = layers.Dense(1024, activation='relu')(main_input) # 2048
        dense2 = layers.Dense(512, activation='relu')(dense1) # 2048
        output = layers.Dense(self.action_size, activation='linear')(dense2)
        
        model = models.Model(inputs=main_input, outputs=output)
        model.compile(optimizers.Adam(learning_rate=self.learning_rate),losses.MeanSquaredError())
        model.summary()

        return model

    def remember(self, state, loss, fight ):
        if(len(self.memory)) > 20000: # 500/15000/25000
           self.memory = self.memory[500:] # 2000
        self.memory.append([state, loss, fight])

    def replay(self, batch_size):
        minibatch = random.sample(self.memory, batch_size)

        main_state_sample =   np.array([minibatch[i][0] for i in range(batch_size)])
        expected_loss =      np.array([minibatch[i][1] for i in range(batch_size)])
        loss_index =      np.array([minibatch[i][2] for i in range(batch_size)])
        
        allpred = np.array(self.model.__call__(main_state_sample))
        #print(allpred[0])
        for i in range(batch_size):
            allpred[i][63 + loss_index[i]] = abs(expected_loss[i] - allpred[i][loss_index[i]])
            allpred[i][loss_index[i]] = expected_loss[i]
            for j in range(len(allpred[i])):
                if allpred[i][j] > 110:
                    allpred[i][j] = 110
                elif allpred[i][j] < 0:
                    allpred[i][j] = 0
        self.model.fit(main_state_sample, allpred, epochs=5, verbose=1)

    def load(self, name):
        self.model.load_weights(name)

    def save(self, name):
        self.model.save_weights(name)


agent = DQNAgent(state_size, action_size)

def takeStatesUpdateNetwork( state, loss, fight ):
    agent.count+=1
    agent.remember( state, loss, fight )
    if len(agent.memory) > batch_size and len(agent.memory) > 2000 and agent.count % 100 == 0:
        for _ in range(50):
            agent.replay(batch_size)
        
    if agent.count % update_target_network == 0:
        agent.save(output_dir + "PRED" + '{:04d}'.format(agent.count) + ".hdf5")

def callAllPredS(current_state):
    current_state = np.reshape( current_state, (1, len(current_state)))

    return agent.model.__call__( current_state )[0]
def callAllPredB(current_state):
    current_state = np.array( current_state )

    return agent.model.__call__( current_state )

def loadModel():
    agent.load(".\model\stsPRED.hdf5")
    L=0
    