#include<vector>
#include "Number.h"
#ifndef EXPONENTIAL_H_
#define EXPONENTIAL_H_
namespace std
{
	class Exponential : public Number
	{
	public:
		Exponential(Number* b, Number* e, Number* c);
		virtual ~Exponential();
		Number* getBase();
		Number* getExponent();
		Number* getCoefficient();
		Number* simplify();
		Number* add(Number* value);
		Number* subtract(Number* value);
		Number* multiply(Number* value);
		Number* divide(Number* value);
		string toString();
		string getType();
		bool equals(Number* value);
		string getObjectType();
		Number* copy();
	private:
		Number* base;
		Number* exponent;
		Number* coefficient;

		string getSign(Number* value);
		vector<long> primeFactorization(long value);
		vector<long> primeFactorization(long value, vector<long> primes, long lowestLeft);
	};

}
#endif /* EXPONENTIAL_H_ */
