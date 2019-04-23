## 1

1110 1

0110 0

1001 0

1101 1

1100 0



## 5

1010101010 0000 / 10011 = 10 0011 1110 ... 0110

R = 0110



## 6

a) R = 0101

b) R = 0001

c) R = 0010



## 7

a) The single-bit error will lead to divided result not being 0 at receiver end.

b) In some cases can, since there might an odd number of 1's sequence that can be devided by G. such as 1010 0010 can be devided by 1001, that means if 1th, 5th and 7th bits flip, the error can not be detected.



## 13

suppose $S = \sum_{i=1}^N x_i$, where $x_i$ denotes the bits to transmit of node i.
$$
throughput = \frac{S}{\frac{S}{R} + Nd_{poll}} = \frac{1}{\frac{1}{R} + \frac{Nd_{poll}}{S}} \le \frac{1}{\frac{1}{R} + \frac{d_{poll}}{Q}}
$$
