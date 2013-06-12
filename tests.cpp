#include "Matriz.h"
#include <iostream>
#include <tuple>
#include "misc.h"
using namespace std;

void houseHolderTest() {
	cout << "HouseHolder test" << endl;
	cout << "Matriz A" << endl;
	//Matriz de la práctica
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
	Matriz *QT = new Matriz((*get<0>(res)));
	QT->transponer();
	Matriz *QQT = (*get<0>(res))*(*QT);
	cout << "Q*QT"<< endl;
	QQT->print();
	delete QR;
	delete get<0>(res);
	delete get<1>(res);
	delete QQT;
	delete QT;
}

void diagonalizacionQRTest() {
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
	tuple <Matriz*, Matriz*> res = A.diagonalizacionQR(0.01);
	cout << "Matriz Qk" << endl;
	get<0>(res)->print();
	cout << "Matriz Ak" << endl;
	get<1>(res)->print();
	cout << "Matriz A = Q*Ak*Qk'" << endl;
	Matriz *QA = (*get<0>(res))*(*get<1>(res));
	get<0>(res)->transponer();
	Matriz *QAQt = (*QA)*(*get<0>(res));
	QAQt->print();
	delete QAQt;
	delete QA;
	delete get<0>(res);
	delete get<1>(res);
}

void leerImagenTest() {
	Matriz* imagenes = leerMNISTimages((char*) "train-images.idx3-ubyte");
	for(int i=1;i<28;i++) {
		for(int j=0;j<28;j++) {
			cout << imagenes->elem(0,j*i);
		}
		cout << endl;
	}
	delete imagenes;
}

void grabarYCargarMatriz() {
	cout << "Matriz A" << endl;
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
	A.save((char*)"podonga.mat");
	Matriz N((char*)"podonga.mat");
	cout << "Matriz N" << endl;
	N.print();
}

void mediaMatricialTest() {
	cout << "Matriz A" << endl;
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
	Matriz *media = A.media();
	cout << "Matriz media" << endl;
	media->print();
}