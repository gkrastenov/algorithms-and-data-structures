#include "container.h"

FunctionNumber& FunctionContainer::find_functionNumber(const string& name) const
{
	for (size_t i = 0; i < functionNumbers.size(); i++)
	{
		if (functionNumbers[i].getName() == name)
			return functionNumbers[i];
	}
	return FunctionNumber();
}

FunctionList& FunctionContainer::find_functionList(const string& name) const
{
	for (size_t i = 0; i < functionLists.size(); i++)
	{
		if (functionLists[i].getName() == name)
			return functionLists[i];
	}
	return FunctionList();
}