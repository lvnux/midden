#include "log.h"

#include <stdio.h>
#include <unistd.h>


int main(int argc, char* argv[])
{
    CLog::get_instance()->init("/var/log", "test_midden", 0);

    log_warn("test midden");

    return 0;
}