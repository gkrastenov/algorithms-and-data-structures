#include "worker.h"
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

struct MyClient : Client {
private:
	// The index of the client
	int index;
public:
	MyClient(const int index, const Client& client);
	MyClient() {
		index = -1;
	}
	bool operator>(const MyClient& other)
	{
		if (this->arriveMinute + this->maxWaitTime == other.arriveMinute + other.maxWaitTime)
		{
			if (this->index < other.index)
			{
				return true;
			}
			return false;
		}

		if (this->arriveMinute + this->maxWaitTime < other.arriveMinute + other.maxWaitTime) {
			return true;
		}

		return false;
	}

	int getIndex() const;
};

MyClient::MyClient(const int index, const Client& client)
{
	this->index = index;
	this->arriveMinute = client.arriveMinute;
	this->banana = client.banana;
	this->schweppes = client.schweppes;
	this->maxWaitTime = client.maxWaitTime;
}


int MyClient::getIndex() const
{
	return index;
}


struct MyStore : Store {

	ActionHandler* actionHandler = nullptr;

	int bananasQuantity = 0;
	int schweppesQuantity = 0;
	int workerCount = 0;

	bool haveSentWorkersForBananas = false;
	bool haveSentWorkersForSchweppes = false;

	LinkedQueue<MyClient> clients;

	void setActionHandler(ActionHandler* handler) override {
		actionHandler = handler;
	}

	void init(int workerCount, int startBanana, int startSchweppes) override {
		this->workerCount = workerCount;
		this->bananasQuantity = startBanana;
		this->schweppesQuantity = startSchweppes;
	}

	void addClients(const Client* clients, int count) override {
		for (size_t i = 0; i < count; i++)
		{
			MyClient newClient(i, clients[i]);
			this->clients.enqueue(newClient);
		}
	}

