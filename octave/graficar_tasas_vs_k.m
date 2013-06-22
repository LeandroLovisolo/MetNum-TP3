function graficar_tasas_vs_k()

rango_k = 1:100;

load('tasas_vs_k.mat');
plot(rango_k, tasas_vs_k);

title('Tasa de aciertos en función de $k$');
xlabel('$k$');
ylabel('Aciertos [\%]');

print('tasas-vs-k', '-S800,550', '-dtex');