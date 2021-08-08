#include <iostream>

template <typename T>
struct box
{
    box(const T _data, box<T>* _next)
    {
        data = _data;
        next = _next;
    }
    box()
    {
        data = 0;
        next = nullptr;
    }

    T data;
    box<T>* next;
};
template <typename T>
class LList
{
public:
    LList()
    {
        first = nullptr;
    }

    // 12.2. Да се реализира конструктор с два аргумента x и y от тип int. Конструкторът създава списък 
    // с елементи x, x + 1, ..., y, при положение, че x ≤ y.
    LList(const int x, const int y)
    {
        for (size_t i = x; i <=y; i++)
        {
            push(i);
        }
    }
    LList(const LList<T>& list)
    {
        box<T>* current = list.first;
        while (current != nullptr)
        {
            push(current->data);
            current = current->next;
        }
    }

    void push(const T& _data)
    {
        box<T>* temp = new box<T>();
        temp->data = _data;
        temp->next = first;
        first = temp;
    }

    // Да се реализира метод LList<T>::push_back за добавяне на елемент от
    // тип T към края на списъка
    void push_back(const T& _data)
    {
        if (first == nullptr)
        {
            box<T>* temp = new box<T>();
            temp->data = _data;
            first = temp;
            return;
        }

        box<T>* current = first;
        while (current->next != nullptr)
        {
            current = current->next;
        }

        box<T>* temp = new box<T>();
        temp->data = _data;
        current->next = temp;
    }

    // 12.4. Да се реализира метод оператор LList<T>::operator += за добавяне на
    // елемент от тип T към края на списъка.
    LList& operator +=(const T& _data)
    {
        this->push_back(_data);
        return *this;
    }

    // 12.5.Да се реализира метод LList<T>::get_ith(int n) за намиране на n - тия
    // поред елемент на списъка
    void get_inth(int n)
    {
        box<T>* current = first;
        while (current != nullptr)
        {
            if (n==0)
            {
                std::cout << current->data << std::endl;
                return;
            }
            current = current->next;
            n--;
        }
        std::cout << "N is too big" << std::endl;
    }

    void pop() 
    {
        box<T>* temp = first->next;
        delete[] first;
        first = temp;
    }

    // 12.7. Да се реализира метод LList::removeAll (x), който изтрива всички срещания на елемента x от списъка
    void removeAll(const T& x)
    {
        box<T>* current = first;
        while (first->data == x)
        {
            first = first->next;
        }
        while (current->next != nullptr)
        {
            if (current->next->data == x)
            {
                current->next = current->next->next;
            }
            else
            {
                current = current->next;
            }
        }
    }

    // 12.10. Да се дефинират оператори LList<T>::operator+= и LList<T>::operator+,
    // съответни на методите append и concat.
    LList<T> operator +(const LList<T>& list)
    {
        return this->concat(list);
    }

    void operator +=(const LList<T>& list)
    {
        this->append(list);
    }

    // 12.15. За шаблона на клас LList да се разработят контруктор за копиране,
    // оператор за присвояване и деструктор.
    LList<T>& operator = (const LList<T>& list)
    {
        if (this != &list)
        {
            // deleteing all elements
            box<T>* current = first;
            while (current != nullptr) {
                box<T>* next = current->next;
                delete current;
                current = next;
            }
            first = nullptr;

            // adding all new elements
            current = list.first;
            while (current != nullptr) {
                this->push_back(current->data);
                current = current->next;
            }
        }

        return *this;
    }

    // 12.8. Да се реализира метод l1.append(l2), която добавя към края на списъка
    // l1 всички елементи на списъка l2.
    void append(const LList<T>& list)
    {
        box<T>* current = list.first;
        while (current != nullptr)
        {
            this->push_back(current->data);
            current = current->next;
        }
    }

    // 12.9. Да се реализира метод LList<T>::concat, който съединява два списъка
    // в нов, трети списък.Т.е.l1.concat(l2) създава и връща нов списък от
    // елементите на l1, следвани от елементите на l2.
    LList<T> concat(const LList<T>& list)
    {
        LList<T> newList = LList<T>();
        
        box<T>* current = first;
        while (current != nullptr)
        {
            newList.push(current->data);
            current = current->next;
        }
        
        newList.append(list);
        return newList;
    }

    void print() const
    {
        box<T>* current = first;
        while (current!= nullptr)
        {
            std::cout << current->data<<' ';
            current = current->next;
        }
        std::cout << std::endl;
    }

    // 12.1. Да се реализира метод int LList<T>::count(int x), който преброява
    // колко пъти елементът x се среща в списъка.
    int count(const int x)
    {
        int result = 0;

        box<T>* current = first;
        while (current != nullptr)
        {
            if (current->data == x)
            {
                result++;
            }
            current = current->next;
        }

        return result;
    }

    // 12.12. Да се дефинира метод LList::reverse, който обръща реда на елементите на списъка. 
    // Например, списъкът с елементи 1, 2, 3 ще се преобразува до списъка с елементи 3, 2, 1.
    void reverse()
    {
        // Initialize current, previous and next pointers
        box<T>* current = this->first;
        box<T>* prev = nullptr;
        box<T>* next = nullptr;

        while (current != nullptr) {
            next = current->next;

            current->next = prev;
            prev = current;
            current = next;
        }

        this->first = prev;
    }

    ~LList()
    {
        box<T>* current = first;
        while (current != nullptr) {
            box<T>* next = current->next;
            delete current;
            current = next;
        }
        first = nullptr;
    }

private:
    box<T>* first;
};

int main()
{
    LList<int> l1 = LList<int>();
    l1.push(1);
    l1.push(2);
    l1.push(3);
    l1.print();
    l1.pop();
    l1.print();
    std::cout << l1.count(1) << std::endl;

    LList<int> l2 = LList<int>(5,10);
    l2.print();
    l2.push_back(100);
    l2.print();
    l2 += 700;
    l2.print();

    l2.get_inth(500);
    l2.get_inth(6);

    l2.push(100);
    l2.removeAll(2);
    std::cout << l2.count(2) << std::endl; // -> should return 0

    l2.print();
    l1.print();

    l2 += l1; // equal to l2.append(l1)
    l2.print();

    l2.print();
    l1.print();

    LList<int> l3 = l2 + l1; // equal to l2.concat(l1);
    l3.print();

    std::cout << "Reversing: " << std::endl;
    l2.print();
    l2.reverse();
    l2.print();

    LList<int> l4 = LList<int>();
    l4 = l2;
    l4.print();
}