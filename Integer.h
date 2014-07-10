#ifndef INTEGER_H_ 
#define INTEGER_H_

#include "Number.h" 

class Integer : public Number {
private:
	long value;
public:
	Integer(Number * in);
	Integer(long in);
	bool equals(Number* n);
	Number * getCoefficient();
	std::string getType();
	std::string toString();
	long getValue();
	Number* add(Number * j);
	Number* subtract(Number * j);
	Number* multiply(Number * j);
	Number* divide(Number * j);
	Number* simplify();
	std::string getObjectType();
	Number* copy();
};
#endif