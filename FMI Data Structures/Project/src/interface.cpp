#include "interface.h"

void Function::addList(const LList<double> otherList)
{
	this->list += otherList;
}

void Function::replaceList(const size_t index, const LList<double>& list)
{
	this->list.replace_fromPos(index, list);
}

void Function::copy(const Function& other)
{
	this->name = other.name;
	this->type = other.type;
	this->list = other.list;
}
