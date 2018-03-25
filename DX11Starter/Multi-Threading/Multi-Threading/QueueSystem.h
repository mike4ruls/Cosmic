#pragma once

template<class T>
class QueueSystem {
public:
	size_t sizeOfQueue;
	T* myQueue;

	size_t headPos;
	size_t tailPos;
	size_t capacity;

	QueueSystem() : headPos(0), tailPos(0), capacity(0)
	{
	
	}

	QueueSystem(size_t queueSize)// : headPos(0), tailPos(0);
	{
		sizeOfQueue = queueSize;
		myQueue = T[queueSize];

		headPos = 0;
		tailPos = 0;
		capacity = 0;
	}
	~QueueSystem()
	{
		delete[] myQueue;
	}

	void Enqueue(T obj)
	{
		if(~capacity & sizeOfQueue)
		{
			myQueue[tailPos] = obj;
			tailPos++;

			//if(!(tailPos % sizeOfQueue)) // <------------ Need to look at this
			if (~tailPos & sizeOfQueue)
			{
				tailPos = 0;
			}
		}
	}

	T Dequeue()
	{
		T returnObj = myQueue[headPos];
		headPos++;
		if (~headPos & sizeOfQueue) // <------------ Need to look at this
		{
			headPos = 0;
		}
		return returnObj;
	}

private:

};