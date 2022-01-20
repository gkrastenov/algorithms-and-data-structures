#pragma once
#include "interface.h"
#include "interpreter.h"
#include "container.h"

#include "Exeptions/errorlogger.h"
#include "Structures/ast.h"

#include <stack>
#include <set>
#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>

class Compilator
{
private:
	class ContainerValue {
	private:
		unsigned int numberOfArguments{};
		string code{};
	public:
		ContainerValue() {};
		ContainerValue(const unsigned int numberOfArguments, const string& code)
			:numberOfArguments(numberOfArguments),
			code(code) {};
		~ContainerValue() = default;

		string getCode() const { return code; }
		unsigned int getNumberOfArguments() const { return numberOfArguments;}
	};
private:
	const string DELIMETAR = "->"; // Before "->" is the name of function after that is given code from the user.
	const string ARGUMENT_NUMBER = "Arg number";
	const string ARGUMENT_LIST = "Arg List";
	const string COMMA = ",";
	
	std::unordered_map<string, ContainerValue> container{};

	ErrorLogger logger{}; // handle all exeption during Compilator execution
	ASTNode* tree{};

	bool isCreated = false;
	bool canBePrinted = false;

	Function compiledFunction{};
public:
	Compilator() {};

	int getContainerSize() const { return container.size(); }
	const ErrorType getErrorType() const { return logger.getErrorType(); }
	bool getIsCreated() const { return isCreated; }

	void compileCode(std::istream& stream);

	std::multiset<double> output() const;

	bool isValidBasicFunction(const string& basicFuncCode) const;
private:
	void setErrorType(const ErrorType errorType) const;
	void createNumberNode(std::stack< ASTNode*>& stack, const string& code);
	
	double stringToNumber(const string& number);

	FunctionType getFunctionType(const string& funcName) const;

	int createTreeBody(const string& funCode);
	ASTNode* runTreeBody(ASTNode* astNode);

	// Check if in the container exist function with given name.
	bool containerContains(const string& funcName) const { return container.find(funcName) != container.end(); }


	// Built-in function
	void head(ASTNode* root, Function& listFunciton);
	void tail(ASTNode* root, Function& listFunciton);
	void intiger(ASTNode* root, Function& numberFunction);
	void mod(ASTNode* root, Function& numberFunction);


	string buildCompileCode(const string& code, const std::vector<string>& arguments);
	bool replace(string& str, const string& from, const string& to);

	std::vector<string> getArguments(std::string& code) const;
	unsigned int countArguments(std::string& code) const;
};