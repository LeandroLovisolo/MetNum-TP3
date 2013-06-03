function Xs = leer_imagenes(images_path, labels_path)

images = leerMNISTimage(images_path); %(:,:,1:1000);
labels = leerMNISTlabel(labels_path); %(    1:1000,:);

Xs = {};

for i = 0:9
    printf('Filtrando dígito %d...\n', i);
    fflush(stdout);
    X = filtrar_por_digito(images, labels, i);
    Xs{end + 1} = X;
end