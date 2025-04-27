#ifndef USER_PRIORITY_QUEUE_H
#define USER_PRIORITY_QUEUE_H

#include "PriorityQueue.h"
#include "Logger.h"
#include <fstream>

class UserPriorityQueue : public PriorityQueue<u_int64> {
    private:
        Logger logger;

    public:
        UserPriorityQueue() {};

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
};

#endif
