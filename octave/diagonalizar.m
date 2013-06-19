function [Ak Qk] = diagonalizar(A, cota)

Ak = A;
Qk = eye(size(A));

while(sum(sum(abs(Ak - triu(Ak)))) > cota)
	[Q R] = qr(Ak);
	Ak = R*Q;
	Qk = Qk*Q;
	printf('Suma: %f\n', sum(sum(abs(Ak - triu(Ak)))));
	fflush(stdout);
endwhile


% function [Ak Q] = diagonalizar(A, cota)

% Q = eye(size(A));
% Ak = A;
% Abuf = Ak;
% Abuf(logical(eye(size(Abuf)))) = 0;
% suma = sum(sum(abs(tril(Abuf))));
% while suma > cota
% 	[Qk, Rk] = qr(Ak);
% 	Q = Q*Qk;
% 	Ak = Rk*Qk;
% 	Abuf = Ak;
% 	Abuf(logical(eye(size(Abuf)))) = 0;
% 	suma = sum(sum(abs(tril(Abuf))));
% 	printf('Suma: %f\n', suma);
% 	fflush(stdout);	
% endwhile