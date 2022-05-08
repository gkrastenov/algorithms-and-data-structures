#pragma once
#include "queue.h"

template <class T>
class LinkedQueue : public Queue<T>
{
private:
    struct node
    {
        T data;
        node* next;
        node(const T& data, node* n = nullptr)
            : data(data), next(n) {}
    } *begin, * end;

public:
    LinkedQueue()
        : begin(nullptr)
        , end(nullptr)
    {}
    LinkedQueue(const LinkedQueue<T>& rhs);

    LinkedQueue<T>& operator=(const LinkedQueue<T>& rhs);
    ~LinkedQueue();

    void enqueue(const T& data) override;
    void dequeue() override;
    const T& front() const override;
    bool isEmpty() const override;
    bool isFull() const override { return false; }

private:
    void clear();
    void copy(const node* start);
};
