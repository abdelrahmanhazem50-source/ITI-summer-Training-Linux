#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <sys/stat.h>
#include <string>

class FileManager
{
public:
    void showFileInfo();
    void changePermissions();

private:
    std::string getFileType(mode_t mode);
    std::string getPermissionString(mode_t mode);
    bool validPermission(const std::string &permission);
};

#endif
