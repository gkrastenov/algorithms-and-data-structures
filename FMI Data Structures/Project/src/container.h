#pragma once
/*
#include<vector>

#include "functionList.h"
#include "functionNumber.h"

/// <summary>
/// Class container who will record all created functions from user.
/// </summary>
class FunctionContainer
{
private:
	/// <summary>
	/// Functions of type FunctionNumber who are created. 
	/// </summary>
	std::vector<FunctionNumber> functionNumbers;

	/// <summary>
	/// Functions of type FunctionList who are created. 
	/// </summary>
	std::vector<FunctionList> functionLists;
public:
	FunctionContainer() {};

	int getFunctionNumbersSize() const { return functionNumbers.size(); }
	int getFunctionListsSize() const { return functionLists.size(); }

	/// <summary>
	/// Find functionNumber by given name in container.
	/// </summary>
	/// <returns>If exists function return it if not return null.</returns>
	FunctionNumber& find_functionNumber(const string& name) const;
	/// <summary>
	/// Find FunctionList by given name in container.
	/// </summary>
	/// <returns>If exists function return it if not return null.</returns>
	FunctionList& find_functionList(const string& name) const;
	
	void add_functionNumber(const FunctionNumber& funcNumber) { functionNumbers.push_back(funcNumber);}
	void add_functionList(const FunctionList& funcList) { functionLists.push_back(funcList); }

};
*/