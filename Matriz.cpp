#include <cmath>
#include <iostream>
#include <fstream>
#include "Matriz.h"

using namespace std;

Matriz::Matriz(const int filas, const int columnas) {
	_filas = filas;
	_columnas = columnas;
	_transpuesta = false;
	vectorMatriz = new double[filas * columnas];
}

Matriz::Matriz(Matriz& otra) {
	_filas = otra._filas;
	_columnas = otra._columnas;
	_transpuesta = otra._transpuesta;
	vectorMatriz = new double[_filas * _columnas];
	for(int i = 0; i < _filas * _columnas; i++) vectorMatriz[i] = otra.vectorMatriz[i];
}

Matriz::~Matriz() {
	delete vectorMatriz;
}

void Matriz::save(char *fileName) {
	ofstream file(fileName, ios::binary);
	if(file.is_open()) {
		file.write((char*)&_columnas, 4);
		file.write((char*)&_filas, 4);
		file.write((char*)&_transpuesta, 1);
		file.write((char*)vectorMatriz, _columnas*_filas*sizeof(double));
	}
	else {
		cout << "Error grabando la matriz!" << endl;
	}
}

Matriz::Matriz(char *fileName) {
	ifstream file(fileName, ios::binary);
	if(file.is_open()) {
		file.read((char *)&_columnas, 4);
		file.read((char *)&_filas, 4);
		file.read((char *)&_transpuesta, 1);
		vectorMatriz = new double[_filas * _columnas];
		file.read((char*)vectorMatriz,_columnas*_filas*sizeof(double));
	}
	else {
		cout << "Error cargando matriz!" << endl;
	}
}

Matriz* Matriz::identidad(int n) {
	Matriz *m = new Matriz(n, n);
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			m->elem(i, j) = (i == j ? 1 : 0);
		}
	}
	return m;
}

Matriz* Matriz::submatriz(const int desdeFil, const int hastaFil, const int desdeCol, const int hastaCol) {
	Matriz *submatriz = new Matriz((hastaFil - desdeFil + 1), (hastaCol - desdeCol + 1));
	for(int i = desdeFil; i <= hastaFil; i++) {
		for(int j = desdeCol; j <= hastaCol; j++) {
			submatriz->elem(i - desdeFil, j - desdeCol) = elem(i,j);
		}
	}
	return submatriz;
}

void Matriz::cambiarSubmatriz(Matriz& submatriz, const int desdeFil, const int hastaFil, const int desdeCol, const int hastaCol) {
	for(int i = desdeFil; i <= hastaFil; i++) {
		for(int j = desdeCol; j <= hastaCol; j++) {
			elem(i,j) = submatriz.elem(i - desdeFil, j - desdeCol);
		}
	}
}

void Matriz::cambiarColumna(Matriz& mat, const int columna) {
	for(int i = 0;i < _filas; i++) {
		elem(i,columna) = mat.elem(i,0);
	}
}

int Matriz::filas() const {
	return _filas;
}

int Matriz::columnas() const {
	return _columnas;
}

double* Matriz::transponerCabeza() {
	double *vectorBuf = new double[_filas*_columnas];
	for(int i=0;i<_columnas;i++) {
		for(int j=0;j<_filas;j++) {
			vectorBuf[i * _filas + j] = vectorMatriz[j * _columnas + i];
		}
	}
	return vectorBuf;
}

void Matriz::transponer(){
	double *vectorBuf = new double[_filas*_columnas];
	for(int i=0;i<_columnas;i++) {
		for(int j=0;j<_filas;j++) {
			vectorBuf[i * _filas + j] = vectorMatriz[j * _columnas + i];
		}
	}
	delete vectorMatriz;
	vectorMatriz = vectorBuf;
	int temp = _filas;
	_filas = _columnas;
	_columnas = temp;
}

double &Matriz::elem(const int fila, const int columna) {
	return vectorMatriz[fila * _columnas + columna];
}

double Matriz::max() {
	double max = elem(0, 0);
	for(int i = 0; i < _filas; i++) {
		for(int j = 0; j < _columnas; j++) {
			if(elem(i ,j) > max) {
				max = elem(i,j);
			}
		}
	}
	return max;
}

