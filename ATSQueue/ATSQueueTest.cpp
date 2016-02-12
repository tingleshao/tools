#include "ATSQueue.tcc"
#include <cstdio>
#include <ctime>
#include <random>

/**
* @brief Test: prints the size of the queue for debugging
*
* @param q The queue
* @param print False suppresses output
*/
void print_size(ATSQueue<int> &q, bool print)
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
void add_to_queue(ATSQueue<int> &q, int num, bool print)
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
void remove_from_queue(ATSQueue<int> &q, bool print)
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
void peek_at_queue(ATSQueue<int> &q, bool print)
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
void push_to_queue(ATSQueue<int> &q, int num, bool print)
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
void pop_from_queue(ATSQueue<int> &q, bool print)
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
bool test_ATSQueue(unsigned int numThreads, bool print)
{
    ATSQueue<int> q;

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

int main(int, char**)
{
    if(!test_ATSQueue()) {
        std::cout << "ATSQueue test failed." << std::endl;
        return 0;
    } else {
        std::cout << "ATSQueue test passed." << std::endl;
        return 1;
    }
}
