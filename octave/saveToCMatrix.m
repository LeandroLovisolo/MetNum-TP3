function saveToCMatrix(X, fname)
	fp = fopen(fname, 'w');
	if fp == -1
		error(['Error: no se puede abrir archivo ' fname]);
	else
		fprintf('Archivo abierto \n');
		fflush(stdout);
	endif
	matrixInfo = [size(X)(2), size(X)(1)]; %Cols y fils
	fwrite(fp,matrixInfo, 'uint32', 0,  'native');
	matrixInfo = [0];
	fwrite(fp,matrixInfo, 'uchar', 0, 'native');
	fwrite(fp, X, 'double', 0, 'native');
	fclose(fp);