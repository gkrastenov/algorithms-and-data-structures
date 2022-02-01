#include <iostream>
#include <string>
#include <sstream>     
#include <fstream>

#include "../src/compilator.h"

bool runCode(Compilator& compilator, std::stringstream& ss)
{
	try
	{
		compilator.compileCode(ss);
		auto vector = compilator.output();

		std::cout << "> ";

		if (compilator.isListCompiledFunction() && compilator.getCanBePrint())
		{
			std::cout << '[';
			for (size_t i = 0; i < vector.size() - 1; i++)
			{
				std::cout << vector[i];
				std::cout << ' ';
			}
			std::cout << vector[vector.size() - 1];
			std::cout << ']' << std::endl;
		}
		else {
			if (vector.size() == 1) {
				std::cout << vector[0] << std::endl;
			}
			else {
				for (size_t i = 0; i < vector.size() - 1; i++)
				{
					std::cout << vector[i];
					std::cout << ' ';
				}
				std::cout << vector[vector.size() - 1] << std::endl;
			}
		}
	}
	catch (const std::exception&)
	{
		std::cout << compilator.printErrorMessage() << std::endl;
		return false;
	}
	return true;
}

void console(Compilator& compilator)
{
	std::cout << "Write 'exit' for exiting the program\n";

	std::string input;
	std::cin.ignore();
	std::getline(std::cin, input);

	while (input != "exit")
	{
		std::stringstream ss(input);
		if (runCode(compilator, ss) == false)
			break;
		else std::getline(std::cin, input);
	}
}
void file(Compilator& compilator)
{
	std::cout << "File name:\n";
	std::string fileName;
	std::cin.ignore();
	std::getline(std::cin, fileName);

	std::ifstream file(fileName);
	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line))
		{
			std::cout << line << std::endl;
			std::stringstream ss(line);
			if (runCode(compilator, ss) == false)
				break;
		}
		file.close();
	}
	else {
		std::cout << "Can not be open this file.\n";
	}
}

int main()
{
	Compilator compilator;

	std::cout << "Write 'c' if you want to use console or write 'f' for file: c/f \n";
	char symbol;
	std::cin >> symbol;

	try
	{
		switch (symbol)
		{
		case 'c': console(compilator);
			break;
		case 'f': file(compilator);
			break;
		default:
			std::cout << "Enter correct symbol: c or f\n";
			break;
		}
	}
	catch (const std::exception&)
	{
		std::cout << "Code can not be compiled\n";
	}
}