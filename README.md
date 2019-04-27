
# Single Producer Multi Consumer Scheduler

An implementation of single producer multi consumer scheduler in C language using queue data structure.

## Installation
Execute the following commands to run the project:

    git clone https://github.com/faisal-zulfiqar/producer-consumer-scheduler.git
    
    cd producer-consumer-scheduler
    
    make scheduler
    
    ./scheduler 5

## Working

The make file first compliles and then runs a file called **random** which produces 20 random digits one per line in a file called **task_list.txt**. Each random digit is in a range of 1 - 5.

When `./scheduler 5` is run, the program allocates memory for a queue which can hold 5 integers. The size of the queue can range from 1 to 10. The program creates 4 threads in total namely *taskt*, *cpu_1*, *cpu_2*, and *cpu_3*. The *taskt* thread runs the **task** method while the other threads run **cpu** method. 

The **task** method reads from the **task_list.txt** file until it reaches the **EOF** then it inserts each scanned digit into the queue using enqueue method. When the EOF is reached and the while loop terminates, the method inserts three cpu bursts of value -1. This is done in order to terminate the three threads running the **cpu** method.

The **cpu** method simulates the operations of each of the three cpu's. If there's at least one task inside the queue, one of the cpu's will fetch the task from the queue and simulate the event of of executing the task for its entire burst using **sleep** call.

The **enqueue** method protects the *ciritical section* using **pthread_mutex_lock()** function and if the space is 0 or in other words if the queue is full then it waits for any cpu thread to fetch a task from it and make space for inserting the new task. This conditional wait is done using **pthread_cond_wait()** method and the code resumes execution from here once any other thread signals that this condition has been met using **pthread_cond_signal()** method, which in this case is done inside **dequeue** method. The **dequeue** method removes a task from the queue if the queue is not empty and returns it, otherwise it waits for the enqueue task to insert at least 1 task using **pthread_cond_wait()** method.
