% ESTE ALGORITMO SE ENCARGA DE GRAFICAR EL TRAYECTO DE LA DESVIACION ESTANDAR
% CONFORME SE VAN ACUMULANDO DATOS


clear all

fileID = fopen('pruebillaExpansorFlag1.txt','r');
formatSpec = '%f,%f,%f,%f,%f\n';
size = [5, Inf];

A = fscanf(fileID,formatSpec,size);
size2 = length(A);
A = abs(A);



%% Calculo de varianza
var = zeros(size2,1);
K = A(2,1);     % un dato cualquiera de la muestra. En este caso es el valor inicial
n = 0;
sum = 0;
sumSqr = 0;

for i = 1:size2
    Xi = A(2,i);
    n = n+1;
    sum = sum + (Xi - K);
    sumSqr = sumSqr + (Xi - K)*(Xi - K);
    var(i,1) = (sumSqr - (sum*sum)/n)/(n-1);
end


%% Conversion a STD
std = var.^(1/2);


%% Graficas
figure(1)
hold off

plot(A(1,:)/1000,A(2,:))
hold on
plot(A(1,:)/1000,1000*A(5,:))

figure(2)
hold off
plot(A(1,:)/1000,std(:,1))
hold on
plot(A(1,:)/1000,50*A(5,:))
