#include <iostream>
#include <fstream>
#include <tuple>
#include "ecuaciones.h"
#include "Matriz.h"
#include "misc.h"
#include "tests.h"

using namespace std;

void Ayuda(string ejecutable) {
    exit(-1);
}

int main(int argc, char *argv[]) {
	//houseHolderTest();
	
	Matriz* trainImages = leerMNISTimages((char*) "train-images.idx3-ubyte");
	Matriz* trainLabels = leerMNISTlabels((char*) "train-labels.idx1-ubyte");
	cout << "Leyo las imagenes de entrenamiento" << endl;
	Matriz* Xtrain = matrizDeCovarianza(*trainImages);
	cout << "Matriz X de entrenamiento" << endl;
	Matriz* testImages = leerMNISTimages((char*)"t10k-images.idx3-ubyte");
	Matriz* testLabels = leerMNISTlabels((char*)"t10k-labels.idx1-ubyte");
	
	cout << "Leyo las imagenes de test" << endl;
	Matriz* Xtest = matrizDeCovarianza(*testImages);
	cout << "Matriz X de test" << endl;
	Matriz* Vt = generarVt(*Xtrain);
	cout << "Cargado Vt" << endl;
	Matriz* medias = matrizDeMedias(*trainImages,*trainLabels,*Vt);
	cout << "Matriz de medias" << endl;
	double aciertos = adivinarDigitoMasivamente(*Xtest,*testLabels,*medias,*Vt);
	cout << "Porcentaje de aciertos = " << aciertos << endl;
	
}
