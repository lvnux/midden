#ifndef __FILE_TOOL_H__
#define __FILE_TOOL_H__

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

class FileTool
{
public:
    static int is_file_exist(const char* file_name);
    static int is_dir_exist(const char* dir_path);
    
    static int create_dir(const char *pathname, mode_t mode=S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO); 
    
    static void get_cur_directory(char* szpath, int nlen);
};

#endif  // __FILE_TOOL_H__