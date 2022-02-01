#include "interface.h"

void Function::addList(const LList<double> otherList)
{
	this->list += otherList;
}

void Function::replaceList(const size_t index, LList<double>& list)
{
	if (index == 1)
		this->list.replace_fromPos(index, list);
   else this->list.replace(list);
}

void Function::replaceList(const double start, const double step = 1, const size_t count = 0)
{
	this->list.setIsLoop(true);
	this->list.setStart(start);
	this->list.setStep(step);
	this->list.setCount(count);
}

void Function::copy(const Function& other)
{
	this->name = other.name;
	this->type = other.type;
	this->list = other.list;
}
