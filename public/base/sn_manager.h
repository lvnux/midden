/*
 * sn_manager.h
 * 常用类型定义，兼容不同平台
 */

 #ifndef __SN_MANAGER_H__
 #define __SN_MANAGER_H__

 #include "platform.h"

class SnManager
{
public:
	SnManager();
	virtual ~SnManager();
	uint64 get_sn();

private:
	uint32 sn_;
};

 #endif  // __SN_MANAGER_H__