double Matriz::min() {
	double min = elem(0, 0);
	for(int i = 0; i < _filas; i++) {
		for(int j = 0; j < _columnas; j++) {
			if(elem(i, j) < min) {
				min = elem(i, j);
			}
		}
	}
	return min;
}

double Matriz::rango() {
	return max() - min();
}

Matriz* Matriz::operator+(Matriz &m) {
	Matriz* suma = new Matriz(*this);
	for(int i = 0;i<_filas*_columnas;i++) {
		suma->vectorMatriz[i] += m.vectorMatriz[i];
	}
	return suma;
}

Matriz* Matriz::operator-(Matriz &m) {
	Matriz* resta = new Matriz(*this);
	for(int i = 0;i<_filas*_columnas;i++) {
		resta->vectorMatriz[i] -= m.vectorMatriz[i];
	}
	return resta;
}

Matriz* Matriz::operator*(Matriz &m) {
	if(_columnas != m.filas()) {
		cout << "Producto de matrices invalido" << endl;
		cout << "Columnas de matriz izquierda = " << _columnas << " filas derecha = " << m.filas() << endl;
		exit(1);
	}
	Matriz* producto = new Matriz(_filas, m._columnas);
	double* vectorM = m.transponerCabeza();
	for(int i=0;i<_filas;i++) {
		for(int j=0;j<m._columnas;j++) {
			double valor = 0;
			for(int h=0;h<_columnas;h++) {
				 valor += vectorMatriz[i*_columnas + h] * vectorM[j*m._filas+h];
			}
			producto->vectorMatriz[i * m._columnas + j] = valor;
		}
	}
	delete vectorM;
	return producto;
}

Matriz* Matriz::multiplicarPorInversa(Matriz &M) {
	tuple<Matriz*, Matriz*, Matriz*> plu = M.factorizacionPLU();
	//Hago Ly = Px
	Matriz* Px = (*get<0>(plu)) * (*this);
	Matriz* y = get<1>(plu)->forwardSubstitution(*Px);
	//Hago Uj = y
	Matriz* j = get<2>(plu)->backwardSubstitution(*y);
	delete y;
	delete Px;
	delete get<0>(plu);
	delete get<1>(plu);
	delete get<2>(plu);
	return j;
}

Matriz* Matriz::operator*(double k) {
	Matriz* producto = new Matriz(*this);
	for(int i = 0; i < _filas*_columnas; i++) {
		producto->vectorMatriz[i] *= k;
	}
	return producto;
}

tuple <Matriz*, Matriz*, Matriz*> Matriz::factorizacionPLU() {
	Matriz *P = identidad(_filas);
	Matriz *L = identidad(_filas);
	Matriz *U = new Matriz(*this);

	for(int j = 0; j < _columnas - 1; j++) {
		// Intercambio la fila con el máximo absoluto por la actual
		// Tener en cuenta que las columnas también determinan la fila a la cual intecambiar el mayor
		// ya que vamos moviendonos diagonalmente, (j,j) va a tener siempre el maximo

		int jp = U->filaConMayorAbsEnCol(j, j);
		P->intercambiarFilas(jp, j);
		L->intercambiarFilas(jp, j, j);
		U->intercambiarFilas(jp, j);

		for(int i = j + 1; i < _filas; i++) {
			L->elem(i, j) = U->elem(i, j) / U->elem(j, j); // Pongo en L Mij
			U->elem(i, j) = 0; // Pongo en 0 el elemento eliminado
			for(int x = j + 1; x < _columnas; x++) {
				U->elem(i, x) = U->elem(i, x) - L->elem(i, j) * U->elem(j, x);
			}
		}
	}
	return make_tuple(P, L, U);
}

//Solo funciona con vectores!
double Matriz::normaCuadradoVectorial() {
	double acum = 0;
	for(int i = 0; i < _filas; i++) {
		acum += vectorMatriz[i] * vectorMatriz[i];
	}
	return sqrt(acum);
}

