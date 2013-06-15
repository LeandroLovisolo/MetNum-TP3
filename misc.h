#ifndef MISC_H_
#define MISC_H_
#include "Matriz.h"
#include <tuple>
using namespace std;

Matriz* leerMNISTimages(char *fileName);
Matriz* leerMNISTlabels(char *fileName);
Matriz* filtrarPorDigito(unsigned char digito, Matriz& img, Matriz& lab);
/*
tuple <Matriz*,Matriz*,Matriz*,Matriz*,Matriz*,Matriz*,
Matriz*,Matriz*,Matriz*,Matriz*> tuplaDeDigitos(Matriz& img, Matriz& lab);

tuple <Matriz*,Matriz*,Matriz*,Matriz*,Matriz*,Matriz*,
Matriz*,Matriz*,Matriz*,Matriz*> tuplaDeDigitosTransformados(
	tuple <Matriz*,Matriz*,Matriz*,Matriz*,Matriz*,Matriz*,
Matriz*,Matriz*,Matriz*,Matriz*> digitos, Matriz &Vt);
*/

Matriz* matrizDeMedias(Matriz& trainImages, Matriz& trainLabels, Matriz& Vt);
double adivinarDigitoMasivamente(Matriz &x, Matriz &testLabels, Matriz &medias, Matriz& Vt);
int adivinarDigito(Matriz &x, Matriz &medias, Matriz& Vt);
bool fileExists(const char *fileName);

#endif