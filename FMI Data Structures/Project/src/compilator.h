#pragma once
#include "interface.h"

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

	ErrorLogger errorLogger{}; // handle all exeption during Compilator execution
	ASTNode* tree{};

	bool isCreated = false;
	bool canBePrinted = false;

	Function compiledFunction{};
public:
	Compilator() {};

	int getContainerSize() const { return container.size(); }
	const ErrorType getErrorType() const { return errorLogger.getErrorType(); }
	bool getIsCreated() const { return isCreated; }
	bool getCanBePrint() const { return canBePrinted; }
	bool isListCompiledFunction() const { return isTypeList(compiledFunction.getType()); }

	void compileCode(std::istream& stream);

	std::vector<double> output();
	string printErrorMessage() { return errorLogger.getErrorMessages(); }

private:
	void setErrorLogger(const ErrorType errorType);
	void createNumberNode(std::stack<ASTNode*>& stack, const string& code);
	
	double stringToNumber(const string& number);

	FunctionType getFunctionType(const string& funcName) const;

	int createTreeBody(const string& funCode);
	ASTNode* runTreeBody(ASTNode* astNode);

	// Check if in the container exist function with given name.
	bool containerContains(const string& funcName) const { return container.find(funcName) != container.end(); }


	// Built-in function
	void head(ASTNode* root);
	void tail(ASTNode* root);
	void integer(ASTNode* root);
	void mod(ASTNode* root);
	void div(ASTNode* root);
	void eq(ASTNode* root);
	void le(ASTNode* root);
	void length(ASTNode* root);
	void add(ASTNode* root);
	void sqrt(ASTNode* root, Function& numberFunction);
	void sub(ASTNode* root);
	void concat(ASTNode* root);
	void list(ASTNode* root, const int countOfArguments);
	//void write(ASTNode* root);

	string buildCompileCode(const string& code, const std::vector<string>& arguments);
	bool replace(string& str, const string& from, const string& to);

	std::vector<string> getArguments(std::string& code) const;
	unsigned int countArguments(std::string& code) const;

	bool isTypeList(const FunctionType type) const;
	bool isValidBasicFunction(const string& code) const;
	bool isBasicFunction(const string& code) const;
};