#include <iostream>
#include <fstream>
#include <tuple>
#include "ecuaciones.h"
#include "Matriz.h"
#include "misc.h"

using namespace std;

void Ayuda(string ejecutable) {
    exit(-1);
}

int main(int argc, char *argv[]) {
	Matriz* imagenes = leerMNISTimages((char*) "train-images.idx3-ubyte");
	cout << "Leyo las imagenes" << endl;
	Matriz* X = matrizDeCovarianza(*imagenes);
	cout << "Matriz X" << endl;
	Matriz* Xt = new Matriz(*X);
	Xt->transponer();
	cout << "Matrix Xt" << endl;
	Matriz* XtX = (*Xt)*(*X);
	cout << "Matriz XtX columnas = " << XtX->columnas();
	cout << "Matriz XtX filas = " << XtX->filas();
	XtX->save((char*)"XtX.mat");
	delete imagenes;
	delete X;
	delete Xt;
	delete XtX;
}
