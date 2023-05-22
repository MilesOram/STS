import random
#from tkinter import CURRENT
#from typing import Concatenate
import numpy as np
import tensorflow as tf
import os
from keras import layers, losses, optimizers, models


state_size = 878 #752,878
action_size = 1 
batch_size = 64
output_dir = './model/sts'
update_after_actions = 20 
update_target_network = 200


class DQNAgent:
    def __init__(self,_state_size,_action_size):
        self.state_size = _state_size
        self.action_size = _action_size
        self.memory = []

        self.epsilon = 0.0
        self.epsilon_decay = 0.999
        self.epsilon_min = 0.0
        self.learning_rate = 0.0001

        self.model = self._build_model()
        self.optimizer = optimizers.Adam(learning_rate=self.learning_rate)
        self.loss_function = losses.MeanSquaredError()
        self.count = 0

    def _build_model(self):
        main_input = layers.Input(shape=(self.state_size,))
        dense1 = layers.Dense(3072, activation='relu')(main_input) # 1536
        dense2 = layers.Dense(2048, activation='relu')(dense1) # 512
        output = layers.Dense(self.action_size, activation='linear')(dense2)
        
        model = models.Model(inputs=main_input, outputs=output)
        model.compile(optimizers.Adam(learning_rate=self.learning_rate),losses.Huber())
        model.summary()

        return model

    def remember(self, states,floor ):
        if(len(self.memory)) > 10000: # 500/15000/25000
           self.memory = self.memory[500:] # 2000
        for i in range(len(states)):
            self.memory.append([states[i],floor])

    def replay(self, batch_size):
        minibatch = random.sample(self.memory, batch_size)

        main_state_sample =   np.array([minibatch[i][0] for i in range(batch_size)])
        expected_floor =      np.array([minibatch[i][1] for i in range(batch_size)])
        
        self.model.fit(main_state_sample, expected_floor, epochs=3, verbose=1)

        if self.epsilon > self.epsilon_min:
            self.epsilon *= self.epsilon_decay
    def load(self, name):
        self.model.load_weights(name)

    def save(self, name):
        self.model.save_weights(name)


agent = DQNAgent(state_size, action_size)

def takeStatesUpdateNetwork(states, floor,preds):
    current_state = np.array([np.append(states[i],preds[i],axis=0) for i in range(len(preds))])
    agent.count+=1
    agent.remember( current_state, floor)
    if len(agent.memory) > batch_size and len(agent.memory) > 5000 and agent.count % 4 == 0:
        print("e: {:.2}".format(agent.epsilon))
        print("Count: {}".format(agent.count))
        for _ in range(10):
            agent.replay(batch_size)
        
    if agent.count % update_target_network == 0:
        agent.model.set_weights(agent.model.get_weights())
        agent.save(output_dir + "MACRO" + '{:04d}'.format(agent.count) + ".hdf5")
def getEpsilon():
    return round(agent.epsilon,4)

def callExpectedforSim(current_state, preds):
    ps = np.array(preds)
    current_state = np.array(current_state)
    current_state = np.append(current_state,ps)
    current_state = np.reshape( current_state, (1, len(current_state)))
        
    if np.random.rand() <= agent.epsilon:
        return agent.model.__call__( current_state ) + (np.random.rand()*5)

    else:
        return agent.model.__call__( current_state )

def getBestExpectedIndex(states,preds):
    return np.argmax(agent.model.__call__(np.concatenate((states,preds),axis=1)))
    

def getBestExpectedValue(states,preds):
    return np.amax(agent.model.__call__(np.concatenate((states,preds),axis=1)))
    

def loadModel():
    agent.load(".\model\stsMACRO.hdf5")
