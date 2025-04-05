#include <sys/stat.h>
#include "FileManager.h"

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
