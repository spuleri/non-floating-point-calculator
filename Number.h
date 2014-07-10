#ifndef NUMBER_H_
#define NUMBER_H_

//#pragma once
#include<string>
#include "Object.h"

class Number :
	public Object
{
public:

	virtual Number* simplify() = 0;
	virtual Number* add(Number* value) = 0;
	virtual Number* subtract(Number* value) = 0;
	virtual Number* multiply(Number* value) = 0;
	virtual Number* divide(Number* value) = 0;
	virtual std::string toString() = 0;
	virtual Number* getCoefficient() = 0;
	virtual bool equals(Number* value) = 0;
	virtual std::string getType() = 0;
	virtual std::string getObjectType() = 0;
	virtual Number* copy()=0;
};

#endif