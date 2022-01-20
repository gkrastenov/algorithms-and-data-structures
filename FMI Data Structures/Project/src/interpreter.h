#pragma once
#include <math.h> 

#include "interface.h"


enum class InterpreterFunctionsType {
	Eq,
	Read,
	Head,
	Add,
	Sqrt,
	Mul,
	Unknown,
};

/*
class InterpreterFunctions
{
private:

public:
	/// <summary>
	/// Compare two FunctionNumber if they are equal.
	/// <para>Two FunctionNumber are equal when their values ​​are equal.</para> 
	/// </summary>
	/// <returns>Returns the Boolean type to func1 == func2.</returns>
	bool eq(const FunctionNumber& func1, const FunctionNumber& func2) const { return func1 == func2; }

	/// <summary>
	/// Compare two FunctionsList if they are equal.
	/// <para>Two lists are equal when their respective elements are equal.</para> 
	/// </summary>
	/// <returns>Returns the Boolean type to func1 == func2.</returns>
	bool eq(const FunctionList& func1, const FunctionList& func2) const { return func1 == func2; }

	/// <summary>
	/// Compare FunctionNumber and FunctionList if they are equal.
	/// <para>FunctionNumber and FunctionList are equal if the list has 1 element and it is equal to the number.</para>
	/// </summary>
	/// <returns>Returns the Boolean type to func1 == func2.</returns>
	bool eq(const FunctionNumber& func1, const FunctionList& func2) const { return func1 == func2; }

	/// <summary>
	/// Returns a number read from standard input.
	/// </summary>
	/// <returns>Double value given by user.</returns>
	double read() const;

	/// <summary>
	/// Returns the first element in a list.
	/// </summary>
	/// <returns>The first element in the given list.</returns>
	double head(const FunctionList& func) const { return func[0]; }

	/// <summary>
	/// Sum of two FunctionNumbers.
	/// </summary>
	/// <returns>Return the sum of two FunctionNumbers: func1 + func2.</returns>
	double add(const FunctionNumber& func1, const FunctionNumber& func2) const { return func1.getValue() + func2.getValue(); }

	/// <summary>
	/// Sqrt of func.
	/// </summary>
	/// <returns>Return the sqrt of func.</returns>
	double sqrt(const FunctionNumber& func) const { return std::sqrt(func.getValue()); }

	/// <summary>
	/// Sum of two FunctionNumbers.
	/// </summary>
	/// <returns>Return the sum of two FunctionNumbers: func1 + func2.</returns>
	double mul(const FunctionNumber& func1, const FunctionNumber& func2) const { return func1.getValue() + func2.getValue(); }
};
*/