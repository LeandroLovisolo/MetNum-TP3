function generateV()
	msg('Cargando imágenes de entrenamiento...\n');
	%images = leerMNISTimage('../train-images-idx3-ubyte');
	X = leerMNISTimageRows('../train-images-idx3-ubyte');

	msg('Armando matriz X...\n');

	mu = mean(X);
	for i = 1:rows(X)
	    X(i,:) = X(i,:) - mu;
	end

	X = X / sqrt(columns(X) - 1);

	msg('Diagonalizando X...\n');

	[Xk Q] = diagonalizar(X' * X, 1000);
	[_ i] = sort(diag(Xk), 'descend');
	V = Q * eye(columns(Q))(i, :);

	saveToCMatrix(V, 'VmatAllimgs.mat');

function msg(m)

printf(m);
fflush(stdout);