#pragma once
#include "interface.h"

struct Worker {
private:
	int sentMinute;
	ResourceType type;
public:
	Worker(const int sentMinute, const ResourceType type)
	{
		this->sentMinute = sentMinute;
		this->type = type;
	}

	const int getBackMinute() const
	{
		return sentMinute + 60;
	}
	const ResourceType getResourceType() const
	{
		return type;
	}
};