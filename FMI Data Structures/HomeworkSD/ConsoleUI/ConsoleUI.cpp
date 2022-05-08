#include <iostream>

#include "..\src\interface.h"
#include <vector>

struct StoreUI : ActionHandler {
	Store* impl = nullptr;
	StoreUI()
		: impl(createStore()) {
		impl->setActionHandler(this);
	}

	~StoreUI() {
		delete impl;
	}

	StoreUI(const StoreUI&) = delete;
	StoreUI& operator=(const StoreUI&) = delete;

	void init(int workerCount, int startBanana, int startSchweppes) {
		impl->init(workerCount, startBanana, startSchweppes);
	}

	void advanceTo(int minute) {
		impl->advanceTo(minute);
	}

	void addClients(const std::vector<Client>& clients) {
		impl->addClients(clients.data(), clients.size());
	}

	void onWorkerSend(int minute, ResourceType resource) override {
		std::cout << 'W' << ' '<< minute << ' ';
		printBananaOrSchweppes(resource);	
	}

	void onWorkerBack(int minute, ResourceType resource) override {
		std::cout << 'D' << ' ' << minute << ' ';
		printBananaOrSchweppes(resource);
	}

	void onClientDepart(int index, int minute, int banana, int schweppes) override {
		std::cout << index << ' ' << minute << ' ' << banana << ' ' << schweppes << std::endl;
	}
private:
	void printBananaOrSchweppes(const ResourceType resource)
	{
		if (resource == ResourceType::banana){
			std::cout << "banana" << std::endl;
		}
		else {
			std::cout << "schweppes" << std::endl;
		}
	}

};

int main()
{
	std::vector<Client> clientList;
	int workers, clientsCount;
	std::cin >> workers >> clientsCount;
	for (size_t i = 0; i < clientsCount; i++)
	{
		int arriveMinute, bananas, schweppes, maxWaitTime;
		std::cin >> arriveMinute >> bananas >> schweppes >> maxWaitTime;
		clientList.push_back(Client{ arriveMinute ,bananas , schweppes,maxWaitTime });
	}

	StoreUI store;
	store.init(workers, 0, 0);

	store.addClients(clientList);
	std::cout<<std::endl;
	store.advanceTo(INT_MAX);
}