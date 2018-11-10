import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import norm

xi = [3.2, 3.6, 3, 6, 2.5, 1.1]
h = np.arange(1, 4, 0.5)


def p6(x: float, hN: float) -> float:
    return sum(map(norm.pdf, map(lambda _xi: (x - _xi) / hN, xi))) / len(xi) / hN ** 1


x = np.arange(0, 6, 0.1)

for hN in h:
    p6_hN = lambda x: p6(x, hN)
    y = list(map(p6_hN, x))
    plt.plot(x, y)

plt.legend(list(map(lambda hN: "hN = " + str(hN), h)), loc='upper left')
plt.xlabel("x")
plt.ylabel("probability")

plt.show()
