#include "general.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>


int daemon_init()
{
	int cpid;

	signal(SIGALRM, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGCLD, SIG_IGN); 
	signal(SIGPIPE, SIG_IGN);

	if((cpid = fork()) < 0) 
	{ 
		perror("create daemon program error\n");
		exit(-1);
	}
	else if(cpid>0)  
	{
		exit(0);               
	}

	if(setsid() == -1) 
	{
		perror("setsid error\n");
		exit(-1);
	}

	signal(SIGHUP,SIG_IGN);
	if((cpid=fork() ) < 0)
	{   
		perror("create daemon program error\n");
		exit(-1);
	}
	else if(cpid > 0) 
	{
		exit(0); 
	}

	return 0;
}

int check_file_exists(const char* pszfilename)
{
	return access(pszfilename, F_OK);
}

void get_cur_directory(char* szpath, int nlen)
{
	getcwd(szpath, nlen);
}

std::string trim(const std::string& src)   
{  
    std::string dst(src);
    if (dst.empty())   
    {  
        return dst;
    }  
  
    dst.erase(0, dst.find_first_not_of(" "));  
    dst.erase(dst.find_last_not_of(" ") + 1);  
    return dst;  
}  

void split_to_vec(std::string src, std::string separator, std::vector<std::string>& vec)
{
    int separator_len = separator.size();
    std::string::size_type index = -1;
    std::string::size_type start = 0;
    
    while (std::string::npos != (index = src.find(separator, start)))
    {
        vec.push_back(trim(src.substr(start, index-start)));
        start = index + separator_len;
    }
    
    std::string last = src.substr(start);
    if (false == last.empty())
    {
        vec.push_back(trim(last));
    }
    
    return;
}
