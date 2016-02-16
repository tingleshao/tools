#include "ThreadSafeQueue.h"
#include <iostream>
#include <cstdio>
#include <ctime>
#include <random>

namespace atl
{
   /**
   * @brief Destructor.  Deletes all nodes in the queue INCLUDING THEIR DATA
   */
   template<class T> ThreadSafeQueue<T>::~ThreadSafeQueue()
   {
       delete_all();
   }
   
   /**
   * @brief Deletes all nodes in the queue INCLUDING THEIR DATA
   */
   template<class T> void ThreadSafeQueue<T>::delete_all()
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
   template<class T> bool ThreadSafeQueue<T>::enqueue(T* data, bool force)
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
   * @param blocking If empty, true blocks until queue is non-empty, false return nullptr immediately.
   * @param timeout How long to block before timeout in milliseconds.  0 for indefinite wait.
   *
   * @return The data contained in the head
   */
   template<class T> T* ThreadSafeQueue<T>::dequeue(bool blocking, uint16_t timeout)
   {
       std::unique_lock<std::mutex> lock(m);
       if(!blocking && length == 0) {
           return nullptr;
       }
       if(timeout == 0) {
           cv.wait(lock, [this]{return length > 0;});
       } else {
          if(!cv.wait_for(lock, std::chrono::milliseconds(timeout), [this]{return length > 0;})) {
               std::cerr << "Thread timed out with no data on queue.  Returning null" << std::endl;
               return nullptr;
           }
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
   template<class T> bool ThreadSafeQueue<T>::push(T* data, bool force)
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
   * @param blocking If empty, true blocks until queue is non-empty, false return nullptr immediately.
   * @param timeout How long to block before timeout in milliseconds.  0 for indefinite wait.
   *
   * @return The data contained in the head
   */
   template<class T> T* ThreadSafeQueue<T>::pop(bool blocking, uint16_t timeout)
   {
       return dequeue(blocking, timeout);
   }
   
   /**
   * @brief Returns the data in the head of the queue without removing it
   *
   * @return The data in the head of the queue
   */
   template<class T> T* ThreadSafeQueue<T>::peek()
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
   template<class T> size_t ThreadSafeQueue<T>::size()
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
   template<class T> void ThreadSafeQueue<T>::set_max_size(size_t size)
   {
       std::lock_guard<std::mutex> lock(m);
       max_size = size;
   }
   
   /**
   * @brief Returns the number of items in the queue
   *
   * @return the number of items in the queue
   */
   
   template<class T> size_t ThreadSafeQueue<T>::get_max_size()
   {
       std::lock_guard<std::mutex> lock(m);
       return max_size;
   }
   
   /**
   * @brief Test: prints the size of the queue for debugging
   *
   * @param q The queue
   * @param print False suppresses output
   */
   void print_size(ThreadSafeQueue<int> &q, bool print)
   {
       if(print) {
           std::cout << "Queue length: " << q.size() << std::endl;
       } else {
           int i = q.size();
       }
   }
   
   /**
   * @brief Test: adds a number to the queue and prints for debugging
   *
   * @param q the queue
   * @param num the number
   * @param print False suppresses output
   */
   void add_to_queue(ThreadSafeQueue<int> &q, int num, bool print)
   {
       int* number = new int(num);
       if(print) {
           std::cout << "Adding " << num << " to queue" << std::endl;
       }
       q.enqueue(number);
   }
   
   /**
   * @brief Removes the head of the queue and prints it for debugging
   *
   * @param q The queue
   * @param print False suppresses output
   */
   void remove_from_queue(ThreadSafeQueue<int> &q, bool print)
   {
       std::cout<< "Removing from queue" << std::endl;
       int* num = q.dequeue(true, 1000);
       if(print) {
           if(num != nullptr) {
               std::cout << "Removed " << *num << " from queue" << std::endl;
           } else {
               std::cerr << "Could not remove null element" << std::endl;
           }
       }
       delete num;
   }
   
   /**
   * @brief Peeks at the head element in the queue
   *
   * @param q The queue
   * @param print False suppresses output
   */
   void peek_at_queue(ThreadSafeQueue<int> &q, bool print)
   {
       std::cout << "Peeking" << std::endl;
       int* num = q.peek();
       if(print) {
           if(num != nullptr) {
               std::cout << "Head: " << *num << std::endl;
           } else {
               std::cerr << "Head is null" << std::endl;
           }
       }
   }
   
   /**
   * @brief Pushes an element to the head of the queue
   *
   * @param q The queue
   * @param print False suppresses output
   */
   void push_to_queue(ThreadSafeQueue<int> &q, int num, bool print)
   {
       int* number = new int(num);
       if(print) {
           std::cout << "Adding " << num << " to queue" << std::endl;
       }
       q.push(number);
   }
   
   /**
   * @brief Pops an element off the head of the queue
   *
   * @param q The queue
   * @param print False suppresses output
   */
   void pop_from_queue(ThreadSafeQueue<int> &q, bool print)
   {
       std::cout << "Popping from queue" << std::endl;
       int* num = q.pop(false);
       if(print) {
           if(num != nullptr) {
               std::cout << "Popped " << *num << " from queue" << std::endl;
           } else {
               std::cerr << "Could not remove null element" << std::endl;
           }
       }
       delete num;
   }
   
   /**
   * @brief Tests the thread-safe queue with multiple threads
   *
   * @param numThreads The number of threads to spawn
   * @param print False supresses output
   *
   * @return true on success
   */
   bool test_ThreadSafeQueue(unsigned int numThreads, bool print)
   {
       ThreadSafeQueue<int> q;
   
       std::thread* t = new std::thread[numThreads];
       
       std::random_device rd;
       std::mt19937 gen(rd());
       std::uniform_int_distribution<> dis(1, 6);
   
       std::cout << "Default max size: " << q.get_max_size() << std::endl;
       q.set_max_size(10);
       std::cout << "New max size: " << q.get_max_size() << std::endl;
   
       std::clock_t start;
       double duration;
   
       std::cout << "Operations: " << numThreads << std::endl << "Threads: " << numThreads << std::endl;
   
       start = std::clock();
   
       for(int i = 0; i < numThreads; i++) {
           switch(dis(gen))
           {
               case 1:
                   t[i] = std::thread(add_to_queue, std::ref(q), i, print);
                   break;
               case 2:
                   t[i] = std::thread(remove_from_queue, std::ref(q), print);
                   break;
               case 3:
                   t[i] = std::thread(peek_at_queue, std::ref(q), print);
                   break;
               case 4:
                   t[i] = std::thread(print_size, std::ref(q), print);
                   break;
               case 5:
                   t[i] = std::thread(print_size, std::ref(q), print);
                   break;
               case 6:
                   t[i] = std::thread(print_size, std::ref(q), print);
                   break;
               default:
                   break;
           }
       }
   
       for(int i=0; i < numThreads; i++) {
           t[i].join();
       }
   
       duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
       std::cout << "Time: " << duration << std::endl;
   
       delete[] t;
   
       return true;
   }
}
