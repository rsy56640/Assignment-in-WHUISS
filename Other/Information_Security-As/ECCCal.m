%file:ECCCal.m
%a,bΪ��Բ������pΪ������xΪ������ֵ 
function y = ECCCal( a,b,p,x )
yy=1;
mm = mod(x^3+a*x+b,p);
%index = 1;
while yy > 0
    y = randi(p-1)+1;
    if mod(y^2,p) == mm
        %y(index)=yy;
        return;
        %index=index+1;
    end
end
end