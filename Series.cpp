#include "Number.h" 
#include <vector> 
#include "Series.h"
#include "Integer.h"
#include "Logarithm.h"
#include "Irrational.h"
#include <sstream>
#include<string>
#include<stdexcept>
using namespace std;

Series::Series()
{

}

Series::Series(vector<Number*> copy) {
	values.swap(copy);
}

Number* Series::copy(){
	vector<Number*> list = getValues();
	for (int i = 0; i < list.size(); i++){
		list[i] = list[i]->copy();
	}
	return 	new Series(list);
}

string Series::toString(){
	Number* s1 = simplify();
	if (s1->getType().compare("series") != 0)
		return s1->toString();
	Series* s = (Series*)s1;
	ostringstream value;
	string c;

	for (int i = 0; i < s->values.size(); i++){

		if (i == 0){
			value << (s->values[i])->toString();
		}

		else{
			c = getSign(s->values[i]);
			if (c.compare(" - ") == 0)
				s->values[i] = s->values[i]->multiply(new Integer(-1))->simplify();
			value << c << (s->values[i])->toString();
		}

	}
	return value.str();
}

string Series::getSign(Number* j){
	if (j->getType().compare("integer") == 0){
		if (((Integer*)j)->getValue() > 0)
			return " + ";
		else
			return " - ";
	}

	return getSign(j->getCoefficient());
}

Number* Series::getCoefficient(){
	Integer* c = new Integer(1);
	return c;
}

Number* Series::add(Number * n){
	if (n->getType().compare("series") == 0) {
		vector<Number*> list = ((Series*)n)->values;
		for (int i = 0; i<list.size(); i++)
			values.push_back(list[i]);
	}
	else if (n->getType().compare("exponential")==0){
		Exponential* en = (Exponential*)n;
		for (int i = 0; i < values.size(); i++){
			if (values[i]->getType().compare("exponential") == 0){
				Exponential* eval = (Exponential*)values[i];
				if (eval->getBase()->equals(en->getBase()) && eval->getExponent()->equals(en->getExponent())){
					values[i] = values[i]->add(n)->simplify();
					return this;
				}
			}
		}
		values.push_back(n);
		return this;
	}
	else
		values.push_back(n);
	return this;
}

Number* Series::subtract(Number * n){
	for (vector<Number*>::iterator iter = values.begin(); iter != values.end(); ++iter){
		if ((*iter)->equals(n)){
			values.erase(iter);
			return this;
		}
		else
			return add(n->multiply(new Integer(-1)));
	}
	return this;
}

Number* Series::multiply(Number * n){
	if (n->getType().compare("series") == 0){
		Number* s = new Series();
		for (int i = 0; i < values.size(); i++){
			for (int j = 0; j < ((Series*)n)->values.size(); j++){
				s = s->add(values[i]->multiply(((Series*)n)->values[j]));
			}
		}
		return s->simplify();
	}
	for (int i = 0; i < values.size(); i++){
		values[i] = values[i]->multiply(n);
	}
	return this;
}

Number* Series::divide(Number * n){
	for (int i = 0; i < values.size(); i++){
		values[i] = values[i]->divide(n);
	}
	return this;
}


Number* Series::simplify(){
	vector<Number*> values = ((Series*)copy())->values;
	if (values.size() == 1)
		return values.front();
	bool changed = false;
	vector<Number*> newValues;
	Number* temp;
	for (int i = 0; i < values.size(); i++)
	{
		temp = values[i]->simplify();
		for (int j = i + 1; j < values.size(); j++)
		{
			if (temp->getType().compare(values[j]->getType()) == 0 
				|| (temp->getType().compare("integer") == 0 && values[j]->getType().compare("fraction") == 0 && ((Fraction*)values[j])->getNumerator()->getType().compare("integer") == 0 && ((Fraction*)values[j])->getDenominator()->getType().compare("integer") == 0)
				|| (values[j]->getType().compare("integer") == 0 && temp->getType().compare("fraction") == 0 && ((Fraction*)temp)->getNumerator()->getType().compare("integer") == 0 && ((Fraction*)temp)->getDenominator()->getType().compare("integer") == 0))
			{
				if (temp->getType().compare("irrational") == 0 && ((Irrational*)temp)->getValue().compare(((Irrational*)values[j])->getValue()) == 0){
					temp = temp->add(values[j]);
					values[j] = new Integer((long)0);
				}
				if (temp->getType().compare("logarithm") != 0)
				{
					temp = temp->add(values[j]);
					if (temp->getType().compare("series") != 0)
						temp = temp->simplify();
					values[j] = new Integer((long)0);
				}
				else
				{
					if (((Logarithm*)temp)->getBase()->equals(((Logarithm*)(values[j]))->getBase()) && ((Logarithm*)temp)->getX()->equals(((Logarithm*)(values[j]))->getX()))
					{

						temp = temp->add(values[j]);
							if(temp->getType().compare("series") !=0)
								temp=temp->simplify();
						values[j] = new Integer((long)0);

					}
				}
			}
		}

		if (temp->getType().compare("series") == 0) {
			vector<Number*> list = ((Series*)temp)->values;
			for (int i = 0; i < list.size(); i++){
				if (!(list[i]->equals(new Integer((long)0))))
					newValues.push_back(list[i]);
			}
			changed = true;
		}

		else if (!(temp->equals(new Integer((long)0))))
			newValues.push_back(temp);
	}
	if (newValues.size() == 1)
		return newValues.front();
	Number* newwer = new Series(newValues);
	if (changed && newwer->getType().compare("series") != 0){
		return newwer->simplify();
	}
	else
		return newwer;
}

string Series::getType()
{
	return "series";
}

vector<Number*> Series::getValues()
{
	return values;
}

bool Series::equals(Number* val)
{
	if (val->getType().compare("series") == 0){
		vector<Number*> v1=((Series*)val)->getValues();
		vector<Number*> v2 = this->getValues();
		if (v1.size() != v2.size())
			return false;
		for (int i = v1.size() - 1; i >= 0; i--){
			for (int j = v2.size() - 1; j >= 0; j--){
				if (v1[i]->equals(v2[j])){
					v1.erase(v1.begin() + i);
					v2.erase(v2.begin() + j);
					break;
				}
			}
		}
		if (v1.size() == 0)
			return true;
	}
	else
		return false;
}

string Series::getObjectType()
{
	return "number";
}