	void advanceTo(int minute) override {

		if (clients.isEmpty())
		{
			return;
		}

		// Sent workers for bananas or schweppes
		LinkedQueue<Worker> sentWorkers;

		// Clients who wait for bananas.
		PriorityQueue<MyClient> bananasClients;
		// Clients who wait for schweppes.
		PriorityQueue<MyClient> schweppesClients;
		// Clients who wait for bananas and schweppes.
		PriorityQueue<MyClient> bananasAndSchweppesClients;

		while (clients.isEmpty() != true || sentWorkers.isEmpty() != true
			|| bananasClients.isEmpty() != true || schweppesClients.isEmpty() != true
			|| bananasAndSchweppesClients.isEmpty() != false)
		{
			// todo1: proverka dali idvat rabotnici, ako da da vurna event
			// todo2: da proverq dali ima xora v opashkite za banani i shfeps

			// case when I have clients who will come to the store
			if (clients.isEmpty() != true) {

				MyClient currentClient = clients.front();

				if (currentClient.arriveMinute > minute) return;

				// case when store has enough bananas and schweppes for client
				if (currentClient.banana <= bananasQuantity && currentClient.schweppes <= schweppesQuantity)
				{
					actionHandler->onClientDepart(currentClient.getIndex(), currentClient.arriveMinute, currentClient.banana, currentClient.schweppes);
					bananasQuantity -= currentClient.banana;
					schweppesQuantity -= currentClient.schweppes;
					clients.dequeue();
					continue;
				}

				// case when store does not have enough bananas and schweppes for client
				// if we have two or more workers we can send two of them 
				if (currentClient.banana > bananasQuantity && currentClient.schweppes > schweppesQuantity)
				{
					// case if worker will be back before client arriveMinute + maxWaitTime to depart and we have 0 workers, it is not necessary to sent new worker
					// if worker will be not back before client arriveMinute + maxWaitTime to depart, we have to sent new worker
					if ((haveSentWorkersForBananas && haveSentWorkersForSchweppes)
						 || workerCount == 0)
					{
						bananasAndSchweppesClients.enqueue(currentClient);
						clients.dequeue();
						continue;
					}

					// case when we have more than one worker
					if (workerCount >= 2)
					{
						actionHandler->onWorkerSend(currentClient.arriveMinute, ResourceType::banana);
						actionHandler->onWorkerSend(currentClient.arriveMinute, ResourceType::schweppes);
						workerCount -= 2;

						Worker sentWorkerOne(currentClient.arriveMinute, ResourceType::banana);
						Worker sentWorkerTwo(currentClient.arriveMinute, ResourceType::schweppes);


						sentWorkers.enqueue(sentWorkerOne);
						sentWorkers.enqueue(sentWorkerTwo);
						haveSentWorkersForBananas = true;
						haveSentWorkersForSchweppes = true;

						bananasAndSchweppesClients.enqueue(currentClient);
						clients.dequeue();
						continue;
					}

					// case when we have one available worker
					if (workerCount == 1)
					{
						bool haveToGoForBananas = (bananasQuantity - currentClient.banana) >= (schweppesQuantity - currentClient.schweppes);
						if (haveToGoForBananas)
						{
							Worker sentWorker(currentClient.arriveMinute, ResourceType::banana);
							sentWorkers.enqueue(sentWorker);
							haveSentWorkersForBananas = true;

							bananasAndSchweppesClients.enqueue(currentClient);
							clients.dequeue();
						}
						else { // if haveToGoForBananas is false, we have to send worker for schweppes
							Worker sentWorker(currentClient.arriveMinute, ResourceType::schweppes);
							sentWorkers.enqueue(sentWorker);
							haveSentWorkersForSchweppes = true;

							bananasAndSchweppesClients.enqueue(currentClient);
							clients.dequeue();
						}

						workerCount--;
						continue;
					}
				}

				// case when store does not have enough bananas for client
				if (currentClient.banana > bananasQuantity && currentClient.schweppes <= schweppesQuantity)
				{
					// if worker will be back before client arriveMinute + maxWaitTime to depart and we have 0 workers, it is not necessary to sent new worker
					if (haveSentWorkersForBananas || workerCount == 0)
					{
						bananasClients.enqueue(currentClient);
						clients.dequeue();
						continue;
					}

					Worker sentWorker(currentClient.arriveMinute, ResourceType::banana);
					sentWorkers.enqueue(sentWorker);
					haveSentWorkersForBananas = true;

					bananasClients.enqueue(currentClient);
					clients.dequeue();
					workerCount--;
					continue;
				}

				// case when store does not have enough schweppes for client
				if (currentClient.banana <= bananasQuantity && currentClient.schweppes > schweppesQuantity)
				{
					// if worker will be back before client arriveMinute + maxWaitTime to depart and we have 0 workers, it is not necessary to sent new worker
					if (haveSentWorkersForSchweppes|| workerCount == 0)
					{
						schweppesClients.enqueue(currentClient);
						clients.dequeue();
						continue;
					}

					Worker sentWorker(currentClient.arriveMinute, ResourceType::schweppes);
					sentWorkers.enqueue(sentWorker);
					haveSentWorkersForSchweppes = true;


					schweppesClients.enqueue(currentClient);
					clients.dequeue();
					workerCount--;
					continue;
				}
			}
		}
	}

	virtual int getBanana() const {
		return bananasQuantity;
	}

	virtual int getSchweppes() const {
		return schweppesQuantity;
	}

private:
	void CheckForPreviousAction(const LinkedQueue<Worker>& sentWorkers,
		const PriorityQueue<MyClient>& waitingClientsForBananas, const PriorityQueue<MyClient>& waitingClientsForSchweppes,
		const PriorityQueue<MyClient>& waitingClientsForBananasAndSchweppes, const int minute,
		const bool haveClients)
	{
		int actionToMinute = 0;

		// ako dobavq prioritetna opashka za tova kak shte se izpulnqt procesite po minutite

		bool haveWaitingsClientsForBananas = !waitingClientsForBananas.isEmpty();
		bool haveWaitingsClientsForSchweppes = !waitingClientsForSchweppes.isEmpty();
		bool haveWaitingsClientsForBananasAndSchweppes = !waitingClientsForBananasAndSchweppes.isEmpty();

		bool havePersonInQueues = haveWaitingsClientsForBananas || haveWaitingsClientsForSchweppes
			|| haveWaitingsClientsForBananasAndSchweppes || haveSentWorkersForBananas || haveSentWorkersForSchweppes;

		if (havePersonInQueues == false) {
			return;
		}

		if (haveClients)
		{
			actionToMinute = clients.front().arriveMinute;
		}
		else {
			actionToMinute = minute;
		}
	}
};

Store* createStore() {
	return new MyStore();
}