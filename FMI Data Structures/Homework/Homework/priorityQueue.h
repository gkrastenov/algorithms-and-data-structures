#pragma once
#include "queue.h"

template <class T>
class PriorityQueue : public Queue<T> {

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
	PriorityQueue(const PriorityQueue<T>& rhs);

	PriorityQueue<T>& operator=(const PriorityQueue<T>& rhs);
	~PriorityQueue();

	void enqueue(const T& data) override;
	void dequeue() override;
	const T& front() const override;
	bool isEmpty() const override;
	bool isFull() const override { return false; }

private:
	void sort();
	void clear();
	void copy(const node* start);
};