#include<iostream> 
#include<sstream> 
#include<fstream>
#include<string> 
#include<vector> 
#include<stack> 
#include<queue> 
#include<stdexcept> 
#include<cstring> 
#include<typeinfo> 
#include<cmath>
#include"Main.h" 
#include"Operation.h" 
#include"Fraction.h"
#include"Answer.h"
#include"Logarithm.h"
#include"Irrational.h"
#include"Exponential.h"
#include"Integer.h"

using namespace std;

int main(int argc, char *argv[])
{
	int inputMode;
	int returnMode = 1;
	char* file = 0;
	string filename;
	string answer;
	string answerD;
	double answerDblVal;
	string inputString;
	queue<string> fileInputs;
	queue<string> fileOutputs;
	ifstream in; //only used when reading from file
	vector<string> tokens;
	queue<string> postfixStrings;
	queue<string> postfixStringsD;
	queue<Object*> postfixObjects;
	stringstream s;
	Answer* ans = new Answer();
	
	//check for input mode argument
	if (argc > 2)
	{
		if (strcmp(argv[1], "file") == 0)
		{
			inputMode = 1;
			file = argv[2];
			
			//creating filename for answers file
			filename.append(file);
			filename = filename.substr(0, filename.length() - 4);
			filename.append("ANSWERS.txt");
			
			ifstream ifile(file);
			if (!ifile)
			{
				cout << "invalid file name" << endl;
				return 0;
			}
		}
		else inputMode = 2;
	}
	else inputMode = 2;
	//
	
	printInitialMenu(inputMode, filename);
	
	if (inputMode == 1) //read fileInput
	{
		in.open(file);
		
		while(true)
		{
			inputString.clear();
			getline(in, inputString, '\n');
			if (inputString.length() == 0)
			{
				break;
			}

			fileInputs.push(inputString.substr(0, inputString.length() - 1));
		}

		in.close();
	}

	while (true) //while user has not selected to quit OR while there are functions left in the file
	{
		try
		{
			while (true) //navigation between modes input, help, etc.
			{
				//reading expression
				if (inputMode == 2) //user input mode
				{
					cout << "> ";
					getline(cin, inputString);
					if (inputString.length() == 0)
					{
						continue;
					}
				}
				else //file input mode
				{
					if (fileInputs.empty()) //no more expressions left
					{
						printToFile(filename, fileOutputs);
						delete ans;
						return 0;
					}
					
					inputString = fileInputs.front();
					cout << "> " << inputString << endl;
					fileInputs.pop();
				}
				
				//checking for special input characters
				if (inputString.at(0) == 'q') //quit
				{
					return 0;
				}
				if (inputString.at(0) == 'h')
				{
					printHelpMenu();
					continue;
				}
				if (inputString.at(0) == 'b')
				{
					printPreviousAnswers(ans, returnMode);
					continue;
				}
				if (inputString.at(0) == '=') //change "ans" to other previous answer
				{
					parseChangeAns(ans, inputString);
					continue;
				}
				if (inputString.at(0) == 'd') //decimal mode toggle
				{
					switch (returnMode)
					{
						case 1:		returnMode = 2;
									cout << "  decimal mode" << endl << endl;
									break;
						case 2:		returnMode = 1;
									cout << "  non-decimal mode" << endl << endl;
									break;
						default:	throw runtime_error("returnMode error");
					}
					
					continue;
				}
				
				break;
			}
			
			tokens = tokenizeInput(inputString, ans); //splits inputString into tokens
			
			/*
			//TESTING
			cout << "TOKENS:" << endl;
			for (int i = 0; i < tokens.size(); i++)
			{
				cout << "|" << tokens[i] << "|" << endl;
			}
			//
			*/

			postfixStrings = shuntingYard(tokens); //infix to postfix notation
			postfixStringsD = postfixStrings; //makes copy for decimal calculation
			postfixObjects = createObjects(postfixStrings); //creates Number objects for each non-operator/non-function 
			
			answer = postfix(postfixObjects)->toString(); //performs calculations
			answerDblVal = postfixFloatingPoint(postfixStringsD); //performs decimal calculations
			
			//converts decimal answer to string representation
			s << answerDblVal;
			answerD = s.str();
			s.str(string());
			
			ans->setLastAnswer(answer); //adds current exact value answer to the list of previous answers
			ans->setLastDAnswer(answerD); //adds current decimal answer to list of previous answers
			ans->setLastInput(inputString); //adds current input expression to list of previous inputs
			
			//determines which answer to print
			if (returnMode == 1) //exact value
			{
				fileOutputs.push(inputString + " = " + answer);
				cout << "  " << answer << endl << endl; //prints answer to the screen
			}
			else //decimal value
			{
				fileOutputs.push(inputString + " = " + answerD);
				cout << "  " << answerD << endl << endl; //prints answer to the screen
			}
		}
		catch (exception &e) //prints error to the screen
		{
			cerr << "  " << e.what() << endl << endl;
		}
	}
	
	delete ans;
	return 0;
}

