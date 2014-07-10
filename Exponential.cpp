#include<vector>
#include"Exponential.h"
#include"Integer.h"
#include"Irrational.h"
#include"Number.h"
#include"Series.h"
#include "Logarithm.h"
#include "Fraction.h"
#include <cmath>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <climits>
using namespace std;

static const long MAXIMUM = pow(2, 100);

//constructor
Exponential::Exponential(Number* b, Number* e, Number* c)
{
	this->base = b;
	this->exponent = e;
	this->coefficient = c;
}

//destructor
Exponential::~Exponential()
{

}

Number* Exponential::copy(){
	return new Exponential(base->copy(), exponent->copy(), coefficient->copy());
}

//accessor methods
Number* Exponential::getBase()
{
	return base;
}

Number* Exponential::getExponent()
{
	return exponent;
}

Number* Exponential::getCoefficient()
{
	return coefficient;
}

string Exponential::getSign(Number* value){
	if (value->getType().compare("integer") == 0){
		if (((Integer*)value)->getValue() > 0)
			return " + ";
		else
			return " - ";
	}

	return getSign(value->getCoefficient());
}

//method's purpose is to return a number that simplifies the exponential this
Number* Exponential::simplify()
{
	//Begin additional  part for root

	coefficient = coefficient->simplify();
	base = base->simplify();
	exponent = exponent->simplify();

	if (getSign(base).compare(" - ") == 0){
		if (exponent->getType().compare("fraction") == 0){
			Fraction* f = (Fraction*)exponent;
			if (f->getDenominator()->getType().compare("integer") == 0){
				Integer* i = (Integer*)(f->getDenominator());
				if (i->getValue() % 2 == 0)
					throw invalid_argument("Can't take even root of negative number. Imaginary numbers are not supported)");
				else{
					coefficient = coefficient->multiply(new Integer(-1))->simplify();
					base = base->multiply(new Integer(-1));
				}
			}
			else
				throw invalid_argument("Can't take irrational root of negative number. Imaginary numbers are not supported)");
		}
	}

	if (base->equals(new Integer(1))) return coefficient;

	if (exponent->equals(new Integer((long)0))){
		if (base->equals(new Integer((long)0)))
			throw invalid_argument("Error: O cannot be both a base and power.");
		else
			return coefficient;
	}
	if (base->equals(new Integer((long)0)))
		return new Integer((long)0);

	if (base->getType().compare("irrational") == 0 && getSign(exponent).compare(" - ") == 0){
		return new Fraction(new Integer(1), new Exponential(base, exponent->multiply(new Integer(-1)), coefficient));
	}
	if (exponent->getType().compare("series") == 0){
		Number* n = coefficient;
		vector<Number*> expo = ((Series*)exponent)->getValues();
		for (int i = 0; i < expo.size(); i++)
			n = n->multiply(new Exponential(base, expo[i], new Integer(1)))->simplify();
		return n;
	}

	if (base->getType().compare("series") == 0){
		if (exponent->getType().compare("integer") == 0){
			Number* n = new Integer(1);
			for (int i = 0; i < ((Integer*)exponent)->getValue(); i++)
				n = n->multiply(base->copy())->simplify();
			return n;
		}
		return this;
	}
	if (base->getType().compare("exponential") == 0){
		Exponential* exb = ((Exponential*)base);
		Number* temp = new Exponential(exb->coefficient, exponent, new Integer(1));
		temp = temp->simplify();
		coefficient = getCoefficient()->multiply(temp);
		exponent = exb->getExponent()->multiply(exponent)->simplify();
		base = ((Exponential*)base)->getBase()->simplify();
		return this->simplify();
	}

	if (base->getType().compare("integer") == 0){
		if (exponent->getType().compare("fraction") == 0){
			Number* newExp = ((Fraction*)exponent)->getNumerator();
			if (getSign(newExp).compare(" - ") == 0){
				exponent = exponent->multiply(new Integer(-1));
				return (new Fraction(coefficient, new Exponential(base, exponent, new Integer(1))))->simplify();
			}
			if (newExp->getType().compare("integer") == 0){
				base = new Exponential(base, newExp, new Integer(1));
				base = base->simplify();
				newExp = new Integer(1);
			}

			Number* root = ((Fraction*)exponent)->getDenominator();
			if (root->getType().compare("integer") == 0){
				long b = ((Integer*)base)->getValue();
				long r = ((Integer*)root)->getValue();
				long c = 1;
				long i = 0;
				vector<long> list = primeFactorization(b);
				long n = list[0];
				for (int j = 0; j < list.size(); j++){
					if (list[j] == n){
						i++;
					}
					else{
						if (i >= r) {
							b /= pow(n, i - (i % r));
							c *= pow(n, (i / r));
						}
						i = 1;
						n = list[j];
					}
				}
				if (i >= r){
					b /= pow(n, i - (i % r));
					c *= pow(n, (i / r));
				}
				coefficient = new Exponential(new Integer(c), newExp, coefficient);
				coefficient = coefficient->simplify();
				base = new Integer(b);
				exponent = new Fraction(newExp, ((Fraction*)exponent)->getDenominator());
			}
		}
		if (base->equals(new Integer(1)))
			return coefficient->simplify();
	}
	if (exponent->getType().compare("integer") == 0 && exponent->equals(new Integer(1)))
		return base->multiply(coefficient)->simplify();


	Exponential* e1 = dynamic_cast<Exponential*> (this);
	Number* bs = e1->getBase();
	Number* expo = e1->getExponent();
	Number* coeff = e1->getCoefficient();

	if (expo->getType().compare("integer") == 0 && ((Integer*)expo)->getValue() == 0 &&
		bs->getType().compare("integer") == 0 && ((Integer*)bs)->getValue() == 0)
	{
		throw invalid_argument("Error: O cannot be both a base and power.");
	}
	else if (expo->getType().compare("integer") == 0 && ((Integer*)expo)->getValue() == 0)
	{
		return coeff->simplify();
	}
	//in the case of the base, power, and coefficient being irrational the only way in
	//which the exponent will change is if the coefficient and base is the same raising
	//the power of the exponent by 1
	else if (bs->getType().compare("irrational") == 0 && !(bs->getCoefficient()->equals(new Integer(1)))){
		Number* result = (new Exponential(base->getCoefficient(), exponent, coefficient))->simplify();
		result = (new Exponential(new Irrational(((Irrational*)base)->getValue()), exponent, result))->simplify();
		return result;
	}
	else if (bs->getType().compare("irrational") == 0 && expo->getType().compare("irrational") == 0 && coeff->getType().compare("irrational") == 0)
	{
		if (((Irrational*)coeff)->getValue().compare(((Irrational*)bs)->getValue()) == 0)
		{
			Exponential* answer1 = new Exponential(bs, expo->add(new Integer(1)), coeff);
			return answer1;
		}
		else
		{
			Exponential* answer1 = new Exponential(bs, expo, coeff);
			return answer1;
		}
	}

	else if (bs->getType().compare("irrational") == 0 && expo->getType().compare("integer") == 0)
	{
		Number* c = (new Exponential(bs->getCoefficient(), expo, new Integer(1)))->simplify();
		return new Exponential(new Irrational(((Irrational*)bs)->getValue()), expo, coeff->multiply(c));
		if (((Irrational*)coeff)->getValue().compare(((Irrational*)bs)->getValue()) == 0)
		{
			Exponential* answer1 = new Exponential(bs, expo->add(new Integer(1)), coeff);
			return answer1;
		}
		else
		{
			Exponential* answer1 = new Exponential(bs, expo, coeff);
			return answer1;
		}
	}
	//in the case of the base, power, and coefficient being logarithmic the only way in
	//which the exponent will change is if the coefficient and base is the same raising
	//the power of the exponent by 1
	else if (bs->getType().compare("logarithm") == 0 && coeff->getType().compare("logarithm") == 0)
	{
		if (((Logarithm*)coefficient)->equals(((Logarithm*)bs)))
		{

			Exponential* answer1 = new Exponential(bs, expo->add(new Integer(1)), coeff);
			return answer1;

		}
		else
		{
			Exponential* answer1 = new Exponential(bs, expo, coeff);
			return answer1;
		}
	}

	else if (expo->getType().compare("logarithm") == 0){
		Logarithm* l = ((Logarithm*)expo);
		if (l->getBase()->equals(base))
			return coefficient->multiply(l->getX())->simplify();
		if (l->getBase()->getType().compare("exponential") == 0){
			Exponential* b = ((Exponential*)(l->getBase()));
			if (b->getBase()->equals(base))
				return coefficient->multiply((new Exponential(l->getX(), new Fraction(new Integer(1), b->getExponent()), new Integer(1)))->simplify())->simplify();

		}
		return this;
	}

	// when everything is an ineger it'll evaluate to an integer
	else if (bs->getType().compare("integer") == 0 && expo->getType().compare("integer") == 0 && coeff->getType().compare("integer") == 0)
	{
		if (((Integer*)expo)->getValue() > 0)
		{
			Integer* answer1 = new Integer(pow(((Integer*)bs)->getValue(), ((Integer*)expo)->getValue()) * ((Integer*)coeff)->getValue());
			//			cout << answer1->getValue() << endl << ULONG_MAX << endl << LLONG_MAX << endl;
			if (answer1->getValue() == MAXIMUM)
				throw invalid_argument("Overflow, sorry, your numbers are too big to manage. Are you trying to compensate for something?");
			return answer1;
		}
		else
		{
			Integer* answer1 = new Integer(pow(((Integer*)bs)->getValue(), -((Integer*)expo)->getValue()));
			Fraction* answer2 = new Fraction(coeff, answer1);
			return answer2;
		}
	}

	
	else if (bs->getType().compare("fraction") == 0)
	{
		Number* e1 = (new Exponential(((Fraction*)bs)->getNumerator(), expo, coefficient))->simplify();
		Number* e2 = (new Exponential(((Fraction*)bs)->getDenominator(), expo, new Integer(1)))->simplify();
		return (new Fraction(e1, e2));
		

	}
	else
	{
		Exponential* answer1 = new Exponential(bs, expo, coeff);
		return answer1;
	}
}

