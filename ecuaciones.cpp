#include "Matriz.h"
#include <iostream>
#include "misc.h"
#include <tuple>
#include <cmath>
using namespace std;

Matriz* matrizDeCovarianza(Matriz& A) {
	Matriz *X = A.transformarAMediaCero();
	Matriz *Xret = (*X)*(1/sqrt(X->filas()-1));
	delete X;
	return Xret;
}

Matriz* TC(Matriz& x, Matriz&Vt) {
	//Transponemos ya que tenemos las imágenes como filas
	//Cuidado que transponer modifica x
	x.transponer();
	return Vt*x;
}

Matriz* generarVt(Matriz& A, char* fileName=NULL) {
	if(fileName == NULL) {
		Matriz* imagenes = leerMNISTimages((char*) "train-images.idx3-ubyte");
		cout << "Leyo las imagenes" << endl;
		Matriz* X = matrizDeCovarianza(*imagenes);
		cout << "Matriz X" << endl;
		Matriz* Xt = new Matriz(*X);
		Xt->transponer();
		cout << "Matrix Xt" << endl;
		Matriz* XtX = (*Xt)*(*X);
		cout << "Matriz XtX columnas = " << XtX->columnas() << endl;
		cout << "Matriz XtX filas = " << XtX->filas() << endl;
		XtX->save((char*)"XtX.mat");
		delete imagenes;
		delete X;
		delete Xt;
		tuple <Matriz*, Matriz*> res = XtX->diagonalizacionQR(0.1);
		delete get<1>(res);
		delete XtX;
		get<0>(res)->transponer();
		return get<0>(res);
	}
	else {
		cout << "Cargando Xtx" << endl;
		Matriz XtX(fileName);
		cout << "Matriz XtX cargada" << endl;
		cout << "Columnas XtX " << XtX.columnas() << endl;
		int count = 0;
		for(int i=0;i<XtX.filas();i++) {
			for(int j=0;j<XtX.columnas();j++) {
				if(XtX.elem(i,j) == 0) {
					count++;
				}
			}
		}
		cout << "Ceros encontrados " << count << endl;
		tuple <Matriz*, Matriz*> res = XtX.diagonalizacionQR(0.1);
		cout << "Matriz diagonalizada" << endl;
		delete get<1>(res);
		get<0>(res)->transponer();
		return get<0>(res);
	}
}