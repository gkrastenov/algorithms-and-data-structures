#include <iostream>
using namespace std;
/*
Да се реализира шаблон на функция bool ordered([подходящ тип] array, int n), 
която проверява дали елементите на масива array от
произволен тип T с големина n образуват монотонно - растяща редица
спрямо релацията < .
Какви са допустимите типове T за този шаблон ?
Да се реализира и изпълни подходящ тест за функцията
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
bool ordered(T* array, int n)
{
    if (n == 1)
    {
        return true;
    }

    bool result = true;
    for (size_t i = 0; i < n-1; i++)
    {
        if (array[i] > array[i+1])
        {
            result = false;
        }
    }

    return result;
}
int main()
{
    int n;
    cout << "Enter n: ";
    cin >> n;

    int* arr = new int[n];

    input(arr, n);
    cout << boolalpha << ordered(arr, n);
}