string Exponential::toString()
{
	//	ostringstream printStatement;
	Number* starting = this->simplify();

	Exponential* printExp;
	Number* baseExp;
	Number* exponExp;
	Number* coeffExp;

	if (starting->getType().compare("exponential") == 0)
	{
		printExp = dynamic_cast<Exponential*>(starting);
		baseExp = printExp->getBase();
		exponExp = printExp->getExponent();
		coeffExp = printExp->getCoefficient();

		string b = baseExp->toString(), e = exponExp->toString(), c = coeffExp->toString(), str = "";
		if (baseExp->getType().compare("series") == 0 || baseExp->getType().compare("logarithm") == 0
			|| baseExp->getType().compare("integer") != 0 && baseExp->getType().compare("fraction") != 0 && !baseExp->getCoefficient()->equals(new Integer(1)))
			b = "(" + b + ")";
		if (coeffExp->getType().compare("series") == 0 || coeffExp->getType().compare("logarithm") == 0
			|| coeffExp->getType().compare("integer") != 0 && coeffExp->getType().compare("fraction") != 0 && !coeffExp->getCoefficient()->equals(new Integer(1)))
			c = "(" + c + ")";
		if (exponExp->getType().compare("series") == 0 || exponExp->getType().compare("logarithm") == 0
			|| exponExp->getType().compare("integer") != 0 && exponExp->getType().compare("fraction") != 0 && !exponExp->getCoefficient()->equals(new Integer(1)))
			e = "(" + e + ")";

		if (exponExp->getType().compare("fraction") == 0){
			if (((Fraction*)exponExp)->getDenominator()->equals(new Integer(2)))
				str += "sqrt:" + b;
			else
				str += ((Fraction*)exponExp)->getDenominator()->toString() + "rt:" + b;

			if (!((Fraction*)exponExp)->getNumerator()->equals(new Integer(1)))
				str = "(" + str + ")^" + ((Fraction*)exponExp)->getNumerator()->toString();

		}
		else{
			str += b + "^" + e;
		}
		if (coeffExp->getType().compare("integer") != 0 || (((Integer*)coeffExp)->getValue() != 1 && ((Integer*)coeffExp)->getValue() != -1))
			str = c + "*" + str;
		if (((Integer*)coeffExp)->getValue() == -1)
			str = "-(" + str + ")";
		return str;
	}
	else
	{
		return starting->toString();
	}

}

