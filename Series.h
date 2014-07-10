#ifndef SERIES_H_ 
#define SERIES_H_ 

#include "Number.h" 
#include <vector>
#include<string>
using namespace std;

class Series : public Number{
private:
	vector<Number*> values;
public:
	Series();
	Series(std::vector<Number*> v);
	std::string toString();
	Number* add(Number * j);
	Number* subtract(Number * j);
	Number* multiply(Number * j);
	Number* divide(Number * j);
	Number* simplify();
	Number* getCoefficient();
	bool equals(Number * val);
	string getType();
	vector<Number*> getValues();
	string getObjectType();
	string getSign(Number* j);
	Number* copy();
};
#endif

