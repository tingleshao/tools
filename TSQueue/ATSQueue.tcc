#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
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
template <class T> class ATSQueue
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
        ~ATSQueue();                     //<! Destructor.  Deletes all data in queue
        bool enqueue(T*, bool force=false);     //<! Add data to the tail of the queue
        T* dequeue(uint16_t timeout=0);
                                                //<! Remove and return data from the head of the queue
        bool push(T*, bool force=false);        //<! Add data to the head of the queue (as a stack)
        T* pop(uint16_t timeout=0);
                                                //<! Pop data off the head of the queue (as a stack)
        T* peek();                              //<! Peek at the head of the queue
        size_t size();                          //<! Return the size of the queue
        void delete_all();                      //<! Deletes all nodes in the queue
        void set_max_size(size_t);              //<! Sets max size
        size_t get_max_size();                  //<! Returns max size
};

bool test_ATSQueue(unsigned int numThreads=20, bool print=true);

/**
* @brief Node struct for linked list
*/
template<class T> struct ATSQueue<T>::QNode
{
    QNode(T* new_data)
    {
        data = new_data;
    }
    T* data = nullptr;
    QNode* next = nullptr;
    QNode* prev = nullptr;
};

/**
* @brief Destructor.  Deletes all nodes in the queue INCLUDING THEIR DATA
*/
template<class T> ATSQueue<T>::~ATSQueue()
{
    delete_all();
}

/**
* @brief Deletes all nodes in the queue INCLUDING THEIR DATA
*/
template<class T> void ATSQueue<T>::delete_all()
{
    QNode* temp;
    while(head != nullptr)
    {
        temp = head;
        head = head->prev;
        if(temp->data != nullptr) {
            delete temp->data;
        }
        delete temp;
    }
    tail = nullptr;
}

/**
* @brief Adds a node to the tail of the queue
*
* @param data The data to be contained in the Node
* @param force True will push data even if the length is greater than max_size
*/
template<class T> bool ATSQueue<T>::enqueue(T* data, bool force)
{
    std::unique_lock<std::mutex> lock(m);
    if(!force && length >= max_size)
        return false;
    QNode* temp = new QNode(data);
    if(tail != nullptr) {
        tail->prev = temp;
        temp->next = tail;
        tail = temp;
    } else {
        tail = temp;
        head = temp;
    }
    length++;
    lock.unlock();
    cv.notify_one();
    return true;
}

/**
* @brief Removes and returns the head of the queue
* @param timeout How long to block before timeout in milliseconds.
*
* @return The data contained in the head
*/
template<class T> T* ATSQueue<T>::dequeue(uint16_t timeout)
{
    std::unique_lock<std::mutex> lock(m);
    if(!cv.wait_for(lock, std::chrono::milliseconds(timeout), [this]{return length > 0;})) {
            std::cerr << "Thread timed out with no data on queue.  Returning null" << std::endl;
            return nullptr;
    }
    QNode* temp = head;
    head = head->prev;
    if(head != nullptr) {
        head->next = nullptr;
    } else {
        tail = nullptr;
    }
    T* data = temp->data;
    delete temp;
    length--;
    return data;
}

/**
* @brief Adds a node to the head of the queue (as a stack)
*
* @param data The data to be contained in the Node
* @param force True will push data even if the length is greater than max_size
*/
template<class T> bool ATSQueue<T>::push(T* data, bool force)
{
    std::unique_lock<std::mutex> lock(m);
    if(!force && length >= max_size)
        return false;
    QNode* temp = new QNode(data);
    if(head != nullptr) {
        head->next = temp;
        temp->prev = head;
        head = temp;
    } else {
        head = temp;
        tail = temp;
    }
    length++;
    lock.unlock();
    cv.notify_one();
    return true;
}

/**
* @brief Removes and returns the head of the queue (stack notation)
* @param timeout How long to block before timeout in milliseconds.
*
* @return The data contained in the head
*/
template<class T> T* ATSQueue<T>::pop(uint16_t timeout)
{
    return dequeue(timeout);
}

/**
* @brief Returns the data in the head of the queue without removing it
*
* @return The data in the head of the queue
*/
template<class T> T* ATSQueue<T>::peek()
{
    std::lock_guard<std::mutex> lock(m);
    if(head != nullptr) {
        return head->data;
    } else {
        return nullptr;
    }
}

/**
* @brief Returns the number of items in the queue
*
* @return the number of items in the queue
*/
template<class T> size_t ATSQueue<T>::size()
{
    std::lock_guard<std::mutex> lock(m);
    return length;
}

/**
* @brief Sets the maximum size of the queue.  If the queue is currently
*           longer than the max size, the queue will not be modified,
*           but no more elements will be able to be added until the queue
*           is shorter than the max_size
*/
template<class T> void ATSQueue<T>::set_max_size(size_t size)
{
    std::lock_guard<std::mutex> lock(m);
    max_size = size;
}

/**
* @brief Returns the number of items in the queue
*
* @return the number of items in the queue
*/

template<class T> size_t ATSQueue<T>::get_max_size()
{
    std::lock_guard<std::mutex> lock(m);
    return max_size;
}