void printInitialMenu(int inputMode, string filename)
{
	cout << endl;
	cout << "============================================================" << endl;
	cout << "=                        Calculator                        =" << endl;
	cout << "============================================================" << endl;
	if (inputMode == 1)
	{
		cout << "File-parsing mode." << endl;
		cout << "Answers are printed to the screen. A new file called" << endl;
		cout << "  \"" << filename << "\" will also be created and saved" << endl;
		cout << "  in the source directory." << endl;
	}
	else
	{
	cout << "Enter \"q\" at any time to quit." << endl;
	cout << "Enter \"h\" for help/instructions." << endl;
	cout << "Enter \"b\" to view previous answers." << endl;
	cout << "Enter \"d\" to switch between decimal and non-decimal outputs" << endl;
	cout << "  (floating-point and non-floating-point.) The default is" << endl;
	cout << "  non-decimal." << endl;
	}
	
	cout << endl;
}

void printHelpMenu()
{
	cout << endl;
	cout << "----------------------------------------------------------" << endl;
	cout << "  Valid operators:    + - * / ^" << endl;
	cout << "  Valid functions:    sqrt, rt, log" << endl;
	cout << "  Valid irrationals:  e, pi" << endl;
	cout << endl;
	cout << "  Spaces are not required between numbers, operands, and" << endl;
	cout << "    functions;" << endl;
	cout << "    e.g., \"((3+6)*7)/9\"." << endl;
	cout << endl;
	cout << "  Functions always take precedence over operators;" << endl;
	cout << "    e.g., \"3rt:8^2\" -> \"(the cube root of eight) squared\"." << endl;
	cout << endl;
	cout << "  Expressions as function arguments must be parenthesized," << endl;
	cout << "    including negative signs;" << endl;
	cout << "    e.g., \"log_(3^3):27\" equals 1, while \"log_3^3:27\"" << endl;
	cout << "    returns an error." << endl;
	cout << endl;
	cout << "  Multiplication is implied when not explicitly stated;" << endl;
	cout << "    e.g., \"4pi\" -> \"four times pi\"." << endl;
	cout << endl;
	cout << "  Negative numbers must not have a space between the" << endl;
	cout << "    dash and the number" << endl;
	cout << "    e.g., \"-6 * 3\", rather than \"- 6 * 3\"." << endl;
	cout << endl;
	cout << "  ------Function Format------" << endl;
	cout << "  log_<base>:<value>   (underscore and colon must be used)" << endl;
	cout << "  sqrt:<value>         (colon must be used)" << endl;
	cout << "  <value>rt:<value>    (colon must be used)" << endl;
	cout << endl;
	cout << "  -----Function Examples-----" << endl;
	cout << "  4sqrt:9      ->   \"four times the square root of nine\"" << endl;
	cout << "  (4*3)rt:8    ->   \"the 12th root of 8\"" << endl;
	cout << "  4*3rt:8      ->   \"four times the cube root of 8\"" << endl;
	cout << "  4(3)rt:8     ->   \"four times the cube root of 8\"" << endl;
	cout << "  4log_3:8     ->   \"four times log base 3 of 8\"" << endl;
	cout << "----------------------------------------------------------" << endl << endl;
}

void printPreviousAnswers(Answer* ans, int returnMode)
{
	vector<string> answers;
	switch(returnMode)
	{
		case 1:		answers = ans->getAnswers();
					break;
		case 2:		answers = ans->getDAnswers();
					break;
		default:	throw runtime_error("d answers error");
	}
	
	if (answers.size() == 0)
	{
		cout << "  No previous answers stored." << endl << endl;
		return;
	}
	
	cout << endl;
	cout << "----------------------------------------------------------" << endl;
	
	for (int i = 0; i < answers.size(); i++)
	{
		cout << "  [" << answers.size() - i << "]\t";
		cout << ans->getInputs()[i] << "  =  ";
		cout << answers[i] << endl;
	}
	
	cout << endl;
	cout << "  Enter \"=\" followed by a space and a number from the" << endl;
	cout << "    list above to set the \"ans\" keyword to that" << endl;
	cout << "    answer. To view decimal answers, switch output" << endl;
	cout << "    modes by entering \"d\". Then enter \"b\" again." << endl;
	cout << "----------------------------------------------------------" << endl << endl;
}

