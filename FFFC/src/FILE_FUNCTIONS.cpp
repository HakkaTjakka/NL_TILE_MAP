#include <conio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <io.h>
#include <stdio.h>
#include <iostream>
#include <cassert>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <SFML\Graphics.hpp>
#include <FILE_FUNCTIONS.hpp>
extern void (*CALLER)(char *,char *);
//extern void ACTION(char* directory, char* file);

//#include <conio.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <windows.h>
//#include <io.h>
//#include <math.h>
//#include <dirent.h>
//#include <sys/stat.h>
//#include <string.h>
//#include <vector>
//#include <map>
//#include <limits>
//#include <SFML\Graphics.hpp>
//#include <SFML\Graphics\Shader.hpp>
//#include <SFML\Graphics\Image.hpp>
//#include <iostream>
//#include <errno.h>

using namespace std;

std::string GetFileName(const std::string& filepath)
{
    if (filepath.find_last_of("/\\") != std::string::npos)
        return filepath.substr(1+filepath.find_last_of("/\\"),filepath.length());
    return "";
}

std::string GetBaseDir(const std::string& filepath)
{
    if (filepath.find_last_of("/\\") != std::string::npos)
        return filepath.substr(0, filepath.find_last_of("/\\"));
    return "";
}

struct stat stat_buffer;

bool file_exists(const char * filename)
{
    int exist = stat(filename,&stat_buffer);
    if (exist==0) return true;
    return false;
}

int replace_string(char *str, char *orig, char *rep)
{
    char buffer[strlen(str)+1000];
    char *p;
    if(!(p = strstr(str, orig)))  // Is 'orig' even in 'str'?
        return false;
    strncpy(buffer, str, p-str); // Copy characters from 'str' start to 'orig' st$
    buffer[p-str] = '\0';
    sprintf(buffer+(p-str), "%s%s", rep, p+strlen(orig));
    strcpy(str,buffer);
    return true;
}

int read_dir(std::string dir, std::vector<struct file_list_struct> &file_list) {
    int l=dir.length();
    if (l>1) {
        if (dir.substr(l-1, 1) == "/") {
            dir=dir.substr(0,l-1);
        }
    }

    char* directory=(char*)(dir.c_str());
    DIR* m_dir;
    struct dirent *m_dirent;
    struct stat file_stats;

//    printf("%s\\\n",directory);
    m_dir = opendir(directory);
    struct file_list_struct one_file;

    int files=0;
    int files2=1;
    char* name;
    char lname[1000];
    m_dirent = readdir(m_dir);
    while (m_dirent != NULL) {
        name=m_dirent->d_name;
        strcpy(lname,name);
        if (string(name) != string(".") && string(name) != string("..")) {
//            printf("%s\n",name);
            one_file.filename=name;
            file_list.push_back(one_file);
            ++files;
//            ACTION((char*)(dir+"/").c_str(),name);
            CALLER((char*)(dir+"/").c_str(),name);
        }
        m_dirent = readdir(m_dir);
    }
//    if (files>0) printf("\nTotal=%d files\n",files);
    closedir(m_dir);
    fflush(stdout);
    return file_list.size();
}

bool isDir(string dir)
{
    struct stat fileInfo;
    stat(dir.c_str(), &fileInfo);
    if (S_ISDIR(fileInfo.st_mode)) {
        return true;
    } else {
        return false;
    }
}

//Argument should contain the trailing slash ( eg. /etc/httpd/, NOT /etc/httpd ).
int read_subdir(std::string baseDir, std::vector<struct file_list_struct> &file_list, bool recursive)
{
    static int depth=0;
    static int files=0;
    static int dirs=0;

    if (depth==0) {
        files=0;
        dirs=0;
        //if (baseDir=="./") baseDir=".";
        int l=baseDir.length();
        if (l>1) {
            if (baseDir.substr(l-1, 1) == "/") {
                baseDir=baseDir.substr(0,l-1);
            }
        }
    }
    depth++;

    baseDir=baseDir + "/";
//    printf("Reading %s\n",baseDir.c_str());
    struct file_list_struct one_file;

    DIR *dp;
    struct dirent *dirp;
    if ((dp = opendir(baseDir.c_str())) == NULL) {
        cout << "[ERROR: " << errno << " ] Couldn't open " << baseDir << "." << endl;
        depth=0;
        fflush(stdout);
        return -1;
    } else {
        char* name;
        while ((dirp = readdir(dp)) != NULL) {
            name=dirp->d_name;
            if (string(name) != string(".") && string(name) != string("..")) {
                if (isDir(baseDir + name) == true) {
                    dirs++;
//                    printf("%s%s/\n",baseDir.c_str(),name);
                    if (recursive) {
                        read_subdir(baseDir + name , file_list, true);
                    }
                } else {
                    one_file.filename=baseDir+name;
                    file_list.push_back(one_file);
//                    printf("%s\n",one_file.filename.c_str());
                    files++;
//                    ACTION((char*)baseDir.c_str(),name);
                    CALLER((char*)baseDir.c_str(),name);
                }
            }
        }
        closedir(dp);
    }
//    if (depth--==1) if (files>0) printf("Total= %d files %d dir(s)\n", file_list.size(), dirs);
    fflush(stdout);

    return file_list.size();
}

#define PATH_MAX_STRING_SIZE 1024

/* recursive mkdir */
int mkdir_p(const char *dir) {
    char tmp[PATH_MAX_STRING_SIZE];
    char *p = NULL;
    struct stat sb;
    size_t len;

    /* copy path */
    len = strnlen (dir, PATH_MAX_STRING_SIZE);
    if (len == 0 || len == PATH_MAX_STRING_SIZE) {
        return -1;
    }
    memcpy (tmp, dir, len);
    tmp[len] = '\0';

    /* remove trailing slash */
    if(tmp[len - 1] == '/') {
        tmp[len - 1] = '\0';
    }

    /* check if path exists and is a directory */
    if (stat (tmp, &sb) == 0) {
        if (S_ISDIR (sb.st_mode)) {
            return 0;
        }
    }

    /* recursive mkdir */
    for(p = tmp + 1; *p; p++) {
        if(*p == '/') {
            *p = 0;
            /* test path */
            if (stat(tmp, &sb) != 0) {
                /* path does not exist - create directory */
                if (mkdir(tmp) < 0) {
                    return -1;
                }
            } else if (!S_ISDIR(sb.st_mode)) {
                /* not a directory */
                return -1;
            }
            *p = '/';
        }
    }
    /* test path */
    if (stat(tmp, &sb) != 0) {
        /* path does not exist - create directory */
        if (mkdir(tmp) < 0) {
            return -1;
        }
    } else if (!S_ISDIR(sb.st_mode)) {
        /* not a directory */
        return -1;
    }
    return 0;
}
