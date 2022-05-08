#define CATCH_CONFIG_MAIN

#include "interface.h"
#include "catch.hpp"
#include <vector>

const int RESTOCK_TIME = 60;
const int RESTOCK_AMOUNT = 100;

typedef std::vector<Client> ClientList;

struct StoreEvent {
	enum Type {
		WorkerSend, WorkerBack, ClientDepart
	};

	Type type;
	int minute;

	struct Worker {
		ResourceType resource;
	} worker;

	struct Client {
		int index;
		int banana;
		int schweppes;
	} client;
};

struct TestStore : ActionHandler {
	Store *impl = nullptr;
	std::vector<StoreEvent> log;

	TestStore()
		: impl(createStore()) {
		impl->setActionHandler(this);
	}

	~TestStore() {
		delete impl;
	}

	TestStore(const TestStore &) = delete;
	TestStore& operator=(const TestStore &) = delete;

	void init(int workerCount, int startBanana, int startSchweppes) {
		impl->init(workerCount, startBanana, startSchweppes);
	}

	void advanceTo(int minute) {
		impl->advanceTo(minute);
	}

	int getBanana() const {
		return impl->getBanana();
	}

	int getSchweppes() const {
		return impl->getSchweppes();
	}

	void addClients(const ClientList &clients) {
		impl->addClients(clients.data(), clients.size());
	}

	void addClients(const Client &single) {
		impl->addClients(&single, 1);
	}

	void onWorkerSend(int minute, ResourceType resource) override {
		StoreEvent ev;
		ev.type = StoreEvent::WorkerSend;
		ev.minute = minute;
		ev.worker.resource = resource;
		log.push_back(ev);
	}

	void onWorkerBack(int minute, ResourceType resource) override {
		StoreEvent ev;
		ev.type = StoreEvent::WorkerBack;
		ev.minute = minute;
		ev.worker.resource = resource;
		log.push_back(ev);
	}

	void onClientDepart(int index, int minute, int banana, int schweppes) override {
		StoreEvent ev;
		ev.type = StoreEvent::ClientDepart;
		ev.minute = minute;
		ev.client.banana = banana;
		ev.client.schweppes = schweppes;
		ev.client.index = index;
		log.push_back(ev);
	}
};

#define LastEvent() (store.log.back())

#pragma region Application layer tests
TEST_CASE("No workers, empty store, up to one client") {
	TestStore store;
	store.init(0, 0, 0);

	SECTION("No events") {
		INFO("Without clients, no events should be generated");
		REQUIRE(store.log.size() == 0);
		store.advanceTo(0);
		REQUIRE(store.log.size() == 0);
		store.advanceTo(1);
		REQUIRE(store.log.size() == 0);

		store.advanceTo(100);
		REQUIRE(store.log.size() == 0);
	}

	SECTION("Advance before depart time") {
		store.addClients(Client{0, 1, 1, 1});
		store.advanceTo(0);
		INFO("Must not generate event before time is advanced to its time");
		REQUIRE(store.log.size() == 0);
	}

	SECTION("Depart time") {
		INFO("maxWaitTime == 1 means, client will wait 1 minute (arrive at 0, depart at 1)");
		INFO("advanceTo(<time>), should generate all events that happen up to and including <time>");
		store.addClients(Client{0, 1, 1, 1});
		store.advanceTo(1);
		REQUIRE(store.log.size() == 1);
	}

	SECTION("client without request") {
		INFO("Client with wait time 0, must generate event at the time of arrival");
		store.addClients(Client{0, 0, 0, 0});
		store.advanceTo(0);
		REQUIRE(store.log.size() == 1);

		INFO("Client without any request should depart empty - (0 banana, 0 schweppes)");
		REQUIRE(LastEvent().minute == 0);
		REQUIRE(LastEvent().type == StoreEvent::ClientDepart);
		REQUIRE(LastEvent().client.banana == 0);
		REQUIRE(LastEvent().client.schweppes == 0);
	}

	SECTION("advance before client departs") {
		store.addClients(Client{0, 1, 1, 10});

		store.advanceTo(3);
		INFO("client must not depart before its max wait time");
		REQUIRE(store.log.size() == 0);
	}
}

