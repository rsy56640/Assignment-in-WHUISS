import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt('./house.txt', delimiter=',')
x = data[:, 0]
Y = data[:, 1]
one = np.ones(len(x))
X = np.column_stack((x, one))

# w = np.random.rand(2, )
w = np.array([1, -100])
# w = np.linalg.inv(np.transpose(X).dot(X)).dot(np.transpose(X)).dot(Y)

#  result can be computed at once
# w is exaclty "np.linalg.inv(np.transpose(X).dot(X)).dot(np.transpose(X)).dot(Y)"
# since the (X^T * X) can be inverted.
print(w)
alpha = 0.0001
n = len(x)
_lambda = 0
for i in range(1000000):
    # w = w - alpha * (np.transpose(X).dot(X.dot(w) - Y)).dot(np.linalg.inv(np.transpose(X).dot(X)))
    w = w - alpha * X.T.dot(X.dot(w) - Y) / n
print(w)
print(2 * X.T.dot(X.dot(w) - Y))
# print(np.linalg.inv(np.transpose(X).dot(X)).dot(np.transpose(X)).dot(Y))

plt.scatter(x, Y, s=6, c='red')
x = np.linspace(0., 200.)
plt.plot(x, w[0] * x + w[1], 'b')
plt.xlabel('Size')
plt.ylabel('Price')
plt.title('Linear Regression')
plt.savefig('house_linear_regression.png')
plt.show()
