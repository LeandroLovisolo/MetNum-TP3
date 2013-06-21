function test_cotas()


num_images = 60000;
num_tests = 10000;
k = 20;
num_iteraciones = 30;

msg('Cargando imágenes de entrenamiento...\n');

images = leerMNISTimage('../train-images.idx3-ubyte');
labels = leerMNISTlabel('../train-labels.idx1-ubyte');

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

msg('Cargando imágenes de prueba...\n');

images = leerMNISTimage('../t10k-images.idx3-ubyte');
labels = leerMNISTlabel('../t10k-labels.idx1-ubyte');

msg('Obteniendo matriz de covarianza...\n');

Ak = X' * X;
Qk = eye(size(Ak));

for i=1:num_iteraciones
    printf('Iteración %d\n', i);
    fflush(stdout);

    msg('Diagonalizando...\n');

    [Ak Qk err] = iteracion_diagonalizar(Ak, Qk);

    printf('Error: %f\n', err);
    fflush(stdout);

    msg('Ordenando autovectores...\n');

    V = autovectores(Ak, Qk);
    Vtransp = V';

    msg('Transformando imágenes de entrenamiento...\n');

    Ts = {[], [], [], [], [], [], [], [], [], []};
    for i = 1:10
        if(rows(imgs{i}) > 0)
            Ts{i} = tc(Vtransp, imgs{i}, k);
        endif
    end

    msg('Hallando medias...\n');

    medias = [];
    for i = 1:10
        medias(end + 1,:) = mean(Ts{i});
    end

    msg('Calculando tasa de aciertos...\n');

    aciertos = 0;

    for i = 1:num_tests
        if(reconocer_digito(Vtransp, medias, k, images(:,:,i)'(:)') == labels(i))
            aciertos++;
        endif
    end

    printf('Tasa de aciertos: %f\n', aciertos * 100 / num_tests);
    fflush(stdout);
end


function [Ak Qk err] = iteracion_diagonalizar(Ak, Qk)

[Q R] = qr(Ak);
Ak = R*Q;
Qk = Qk*Q;
err = sum(sum(abs(Ak - triu(Ak))));


function V = autovectores(Ak, Qk)

[_ i] = sort(diag(Ak), 'descend');
V = Qk * eye(columns(Qk))(i, :);


function T = tc(Vtransp, img, k)

T = Vtransp(1:k,:) * double(img');
T = T';


function d = reconocer_digito(Vtransp, medias, k, img)

T = tc(Vtransp, img, k);

D = zeros(1, 10);
for i = 1:10
    D(i) = norm(T - medias(i,:));
end

[x, idx] = min(D);
d = idx - 1;


function msg(m)

printf(m);
fflush(stdout);