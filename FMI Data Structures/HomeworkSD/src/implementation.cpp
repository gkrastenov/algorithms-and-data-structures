#pragma once
#include "interface.h"
#include "worker.h"
#include "linkedQueue.h"
#include "priorityQueue.h"
#include "myClient.h"

const int RESTOCK_AMOUNT = 100;

struct MyStore : Store {
private:
	ActionHandler* actionHandler = nullptr;

	int storeBananasQuantity = 0;
	int storeSchweppesQuantity = 0;
	int storeWorkerCount = 0;

	int sentWorkersForBananas = 0;
	int sentWorkersForSchweppes = 0;

	LinkedQueue<MyClient> clients;
public:
	void setActionHandler(ActionHandler* handler) override {
		actionHandler = handler;
	}

	void init(int workerCount, int startBanana, int startSchweppes) override {
		storeWorkerCount = workerCount;
		storeBananasQuantity = startBanana;
		storeSchweppesQuantity = startSchweppes;
	}

	void addClients(const Client* clients, int count) override {
		for (size_t i = 0; i < count; i++)
		{
			MyClient newClient(i, clients[i]);
			this->clients.enqueue(newClient);
		}
	}

	void advanceTo(int minute) override {

		if (clients.isEmpty()) return;

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
			|| bananasAndSchweppesClients.isEmpty() != true) // loop will end when all queues are empty
		{
			checkForPreviousAction(sentWorkers, bananasClients, schweppesClients, bananasAndSchweppesClients, minute, !clients.isEmpty());

			// case when have clients who will come to the store
			if (clients.isEmpty() != true) {

				MyClient currentClient = clients.front();

				if (currentClient.arriveMinute > minute) return;

				// case when store has enough bananas and schweppes for client
				if (currentClient.banana <= storeBananasQuantity && currentClient.schweppes <= storeSchweppesQuantity)
				{
					actionHandler->onClientDepart(currentClient.getIndex(), currentClient.arriveMinute, currentClient.banana, currentClient.schweppes);
					storeBananasQuantity -= currentClient.banana;
					storeSchweppesQuantity -= currentClient.schweppes;
					clients.dequeue();
					continue;
				}

				// case when store does not have enough bananas and schweppes for client
				// if we have two or more workers we can send two of them 
				if (currentClient.banana > storeBananasQuantity && currentClient.schweppes > storeSchweppesQuantity)
				{
					// case if worker will be back before client arriveMinute + maxWaitTime to depart and we have 0 workers, it is not necessary to sent new worker
					// if worker will be not back before client arriveMinute + maxWaitTime to depart, we have to sent new worker
					if ((sentWorkersForBananas > 0 && sentWorkersForSchweppes == 0))
					{
						if (storeWorkerCount > 0)
						{
							Worker sentWorker(currentClient.arriveMinute, ResourceType::schweppes);
							sentWorkers.enqueue(sentWorker);
							actionHandler->onWorkerSend(currentClient.arriveMinute, ResourceType::schweppes);
							sentWorkersForSchweppes += 1;
							storeWorkerCount -= 1;
						}
						bananasAndSchweppesClients.enqueue(currentClient);
						clients.dequeue();
						continue;
					}

					if ((sentWorkersForBananas == 0 && sentWorkersForSchweppes > 0))
					{
						if (storeWorkerCount > 0)
						{
							Worker sentWorker(currentClient.arriveMinute, ResourceType::banana);
							sentWorkers.enqueue(sentWorker);
							actionHandler->onWorkerSend(currentClient.arriveMinute, ResourceType::banana);
							sentWorkersForSchweppes += 1;
							storeWorkerCount -= 1;
						}
						bananasAndSchweppesClients.enqueue(currentClient);
						clients.dequeue();
						continue;
					}

					if ((sentWorkersForBananas > 0 && sentWorkersForSchweppes > 0) || storeWorkerCount == 0)
					{
						bananasAndSchweppesClients.enqueue(currentClient);
						clients.dequeue();
						continue;
					}
					else {

						// case when we have more than one worker
						if (storeWorkerCount >= 2)
						{
							while (storeWorkerCount != 0)
							{
								if (storeWorkerCount >= 2)
								{
									if (sentWorkersForBananas * RESTOCK_AMOUNT < currentClient.banana && 
										currentClient.banana - sentWorkersForBananas * RESTOCK_AMOUNT >= currentClient.schweppes - sentWorkersForSchweppes * RESTOCK_AMOUNT)
									{
										Worker sentWorkerOne(currentClient.arriveMinute, ResourceType::banana);
										sentWorkers.enqueue(sentWorkerOne);
										sentWorkersForBananas++;
										actionHandler->onWorkerSend(currentClient.arriveMinute, ResourceType::banana);
										storeWorkerCount--;
										continue;
									}

									if (sentWorkersForSchweppes * RESTOCK_AMOUNT < currentClient.schweppes
										&& currentClient.banana - sentWorkersForBananas * RESTOCK_AMOUNT <= currentClient.schweppes - sentWorkersForSchweppes * RESTOCK_AMOUNT)
									{
										Worker sentWorkerTwo(currentClient.arriveMinute, ResourceType::schweppes);
										sentWorkers.enqueue(sentWorkerTwo);
										sentWorkersForSchweppes += 1;
										actionHandler->onWorkerSend(currentClient.arriveMinute, ResourceType::schweppes);
										storeWorkerCount--;
										continue;
									}
									storeWorkerCount -= 2;
								}
								else {
									if (currentClient.banana - sentWorkersForBananas * RESTOCK_AMOUNT >= currentClient.schweppes - sentWorkersForSchweppes * RESTOCK_AMOUNT)
									{
										Worker sentWorkerOne(currentClient.arriveMinute, ResourceType::banana);
										sentWorkers.enqueue(sentWorkerOne);
										sentWorkersForBananas++;
										actionHandler->onWorkerSend(currentClient.arriveMinute, ResourceType::banana);
										storeWorkerCount--;
										continue;
									}
									else {
										Worker sentWorkerTwo(currentClient.arriveMinute, ResourceType::schweppes);
										sentWorkers.enqueue(sentWorkerTwo);
										sentWorkersForSchweppes += 1;
										actionHandler->onWorkerSend(currentClient.arriveMinute, ResourceType::schweppes);
										storeWorkerCount--;
										continue;
									}
								}
							}
							bananasAndSchweppesClients.enqueue(currentClient);
							clients.dequeue();
							continue;
						}

						// case when we have one available worker
						if (storeWorkerCount == 1)
						{
							bool haveToGoForBananas = (storeBananasQuantity - currentClient.banana) <= (storeSchweppesQuantity - currentClient.schweppes);
							if (haveToGoForBananas)
							{
								Worker sentWorker(currentClient.arriveMinute, ResourceType::banana);
								sentWorkers.enqueue(sentWorker);
								sentWorkersForBananas += 1;

								actionHandler->onWorkerSend(currentClient.arriveMinute, ResourceType::banana);
								storeWorkerCount -= 1;

								bananasAndSchweppesClients.enqueue(currentClient);
								clients.dequeue();
							}
							else { // if haveToGoForBananas is false, we have to send worker for schweppes
								Worker sentWorker(currentClient.arriveMinute, ResourceType::schweppes);
								sentWorkers.enqueue(sentWorker);
								sentWorkersForSchweppes += 1;

								actionHandler->onWorkerSend(currentClient.arriveMinute, ResourceType::schweppes);
								storeWorkerCount -= 1;

								bananasAndSchweppesClients.enqueue(currentClient);
								clients.dequeue();
							}

							storeWorkerCount--;
							continue;
						}
					}
				}

				// case when store does not have enough bananas for client
				if (currentClient.banana > storeBananasQuantity && currentClient.schweppes <= storeSchweppesQuantity)
				{
					// if worker will be back before client to depart and we have 0 workers,
					// it is not necessary to sent new worker
					if (sentWorkersForBananas > 0 || storeWorkerCount == 0)
					{
						bananasClients.enqueue(currentClient);
						clients.dequeue();
						continue;
					}

					Worker sentWorker(currentClient.arriveMinute, ResourceType::banana);
					sentWorkers.enqueue(sentWorker);
					actionHandler->onWorkerSend(currentClient.arriveMinute, ResourceType::banana);

					bananasClients.enqueue(currentClient);
					clients.dequeue();

					sentWorkersForBananas++;
					storeWorkerCount--;
					continue;
				}

				// case when store does not have enough schweppes for client
				if (currentClient.banana <= storeBananasQuantity && currentClient.schweppes > storeSchweppesQuantity)
				{
					if (sentWorkersForSchweppes > 0 || storeWorkerCount == 0)
					{
						schweppesClients.enqueue(currentClient);
						clients.dequeue();
						continue;
					}

					Worker sentWorker(currentClient.arriveMinute, ResourceType::schweppes);
					sentWorkers.enqueue(sentWorker);
					actionHandler->onWorkerSend(currentClient.arriveMinute, ResourceType::schweppes);

					schweppesClients.enqueue(currentClient);
					clients.dequeue();

					sentWorkersForSchweppes++;
					storeWorkerCount--;
					continue;
				}
			}
			return;
		}
	}

	virtual int getBanana() const {
		return storeBananasQuantity;
	}

	virtual int getSchweppes() const {
		return storeSchweppesQuantity;
	}

