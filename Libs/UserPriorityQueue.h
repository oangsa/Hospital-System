#ifndef USER_PRIORITY_QUEUE_H
#define USER_PRIORITY_QUEUE_H

#include "Define.h"
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

        u_int8 has(u_int64 id) {
            Node<u_int64>* tmp = this->head;

            while (tmp) {
                if (*tmp->data() == id) return 1;
                tmp = tmp->next();
            }

            return 0;
        }

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
