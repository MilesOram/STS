# STS
As close to a 1:1 text-based replica of Slay the Spire as possible. Used for Deep Reinforcement Learning of the game.

The game is built entirely in C++ and currently includes everything for a complete, faithful Ironclad run. All cards, enemies, events, potions and relics are present (excluding things like prismatic shard since only the Ironclad and colourless card pools are available). Ascension is not yet implemented so it functions on the base difficulty. The map generation was also recreated. The project evolved over time into a Machine Learning project with the goal to beat the game autonomously, without relying on chance or happenstance.

# Deep Reinforcement Learning
The replica was used as the foundation for a Deep Reinforcement Learning project. Using Python, several TensorFlow models were designed and trained to play the game. This was integrated using the pybind11 library. A mixture of traditional Neural Networks (NNs) and a Deep Q-Learning Network (DQN) are used to handle/learn the micro (in-fight) and macro (out-of-fight) decisions. 

With a few, mostly negligible, compromises, each fight state can be treated as a Markov state, and using the Bellman equation, expected rewards can be learnt and assigned to every possible action available. Actions with the highest expected future reward are chosen, and ideally the best card is played/potion is used or the turn is ended.

A similar DQN would be much less straightforward to implement for the macro decisions. Hierarchical Reinforcement Learning might still have some merit here, but the numerous unique scenarios (each random event) and linked decision making required (map pathing + something), makes me doubt its feasibility.

Instead, two NNs were used. The first returns the expected health loss for every fight in the game, given the current deck, potions and relics. It returns the expected health loss and a measure of variance for each fight and is trained on the results of the micro DQN. This feeds into the main macro NN model which approximates the expected end floor for a given macro state (deck, potions, relics, relic counters, gamestate values, discrete map representation, etc.). At any stage in the run, potential options can be simulated and the option with the highest expected end floor is chosen. RNG outcomes use the average of an appropriately sized sample.

# Comments and Discussion
This is understandably contentious to anyone familiar with the game, encoding so much information into a single value like this will never allow the model to fully converge, especially for a game with so much randomness. This design is far from perfect but I've yet to come up with anything better. I considered having the model return other statistical values: measures of variance, effective upper/lower bounds or quartiles, similar to the NN used for fight health prediction. Without a fixed sample size, the statistical rigorousness of any of the values is dubious and they could only act as representations. Its earlier use is justified as "is this a high or low variance outcome". It seems the only way to handle any additional values outputted by the model would be to feed them into another model, which would compare values for each simulated outcome and select the best. I cannot justify that many levels of abstraction for decision making. I've tried to make as many minor enhancements to the current design as possible.

I've thoroughly considered nearly every aspect of this project and have probably already taken into account thoughts others might have. Even if something appears unfavorable, it still outweighs any other alternatives considered. Anything obvious is likely on a to-do list, for example certain random events have hard-coded responses which ought to be handled by the model.

# Results
The micro model was trained using a mixture of randomly generated and pre-made decks across all fights in the game. The macro model was trained on the results of full runs of the game, 50 floors + 4 floors in act 4.
Currently the highest rolling average end floor, during training, over a sample of 100 runs is: 24.8. Similarly the highest win rate is: 4%. It is worth noting that these two measures are at odds with one another. Training to optimize win rate is a certain way to ingrain sub-optimal tendencies, at the same time the goal is to win. The game has been beaten over 100 times during the 10,000-20,000 training runs. Beating the game with any remote consistency is an achievement and makes the project a success with a promising future.

### See the WriteUp folder for more info and figures.

### For a playable version with the python stripped away, see this [repo](https://github.com/MilesOram/STS-Playable).
