function graficar_tasas_vs_cotas()

rango_iteraciones = 1:50;

load('tasas_vs_iteraciones.mat');
plot(rango_iteraciones, tasas_vs_iteraciones);

title('Tasa de aciertos en función de número de iteraciones $i$');
xlabel('$i$');
ylabel('Aciertos [\%]');

print('tasas-vs-iteraciones', '-S800,550', '-dtex');