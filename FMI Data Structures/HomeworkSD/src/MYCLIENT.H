#pragma once
#include "interface.h"

struct MyClient : Client {
private:
	// The index of the client
	int index;
public:
	MyClient(const int index, const Client& client){
		this->index = index;
		this->arriveMinute = client.arriveMinute;
		this->banana = client.banana;
		this->schweppes = client.schweppes;
		this->maxWaitTime = client.maxWaitTime;
	}
	MyClient(){
		this->index = -1;
		this->arriveMinute = 0;
		this->banana = 0;
		this->schweppes = 0;
		this->maxWaitTime = 0;
	}
	MyClient& operator=(const MyClient& other){
		if (this != &other)
		{
			this->index = other.index;
			this->arriveMinute = other.arriveMinute;
			this->banana = other.banana;
			this->schweppes = other.schweppes;
			this->maxWaitTime = other.maxWaitTime;
		}
		return *this;
	}
	bool operator>(const MyClient& other){
		if (this->arriveMinute + this->maxWaitTime == other.arriveMinute + other.maxWaitTime)
		{
			/*
			if (this->banana > other.banana)
			{
				return true;
			}

			if (this->schweppes > other.schweppes)
			{
				return true;
			}*/

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

	const int getIndex() const {
		return index;
	}
	const int getDepartTime() const {
		return arriveMinute + maxWaitTime;
	}
};