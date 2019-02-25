### 8.

a. users = link rate / transmission rate = 3Mb / 150kb = 20

b. 0.1

c. $\binom{120}{n} \cdot 0.1^n \cdot 0.9^{120-n}​$

d. $1 - \sum_{k=0}^{20} \binom{120}{k} \cdot 0.1^k \cdot 0.9^{120-k}​$

The transmission probability of users obeys the rule of Bernoulli-Distribution. Let $X_i$ be $i.i.d$ random variables that $P(X_i = 1) = p$ denotes that the probability of user $X_i$ is transmitting. Therefore we have
$$
lim_{n\to \infty} P(\frac{n_A - np}{\sqrt{np(1-p)}} \le x) = \Phi(x)
$$
according to [De Moivre–Laplace theorem](https://en.wikipedia.org/wiki/De_Moivre%E2%80%93Laplace_theorem).
$$
\begin{aligned}
P(users\ \ge 21) 
& = 1 - P(\sum_{k=0}^{120} X_k \le 20) \\
& = 1 - P(\frac{\sum_{k=0}^{120} X_k - np}{\sqrt{np(1-p)}} \le \frac{20 - np}{\sqrt{np(1-p)}}) \\
& \approx 1 - \Phi(2.43) \\
& \approx 0.007
\end{aligned}
$$


### 10.

Total delay = $2d_{proc} + \sum (\frac{d_i}{s_i} + \frac{L}{R_i}) = 48.25ms$



### 11.

Total delay = $\frac{L}{R} + \sum \frac{d_i}{s_i}$