tuple <Matriz*, Matriz*> Matriz::factorizacionGivens() {
	Matriz* r = new Matriz(*this);
	Matriz* q = identidad(_filas);
	Matriz* I = new Matriz(*q);
	//cout << "Matriz I original" << endl;
	//I->print();
	int iteracion = 0;
	int lastI = -1, lastJ = -1;
	double x1,x2,norm;
	for(int j=0; j<_columnas-1; j++) {
		for(int i = j+1; i<_filas; i++) {
			if(r->elem(i,j) != 0) {
				x1 = r->elem(j,j);
				x2 = r->elem(i,j); //elemento a poner en 0
				norm = sqrt(abs(x1)*abs(x1)+abs(x2)*abs(x2));
				if(norm == 0) continue;
				iteracion++;
				//Cambio los elementos anteriores
				if(lastI != -1) {
					I->elem(lastI,lastI) = 1;
					I->elem(lastI,lastJ) = 0;
					I->elem(lastJ,lastJ) = 1;
					I->elem(lastJ,lastI) = 0;
				}
				//Pongo los nuevos elementos (tomar j como filas, i como cols)
				lastI = i;
				lastJ = j;
				I->elem(i,i) = x1/norm;
				I->elem(i,j) = -x2/norm;
				I->elem(j,j) = x1/norm;
				I->elem(j,i) = x2/norm;

				Matriz* newR = (*I)*(*r);
				delete r;
				r = newR;
				Matriz* newQ = (*I)*(*q);
				delete q;
				q = newQ;
			}
		}
	}
	delete I;
	q->transponer();
	return make_tuple(q,r);
}

tuple <Matriz*, Matriz*> Matriz::factorizacionHouseHolder() {
	Matriz *r = new Matriz(*this);
	Matriz *q = identidad(_filas);
	for(int i=0; i<_columnas-1; i++) {
		cout << "Columna " << i << endl;
		//Busco la resolución de r
		Matriz *uRes = r->submatriz(0,_filas-1, i, i);
		for(int j=0;j<i;j++) {
			uRes->elem(j,0) = 0;
		}
		uRes->elem(i,0) -= uRes->normaCuadradoVectorial();
		double cte = 2 / pow(uRes->normaCuadradoVectorial(),2);
		if(pow(uRes->normaCuadradoVectorial(),2) == 0) {
			delete uRes;
			continue;
		}
		Matriz* uTRes = new Matriz(*uRes);
		uTRes->transponer();
		Matriz* uTResAk = (*uTRes)*(*r);
		Matriz* uResuTResAk = (*uRes)*(*uTResAk);
		Matriz* cteuResuTResAk = (*uResuTResAk)*cte;
		Matriz* newR = (*r)-(*cteuResuTResAk);
		delete cteuResuTResAk;
		delete uResuTResAk;
		delete uTResAk;
		delete uTRes;
		delete uRes;
		//Consigo el Qk correspondiente para devolverlo
		Matriz* uQ = r->submatriz(i,_filas-1, i, i);
		uRes->elem(0,0) -= uQ->normaCuadradoVectorial();
		Matriz* uTQ = new Matriz(*uQ);
		uTQ->transponer();
		Matriz *uQuTQ = (*uQ)*(*uTQ);
		Matriz *cteuQuTQ = (*uQuTQ)*cte;
		Matriz *I = identidad(cteuQuTQ->_filas);
		Matriz *Hk = (*I)-(*cteuQuTQ);
		Matriz *QTk = identidad(q->_filas);
		QTk->cambiarSubmatriz(*Hk, i, _filas-1, i, _columnas-1);
		Matriz *newQ = (*QTk)*(*q);
		delete uQ;
		delete uTQ;
		delete uQuTQ;
		delete cteuQuTQ;
		delete I;
		delete Hk;
		delete QTk;

		delete r;
		r = newR;
		delete q;
		q = newQ;
	}
	q->transponer();
	return make_tuple(q, r);
}

double Matriz::sumBajoDiagonal() {
	double acum = 0;
	for(int i = 0; i<_columnas; i++) {
		for(int j = i+1; j<_filas; j++) {
			acum += abs(elem(j,i));
		}
	}
	return acum;
}

Matriz* Matriz::ceros(int n, int m) {
	Matriz* ret = new Matriz(n,m);
	for(int i = 0; i < n*m; i++) {
		ret->vectorMatriz[i] = 0;
	}
	return ret;
}

