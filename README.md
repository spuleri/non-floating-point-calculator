Non-Floating Point Calculator
COP 3503
Section 6773

Group 11-3
Group Members:
	Carl Amko
	Monica Gandhi
	Madhumita Mazumder
	Kevin Neumann
	Joshua Novick
	Sergio Puleri

Compilation
	A makefile has been included in the directory. It utilizes the g++ compiler. It is based on C++ 98.
	The makefile creates "Project.exe".
	To run, enter "./Project".
	To run in file-input mode, enter "./Project file filename.txt". More information on file-input mode
	is below.

File-Input Mode
	A .txt file can be used to evalulate expressions. The file must be in the same directory as the
	source and must not contain spaces in its name. Each expression should be entered as would be on the
	calculator (with required spacing, etc.). Separate multiple expressions with a single line break. A
	single line must be at the end of the file. If there are issues with the last character being
	truncated, insert a space after each expression (this is a text editor issue). 
	
	The source directory contains an example file. The contents and name can both be changed, as long as
	the format requirments are met.

Changes Since First Submission
	- General bug fixes for standard capabilities (all test cases work, including highly complex).
	- Improved stability. 
	- Ability to parse .txt files.
	- No longer requires spaces between numbers/operators/parentheses. No spaces are required at all.
	- Multiplication no longer needs to be explicit; i.e., "3(2+3)" outputs "15", and "3pi" is read as
	  "3 * pi".
	- Ability to read in decimals and output in decimal form (decimals can always be read in, and the
	  decimal mode in the program only changes the output format). 
	- Ability to display previous answers in decmial form (when in decimal mode). 
