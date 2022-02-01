#pragma once
#include <iostream>
#include <stdexcept>
#include <cassert>
#include <string>
#include <initializer_list>
#include <set>

template <class T= double>
class LList
{
    struct Node
    {
        Node(const T& val = T(), Node* next = nullptr);
        T data;
        Node* next;
    };

public:
    LList();
    LList(const std::initializer_list<T>& values);
    LList(const LList& other);
    LList(LList&& other);
    LList& operator=(const LList& rhs);
    LList& operator=(const double number);
    LList& operator=(LList&& other);
    LList& operator+=(const LList& other);

    ~LList();

    const T& front() const;
    const T& back() const;
    const T& at(size_t pos) const;

    class iterator;

    iterator begin();
    iterator end() const;

    bool is_empty() const;
    size_t size() const;

    void push_front(const T& val);
    void push_end(const T& val);
    void push_atPos(size_t pos, const T& val);

    void pop_front();
    void pop_end();
    T pop_atPos(size_t pos);

    LList& replace_fromPos(const size_t pos, const LList& other);
    bool eqCompare(const LList<T>& other);
    bool leCompare(const LList<T>& other);
    bool nand();
    void concat(LList<T>& other);

    void setStart(const double st) { start = st; }
    void setStep(const double sp) { step = sp; }
    void setIsLoop(const bool flag) { isLoop = flag; }
    void setCount(const double count) { countLoop = count; }

    void print(std::multiset<T>& set) const;
private:
    Node* head, * tail;
    size_t sz;

    bool isLoop;
    double start;
    double step;
    size_t countLoop;

    void copy(const LList& other);
    void freeHeap();
};

template<class T>
class LList<T>::iterator
{
    friend iterator LList<T>::begin();
    friend iterator LList<T>::end() const;

    LList<T>::Node* ptr;
    iterator(LList<T>::Node* ptr)
        : ptr(ptr) {}

public:
    bool valid() const { return ptr; }
    operator bool() const { return ptr; }

    T& operator*() { return ptr->data; }
    T* operator->() { return &ptr->data; }
    const T& operator*() const { return ptr->data; }
    const T* operator->() const { return &ptr->data; }

    iterator& operator++()
    {
        ptr = ptr->next;
        return *this;
    }
    iterator operator++(int)
    {
        iterator res = *this;
        ++* this;
        return res;
    }

    bool operator == (const iterator& rhs) const
    {
        return ptr == rhs.ptr;
    }
    bool operator != (const iterator& rhs) const
    {
        return !(*this == rhs);
    }
};

template<class T>
inline LList<T>::Node::Node(const T& val, Node* next)
    :data(val), next(next)
{}

template<class T>
inline void LList<T>::copy(const LList<T>& other)
{
    isLoop = other.isLoop;
    start = other.start;
    step = other.step;
    countLoop = other.countLoop;

    Node dummy;
    Node* iter = &dummy;
    Node* curr = other.head;
    while (curr) {
        iter->next = new Node(curr->data);
        iter = iter->next;
        curr = curr->next;
    }
    head = dummy.next;
    tail = head ? iter : nullptr;
    sz = other.sz;
}

template<class T>
inline void LList<T>::freeHeap()
{
    Node* iter = head;
    Node* detach = head;

    for (size_t i = 0; i < sz; ++i) {
        detach = iter;
        iter = iter->next;
        delete detach;
    }

    tail = head = nullptr;
    sz = 0;
}

template<class T>
inline LList<T>::LList()
    :head(nullptr), tail(nullptr), sz(0), isLoop(false), start(0), step(0), countLoop(0) {}

template<class T>
inline LList<T>::LList(const std::initializer_list<T>& values)
    : LList()
{
    for (const T& val : values) {
        push_end(val);
    }
}

template<class T>
inline LList<T>::LList(const LList& other)
    :LList()
{   
     copy(other);
}
/*
template<class T>
inline LList<T>::LList(LList&& other)
    :LList()
{
    std::swap(head, other.head);
    std::swap(tail, other.tail);
    std::swap(size, other.size);
}
*/
template<class T>
inline LList<T>& LList<T>::operator+=(const LList<T>& rhs)
{
    if (this != &rhs) 
    {
        Node* temp = rhs.head;
        while (temp)
        {
            this->push_end(temp->data);
            temp = temp->next;
        }
    }
    return *this;
}
template<class T>
inline LList<T>& LList<T>::operator=(const double number)
{
    freeHeap();
    this->push_front(number);
    return *this;
}

template<class T>
inline LList<T>& LList<T>::operator=(const LList<T>& rhs)
{
    if (this != &rhs) {
        freeHeap();
        copy(rhs);
    }

    return *this;
}

template<class T>
inline LList<T>& LList<T>::operator=(LList<T>&& rhs)
{
    if (this != &rhs) {
        std::swap(head, rhs.head);
        std::swap(tail, rhs.tail);
        std::swap(sz, rhs.sz);
    }
    return *this;
}

template<class T>
inline LList<T>::~LList()
{
    freeHeap();
}

