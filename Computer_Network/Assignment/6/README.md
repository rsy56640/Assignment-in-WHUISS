上次作业做成第六次了，这次是第五次。



## 1

yxu, yxvu, yxwvu

ywxu, ywxvu, ywvu, ywvxu

yzwxu, yzwxvu, yzwvu, yzwvxu



## 8

|      | x    | y    | z    |
| ---- | ---- | ---- | ---- |
| x    | 0    | 3    | 4    |
| y    |      |      |      |
| z    |      |      |      |

|      | x    | y    | z    |
| ---- | ---- | ---- | ---- |
| x    | 0    | 3    | 4    |
| y    | 3    | 0    | 6    |
| z    | 4    | 6    | 0    |

&nbsp;   

|      | x    | y    | z    |
| ---- | ---- | ---- | ---- |
| x    |      |      |      |
| y    | 3    | 0    | 6    |
| z    |      |      |      |

|      | x    | y    | z    |
| ---- | ---- | ---- | ---- |
| x    | 0    | 3    | 4    |
| y    | 3    | 0    | 6    |
| z    | 4    | 6    | 0    |

&nbsp;   

|      | x    | y    | z    |
| ---- | ---- | ---- | ---- |
| x    |      |      |      |
| y    |      |      |      |
| z    | 4    | 6    | 0    |

|      | x    | y    | z    |
| ---- | ---- | ---- | ---- |
| x    | 0    | 3    | 4    |
| y    | 3    | 0    | 6    |
| z    | 4    | 6    | 0    |



## 9

No, because it does not create a loop.

Connecting two nodes is just to decrease the direct link weight from infinity to some value.



## 10

The Bellman-Ford algorithm will end up at N-1 round.



## 11

a)

| w    | inform y Dw(x) = inf |
| ---- | -------------------- |
| w    | inform z Dw(x) = 5   |
| y    | inform w Dy(x) = 4   |
| y    | inform z Dy(x) = 4   |
| z    | inform w Dz(x) = inf |
| z    | inform y Dz(x) = 6   |

b)

w->x = (5, y),        , (10, y),        , ..., (50, y),        ,        , (51, z)

y->x = (4, x), (9, z),                  , ...,        ,        , (53, z),        , (52, z)

z->x = (6, w),        ,        , (11, z), ...,        , (50, x),

30 steps in total



c)

cut y-z link