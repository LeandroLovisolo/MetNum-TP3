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
	return 0;
}

Matriz* TC(Matriz& x, Matriz&Vt) {
	//Transponemos ya que tenemos las imágenes como filas
	//Cuidado que transponer modifica x
	x.transponer();
	return Vt*x;
}

Matriz* generarVt(Matriz& A) {
	Matriz* XtX = NULL;
	if(!fileExists((char*)"XtX.mat")) {
		Matriz* imagenes = leerMNISTimages((char*) "train-images.idx3-ubyte");
		cout << "Leyo las imagenes" << endl;
		Matriz* X = matrizDeCovarianza(*imagenes);
		cout << "Matriz X" << endl;
		Matriz* Xt = new Matriz(*X);
		Xt->transponer();
		cout << "Matrix Xt" << endl;
		XtX = (*Xt)*(*X);
		cout << "Matriz XtX columnas = " << XtX->columnas() << endl;
		cout << "Matriz XtX filas = " << XtX->filas() << endl;
		XtX->save((char*)"XtX.mat");
		delete imagenes;
		delete X;
		delete Xt;
	}
	else {
		cout << "Cargando XtX" << endl;
		XtX = new Matriz((char*)"XtX.mat");
		cout << "Matriz XtX cargada" << endl;
		cout << "Columnas XtX " << XtX->columnas() << endl;
	}

	if(!fileExists((char*)"Vt.mat")) {
		cout << "Generando matriz Vt" << endl;
		tuple <Matriz*, Matriz*> res = XtX->diagonalizacionQR(0.1);
		get<0>(res)->transponer();
		get<0>(res)->save((char*)"Vt.mat");
		get<1>(res)->save((char*)"Avalores.mat");
		cout << "Ordenando autovectores" << endl;
		ordenarAuAv(*get<0>(res), *get<1>(res));
		return get<0>(res);
		delete get<1>(res);
		delete XtX;
	}
	else {
		cout << "Cargada matriz Vt" << endl;
		Matriz* Vt = new Matriz((char*)"Vt.mat");
		return Vt;
	}

}