template<class T>
inline const T& LList<T>::at(size_t pos) const
{
    if (is_empty() || pos >= sz) {
        throw std::invalid_argument("Index out of bounds\n");
    }

    if (pos == sz - 1) {
        return tail->data;
    }

    const Node* iter = head;
    for (size_t i = 0; i < pos; ++i, iter = iter->next)
        ;
    return iter->data;
}

template<class T>
inline const T& LList<T>::front() const
{
    return at(0);
}

template< class T>
inline const T& LList<T>::back() const
{
    return at(sz - 1);
}

template<class T>
inline size_t LList<T>::size() const
{
    return sz;
}

template<class T>
inline typename LList<T>::iterator LList<T>::begin()
{
    return iterator(head);
}

template<class T>
inline typename LList<T>::iterator LList<T>::end() const
{
    return iterator(nullptr);
}

template<class T>
inline void LList<T>::push_atPos(size_t pos, const T& val)
{
    if (pos > sz) {
        throw std::invalid_argument("Index out of bounds\n");
    }

    if (pos == 0) {
        head = new Node(val, head);
        if (!sz) tail = head;
    }
    else if (pos == sz) {
        tail->next = new Node(val);
        tail = tail->next;
    }
    else {
        Node* iter = head;
        for (size_t i = 0; i < pos - 1; ++i, iter = iter->next)
            ;

        iter->next = new Node(val, iter->next);
    }

    ++sz;
}

template<class T>
inline void LList<T>::push_front(const T& val)
{
    push_atPos(0, val);
}

template<class T>
inline void LList<T>::push_end(const T& val)
{
    push_atPos(sz, val);
}

template<class T>
inline LList<T>& LList<T>::replace_fromPos(const size_t pos, const LList& other)
{ 
    isLoop = other.isLoop;
    start = other.start;
    step = other.step;
    countLoop = other.countLoop;

    if (other.size() > 1)
    {
        Node* temp = other.head->next;
        while (temp)
        {
            this->push_end(temp->data);
            temp = temp->next;
        }
    }
    return *this;
}

template<class T>
inline bool LList<T>::eqCompare(const LList<T>& other)
{
    if (sz != other.size())
        return false;

    if (sz == 1 && other.size() == 1)
        return this->front() == other.front();

    Node* firstTemp = this->head;
    Node* secondTemp = other.head;
    while (firstTemp != nullptr && secondTemp != nullptr)
    {
        if (firstTemp->data != secondTemp->data)
            return false;
        firstTemp = firstTemp->next;
        secondTemp = secondTemp->next;
   }
    return true;
}

template<class T>
inline bool LList<T>::leCompare(const LList<T>& other)
{
    if (sz == 0 && other.size() == 0)
        return false;

    if (sz != other.size())
        return false;

    if (sz == 1 && other.size() == 1)
        return this->front() < other.front();

    Node* firstTemp = this->head;
    Node* secondTemp = other.head;
    while (firstTemp != nullptr && secondTemp != nullptr)
    {
        if (firstTemp->data >= secondTemp->data)
            return false;
        firstTemp = firstTemp->next;
        secondTemp = secondTemp->next;
    }
    return true;
}

template<class T>
inline bool LList<T>::nand()
{
    return sz != 0;
}

template<class T>
inline void LList<T>::concat(LList<T>& other)
{
    if (other.size() == 0)
        return;

    if (sz == 0 && other.size() == 0)
        return;

    if (sz == 0 && other.size() == 1)
    {
        this->push_front(other.head);
        return;
    }

    if (this->tail != nullptr)  
        this->tail->next = other.head;  
    else this->tail = other.head;
          
    this->tail = other.tail;
    sz += other.size();
}

template<class T>
inline void LList<T>::print(std::multiset<T>& set) const
{
    if (isLoop)
    {
        if (countLoop == 0)
        {
            // always print first 30 number of list
            double curr = start;
            for (size_t i = 0; i < 15; i++)
            {
                set.insert(curr);
                curr += step;
            }
        }
        else {
            double curr = start;
            for (size_t i = 0; i < countLoop; i++)
            {
                set.insert(curr);
                curr += step;
            }
        }
        return;
    }

    if (sz == 0)
        return;

    Node* temp = head;
    while (temp)
    {
        set.insert(temp->data);
        temp = temp->next;
    }
}

template<class T>
inline T LList<T>::pop_atPos(size_t pos)
{
    if (is_empty() || pos >= sz) {
        throw std::invalid_argument("Index out of bounds\n");
    }

    Node* detach = head;

    if (pos == 0) {
        head = head->next;
        if (head == nullptr) {
            tail = nullptr;
        }
    }
    else {
        Node* iter = head;
        for (size_t i = 0; i < pos - 1; ++i, iter = iter->next)
            ;

        detach = iter->next;
        iter->next = detach->next;

        if (detach == tail) {
            tail = iter;
        }
    }
    --sz;
    T store = detach->data;
    delete detach;
    return store;
}

template<class T>
inline void LList<T>::pop_front()
{
    pop_atPos(0);
}

template<class T>
inline void LList<T>::pop_end()
{
    pop_atPos(sz - 1);
}

template<class T>
inline bool LList<T>::is_empty() const
{
    return sz == 0;
}