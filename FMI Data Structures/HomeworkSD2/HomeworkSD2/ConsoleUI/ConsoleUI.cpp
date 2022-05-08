#include <iostream>
#include "..\src\interface.h"
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>

std::vector<Hierarchy> activesHierarchy;
std::vector<size_t> changedHierarchyIndexs;

const string TheBoss = "Uspeshnia";

enum command {
	help,
	load,
	save,
	find_c,
	num_subordinates,
	manager,
	num_employees,
	overloaded,
	join,
	fire,
	hire,
	salary,
	incorporate,
	modernize,
	exit_c,
	unknown,
};

command convert_command(const string& command)
{
	if (command == "help") return help;
	if (command == "load") return load;
	if (command == "save") return save;
	if (command == "find") return find_c;
	if (command == "num_subordinates") return num_subordinates;
	if (command == "manager") return manager;
	if (command == "num_employees") return num_employees;
	if (command == "overloaded") return overloaded;
	if (command == "join") return join;
	if (command == "fire") return fire;
	if (command == "hire") return hire;
	if (command == "salary") return salary;
	if (command == "incorporate") return incorporate;
	if (command == "modernize") return modernize;
	if (command == "exit") return exit_c;
	return unknown;
}

// Utils
void print_not_founded_hierarchy(const string& name)
{
	std::cout << name
		<< " is an unknown office!\n";
}

void save_as(const string& fileName, const Hierarchy& hierarchy)
{
	std::ifstream ifile;
	ifile.open(fileName);
	if (ifile) {
		// File exists
		std::ofstream ofs;
		ofs.open(fileName, std::ofstream::out | std::ofstream::trunc); // clear the content
		ofs.close();

		std::ofstream fileStream(fileName);
		string data = hierarchy.print();
		fileStream << data;
		fileStream.close();
	}
	else {
		ifile.close();
		std::ofstream fileStream(fileName);
		string data = hierarchy.print();
		fileStream << data;
		fileStream.close();
	}
}
// Functions for command
void print_help_command()
{
	// help
	std::cout << "help - displays a list of supported commands with brief help information about them\n";
	// load
	std::cout << "load object_name file_name - loads hierarchy data from a file with the specified name and creates a "
		<< "tree associated with object_name.This name should consist only of lowercase and uppercase Latin letters, numbers, and an "
		<< "underscore.Then everything to the end of the line is the name of the file from which you need to read the data in the format "
		<< "described above.If the file name is missing, it is assumed that you read the data from the standard input, until you meet the "
		<< "end of the file(ctrl + z / ctrl + d)\n";
	// save
	std::cout << "save object_name file_name - saves the hierarchy information of the specified object to a file with the specified name."
		<< "If the file name is empty, the information is output to standard output\n";
	// find
	std::cout << "find object_name employee_name - checks if an employee with the specified name exists in the specified object\n";
	// num_subordinates
	std::cout << "num_subordinates object_name employee_name - displays the number of direct subordinates of the given employee in the specified object\n";
	// manager
	std::cout << "manager object_name employee_name - displays the name of the manager of the given employee in the specified object\n";
	// overloaded
	std::cout << "overloaded object_name - displays the number of employees in the specified object, for which the number of subordinates (direct or not) is greater than 20\n";
	// join
	std::cout << "join object_name_1 object_name_2 object_name_result - merges the two submitted objects into a new object named object_name_result\n";
	// fire
	std::cout << "fire object_name_employee_name - removes the employee from the respective object\n";
	// hire
	std::cout << "hire name_of_object name_of_employee name_of_leader - appoints the employee in the respective site as subordinate to the submitted manager\n";
	// salary
	std::cout << "salary object_name employee_name - displays the employee's salary\n";
	// incorporate
	std::cout << "incorporate object_name - incorporates the company; the operation is applied to the object object_name\n";
	// modernize
	std::cout << "modernize object_name - modernizes the company; the operation is applied to the object object_name\n";
	// exit
	std::cout << "exit - terminates the program. For all new or changed objects after loading, ask the user if he wants to save them in a file\n";
}

