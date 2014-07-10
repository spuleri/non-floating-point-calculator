#include "Fraction.h"
#include "Series.h"
#include "Exponential.h"
#include "Logarithm.h"
#include "Irrational.h"
#include <cmath>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include"Integer.h"

using namespace std;

Fraction::Fraction(Number* numerator, Number* denominator) {
	this->numerator = numerator;

	if (denominator->getType().compare("integer") == 0) {

		Integer* denom = dynamic_cast<Integer*> (denominator);

		long intDenom = denom->getValue();

		if (intDenom == 0) {
			throw invalid_argument("Can't have a denominator as 0");
		}

		else this->denominator = denominator;

	}

	else this->denominator = denominator;


}

Fraction::~Fraction() {

}

Number* Fraction::copy(){
	return new Fraction(numerator->copy(), denominator->copy());
}

Number* Fraction::getNumerator(){
	return this->numerator;
}

Number* Fraction::getDenominator(){
	return this->denominator;
}

Number* Fraction::getCoefficient(){
	return this->numerator;
}

string Fraction::getType(){
	return "fraction";
}

Number* Fraction::rationalizeDenominator() {
	if (denominator->getType().compare("exponential") == 0 && ((Exponential*)denominator)->getExponent()->getType().compare("fraction") == 0){
		Fraction* expo = (Fraction*)((Exponential*)denominator)->getExponent();

		numerator = new Exponential(((Exponential*)denominator)->getBase(), (new Integer(1))->subtract(expo), numerator);
		numerator = numerator->simplify();
		Number* result = ((Exponential*)denominator)->getBase()->multiply(denominator->getCoefficient());
		result = numerator->divide(result);
		return result->simplify();
	}
	else
		return this;
	
}

bool Fraction::equals(Number* value){

	Fraction* f1 = dynamic_cast<Fraction*> (this);
	Fraction* f2 = dynamic_cast<Fraction*> (value);

	if (this->getType().compare(value->getType()) != 0){
		return false;
	}

	else if (f1->getNumerator()->equals(f2->getNumerator()) && f1->getDenominator()->equals(f2->getDenominator())){
		return true;
	}

	else return false;

}

long Fraction::gcd(long num, long denom){
	if (denom == 0){
		return num;
	}

	else{
		return gcd(denom, (num % denom));
	}

}

string Fraction::toString(){
	ostringstream value;

	Number* simple = this->simplify();

	if (dynamic_cast<Fraction*>(simple) != 0) {

		Fraction* simpler = dynamic_cast<Fraction*> (simple);

		Number* n = simpler->getNumerator();
		Number* d = simpler->getDenominator();
		string nstr = n->toString();
		string dstr = d->toString();
		if (n->getType().compare("series") == 0
			|| (n->getType().compare("integer") != 0 && n->getType().compare("fraction") != 0 && !n->getCoefficient()->equals(new Integer(1)))
			|| n->getType().compare("exponential") == 0)
			nstr = "(" + nstr + ")";
		if (d->getType().compare("series") == 0
			|| (d->getType().compare("integer") != 0 && d->getType().compare("fraction") != 0 && !d->getCoefficient()->equals(new Integer(1)))
			|| d->getType().compare("exponential") == 0)
			dstr = "(" + dstr + ")";

		value << nstr << "/" << dstr;

		return value.str();
	}

	else return simple->toString();



}

