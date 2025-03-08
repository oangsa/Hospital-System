// #pragma once
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
        Node(T d, int k) {
            this->_data = new T(d);
            this->_key = k;
            this->_next = NULL;
        }

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
        T* data() {
            return _data;
        }

        /*
            Return the next node
        */
        Node* next() {
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
        int key() {
            return _key;
        }
};

#endif
