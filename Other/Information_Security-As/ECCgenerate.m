%file:ECCgenerate.m
function [x, y] = ECCgenerate(a,b,p)
y=0;
x=0;
xx=1;
while xx > -1
xx=randi(p-1)+1;
mm = mod(xx^3+a*xx+b, p);

for yy = 0:1:p
    yy = p - yy;
    if mod(yy^2,p) == mm
        y = yy;
        x = xx;
        return;
    end
end

end
end