#include "file_tool.h"


int FileTool::is_file_exist(const char* file_name)
{
	return access(file_name, F_OK);
}

int FileTool::is_dir_exist(const char* dir_path)
{
	if (NULL == opendir(dir_path))
	{
		return -1;
	}

	return 0;
}

int FileTool::create_dir(const char *pathname, mode_t mode)
{
	return mkdir(pathname, mode);
}

void FileTool::get_cur_directory(char* szpath, int nlen)
{
	getcwd(szpath, nlen);
}