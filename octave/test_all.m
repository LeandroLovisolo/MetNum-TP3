function test_all()

num_images = 4000;
num_tests = 500;
k = 10;

msg('Cargando imágenes de entrenamiento...\n');

images = leerMNISTimage('../train-images-idx3-ubyte');
labels = leerMNISTlabel('../train-labels-idx1-ubyte');

msg('Filtrando imágenes de entrenamiento por dígito...\n');

imgs = {[], [], [], [], [], [], [], [], [], []};

for i = 1:num_images
    imgs{labels(i) + 1}(end + 1, :) = images(:, :, i)'(:)';
end

msg('Armando matriz X...\n');

X = [];
for(i = 1:10)
    X = [X; imgs{i}];
end
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

msg('Cargando imágenes de prueba...\n');

images = leerMNISTimage('../t10k-images-idx3-ubyte');
labels = leerMNISTlabel('../t10k-labels-idx1-ubyte');

msg('Calculando tasa de aciertos...\n');

aciertos = 0;

for i = 1:num_tests
    if(reconocer_digito(V, medias, k, images(:,:,i)'(:)') == labels(i))
        aciertos++;
    endif
end

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

[x, idx] = min(D);
d = idx - 1;


function msg(m)

printf(m);
fflush(stdout);