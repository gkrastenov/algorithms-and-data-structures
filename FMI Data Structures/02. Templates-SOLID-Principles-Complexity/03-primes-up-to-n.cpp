#include <iostream>
using namespace std;

/*
* Напишете алгоритъм, който извежда първите n прости
числа.
*/.

bool isPrime(int number)
{
    if (number <= 1)
        return false;

    for (int i = 2; i < number; i++)
        if (number % i == 0)
            return false;

    return true;
}

int main() {
    int n;
    cin >> n;

    if (n < 0)
    {
        throw "Invalid input";

    }

    int number = 2;
    while (n != 0)
    {
        if (isPrime(number))
        {
            n--;
            cout << number << endl;
            number++;
        }
        number++;
    }
}