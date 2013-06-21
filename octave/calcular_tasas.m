function calcular_tasas()

num_images = 60000;
num_tests = 10000;
rango_k = 1:100;

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

msg('Hallando autovectores de X...\n');

V = autovectores(X);
Vtransp = V';

msg('Cargando imágenes de prueba...\n');

images = leerMNISTimage('../t10k-images.idx3-ubyte');
labels = leerMNISTlabel('../t10k-labels.idx1-ubyte');

tasas = [];

for k=rango_k
    printf('Calculando tasa de aciertos para k = %d...\n', k);
    fflush(stdout);

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

    tasas(end + 1) = aciertos * 100 / num_tests;

    printf('Tasa de aciertos para k = %d: %f\n', k, aciertos * 100 / num_tests);
    fflush(stdout);
end

msg('Generando tasas.mat...\n');

save('tasas.mat', 'tasas');

function V = autovectores(X)

% Opcion 1: Usar descomposición SVD
% [U S V] = svd(X);

% Opcion 2: Usar función eig()
[V LAMBDA] = eig(X' * X);
[_ i] = sort(diag(LAMBDA), 'descend');
V = V * eye(columns(V))(i, :);

% Opcion 3: Usar función diagonalizar()
% [Xk Q] = diagonalizar(X' * X, 1000);
% [_ i] = sort(diag(Xk), 'descend');
% V = Q * eye(columns(Q))(i, :);


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