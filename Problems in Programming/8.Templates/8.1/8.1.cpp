#include <iostream>
using namespace std;

/*
 Да се реализира шаблон на функция void input([подходящ тип] array,int n),
 която въвежда от клавиатурата стойностите на елементите на
масива array от произволен тип T с големина n.
Какви са допустимите типове T за този шаблон ? Защо функцията е
от тип void ?
Да се реализира и изпълни подходящ тест за функцията
*/
template<class T>
void input(T * array, int n)
{
    for (size_t i = 0; i < n; i++)
    {
        cin >> array[i];
    }
}

template<class T>
void output(T* array, int n)
{
    for (size_t i = 0; i < n; i++)
    {
        cout << array[i] << ' ';

    }
}
int main()
{
    int n;
    cout << "Enter n: ";
    cin >> n;
   
    int* arr= new int[n];

    input(arr, n);
    output(arr, n);
}