function test_diff()
    msg('Cargando imágenes de entrenamiento...\n');
    trainImages = leerMNISTimageRows('../train-images-idx3-ubyte');
    trainLabels = leerMNISTlabel('../train-labels-idx1-ubyte');
    msg('Cargando imágenes de prueba...\n');
    testImages = leerMNISTimageRows('../t10k-images-idx3-ubyte');
    testLabels = leerMNISTlabel('../t10k-labels-idx1-ubyte');
    msg('Filtrando imágenes de entrenamiento por dígito...\n');
    cantImagenes = [0,0,0,0,0,0,0,0,0,0];
    k = 600;
    for i = 1:60000
        cantImagenes(1,trainLabels(i) + 1)++;
    endfor
    msg('Pasa el conteo de imgs \n');
    imgs = {zeros(cantImagenes(1,1),784), zeros(cantImagenes(1,2),784) , zeros(cantImagenes(1,3),784), zeros(cantImagenes(1,4),784), zeros(cantImagenes(1,5),784), zeros(cantImagenes(1,6),784), zeros(cantImagenes(1,7),784), zeros(cantImagenes(1,8),784), zeros(cantImagenes(1,9),784), zeros(cantImagenes(1,10),784)};
    posEnMatrizDeImg = [1,1,1,1,1,1,1,1,1,1];

    for i = 1:60000
        imgs{trainLabels(i) + 1}(posEnMatrizDeImg(1,trainLabels(i) + 1), :) = trainImages(i,1:end);
        posEnMatrizDeImg(1,trainLabels(i) + 1)++;
    end

    msg('Armando matriz X...\n');
    X = trainImages;
    mu = mean(X);
    for i = 1:rows(X)
        X(i,:) = X(i,:) - mu;
    end
    X = X / sqrt(columns(X) - 1);
    aciertosPorIteracion = [];
    Xk = X' * X;
    Q = eye(size(Xk)(2));
    %for i=fliplr(1000:1000000:10000000)
    %i = 90033704;
    for i=1:50
        fprintf('El i actual es = %i \n', i);
        fflush(stdout);
        %textI = 'Vmat';
        %textF = '.mat';
        %fname = strcat(textI,strcat(num2str(i),textF));
        %fname = 'Vmat4.365e+09.mat';
        %V = readCMatrix('Vmat91000.mat');
        %V = V';
        %[Xk Q] = diagonalizar(Xt, i);
        %msg('Sale de la diagonalizacion \n');
        [Qj R] = qr(Xk);
        Xk = R*Qj;
        Q = Qj*Q;

        [_ i] = sort(diag(Xk), 'descend');
        V = Q * eye(columns(Q))(i, :);
        V = V';
        %V = Xt';
        %Xt = Xk;

        %msg('Transformando imágenes de entrenamiento...\n');
        Ts = {[], [], [], [], [], [], [], [], [], []};
        for i = 1:10
            if(rows(imgs{i}) > 0)
                Ts{i} = tc(V, imgs{i}, k);
            endif
        end

        %msg('Hallando medias...\n');

        medias = [];
        for i = 1:10
            medias(end + 1,:) = mean(Ts{i});
        end

        %msg('Calculando tasa de aciertos...\n');

        aciertos = 0;

        for i = 1:10000
            digito = reconocer_digito(V, medias, k, testImages(i,1:end));
            %fprintf('Digito encontrado %i, digito real %i', digito, testLabels(i));
            %fflush(stdout);
            if(digito == testLabels(i))
                aciertos++;
            endif
        end
        %printf('Aciertos: %d\n', aciertos);
        printf('Tasa de aciertos: %f\n', aciertos * 100 / 10000);
        aciertosPorIteracion(1, end + 1) = aciertos * 100 / 10000;
   endfor

   save('aciertos_diferentes_cotas.mat', aciertosPorIteracion);


function V = autovectores(X)

% Opcion 1: Usar descomposición SVD
% [U S V] = svd(X);

% Opcion 2: Usar función eig()
% [V LAMBDA] = eig(X' * X);
% [_ i] = sort(diag(LAMBDA), 'descend');
% V = V * eye(columns(V))(i, :);

% Opcion 3: Usar función diagonalizar()
    [Xk Q] = diagonalizar(X' * X, 1000);
    [_ i] = sort(diag(Xk), 'descend');
    V = Q * eye(columns(Q))(i, :);


function T = tc(V, img, k)
    T = V(1:k,:) * img';
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
    fprintf(m);
    fflush(stdout);