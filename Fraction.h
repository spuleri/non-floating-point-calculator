#ifndef FRACTION_H_
#define FRACTION_H_

#include <vector>
#include<string>
#include "Number.h"

using namespace std;

	class Fraction : public Number {

	private:
		Number* numerator;
		Number* denominator;
		bool isPrime(long value);
		
	public:
		Fraction(Number* numerator, Number* deonminator);
		virtual ~Fraction();
		Number* getNumerator();
		Number* getDenominator();
		vector<long> primeFactorization(long value);
		vector<long> primeFactorization(long value, vector<long> primes, long lowestLeft);
		Number* lcd(Fraction* other);
		long gcd(long num, long denom);

		Number* rationalizeDenominator();
		Number* copy();

		//abstract methods

		Number* simplify();
		Number* add(Number* value);
		Number* subtract(Number* value);
		Number* multiply(Number* value);
		Number* divide(Number* value);

		bool equals(Number* value);
		string getType();
		Number* getCoefficient();
		string toString();
		string getObjectType();



	};

 /* namespace std */
#endif /* FRACTION_H_ */