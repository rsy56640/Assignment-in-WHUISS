#pragma once

class Polynomial {
private:
	int num;
	double *coef;
	int *exp;
public:
	Polynomial();
	Polynomial(const Polynomial& p);
	~Polynomial();
	Polynomial& operator=(const Polynomial& p);
	friend Polynomial operator*(const Polynomial& p1, const Polynomial& p2);
	void inputPoly();
	void outputPoly();
	void sort();							//sort and merge
};