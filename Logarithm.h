#ifndef LOGARITHM_H_
#define LOGARITHM_H_

//#pragma once
#include "Series.h"
#include <vector>
#include <string>
#include "Fraction.h"
#include "Exponential.h"

using namespace std;

class Logarithm :
	public Number
{
private:
	Number* base;
	Number* x;
	Number* coefficient;

	vector<long> primeFactorization(long value);
	vector<long> primeFactorization(long value, vector<long> primes, long lowestLeft);

public:
	Logarithm(Number* base, Number* x, Number* coefficient);
	~Logarithm();
	Number* simplify();
	Number* add(Number* value);
	Number* subtract(Number* value);
	Number* multiply(Number* value);
	Number* divide(Number* value);
	string toString();
	Number* getCoefficient();
	bool equals(Number*);
	string getType();
	string getObjectType();
	Number* getBase();
	Number* getX();
	Number* copy();
};

#endif