string Exponential::getType()
{
	return "exponential";
}

Number* Exponential::add(Number* value)
{

	value = value->simplify();

	if (value->getType().compare("exponential") == 0)
	{
		Exponential* e1 = dynamic_cast<Exponential*>(this);
		Exponential* e2 = dynamic_cast<Exponential*>(value);
		if ((e1->getBase()->equals(e2->getBase())) && (e1->getExponent()->equals(e2->getExponent())))
		{
			Number* coeff1 = e1->getCoefficient();
			Number* coeff2 = e2->getCoefficient();

			Number* coeffFinal = coeff1->add(coeff2);

			Exponential* answer = new Exponential(e2->getBase(), e2->getExponent(), coeffFinal);
			return answer->simplify();
		}
		else if (e1->getBase()->getType().compare("integer") == 0 && e1->getExponent()->getType().compare("integer") == 0 && e1->getCoefficient()->getType().compare("integer") == 0
			&& e2->getBase()->getType().compare("integer") == 0 && e2->getExponent()->getType().compare("integer") == 0 && e2->getCoefficient()->getType().compare("integer") == 0)
		{
			Integer* answInt1 = new Integer(e1->simplify());
			Integer* answInt2 = new Integer(e2->simplify());
			Integer* finalAnsw = (Integer*)(answInt1->add(answInt2));
			return finalAnsw->simplify();
		}
		else if (e1->getBase()->getType().compare("irrational") == 0 && e2->getBase()->getType().compare("irrational") == 0)
		{
			if (((Irrational*)(e1->getBase()))->getValue().compare(((Irrational*)(e2->getBase()))->getValue()) != 0 || !e1->getExponent()->equals(e2->getExponent()))
			{
				Number* s = new Series();
				s->add(e1);
				s->add(e2);
				return s;
			}
			Irrational* irE1B = (Irrational*)e1->getBase();
			Irrational* irE2B = (Irrational*)e2->getBase();
			Number* coeff1 = e1->getCoefficient();
			Number* coeff2 = e2->getCoefficient();
			if (irE1B->getValue() == irE2B->getValue())
			{
				if (e1->getExponent()->getType().compare("irrational") == 0 && e2->getExponent()->getType().compare("irrational"))
				{
					Irrational* irE1E = (Irrational*)e1->getExponent();
					Irrational* irE2E = (Irrational*)e2->getExponent();
					if (irE1E->getValue() == irE2E->getValue())
					{
						Number* coeffFinal = coeff1->add(coeff2);
						Exponential* answer = new Exponential(e2->getBase(), e2->getExponent(), coeffFinal);
						return answer->simplify();
					}

				}
				else
				{
					if (e1->getExponent()->equals(e2->getExponent()))
					{
						Number* coeffFinal = coeff1->add(coeff2);
						Exponential* answer = new Exponential(e2->getBase(), e2->getExponent(), coeffFinal);
						return answer->simplify();
					}
				}
			}
		}
		vector<Number*> v;
		v.push_back(this);
		v.push_back(value);
		Series* s = new Series(v);
		return s;
	}


	return value->add(this)->simplify();

}

