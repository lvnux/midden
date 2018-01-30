/*
 * IniFile.h
 * ini配置文件解析
 */

#ifndef __INI_FILE_H__
#define __INI_FILE_H__

#include "general.h"

#include <map>
#include <string>
#include <algorithm>

using std::multimap;
using std::map;
using std::string;
using std::less;

typedef map<string, string>	string_map;

class IniFile
{
public:
	IniFile(void);
	~IniFile(void);

public:
	static IniFile* get_instance();
	bool open(const char* ini_name);
	void close();

	std::string get_string(const char* sector, const char* key, const char* default_value="");
	int get_int(const char* sector, const char* key, int default_value=0);
	bool get_value(const char* sector, const char* key, string& value);
	bool get_value(const char* sector, const char* key, int& value);

private:
	typedef multimap<string, string_map>			section_map;
	typedef multimap<string, string_map>::iterator	section_iter;

	char file_name_[260];
	section_map	sect_map_;

private:
	static IniFile* inst_;

private:
	IniFile(const IniFile&);
	IniFile& operator=(const IniFile&);
};

#endif  // __INI_FILE_H__