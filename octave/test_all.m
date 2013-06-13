function test_all()

num_images = 1000;
num_tests = 500;
k = 10;

msg('Cargando imágenes de entrenamiento...\n');

images = leerMNISTimage('../train-images-idx3-ubyte');
labels = leerMNISTlabel('../train-labels-idx1-ubyte');

msg('Filtrando imágenes de entrenamiento por dígito...\n');

imgs = {[], [], [], [], [], [], [], [], [], []};

% for i = 1:num_images
%     imgs{labels(i) + 1}(end + 1, :) = images(:, :, i)'(:)';
% end

i = j = 1;
while(j <= num_images)
    if(labels(i) == 0); imgs{1}(end + 1, :) = images(:,:,i)'(:)'; j++; endif
    if(labels(i) == 5); imgs{6}(end + 1, :) = images(:,:,i)'(:)'; j++; endif
    if(labels(i) == 8); imgs{9}(end + 1, :) = images(:,:,i)'(:)'; j++; endif
    i++;
end

msg(cstrcat(num2str(rows(imgs{1})), ' imágenes dígito 0.\n'));
msg(cstrcat(num2str(rows(imgs{6})), ' imágenes dígito 5.\n'));
msg(cstrcat(num2str(rows(imgs{9})), ' imágenes dígito 8.\n'));

msg('Armando matriz X...\n');

% X = [];
% for(i = 1:10)
%     X = [X; imgs{i}];
% end
X = [imgs{1}; imgs{6}; imgs{9}];
X = double(X);

mu = mean(X);
for i = 1:rows(X)
    X(i,:) = X(i,:) - mu;
end

X = X / sqrt(columns(X) - 1);

msg('Hallando descomposición SVD...\n');

[U S V] = svd(X);

msg('Transformando imágenes de entrenamiento...\n');

Ts = {[], [], [], [], [], [], [], [], [], []};
for i = 1:10
    if(rows(imgs{i}) > 0)
        Ts{i} = tc(V, imgs{i}, k);
    endif
end

msg('Hallando medias...\n');

medias = [];
for i = 1:10
    medias(end + 1,:) = mean(Ts{i});
end

medias

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

msg('Calculando tasa de aciertos...\n');

aciertos = 0;

msg('\nImágenes de prueba dígito 0\n\n');

for i=1:rows(test0)
    d = reconocer_digito(V, medias, k, test0(i,:));
    msg(cstrcat('Dígito reconocido: ', num2str(d), '\n'));
    if(d == 0);
        msg('Acierto!\n');
        aciertos++;
    endif
end

msg('\nImágenes de prueba dígito 5\n\n');

for i=1:rows(test5)
    d = reconocer_digito(V, medias, k, test5(i,:));
    msg(cstrcat('Dígito reconocido: ', num2str(d), '\n'));
    if(d == 5);
        msg('Acierto!\n');
        aciertos++;
    endif
end

msg('\nImágenes de prueba dígito 8\n\n');

for i=1:rows(test8)
    d = reconocer_digito(V, medias, k, test8(i,:));
    msg(cstrcat('Dígito reconocido: ', num2str(d), '\n'));
    if(d == 8);
        msg('Acierto!\n');
        aciertos++;
    endif
end

% i = j = 1
% while(j <= num_tests)
%     if(labels(i) == 0 || labels(i) == 5 || labels(i) == 9)
%         j++;
%         if(reconocer_digito(V, medias, k, images(:,:,i)'(:)') == labels(i))
%             msg('Acierto!\n');
%             aciertos++;
%         endif
%     endif
%     i++;
% end

printf('Tests: %d\n', num_tests);
printf('Aciertos: %d\n', aciertos);
printf('Tasa de aciertos: %f\n', aciertos * 100 / num_tests);


function T = tc(V, img, k)

T = V'(1:k,:) * double(img');
T = T';


function d = reconocer_digito(V, medias, k, img)

T = tc(V, img, k);

D = zeros(1, 10);
for i = 1:10
    D(i) = norm(T - medias(i,:));
end
%[x, idx] = min(D);
[x, idx] = min([D(1) D(6) D(9)]);

if(idx == 1)
    d = 0;
endif
if(idx == 2)
    d = 5;
endif
if(idx == 3)
    d = 8;
endif

%d = idx - 1;


function msg(m)

printf(m);
fflush(stdout);