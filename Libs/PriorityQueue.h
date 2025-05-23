#pragma once
#ifndef PRIORIY_QUEUE_HPP
#define PRIORIY_QUEUE_HPP

#include <vector>
#include <iostream>
#include <string>
#include "Node.h"
#include "Define.h"

using namespace std;

template <typename T>
class PriorityQueue {
    protected:
        Node<T>* head;
        Node<T>* tail;
        u_int32 _size;

    public:
        /*
            Constructor:
            Initialize the head, tail, and size of the queue
        */
        PriorityQueue(): head(nullptr), tail(nullptr), _size(0) {}

        /*
            Destructor:
            Delete all the nodes in the queue
        */
        ~PriorityQueue() {
            while (head != nullptr) {
                Node<T> *temp = head;
                head = head->next();
                delete temp;
            }
        }
        /*
            Param: T data, int priority
            Enqueue the data to the queue
        */
        void enqueue(T data, u_int32 priority) {
            Node<T> *newNode = new Node<T>(data, priority);

            // Note: The priority is in ascending order
            // Note: Use key() to get the priority of the node
            // Note: For patient we use ESI level as the priority

            if (head == nullptr) {
                head = newNode;
                tail = newNode;
            }
            else {
                Node<T>* current = head;
                Node<T>* prev = nullptr;

                while (current != nullptr && current->key() <= priority) {
                    prev = current;
                    current = current->next();
                }

                if (prev == nullptr) {
                    newNode->setNext(head);
                    head = newNode;
                }
                else {
                    prev->setNext(newNode);
                    newNode->setNext(current);
                }
            }

            _size++;
        }

        /*
            Dequeue the first element in the queue
            return the pointer of the data
            user must delete the pointer after using it
        */
        T* dequeue() {
            if (this->empty()) {
                return nullptr;
            }

            Node<T> *temp = head;
            head = head->next();

            T* data = new T(*temp->data());
            delete temp;

            _size--;

            return data;
        }

        /*
            Return the first element in the queue
            return the pointer of the data
            user don't have to delete the pointer after using it
        */
        T* peek() {
            if (this->empty()) {
                return nullptr;
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
        u_int16 empty() {
            return _size == 0;
        }
};

#endif
