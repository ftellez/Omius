for a=1:7
mat=load(strcat('pruebillaExpansorFlag ',int2str(a),'.txt'));

prom=mean(mat(:,2));

figure()
plot(mat(:,2))
title(strcat('Prueba de Expansion ',int2str(a)))
ylabel('Corriente')
xlabel('Tiempo')
hold on
plot([0 size(mat(:,2),1)],[prom prom])
hold off

figure(1)
plot(mat(:,2))
title(strcat('Prueba de Expansion ',int2str(a)))
ylabel('Corriente')
xlabel('Tiempo')
hold on

creado=[];
cambio=[];

 for b=1:size(mat(:,2),1)-1
     creado(a,b)=mat(b,2);
     figure(2)
     plot(creado(a,:),'b')
     hold on
     pause(0.1)
     cambio(1,b)=mat(b+1,2)-mat(b,2);
     promCambio=mean(cambio);
     if mat(b+1,2)-mat(b,2)>promCambio*10
         break
     end
 end

end