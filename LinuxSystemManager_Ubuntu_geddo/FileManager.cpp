#include "FileManager.h"

#include <sys/stat.h>
#include <unistd.h>

#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

string FileManager::getFileType(mode_t mode)
{
    if (S_ISREG(mode))
        return "Regular file";
    if (S_ISDIR(mode))
        return "Directory";
    if (S_ISLNK(mode))
        return "Symbolic link";
    if (S_ISCHR(mode))
        return "Character device";
    if (S_ISBLK(mode))
        return "Block device";
    if (S_ISFIFO(mode))
        return "FIFO / Pipe";
    if (S_ISSOCK(mode))
        return "Socket";

    return "Unknown";
}

string FileManager::getPermissionString(mode_t mode)
{
    string permissions;

    permissions += (mode & S_IRUSR) ? 'r' : '-';
    permissions += (mode & S_IWUSR) ? 'w' : '-';
    permissions += (mode & S_IXUSR) ? 'x' : '-';

    permissions += (mode & S_IRGRP) ? 'r' : '-';
    permissions += (mode & S_IWGRP) ? 'w' : '-';
    permissions += (mode & S_IXGRP) ? 'x' : '-';

    permissions += (mode & S_IROTH) ? 'r' : '-';
    permissions += (mode & S_IWOTH) ? 'w' : '-';
    permissions += (mode & S_IXOTH) ? 'x' : '-';

    return permissions;
}

bool FileManager::validPermission(const string &permission)
{
    if (permission.length() != 3 && permission.length() != 4)
        return false;

    for (char c : permission)
    {
        if (c < '0' || c > '7')
            return false;
    }

    return true;
}

void FileManager::showFileInfo()
{
    cout << "Enter file path: ";
    string path;
    getline(cin, path);

    struct stat info;
    if (lstat(path.c_str(), &info) == -1)
    {
        perror("stat");
        return;
    }

    cout << "\n--- File Information ---\n";
    cout << "File Size   : " << info.st_size << " bytes\n";
    cout << "File Type   : " << getFileType(info.st_mode) << '\n';
    cout << "Owner UID   : " << info.st_uid << '\n';
    cout << "Group GID   : " << info.st_gid << '\n';
    cout << "Permissions : " << getPermissionString(info.st_mode) << " (";
    cout << oct << setw(3) << setfill('0') << (info.st_mode & 0777);
    cout << dec << setfill(' ') << ")\n";
}

void FileManager::changePermissions()
{
    cout << "Enter file path: ";
    string path;
    getline(cin, path);

    struct stat info;
    if (stat(path.c_str(), &info) == -1)
    {
        perror("stat");
        return;
    }

    cout << "Enter permission (example 755, 644, 700): ";
    string permission;
    getline(cin, permission);

    if (!validPermission(permission))
    {
        cout << "Invalid permission. Use octal digits from 0 to 7.\n";
        return;
    }

    mode_t mode = static_cast<mode_t>(stoul(permission, nullptr, 8));

    if (chmod(path.c_str(), mode) == -1)
    {
        perror("chmod");
        return;
    }

    cout << "Permissions changed to " << permission << " successfully.\n";
}
