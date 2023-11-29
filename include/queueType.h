#ifndef H_queueType
#define H_queueType

#include <iostream>
#include <cassert>

#include "queueADT.h"

template <class Type>
class queueType: public queueADT<Type>
{
public:
    const queueType<Type>& operator=(const queueType<Type>&);
    //Overload the assignment operator.
    
    bool isEmptyQueue() const;
    //Function to determine whether the queue is empty.
    //Postcondition: Returns true if the queue is empty,
    //               otherwise returns false.
    
    bool isFullQueue() const;
    //Function to determine whether the queue is full.
    //Postcondition: Returns true if the queue is full,
    //               otherwise returns false.
    
    void initializeQueue();
    //Function to initialize the queue to an empty state.
    //Postcondition: The queue is empty.
    
    Type front() const;
    //Function to return the first element of the queue.
    //Precondition: The queue exists and is not empty.
    //Postcondition: If the queue is empty, the program
    //               terminates; otherwise, the first
    //               element of the queue is returned.
    Type back() const;
    //Function to return the last element of the queue.
    //Precondition: The queue exists and is not empty.
    //Postcondition: If the queue is empty, the program
    //               terminates; otherwise, the last
    //               element of the queue is returned.
    
    void addQueue(const Type& queueElement);
    //Function to add queueElement to the queue.
    //Precondition: The queue exists and is not full.
    //Postcondition: The queue is changed and queueElement
    //               is added to the queue.
    
    void deleteQueue();
    //Function to remove the first element of the queue.
    //Precondition: The queue exists and is not empty.
    //Postcondition: The queue is changed and the first
    //               element is removed from the queue.

    void moveNthFront(int n);
    
    queueType(int queueSize = 100);
    //Constructor
    
    queueType(queueType<Type>& otherQueue);
    //Copy constructor
    
    ~queueType();
    //Destructor
    
private:
    int maxQueueSize; //variable to store the maximum queue size
    int count;        //variable to store the number of
    //elements in the queue
    int queueFront;   //variable to point to the first
    //element of the queue
    int queueRear;    //variable to point to the last
    //element of the queue
    Type *list;       //pointer to the array that holds
    //the queue elements
};

template <class Type>
bool queueType<Type>::isEmptyQueue() const
{
    return (count == 0);
} //end isEmptyQueue

template <class Type>
bool queueType<Type>::isFullQueue() const
{
    return (count == maxQueueSize);
} //end isFullQueue

template <class Type>
void queueType<Type>::initializeQueue()
{
    queueFront = 0;
    queueRear = maxQueueSize - 1;
    count = 0;
} //end initializeQueue

template <class Type>
Type queueType<Type>::front() const
{
    assert(!isEmptyQueue());
    return list[queueFront];
} //end front

template <class Type>
Type queueType<Type>::back() const
{
    assert(!isEmptyQueue());
    return list[queueRear];
} //end back

template <class Type>
void queueType<Type>::addQueue(const Type& newElement)
{
    if (!isFullQueue())
    {
        queueRear = (queueRear + 1) % maxQueueSize; //use mod
        //operator to advance queueRear
        //because the array is circular
        count++;
        list[queueRear] = newElement;
    }
    else
        std::cout << "Cannot add to a full queue." << std::endl;
} //end addQueue

template <class Type>
void queueType<Type>::deleteQueue()
{
    if (!isEmptyQueue())
    {
        count--;
        queueFront = (queueFront + 1) % maxQueueSize; //use the
        //mod operator to advance queueFront
        //because the array is circular
    }
    else {
        std::cout << "Cannot remove from an empty queue." << std::endl;
    }
} //end deleteQueue

template <class Type>
void queueType<Type>::moveNthFront(int n) 
{
    if (!isEmptyQueue() && n >= -1)
    {
        if (n-2 < queueRear-queueFront) {
            Type val = list[queueFront + n - 1];
            for (int i = (queueFront + n - 1); i > queueFront; i--) {
                list[i] = list[i - 1];
            }
            list[queueFront] = val;
        }   else {
            std::cout << "Invalid n. There are not enough entries in the queue!\n";
        }
    }
}

//Constructor
template <class Type>
queueType<Type>::queueType(int queueSize)
{
    if (queueSize <= 0)
    {
        std::cout << "Size of the array to hold the queue must "
        << "be positive." << std::endl;
        std::cout << "Creating an array of size 100." << std::endl;
        
        maxQueueSize = 100;
    }
    else
        maxQueueSize = queueSize;   //set maxQueueSize to
    //queueSize
    
    queueFront = 0;                 //initialize queueFront
    queueRear = maxQueueSize - 1;   //initialize queueRear
    count = 0;
    list = new Type[maxQueueSize];  //create the array to
    //hold the queue elements
} //end constructor

//Destructor
template <class Type>
queueType<Type>::~queueType()
{
    delete [] list;
} //end destructor

template <class Type>
const queueType<Type>& queueType<Type>::operator=
(const queueType<Type>& otherQueue)
{
    auto q = queueType<Type>(otherQueue.maxQueueSize);
    //->maxQueueSize = otherQueue.maxQueueSize;
    q.queueFront = otherQueue.queueFront;
    q->list = new Type[maxQueueSize];
    for (int i = 0; i <= maxQueueSize; i++)
        q->list[i] = otherQueue.list[i];
    q.queueRear = otherQueue.queueRear;
    q.count = otherQueue.count;
    return q;
} //end assignment operator

template <class Type>
queueType<Type>::queueType(queueType<Type>& otherQueue)
{
    this->maxQueueSize = otherQueue.maxQueueSize;
    this->queueFront = otherQueue.queueFront;
    this->list = new Type[maxQueueSize];
    for (int i = 0; i <= maxQueueSize; i++)
        this->list[i] = otherQueue.list[i];
    this->queueRear = otherQueue.queueRear;
    this->count = otherQueue.count;
} //end copy constructor

#endif
