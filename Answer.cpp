#include <iostream>
#include "Answer.h"
using namespace std;

Answer::Answer()
{
	lastAnswer = "error";
}

Answer::Answer (string t)
{
	lastAnswer = t;
}

string Answer::getLastAnswer(){
	return lastAnswer;
}

void Answer::setLastAnswer(string s){
	lastAnswer = s;
	answers.push_back(s);
}

void Answer::setLastInput(string s)
{
	inputs.push_back(s);
}

void Answer::setLastDAnswer(string s)
{
	dAnswers.push_back(s);
}

vector<string> Answer::getAnswers()
{
	return answers;
}

vector<string> Answer::getInputs()
{
	return inputs;
}

vector<string> Answer::getDAnswers()
{
	return dAnswers;
}