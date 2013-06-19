#include <iostream>
#include <fstream>
#include <tuple>
#include "Matriz.h"
#include "ecuaciones.h"
#include <cmath>

using namespace std;

unsigned int convertToLittleEndian(unsigned int value) {
	unsigned int ret;
	unsigned char *dst = (unsigned char*) &ret;
	unsigned char *src = (unsigned char*) &value;
	dst[0] = src[3];
	dst[1] = src[2];
	dst[2] = src[1];
	dst[3] = src[0];
	return ret;
}

Matriz* leerMNISTimages(char *fileName) {
	Matriz *mat = 0;
	ifstream file(fileName, ios::binary);
	if(file.is_open()) {
		unsigned int magicNumber;
		unsigned int nimgs;
		unsigned int nrows;
		unsigned int ncols;
		file.read((char *)&magicNumber, 4);
		file.read((char *)&nimgs, 4);
		file.read((char *)&nrows, 4);
		file.read((char *)&ncols, 4);
		magicNumber = convertToLittleEndian(magicNumber);
		nimgs = convertToLittleEndian(nimgs);
		nrows = convertToLittleEndian(nrows);
		ncols = convertToLittleEndian(ncols);
		mat = new Matriz(nimgs, nrows*ncols);
		unsigned char buf;
		for(unsigned int i=0;i<nimgs;i++) {
			for(unsigned int j=0;j<nrows*ncols;j++) {
				file.read((char*)&buf, 1);
				mat->elem(i,j) = buf;
			}
		}
		file.close();
	}
	else {
		cout << "Error abriendo el archivo!" << endl;
	}
	return mat;
}
//Bien
Matriz* leerMNISTlabels(char *fileName) {
	Matriz *mat = 0;
	ifstream file(fileName, ios::binary);
	if(file.is_open()) {
		unsigned int magicNumber;
		unsigned int nitems;
		file.read((char *)&magicNumber, 4);
		file.read((char *)&nitems, 4);
		magicNumber = convertToLittleEndian(magicNumber);
		nitems = convertToLittleEndian(nitems);
		mat = new Matriz(nitems, 1);
		unsigned char buf;
		for(unsigned int i=0;i<nitems;i++) {
			file.read((char*)&buf, 1);
			mat->elem(i,0) = buf;
		}
		file.close();
	}
	else {
		cout << "Error abriendo el archivo!" << endl;
	}
	return mat;
}
//Bien
Matriz* filtrarPorDigito(unsigned char digito, Matriz& img, Matriz& lab) {
	unsigned int count = 0;
	for(int i=0;i<lab.filas();i++) {
		if(lab.elem(i,0) == digito) {
			count++;
		}
	}
	Matriz *ret = new Matriz(count, img.columnas());
	count = 0;
	for(int i = 0; i < lab.filas(); ++i) {
		if(lab.elem(i,0) == digito) {
			for (int j = 0; j < img.columnas(); ++j) {
				ret->elem(count, j) = img.elem(i,j);
			}
			count++;
		}
	}
	cout << count << " imagenes con el digito " << (int)digito << endl;
	return ret;
}

Matriz** tuplaDeDigitos(Matriz& img, Matriz& lab) {
	Matriz** arr = new Matriz*[10];
	for(int i=0;i<10;i++) {
		cout << "-----------------------------------------------------" << endl;
		cout << "Digito " << i <<" filtrado" << endl;
		arr[i] = filtrarPorDigito(i, img, lab);
		cout << "Columnas = " << arr[i]->columnas() << " filas = " << arr[i]->filas() << endl;
	}
	return arr;
}

Matriz** tuplaDeDigitosTransformados(Matriz** digitos, Matriz &Vt) {
	Matriz** arr = new Matriz*[10];
	cout << "Vt columnas = " << Vt.columnas() << " filas = " << Vt.filas() << endl;
	for(int i=0;i<10;i++) {
		cout << "-----------------------------------------------------" << endl;
		cout << "Transformando digito " << i << endl;
		cout << "Columnas antes de transformar " << digitos[i]->columnas() << endl;
		cout << "Filas antes de transformar " << digitos[i]->filas() << endl;
		arr[i] = TC(*digitos[i],Vt);
		cout << "Transponiendo matriz transformada" << endl;
		arr[i]->transponer();
		cout << "Matriz transpuesta" << endl;
		cout << "Columnas = " << arr[i]->columnas() << " filas = " << arr[i]->filas() << endl;
	}
	return arr;
}

//La operación media funciona correctamente

