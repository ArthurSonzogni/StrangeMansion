#include "utils/Error.hpp"
#include <dirent.h>

#ifndef WIN32
#include <sys/types.h>
#endif

#include <vector>
#include <string>

using namespace std;

vector<string> directoryListFiles(string folder)
{
    vector<string> ret;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (folder.c_str())) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            string s(ent->d_name);
            if ( s != "." and s != "..")
                ret.push_back(s);
        }
        closedir (dir);
    } else {
        /* could not open directory */
        perror ("");
    }
    return ret;
}

vector<string> directoryListFolders(string folder)
{
    throw Error(__LINE__,__FILE__,"Not Implemented");
}

