#pragma once
#include "linkedQueue.h"

template <class T>
LinkedQueue<T>::LinkedQueue(const LinkedQueue& queue)
    : begin(nullptr)
{
    try {
        copy(queue.begin);
    }
    catch (...) {
        clear();
        throw;
    }
}

template <class T>
LinkedQueue<T>::~LinkedQueue()
{
    clear();
}

template <class T>
LinkedQueue<T>& LinkedQueue<T>::operator=(const LinkedQueue<T>& rhs)
{
    if (this != &rhs) {
        clear();
        copy(rhs.begin);
    }
    return *this;
}

template <class T>
void LinkedQueue<T>::enqueue(const T& elem)
{
    node* n = new node(elem);
    if (isEmpty()) {
        begin = n;
    }
    else {
        end->next = n;
    }
    end = n;
}

template <class T>
void LinkedQueue<T>::dequeue()
{
    T res = front();
    node* temp = begin;
    begin = begin->next;
    delete temp;
}

template <class T>
const T& LinkedQueue<T>::front() const
{
    if (!isEmpty())
        return begin->data;
    throw std::underflow_error("Empty queue");
}

template <class T>
bool LinkedQueue<T>::isEmpty() const
{
    return begin == nullptr;
}

template <class T>
void LinkedQueue<T>::clear()
{
    while (!isEmpty()) {
        node* keep = begin;
        begin = begin->next;
        delete keep;
    }
}

template <class T>
void LinkedQueue<T>::copy(const node* start)
{
    assert(begin == nullptr && isEmpty());

    while (start) {
        enqueue(start->data);
        start = start->next;
    }
}