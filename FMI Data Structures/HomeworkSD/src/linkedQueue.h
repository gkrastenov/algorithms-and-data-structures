#pragma once
#include <stdexcept>
#include <cassert>

template <class T>
class LinkedQueue
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

	LinkedQueue(const LinkedQueue<T>& queue)
	{
		try {
			copy(queue.begin);
		}
		catch (...) {
			clear();
			throw;
		}
	}

	LinkedQueue<T>& operator=(const LinkedQueue<T>& rhs)
	{
		if (this != &rhs) {
			clear();
			copy(rhs.begin);
		}
		return *this;
	}

	~LinkedQueue()
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
	}

	void dequeue()
	{
		T res = front(); // todo: to be removed
		node* temp = begin;
		begin = begin->next;
		delete temp;
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