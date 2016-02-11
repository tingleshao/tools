#include <vector>
#include <mutex>
#include <condition_variable>

#pragma once

#define DEFAULT_MAX_SIZE -1

/**
* @brief A thread-safe queue for APL commands between threads
* 
* There must be a destructor for any class used in this template, or
* else there will be a memory leak if the Queue is deleted.
*
* @tparam T  The type of data to be contained in the queue
*/
template <class T> class ThreadSafeQueue
{
    private:
        std::mutex m;                           //<! The mutex that will be used for accessing the queue
        std::condition_variable cv;             //<! The condition variable which waits on blocking dequeue
        size_t length = 0;                      //<! The length of the queue
        struct QNode;                           //<! A simple linked list node
        QNode* head = nullptr;                  //<! The head of the queue
        QNode* tail = nullptr;                  //<! The tail of the queue
        size_t max_size = DEFAULT_MAX_SIZE;     //<! Maximum size of queue

    public:
        ~ThreadSafeQueue();                     //<! Destructor.  Deletes all data in queue
        bool enqueue(T*, bool force=false);     //<! Add data to the tail of the queue
        T* dequeue(bool blocking=true, uint16_t timeout=0);
                                                //<! Remove and return data from the head of the queue
        bool push(T*, bool force=false);        //<! Add data to the head of the queue (as a stack)
        T* pop(bool blocking=true, uint16_t timeout=0);
                                                //<! Pop data off the head of the queue (as a stack)
        T* peek();                              //<! Peek at the head of the queue
        size_t size();                          //<! Return the size of the queue
        void delete_all();                      //<! Deletes all nodes in the queue
        void set_max_size(size_t);              //<! Sets max size
        size_t get_max_size();                  //<! Returns max size
};

bool test_ThreadSafeQueue(unsigned int numThreads=20, bool print=true);

/**
* @brief Node struct for linked list
*/
template<class T> struct ThreadSafeQueue<T>::QNode
{
    QNode(T* new_data)
    {
        data = new_data;
    }
    T* data = nullptr;
    QNode* next = nullptr;
    QNode* prev = nullptr;
};