TEST_CASE("No workers, full store") {
	TestStore store;
	store.init(0, 1000, 1000);
	const ClientList three = {
		Client{0, 0, 10, 10},
		Client{0, 10, 0, 3},
		Client{0, 10, 10, 5}
	};

	SECTION("Non waiting client") {
		store.addClients(Client{0, 10, 0, 0});
		store.advanceTo(0);
		INFO("Client must depart same at the time of arrival when store has enough resources");
		REQUIRE(store.log.size() == 1);
		REQUIRE(store.log[0].type == StoreEvent::ClientDepart);
	}

	SECTION("Client with wait time") {
		store.addClients(Client{0, 10, 0, 10});
		store.advanceTo(0);
		INFO("Client must depart same at the time of arrival when store has enough resources");
		REQUIRE(store.log.size() == 1);
		REQUIRE(store.log[0].type == StoreEvent::ClientDepart);
	}

	SECTION("Multiple clients") {
		store.addClients(three);
		store.advanceTo(0);
		INFO("Client must depart same at the time of arrival when store has enough resources");
		REQUIRE(store.log.size() == 3);
		REQUIRE(store.log[0].type == StoreEvent::ClientDepart);
		REQUIRE(store.log[1].type == StoreEvent::ClientDepart);
		REQUIRE(store.log[2].type == StoreEvent::ClientDepart);
	}

	SECTION("Client indices") {
		store.addClients(three);
		store.advanceTo(0);
		INFO("Indices must be correct");
		REQUIRE(store.log[0].client.index == 0);
		REQUIRE(store.log[1].client.index == 1);
		REQUIRE(store.log[2].client.index == 2);
	}
}

TEST_CASE("Multiple stores") {
	TestStore bananaStore;
	bananaStore.init(0, 100, 0);
	TestStore schweppesStore;
	schweppesStore.init(0, 0, 100);

	SECTION("Two stores") {
		INFO("Multiple stores must be ebable to exist at the same time");

		REQUIRE(bananaStore.getBanana() == 100);
		REQUIRE(bananaStore.getSchweppes() == 0);

		REQUIRE(schweppesStore.getBanana() == 0);
		REQUIRE(schweppesStore.getSchweppes() == 100);
	}

	SECTION("Clients to one of the stores") {
		bananaStore.addClients(Client{0, 10, 0, 10});
		bananaStore.advanceTo(0);
		schweppesStore.advanceTo(0);

		INFO("New client to one store must not generate event in the other store");
		REQUIRE(bananaStore.log.size() == 1);
		REQUIRE(schweppesStore.log.size() == 0);
	}

	SECTION("Clients to both stores") {
		bananaStore.addClients(Client{0, 5, 0, 0});
		schweppesStore.addClients(Client{0, 0, 10, 0});

		bananaStore.advanceTo(0);
		schweppesStore.advanceTo(0);

		INFO("Both stores should generate valid evenets");
		REQUIRE(bananaStore.log.back().type == StoreEvent::ClientDepart);
		REQUIRE(bananaStore.log.back().client.banana == 5);

		REQUIRE(schweppesStore.log.back().type == StoreEvent::ClientDepart);
		REQUIRE(schweppesStore.log.back().client.schweppes == 10);
	}
}