Number* Exponential::subtract(Number* value)
{
	return add(value->multiply(new Integer(-1)));
}
Number* Exponential::multiply(Number* value)
{
	value = value->simplify();

	if (value->getType().compare("exponential") == 0)
	{

		Exponential* e1 = dynamic_cast<Exponential*>(this);
		Exponential* e2 = dynamic_cast<Exponential*>(value);
		if (e1->getBase()->getType().compare("integer") == 0 && e1->getExponent()->getType().compare("integer") == 0 && e1->getCoefficient()->getType().compare("integer") == 0
			&& e2->getBase()->getType().compare("integer") == 0 && e2->getExponent()->getType().compare("integer") == 0 && e2->getCoefficient()->getType().compare("integer") == 0)
		{
			Integer* answInt1 = new Integer(e1->simplify());
			Integer* answInt2 = new Integer(e2->simplify());
			Integer* finalAnsw = (Integer*)(answInt1->multiply(answInt2));
			return finalAnsw;
		}
		else if ((e1->getBase())->getType().compare("irrational") == 0 && (e2->getBase())->getType().compare("irrational") == 0)
		{
			if (((Irrational*)(e1->getBase()))->getValue().compare(((Irrational*)(e2->getBase()))->getValue()) == 0){
				Irrational* irE1B = (Irrational*)e1->getBase();
				Irrational* irE2B = (Irrational*)e2->getBase();
				Number* coeff1 = e1->getCoefficient();
				Number* coeff2 = e2->getCoefficient();
				Number* exp1 = e1->getExponent();
				Number* expp = e2->getExponent();
				if (irE1B->getValue() == irE2B->getValue())
				{
					Number* newExp = exp1->add(expp);
					if (newExp->getType().compare("series") == 0){
						coefficient = coefficient->multiply(value);
						return this;
					}
					Number* newCoeff = coeff1->multiply(coeff2);
					Exponential* answer = new Exponential(e1->getBase(), newExp, newCoeff);
					return answer;
				}
			}
			else{
				coefficient = coefficient->multiply(value);
				return this;
			}
		}

		else if (e1->getBase()->equals(e2->getBase()))
		{
			Number* exponent1 = e1->getExponent();
			Number* exponent2 = e2->getExponent();
			if ((exponent1->getType().compare("integer") == 0 && exponent2->getType().compare("integer") == 0) ||
				(exponent1->getType().compare("irrational") == 0 && exponent2->getType().compare("irrational") == 0) && ((Irrational*)exponent1)->getValue().compare(((Irrational*)exponent1)->getValue()) == 0) {
				Number* coefficient1 = e1->getCoefficient();
				Number* coefficient2 = e2->getCoefficient();
				Number* newExp = exponent1->add(exponent2);
				if (newExp->getType().compare("series") == 0){
					coefficient = coefficient->multiply(value);
					return this;
				}
				Number* newCoeff = coefficient1->multiply(coefficient2);
				Exponential* answer = new Exponential(e1->getBase(), newExp, newCoeff);
				return answer->simplify();
			}
			else{
				coefficient = coefficient->multiply(value);
				return this;
			}
		}

		else if (e1->getExponent()->equals(e2->getExponent()))
		{

			Number* base1 = e1->getBase();
			Number* base2 = e2->getBase();
			Number* coefficient1 = e1->getCoefficient();
			Number* coefficient2 = e2->getCoefficient();
			Number* newBase = base1->multiply(base2)->simplify();
			Number* newCoeff = coefficient1->multiply(coefficient2)->simplify();
			Exponential* answer = new Exponential(newBase, e1->getExponent(), newCoeff);
			return answer->simplify();
		}
	}

	else if (value->getType().compare("integer") == 0 && ((this->getCoefficient())->getType()).compare("integer") == 0)
	{
		Integer* theInteger = dynamic_cast<Integer*>(value);

		Integer* thecoeff = dynamic_cast<Integer*>(this->getCoefficient());


		Number* newcoefff = theInteger->multiply(thecoeff);

		return new Exponential(this->getBase(), this->getExponent(), newcoefff);
	}

	else
	{
		return value->multiply(this);
	}

	coefficient = coefficient->multiply(value);
	return this;

}

