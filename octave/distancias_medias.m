function D = distancias_medias(X_train, X_test, k, n, digito)

Ms = transformadas_medias(X_train, k, n);
Ts = transformar_imagenes(X_test{digito + 1}(1:n,:), k);

D = zeros(10, n);

for i = 1:10
    for j = 1:n
        D(i,j) = norm(Ms(i,:) - Ts(j,:));
    end
end