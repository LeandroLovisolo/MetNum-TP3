function X = readCMatrix(fname)
	fp = fopen(fname);
	if fp == -1
		error(['Error: no se puede abrir archivo ' fname]);
		fprintf('Se cagó todo \n');
		fname
		fflush(stdout);
	else
		fprintf('Archivo abierto \n')
		fflush(stdout);
	endif
  	ncols = fread(fp,1,'uint32');
  	nrows = fread(fp,1,'uint32');
  	trans = fread(fp,1, 'uchar');
  	X = zeros(nrows, ncols);
	for i=1:2
		fila = (fread(fp,ncols,'double')');
		X(i,1:end) = fila(1,1:ncols);
	endfor