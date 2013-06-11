#ifndef MISC_H_
#define MISC_H_
#include "Matriz.h"

Matriz* leerMNISTimages(char *fileName);
Matriz* leerMNISTlabels(char *fileName);
Matriz* filtrarPorDigito(unsigned char digito, Matriz& img, Matriz& lab);
tuple <Matriz*,Matriz*,Matriz*,Matriz*,Matriz*,Matriz*,
Matriz*,Matriz*,Matriz*,Matriz*> tuplaDeDigitos(Matriz& img, Matriz& lab);

#endif