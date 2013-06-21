function saveToCMatrix(X, fname)
	fp = fopen(fname, 'w');
	if fp == -1
		error(['Error: no se puede abrir archivo ' fname]);
	end
	matrixInfo = [size(X)(2), size(X)(1)] %Cols y fils
	fwrite(fp,matrixInfo,'integer*4',0,'native');
	matrixInfo = [0]
	fwrite(fp,matrixInfo,'uchar',0,'native');
	for i=1:size(X)(1)
		fila = X(i,1:end);
		fwrite(fp,fila,'double',0,'native');
	endfor