Number* Fraction::simplify(){ //simplifying a fraction

	numerator = numerator->simplify();
	if (denominator->getType().compare("logarithm") == 0)
		return (new Fraction(numerator->multiply(new Logarithm(((Logarithm*)denominator)->getX(), ((Logarithm*)denominator)->getBase(), new Integer(1))), denominator->getCoefficient()))->simplify();
	denominator = denominator->simplify();
	Number* d1 = this->getDenominator();
	Number* n1 = this->getNumerator();

	//converting all numerators and denominators to Integers
	Integer* i1 = dynamic_cast<Integer*> (n1);
	Integer* i2 = dynamic_cast<Integer*> (d1);

	Fraction* simplified;

	if (i1 && i2) {

		long num = i1->getValue();
		long denom = i2->getValue();

		long gcd_ = this->gcd(num, denom);

		long newNum = (num / gcd_);
		long newDenom = (denom / gcd_);

		if (newDenom < 0){
			newNum = -newNum;
			newDenom = -newDenom;
		}

		if (newNum == newDenom){

			return new Integer(1);
		}

		else if (newNum != newDenom && newDenom == 1){

			return new Integer(newNum);
		}

		else {


			Integer* Numerator = new Integer(newNum);
			Integer* Denominator = new Integer(newDenom);

			simplified = new Fraction(Numerator, Denominator);

			return simplified;

		}

	}

	else if (d1->getType().compare("fraction") == 0){
		numerator = n1->multiply(((Fraction*)d1)->getDenominator());
		denominator = ((Fraction*)d1)->getNumerator();
		return this->simplify();
	}

	else if (n1->getType().compare("fraction") == 0){
		denominator = d1->multiply(((Fraction*)n1)->getDenominator());
		numerator = ((Fraction*)n1)->getNumerator();
		return this->simplify();
	}


	if (n1->getType().compare(d1->getType()) == 0) {
		if (n1->getType().compare("irrational") != 0){
			if (n1->getType().compare("exponential") == 0
				&& (((Exponential*)n1)->getBase()->getType().compare("irrational") != 0)
				|| ((Irrational*)(((Exponential*)n1)->getBase()))->getValue().compare(((Irrational*)(((Exponential*)d1)->getBase()))->getValue()) == 0)
				return n1->divide(d1);
			else{
				Number* f1 = new Fraction(n1->getCoefficient(), d1->getCoefficient());
				f1 = f1->simplify();
				Exponential* n1e = (Exponential*)n1;
				n1e = new Exponential(n1e->getBase(), n1e->getExponent(), new Integer(1));
				Exponential* d1e = (Exponential*)d1;
				d1e = new Exponential(d1e->getBase(), d1e->getExponent(), new Integer(1));
				if (f1->getType().compare("fraction") == 0)
					return new Fraction(n1e->multiply(((Fraction*)f1)->getNumerator()), d1e->multiply(((Fraction*)f1)->getDenominator()));
				else
					return new Fraction(n1e->multiply(f1), d1e);
			}
		}
		if (((Irrational*)n1)->getValue().compare(((Irrational*)d1)->getValue()) == 0)
			return n1->divide(d1);
		else{
			Number* f1 = new Fraction(n1->getCoefficient(), d1->getCoefficient());
			f1 = f1->simplify();
			if (f1->getType().compare("fraction") == 0)
				return new Fraction(new Irrational(((Irrational*)n1)->getValue(), ((Fraction*)f1)->getNumerator()), new Irrational(((Irrational*)d1)->getValue(), ((Fraction*)f1)->getDenominator()));
			else
				return new Fraction(new Irrational(((Irrational*)n1)->getValue(), f1), new Irrational(((Irrational*)d1)->getValue()));

		}
		return this;
	}

	else if (d1->getType().compare("exponential") == 0 && ((Exponential*)d1)->getExponent()->getType().compare("fraction") == 0){
		return this->rationalizeDenominator()->simplify();
	}

	else if (
		(d1->getType().compare("irrational") == 0
		&& n1->getType().compare("exponential") == 0 && ((Exponential*)n1)->getBase()->getType().compare("irrational") == 0
		&& ((Irrational*)(((Exponential*)n1)->getBase()))->getValue().compare(((Irrational*)d1)->getValue()) == 0)
		||
		(n1->getType().compare("irrational") == 0
		&& d1->getType().compare("exponential") == 0 && ((Exponential*)d1)->getBase()->getType().compare("irrational") == 0
		&& ((Irrational*)(((Exponential*)d1)->getBase()))->getValue().compare(((Irrational*)n1)->getValue()) == 0)
		){

		return n1->divide(d1);
	}

	else if (d1->equals(new Integer(1)))
		return numerator;

	numerator = n1;
	denominator = d1;

	Number* num = numerator;
	Number* den = denominator;
	while (num->getType().compare("integer") != 0 || den->getType().compare("integer") != 0){
		num = num->getCoefficient();
		den = den->getCoefficient();
	}
	Number* frac = new Fraction(num->copy(), den->copy());
	frac = frac->simplify();
	if (frac->getType().compare("fraction") == 0){
		if (((Integer*)(((Fraction*)frac)->getNumerator()))->getValue() == ((Integer*)num)->getValue())
			return this;
		numerator = numerator->divide(num)->multiply(((Fraction*)frac)->getNumerator());
		denominator = denominator->divide(den)->multiply(((Fraction*)frac)->getDenominator());
		return this->simplify();
	}
	else{
		numerator = numerator->divide(num)->multiply(frac);
		denominator = denominator->divide(den);

	if ((numerator->getType().compare("integer") == 0 

		|| (numerator->getType().compare("exponential") == 0
		&& ((Exponential*)numerator)->getBase()->getType().compare("irrational") == 0
		&& ((Exponential*)numerator)->getExponent()->getType().compare("fraction") == 0
		&& ((Fraction*)((Exponential*)numerator)->getExponent())->getNumerator()->getType().compare("integer") == 0
		&& ((Fraction*)((Exponential*)numerator)->getExponent())->getDenominator()->getType().compare("integer") == 0
		&& ((Integer*)((Fraction*)((Exponential*)numerator)->getExponent())->getNumerator())->getValue() < ((Integer*)((Fraction*)((Exponential*)numerator)->getExponent())->getDenominator())->getValue())

		&&

		(denominator->getType().compare("irrational") == 0
		||
		(denominator->getType().compare("exponential") == 0
		&& ((Exponential*)denominator)->getBase()->getType().compare("irrational") == 0)))
		
		|| (numerator->getType().compare("irrational") == 0)
		&& 
		
		((denominator->getType().compare("irrational") == 0
		&& ((Irrational*)denominator)->getValue().compare(((Irrational*)numerator)->getValue())==0)
		||
		(denominator->getType().compare("exponential") == 0
		&& ((Exponential*)denominator)->getBase()->getType().compare("irrational") == 0
		&& ((Irrational*)((Exponential*)denominator)->getBase())->getValue().compare(((Irrational*)numerator)->getValue()) == 0)))
		return this;
	else
		return this->simplify();
	}
	return this;
}

