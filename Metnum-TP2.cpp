#include <iostream>
#include <tuple>
#include "Matriz.h"

using namespace std;

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
	*/
	//Matriz simetrica para ver diagonalización
	Matriz A(3,3);
	A.elem(0,0) = 1;
	A.elem(0,1) = 3;
	A.elem(0,2) = 4;

	A.elem(1,0) = 3;
	A.elem(1,1) = 2;
	A.elem(1,2) = 6;

	A.elem(2,0) = 4;
	A.elem(2,1) = 6;
	A.elem(2,2) = 5;
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
	/*
	cout << "Diagonalizo A" << endl;
	Matriz *QA = (*get<0>(res))*A;
	get<0>(res)->transponer();
	Matriz *QAQt = (*QA)*(*get<0>(res));
	QAQt->print();
	delete QAQt;
	delete QA;
	*/
}
