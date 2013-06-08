#include <iostream>
#include <fstream>
#include <tuple>
#include "Matriz.h"
#include <stdio.h>
using namespace std;

unsigned int convertToLittleEndian(unsigned int value) {
	unsigned int ret;
	unsigned char *dst = (unsigned char*) &ret;
	unsigned char *src = (unsigned char*) &value;
	dst[0] = src[3];
	dst[1] = src[2];
	dst[2] = src[1];
	dst[3] = src[0];
}

Matriz *leerMNIST(char *fileName) {
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
		for(unsigned int i=0;i<nimgs;i++) {
			for(unsigned int j=0;j<nrows*ncols;j++) {
				char buf;
				file.read(&buf, 1);
				mat->elem(i,j) = buf;
			}
		}
		file.close();
	}
	return mat;
}

void Ayuda(string ejecutable) {
    exit(-1);
}

int main(int argc, char *argv[]) {
	cout << "HouseHolder test" << endl;
	cout << "Matriz A" << endl;
	/* Matriz de la práctica
	Matriz A(3,3);
	A.elem(0,0) = 0;
	A.elem(0,1) = -3;
	A.elem(0,2) = 4;

	A.elem(1,0) = 4;
	A.elem(1,1) = 0;
	A.elem(1,2) = 2;

	A.elem(2,0) = -3;
	A.elem(2,1) = 5;
	A.elem(2,2) = 1;
	A.print();
	cout << "Matriz Q" << endl;
	tuple <Matriz*, Matriz*> res = A.factorizacionHouseHolder();
	get<0>(res)->print();
	cout << "Matriz R" << endl;
	get<1>(res)->print();
	cout << "Q*R" << endl;
	Matriz *QR = (*get<0>(res))*(*get<1>(res));
	QR->print();
	delete QR;
	*/
	//Matriz simetrica para ver diagonalización
	Matriz A(3,3);
	A.elem(0,0) = 1;
	A.elem(0,1) = 2;
	A.elem(0,2) = 3;

	A.elem(1,0) = 2;
	A.elem(1,1) = 5;
	A.elem(1,2) = 7;

	A.elem(2,0) = 3;
	A.elem(2,1) = 7;
	A.elem(2,2) = 8;
	A.print();
	cout << "Diagonalizo A" << endl;
	tuple <Matriz*, Matriz*> res = A.diagonalizacionQR(0.1);
	cout << "Matriz Qk" << endl;
	get<0>(res)->print();
	cout << "Matriz Ak" << endl;
	get<1>(res)->print();
	cout << "Matriz A = Q*Ak*Qk'" << endl;
	Matriz *QA = (*get<0>(res))*A;
	get<0>(res)->transponer();
	Matriz *QAQt = (*QA)*(*get<0>(res));
	QAQt->print();
	delete QAQt;
	delete QA;
	delete get<0>(res);
	delete get<1>(res);
}