void find_command()
{
	string inputName, inputEmployee;
	std::cin >> inputName >> inputEmployee;
	for (size_t i = 0; i < activesHierarchy.size(); i++)
	{
		if (activesHierarchy[i].getName() == inputName)
		{
			if (activesHierarchy[i].find(inputEmployee)) 
				 std::cout << inputEmployee << " is employed in Lozenec.\n";
			else std::cout << inputEmployee << " not founded in Lozenec.\n";	

			return;
		}
	}
	print_not_founded_hierarchy(inputName);
}

void num_subordinates_command()
{
	string inputName, inputEmployee;
	std::cin >> inputName >> inputEmployee;
	for (size_t i = 0; i < activesHierarchy.size(); i++)
	{
		if (activesHierarchy[i].getName() == inputName)
		{
			int numbers = activesHierarchy[i].num_subordinates(inputEmployee);
			if (numbers != 0)
				 std::cout << inputEmployee << " has " << numbers << " subordinates.\n";
			else std::cout << inputEmployee << " has no subordinates.\n";

			return;
		}
	}
	print_not_founded_hierarchy(inputName);
}

void manager_command()
{
	string inputName, inputEmployee;
	std::cin >> inputName >> inputEmployee;
	for (size_t i = 0; i < activesHierarchy.size(); i++)
	{
		if (activesHierarchy[i].getName() == inputName)
		{
			if (inputEmployee == TheBoss)
			{
				std::cout << TheBoss << " does not have manager.\n";
				return;
			}

			string nameManager = activesHierarchy[i].manager(inputEmployee);
			if (nameManager != "")
				std::cout << "The manager of " << inputEmployee << " is " << nameManager << ".\n";		
			else std::cout << "There is no " << inputEmployee << " in Lozenec.\n";
			
			return;
		}
	}
	print_not_founded_hierarchy(inputName);
}

void num_employees_command()
{
	string inputName;
	std::cin >> inputName;
	for (size_t i = 0; i < activesHierarchy.size(); i++)
	{
		if (activesHierarchy[i].getName() == inputName)
		{
			int number = activesHierarchy[i].num_employees();
			if (number != 0) 
				std::cout << "There are " << number << " employees in " << inputName << ".\n";
			return;
		}
	}
	print_not_founded_hierarchy(inputName);
}

void overloaded_command()
{
	string inputName;
	std::cin >> inputName;
	for (size_t i = 0; i < activesHierarchy.size(); i++)
	{
		if (activesHierarchy[i].getName() == inputName)
		{
			int number = activesHierarchy[i].num_overloaded();
			if (number != 0) 
				std::cout << number << " overloaded employees in " << inputName << ".\n";		
			else std::cout << "No overloaded employees in " << inputName << ".\n";
			
			return;
		}
	}
	print_not_founded_hierarchy(inputName);
}

void incorporate_command()
{
	string inputName;
	std::cin >> inputName;
	for (size_t i = 0; i < activesHierarchy.size(); i++)
	{
		if (activesHierarchy[i].getName() == inputName)
		{
			if (std::count(changedHierarchyIndexs.begin(), changedHierarchyIndexs.end(), i) == 0)
				changedHierarchyIndexs.push_back(i);

			activesHierarchy[i].incorporate();
			std::cout << inputName << " incorporated.\n";
			return;
		}
	}
	print_not_founded_hierarchy(inputName);
}

void modernize_command()
{
	string inputName;
	std::cin >> inputName;
	for (size_t i = 0; i < activesHierarchy.size(); i++)
	{
		if (activesHierarchy[i].getName() == inputName)
		{
			if (std::count(changedHierarchyIndexs.begin(), changedHierarchyIndexs.end(), i) == 0)
				changedHierarchyIndexs.push_back(i);

			activesHierarchy[i].modernize();
			std::cout << inputName << " modernized.\n";
			return;
		}
	}
	print_not_founded_hierarchy(inputName);
}

void hire_command() 
{
	string inputName, manager, employee;
	std::cin >> inputName >> employee >> manager;
	for (size_t i = 0; i < activesHierarchy.size(); i++)
	{
		if (activesHierarchy[i].getName() == inputName)
		{
			if (activesHierarchy[i].hire(employee, manager))
			{
				if (std::count(changedHierarchyIndexs.begin(), changedHierarchyIndexs.end(), i) == 0)
					changedHierarchyIndexs.push_back(i);

				std::cout << employee << " was hired.\n";
			}
			else {
				std::cout << employee << " was not hired.\n";
			}
			return;
		}
	}
	print_not_founded_hierarchy(inputName);
}

