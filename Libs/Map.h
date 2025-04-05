#pragma once
#ifndef MAP_HPP
#define MAP_HPP

#include "Node.h"
#include "Define.h"
#include <vector>
#include <iostream>

using namespace std;

template<typename T>
class Map {
    private:
        u_int32 _size;
        vector<Node<T>*> dataMap;

    public:

        /*
            Constructor:
            Initialize the size of the map and create a vector of Node pointers
        */
        Map(u_int32 size): _size(size) {
            dataMap = vector<Node<T>*>(size, nullptr);
        }

        /*
            Param: int key, T value
            Put the value to the map with the key
        */
        void put(u_int64 key, T value) {
            u_int32 hash = key % _size;

            Node<T>* newNode = new Node<T>(value, key);

            if (dataMap[hash] == nullptr) {
                dataMap[hash] = newNode;
            }
            else {
                Node<T>* current = dataMap[hash];

                while (current->next() != nullptr) {
                    current = current->next();
                }

                current->setNext(newNode);
            }
        }

        /*
            Param: u_int64 key
            Return the value of the given key
            if the key is not found, return nullptr
        */
        T* get(u_int64 key) {
            u_int32 hash = key % _size;

            Node<T>* current = dataMap[hash];

            while (current != nullptr) {
                if (current->key() == key) {
                    return current->data();
                }

                current = current->next();
            }

            return nullptr;
        }

        /*
            Param u_int8 i
            Return Node<T>*
        */
        Node<T>* getList(u_int8 i) {
            return dataMap[i];
        }

        /*
            Param: int key
            Remove the value of the given key
        */
        void remove(u_int64 key) {
            u_int32 hash = key % _size;

            Node<T>* current = dataMap[hash];
            Node<T>* previous = nullptr;

            while (current != nullptr) {
                if (current->key() == key) {
                    if (previous == nullptr) {
                        dataMap[hash] = current->next();
                    }
                    else {
                        previous->setNext(current->next());
                    }

                    delete current;
                    return;
                }

                previous = current;
                current = current->next();
            }
        }

        /*
            Return the size of the map
        */
        u_int32 size() {
            return _size;
        }
};

#endif
