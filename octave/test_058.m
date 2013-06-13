function X = test_058()

num_images = 1000;
num_tests = 500;
k = 10;

msg('Cargando imágenes de entrenamiento...\n');

images = leerMNISTimage('../train-images-idx3-ubyte');
labels = leerMNISTlabel('../train-labels-idx1-ubyte');

msg('Filtrando imágenes por dígito...\n');

img0 = [];
img5 = [];
img8 = [];

i = j = 1;
while(j <= num_images)
    if(labels(i) == 0); img0(end + 1, :) = images(:,:,i)'(:)'; j++; endif
    if(labels(i) == 5); img5(end + 1, :) = images(:,:,i)'(:)'; j++; endif
    if(labels(i) == 8); img8(end + 1, :) = images(:,:,i)'(:)'; j++; endif
    i++;
end

msg(cstrcat(num2str(rows(img0)), ' imágenes dígito 0.\n'));
msg(cstrcat(num2str(rows(img5)), ' imágenes dígito 5.\n'));
msg(cstrcat(num2str(rows(img8)), ' imágenes dígito 8.\n'));

msg('Armando matriz X...\n');

X = [img0; img5; img8];

mu = mean(X);
for i = 1:rows(X)
    X(i,:) = X(i,:) - mu;
end

X = X / sqrt(columns(X) - 1);

msg('Hallando descomposición SVD...\n');

[U S V] = svd(X);

msg('Transformando imágenes...\n');

T_img0 = tc(V, img0, k);
T_img5 = tc(V, img5, k);
T_img8 = tc(V, img8, k);

% msg('Graficando transformadas...\n');

% figure;
% hold on;
% scat(T_img0, 'red');
% scat(T_img5, 'green');
% scat(T_img8, 'blue');
% hold off;

% msg('Graficando medias...\n');

mu_img0 = mean(T_img0);
mu_img5 = mean(T_img5);
mu_img8 = mean(T_img8);

mu_img0
mu_img5
mu_img8

% medias = [mu_img0; mu_img5; mu_img8];

% figure;
% scatter3(medias(:,1), medias(:,2), medias(:,3));

msg('Cargando imágenes de prueba...\n');

images = leerMNISTimage('../t10k-images-idx3-ubyte');
labels = leerMNISTlabel('../t10k-labels-idx1-ubyte');

msg('Filtrando imágenes de prueba por dígito...\n');

test0 = [];
test5 = [];
test8 = [];

i = j = 1;
while(j <= num_tests)
    if(labels(i) == 0); test0(end + 1, :) = images(:,:,i)'(:)'; j++; endif
    if(labels(i) == 5); test5(end + 1, :) = images(:,:,i)'(:)'; j++; endif
    if(labels(i) == 8); test8(end + 1, :) = images(:,:,i)'(:)'; j++; endif
    i++;
end

msg('Generando matriz de distancias respecto de las medias...\n');

T_test0 = tc(V, test0, k);
T_test5 = tc(V, test5, k);
T_test8 = tc(V, test8, k);

D0 = zeros(3, rows(test0));
D5 = zeros(3, rows(test5));
D8 = zeros(3, rows(test8));

aciertos0 = 0;
aciertos5 = 0;
aciertos8 = 0;

msg('\nImágenes de prueba dígito 0\n\n');

for i=1:rows(test0)
    D0(:,i) = [norm(T_test0(i,:) - mu_img0);
               norm(T_test0(i,:) - mu_img5);
               norm(T_test0(i,:) - mu_img8)];
    [x, idx] = min(D0(:,i));
    msg(cstrcat('Dígito reconocido: ', idx2dig(idx), '\n'));
    if(idx == 1);
        msg('Acierto!\n')
        aciertos0++;
    endif
end

msg('\nImágenes de prueba dígito 5\n\n');

for i=1:rows(test5)
    D5(:,i) = [norm(T_test5(i,:) - mu_img0);
               norm(T_test5(i,:) - mu_img5);
               norm(T_test5(i,:) - mu_img8)];
    [x, idx] = min(D5(:,i));
    msg(cstrcat('Dígito reconocido: ', idx2dig(idx), '\n'));
    if(idx == 2);
        msg('Acierto!\n')
        aciertos5++;
    endif
end

msg('\nImágenes de prueba dígito 8\n\n');

for i=1:rows(test8)
    D8(:,i) = [norm(T_test8(i,:) - mu_img0);
               norm(T_test8(i,:) - mu_img5);
               norm(T_test8(i,:) - mu_img8)];
    [x, idx] = min(D8(:,i));
    msg(cstrcat('Dígito reconocido: ', idx2dig(idx), '\n'));
    if(idx == 3);
        msg('Acierto!\n')
        aciertos8++;
    endif
end

msg('Dígito 0:\n');
%D0
printf('Tasa de aciertos: %f\n', aciertos0 * 100 / rows(test0));

msg('Dígito 5:\n');
%D5
printf('Tasa de aciertos: %f\n', aciertos5 * 100 / rows(test5));

msg('Dígito 8:\n');
%D8
printf('Tasa de aciertos: %f\n', aciertos8 * 100 / rows(test8));

tests_total = rows(test0) + rows(test5) + rows(test8);
aciertos_total = aciertos0 + aciertos5 + aciertos8;

printf('Tests total: %f\n', tests_total);
printf('Aciertos total: %f\n', aciertos_total);
printf('Tasa de aciertos total: %f\n', aciertos_total * 100 / tests_total);

function T = tc(V, img, k)

T = V'(1:k,:) * double(img');
T = T';


function scat(imgs, color)

scatter3(imgs(:,1), imgs(:,2), imgs(:,3), color);


function msg(m)

printf(m);
fflush(stdout);


function d = idx2dig(idx)

if(idx == 1)
  d = '0';
endif
if(idx == 2)
  d = '5';
endif
if(idx == 3)
  d = '9';
endif