Matriz* Matriz::media() {
	Matriz *mu = ceros(1, _columnas);
	//Consigo vector^t de medias
	for(int i=0;i<_filas;i++) {
		for(int j=0;j<_columnas;j++) {
			mu->elem(0,j) += elem(i,j);
		}
	}
	for(int j=0;j<_columnas;j++) {
		mu->elem(0,j) /= _filas;
	}
	return mu;
}

Matriz* Matriz::transformarAMediaCero() {
	Matriz* mu = media();
	Matriz *original = new Matriz(*this);
	//Resto las medias
	for(int i=0;i<_filas;i++) {
		for(int j=0;j<_columnas;j++) {
			original->elem(i,j) -= mu->elem(0,j);
		}
	}
	delete mu;
	return original;
}

tuple <Matriz*, Matriz*> Matriz::diagonalizacionQR(double cota) {
	Matriz *Q = identidad(_filas);
	Matriz *Ak = new Matriz(*this);
	int i = 0;
	while(Ak->sumBajoDiagonal() > cota/*i != 5*/) {
		cout << "Suma bajo la diagonal " << Ak->sumBajoDiagonal() << endl;
		tuple <Matriz*, Matriz*> res = Ak->factorizacionHouseHolder();//Ak->factorizacionHouseHolder();
		delete Ak;
		//Qk = Q(k-2) * Q(k-1)
		Matriz *newQ = (*Q)*(*get<0>(res));
		delete Q;
		Q = newQ;
		//Ak = Rk*Qk
		Ak = (*get<1>(res))*(*get<0>(res));
		delete get<0>(res);
		delete get<1>(res);
		i++;
	}
	cout << "Suma bajo la diagonal final" << Ak->sumBajoDiagonal() << endl;
	cout << "Numero de iteraciones realizadas: " << i << endl;
	return make_tuple(Q, Ak);
}

Matriz* Matriz::backwardSubstitution(Matriz &b) {
	Matriz *x = new Matriz(_columnas, 1);

	// Voy de la fila de abajo para arriba
	for(int i = _filas - 1; i >= 0; i--) {
		// Me armo un acumulador del nuevo valor Xi, voy construyendo el X de abajo hacia arriba
		// Utilizo Xi = (Bi - sum(Aij, Xj)/Aii j=i+1 hasta n (cols))
		double valorX = b.elem(i, 0);

		// Hago Xi = (Bi - sum(Aij, Xj)
		// Recorro las columnas de la posición + 1 en que tengo mi incognita
		for(int j = i + 1; j < _columnas; j++) {
			valorX -= elem(i,j) * x->elem(j,0);
		}

		// Xi/Aii para terminar
		x->elem(i,0) = valorX / elem(i, i);
	}
	return x;
}

Matriz* Matriz::forwardSubstitution(Matriz &b) {
	Matriz *x = new Matriz(this->_columnas,1);

	for(int i = 0; i < _filas; i++) {
		// Me armo un acumulador del nuevo valor Xi, voy construyendo el X de arriba hacia abajo
		// Utilizo Xi = (Bi - sum(Aij, Xj)/Aii j=i+1 hasta n (cols))
		double valorX = b.elem(i, 0);

		// Hago Xi = (Bi - sum(Aij, Xj)
		for(int j = 0; j < i; j++) {
			valorX -= elem(i, j) * x->elem(j, 0);
		}

		//Xi/Aii para terminar
		x->elem(i, 0) = valorX / elem(i, i);
	}
	return x;
}

void Matriz::print(){
	for(int i = 0; i < _filas; i++) {
		for(int j = 0; j < _columnas; j++) {
			cout << elem(i,j) << '\t';
		}
		cout << endl;
	}
}

void Matriz::intercambiarFilas(const int i, const int j) {
	intercambiarFilas(i, j, _columnas);
}

void Matriz::intercambiarFilas(const int i, const int j, const int hasta) {
	if(i == j) return;
	for(int x = 0; x < hasta; x++) {
		double elemento = elem(i, x);
		elem(i,x) = elem(j,x);
		elem(j,x) = elemento;
	}
}

int Matriz::filaConMayorAbsEnCol(const int col, const int desde) {
	int filaMayor = desde;
	int mayor = elem(desde, col);
	for(int y = desde + 1; y < _filas; y++) {
		if(abs(elem(y, col)) > mayor) {
			mayor = abs(elem(y, col));
			filaMayor = y;
		}
	}
	return filaMayor;
}