Matriz* matrizDeMedias(Matriz& trainImages, Matriz& trainLabels, Matriz& Vt) {
	Matriz** filteredTrain = tuplaDeDigitos(trainImages, trainLabels);

	Matriz** trainImgTrans = tuplaDeDigitosTransformados(filteredTrain, Vt);
	Matriz* ret = new Matriz(10, Vt.columnas());
	cout << "Matriz de medias en blanco" << endl;
	for(int i=0;i<10;i++) {
		cout << "Fila de medias " << i << endl;
		Matriz* media = trainImgTrans[i]->media();
		media->transponer();
		cout << "Norma cuadrado " << media->normaCuadradoVectorial() << endl;;
		exit(1);
		for(int j=0;j<media->columnas();j++) {
			ret->elem(i,j) = media->elem(0,j);
		}
		delete media;
		delete trainImgTrans[i];
		delete filteredTrain[i];
	}
	return ret;
}

int adivinarDigito(Matriz &x, Matriz &medias, Matriz& Vt) {
	Matriz* transformada = TC(x, Vt);
	double minNorm = 0; //cambiar
	for(int j=0;j<transformada->filas();j++) {
		minNorm += pow(transformada->elem(j,0) - medias.elem(0,j),2);
	}
	minNorm = sqrt(minNorm);

	int minNormIndex = 0;
	for(int i=0;i<10;i++) {
		double normBuf = 0;
		for(int j=0;j<transformada->filas();j++) {
			normBuf = pow(transformada->elem(j,1) - medias.elem(i,j),2);
		}
		normBuf = sqrt(normBuf);
		if(minNorm > normBuf) {
			minNorm = normBuf;
			minNormIndex = i;
		}
	}
	return minNormIndex;
}

double adivinarDigitoMasivamente(Matriz &x, Matriz &testLabels, Matriz &medias, Matriz& Vt) {
	Matriz* transformada = TC(x, Vt); //X queda transpuesto después de esto
	transformada->transponer();
	cout << "----------------------------------------------------------" << endl;
	cout << "Adiviando digitos" << endl;
	cout << "Transformada filas " << transformada->filas() << " transformada columnas = " << transformada->columnas() << endl;
	int aciertos = 0;
	//Para cada (h) columna de transformada (imagenes) veo que digito es
	for(int h=0;h<testLabels.filas();h++) {
		double minNorm = 0;
		int minNormIndex = 0;
		//Consigo la distancia entre la matriz de medias 0 y el vector a comparar
		for(int j=0;j<transformada->filas();j++) {
			minNorm += pow(transformada->elem(j,h) - medias.elem(0,j),2);
		}
		minNorm = sqrt(minNorm);
		//Veo las demás distancias y busco la mínima
		for(int i=1;i<10;i++) {
			double normBuf = 0;
			for(int j=0;j<transformada->filas();j++) {
				normBuf += pow(transformada->elem(j,h) - medias.elem(i,j),2);
			}
			normBuf = sqrt(normBuf);
			//cout << "Norma actual = " << normBuf << " Norma minima = " << minNorm << endl;
			if(minNorm > normBuf) {
				minNorm = normBuf;
				minNormIndex = i;
			}
		}
		cout << "Digito de la imagen " << testLabels.elem(h,0) << " digito adivinado = " << minNormIndex << endl;
		if(testLabels.elem(h,0) == minNormIndex) {
			aciertos++;
		}
	}
	cout << "Aciertos " << (double)aciertos << endl;
	cout << "Cantidad de imagenes " << testLabels.filas() << endl;
	return testLabels.filas()/(double)aciertos;
}

bool fileExists(const char *fileName) {
    ifstream infile(fileName);
    return infile.good();
}

void ordenarAuAv(Matriz& Avectores, Matriz& Avalores) {
	double autovalores[Avalores.columnas()];
	//Cargo los Av en un arreglo para hacer más rápida su recorrida
	for(int i=0;i<Avalores.columnas();i++) {
		autovalores[i] = abs(Avalores.elem(i,i));
	}
	//Transpongo porque los Avectores están en columnas
	Avectores.transponer();
	double max = 0;
	int maxPos = 0;
	//Selection sort con filas
	for(int i=0;i<Avalores.columnas();i++) {
		max = 0;
		maxPos = i;
		for(int j=i;j<Avalores.columnas();j++) {
			if(autovalores[j] > max) {
				max = autovalores[j];
				maxPos = j;
			} 
		}
		Avectores.intercambiarFilas(i,maxPos);
		Avalores.intercambiarFilas(i,maxPos);
	}
	Avectores.transponer();
}