void printToFile(string filename, queue<string> fileOutputs)
{
	ofstream out;
	
	
	
	out.open(filename.c_str());

	while (!fileOutputs.empty())
	{
		out << fileOutputs.front() + "\n\n";
		fileOutputs.pop();

	}
	
	out.close();
}

void parseChangeAns(Answer* ans, string inputString)
{
	int index = 0;
	
	if (inputString.length() < 3)
	{
		throw invalid_argument("invalid input");
	}
	
	try
	{
		index = (ans->getAnswers().size()) - (parseLong(inputString.substr(2, inputString.length() - 2))); 
		//parses int and processes into value that following vectors can handle
	}
	catch (exception &e)
	{
		throw invalid_argument("invalid input");
	}

	if ((index < 0) || (index > (ans->getAnswers().size() - 1)))
	{
		throw invalid_argument("invalid number");
	}

	ans->setLastAnswer(ans->getAnswers()[index]);
	ans->setLastInput(ans->getInputs()[index]);
	
	cout << endl << "  ans = " << ans->getLastAnswer() << endl << endl;
}

queue<Object*> createObjects(queue<string> &postfixStrings)
{
	queue<Object*> input;
	
	while (!postfixStrings.empty())
	{
		if (isInt(postfixStrings.front())) //int
		{
			input.push(new Integer(parseLong(postfixStrings.front())));
		}
		else if (isDouble(postfixStrings.front()))
		{
			input.push(doubleStrToFraction(postfixStrings.front()));
		}
		else if (isE(postfixStrings.front()) || isPI(postfixStrings.front())) //e or pi
		{
			input.push(new Irrational(postfixStrings.front()));
		}
		else if (isNegE(postfixStrings.front()) || isNegPI(postfixStrings.front())) //negative e or pi
		{
			cout << postfixStrings.front().substr(1, postfixStrings.front().length() - 2) << endl;
			input.push(new Irrational(postfixStrings.front().substr(1, postfixStrings.front().length() - 1), new Integer(-1)));
		}
		else //operator or function
		{
			input.push(new Operation(postfixStrings.front()));
		}
		
		postfixStrings.pop();
	}
	
	return input;
}

Fraction* doubleStrToFraction(string str)
{
	//find decimal point
	int i;
	for (i = 0; i < str.length(); i++)
	{
		if (str.at(i) == '.')
		{
			break;
		}
	}
	
	double dbl = parseDouble(str);
	double numDecimals = str.length() - i - 1;
	int numeratorInt = (int)(dbl * pow(10.0, numDecimals));
	int denominatorInt = (int)(pow(10.0, numDecimals));
	Integer* numerator = new Integer(numeratorInt);
	Integer* denominator = new Integer(denominatorInt);
	
	return new Fraction(numerator, denominator);
}

