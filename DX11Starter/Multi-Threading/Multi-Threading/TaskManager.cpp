#include "stdafx.h"
#include "TaskManager.h"



TaskManager::TaskManager(size_t queueSize)
{
	myTaskQueue = QueueSystem<Task>(queueSize);
	for(int i = 0; i < 8; i++)
	{
		myThreads[i] = std::thread();
	}
}

TaskManager::~TaskManager()
{
}
