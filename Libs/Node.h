#pragma once
#ifndef NODE_HPP
#define NODE_HPP

#include "Define.h"

template<typename T>
class Node {
    private:
        u_int32 _key;
        T* _data;
        Node* _next;

    public:
        /*
            Constructor:
            Initialize the data and key of the node
        */
        Node(T d, u_int32 k): _key(k),  _data(new T(d)), _next(nullptr) {}

        /*
            Return the data of the node
        */
        T* data() const {
            return this->_data;
        }

        /*
            Return the next node
        */
        Node* next() const {
            return this->_next;
        }

        /*
            Param: Node* node
            Set the next node
        */
        void setNext(Node* node) {
            this->_next = node;
        }

        /*
            Return the key of the node
        */
        u_int32 key() const {
            return this->_key;
        }
};

#endif
