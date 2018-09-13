%file:ECCPlot.m
%a,b�ֱ�Ϊ��Բ�Ĳ�����pΪһ������
function [x,y] =  ECCPlot(a, b, p)
x = [];
y = [];
for xr = 0:1:p
   y1 = ECCCal(a, b, p, xr);
   if(size(y1, 2) > 0)
       x1 = zeros(1, size(y1, 2));
       x1(1:size(y1, 2)) = xr;
       x = [x,x1];
       y = [y,y1];
   end
end
plot(x, y, '*')
hold on
grid on
end
