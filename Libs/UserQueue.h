#ifndef USER_QUEUE_H
#define USER_QUEUE_H

#include "Queue.h"
#include "Logger.h"
#include <fstream>

class UserQueue : public Queue<u_int64> {
    private:
        Logger logger;

    public:
        UserQueue() {};

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
