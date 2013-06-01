function [Ak,Q] = diagonalizar(A,cota)
	Q = eye(3);
	Ak = A;
	Abuf = Ak;
	Abuf(logical(eye(size(Abuf)))) = 0;
	suma = sum(sum(abs(tril(Abuf))));
	while suma > cota
		[Qk, Rk] = qr(Ak);
		Q = Q*Qk;
		Ak = Rk*Qk;
		Abuf = Ak;
		Abuf(logical(eye(size(Abuf)))) = 0;
		suma = sum(sum(abs(tril(Abuf))));
	endwhile
