#ifndef STACK_H
#define STACK_H

#include "Node.h"
#include "Define.h"

template <typename T>
class Stack {
    private:
        Node<T>* _top;
        u_int32 _size;

    public:
        /*
            Constructor
            Initialize the stack with top as NULL and size as 0
        */
        Stack<T>(): _top(NULL), _size(0) {}

        /*
            Pop the top element from the stack
            Return the data of the top element
        */
        T* pop() {
            if (!this->_top) return NULL;

            Node<T>* temp = this->_top;
            T* data = temp->data();

            this->_top = this->_top->next();
            this->_size--;

            delete temp;

            return data;
        }

        /*
            Push a new element to the top of the stack
            Parameters:
                T data: The data to be pushed
        */
        void push(T data) {
            Node<T>* newNode = new Node<T>(data, 0);
            if (!this->_top) {
                this->_top = newNode;
            }
            else {
                newNode->setNext(this->_top);
                this->_top = newNode;
            }
        }

        /*
            Peek the top element of the stack
            Return the data of the top element
        */
        T* peek() {
            if (this->_top) return this->_top->data();
            return NULL;
        }

        /*
            return the size of the stack
        */
        u_int32 size() {
            return this->_size;
        }

        /*
            Check if the stack is empty
            Return 1 if empty, 0 if not
        */
        u_int8 isEmpty() {
            return this->_top == NULL;
        }

};

#endif