TEST_CASE("Example") {
	TestStore store;
	store.init(5, 0, 0);

	store.addClients({
		Client{0, 10, 0, 10},
		Client{45, 35, 0, 30},
		Client{46, 30, 20, 100},
		Client{200, 10, 10, 1}
	});

	SECTION("First client") {
		store.advanceTo(0);
		INFO("First client will trigger 1 worker");
		REQUIRE(store.log.size() == 1);
		REQUIRE(LastEvent().type == StoreEvent::WorkerSend);
	}

	SECTION("First client") {
		store.advanceTo(10);
		INFO("First client must depart without anything");
		REQUIRE(store.log.size() == 2);
		REQUIRE(LastEvent().type == StoreEvent::ClientDepart);
		REQUIRE(LastEvent().client.banana == 0);
	}

	SECTION("Last client") {
		store.advanceTo(200);
		INFO("Last client departs same time as arrival");
		REQUIRE(store.log.size() == 8);
		REQUIRE(LastEvent().type == StoreEvent::ClientDepart);
		REQUIRE(LastEvent().client.banana == 10);
		REQUIRE(LastEvent().client.schweppes == 10);
	}

	SECTION("Remaining resources") {
		store.advanceTo(500);
		int bananas = 0;
		int schweppes = 0;
		for (int c = 0; c < store.log.size(); c++) {
			const StoreEvent &ev = store.log[c];
			if (ev.type == StoreEvent::WorkerBack) {
				REQUIRE(store.log[c].worker.resource >= ResourceType::banana);
				REQUIRE(store.log[c].worker.resource <= ResourceType::schweppes);
				if (ev.worker.resource == ResourceType::banana) {
					bananas += RESTOCK_AMOUNT;
				} else if (ev.worker.resource == ResourceType::schweppes) {
					schweppes += RESTOCK_AMOUNT;
				}
			} else if (ev.type == StoreEvent::ClientDepart) {
				bananas -= ev.client.banana;
				schweppes -= ev.client.schweppes;
			}
		}

		INFO("Stock amount - client depart amounth must be what is left in the store");
		REQUIRE(store.getBanana() == bananas);
		REQUIRE(store.getSchweppes() == schweppes);
	}
}

TEST_CASE("Workers can be sent out only when needed") {
	TestStore store;
	store.init(1, 100, 0);

	store.addClients({
		Client{0, 100, 0, 1}, // this client will get everything
		Client{10, 1, 0, 1}
	});


	SECTION("Before second client") {
		store.advanceTo(9);

		INFO("Worker must not be sent before client arrives");
		REQUIRE(store.log.size() == 1);
		REQUIRE(LastEvent().type == StoreEvent::ClientDepart);
		REQUIRE(store.getBanana() == 0);
	}

	SECTION("After second client") {
		store.advanceTo(10);

		INFO("Worker must be sent the same minute second clients arrives");
		REQUIRE(store.log.size() == 2);
		REQUIRE(LastEvent().type == StoreEvent::WorkerSend);
		REQUIRE(LastEvent().minute == 10);
	}
}

TEST_CASE("Workers must not be sent if resource will be available before client departs") {
	TestStore store;
	store.init(2, 0, 0);

	store.addClients({
		Client{0, 10, 0, 200},
		Client{10, 10, 0, 200}
	});

	SECTION("First client must trigger a worker") {
		store.advanceTo(5);
		REQUIRE(store.log.size() == 1);
	}

	SECTION("Second client must not trigger a worker") {
		store.advanceTo(15);
		REQUIRE(store.log.size() == 1);
	}
}

TEST_CASE("Clients depart and take what they can") {
	TestStore store;
	store.init(5, 10, 0);

	store.addClients({
		Client{0, 0, 10, 0}, // Trigger worker sent for schweppes
		Client{1, 20, 0, 5}
	});

	SECTION("Sent out workers") {
		store.advanceTo(3);
		INFO("Store must send 2 workers, and 1 client has departed");
		REQUIRE(store.log.size() == 3);
	}

	SECTION("Client departs with only part of requirement") {
		store.advanceTo(1 + 5);

		INFO("Client must take whatever is available");
		REQUIRE(LastEvent().type == StoreEvent::ClientDepart);
		REQUIRE(LastEvent().client.banana == 10);
		REQUIRE(LastEvent().client.schweppes == 0);
	}
}

TEST_CASE("Clients arrive/depart in mixed order") {
	TestStore store;
	store.init(2, 10, 0);

	store.addClients({
		Client{0, 10, 10, 20},
		Client{10, 10, 0, 0}
	});

	SECTION("One worker must be sent") {
		store.advanceTo(0);

		INFO("First client must trigger a worker to restock schweppes");
		REQUIRE(store.log.size() == 1);
		REQUIRE(LastEvent().type == StoreEvent::WorkerSend);
		REQUIRE(LastEvent().worker.resource == ResourceType::schweppes);
	}

	SECTION("Second client") {
		store.advanceTo(10);

		INFO("Second client comes at min 10, but waits 0, departs at min 10");
		REQUIRE(store.log.size() == 2);
		REQUIRE(LastEvent().type == StoreEvent::ClientDepart);
		REQUIRE(LastEvent().client.banana == 10);
		REQUIRE(LastEvent().client.index == 1);
	}

	SECTION("First client") {
		store.advanceTo(20);

		INFO("First client departs nothing after second client");
		REQUIRE(store.log.size() == 3);
		REQUIRE(LastEvent().type == StoreEvent::ClientDepart);
		REQUIRE(LastEvent().client.banana == 0);
		REQUIRE(LastEvent().client.index == 0);
	}
}

