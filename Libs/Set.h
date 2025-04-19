#ifndef SET_H
#define SET_H

#include "Define.h"
#include <typeinfo>
#include <vector>

// Maximum count of patient in the system for each types
#define MAX_SIZE_OF_SET 10000

/*
    Class Set
    This class is used to store the data of patients in the system
    NOTE: USE THIS CLASS TO STORE PATIENT'S ID ONLY
*/
class Set {
    private:
        u_int32 _size;
        std::vector<u_int64> _data;

    public:
        /*
            Constructor
            NOTE: The size of the set is double the maximum size of the set because there are two types of patients OPD and IPD
        */
        Set(): _size(MAX_SIZE_OF_SET * 2) {
            this->_data.resize(_size, 0);
        }
        
        /*
            Destructor
            Clear the data of the set
        */
        ~Set() {
            this->_data.clear();
        }

        /*
            Add a given value to the set
            Parameters:
                T value: The value to be added
                UserType type: The type of the user (OPD or IPD)
        */
        void add(u_int64 id, UserType type) {
            if (this->contains(id, type)) return;
            u_int32 index = id % 1000;
            
            if (type == UserType::IPD) {
                index += MAX_SIZE_OF_SET;
            }
            
            this->_data[index] = 1;
        }
        
        /*
            Check if a given value is in the set
            Parameters:
                T value: The value to be removed
                UserType type: The type of the user (OPD or IPD)

            Returns: 1 if the value is exists, 0 if not
        */
        u_int8 contains(u_int64 id, UserType type) {
            u_int32 index = id % 1000;

            if (type == UserType::IPD) {
                index += MAX_SIZE_OF_SET;
            }
        
            if (this->_data[index] == 0) return 0;
            return 1;
        }
};

#endif
