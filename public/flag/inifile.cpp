#include "inifile.h"
#include <fstream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>


AnalyzeIniEx::AnalyzeIniEx(std::multimap<string, string_map>& sect) 
 : sector_map_(sect), sector_itor_(sect.end())
{

}

void AnalyzeIniEx::operator()(const std::string & strini)
{
    if(strini.empty() == true)
        return;

    std::size_t first = strini.find('[');
    std::size_t last = strini.rfind(']');
    if( first != std::string::npos && last != std::string::npos && first != last+1)
    {
        sector_ = strini.substr(first + 1, last - first - 1);
        sector_itor_ = sector_map_.insert(make_pair(sector_, string_map()));
        return;
    }

    if(sector_.empty())
        return ;

    if((first=strini.find('=')) == std::string::npos)
        return ;

    std::string strtmp1= strini.substr(0, first);
    std::string strtmp2 = strini.substr(first + 1, strini.length());

    first = strtmp1.find_first_not_of(" \t");
    last = strtmp1.find_last_not_of(" \t");
    if(first == std::string::npos || last == std::string::npos)
        return ;

    std::string strkey = strtmp1.substr(first, last - first + 1);
    first = strtmp2.find_first_not_of(" \t");
    if(((last = strtmp2.find("\t#", first )) != std::string::npos) ||
        ((last = strtmp2.find(" #", first )) != std::string::npos) ||
        ((last = strtmp2.find("\t//", first )) != std::string::npos) ||
        ((last = strtmp2.find(" //", first )) != std::string::npos))
    {
        strtmp2 = strtmp2.substr(0, last - first);
    }

    last = strtmp2.find_last_not_of(" \t");
    if(first == std::string::npos || last == std::string::npos)
        return ;

    if (sector_itor_ != sector_map_.end())
    {
        std::string value = strtmp2.substr(first, last - first + 1);
        sector_itor_->second[strkey] = value;
    }

    return ;
}


IniFile* IniFile::inst_ = new IniFile();

IniFile::IniFile()
{

}

IniFile::~IniFile(void)
{
}

IniFile* IniFile::get_instance()
{
    return inst_;
}

bool IniFile::open(const char* ini_name)
{
    char filename[260] = {0};

    const char* valid_path = strstr(ini_name, "/");

    if(valid_path == NULL)
    {
        get_cur_directory(filename, 260);
        strcat(filename, "/");
        strcat(filename, ini_name);
    }
    else
    {
        strcat(filename, ini_name);
    }

    if ( 1 == check_file_exists(filename))
    {
        std::cout << "no configure file found" << std::endl;
        return false;
    }

    std::ifstream fin(filename);
    if(!fin.is_open())
        return false;

    strcpy(file_name_, filename);

    std::vector<string> strvect;
    while(!fin.eof())
    {
        string inbuf;
        getline(fin, inbuf, '\n');
        if (inbuf.size()>0)
        {
            if (*(inbuf.c_str() + inbuf.size() - 1) == '\r')
            {
                // 去掉'\r'
                // 这样windows不会出现多回车符问题
                *((char*)inbuf.c_str() + inbuf.size() - 1) = '\0';
                inbuf.resize(inbuf.size() - 1);
            }
        }
        
        strvect.push_back(inbuf);
    }

    if(strvect.empty())
        return false;

    std::for_each(strvect.begin(), strvect.end(), AnalyzeIniEx(sect_map_));

    return !sect_map_.empty();
}

void IniFile::close()
{
    sect_map_.clear();
}

std::string IniFile::get_string(const char* sector, const char* key, const char *default_value)
{
    string value = default_value;

    section_iter itor = sect_map_.find(sector);
    if (itor != sect_map_.end())
    {
        string_map::iterator itor_key = itor->second.find(key);
        if (itor_key != itor->second.end())
        {
            value = itor_key->second;
        }
    }

    return value;
}

int IniFile::get_int(const char* sector, const char* key, int default_value)
{
    int value = default_value;
    section_iter itor = sect_map_.find(sector);
    if (itor != sect_map_.end())
    {
        string_map::iterator itor_key = itor->second.find(key);
        if (itor_key != itor->second.end())
        {
            value = atoi(itor_key->second.c_str());
        }
    }

    return value;
}

bool IniFile::get_value(const char* sector, const char* key, string& value)
{
    section_iter itor = sect_map_.find(sector);
    if (itor != sect_map_.end())
    {
        string_map::iterator itor_key = itor->second.find(key);
        if (itor_key != itor->second.end())
        {
            value = itor_key->second;
            return  true;
        }
    }

    return  false;
}

bool IniFile::get_value(const char* sector, const char* key, int& value)
{
    section_iter itor = sect_map_.find(sector);
    if (itor != sect_map_.end())
    {
        string_map::iterator itor_key = itor->second.find(key);
        if (itor_key != itor->second.end())
        {
            value = atoi(itor_key->second.c_str());
            return  true;
        }
    }

    return  false;
}