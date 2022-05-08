#pragma once
#include <stdexcept>
#include <cassert>

template <class T>
class Queue
{
public:
    virtual ~Queue() {};
    virtual void dequeue() = 0;
    virtual void enqueue(const T& data) = 0;
    virtual const T& front() const = 0;
    virtual bool isEmpty() const = 0;
    virtual bool isFull() const = 0;
};