#pragma endregion

// add header files for testing other functionalities in project
#include "linkedQueue.h"
#include "priorityQueue.h"
#include "worker.h"
#include "myClient.h"

#pragma region LinkedQueue Tests
TEST_CASE("Adding a new element in LinkedQueue")
{
	LinkedQueue<int> queueInt;

	LinkedQueue<Client> queueClients;

	SECTION("Add numbers") {
		INFO("Before we add elements to the new queue it have to be empty");
		REQUIRE(queueInt.isEmpty());

		queueInt.enqueue(1);
		queueInt.enqueue(2);
		INFO("When we add elements to the queue it have not be empty");
		REQUIRE(queueInt.isEmpty() == false);
	}


	SECTION("Add Clients") {
		REQUIRE(queueClients.isEmpty());
		queueClients.enqueue(Client{ 0, 10, 10, 20 });
		INFO("When we add client to the queue it must not be empty");
		REQUIRE(queueClients.isEmpty() == false);
	}
}

TEST_CASE("Front method in LinkedQueue")
{
	LinkedQueue<int> queueInt;

	SECTION("Front element by queue if it is empty") {
		bool exceptionThrown = false;

		try	{
			int frontInt = queueInt.front();
		}
		catch (std::underflow_error) { // special exception type		
			exceptionThrown = true;
		}
		catch (...)		{
			exceptionThrown = true;
		}

		INFO("Front method has to throw underflow error exception if queue is empty and we want to use this method");
		REQUIRE(exceptionThrown);
	}

	SECTION("Front element from queue if it is noy empty") {
		queueInt.enqueue(1);
		queueInt.enqueue(-2);
		queueInt.enqueue(3);
		queueInt.enqueue(4);
		queueInt.enqueue(-5);
		queueInt.enqueue(6000);

		INFO("Front method has to return first added element");
		REQUIRE(queueInt.isEmpty() == false);
		REQUIRE(queueInt.front() == 1);
	}
}

TEST_CASE("Removing a new element in LinkedQueue")
{
	LinkedQueue<Client> queueClients;

	SECTION("Dequeue one client") {
		queueClients.enqueue(Client{ 0, 30, 10, 20 });
		REQUIRE(queueClients.isEmpty() == false);
		queueClients.dequeue();
		REQUIRE(queueClients.isEmpty());

	}
	SECTION("Dequeue many clients")
	{
		queueClients.enqueue(Client{ 0, 30, 10, 20 });
		queueClients.enqueue(Client{ 1, 40, 20, 0 });
		queueClients.enqueue(Client{ 10, 50, 30, 0 });

		REQUIRE(queueClients.isEmpty() == false);
		queueClients.dequeue();
		queueClients.dequeue();
		queueClients.dequeue();

		REQUIRE(queueClients.isEmpty());
	}
}

