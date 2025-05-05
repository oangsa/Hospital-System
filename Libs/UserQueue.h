#ifndef USER_QUEUE_H
#define USER_QUEUE_H

#include "Queue.h"
#include "Logger.h"
#include <fstream>

// UserQueue class derived from Queue class
// It is used to manage a queue of user IDs
class UserQueue : public Queue<u_int64> {
    private:
        Logger logger;

    public:
        /* Constructor: Initialize the head, tail, and size of the queue */
        UserQueue() {};

        /*
            Save the queue to a file
            The file is saved in the format of "Database/Temp/_PaQ.csv"
            Each line contains a user ID
        */
        void saveToFile() {
            Node<u_int64>* cur = this->head;
            std::ofstream file;
            file.open("Database/Temp/_PaQ.csv", std::ios::out | std::ios::trunc);

            while (cur && file.is_open()) {
                logger.log("%llu", *(cur->data()));
                file << *cur->data() << "\n";
                cur = cur->next();
            }
            file.close();
        }
};

#endif