Number* Exponential::divide(Number* value)
{
	value = value->simplify();

	Number* n = this->simplify();
	if (n->getType().compare("exponential") != 0)
		return n->divide(value)->simplify();
	if (value->getType().compare("exponential") == 0 && n->getType().compare("exponential") == 0)
	{
		Exponential* e1 = dynamic_cast<Exponential*>(n);
		Exponential* e2 = dynamic_cast<Exponential*>(value);
		if (e2->simplify() == 0)
		{
			throw invalid_argument("Error: O cannot be a denominator.");
		}
		else if (e1->getBase()->getType().compare("integer") == 0 && e1->getExponent()->getType().compare("integer") == 0 && e1->getCoefficient()->getType().compare("integer") == 0
			&& e2->getBase()->getType().compare("integer") == 0 && e2->getExponent()->getType().compare("integer") == 0 && e2->getCoefficient()->getType().compare("integer") == 0)
		{
			Integer* answInt1 = new Integer(e1->simplify());
			Integer* answInt2 = new Integer(e2->simplify());
			Integer* finalAnsw = (Integer*)(answInt1->divide(answInt2));
			return finalAnsw->simplify();
		}
		else if (e1->getBase()->getType().compare("irrational") == 0 && e2->getBase()->getType().compare("irrational"))
		{
			Irrational* irE1B = (Irrational*)e1->getBase();
			Irrational* irE2B = (Irrational*)e2->getBase();
			Number* coeff1 = e1->getCoefficient();
			Number* coeff2 = e2->getCoefficient();
			Number* exp1 = e1->getExponent();
			Number* exp2 = e2->getExponent();
			if (irE1B->getValue().compare(irE2B->getValue()) == 0)
			{
				Number* newExp = exp1->subtract(exp2);
				if (newExp->getType().compare("series") == 0){
					return (new Fraction(this, value));
				}
				Number* newCoeff = coeff1->divide(coeff2);
				Exponential* answer = new Exponential(e1->getBase(), newExp, newCoeff);
				return answer->simplify();
			}
		}
		else if (e1->getBase()->equals(e2->getBase()))
		{
			Number* exponent1 = e1->getExponent();
			Number* exponent2 = e2->getExponent();
			Number* coefficient1 = e1->getCoefficient();
			Number* coefficient2 = e2->getCoefficient();
			Number* newExp = exponent1->subtract(exponent2);
			Number* newCoeff = coefficient1->divide(coefficient2);
			if (newExp->getType().compare("series") == 0){
				return (new Fraction(this, value));
			}
			Exponential* answer = new Exponential(e1->getBase(), newExp, newCoeff);
			return answer->simplify();
		}

	}

	else if (value->equals(base)){
		if (exponent->getType().compare("fraction") == 0){
			Integer* n1 = dynamic_cast<Integer*>(((Fraction*)exponent)->getNumerator());
			Integer* d1 = dynamic_cast<Integer*>(((Fraction*)exponent)->getDenominator());
			if ((n1 && d1) && n1->getValue() > d1->getValue()){
				exponent = exponent->subtract(new Integer(1));
				return this->simplify();
			}
		}
		else{
			exponent = exponent->subtract(new Integer(1));
			return this->simplify();
		}
	}



	coefficient = coefficient->divide(value)->simplify();
	if (coefficient->getType().compare("fraction") != 0)
		return this->simplify();
	else{
		Fraction* c = (Fraction*)coefficient;
		Number* den = c->getDenominator();
		coefficient = c->getNumerator();
		return new Fraction(this, den);
	}
	coefficient = coefficient->divide(value);
	return this->simplify();

}

bool Exponential::equals(Number* value)
{
	if (this->getType().compare(value->getType()) != 0)
	{
		return false;
	}
	Exponential* e = ((Exponential*)value);
	return base->equals(e->base) && exponent->equals(e->exponent) && coefficient->equals(e->coefficient);
}

string Exponential::getObjectType()
{
	return "number";
}

vector<long> Exponential::primeFactorization(long value, vector<long> primes, long lowestLeft)
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

vector<long> Exponential::primeFactorization(long value)
{
	vector<long> primes;
	return primeFactorization(value, primes, 2);
}