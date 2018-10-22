import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

w1 = np.array([[0, 0, 0, 1],
               [1, 0, 0, 1],
               [1, 0, 1, 1],
               [1, 1, 0, 1]])
w2 = np.array([[0, 0, 1, 1],
               [0, 1, 1, 1],
               [0, 1, 0, 1],
               [1, 1, 1, 1]])
w2 = w2 * -1
W = np.array([-1, -2, -2, 0])
c = 0.1

for times in range(100):
    for i in range(4):
        if W.T.dot(w1[i]) <= 0:
            W = W + c * w1[i]
        if W.T.dot(w2[i]) <= 0:
            W = W + c * w2[i]

print(W)
for i in range(4):
    print(W.T.dot(w1[i]))
    print(W.T.dot(w2[i]))

fig = plt.figure()
ax = fig.add_subplot(1, 1, 1, projection='3d')
for i in range(4):
    ax.scatter(w1[i, 0], w1[i, 1], w1[i, 2], edgecolors='b')
    ax.scatter(-w2[i, 0], -w2[i, 1], -w2[i, 2], edgecolors='r')  # anti-normalization
x = np.linspace(-1, 2, 100)
y = np.linspace(-1, 2, 100)
X, Y = np.meshgrid(x, y)
Z = (-W[3] - W[0] * X - W[1] * Y) / W[2]
ax.plot_surface(X, Y, Z)
plt.show()
