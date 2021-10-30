#ifndef FILE_FUNCTIONS_HPP_INCLUDED
#define FILE_FUNCTIONS_HPP_INCLUDED

struct file_list_struct {
    std::string filename;

    bool operator < (const info_3d_struct &B) const
    {
        return filename < B.filename;
    }


};

struct to_do_list_struct {
    std::string filename;
    std::string out;
};

std::string GetFileName(const std::string& filepath);
std::string GetBaseDir(const std::string& filepath);
bool file_exists(const char * filename);
int replace_string(char *str, char *orig, char *rep);
int read_dir(std::string directory, std::vector<file_list_struct> &file_list);
int read_subdir(std::string baseDir, std::vector<struct file_list_struct> &file_list, bool recursive);
bool isDir(std::string dir);
int mkdir_p(const char *dir);

#endif // FILE_FUNCTIONS_HPP_INCLUDED
