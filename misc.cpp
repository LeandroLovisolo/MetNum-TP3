#include <iostream>
#include <fstream>
#include <tuple>
#include "Matriz.h"
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

tuple <Matriz*,Matriz*,Matriz*,Matriz*,Matriz*,Matriz*,
Matriz*,Matriz*,Matriz*,Matriz*> tuplaDeDigitos(Matriz& img, Matriz& lab) {
	Matriz* m0;
	Matriz* m1;
	Matriz* m2;
	Matriz* m3;
	Matriz* m4;
	Matriz* m5;
	Matriz* m6;
	Matriz* m7;
	Matriz* m8;
	Matriz* m9;
	m0 = filtrarPorDigito(0, img, lab);
	m1 = filtrarPorDigito(1, img, lab);
	m2 = filtrarPorDigito(2, img, lab);
	m3 = filtrarPorDigito(3, img, lab);
	m4 = filtrarPorDigito(4, img, lab);
	m5 = filtrarPorDigito(5, img, lab);
	m6 = filtrarPorDigito(6, img, lab);
	m7 = filtrarPorDigito(7, img, lab);
	m8 = filtrarPorDigito(8, img, lab);
	m9 = filtrarPorDigito(9, img, lab);
	return make_tuple(m0,m1,m2,m3,m4,m5,m6,m7,m8,m9);
}