function X = readCMatrix(fname)
	fp = fopen(fname);
	if fp == -1
		error(['Error: no se puede abrir archivo ' fname]);
		fprintf('Se cagó todo \n');
		fname
		fflush(stdout);
	else
		fprintf('Archivo abierto \n');
		fflush(stdout);
	endif
  	ncols = fread(fp,1,'uint32', 0,  'native');
  	nrows = fread(fp,1,'uint32', 0,  'native');
  	trans = fread(fp,1, 'uchar', 0,  'native');
  	X = zeros(nrows, ncols);
	for i=1:nrows
		fila = (fread(fp,ncols,'double')');
		%size(fila)
		X(i,1:end) = fila(1,1:end);
	endfor
	%fread(fp,X,'double', 0,  'native')
	fclose(fp);