function X = filtrar_por_digito(images, labels, digito)

X = [];

for i = 1:size(images)(3)
    if(labels(i) == digito)
        imgrow = images(:,:,i)'(:)';
        if(length(X) == 0)
            X = imgrow;
        else
            X = [X; imgrow];
        endif
    endif
endfor

mu = mean(X);
n = size(X)(1);
X = (X - mu) / sqrt(n - 1);