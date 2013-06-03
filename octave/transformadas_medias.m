function Ms = transformadas_medias(Xs, k, n)

Ms = zeros(length(Xs), k);

for i=1:length(Xs)
    T = transformar_imagenes(Xs{i}(1:n,:), k);
    Ms(i,:) = mean(T);
end