private:
	/// <summary>
	/// Create actions before starting a new client processing
	/// </summary>
	/// <param name="sentWorkers">Queue which hold sent workers</param>
	/// <param name="waitingClientsForBananas">Queue which hold waitings clients for bananas</param>
	/// <param name="waitingClientsForSchweppes">Queue which hold waitings clients for schweppes</param>
	/// <param name="waitingClientsForBananasAndSchweppes">Queue which hold waitings clients for  bananas and schweppes</param>
	/// <param name="minute">given minute from advanceTo method</param>
	/// <param name="haveClients">If have client in queue</param>
	void checkForPreviousAction(LinkedQueue<Worker>& sentWorkers,
		PriorityQueue<MyClient>& waitingClientsForBananas, PriorityQueue<MyClient>& waitingClientsForSchweppes,
		PriorityQueue<MyClient>& waitingClientsForBananasAndSchweppes, const int minute,
		const bool haveClients)
	{
		int actionToMinute = 0;
		bool havePersonInQueues = !waitingClientsForBananas.isEmpty() || !waitingClientsForSchweppes.isEmpty()
			|| !waitingClientsForBananasAndSchweppes.isEmpty() || !sentWorkers.isEmpty();

		if (havePersonInQueues == false) return;

		if (haveClients)
		{
			if (clients.front().arriveMinute > minute)
			{
				actionToMinute = minute;
			}
			else {
				actionToMinute = clients.front().arriveMinute;
			}
		}
		else {
			actionToMinute = minute;
		}

		int nextActionIndex = -1;
		do
		{
			int* indexs = new int[4]{ -1,-1,-1,-1 };
			if (!sentWorkers.isEmpty())
			{
				if (sentWorkers.front().getBackMinute() <= actionToMinute) {
					indexs[0] = sentWorkers.front().getBackMinute();
				}
			}

			if (!waitingClientsForBananas.isEmpty())
			{
				if (waitingClientsForBananas.front().getDepartTime() <= actionToMinute) {
					indexs[1] = waitingClientsForBananas.front().getDepartTime();
				}
			}

			if (!waitingClientsForSchweppes.isEmpty())
			{
				if (waitingClientsForSchweppes.front().getDepartTime() <= actionToMinute) {
					indexs[2] = waitingClientsForSchweppes.front().getDepartTime();
				}
			}

			if (!waitingClientsForBananasAndSchweppes.isEmpty())
			{
				if (waitingClientsForBananasAndSchweppes.front().getDepartTime() <= actionToMinute) {
					indexs[3] = waitingClientsForBananasAndSchweppes.front().getDepartTime();
				}
			}

			nextActionIndex = findNextAction(indexs);
			switch (nextActionIndex)
			{
			case 0:
				if (sentWorkers.front().getResourceType() == ResourceType::banana){
					sentWorkersForBananas--;
					storeBananasQuantity += RESTOCK_AMOUNT;

					actionHandler->onWorkerBack(sentWorkers.front().getBackMinute(), sentWorkers.front().getResourceType());
					createActionsAfterWorkerIsBackWithBananas(waitingClientsForBananas, waitingClientsForBananasAndSchweppes,
						sentWorkers.front().getBackMinute());
				}
				else {
					sentWorkersForSchweppes -= 1;
					storeSchweppesQuantity += RESTOCK_AMOUNT;

					actionHandler->onWorkerBack(sentWorkers.front().getBackMinute(), sentWorkers.front().getResourceType());
					createActionsAfterWorkerIsBackWithSchweppes(waitingClientsForSchweppes, waitingClientsForBananasAndSchweppes,
						sentWorkers.front().getBackMinute());
				}
				sentWorkers.dequeue();
				break;
			case 1:
				createActionForDepartClient(waitingClientsForBananas.front());
				waitingClientsForBananas.dequeue();
				break;
			case 2:
				createActionForDepartClient(waitingClientsForSchweppes.front());
				waitingClientsForSchweppes.dequeue();
				break;
			case 3:
				createActionForDepartClient(waitingClientsForBananasAndSchweppes.front());
				waitingClientsForBananasAndSchweppes.dequeue();
				break;
			}
			delete[] indexs;
		} while (nextActionIndex != -1);

	}

	/// <summary>
	/// Find next action
	/// </summary>
	/// <param name="values">array with capacity 4</param>
	/// <returns>
	/// 0 - queue for sent workers
	/// 1 - queue for waiting clients bananas
	/// 2 - queue for waiting clients schweppes
	/// 3 - queue for waiting clients bananas and schweppes
	/// </returns>
	int findNextAction(int values[4])
	{
		int currentValue = INT_MAX;
		int result = -1;
		int index = 0;

		while (index != 4)
		{
			if (values[index] == -1)
			{
				index++;
				continue;
			}

			if (values[index] < currentValue)
			{
				currentValue = values[index];
				result = index;
			}
			index++;
		}
		return result;
	}

	/// <summary>
	/// In this case client can not wait more time and takes what is available.
	/// </summary>
	/// <param name="client">Next waiting client</param>
	void createActionForDepartClient(const MyClient& client)
	{
		int haveToGetBananas = 0;
		int haveToGetSchweppes = 0;

		if (storeBananasQuantity - client.banana <= 0){
			haveToGetBananas = storeBananasQuantity;
		}
		else {
			haveToGetBananas = client.banana;
		}

		if (storeSchweppesQuantity - client.schweppes <= 0){
			haveToGetSchweppes = storeSchweppesQuantity;
		}
		else {
			haveToGetSchweppes = client.schweppes;
		}

		storeBananasQuantity -= haveToGetBananas;
		storeSchweppesQuantity -= haveToGetSchweppes;

		actionHandler->onClientDepart(client.getIndex(), client.getDepartTime(), haveToGetBananas, haveToGetSchweppes);
	}

	/// <summary>
	/// After worker with a product has arrived, all possible actions are creating for the clients who are waiting for the product to arrive.
	/// </summary>
	/// <param name="queue"></param>
	/// <param name="waitingClientsForBananasAndSchweppes">The waitings clients who wait for both bananas and schweppes</param>
	/// <param name="workerBackMinute"></param>
	void createActionsAfterWorkerIsBackWithBananas(PriorityQueue<MyClient>& waitingsClientsForBananas, 
		PriorityQueue<MyClient>& waitingClientsForBananasAndSchweppes, 
		const int workerBackMinute)
	{
		bool flag = true;
		while (flag)
		{
			// if don't have clients who waiting, we don't create action
			if (waitingsClientsForBananas.isEmpty() && waitingClientsForBananasAndSchweppes.isEmpty()) return;

			// If have clients in both possible queues, create action for the client who is next
			if (!waitingsClientsForBananas.isEmpty() && !waitingClientsForBananasAndSchweppes.isEmpty())
			{
				// Choose who is next
				if (waitingsClientsForBananas.front().getIndex() > waitingClientsForBananasAndSchweppes.front().getIndex())
				{
					// if have enough resources create action
					if (waitingsClientsForBananas.front().banana <= storeBananasQuantity)
					{
						actionHandler->onClientDepart(waitingsClientsForBananas.front().getIndex(), workerBackMinute,
							waitingsClientsForBananas.front().banana, waitingsClientsForBananas.front().schweppes);

						storeBananasQuantity -= waitingsClientsForBananas.front().banana;
						waitingsClientsForBananas.dequeue();
					}
					else {
						flag = false;
					}
				}
				else {
					// if have enough resources create action
					if (waitingClientsForBananasAndSchweppes.front().banana <= storeBananasQuantity
						&& waitingClientsForBananasAndSchweppes.front().banana <= storeSchweppesQuantity)
					{
						actionHandler->onClientDepart(waitingClientsForBananasAndSchweppes.front().getIndex(), workerBackMinute,
							waitingClientsForBananasAndSchweppes.front().banana, waitingClientsForBananasAndSchweppes.front().schweppes);

						storeBananasQuantity -= waitingClientsForBananasAndSchweppes.front().banana;
						storeSchweppesQuantity -= waitingClientsForBananasAndSchweppes.front().schweppes;
						waitingClientsForBananasAndSchweppes.dequeue();
					}
					else {
						flag = false;
					}
				}
			}

			// If have clients only in bananas queue
			if (!waitingsClientsForBananas.isEmpty())
			{
				if (waitingsClientsForBananas.front().banana <= storeBananasQuantity)
				{
					actionHandler->onClientDepart(waitingsClientsForBananas.front().getIndex(), workerBackMinute,
						waitingsClientsForBananas.front().banana, waitingsClientsForBananas.front().schweppes);

					storeBananasQuantity -= waitingsClientsForBananas.front().banana;
					waitingsClientsForBananas.dequeue();
				}
				else {
					flag = false;
					continue;
				}
			}

			// If have clients only in bananas and schweppes queues
			if (!waitingClientsForBananasAndSchweppes.isEmpty())
			{
				if (waitingClientsForBananasAndSchweppes.front().banana <= storeBananasQuantity 
					&& waitingClientsForBananasAndSchweppes.front().banana <= storeSchweppesQuantity)
				{
					actionHandler->onClientDepart(waitingClientsForBananasAndSchweppes.front().getIndex(), workerBackMinute,
						waitingClientsForBananasAndSchweppes.front().banana, waitingClientsForBananasAndSchweppes.front().schweppes);

					storeBananasQuantity -= waitingClientsForBananasAndSchweppes.front().banana;
					storeSchweppesQuantity -= waitingClientsForBananasAndSchweppes.front().schweppes;
					waitingClientsForBananasAndSchweppes.dequeue();
				}
				else {
					flag = false;
					continue;
				}
			}
		}
	}

	void createActionsAfterWorkerIsBackWithSchweppes(PriorityQueue<MyClient>& waitingsClientsForSchweppes,
		PriorityQueue<MyClient>& waitingClientsForBananasAndSchweppes,
		const int workerBackMinute)
	{
		bool flag = true;
		while (flag)
		{
			// if don't have clients who waiting, we don't create action
			if (waitingsClientsForSchweppes.isEmpty() && waitingClientsForBananasAndSchweppes.isEmpty()) return;

			// If have clients in both possible queues, create action for the client who is next
			if (!waitingsClientsForSchweppes.isEmpty() && !waitingClientsForBananasAndSchweppes.isEmpty())
			{
				// Choose who is next
				if (waitingsClientsForSchweppes.front().getIndex() > waitingClientsForBananasAndSchweppes.front().getIndex())
				{
					// if have enough resources create action
					if (waitingsClientsForSchweppes.front().schweppes <= storeSchweppesQuantity)
					{
						actionHandler->onClientDepart(waitingsClientsForSchweppes.front().getIndex(), workerBackMinute,
							waitingsClientsForSchweppes.front().banana, waitingsClientsForSchweppes.front().schweppes);

						storeSchweppesQuantity -= waitingsClientsForSchweppes.front().schweppes;
						waitingsClientsForSchweppes.dequeue();
					}
					else {
						flag = false;
					}
				}
				else {
					// if have enough resources create action
					if (waitingClientsForBananasAndSchweppes.front().banana <= storeBananasQuantity
						&& waitingClientsForBananasAndSchweppes.front().banana <= storeSchweppesQuantity)
					{
						actionHandler->onClientDepart(waitingClientsForBananasAndSchweppes.front().getIndex(), workerBackMinute,
							waitingClientsForBananasAndSchweppes.front().banana, waitingClientsForBananasAndSchweppes.front().schweppes);

						storeBananasQuantity -= waitingClientsForBananasAndSchweppes.front().banana;
						storeSchweppesQuantity -= waitingClientsForBananasAndSchweppes.front().schweppes;
						waitingClientsForBananasAndSchweppes.dequeue();
					}
					else {
						flag = false;
					}
				}
			}

			// If have clients only in schweppes queues,
			if (!waitingsClientsForSchweppes.isEmpty())
			{
				if (waitingsClientsForSchweppes.front().banana <= storeSchweppesQuantity)
				{
					actionHandler->onClientDepart(waitingsClientsForSchweppes.front().getIndex(), workerBackMinute,
						waitingsClientsForSchweppes.front().banana, waitingsClientsForSchweppes.front().schweppes);

					storeSchweppesQuantity -= waitingsClientsForSchweppes.front().schweppes;
					waitingsClientsForSchweppes.dequeue();
				}
				else {
					flag = false;
					continue;
				}
			}

			// If have clients only in bananas and schweppes queues
			if (!waitingClientsForBananasAndSchweppes.isEmpty())
			{
				if (waitingClientsForBananasAndSchweppes.front().banana <= storeBananasQuantity
					&& waitingClientsForBananasAndSchweppes.front().banana <= storeSchweppesQuantity)
				{
					actionHandler->onClientDepart(waitingClientsForBananasAndSchweppes.front().getIndex(), workerBackMinute,
						waitingClientsForBananasAndSchweppes.front().banana, waitingClientsForBananasAndSchweppes.front().schweppes);

					storeBananasQuantity -= waitingClientsForBananasAndSchweppes.front().banana;
					storeSchweppesQuantity -= waitingClientsForBananasAndSchweppes.front().schweppes;
					waitingClientsForBananasAndSchweppes.dequeue();
				}
				else {
					flag = false;
					continue;
				}
			}
		}
	}

};

Store* createStore() {
	return new MyStore();
}