vector<string> tokenizeInput(string input, Answer* answer)
{
	vector<string> tokens;
	int numWhitespace = 3; //number of whitespace characters
	char whitespace[] = {' ',':','_'}; //whitespace characters
	int k;
	
	bool logmode = false; //check for log arguments for implied multiplication
	int logArg = 1;
	int logLP1 = 0;
	int logRP1 = 0;
	int logLP2 = 0;
	int logRP2 = 0;
	int logiter = 0;
	
	bool correct;
	bool justEnded = false;
	
	string currentToken = "";
	input += " "; //allows proper parsing of last character
	
	for (int i = 0; i < input.length(); i++) //iterate through characters in input
	{
		//logmode check
		if (tokens.size() > 0)
		{
			if (tokens.back().compare("log") == 0)
			{
				logmode = true;
				logArg = 1;
			}
			if (logmode && logArg == 1 && tokens.back().compare("(") == 0)
			{
				logLP1++;
			}
			if (logmode && logArg == 1 && tokens.back().compare(")") == 0)
			{
				logRP1++;
			}
			if (tokens.size() > 0)
			{
				if (logmode && logArg == 2 && tokens.back().compare("(") == 0)
				{
					logLP2++;
				}
				if (logmode && logArg == 2 && tokens.back().compare(")") == 0)
				{
					if (!justEnded)
					{
						logRP2++;
					}
				}
				
				if (logmode && logArg == 2)
				{
					logiter++;
				}
			}
		}
		if (logmode && logArg == 2 && logLP2 == logRP2 && logLP2 > 0 && logRP2 > 0 && !isInt(tokens.back()) && !isDouble(tokens.back()))
		{
			logmode = false;
			logLP1 = 0;
			logRP1 = 0;
			logLP2 = 0;
			logRP2 = 0;
			logiter = 0;
		}
		if (logmode && logArg == 2 && logLP2 == logRP2 && logiter > 1)
		{
			logmode = false;
			logLP1 = 0;
			logRP1 = 0;
			logLP2 = 0;
			logRP2 = 0;
			logiter = 0;
		}
		//
			
		for (int j = 0; j < numWhitespace; j++) //check if whitespace character
		{
		
		
			if (input.at(i) == whitespace[j]) //if whitespace character
			{
				if (currentToken.compare("") != 0)
				{
					tokens.push_back(currentToken);
					currentToken = "";
				}
				
				if (whitespace[j] == ':' && logmode && !isFunction(tokens.back()))
				{
					justEnded = true;
					logArg = 2;
				}
				
				break;
			}
			else if (j == (numWhitespace - 1)) //if not whitespace character
			{
				justEnded = false;
				//pushing back special tokens
				
				//operator or parenthesis
				if (input.at(i) == '(' ||
					input.at(i) == ')' ||
					input.at(i) == '+' ||
					input.at(i) == '-' ||
					input.at(i) == '*' ||
					input.at(i) == '/' ||
					input.at(i) == '^')
				{
					if (currentToken.compare("") != 0)
					{
						tokens.push_back(currentToken);
						currentToken = "";
					}
					if (input.at(i) == '-') //operator: minus
					{
						for (k = 1; k < input.length() - i; k++) //sets k = number of whitespace characters after input.at(i)
						{
							for (int l = 0; l < numWhitespace; l++)
							{
								if (i + k < input.length())
								{
									if (input.at(i + k) != whitespace[l])
									{
										break;
									}
								}
							}
							
							break;
						}
						
						if (k > 0 && (i + k) < input.length())
						{
							//cout << "tokens size = " << tokens.size() << endl;
							if ((tokens.size() > 0 && !logmode && 
								(/*input.at(i + k) == 'e' ||
								 input.at(i + k) == 'p' ||*/
								 input.at(i + k) == 's' ||
								 input.at(i + k) == 'a' ||
								 input.at(i + k) == 'l' ||
								 input.at(i + k) == '(') &&
								!isInt(tokens.back()) && 
								!isDouble(tokens.back()) && 
								!isE(tokens.back()) && 
								!isPI(tokens.back()) &&
								!isRP(tokens.back()) && 
								!isAns(tokens.back())) || 
							    (tokens.size() == 0 && input.at(i + k) != 'e' && input.at(i + k) != 'p'))
							{
								tokens.push_back("-1");
								tokens.push_back("*");
								break;
							}
						}
						
						if (tokens.size() > 0)
						{
							if(!isOperator(tokens.back()) && 
							   !isLP(tokens.back()) &&
							   !isFunction(tokens.back())/* && //possible error from this patch
							   !logmode*/) //possible error from this patch
							{
								tokens.push_back("-");
								break;
							}
						}
						
						currentToken = "-";
					}
					else if (input.at(i) == '(') //operator: left parenthesis
					{
						if (tokens.size() > 0)
						{
							if (!logmode && 
								(isInt(tokens.back()) ||
								 isDouble(tokens.back()) ||
								 isRP(tokens.back()) ||
								 isE(tokens.back()) ||
								 isPI(tokens.back()) ||
								 isAns(tokens.back())))
							{
								tokens.push_back("*");
							}
						}
						
						currentToken += input.at(i);
						tokens.push_back(currentToken);
						currentToken = "";
					}
					else if (input.at(i) == ')') //operator: right parenthesis
					{
						currentToken += input.at(i);
						tokens.push_back(currentToken);
						currentToken = "";
						
						for (k = 1; k < input.length() - i; k++) //sets k = number of whitespace characters after input.at(i)
						{
							for (int l = 0; l < numWhitespace; l++)
							{
								if (i + k < input.length())
								{
									if (input.at(i + k) != whitespace[l])
									{
										break;
									}
								}
							}
							
							break;
						}
						
						if (k > 0 && (i + k) < input.length())
						{
							currentToken += input.at(i + k);
							if (!logmode && 
								(isInt(currentToken) || isDouble(currentToken) || isE(currentToken) || 
								 input.at(i + k) == 'p' || input.at(i + k) == 's' || isAns(currentToken)))
							{
								tokens.push_back("*");
							}
							currentToken = "";
						}
					}
					else //other operator (not minus, RP, or LP)
					{
						currentToken += input.at(i);
						tokens.push_back(currentToken);
						currentToken = "";
					}
				}
				//ans keyword
				else if (input.at(i) == 'a')
				{
					if (input.size() - i > 2)
					{
						if (input.at(i + 1) == 'n' && 
							input.at(i + 2) == 's')
						{
							if (currentToken.compare("") != 0)
							{
								tokens.push_back(currentToken);
								currentToken = "";
							}
							
							if (!logmode && input.size() - i > 1 && tokens.size() > 0)
							{
								if (isRP(tokens.back()) ||
									isInt(tokens.back()) ||
									isDouble(tokens.back()) ||
									isE(tokens.back()) ||
									isPI(tokens.back()) ||
									isAns(tokens.back()))
								{
									tokens.push_back("*");
								}
							}
							
							tokens.push_back("ans");
							i += 2;
							
							for (k = 1; k < input.length() - i; k++) //sets k = number of whitespace characters after input.at(i)
							{
								for (int l = 0; l < numWhitespace; l++)
								{
									if (i + k < input.length())
									{
										if (input.at(i + k) != whitespace[l])
										{
											break;
										}
									}
								}
								
								break;
							}
							
							if (k > 0 && (i + k) < input.length())
							{
								currentToken += input.at(i + k);
								if (!logmode && 
									(isInt(currentToken) ||
									 isDouble(currentToken) ||
									 isE(currentToken) || 
									 input.at(i + k) == 'p' ||
									 input.at(i + k) == 's' ||
									 isAns(currentToken)))
								{
									tokens.push_back("*");
								}
								currentToken = "";
							}
							
							
						}
					}
				}
				//sqrt or log function
				else if (input.at(i) == 's' || input.at(i) == 'l')
				{
					correct = false;
					
					if (input.size() - i > 3)
					{
						if (input.at(i) == 's' &&
							input.at(i + 1) == 'q' &&
							input.at(i + 2) == 'r' &&
							input.at(i + 3) == 't')
						{
							correct = true;
						}
						else if (input.at(i) == 'l' &&
								 input.at(i + 1) == 'o' &&
								 input.at(i + 2) == 'g')
						{
							correct = true;
						}
						
						if (correct)
						{
							if (currentToken.compare("") != 0)
							{
								tokens.push_back(currentToken);
								currentToken = "";
							}

							if (!logmode && input.size() - i > 1 && 
								tokens.size() > 0)
							{
								if (isRP(tokens.back()) ||
									isInt(tokens.back()) ||
									isDouble(tokens.back()) ||
									isE(tokens.back()) ||
									isPI(tokens.back()) ||
									isAns(tokens.back()))
								{
									tokens.push_back("*");
								}
							}
							
							if (input.at(i) == 's')
							{
								tokens.push_back("sqrt");
								i += 3;
							}
							else
							{
								tokens.push_back("log");
								i += 2;
							}
						}
					}
				}
				//rt function
				else if (input.at(i) == 'r')
				{
					if (input.size() - i > 1)
					{
						if (input.at(i + 1) == 't')
						{
							if (currentToken.compare("") != 0)
							{
								tokens.push_back(currentToken);
								currentToken = "";
							}
							
							tokens.push_back("rt");
							i++;
						}
					}
				}
				//irrational pi or e
				else if (input.at(i) == 'e' ||
						 input.at(i) == 'p')
				{
					correct = false;
					
					if (input.at(i) == 'p')
					{
						if (input.size() - i > 1)
						{
							if (input.at(i + 1) == 'i')
							{
								correct = true;
							}
						}
					}
					else if (input.at(i) == 'e')
					{
						correct = true;
					}
					
					if (correct)
					{
						if (currentToken.compare("") != 0 && currentToken.compare("-") != 0)
						{
							tokens.push_back(currentToken);
							currentToken = "";
						}
						if ((!logmode || 
							 (logmode && logArg == 1 && logLP1 != logRP1) || 
							 (logmode && logArg == 2 && logLP2 != logRP2)) && 
							input.size() - i > 1 && 
							tokens.size() > 0)
						{
							if (isRP(tokens.back()) ||
								isInt(tokens.back()) ||
								isDouble(tokens.back())||
								isE(tokens.back()) ||
								isPI(tokens.back()) ||
								isAns(tokens.back()))
							{
								tokens.push_back("*");
							}
						}
						
						if (input.at(i) == 'e')
						{
							currentToken += "e";
							tokens.push_back(currentToken);
							currentToken = "";
						}
						else
						{
							currentToken += "pi";
							tokens.push_back(currentToken);
							currentToken == "";
							i++;
						}

						for (k = 1; k < input.length() - i; k++) //sets k = number of whitespace characters after input.at(i)
						{
							for (int l = 0; l < numWhitespace; l++)
							{
								if (i + k < input.length())
								{
									if (input.at(i + k) != whitespace[l])
									{
										break;
									}
								}
							}
							
							break;
						}
						
						if (k > 0 && (i + k) < input.length())
						{
							currentToken += input.at(i + k);
							if ((!logmode || 
								 (logmode && logArg == 1 && logLP1 != logRP1) || 
								 (logmode && logArg == 2 && logLP2 != logRP2)) && 
								(isInt(currentToken) ||
								 isDouble(currentToken) ||
								 isE(currentToken) ||
								 isLP(currentToken) ||
								 input.at(i + k) == 'p' ||
								 input.at(i + k) == 's' ||
								 isAns(currentToken)))
							{
								tokens.push_back("*");
							}
							currentToken = "";
						}
					}
					else
					{
						currentToken += input.at(i);
					}
				}
				//other character (number)
				else
				{
					currentToken += input.at(i);
				}
			}
		}
	}
	
	return parseAns(tokens, answer);
}

