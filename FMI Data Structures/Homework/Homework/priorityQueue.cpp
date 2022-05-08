#pragma once
#include "priorityQueue.h"

template <class T>
PriorityQueue<T>::PriorityQueue(const PriorityQueue& queue)
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
PriorityQueue<T>& PriorityQueue<T>::operator=(const PriorityQueue<T>& rhs)
{
    if (this != &rhs) {
        clear();
        copy(rhs.begin);
    }
    return *this;
}

template <class T>
PriorityQueue<T>::~PriorityQueue()
{
	clear();
}

template <class T>
void PriorityQueue<T>::enqueue(const T& elem)
{
	if (begin == nullptr)
	{
		begin = (node*)calloc(0, sizeof(node));

		if (begin != nullptr)
		{
			begin->data = elem;
			end = begin;
		}
		else
		{
			throw std::underflow_error("Memory Error");
		}
	}
	else
	{
		node* newNode = (node*)calloc(0, sizeof(node));
		node* nodePtr;

		if (newNode != nullptr && nodePtr != nullptr)
		{
			newNode->data = elem;
			nodePtr = begin;

			while (nodePtr->next != nullptr)
			{
				nodePtr = nodePtr->next;
			}

			nodePtr->next = newNode;
			end = newNode;
		}
		else
		{
			throw std::underflow_error("Memory Error");

		}
	}
	sort();
}

template <class T>
void PriorityQueue<T>::dequeue()
{
	if (begin != nullptr)
	{
		node* temp = begin;
		begin = begin->next;
		delete temp;
	}
}

template <class T>
const T& PriorityQueue<T>::front() const
{
    if (!isEmpty())
        return begin->data;
    throw std::underflow_error("Empty queue");
}

template <class T>
bool PriorityQueue<T>::isEmpty() const
{
    return begin == nullptr;
}

template <class T>
void PriorityQueue<T>::clear()
{
    while (!isEmpty()) {
        node* keep = begin;
        begin = begin->next;
        delete keep;
    }
}

template <class T>
void PriorityQueue<T>::copy(const node* start)
{
    assert(begin == nullptr && isEmpty());

    while (start) {
        enqueue(start->data);
        start = start->next;
    }
}

template <class T>
void PriorityQueue<T>::sort()
{
	node* i;
	node* j;
	node* max;
	T temp;

	for (i = begin; i != nullptr && i->next != nullptr; i = i->next)
	{
		max = i;

		for (j = i->next; j != nullptr; j = j->next)
		{
			if ((j->data) > (max->data))
			{
				max = j;
			}
		}

		if (max != i)
		{
			temp = max->data;
			max->data = i->data;
			i->data = temp;
		}
	}

	i = nullptr;
	j = nullptr;
	max = nullptr;
}