#include "hierarchyPair.h"


ManagerEmployeePair::ManagerEmployeePair() 
	: manager()
	, employee()
{

};

ManagerEmployeePair::ManagerEmployeePair(const string& manager, const string& employee)
{
	this->manager = manager;
	this->employee = employee;
}

ManagerEmployeePair::~ManagerEmployeePair()
{

}

const string& ManagerEmployeePair::getManager() const
{
	return this->manager;
}

const string& ManagerEmployeePair::getEmployee() const
{
	return this->employee;
}

void ManagerEmployeePair::setManager(const string& manager)
{
	this->manager = manager;
}

void ManagerEmployeePair::setEmployee(const string& employee)
{
	this->employee = employee;
}
