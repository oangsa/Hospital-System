#pragma once
#ifndef NODE_HPP
#define NODE_HPP

template<typename T>
class Node {
    private:
        int _key;
        T* _data;
        Node* _next;

    public:
        /*
            Constructor:
            Initialize the data and key of the node
        */
        Node(T d, int k): _data(new T(d)), _key(k), _next(nullptr) {}

        /*
            Destructor:
            Delete the data of the node
        */
        ~Node() {
            delete _data;
        }

        /*
            Return the data of the node
        */
        T* data() const {
            return _data;
        }

        /*
            Return the next node
        */
        Node* next() const {
            return _next;
        }

        /*
            Param: Node* node
            Set the next node
        */
        void setNext(Node* node) {
            _next = node;
        }

        /*
            Return the key of the node
        */
        int key() const {
            return _key;
        }
};

#endif
