clear all; clc;
fileNameList = dir("Zhat*");
for i=1:length(fileNameList)
    B = readmatrix(fileNameList(i).name);
    omega = B(:,1);
    real_ = B(:,2);
    imag_ = B(:,3);
    modl_ = sqrt(real_.^2+imag_.^2);
    f=figure();
    plot(omega, modl_)
    grid on
    xlabel("omega")
    ylabel("|Z|(gs^{-1}cm^{-4})")
    newfigname = join([fileNameList(i).name, ".tif"],"");
    saveas(f,newfigname) 
end

% cd("tmp/")
% tagname = ["gamma","lrr","r_min","xi","Zt_bot"];
% ylimits = [[1.2e8 4e8];[1e8 4e8];[1e8 6e8];[1e8 4.5e8];[1.4e8 3e8]];
% % tagname = ["r_min"];
% for i =1:length(tagname)
%    plotR(tagname(i), ylimits(i,:));
% end
% cd("..")
% 
% 
% function []=plotR(tagName, ylimit)
% dirName = join(["fig", tagName],"");
% if ~exist(dirName,"dir")
%     mkdir(dirName);
% end
% tagFile = join([tagName, "*.dat"],"");
% dirlist = dir(fullfile(pwd, tagFile));
% N = length(dirlist);
% x = zeros(1, N);
% Z0 = zeros(1, N);
% for i=1:N
%     filename = dirlist(i).name;
%     getNumName = erase(filename, [tagName, "=", ".dat"]);
%     x(i) = str2double(getNumName);
%     Zt=readmatrix(filename);
%     Z0(i) = Zt(501,4);
%     plot(Zt(:,1),Zt(:,4),'LineWidth',1)
%     title(erase(filename, ".dat"))
%     xlabel("\omega(s^{-1})")
%     ylabel("|Z|(kgs^{-1}m^{-4})")
%     xlim([-1000 1000])
%     ylim(ylimit)
%     grid on
%     figName = join([dirName, "/", erase(filename, ".dat"), ".tif"],"");
%     saveas(gcf, figName)
% end
% f=figure();
% plot(x, Z0)
% grid on
% xlabel(tagName)
% ylabel("|Z(0,0)|(kgs^{-1}m^{-4})")
% newfigname = join([dirName, "/", tagName, ".tif"],"");
% saveas(f,newfigname) 
% end