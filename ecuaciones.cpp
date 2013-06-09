#include "Matriz.h"

Matriz* matrizDeCovarianza(Matriz& A) {
	Matriz *X = A.transformarAMediaCero();
	Matriz *Xret = (*X)*(1/(X->filas()-1));
	delete X;
	return Xret;
}