#pragma once
#include <string>

using std::string;

class ManagerEmployeePair
{
public:
	ManagerEmployeePair()
		: manager()
		, employee()
	{}
	ManagerEmployeePair(const string& manager, const string& employee)
	{
		this->manager = manager;
		this->employee = employee;
	}
	ManagerEmployeePair(const string& data) 
	{
		set_empty_values();
		create_pair(data);
	};
	~ManagerEmployeePair()
	{

	}

	const string& getManager() const { return this->manager; }
	const string& getEmployee() const { return this->employee; }
	void setManager(const string& managerData) { this->manager = validate_input(managerData); }
	void setEmployee(const string& employeeData) { this->employee = validate_input(employeeData); }
private:
	string manager{}, employee{};
	const char DELIMETAR = '-';
private:

	void create_pair(const string& data)
	{
		if (data == "") {
			throw "Can not be created pair, data was empty";
		}

		std::string::size_type pos = 0;
		std::string::size_type prev = 0;
		if (pos = data.find(DELIMETAR, prev)) {			
			if (manager != "")
				throw "Can not be created two times manager";

			string inputManager = data.substr(prev, pos - prev);
			setManager(inputManager);

			prev = pos + 1;

			if (employee != "")
				throw "Can not be created two times manager";

			string inputEmployee = data.substr(prev);
			setEmployee(inputEmployee);
		}
		else {
			throw "Can not be created pair, delimetar: - was miss.";
		}
	}
	void set_empty_values() 
	{
		manager = employee = "";
	}
	string validate_input(const string& data)
	{
		bool startAlphabetic = false;
		bool endAlphabetic = false;
		size_t index = 0;
		string result = "";
		while (index != data.size())
		{
			if (data[index] != ' ' && data[index] != '-')
			{
				if (startAlphabetic && endAlphabetic) 
					throw "More than one word can be exist in name";
				
				if (!startAlphabetic)
					startAlphabetic = true;

				result += data[index];
			}
			else {
				if(startAlphabetic)
						endAlphabetic = true;

				if(data[index] == '-')
					throw "Too much delimetars";						
			}
			index++;
		}
		return result;;
	}
};