#pragma once
#include <string>
#include <iostream>
#include <list>

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
	NAND,
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
	// Function(Function&& fnc) noexcept {}; TODO: which constructor to use for creating astNODE move or copy

	const FunctionType getType() const { return type; }
	const FunctionType setType(const FunctionType funcType){ return type = funcType;}

	const string getName() const { return name; }
	LList<double> getList() const{ return list; }
	void add_number(double number) { list.push_end(number); }
	void add_list(const LList<double> otherList);

	void replace_list(const double number) { this->list = number; }	
	void replace_list(const size_t index,const LList<double>& list);

private:
	void copy(const Function& other);
};