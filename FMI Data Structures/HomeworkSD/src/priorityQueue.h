#pragma once
#include <stdexcept>
#include <cassert>

template <class T>
class PriorityQueue {

private:
	struct node
	{
		T data;
		node* next;
		node(const T& data, node* n = nullptr)
			: data(data), next(n) {}
	} *begin, * end;

public:
	PriorityQueue()
		: begin(nullptr)
		, end(nullptr)
	{}
	PriorityQueue(const PriorityQueue<T>& queue) : begin(nullptr)
	{
		try {
			copy(queue.begin);
		}
		catch (...) {
			clear();
			throw;
		}
	}

	PriorityQueue<T>& operator=(const PriorityQueue<T>& rhs)
	{
		if (this != &rhs) {
			clear();
			copy(rhs.begin);
		}
		return *this;
	}

	~PriorityQueue()
	{
		clear();
	}

	void enqueue(const T& elem)
	{
		node* n = new node(elem);
		if (isEmpty()) {
			begin = n;
		}
		else {
			end->next = n;
		}
		end = n;

		sort();
	}

	void dequeue()
	{
		if (begin != nullptr)
		{
			node* temp = begin;
			begin = begin->next;
			delete temp;
		}
	}
	const T& front() const
	{
		if (!isEmpty())
			return begin->data;
		throw std::underflow_error("Empty queue");
	}
	bool isEmpty() const
	{
		return begin == nullptr;
	}

private:
	void sort()
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
	void clear()
	{
		while (!isEmpty()) {
			node* keep = begin;
			begin = begin->next;
			delete keep;
		}
	}
	void copy(const node* start)
	{
		assert(begin == nullptr && isEmpty());

		while (start) {
			enqueue(start->data);
			start = start->next;
		}
	}
};