/*
New LCD method using lcd of a and b ---> lcd = ab/gcd(a,b);
*/
Number* Fraction::lcd(Fraction* other){

	Number* denom1 = other->getDenominator();
	Number* denom2 = this->getDenominator();

	Integer* i = dynamic_cast<Integer*>(denom1);
	Integer* ii = dynamic_cast<Integer*>(denom2);

	if (i && ii)  {

		long long1 = i->getValue();
		long long2 = ii->getValue();

		long factor = (long1*long2);
		long gcd_ = this->gcd(long1, long2);

		long lcm = factor / gcd_;

		Integer* thelcd = new Integer(lcm);

		return thelcd;
	}

}

Number* Fraction::subtract(Number* value){
	Fraction* f1 = dynamic_cast<Fraction*> (this);
	Fraction* f2 = dynamic_cast<Fraction*> (value);

	if (f1 && f2)   {

		Number* d1 = f1->getDenominator();
		Number* d2 = f2->getDenominator();
		Number* n1 = f1->getNumerator();
		Number* n2 = f2->getNumerator();

		//converting all numerators and denominators to Integers
		Integer* i1 = dynamic_cast<Integer*> (d1);
		Integer* i2 = dynamic_cast<Integer*> (d2);
		Integer* i3 = dynamic_cast<Integer*> (n1);
		Integer* i4 = dynamic_cast<Integer*> (n2);

		if (i1 && i2 && i3 && i4) {

			Number* lcm = f2->lcd(f1);

			Integer* lcdyo = dynamic_cast<Integer*> (lcm);

			long e1 = i1->getValue();
			long e2 = i2->getValue();
			long e3 = i3->getValue();
			long e4 = i4->getValue();
			long lcm_long = lcdyo->getValue();

			long factor1 = lcm_long / e1;
			long factor2 = lcm_long / e2;

			e1 *= factor1;
			e3 *= factor1;
			e2 *= factor2;
			e4 *= factor2;

			long difference = e3 - e4; //sum is the numerator which is over lcd
			Integer* newNumerator = new Integer(difference);
			Integer* newDenominator = new Integer(lcm_long);

			Fraction* answer = new Fraction(newNumerator, newDenominator);

			Number* simplifiedAnswer = answer->simplify();

			return simplifiedAnswer;
		}

		vector<Number*> v;
		v.push_back(this);
		Series* s = new Series(v);
		s->subtract(value);  //pushback -value? cuz subtract
		return s;
	}

	else if (this->getType().compare("fraction") == 0 && value->getType().compare("integer") == 0){
		Integer* subbedInteger = dynamic_cast<Integer*> (value);
		Fraction* subbingfrac = new Fraction(subbedInteger, new Integer(1));

		this->subtract(subbingfrac);
	}

	else {

		vector<Number*> v;
		v.push_back(this);
		Series* s = new Series(v);
		s->subtract(value);  //pushback -value? cuz subtract
		return s;

	}
}

