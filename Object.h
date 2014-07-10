#ifndef OBJECT_H_
#define OBJECT_H_
#include<string>
using namespace std;
class Object 
{
	public:
		virtual string getObjectType() = 0;
};
#endif