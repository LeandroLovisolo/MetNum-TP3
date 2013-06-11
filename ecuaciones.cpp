#include "Matriz.h"
#include <iostream>
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