#ifndef OPERATION_H_
#define OPERATION_H_

#include"Object.h" 
#include<string> 

using namespace std;

class Operation : public Object
{
public:
	Operation(string str);
	string getOperation();
	string getObjectType();

private:
	string operation;
};
#endif