vector<string> parseAns(vector<string> tokens, Answer* answer)
{
	vector<string> newTokens;
	vector<string> returnTokens;
	
	for (int i = 0; i < tokens.size(); i++)
	{
		if (tokens[i].compare("ans") == 0)
		{
			newTokens = tokenizeInput(answer->getLastAnswer() + " ", answer);
			
			for (int j = 0; j < i; j++)
			{
				returnTokens.push_back(tokens[j]);
			}
			
			returnTokens.push_back("(");
			
			for (int j = 0; j < newTokens.size(); j++)
			{
				returnTokens.push_back(newTokens[j]);
			}
			
			newTokens.clear();
			
			returnTokens.push_back(")");
			
			for (int j = i + 1; j < tokens.size(); j++)
			{
				returnTokens.push_back(tokens[j]);
			}
			
			return parseAns(returnTokens, answer);
		}
	}
	
	if (returnTokens.size() == 0) //no ans keyword
	{
		return tokens;
	}
	
	return returnTokens;
}

queue<string> shuntingYard(vector<string> tokens)
{
	queue<string> output;
	stack<string> function;

	for (int i = 0; i < tokens.size(); i++)
	{
		if (isInt(tokens[i]) || isDouble(tokens[i])) //if the token is a number 
		{
			output.push(tokens[i]);
		}
		else if (isE(tokens[i]) || isPI(tokens[i]) || isNegE(tokens[i]) || isNegPI(tokens[i]))
		{
			output.push(tokens[i]);
		}
		else if (isFunction(tokens[i])) //if the token is a function 
		{
			if (i == tokens.size() - 1)
			{
				throw invalid_argument("invalid parameters on " + tokens[i] + " operator");
			}
			else if (isInt(tokens[i + 1]) || isDouble(tokens[i + 1]) ||
					 isLP(tokens[i + 1]) || 
					 isPI(tokens[i + 1]) || isE(tokens[i + 1]))
			{
				function.push(tokens[i]);
			}
			else
			{
				tokens.clear();
				throw invalid_argument("invalid parameters on " + tokens[i] + " operator");
			}
		}
		else if (isOperator(tokens[i])) //if the token is an operator 
		{
			while (!function.empty()) //while there are still elements in the stack 
			{
				if (isOperator(function.top())) //if the top element is an operator 
				{
					if (precedence(parseOF(tokens[i])) >= precedence(parseOF(function.top()))) //if the precedence of the token is less than or equal to the precedence of the top operator in the stack 
					{
						output.push(function.top());
						function.pop();
					}
					else
					{
						break;
					}
				}
				else if (isFunction(function.top()))
				{
					output.push(function.top());
					function.pop();
				}
				else
				{
					break;
				}
			}

			function.push(tokens[i]);
		}
		else if (isLP(tokens[i])) //if the token is a left parenthesis 
		{
			function.push(tokens[i]);
		}
		else if (isRP(tokens[i])) //if the token is a right parenthesis 
		{
			if (function.empty())
			{
				throw invalid_argument("mismatched parentheses");
			}

			while (!isLP(function.top()))
			{
				output.push(function.top());
				function.pop();

				if (function.empty())
				{
					throw invalid_argument("mismatched parentheses");
				}
			}
			
			function.pop(); //pops the LP from the stack (and not onto the output queue)
			if (!function.empty())
			{
				if (isFunction(function.top()))
				{
					if (function.top().compare("log") != 0)
					{
						output.push(function.top());
						function.pop();
					}
				}
			}
		}
		else
		{
			throw invalid_argument("invalid input, syntax error");
		}
	}

	//no more tokens to be read 
	while (!function.empty())
	{
		if (isLP(function.top()) || isRP(function.top()))
		{
			throw invalid_argument("mismatched parentheses");
		}
		else
		{
			output.push(function.top());
			function.pop();
		}
	}
	return output;
}

