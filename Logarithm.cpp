#include "Logarithm.h"
#include"Integer.h"
#include"Series.h"
#include"Fraction.h"
#include"Number.h"
#include"Irrational.h"
#include<stdexcept>
#include<cmath>
using namespace std;
Logarithm::Logarithm(Number* base, Number* x, Number* coefficient)
{
	this->base = base;
	this->x = x;
	this->coefficient = coefficient;
}

Logarithm::~Logarithm()
{
	delete base, x, coefficient;
}

Number* Logarithm::copy(){
	return new Logarithm(base->copy(), x->copy(), coefficient->copy());
}

Number* Logarithm::simplify()
{
	base = base->simplify();
	coefficient = coefficient->simplify();
	x = x->simplify();
	if (x->equals(base)){
		return coefficient;
	}
	if (x->getType().compare("exponential") == 0){
		Number* n = new Logarithm(base, ((Exponential*)x)->getBase(), ((Exponential*)x)->getExponent()->multiply(coefficient));
//		coefficient = coefficient->multiply(((Exponential*)x)->getExponent());
//		x = ((Exponential*)x)->getBase();
		return n->simplify();
	}
	if (base->getType().compare("exponential") == 0){
		Number* n = new Logarithm(((Exponential*)base)->getBase(), x, coefficient);
		n = n->divide(((Exponential*)base)->getExponent());
		return n->simplify();
	}
	if (x->getType().compare("irrational") == 0 && !x->getCoefficient()->equals(new Integer(1))){
		Number* c = x->getCoefficient();
		Number* r = new Irrational(((Irrational*)x)->getValue());
		Number* l1 = new Logarithm(base, c, coefficient);
		Number* l2 = new Logarithm(base, r, coefficient);
		return l1->simplify()->add(l2->simplify());
	}
	if (x->getType().compare("fraction") == 0){
		bool changed = false;
		if (base->getType().compare("fraction") == 0){

			Number* nx = ((Fraction*)x)->getNumerator();
			Number* dx = ((Fraction*)x)->getDenominator();;
			Number* nb = ((Fraction*)base)->getNumerator();
			Number* db = ((Fraction*)base)->getDenominator();

			if (nx->getType().compare("integer") == 0 && dx->getType().compare("integer") == 0 && nb->getType().compare("integer") == 0 && db->getType().compare("integer") == 0){


				long c = 0;
				double nxi = ((Integer*)nx)->getValue();
				double dxi = ((Integer*)dx)->getValue();
				double nbi = ((Integer*)nb)->getValue();
				double dbi = ((Integer*)db)->getValue();
				if (nbi / dbi < 0 || nbi == dbi)
					throw invalid_argument("Base of logarithm must be greater than zero but not equal to one");
				if (((nbi > 1 && nbi > nxi) || (nbi < 1 && nbi < nxi)) && ((dbi > 1 && dbi > dxi) || (dbi < 1 && dbi < dxi))){
					Number* n1 = new Integer((long)nbi);
					Number* n2 = new Integer((long)nxi);
					Number* n3 = new Integer((long)dbi);
					Number* n4 = new Integer((long)dxi);
					Number* log = new Logarithm(new Fraction(n2, n4), new Fraction(n1, n3), new Integer(1));
					log = log->simplify();
					if (log->simplify()->getType().compare("integer") == 0){
						log = log->simplify();
						Fraction* f = new Fraction(coefficient, log);
						return f->simplify();
					}
				}

				while (floor(nxi / nbi) == nxi / nbi && floor(dxi / dbi) == dxi / dbi)
				{
					nxi /= nbi;
					dxi /= dbi;
					c++;
				}
				if (c != 0){
					Number* coeffnew = coefficient->multiply(new Integer(c));
					x = new Fraction(new Integer(nxi), new Integer(dxi));
					Number* s = new Series();
					s->add(this);
					s->add(coeffnew);
					return s->simplify();
				}




			}
		}
		else{
			Logarithm* l1 = new Logarithm(base, ((Fraction*)x)->getNumerator(), coefficient);
			Logarithm* l2 = new Logarithm(base, ((Fraction*)x)->getDenominator(), coefficient);
			return l1->simplify()->subtract(l2->simplify());

		}
	}
	if (x->getType().compare("integer") == 0){
		double x2 = ((Integer*)x)->getValue();
		if (x2 <= 0)
			throw invalid_argument("Cannot take logarithm of zero or a negative value");
		if (base->getType().compare("integer") == 0){
			if (((Integer*)x)->getValue() == 1)
				return new Integer((long)0);
			long c = 0;
			double b = ((Integer*)base)->getValue();
			if (b <= 0 || b == 1)
				throw invalid_argument("Base of logarithm must be greater than zero but not equal to one");
			if ((b > 1 && b > x2) || (b < 1 && b < x2)){
				Number* n1 = new Integer((long)b);
				Number* n2 = new Integer((long)x2);
				Number* log = new Logarithm(n2, n1, new Integer(1));
				log = log->simplify();
				if (log->simplify()->getType().compare("integer") == 0){
					log = log->simplify();
					Fraction* f = new Fraction(coefficient, log);
					return f->simplify();
				}
			}

			while (floor(x2 / b) == x2 / b)
			{
				x2 /= b;
				c++;
			}
			if (c != 0){
				Number* coeffnew = coefficient->multiply(new Integer(c));
				x = new Integer(x2);
				Number* s = new Series();
				s->add(this->simplify());
				s->add(coeffnew);
				return s->simplify();
			}
		}
		if (((Integer*)x)->getValue() == 1)
			return new Integer((long)0);
		if (((Integer*)x)->getValue() <= 0)
			throw invalid_argument("Logarithm must be of a value greater than 0");
		vector<long> list = primeFactorization(((Integer*)x)->getValue());
		if (list.size() == 1)
			return this;
		Series* s = new Series();
		for (int i = 0; i < list.size(); i++)
			s = (Series*)s->add(new Logarithm(base, new Integer(list.at(i)), coefficient));
		return s->simplify();
	}
	return this;
}

