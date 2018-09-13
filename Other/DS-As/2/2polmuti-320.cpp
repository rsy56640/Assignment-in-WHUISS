#include <iostream> 
#include"polynomial.h"
using namespace std;

Polynomial::Polynomial() {
	num = 0;
	coef = nullptr;
	exp = nullptr;
}
Polynomial::Polynomial(const Polynomial& p) {
	if (p.num > 0) {
		num = p.num;
		coef = new double[num];
		exp = new int[num];
		for (int i = 0; i < num; ++i) {
			coef[i] = p.coef[i];
			exp[i] = p.exp[i];
		}
	}
	else {
		num = 0;
		coef = nullptr;
		exp = nullptr;
		cerr << "ERROR: num <= 0." << endl;
		return;
	}
}
Polynomial::~Polynomial() {
	if (num > 0) {
		delete[]coef;
		delete[]exp;
	}
}
Polynomial& Polynomial::operator=(const Polynomial& p) {
	if (coef == p.coef)return *this;							//in case that a=a;
	if (p.num <= 0) {
		num = 0;
		if (coef)delete[]coef;
		coef = nullptr;
		if (exp)delete[]exp;
		exp = nullptr;
		cerr << "ERROR: num <= 0." << endl;
		return *this;
	}
	if (p.num > 0) {
		num = p.num;
		if (coef)delete[]coef;
		if (exp)delete[]exp;
		coef = new double[num];
		exp = new int[num];
		for (int i = 0; i < num; ++i) {
			coef[i] = p.coef[i];
			exp[i] = p.exp[i];
		}
		return *this;
	}
}
Polynomial operator*(const Polynomial& p1, const Polynomial& p2) {
	Polynomial p;
	int n = p1.num * p2.num;
	if (n == 0)return p;
	int *texp = new int[n];
	double *tcoef = new double[n];
	int cur = 0;
	for (int i = 0; i < p1.num; ++i) {
		for (int j = 0; j < p2.num; ++j) {
			texp[cur] = p1.exp[i] + p2.exp[j];
			tcoef[cur] = p1.coef[i] * p2.coef[j];
			++cur;
		}
	}
	p.num = n;
	p.exp = texp; texp = nullptr;
	p.coef = tcoef; tcoef = nullptr;
	p.sort();
	return p;
}
void Polynomial::inputPoly() {
	if (num > 0) {
		delete[]coef;
		delete[]exp;
	}
	cout << "please input the number of terms." << endl;
	cin >> num;
	if (num > 0) {
		coef = new double[num];
		exp = new int[num];
	}
	else {
		num = 0;
		coef = nullptr;
		exp = nullptr;
		cerr << "ERROR: num <= 0." << endl;
		return;
	}
	cout << "please input the coefficient sequence." << endl;
	for (int i = 0; i < num; ++i)
		cin >> coef[i];
	cout << "please input the exponent sequence." << endl;
	for (int i = 0; i < num; ++i)
		cin >> exp[i];
	this->sort();
}
void Polynomial::outputPoly() {
	if (num <= 0) {
		cerr << "ERROR: num <= 0." << endl;
		return;
	}
	else {
		cout << "outputPoly: ";
		for (int i = 0; i < num - 1; ++i)
			cout << coef[i] << "*x^" << exp[i] << " + ";
	}
	cout << coef[num - 1] << "*x^" << exp[num - 1] << endl;
}
void Polynomial::sort() {
	if (num <= 0) {
		cerr << "ERROR: num <= 0." << endl;
		return;
	}
	for (int i = 0; i < num - 1; ++i) {
		for (int j = 0; j < num - 1 - i; ++j) {
			if (exp[j] > exp[j + 1]) {
				swap(exp[j], exp[j + 1]);
				swap(coef[j], coef[j + 1]);
			}
		}
	}
	int rest = 0;
	for (int i = 0; i < num - 1; ++i) {
		if (exp[i] == exp[i + 1])rest++;
	}
	if (rest == 0)return;
	int *texp = new int[num - rest];
	double *tcoef = new double[num - rest];
	int cur = 0;
	texp[0] = exp[0];
	tcoef[0] = coef[0];
	for (int i = 1; i < num; ++i) {
		if (texp[cur] == exp[i]) {
			tcoef[cur] += coef[i];
		}
		else {
			cur++;
			texp[cur] = exp[i];
			tcoef[cur] = coef[i];
		}
	}
	num -= rest;
	rest = 0;
	for (int i = 0; i < num; ++i) {
		if (abs(tcoef[i]) <= 1e-3)rest++;
	}
	if (rest == 0) {
		delete[]exp; exp = texp; texp = nullptr;
		delete[]coef; coef = tcoef; tcoef = nullptr;
	}
	else {
		delete[]exp; exp = new int[num - rest];
		delete[]coef; coef = new double[num - rest];
		cur = 0;
		for (int i = 0; i < num; ++i) {
			if (abs(tcoef[i]) > 1e-3) {
				exp[cur] = texp[i];
				coef[cur] = tcoef[i];
				++cur;
			}
		}
		num -= rest;
		delete[]texp; texp = nullptr;
		delete[]tcoef; tcoef = nullptr;
	}
}

int main()
{
	Polynomial ah, bh, ch;
	ah.inputPoly();      //input polynomial a
	ah.outputPoly();
	bh.inputPoly();      //input polynomial b
	bh.outputPoly();
	ch = ah*bh;
	ch.outputPoly();	 //output the result

	system("pause");
	return 0;
}
