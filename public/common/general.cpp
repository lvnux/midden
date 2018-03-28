#include "general.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


int check_file_exists(const char* pathname)
{
    return access(pathname, F_OK);
}

void get_cur_directory(char* buffer, int nlen)
{
    getcwd(buffer, nlen);
}

int create_directory(const char* pathname, mode_t mode)
{
    return mkdir(pathname, mode);
}