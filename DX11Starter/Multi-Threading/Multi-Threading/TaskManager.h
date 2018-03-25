#pragma once
#include <stdio.h>
#include <thread>
#include "QueueSystem.h"
#include "Task.h"

class TaskManager
{
public:
	QueueSystem<Task> myTaskQueue;

	uint8_t freeThreads;
	std::thread myThreads[8];

	TaskManager(size_t queueSize);
	~TaskManager();
};

