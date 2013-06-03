function T = transformar_imagen(X, k, x)

[U S V] = svd(X);

T = V'(1:k,:) * double(x');
T = T';