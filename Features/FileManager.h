#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include "../Libs/Define.h"

using namespace std;

class FileManager {
    public:
        FileManager();
        ~FileManager();

        bool isFileExists(string& filename);
        bool isDirExists(string& dirname);

        u_int8 checkAllFilesAndCreate();

        void writeUserCache(u_int64 id);
        u_int8 isEmpty(string& filename);
};

#endif
