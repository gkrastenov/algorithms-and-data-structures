#include <iostream>

/**
 * Напишете функция, която разлага число на прости делители
 */

int FindNextDevisor(int number)
{
	if (number == 1)
	{
		std::cout << number << std::endl;
		return 0;
	}
	int div = 2;

	while (number % div != 0)
	{
		div++;
	}

	std::cout << div <<" * ";
	return number / div;
}

int main() {
	int number;
	std::cin >> number;

	std::cout << "Divisors of " << number << " are: ";
	while (number != 0)
	{
		number = FindNextDevisor(number);
	}
}