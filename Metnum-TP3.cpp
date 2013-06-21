#include <iostream>
#include <fstream>
#include <tuple>
#include "ecuaciones.h"
#include "Matriz.h"
#include "misc.h"
#include "tests.h"
#include <stdlib.h> 

using namespace std;

void Ayuda(string ejecutable) {
    exit(-1);
}

int main(int argc, char *argv[]) {
	//productoParalelo();

	Matriz* trainImages = leerMNISTimages((char*) "train-images.idx3-ubyte");
	Matriz* trainLabels = leerMNISTlabels((char*) "train-labels.idx1-ubyte");
	cout << "Leyo las imagenes de entrenamiento" << endl;
	Matriz* Xtrain = matrizDeCovarianza(*trainImages);
	cout << "Matriz X de entrenamiento" << endl;
	Matriz* testImages = leerMNISTimages((char*)"t10k-images.idx3-ubyte");
	Matriz* testLabels = leerMNISTlabels((char*)"t10k-labels.idx1-ubyte");
	
	cout << "Leyo las imagenes de test" << endl;
	//Matriz* Xtest = matrizDeCovarianza(*testImages);
	//cout << "Matriz X de test" << endl;
	Matriz* VtOriginal = generarVt(*Xtrain);
	for(int i=0;i<VtOriginal->filas();i+=50) {
		Matriz* Vt = VtOriginal->submatriz(0, i, 0, VtOriginal->columnas()-1);
		Matriz* medias = matrizDeMedias(*trainImages,*trainLabels,*Vt);
		int aciertos = adivinarDigitoDeAUno(*testImages,*testLabels,*medias,*Vt);
		double porcentaje = ((double)aciertos/10000)*100;
		cout << "Porcentaje de aciertos = " << porcentaje << " para k = " << i << endl;
		delete medias;
		delete Vt;
	}
	//cout << "Cargado Vt" << endl;
	//cout << "Matriz de medias" << endl;
	//double aciertos = adivinarDigitoMasivamente(*Xtest,*testLabels,*medias,*Vt);
	//double aciertos = adivinarDigitoDeAUno(*Xtest,*testLabels,*medias,*Vt);
	delete VtOriginal;
	delete testImages;
	delete testLabels;
	delete trainImages;
	delete trainLabels;
	delete Xtrain;
}