Number* postfix(queue<Object*> postfixObjects)
{
	stack<Object*> value;
	Number* arg1;
	Number* arg2;

	while (!postfixObjects.empty())
	{
		if (postfixObjects.front()->getObjectType().compare("number") == 0)
		{
			if (((Number*)postfixObjects.front())->getType().compare("integer") == 0 ||
				((Number*)postfixObjects.front())->getType().compare("irrational") == 0 ||
				((Number*)postfixObjects.front())->getType().compare("fraction") == 0) //if element is an integer, irrational, or fraction
			{
				value.push(postfixObjects.front()); //push it onto the stack 
			}
		}
		else
		{
			if (((Operation*)postfixObjects.front())->getOperation().compare("sqrt") == 0)//operation is sqrt, only takes 1 argument 
			{
				if (value.size() < 1)//if there too few values on the stack 
				{
					while (!value.empty())
					{
						value.pop();
					}
					while (!postfixObjects.empty())
					{
						postfixObjects.pop();
					}
					
					throw invalid_argument("not enough arguments for sqrt operator");
				}
				else
				{
					arg1 = (Number*)value.top();
					value.pop();
					value.push(new Exponential(arg1, new Fraction(new Integer(1), new Integer(2)), new Integer(1)));
				}
			}
			else //any other operator (taking 2 arguments)
			{
				if (value.size() < 2)//if there too few values on the stack 
				{
					string err = ((Operation*)postfixObjects.front())->getOperation();
					while (!value.empty())
					{
						value.pop();
					}
					while (!postfixObjects.empty())
					{
						postfixObjects.pop();
					}
					
					throw  invalid_argument("not enough arguments for " + err + " operator");
				}
				else
				{
					arg2 = (Number*)value.top();
					value.pop();
					arg1 = (Number*)value.top();
					value.pop();
					switch (((Operation*)postfixObjects.front())->getOperation().at(0))
					{
					case '+':   value.push(arg1->add(arg2));
								break;
					case '-':   value.push(arg1->subtract(arg2));
								break;
					case '*':   value.push(arg1->multiply(arg2));
								break;
					case '/':   value.push(new Fraction(arg1, arg2));
								break;
					case '^':   value.push(new Exponential(arg1, arg2, new Integer(1)));
								break;
					case 'r':   value.push(new Exponential(arg2, new Fraction(new Integer(1), arg1), new Integer(1)));
								break;
					case 'l':   value.push(new Logarithm(arg1, arg2, new Integer(1)));
								break;
					default:    throw runtime_error("error 1");
					}
				}
			}
		}

		postfixObjects.pop();

	}
	
	if (value.size() != 1)
	{
		while (!value.empty())
		{
			value.pop();
		}
		
		throw invalid_argument("too many arguments entered");
	}
	
	Number* returnVal = (Number*)(value.top());
	value.pop();
	
	return returnVal;
}

