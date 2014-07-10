#include"Operation.h" 
#include<string> 
using namespace std;

Operation::Operation(string str)
{
	operation = str;
}

string Operation::getOperation()
{
	return operation;
}

string Operation::getObjectType()
{
	return "operation";
}