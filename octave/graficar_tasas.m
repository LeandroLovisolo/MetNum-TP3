function graficar_tasas()

rango_k = 1:100;

load('tasas.mat');
plot(rango_k, tasas);

title('Tasa de aciertos en función de $k$');
xlabel('$k$');
ylabel('Aciertos [\%]');

print('tasas-vs-k', '-S800,550', '-dtex');