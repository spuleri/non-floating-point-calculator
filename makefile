all: Project
	
Project: Answer.o Exponential.o Fraction.o Integer.o Irrational.o Logarithm.o Main.o Operation.o Series.o
	g++ -o Project Answer.o Exponential.o Fraction.o Integer.o Irrational.o Logarithm.o Main.o Operation.o Series.o

Main.o: Main.cpp
	g++ -c Main.h Main.cpp

Answer.o: Answer.cpp
	g++ -c Answer.h Answer.cpp

Exponential.o: Exponential.cpp
	g++ -c Exponential.h Exponential.cpp

Fraction.o: Fraction.cpp
	g++ -c Fraction.h Fraction.cpp

Integer.o: Integer.cpp
	g++ -c Integer.h Integer.cpp

Irrational.o: Irrational.cpp
	g++ -c Irrational.h Irrational.cpp

Logarithm.o: Logarithm.cpp
	g++ -c Logarithm.h Logarithm.cpp

Operation.o: Operation.cpp
	g++ -c Operation.h Operation.cpp

Series.o: Series.cpp
	g++ -c Series.h Series.cpp

clean:
	rm -rf *o *gch *stackdump Project