#include <iostream>
#include <fstream>
#include <tuple>
#include "ecuaciones.h"
#include "Matriz.h"
#include "misc.h"
#include "tests.h"

using namespace std;

int main(int argc, char *argv[]) {
	Matriz* trainImages = leerMNISTimages((char*) "train-images.idx3-ubyte");
	Matriz* trainLabels = leerMNISTlabels((char*) "train-labels.idx1-ubyte");
	cout << "Leyo las imagenes de entrenamiento" << endl;
	Matriz* Xtrain = matrizX(*trainImages);
	cout << "Matriz X de entrenamiento" << endl;
	Matriz* testImages = leerMNISTimages((char*)"t10k-images.idx3-ubyte");
	Matriz* testLabels = leerMNISTlabels((char*)"t10k-labels.idx1-ubyte");	
	cout << "Leyo las imagenes de test" << endl;

	cout << "Cargando Vt" << endl;
	Matriz* Vt = generarVt(*Xtrain);
	cout << "Matriz de medias" << endl;
	Matriz* medias = matrizDeMedias(*trainImages,*trainLabels,*Vt);
	int aciertos = aciertosAdivinacion(*testImages,*testLabels,*medias,*Vt);
	double porcentaje = ((double)aciertos/10000)*100;
	cout << "Aciertos " << porcentaje << endl;
	delete medias;
	delete testImages;
	delete testLabels;
	delete trainImages;
	delete trainLabels;
	delete Xtrain;
	delete Vt;
}