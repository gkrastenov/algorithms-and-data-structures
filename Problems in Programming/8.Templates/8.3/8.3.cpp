#include <iostream>
using namespace std;
/*
Да се реализира шаблон на функция bool member ([подходящ тип] array,int n, [подходящ тип]x), 
която проверява дали x е елемент на масива
array от произволен тип T с големина n.
Има ли в C++ тип T, който не е съвместим с този шаблон?
Да се реализира и изпълни подходящ тест за функцията.
*/

template<class T>
void input(T* array, int n)
{
    for (size_t i = 0; i < n; i++)
    {
        cin >> array[i];
    }
}

template<class T>
bool member(T* array, int n, T x)
{
    for (size_t i = 0; i < n; i++)
    {
        if (array[i] == x )
        {
            return true;
        }
    }
    return false;
}
int main()
{
    int n;
    cout << "Enter n: ";
    cin >> n;

    int* arr = new int[n];

    input(arr, n);

    int x;
    cout << "Enter x: ";
    cin >> x;
    cout << boolalpha << member(arr, n, x);
}