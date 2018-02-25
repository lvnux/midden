#include "log.h"
#include "test_thread.h"

#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    TestThread* test = new TestThread();
    test->init();
    test->start();

    while (true)
    {
        sleep(2);
    }

    return 0;
}