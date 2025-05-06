/**************************************************
 *                                                *
 *     FileManager Class Implementation File      *
 *     Latest Update: May 5, 2025                 *
 *                                                *
 **************************************************/


#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include "FileManager.h"
#include <vector>
#include <sys/types.h>

// For Windows compatibility
#ifdef _WIN32
    #include <direct.h>
    #define mkdir(path, mode) _mkdir(path)  // Windows doesn't use mode
#else
    #include <unistd.h>
#endif

typedef struct stat stat_t;

using namespace std;

FileManager::FileManager(){}
FileManager::~FileManager(){}

/*
    Check if the file exists
    Params: string& filename
    return: 1 if file exists, 0 if not
*/
bool FileManager::isFileExists(string& filename) {
    stat_t st;
    return stat(filename.c_str(), &st) == 0;
}

/*
    Check if the directory exists
    Params: string& dirname
    return: 1 if directory exists, 0 if not
*/
bool FileManager::isDirExists(string& dirname) {
    stat_t st;
    return stat(dirname.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
}

/*
    Check all files and create them if they don't exist
    Params: None
    return: 1 if user cache file exists, 0 if not
 */
u_int8 FileManager::checkAllFilesAndCreate() {
    vector<string> dirnames = {
        "Database",
        "Database/Users",
        "Database/History",
        "Database/Cache",
        "Database/Informations",
        "Database/Temps"
    };

    vector<string> filenames = {
        "Database/Users/users.csv",
    };

    string cacheFile = "Database/Cache/user.csv";

    for (string dirname : dirnames) {
        if (!this->isDirExists(dirname)) {
            #ifdef _WIN32
                _mkdir(dirname.c_str());
            #else
                mkdir(dirname.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            #endif
        }
    }

    for (string filename : filenames) {
        if (!this->isFileExists(filename)) {
            FILE* file = fopen(filename.c_str(), "w+");
            fclose(file);
        }
    }

    if (!this->isFileExists(cacheFile)) {
        return 0;
    }

    if (this->isEmpty(cacheFile)) {
        return 0;
    }

    return 1;
}

/*
    Check if the file is empty
    Params: string& filename
    return: 1 if file is empty, 0 if not
*/
u_int8 FileManager::isEmpty(string& filename) {
    return std::filesystem::is_empty(filename);
}

/*
    Write the user ID to the cache file
    Params: u_int64 id
    return: None
*/
void FileManager::writeUserCache(u_int64 id) {
    fstream file;
    file.open("Database/Cache/user.csv", ios::out | ios::trunc);
    file << id;
    file.close();
}
