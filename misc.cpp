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
	return ret;
}

Matriz** tuplaDeDigitos(Matriz& img, Matriz& lab) {
	Matriz** arr = new Matriz*[10];
	for(int i=0;i<10;i++) {
		arr[i] = filtrarPorDigito(i, img, lab);
	}
	return arr;
}

Matriz** tuplaDeDigitosTransformados(Matriz** digitos, Matriz &Vt) {
	Matriz** arr = new Matriz*[10];
	for(int i=0;i<10;i++) {
		arr[i] = TC(*digitos[i],Vt);
		arr[i]->transponer();
	}
	return arr;
}

//La operación media funciona correctamente

Matriz* matrizDeMedias(Matriz& trainImages, Matriz& trainLabels, Matriz& Vt) {
	Matriz** filteredTrain = tuplaDeDigitos(trainImages, trainLabels);

	Matriz** trainImgTrans = tuplaDeDigitosTransformados(filteredTrain, Vt);
	Matriz* ret = new Matriz(10, Vt.columnas());
	for(int i=0;i<10;i++) {
		Matriz* media = trainImgTrans[i]->media();
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
	double minNorm = 0;
	for(int j=0;j<transformada->filas();j++) {
		minNorm += pow(transformada->elem(j,0) - medias.elem(0,j),2);
	}
	minNorm = sqrt(minNorm);
	//cout << "minNorm inicial = " << minNorm << endl;
	int minNormIndex = 0;
	for(int i=1;i<10;i++) {
		double normBuf = 0;
		for(int j=0;j<transformada->filas();j++) {
			normBuf += pow(transformada->elem(j,0) - medias.elem(i,j),2);
		}
		normBuf = sqrt(normBuf);
		//cout << "Norma actual " << normBuf << " vs minNorm " << minNorm << endl;
		if(minNorm > normBuf) {
			minNorm = normBuf;
			minNormIndex = i;
		}
	}
	delete transformada;
	return minNormIndex;
}


int aciertosAdivinacion(Matriz &trainSet, Matriz &testLabels, Matriz &medias, Matriz& Vt) {
	//Me muevo por las imágenes
	int aciertos = 0;
	for(int h=0;h<trainSet.filas();h++) {
		Matriz* imagen = trainSet.submatriz(h,h,0,trainSet.columnas()-1);
		int digito = adivinarDigito(*imagen, medias, Vt);
		if(digito == testLabels.elem(h,0)) {
			aciertos++;
		}
		delete imagen;
	}
	return aciertos;
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
		double tmp = autovalores[i];
		autovalores[i] = autovalores[maxPos];
		autovalores[maxPos] = tmp;
		Avectores.intercambiarFilas(i,maxPos);
		Avalores.intercambiarFilas(i,maxPos);
	}
	Avectores.transponer();
}