Number* Fraction::add(Number* value){ //adding fractions

	Fraction* f1 = dynamic_cast<Fraction*> (value);
	Fraction* f2 = dynamic_cast<Fraction*> (this);

	if (f1 && f2)   {

		Number* d1 = f1->getDenominator();
		Number* d2 = f2->getDenominator();
		Number* n1 = f1->getNumerator();
		Number* n2 = f2->getNumerator();

		//converting all numerators and denominators to Integers
		Integer* i1 = dynamic_cast<Integer*> (d1);
		Integer* i2 = dynamic_cast<Integer*> (d2);
		Integer* i3 = dynamic_cast<Integer*> (n1);
		Integer* i4 = dynamic_cast<Integer*> (n2);

		if (i1 && i2 && i3 && i4) {

			Number* lcm = f2->lcd(f1);

			Integer* lcdyo = dynamic_cast<Integer*> (lcm);

			long e1 = i1->getValue();
			long e2 = i2->getValue();
			long e3 = i3->getValue();
			long e4 = i4->getValue();
			long lcm_long = lcdyo->getValue();

			long factor1 = lcm_long / e1;
			long factor2 = lcm_long / e2;

			e1 *= factor1;
			e3 *= factor1;
			e2 *= factor2;
			e4 *= factor2;

			long sum = e3 + e4; //sum is the numerator which is over lcd
			Integer* newNumerator = new Integer(sum);
			Integer* newDenominator = new Integer(lcm_long);

			Fraction* answer = new Fraction(newNumerator, newDenominator);

			Number* simplifiedAnswer = answer->simplify();

			return simplifiedAnswer;
		}
		else {
			vector<Number*> v;
			v.push_back(this);
			v.push_back(value);
			Series* s = new Series(v);
			return s;
		}
	}
	else if (this->getType().compare("fraction") == 0 && value->getType().compare("integer") == 0){

		Integer* addedInteger = dynamic_cast<Integer*> (value);
		Fraction* addingfrac = new Fraction(addedInteger, new Integer(1));

		this->add(addingfrac);
	}

	else{

		vector<Number*> v;
		v.push_back(this);
		v.push_back(value);
		Series* s = new Series(v);
		return s;
	}
}

