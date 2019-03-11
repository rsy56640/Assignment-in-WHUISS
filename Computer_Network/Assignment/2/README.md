### 3.

transport layer: UDP, TCP

application layer: DNS, HTTP



### 7.

receive IP from DNS takes $\sum_{i=1}^n RTT_i$

setup TCP connection and object transmission takes 2$RTT_0$

so in total: $2*RTT_0 + \sum_{i=1}^n RTT_i$



### 23.

a) server sends to each clients with the exactly same rate $\frac{u_s}{N}$, in total, the whole amount is $NF$, the total speed is $u_s$, thus the time turns out to be $NF/u_s$.

b) server sends to each clients with the exactly same rate $d_{min}$, with  respect to each client, the time is $F/d_{min}$.

c) with respect to the server, the total transmission size is $NF$, and the maximum transmission rate is $min\{ u_s, N d_{min} \}$, thus the minimum transmission time is given by $max\{ NF/u_s, F/d_{min} \}$.



### 25.

$N$ nodes and $\binom{N}{2}$ edges.



### 31.

a) the client cannot setup a TCP connection.

b) it works fine.

c) the client will send a connection request to the inappropriate port that the server does not listen on, thus no TCP connection will be set up.