TEST_CASE("Big Four in LinkedQueue")
{
	SECTION("Default constructor")
	{
		LinkedQueue<int> queueInt;

		REQUIRE(queueInt.isEmpty());
	}
	SECTION("Copy Constructor")
	{
		LinkedQueue<int> queueInt;
		// it will be added 0, 30, 60
		for (size_t i = 0; i <=60; i+=30)
		{
			queueInt.enqueue(i);
		}

		REQUIRE(queueInt.isEmpty() == false);
		LinkedQueue<int> copyQueueInt(queueInt);
		REQUIRE(copyQueueInt.isEmpty() == false);

		for (size_t i = 0; i < 3; i++)
		{
			REQUIRE(copyQueueInt.front() == queueInt.front());
			queueInt.dequeue();
			copyQueueInt.dequeue();
		}
		REQUIRE(queueInt.isEmpty() == copyQueueInt.isEmpty());
	}
	SECTION("Destructor")
	{
		LinkedQueue<int> queueInt;
		queueInt.enqueue(3);
		queueInt.enqueue(3);
		queueInt.enqueue(3);
		queueInt.enqueue(3);
		REQUIRE(queueInt.isEmpty() == false);
		queueInt.~LinkedQueue();
		INFO("The queue must be empty after calling the destructor");
		REQUIRE(queueInt.isEmpty());
	}
	SECTION("Copy assignment operator")
	{
		LinkedQueue<int> queueInt;
		for (size_t i = 0; i < 3; i++)	{
			queueInt.enqueue(i);
		}

		REQUIRE(queueInt.isEmpty() == false);
		LinkedQueue<int> copyQueueInt = queueInt;
		REQUIRE(copyQueueInt.isEmpty() == false);

		for (size_t i = 0; i < 3; i++)	{
			REQUIRE(copyQueueInt.front() == queueInt.front());
			queueInt.dequeue();
			copyQueueInt.dequeue();
		}

		REQUIRE(queueInt.isEmpty() == copyQueueInt.isEmpty());
	}
}
#pragma endregion


#pragma region PriorityQueue Tests
TEST_CASE("Adding a new element in PriorityQueue")
{
	MyClient client1(1, Client{ 0, 10, 10, 20 });
	MyClient client2(2, Client{ 0, 10, 10, 20 });

	PriorityQueue<MyClient> queueClients;

	SECTION("Add Clients") {
		REQUIRE(queueClients.isEmpty());
		queueClients.enqueue(client1);
		queueClients.enqueue(client2);

		INFO("When we add client to the queue it must not be empty");
		REQUIRE(queueClients.isEmpty() == false);
	}
}

TEST_CASE("Front method in PriorityQueue")
{
	PriorityQueue<MyClient> queueClients;

	SECTION("Front element by queue if it is empty") {
		bool exceptionThrown = false;

		try {
			MyClient frontElement = queueClients.front();
		}
		catch (std::underflow_error) { // special exception type		
			exceptionThrown = true;
		}
		catch (...) {
			exceptionThrown = true;
		}

		INFO("Front method has to throw underflow error exception if queue is empty and we want to use this method");
		REQUIRE(exceptionThrown);
	}

	SECTION("Front element from queue if it is not empty") {
		MyClient client1(1, Client{ 0, 10, 10, 20 });
		MyClient client2(2, Client{ 0, 10, 10, 20 });

		queueClients.enqueue(client2);
		queueClients.enqueue(client1);

		INFO("Front method has to return first added element");
		REQUIRE(queueClients.isEmpty() == false);
		REQUIRE(queueClients.front().getIndex() == 1);
	}
}

TEST_CASE("Removing a new element in PriorityQueue")
{
	PriorityQueue<MyClient> queueClients;

	SECTION("Dequeue one client") {
		queueClients.enqueue(MyClient(1, Client{ 0, 10, 10, 20 }));
		REQUIRE(queueClients.isEmpty() == false);
		queueClients.dequeue();
		REQUIRE(queueClients.isEmpty());

	}
	SECTION("Dequeue many clients")
	{
		queueClients.enqueue(MyClient(1, Client{ 0, 10, 10, 20 }));
		queueClients.enqueue(MyClient(2, Client{ 0, 10, 10, 20 }));
		queueClients.enqueue(MyClient(3, Client{ 0, 10, 10, 20 }));

		REQUIRE(queueClients.isEmpty() == false);
		queueClients.dequeue();
		queueClients.dequeue();
		queueClients.dequeue();

		REQUIRE(queueClients.isEmpty());
	}
}

