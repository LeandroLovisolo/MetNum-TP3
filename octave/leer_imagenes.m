function [Img, Lab] = leer_imagenes(images_path, labels_path)
images = leerMNISTimage(images_path);
printf('Imagenes leidas')
Lab = leerMNISTlabel(labels_path);
printf('Labels leidas')
Img = [];
for i = 1:size(images)(3)
	imgrow = images(:,:,i)'(:)';
    if(length(Img) == 0)
		Img = imgrow;
	else
		Img = [Img; imgrow];
	endif
endfor
mu = mean(Img);
size(Img)
n = size(X)(1);
Img = (Img - mu) / sqrt(n - 1);