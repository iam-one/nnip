import numpy as np
import matplotlib.pyplot as plt
import gymnasium as gym

def sigmoid(x):
    return 1.0 / (1.0 + np.exp(-x))

env = gym.make('CartPole-v1', render_mode='human')
desired_state = np.array([0, 0, 0, 0])
desired_mask = np.array([0, 0, 1, 0])

P, I, D = 0.01, 0.01, 0.5

state = env.reset()[0]
integral = 0
derivative = 0
prev_error = 0

s = []
v = []
a = []
w = []

for t in range(200):
    env.render()
    error = state - desired_state

    integral += error
    derivative = error - prev_error
    prev_error = error

    pid = np.dot(P * error + I * integral + D * derivative, desired_mask)
    action = sigmoid(pid)
    action = np.round(action).astype(np.int32)

    state, reward, done = env.step(action)[:3]

    s.append(state[0])
    v.append(state[1])
    a.append(state[2])
    w.append(state[3])

    if done:
        print("Episode finished after {} timesteps".format(t+1))
        break

env.close()

plt.subplot(2, 2, 1)
plt.plot(s)
plt.subplot(2, 2, 2)
plt.plot(v)
plt.subplot(2, 2, 3)
plt.plot(a)
plt.subplot(2, 2, 4)
plt.plot(w)

plt.show()