TEST_CASE("Big Four in PriorityQueue")
{
	SECTION("Default constructor")
	{
		PriorityQueue<MyClient> queueClients;

		REQUIRE(queueClients.isEmpty());
	}
	SECTION("Copy Constructor")
	{
		PriorityQueue<MyClient> queueClients;
		for (size_t i = 0; i <= 60; i += 30)
		{
			queueClients.enqueue(MyClient(i, Client{ 0, 10, 10, 20 }));
		}

		REQUIRE(queueClients.isEmpty() == false);
		PriorityQueue<MyClient> copyQueueClients(queueClients);
		REQUIRE(copyQueueClients.isEmpty() == false);

		for (size_t i = 0; i < 3; i++)
		{
			REQUIRE(copyQueueClients.front().getIndex() == queueClients.front().getIndex());
			queueClients.dequeue();
			copyQueueClients.dequeue();
		}
		REQUIRE(queueClients.isEmpty() == copyQueueClients.isEmpty());
	}
	SECTION("Destructor")
	{
		PriorityQueue<MyClient> queueClients;

		queueClients.enqueue(MyClient(1, Client{ 0, 10, 10, 20 }));
		queueClients.enqueue(MyClient(2, Client{ 0, 10, 10, 20 }));
		queueClients.enqueue(MyClient(3, Client{ 0, 10, 10, 20 }));

		REQUIRE(queueClients.isEmpty() == false);
		queueClients.~PriorityQueue();
		INFO("The queue must be empty after calling the destructor");
		REQUIRE(queueClients.isEmpty());
	}
}
#pragma endregion

 
#pragma region Entities Tests
TEST_CASE("Worker functionality") {
	Worker bananaWorker(10, ResourceType::banana);
	Worker schweppesWorker(10, ResourceType::schweppes);

	SECTION("Worker back minute, resource type")
	{
		REQUIRE(bananaWorker.getResourceType() == ResourceType::banana);
		REQUIRE(schweppesWorker.getResourceType() == ResourceType::schweppes);
		REQUIRE(bananaWorker.getBackMinute() == schweppesWorker.getBackMinute());
	}
}

TEST_CASE("MyClient Constructors") {

	SECTION("MyClient default constructors")
	{
		MyClient defaultClient;
		INFO("Default value for index is -1")
		REQUIRE(defaultClient.getIndex() == -1);
		REQUIRE(defaultClient.arriveMinute == 0);
		REQUIRE(defaultClient.banana == 0);
		REQUIRE(defaultClient.schweppes == 0);
		REQUIRE(defaultClient.maxWaitTime == 0);	
		REQUIRE(defaultClient.getDepartTime() == 0);
	}
	SECTION("MyClient parametric constructor ")
	{
		MyClient paramClient(3, Client{ 0, 10, 10, 20 });
		REQUIRE(paramClient.getIndex() == 3);
		REQUIRE(paramClient.arriveMinute == 0);
		REQUIRE(paramClient.banana == 10);
		REQUIRE(paramClient.schweppes == 10);
		REQUIRE(paramClient.maxWaitTime == 20);
		REQUIRE(paramClient.getDepartTime() == 20);
	}
}
TEST_CASE("MyClient operators") {
	SECTION("MyClient operator =")
	{
		MyClient client1(2, Client{ 0, 10, 10, 20 });
		MyClient client2 = client1;
		REQUIRE(client2.getIndex() == client1.getIndex());
		REQUIRE(client2.arriveMinute == client1.arriveMinute);
		REQUIRE(client2.banana == client1.banana);
		REQUIRE(client2.schweppes == client1.schweppes);
		REQUIRE(client2.maxWaitTime == client1.maxWaitTime);
		REQUIRE(client2.getDepartTime() == client1.getDepartTime());
	}
	SECTION("MyClient operator >, smaller index")
	{
		MyClient client1(2, Client{ 0, 10, 10, 20 });
		MyClient client2(3, Client{ 0, 10, 10, 20 });
		INFO("Clients with equal arriveMinute + maxWaitTime are compared by index");
		REQUIRE((client1 > client2));
	}

	SECTION("MyClient operator >, smaller arriveMinute and maxWaitTime")
	{
		MyClient client1(5, Client{ 0, 10, 10, 1 });
		MyClient client2(3, Client{ 0, 10, 10, 20 });
		INFO("Client with smaller arriveMinute + maxWaitTime");
		REQUIRE((client1 > client2));
	}
	SECTION("MyClient operator >, smaller arriveMinute and maxWaitTime")
	{
		MyClient client1(5, Client{ 0, 10, 10, 100 });
		MyClient client2(3, Client{ 0, 10, 10, 20 });
		INFO("Client with bigger arriveMinute + maxWaitTime");
		REQUIRE((client1 > client2) == false);
	}
}
#pragma endregion
