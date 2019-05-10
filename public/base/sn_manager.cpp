#include "sn_manager.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


SnManager::SnManager()
 : sn_(0)
{

}

SnManager::~SnManager()
{

}

uint64 SnManager::get_sn()
{
	uint32 now = time(NULL);
	if (sn_ > 0x00000000000F423E)
	{
		sn_ = 0;
	}
	++sn_;

	char sn_str[64] = {0};
	snprintf(sn_str, 64, "%010d%06d", now, sn_);
	uint64 sn = atol(sn_str);

	return sn;
}