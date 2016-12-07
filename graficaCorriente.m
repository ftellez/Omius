%close all
clear all

fileID = fopen('test16_061216.txt','r');
formatSpec = '%f,%f,%f,%f,%f\n';
size = [5, Inf];

A = fscanf(fileID,formatSpec,size);

figure(1)
hold on
%subplot(2,1,1)
plot(A(1,:)/1000,A(2,:))
%plot(A(1,:)/1000,1000*A(5,:))
%plot(A(1,:),(A(2,:)*10/255))
%subplot(2,1,2)
plot(A(1,:)/1000,A(5,:))
figure(2)
hold on
plot(A(1,:)/1000,A(3,:))
plot(A(1,:)/1000,10*A(5,:))