Number* Logarithm::add(Number* value)
{
	if (value->getType().compare(getType()) == 0 && ((Logarithm*)value)->base->equals(this->base)) {
		if (((Logarithm*)value)->x->equals(this->x))
			return new Logarithm(this->base, this->x, coefficient->add(value->getCoefficient()));
		else if(((Logarithm*)value)->x->getType().compare(x->getType())==0) {
			Number* e = new Exponential(x, coefficient, new Integer(1));
			Number* e1 = new Exponential(((Logarithm*)value)->x, value->getCoefficient(), new Integer(1));
			e = e->simplify();
			e1 = e1->simplify();
			return new Logarithm(this->base, e->multiply(e1)->simplify(), new Integer(1));
		}
	}
	if (value->getType().compare("series") == 0)
		return value->add(this);
	else
	{
		Series* s = new Series();
		return s->add(this)->add(value);
	}
}

Number* Logarithm::subtract(Number* value)
{
	return add(value->multiply(new Integer(-1)));
}

Number* Logarithm::multiply(Number* value)
{
	if (value->getType().compare(this->getType()) == 0 &&
		((Logarithm*)value)->base->equals(this->base) &&
		((Logarithm*)value)->x->equals(this->x))
	{
		return new Exponential(this, new Integer(2), coefficient->multiply(value->getCoefficient()));
	}

	if (value->getType().compare("exponent") == 0 &&
		this->equals(((Exponential*)value)->getBase()))
		return new Exponential(this, ((Exponential*)value)->getExponent()->add(new Integer(1)), coefficient->multiply(value->getCoefficient()));

	if (value->getType().compare("series") == 0)
		return value->add(this);

	return new Logarithm(base, x, coefficient->multiply(value));
}

Number* Logarithm::divide(Number* value)
{
	if (value->equals(new Integer((long)0)))
		throw invalid_argument("Silly Rabbit, Trix are for kids not dividing by zero!");
	if (value->getType().compare("logarithm") == 0 && ((Logarithm*)value)->base->equals(base))
		return new Logarithm(((Logarithm*)value)->x, x, coefficient->divide(((Logarithm*)value)->coefficient));

	return new Fraction(this, value);
}

vector<long> Logarithm::primeFactorization(long value)
{
	vector<long> primes;
	return primeFactorization(value, primes, 2);
}

vector<long> Logarithm::primeFactorization(long value, vector<long> primes, long lowestLeft)
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

string Logarithm::getType()
{
	return "logarithm";
}

bool Logarithm::equals(Number* value)
{
	if (getType().compare(value->getType()) != 0)
		return false;
	return ((Logarithm*)value)->base->equals(base) && ((Logarithm*)value)->x->equals(x) && ((Logarithm*)value)->coefficient->equals(coefficient);
}

Number* Logarithm::getCoefficient()
{
	return coefficient;
}

Number* Logarithm::getBase()
{
	return base;
}

Number* Logarithm::getX()
{
	return x;
}

string Logarithm::toString()
{
	Number* n = this->simplify();
	string bstr, cstr, xstr;
	if (n->getType().compare("logarithm") != 0)
		return n->toString();
	Number* nb = ((Logarithm*)n)->base;
	Number* nc = ((Logarithm*)n)->coefficient;
	Number* nx = ((Logarithm*)n)->x;
	bstr = nb->toString();
	cstr = nc->toString();
	xstr = nx->toString();
	if (nb->getType().compare("series") == 0
		|| nb->getType().compare("fraction") == 0
		|| (nb->getType().compare("integer") != 0 && !nb->getCoefficient()->equals(new Integer(1)))
		|| nb->getType().compare("exponential") == 0)
		bstr = "(" + bstr + ")";
	if (nx->getType().compare("series") == 0
		|| nx->getType().compare("fraction") == 0
		|| (nx->getType().compare("integer") != 0 && !nx->getCoefficient()->equals(new Integer(1)))
		|| nx->getType().compare("exponential") == 0)
		xstr = "(" + xstr + ")";
	if (nc->getType().compare("series") == 0
		|| nc->getType().compare("fraction") == 0
		|| (nc->getType().compare("integer") != 0 && !nc->getCoefficient()->equals(new Integer(1)))
		|| nc->getType().compare("exponential") == 0)
		cstr = "(" + cstr + ")";
	if (nc->equals(new Integer(1)))
		return "log_" + bstr + ":" + xstr;
	return cstr + " * " + "log_" + bstr + ":" + xstr;
}

string Logarithm::getObjectType()
{
	return "number";
}
