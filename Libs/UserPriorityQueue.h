#ifndef USER_PRIORITY_QUEUE_H
#define USER_PRIORITY_QUEUE_H

#include "Define.h"
#include "PriorityQueue.h"
#include "Logger.h"
#include <fstream>

// UserPriorityQueue class derived from PriorityQueue class
// It is used to manage a priority queue of user IDs
class UserPriorityQueue : public PriorityQueue<u_int64> {
    private:
        Logger logger;

    public:
        /*
            Constructor: Initialize the head, tail, and size of the queue
        */
        UserPriorityQueue() {};

        /*
            Save the queue to a file
            The file is saved in the format of "Database/Temp/PaPQ.csv"
        */
        void saveToFile() {
            Node<u_int64>* cur = this->head;
            std::ofstream file;
            file.open("Database/Temp/PaPQ.csv", std::ios::out | std::ios::trunc);

            while (cur && file.is_open()) {
                file << *cur->data() << "," << cur->key() << "\n";
                cur = cur->next();
            }
            file.close();
        }

        /*
            Check if the queue has the given ID
            Return 1 if the ID is found, else return 0
        */
        u_int8 has(u_int64 id) {
            Node<u_int64>* tmp = this->head;

            while (tmp) {
                if (*tmp->data() == id) return 1;
                tmp = tmp->next();
            }

            return 0;
        }

        /*
            Get the index of the given ID in the queue
            Return the index if found, else return -1
            NOTE: The index is 0-based
        */
        u_int32 getIndexOf(u_int64 id) {
            Node<u_int64>* tmp = this->head;
            u_int32 ctr = 0;

            while (tmp) {
                if (*tmp->data() == id) return ctr;
                tmp = tmp->next();
                ctr++;
            }

            return -1;
        }
};

#endif
