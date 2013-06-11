function Xs = leer_imagenes(images_path, labels_path)

images = leerMNISTimage(images_path);
labels = leerMNISTlabel(labels_path);

Xs = {};

for i = 0:9
    X = filtrar_por_digito(images, labels, i);
    Xs{end + 1} = X;
end