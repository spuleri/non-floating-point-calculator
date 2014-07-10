#ifndef IRRATIONAL_H_ 
#define IRRATIONAL_H_ 

#include "Number.h" 
#include<string>

using namespace std;

class Irrational : public Number 
{
private:
	string value;
	Number* coefficient;
public:
	bool equals(Number* n);
	Irrational(string s, Number* coef);
	Irrational(string s);
	string getType();
	string getValue();
	string toString();
	Number* add(Number* j);
	Number* subtract(Number* j);
	Number* multiply(Number* j);
	Number* divide(Number* j);
	Number* simplify();
	Number* getCoefficient();
	string getObjectType();
	Number* copy();
};
#endif
