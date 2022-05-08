#pragma once
#include "interface.h"

struct MyClient : Client {
private:
	// The index of the client
	int index;
public:
	MyClient(const int index, const Client& client);
	MyClient(); // TODO: implement initilize constructor
	bool operator>(const MyClient& other);

	int getIndex() const;
};