void fire_command() {
	string inputName, employee;
	std::cin >> inputName >> employee;
	for (size_t i = 0; i < activesHierarchy.size(); i++)
	{
		if (activesHierarchy[i].getName() == inputName)
		{
			if (activesHierarchy[i].fire(employee))
			{
				if (std::count(changedHierarchyIndexs.begin(), changedHierarchyIndexs.end(), i) == 0)
					changedHierarchyIndexs.push_back(i);
				
				std::cout << employee << " was fired.\n";
			}
			else {
				std::cout << employee << " was not fired.\n";
			}
			return;
		}
	}
	print_not_founded_hierarchy(inputName);
}

void salary_command() {
	string inputName, employee;
	std::cin >> inputName >> employee;
	for (size_t i = 0; i < activesHierarchy.size(); i++)
	{
		if (activesHierarchy[i].getName() == inputName)
		{
			unsigned int value = activesHierarchy[i].getSalary(employee);
			if (value == -1) 
				std::cout << "Not founded employee with name: " << employee << ".\n";
			else std::cout << "The salary is " << value << " BGN.\n";

			return;
		}
	}
	print_not_founded_hierarchy(inputName);
}

Hierarchy load_command()
{
	string objectName, fileName;
	std::cin >> objectName;
	std::getline(std::cin, fileName);
	if (fileName == ""){
		string inputData = "";
		string inputLine = "";
		while (std::getline(std::cin, inputLine))
		{
			if (inputLine == "^Z" || inputLine == "^D")
				break;

			inputData += inputLine;
			inputData += "\n";
		}
		Hierarchy toBeCreated(inputData);
		toBeCreated.setName(objectName);
		std::cout << toBeCreated.getName() << " loaded successfully!\n";
		return toBeCreated;
	}
	else {
		string fileData = "";
		string line;
		std::ifstream myfile(fileName);
		if (myfile.is_open())
		{
			while (getline(myfile, line))
			{
				fileData += line + "\n";
			}
			myfile.close();

			Hierarchy toBeCreated(fileData);
			toBeCreated.setName(objectName);
			std::cout << toBeCreated.getName() << " loaded successfully!\n";
			return toBeCreated;
		}
		else {
			std::cout << "Not founded file.\n";
		}
	}
	throw "Not loaded";
}

void join_command()
{
	size_t leftHierarchyIndex = -1;
	size_t rigthHierarchyIndex = -1;

	string leftHierarchyName, rightHierarchyName, joinHierarchyName;
	std::cin >> leftHierarchyName >> rightHierarchyName >> joinHierarchyName;
	for (size_t i = 0; i < activesHierarchy.size(); i++)
	{
		if (activesHierarchy[i].getName() == leftHierarchyName)
			leftHierarchyIndex = i;

		if (activesHierarchy[i].getName() == rightHierarchyName)
			rigthHierarchyIndex = i;

		if (leftHierarchyIndex != -1 && rigthHierarchyIndex != -1)
			break;
	}

	if (leftHierarchyIndex != -1 && rigthHierarchyIndex != -1)
	{
		Hierarchy joinHierarchy = activesHierarchy[leftHierarchyIndex].join(activesHierarchy[rigthHierarchyIndex]);
		joinHierarchy.setName(joinHierarchyName);
		activesHierarchy.push_back(joinHierarchy);
		std::cout << joinHierarchyName << " created.\n";
	}
	else {
		if (leftHierarchyIndex == -1 && rigthHierarchyIndex == -1)
		{
			std::cout << "Hierarchies with names: " << leftHierarchyName << " and " << rightHierarchyName << "unknown\n";
			return;
		}
		else {
			if(leftHierarchyIndex == -1)
				print_not_founded_hierarchy(leftHierarchyName);

			if (rigthHierarchyIndex == -1)
				print_not_founded_hierarchy(rightHierarchyName);
		}
	}
}

