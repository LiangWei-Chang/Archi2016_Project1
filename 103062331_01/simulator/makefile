CC = g++ -std=c++11

single_cycle: Processor.o Binary2Assembly.o Register.o Instruction.o
	g++ -o single_cycle Processor.o Binary2Assembly.o Register.o Instruction.o
Processor.o: Processor.cpp Binary2Assembly.h Register.h
	g++ -c Processor.cpp
Binary2Assembly.o: Binary2Assembly.cpp Instruction.h
	g++ -c Binary2Assembly.cpp
Register.o: Register.cpp
	g++ -c Register.cpp
Instruction.o: Instruction.cpp
	g++ -c Instruction.cpp
	
clean:
	rm -f Processor.o Binary2Assembly.o Register.o Instrucion.o