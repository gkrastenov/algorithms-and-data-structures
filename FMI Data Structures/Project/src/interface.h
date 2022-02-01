#pragma once
#include <string>

#include "Structures/llist.h"

using std::string;

enum class FunctionType
{
	NUMBER,
	LIST,
	NOT_CREATED_LIST,
	NOT_CREATED_NUMBER,
	CREATED_LIST,
	HEAD,
	TAIL,
	SQRT,
	EQ,
	ADD,
	READ,
	INT,
	MUL,
	CONCAT,
	LE,
	LENGTH,
	IF,
	WRITE,
	MOD,
	DIV,
	NAND,
	SUB,
	UNKNOWN,
};

class Function
{
private:
	FunctionType type{};
	string name{};
	LList<double> list{};

public:
	Function(){};
	Function(const string& name, const FunctionType type)
		: name(name)
		, type(type)
	{};
	Function(const string& data) {};
	Function(const Function& other) { copy(other); }
	~Function() {};

	const FunctionType getType() const { return type; }
	const FunctionType setType(const FunctionType funcType){ return type = funcType;}

	const string getName() const { return name; }
	LList<double>& getList() { return list; }
	void addNumber(double number) { list.push_end(number); }
	void addList(const LList<double> otherList);

	void replaceList(const double number) { this->list = number; }	
	void replaceList(const size_t index, LList<double>& list);
	void replaceList(const double start, const double step, const size_t count); // only used for loop list
private:
	void copy(const Function& other);
};