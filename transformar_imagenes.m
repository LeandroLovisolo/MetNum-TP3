function T = transformar_imagenes(X, k)

[U S V] = svd(X);

T = V'(1:k,:) * double(X');
T = T';