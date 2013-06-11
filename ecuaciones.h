#ifndef ECUACIONES_H_
#define ECUACIONES_H_
#include "Matriz.h"

Matriz* matrizDeCovarianza(Matriz& A);
Matriz* TC(Matriz& x, Matriz&Vt);
Matriz* generarVt(Matriz& A, char* fileName=NULL);

#endif