%file:Add.m
%calculate P(x1, y1) + Q(x2, y2) in E_p(a, b)
function [resx, resy] = Add(a, b, p, x1, y1, x2, y2)
% Inf
if x1==inf
    resx = x2;
    resy = y2;
    return;
end
if x2 == inf
    resx = x1;
    resy = y1;
    return;
end
% Tangency
if x1==x2 && y1==y2
    k = modfrac(3 * x1^2 + a, 2*y1, p);
    resx = mod(k^2 - x1 - x2, p);
    resy = mod(k * (x1-resx) - y1, p);
    return;
end
% P + Q = 0
if x1==x2 && y1~=y2
    resx = inf;
    resy = inf;
    return;
end
% Normal
if x1 ~= x2
    k = modfrac(y2-y1, x2-x1, p);
    resx = mod(k^2 - x1 - x2,p);
    resy = mod(k * (x1-resx) - y1, p);
    return;
end
end