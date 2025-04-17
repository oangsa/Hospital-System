#include <string>
#include <string>
#include <sys/stat.h>
#include "FileManager.h"
#include <vector>
#include <vector>

typedef struct stat stat_t;

FileManager::FileManager(){}
FileManager::~FileManager(){}

// Don't ask me, go ask GPT
bool FileManager::isFileExists(string& filename) {
    stat_t st;
    return stat(filename.c_str(), &st) == 0;
}

// Don't ask me, go ask GPT
bool FileManager::isDirExists(string& dirname) {
    stat_t st;
    return stat(dirname.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
}

u_int8 FileManager::checkAllFilesAndCreate() {
    vector<string> dirnames = {
        "Database",
        "Database/Users",
        "Database/History"
    };

    vector<string> filenames = {
        "Database/Users/users.csv",
    };

    for (string dirname : dirnames) {
        if (!this->isDirExists(dirname)) {
            mkdir(dirname.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        }
    }

    for (string filename : filenames) {
        if (!this->isFileExists(filename)) {
            FILE* file = fopen(filename.c_str(), "w+");
            fclose(file);
        }
    }

    return 1;
}