double postfixFloatingPoint(queue<string> postfixStrings)
{
	stack<double> value;
	double arg1;
	double arg2;

	while (!postfixStrings.empty())
	{
		if (isDouble(postfixStrings.front()))
		{
			value.push(parseDouble(postfixStrings.front())); //push it onto the stack 
		}
		else if (isE(postfixStrings.front()))
		{
			value.push(2.7182818285);
		}
		else if (isPI(postfixStrings.front()))
		{
			value.push(3.1415926536);
		}
		else if (isNegE(postfixStrings.front()))
		{
			value.push(-2.7182818285);
		}
		else if (isNegPI(postfixStrings.front()))
		{
			value.push(-3.1415926536);
		}
		else
		{
			if (parseOF(postfixStrings.front()) == 's')//operation is sqrt, only takes 1 argument 
			{
				if (value.size() < 1)//if there too few values on the stack 
				{
					while (!value.empty())
					{
						value.pop();
					}
					while (!postfixStrings.empty())
					{
						postfixStrings.pop();
					}
					
					throw invalid_argument("not enough arguments for sqrt operator");
				}
				else
				{
					arg1 = value.top();
					value.pop();
					value.push(sqrt(arg1));
				}
			}
			else //function that takes 2 arguments
			{
				if (value.size() < 2)//if there too few values on the stack 
				{
					string err = postfixStrings.front();
					while (!value.empty())
					{
						value.pop();
					}
					while (!postfixStrings.empty())
					{
						postfixStrings.pop();
					}
					
					throw  invalid_argument("not enough arguments for " + err + " operator");
				}
				else
				{
					arg2 = value.top();
					value.pop();
					arg1 = value.top();
					value.pop();
					switch (parseOF(postfixStrings.front()))
					{
					case '+':   value.push(arg1 + arg2);
								break;
					case '-':   value.push(arg1 - arg2);
								break;
					case '*':   value.push(arg1 * arg2);
								break;
					case '/':   value.push(arg1 / arg2);
								break;
					case '^':   value.push(pow(arg1, arg2));
								break;
					case 'r':   value.push(pow(arg2, 1 / arg1));
								break;
					case 'l':   value.push(log10(arg2) / log10(arg1));
								break;
					default:    throw runtime_error("error 1");
					}
				}
			}
		}

		postfixStrings.pop();

	}
	
	if (value.size() != 1)
	{
		while (!value.empty())
		{
			value.pop();
		}
		
		throw invalid_argument("too many arguments entered");
	}
	
	double returnVal = value.top();
	value.pop();
	
	return returnVal;
}

