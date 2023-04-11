clc;clear all;
dt=1/16384;  % 8192 16384
t=0:dt:1.0;
for i=1:length(t)
   y(i) = init(t(i), dt, 6);
end
plot(t,y);
writematrix(y',"../input.dat")
%load sor06.dat
%t1=0:1.1/8192:1.1;
%dt=1.1/16384;
%t = 0:dt:1.1;
%y=spline(t1,sor06,t);
%plot(t,y)
%writematrix(y',"input.dat")
% quit;
