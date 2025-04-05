#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>

using namespace std;

class FileManager {
    public:
        FileManager();
        ~FileManager();

        bool isFileExists(string& filename);
        bool isDirExists(string& dirname);
};

#endif
