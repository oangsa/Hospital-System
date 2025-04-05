#pragma once
#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "Node.h"
#include "Define.h"
#include <iostream>

using namespace std;

template<typename T>
class Queue {
    private:
        Node<T>* head;
        Node<T>* tail;
        u_int32 _size;

    public:
        /*
            Constructor:
            Initialize the head, tail, and size of the queue
        */
        Queue(): head(NULL), tail(NULL), _size(0) {}

        /*
            Param: T data
            Enqueue the data to the queue
        */
        void enqueue(T data) {
            Node<T> *newNode = new Node<T>(data, 0);

            if (head == NULL) {
                head = newNode;
                tail = newNode;
            }
            else {
                tail->setNext(newNode);
                tail = newNode;
            }

            _size++;
        }

        /*
            Dequeue the first element in the queue
            return the pointer of the data
            user must delete the pointer after using it
        */
        T* dequeue() {
            if (head == NULL) {
                throw "Queue is empty";
            }

            Node<T> *temp = head;
            T* data = new T(*temp->data());

            head = head->next();
            delete temp;

            _size--;

            return data;
        }

        /*
            Return the first element in the queue
            return the pointer of the data
            user must not delete the pointer after using it
        */
        T* peek() {
            if (head == NULL) {
                throw "Queue is empty";
            }

            return head->data();
        }

        /*
            Return the size of the queue
        */
        u_int32 size() {
            return _size;
        }

        /*
            Return true if the queue is empty
        */
        u_int16 isEmpty() {
            return _size == 0;
        }
};

#endif
