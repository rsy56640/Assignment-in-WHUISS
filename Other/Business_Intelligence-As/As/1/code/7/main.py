import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt('./house.txt', delimiter=',')
x = data[:, 0]
Y = data[:, 1]
one = np.ones(len(x))
X = np.column_stack((x, one))
w = np.random.rand(2, )
#  result can be computed at once
# w is exaclty "np.linalg.inv(np.transpose(X).dot(X)).dot(np.transpose(X)).dot(Y)"
# since the (X^T * X) can be inverted.
print(w)
alpha = 0.7
for i in range(100):
    w = w - alpha * (np.transpose(X).dot(X.dot(w) - Y)).dot(np.linalg.inv(np.transpose(X).dot(X)))
print(w)

plt.scatter(x, Y, s=6, c='red')
x = np.linspace(0., 200.)
plt.plot(x, w[0] * x + w[1], 'b')
plt.xlabel('Size')
plt.ylabel('Price')
plt.title('Linear Regression')
plt.savefig('house_linear_regression.png')
plt.show()
