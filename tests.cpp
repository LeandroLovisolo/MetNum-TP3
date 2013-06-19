#include "Matriz.h"
#include <iostream>
#include <tuple>
#include "misc.h"
using namespace std;

void productoParalelo() {
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
	cout << "Matriz A" << endl;
	A.print();
	cout << "Matriz At" << endl;
	Matriz At(A);
	At.transponer();
	At.print();
	cout << "Matriz At*A" << endl;
	Matriz *res = A*At;
	res->print();
	delete res;
}

void givensTest() {
	cout << "Givens test" << endl;
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
	tuple <Matriz*, Matriz*> res = A.factorizacionGivens();
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
	tuple <Matriz*, Matriz*> res = A.factorizacionHouseHolderDos();
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

void diagonalizacionQRTest(double precision) {
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
	tuple <Matriz*, Matriz*> res = A.diagonalizacionQR(precision);
	cout << "Matriz Qk" << endl;
	get<0>(res)->print();
	cout << "Matriz Ak" << endl;
	get<1>(res)->print();
	cout << "Matriz A = Q*Ak*Qk'" << endl;
	Matriz *QA = (*get<0>(res))*(*get<1>(res));
	get<0>(res)->transponer();
	Matriz *QAQt = (*QA)*(*get<0>(res));
	QAQt->print();
	cout << "Matriz Q*Qt" << endl;
	Matriz* Q = new Matriz(*get<0>(res));
	get<0>(res)->transponer();
	Matriz* QQt = (*Q)*(*get<0>(res));
	QQt->print();
	delete Q;
	delete QQt;
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

void normaCuadradaVectorial() {
	cout << "Matriz A" << endl;
	Matriz A(6,1);
	A.elem(0,0) = 1;
	A.elem(1,0) = 2;
	A.elem(2,0) = 3;
	A.elem(3,0) = 4;
	A.elem(4,0) = 5;
	A.elem(5,0) = 6;
	A.print();
	cout << "Norma cuadrado " << A.normaCuadradoVectorial();
}

void nuevasOperacionseMatriz() {
	cout << "Matriz A" << endl;
	Matriz A(1,3);
	A.elem(0,0) = 1;
	A.elem(0,1) = 2;
	A.elem(0,2) = 3;
	A.print();
	cout << "Matriz A transpuesta" << endl;
	Matriz At(A);
	At.transponer();
	At.print();
	Matriz* res = At*A;
	res->print();
	cout << "Matriz res por cte" << endl;
	Matriz* cteRes = (*res)*2;
	cteRes->print();
	Matriz* restaCteRes = (*cteRes) - (*cteRes);
	cout << "Matriz restaCteRes" << endl;
	restaCteRes->print();
	delete res;
	delete cteRes;
	delete restaCteRes;
}