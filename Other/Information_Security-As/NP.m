%file:NP.m
%calculate n * P(x, y)
function [resx, resy] = NP(a, b, p, n, x, y)
resx = inf;
resy = inf;
while(n >= 1)
    if(mod(n, 2) == 1)
        [resx, resy] = Add(a, b, p, resx, resy, x, y);
    end
    n = floor(n/2);
    [x, y] = Add(a, b, p, x, y, x, y);
end
end