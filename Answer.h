#ifndef ANSWER_H_ 
#define ANSWER_H_

#include<iostream>
#include <string>
#include<vector>
using namespace std;
class Answer
{
private:
	string lastAnswer;
	vector<string> answers;
	vector<string> inputs;
	vector<string> dAnswers;

public:
	Answer();
	Answer(string str);
	string getLastAnswer();
	void setLastAnswer(string s);
	void setLastInput(string s);
	void setLastDAnswer(string s);
	vector<string> getAnswers();
	vector<string> getInputs();
	vector<string> getDAnswers();
};

#endif