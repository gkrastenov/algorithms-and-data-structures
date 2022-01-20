#include "interpreter.h"
double InterpreterFunctions::read() const
{
	double value;
	std::cin >> value;
	return value;
}