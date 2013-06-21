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
	Xt = X' * X;
	Q = eye(size(Xt)(2));
	textI = 'Vmat';
	textF = '.mat';
	msg('Inicio del loop \n');
	for i=fliplr(1000:2500:105000)
		fprintf('El i actual es = %i \n', i);
		fflush(stdout);
		[Xk Q] = diagonalizar(Xt, i);
		msg('Sale de la diagonalizacion \n');
		fname = strcat(textI,strcat(num2str(i),textF));
		[_ i] = sort(diag(Xk), 'descend');
		V = Q * eye(columns(Q))(i, :);
		saveToCMatrix(V, fname);
		Xt = Xk;
	endfor

function msg(m)

fprintf(m);
fflush(stdout);