Number* Fraction::multiply(Number* value){  //multiplying fractions

	Fraction* f1 = dynamic_cast<Fraction*> (this);
	Fraction* f2 = dynamic_cast<Fraction*> (value);

	if (f1 && f2) {

		Number* d1 = f1->getDenominator(); //this's denom 
		Number* d2 = f2->getDenominator(); //value's denom
		Number* n1 = f1->getNumerator(); // this num
		Number* n2 = f2->getNumerator();// values num

		//converting all numerators and denominators to Integers //dont even need this integer class handles
		Integer* i1 = dynamic_cast<Integer*> (d1);
		Integer* i2 = dynamic_cast<Integer*> (d2);
		Integer* i3 = dynamic_cast<Integer*> (n1);
		Integer* i4 = dynamic_cast<Integer*> (n2);

		Number* newNumerator;
		Number* newDenominator;

		if (i3 && i4) { //dont even need this integer class handles


			long e3 = i3->getValue(); //n1
			long e4 = i4->getValue(); //n2

			newNumerator = new Integer(e3*e4);

		}

		else{

			newNumerator = n2->multiply(n1);
		}

		if (i2 && i1) { //dont even need this integer class handles

			long e1 = i1->getValue(); //d1
			long e2 = i2->getValue(); //d2

			newDenominator = new Integer(e1*e2);

		}

		else {

			newDenominator = d2->multiply(d1);
		}

		Fraction* answer = new Fraction(newNumerator, newDenominator);

		Number* simplifiedAnswer = answer->simplify();

		return simplifiedAnswer;

	}

	else if (this->getType().compare("fraction") == 0 && value->getType().compare("integer") == 0){
		Integer* Integerbeingmultipledby = dynamic_cast<Integer*> (value);
		Fraction* convertedInteger = new Fraction(Integerbeingmultipledby, new Integer(1));
		this->multiply(convertedInteger);
	}

	else {
		Fraction* convertedInput = new Fraction(value, new Integer(1));

		return this->multiply(convertedInput);
	}


}

Number* Fraction::divide(Number* value){  //dividing fractions

	Fraction* f1 = dynamic_cast<Fraction*> (this);
	Fraction* f2 = dynamic_cast<Fraction*> (value);

	if (f1 && f2) {

		Number* d1 = f1->getDenominator();
		Number* d2 = f2->getDenominator();
		Number* n1 = f1->getNumerator();
		Number* n2 = f2->getNumerator();

		//converting all numerators and denominators to Integers //dont even need this integer class handles
		Integer* i1 = dynamic_cast<Integer*> (d1);
		Integer* i2 = dynamic_cast<Integer*> (d2);
		Integer* i3 = dynamic_cast<Integer*> (n1);
		Integer* i4 = dynamic_cast<Integer*> (n2);

		Number* newNumerator;
		Number* newDenominator;

		if (i3 && i2) { //dont even need this integer class handles
			long e3 = i3->getValue(); //n1
			long e4 = i2->getValue(); //d2

			newNumerator = new Integer(e3*e4);
		}

		else{

			newNumerator = n1->multiply(d2);
		}

		if (i1 && i4) { //dont even need this integer class handles

			long e1 = i1->getValue(); //d1
			long e2 = i4->getValue(); //n2

			newDenominator = new Integer(e1*e2);

		}

		else {

			newDenominator = n2->multiply(d1);
		}

		Fraction* answer = new Fraction(newNumerator, newDenominator);

		Number* simplifiedAnswer = answer->simplify();

		return simplifiedAnswer;

	}

	else if (this->getType().compare("fraction") == 0 && value->getType().compare("integer") == 0){
		Integer* Integerbeingdividedby = dynamic_cast<Integer*> (value);
		Fraction* convertedInteger = new Fraction(Integerbeingdividedby, new Integer(1));
		this->divide(convertedInteger);
	}

	else {
		Fraction* convertedInput = new Fraction(value, new Integer(1));

		return this->divide(convertedInput);
	}
}

bool Fraction::isPrime(long value){ //Don't really need.
	long i;
	if (value <= 1) {
		return false;
	}
	else if (value == 2) {
		return true;
	}
	for (i = 2; i < (sqrt(value) + 1); i++) {
		if (value % i == 0) {
			return false;
		}
	}
	return true;
}

vector<long> Fraction::primeFactorization(long value, vector<long> primes, long lowestLeft)
{
	if (value == 1)
		return primes;
	while (true)
	{
		if (value%lowestLeft == 0)
		{
			primes.push_back(lowestLeft);

			return primeFactorization(value / lowestLeft, primes, lowestLeft);
		}
		lowestLeft++;
	}
}

vector<long> Fraction::primeFactorization(long value) {

	vector<long> primes;
	return primeFactorization(value, primes, 2);
}

string Fraction::getObjectType()
{
	return "number";
}