void save_command()
{
	size_t saveHierarchyIndex = -1;

	string saveHierarchyName, fileName;
	std::cin >> saveHierarchyName;
	std::getline(std::cin, fileName);
	for (size_t i = 0; i < activesHierarchy.size(); i++)
	{
		if (activesHierarchy[i].getName() == saveHierarchyName){
			saveHierarchyIndex = i;
			break;
		}
	}

	if (fileName == ""){
		std::cout<< activesHierarchy[saveHierarchyIndex].print();
		return;
	}

	if (saveHierarchyIndex != -1)
	{
		save_as(fileName, activesHierarchy[saveHierarchyIndex]);
		std::cout << saveHierarchyName << " saved.\n";

		size_t changedHierarchyIndex = -1;

		if (changedHierarchyIndexs.size() == 0)
			return;

		for (size_t i = 0; i < changedHierarchyIndexs.size(); i++)
		{
			if (changedHierarchyIndexs[i] == saveHierarchyIndex)
			{
				changedHierarchyIndex = i;
				break;
			}
		}

		if(changedHierarchyIndex != -1)
			changedHierarchyIndexs.erase(changedHierarchyIndexs.begin() + changedHierarchyIndex);
		
	}

	if(saveHierarchyIndex == -1)
		print_not_founded_hierarchy(saveHierarchyName);
}

void exit_command()
{
	for (size_t i = 0; i < changedHierarchyIndexs.size(); i++)
	{
		std::cout << activesHierarchy[changedHierarchyIndexs[i]].getName() << " is modified, but not saved.\n";
		std::cout << "Enter file name to save it :\n";
		string fileName;
		std::cin >> fileName;
		save_as(fileName, activesHierarchy[changedHierarchyIndexs[i]]);
		std::cout << activesHierarchy[changedHierarchyIndexs[i]].getName() << " saved.\n";
	}

	std::cout << "Goodbye!\n";
}
int main()
{
	command console_command;
	do
	{
		string input;
		std::cin >> input;
		console_command = convert_command(input);

		switch (console_command)
		{
		 case help:
			 print_help_command();
			break;
		 case load: 
			 activesHierarchy.push_back(load_command());
			 break;
		 case save:
			 if (!activesHierarchy.empty())
				 save_command();
			 else std::cout << "Not loaded Hierarchy";
			 break;
		 case find_c:
			 if (!activesHierarchy.empty())
				 find_command();
			 else std::cout << "Not loaded Hierarchy";		 
			 break;
		 case num_subordinates:
			 if (!activesHierarchy.empty())
				 num_subordinates_command();
			 else std::cout << "Not loaded Hierarchy";
			 break;
		 case manager:
			 if (!activesHierarchy.empty())
				 manager_command();
			 else std::cout << "Not loaded Hierarchy";			 
			 break;
		 case num_employees:
			 if (!activesHierarchy.empty())
				 num_employees_command();
			 else std::cout << "Not loaded Hierarchy";			 
			 break;
		 case overloaded:
			 if (!activesHierarchy.empty())
				 overloaded_command();
			 else std::cout << "Not loaded Hierarchy";		 
			 break;
		 case join:
			 if (!activesHierarchy.empty())
				 join_command();
			 else std::cout << "Not loaded Hierarchy";
			 break;
		 case fire:
			 if (!activesHierarchy.empty())
				 fire_command();
			 else std::cout << "Not loaded Hierarchy";		 
			 break;
		 case hire:
			 if (!activesHierarchy.empty())
				 hire_command();
			 else std::cout << "Not loaded Hierarchy";
			 break;
		 case salary:
			 if (!activesHierarchy.empty())
				 salary_command();
			 else std::cout << "Not loaded Hierarchy";
			 break;
		 case incorporate:
			 if (!activesHierarchy.empty())
				 incorporate_command();
			 else std::cout << "Not loaded Hierarchy";
			 break;
		 case modernize:
			 if (!activesHierarchy.empty())
				 modernize_command();
			 else std::cout << "Not loaded Hierarchy";
			 break;
		 case exit_c:
			 exit_command();
			 break;
		default:
			std::cout << "Not founded command: " << input << std::endl;
			break;
		}
	} while (console_command != exit_c);
}