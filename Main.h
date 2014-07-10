#ifndef MAIN_H
#define MAIN_H

#include<string> 
#include<vector> 
#include<queue> 
#include<stdexcept> 
#include"Operation.h" 
#include"Number.h"
#include"Answer.h"
#include"Fraction.h"
using namespace std;

//input manipulation
vector<string> tokenizeInput(string input, Answer* answer);
vector<string> parseAns(vector<string> tokens, Answer* answer);
queue<string> shuntingYard(vector<string> tokens);
Number* postfix(queue<Object*> input);
queue<Object*> createObjects(queue<string> &output);
double postfixFloatingPoint(queue<string> postfixStrings);

// string-checking methods
bool isInt(string str);
bool isDouble(string str);
bool isLP(string str);
bool isRP(string str);
bool isE(string str);
bool isPI(string str);
bool isNegE(string str);
bool isNegPI(string str);
bool isOperator(string str);
bool isFunction(string str);
bool isAns(string str);

//parsing methods
long parseLong(string str);
double parseDouble(string str);
char parseOF(string str);
void parseChangeAns(Answer* ans, string inputString);
Fraction* doubleStrToFraction(string str);

//printing methods
void printInitialMenu(int inputMode, string filename);
void printHelpMenu();
void printPreviousAnswers(Answer* ans, int returnMode);
void printToFile(string filename, queue<string> fileOutputs);

//misc
int precedence(char c);

#endif