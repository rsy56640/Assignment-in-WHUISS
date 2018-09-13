%file:modfrac.m
% n numerator  d denominator   m module
function y = modfrac(n, d, m)
n = mod(n, m);
d = mod(d, m);
i = 1;
while mod(d*i, m) ~= 1
    i = i+1;
end
y = mod(n*i, m);
end