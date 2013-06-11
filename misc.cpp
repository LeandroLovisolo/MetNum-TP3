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

Matriz *leerMNISTimages(char *fileName) {
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