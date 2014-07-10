#include "Integer.h" 
#include "Series.h"
#include "Irrational.h"
#include "Exponential.h"
#include "Fraction.h"
#include <vector>
#include<string>
#include<sstream>
#include<stdexcept>
#include<climits>
#include<iostream>
#include<cmath>
using namespace std;

static const long MAXIMUM = pow(2, 100);

Integer::Integer(Number* in)
{
	Integer* poop = dynamic_cast<Integer*> (in);
	long numb = poop->getValue();

	value = numb;


}

Integer::Integer(long in)
{
	value = in;
	if (in + 1 <= in || in - 1 >= in || in >= LONG_MAX || in <= LONG_MIN)
		throw invalid_argument("Overflow, sorry, your numbers are too big to manage. Are you trying to compensate for something?");

}

Number* Integer::copy(){
	return new Integer(value);
}

long Integer::getValue(){ return value; }

string Integer::getType() { return "integer"; }

string Integer::toString(){

	ostringstream value;

	long i = this->getValue();
	value << i;
	return value.str();
}

Number* Integer::getCoefficient() { return this; } //is its own coefficient, for all practical purposes  

bool Integer::equals(Number * j){
	if (this->getType().compare(j->getType()) != 0) // input not an Integer 
		return false;
	else
		return (this->value == ((Integer*)j)->value ? true : false);
}

Number* Integer::add(Number* j){ //add 

	j = j->simplify();

	if (this->getType().compare(j->getType()) == 0){
		Integer* i = new Integer(this->value + ((Integer*)j)->value);
		if (((Integer*)j)->value > 0 && i->value < this->value ||
			((Integer*)j)->value < 0 && i->value > this->value)
			throw overflow_error("WOW! THAT'S HUGE!!! I can't take something that big! Try some smaller numbers");
		return i;
	}

	else if ((j->getType()).compare("fraction") == 0){
		Fraction* adder = dynamic_cast<Fraction*> (j);
		return adder->add(this);
	}
	vector<Number*> v;
	v.push_back(this);
	v.push_back(j);
	Series* s = new Series(v);
	return s;

}

Number* Integer::subtract(Number* j){ //subtract 
	return add(j->multiply(new Integer(-1)));
}

Number* Integer::multiply(Number* j){ //multiply 

	j = j->simplify();
	if (value == 1)
		return j;
	if (this->getValue() == 0 || ((Integer*)j)->getValue() == 0)
		return new Integer((long)0);
	if (this->getType().compare(j->getType()) == 0){
		Integer* i = new Integer(this->value * ((Integer*)j)->value);
		if (i->getValue() / ((Integer*)j)->getValue() != this->getValue())
			throw overflow_error("SO! BIG! IT! HURTS! MY! ... CPU! What did you think I was gonna say?\n  I'm a computer not a human. Try multiplying smaller numbers.");

		return i;
	}
	if (j->getType().compare("irrational") == 0){
		Irrational * k = dynamic_cast<Irrational*>(j);
		Irrational *ni = new Irrational(k->getValue(), k->getCoefficient()->multiply(this));
		return ni;
	}
	if (j->getType().compare("exponential") == 0) {
		Exponential * w = dynamic_cast<Exponential*>(j);
		Exponential *ne = new Exponential(w->getBase(), w->getExponent(), w->getCoefficient()->multiply(this));
		return ne;
	}
	else
		return j->multiply(this);
}

Number* Integer::divide(Number* j){ //divide 

	j = j->simplify();

	j = j->simplify();
	if (this->getType().compare(j->getType()) == 0){
		if (((Integer*)j)->value == 0){
			throw invalid_argument("ERROR: Cannot divide by 0, or... can you?");
		}
		Number* f = new Fraction(new Integer(this->value), new Integer(((Integer*)j)->value));
		return f;
	}

	else if ((j->getType()).compare("fraction") == 0){
		Fraction* temp = dynamic_cast<Fraction*> (j);

		Fraction* tempfrac = new Fraction(this, new Integer(1));

		tempfrac->divide(temp);

	}
	else {
		return (new Fraction(this, j))->simplify();
	}
}

Number* Integer::simplify(){ return this; } // so simple 

string Integer::getObjectType()
{
	return "number";
}