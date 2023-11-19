import gymnasium as gym
import random

RENDER_MODE = 'human'

env = gym.make('CartPole-v1', render_mode=RENDER_MODE)

env.reset()

for i in range(10):
    observation = [0, 0, 0, 0] #env.reset()[0]

    for t in range(200):
        env.render()

        if observation[2] > 0:
            action = 1
        elif observation[2] < 0:
            action = 0
        else:
            action = random.randint(0, 1)

        state, reward, done = env.step(action)[:3]

        if done:
            print(observation, end=", ")
            print(reward, end=", ")
            print(done)
            break

env.close()