bool isInt(string str)
{
	for (int i = 0; i < str.length(); i++)
	{
		if (!isdigit(str.at(i)))
		{
			if (i != 0 || str.length() == 1 || str.at(i) != '-')
			{
				return false;
			}
		}
	}
	
	return true;
}

bool isDouble(string str)
{
	int decimals = 0;
	for (int i = 0; i < str.length(); i++)
	{
		if (!isdigit(str.at(i)) && str.at(i) != '.')
		{
			if (i != 0 || str.length() == 1 || str.at(i) != '-')
			{
				return false;
			}
		}
		
		if (str.at(i) == '.')
		{
			decimals++;
		}
		
		if (decimals > 1)
		{
			throw invalid_argument("error: multiple decimals");
		}
	}
	
	return true;
}

bool isOperator(string str)
{
	if (str.length() == 1)
	{
		if (str.compare("+") == 0 ||
			str.compare("-") == 0 ||
			str.compare("*") == 0 ||
			str.compare("/") == 0 ||
			str.compare("^") == 0)
		{
			return true;
		}
	}

	return false;
}

bool isFunction(string str)
{
	if (str.length() == 4 &&
		str.compare("sqrt") == 0)
	{
		return true;
	}
	else if (str.length() == 3 &&
		str.compare("log") == 0)
	{
		return true;
	}
	else if (str.length() == 2 &&
		str.compare("rt") == 0)
	{
		return true;
	}

	return false;
}

bool isLP(string str)
{
	if (str.length() == 1)
	{
		if (str.compare("(") == 0)
		{
			return true;
		}
	}

	return false;
}

bool isRP(string str)
{
	if (str.length() == 1)
	{
		if (str.compare(")") == 0)
		{
			return true;
		}
	}

	return false;
}

bool isE(string str)
{
	if (str.length() == 1)
	{
		if (str.compare("e") == 0)
		{
			return true;
		}
	}

	return false;
}

bool isNegE(string str)
{
	if (str.length() == 2)
	{
		if (str.compare("-e") == 0)
		{
			return true;
		}
	}

	return false;
}

bool isPI(string str)
{
	if (str.length() == 2)
	{
		if (str.compare("pi") == 0)
		{
			return true;
		}
	}

	return false;
}

bool isNegPI(string str)
{
	if (str.length() == 3)
	{
		if (str.compare("-pi") == 0)
		{
			return true;
		}
	}

	return false;
}

bool isAns(string str)
{
	return str.compare("ans") == 0;
}

long parseLong(string str)
{
	if (!isInt(str))
	{
		throw invalid_argument(str + " is not an int");
	}
	long i;
	istringstream(str) >> i;
	return i;
}

double parseDouble(string str)
{
	if(!isDouble(str))
	{
		throw invalid_argument(str + " is not a double");
	}
	
	double i;
	stringstream s;
	s << str;
	s >> i;

	return i;
}

char parseOF(string str)
{
	return str.at(0);
}

int precedence(char c)
{
	int p;

	switch (c)
	{
		case '^':   p = 2;
					break;
		case '*':
		case '/':   p = 3;
					break;
		case '+':
		case '-':   p = 4;
					break;
		default:	p = 1;
	}

	return p;
}
	