#include "Irrational.h"  
#include "Integer.h"  
#include "Series.h" 
#include "Fraction.h" 
#include "Exponential.h" 
#include <vector> 
#include <stdexcept> 
#include<iostream> 
#include<string> 
using namespace std;

Irrational::Irrational(string s, Number * coef){
	if (s.compare("e") != 0 && s.compare("pi") != 0)
		throw invalid_argument("ERROR: not e or pi");
	else{
		value = s;
		coefficient = coef;
	}
}

Irrational::Irrational(string s){
	if (s.compare("e") != 0 && s.compare("pi") != 0)
		throw invalid_argument("ERROR: not e or pi");
	else{
		value = s;
		Integer* o = new Integer(1);
		coefficient = o;
	}
}

Number* Irrational::copy(){
	return new Irrational(getValue(), coefficient->copy());
}

string Irrational::getType(){ return "irrational"; }
string Irrational::getValue(){
	return value;
}
Number* Irrational::getCoefficient(){ return coefficient; }

string Irrational::toString(){
	Number* n = simplify();
	if (n->getType().compare(getType()) != 0)
		return n->toString();
	Irrational* n1 = (Irrational*)n;
	string s;
	if (n1->coefficient->equals(new Integer(-1)))
		s += "-";
	else if (!(n1->coefficient->equals(new Integer(1))))
		s += n1->getCoefficient()->toString();
	if (n1->coefficient->getType().compare("irrational") == 0)
		s += "*";
	if (n1->coefficient->getType().compare("exponential") == 0 || n1->coefficient->getType().compare("logarithm") == 0)
		return value + "*" + s;
	s += n1->getValue();
	return s;
}

bool Irrational::equals(Number * i){
	if (i->getType() != this->getType())
		return false;
	else if (this->getValue().compare(((Irrational*)i)->getValue()) == 0 && this->getCoefficient()->equals(i->getCoefficient()))
		return true;
	else if (coefficient->getType().compare("irrational") == 0 && i->getCoefficient()->getType().compare("irrational") == 0){
		if (((Irrational*)i->getCoefficient())->getValue().compare(value) == 0 && ((Irrational*)coefficient)->getValue().compare(((Irrational*)i)->getValue())){
			return i->getCoefficient()->getCoefficient()->equals(coefficient->getCoefficient());
		}
	}
	else
		return this->getValue().compare(((Irrational*)i)->getValue()) == 0 && this->getCoefficient()->equals(i->getCoefficient());
}

Number* Irrational::add(Number * n){
	n = n->simplify();
	if (this->getType().compare(n->getType()) == 0)   // if same Number  
	{
		if (this->getValue().compare(((Irrational*)n)->getValue()) == 0){ // and also if same "type" of Irrational  
			Irrational* ni = new Irrational(this->getValue(), this->getCoefficient()->add(n->getCoefficient())); //adds coeff  
			return ni;
		}
	}
	vector<Number*> v;
	v.push_back(this);
	v.push_back(n);
	Series* s = new Series(v);
	return s;
}

Number* Irrational::subtract(Number * n){
	return this->add(n->multiply(new Integer(-1)));
}

Number* Irrational::multiply(Number * n){
	if (this->getType().compare(n->getType()) == 0){ // if same Number  
		if (this->getValue().compare(((Irrational*)n)->getValue()) == 0){ // and also if same "type" of Irrational  
			Number* ni = new Exponential(new Irrational(this->getValue()), new Integer(2), this->getCoefficient()->multiply(n->getCoefficient())); //multiplies coeff  
			return ni;
		}
		else{
			coefficient = coefficient->multiply(n)->simplify();
			return this;
		}

	}
	else if (n->getType().compare("exponential") == 0) {
		Exponential* e = (Exponential*)n;
		if (e->getBase()->getType().compare("irrational") == 0){
			if (((Irrational*)(e->getBase()))->getValue().compare(value) == 0){
				if (e->getExponent()->getType().compare("irrational") != 0 && e->getExponent()->getType().compare("logarithm") != 0){
					Number* temp = (new Exponential(e->getBase(), e->getExponent()->add(new Integer(1)), coefficient->multiply(e->getCoefficient())));

					//      Number*  
					return temp->simplify();
				}
			}
			else{
				Number* c = n->getCoefficient()->multiply(this);
				Number* b = ((Exponential*)n)->getBase();
				Number* e = ((Exponential*)n)->getExponent();
				return (new Exponential(b, e, c))->simplify();
			}
		}
	}
	//  else { // otherwise..   
	Irrational* ni = new Irrational(this->getValue(), this->getCoefficient()->multiply(n));
	return ni;
	//  } 
}

Number* Irrational::divide
(Number * n){
	if (this->getType().compare(n->getType()) == 0){ // if same Number  
		if (this->getValue().compare(((Irrational*)n)->getValue()) == 0){ // and also if same "type" of Irrational  
			Fraction* ni = new Fraction(this->getCoefficient(), n->getCoefficient());
			return ni->simplify();
		}
		else{
			Fraction* ni = new Fraction(this, n);
			return ni->simplify();
		}
	}
	else if (n->getType().compare("exponential") == 0 && (((Exponential*)n)->getBase())->getType().compare("irrational") == 0 && (((Irrational*)((Exponential*)n)->getBase())->getValue()).compare(this->getValue()) == 0){

		if (((Exponential*)n)->getExponent()->getType().compare("integer") == 0){

			Number* newexp = (((Exponential*)n)->getExponent())->subtract(new Integer(1));

			return new Fraction(new Integer(1), new Exponential(((Exponential*)n)->getBase(), newexp, ((Exponential*)n)->getCoefficient()));

		}

		else if (((Exponential*)n)->getExponent()->getType().compare("fraction") == 0){

			Number* newexp = (((Exponential*)n)->getExponent())->subtract(new Integer(1));

			return new Fraction(new Integer(1), new Exponential(((Exponential*)n)->getBase(), newexp, ((Exponential*)n)->getCoefficient()));

		}
		else{
			return new Fraction(this, n);
		}






	}          //divides coeff  
	else { // otherwise..   
		Irrational* ni = new Irrational(this->getValue(), this->getCoefficient()->divide(n));
		return ni;
	}
}

Number* Irrational::simplify(){
	if (this->getCoefficient()->getType().compare("integer") == 0) {
		if (getCoefficient()->equals(new Integer((long)0)))
			return new Integer((long)0);
		return this;
	}
	else if (coefficient->getType().compare("fraction") == 0){
		coefficient = coefficient->multiply(new Irrational(getValue()));
		return coefficient;
	}
	else
		return this;
}

string Irrational::getObjectType()
{
	return "number";
}