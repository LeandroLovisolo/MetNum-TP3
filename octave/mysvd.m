function [U S V] = mysvd(X)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Usando eig()

% [U LAMBDA] = eig(X * X');
% [_ i] = sort(diag(LAMBDA), 'descend');
% U = U * eye(columns(U))(i, :);

% [V LAMBDA] = eig(X' * X);
% [_ i] = sort(diag(LAMBDA), 'descend');
% V = V * eye(columns(V))(i, :);

% s = sort(diag(LAMBDA), 'descend');
% s = sqrt(abs(s));

% S = zeros(rows(X), columns(X));

% for i = 1:length(s)
%     S(i,i) = s(i);
% end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Usando diagonalizar()

cota = 0.01;

[Uk Q] = diagonalizar(X * X', cota);
[_ i] = sort(diag(Uk), 'descend');
U = Q * eye(columns(Q))(i, :);

[Vk Q] = diagonalizar(X' * X, cota);
[s i] = sort(diag(Vk), 'descend');
V = Q * eye(columns(Q))(i, :);

s = sqrt(abs(s));

S = zeros(rows(X), columns(X));

for i = 1:min(length(s), rows(S))
    S(i,i) = s(i);
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%