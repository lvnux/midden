#include "log.h"
#include "net_server_socket.h"
#include "net_service_manager.h"

#include <stdio.h>
#include <unistd.h>


int main(int argc, char* argv[])
{
    NetServiceManager* manager = new NetServiceManager();
    if (false == manager->init())
    {
        printf("manager->init() failed\n");
        return -1;
    }

    NetServerSocket* server = manager->create_server_socket();
    if (false == server->listen(NetAddress(8888)))
    {
        printf("manager->init() failed\n");
        return -1;
    }

    while (true)
    {
        manager->get_selector()->select();

        sleep(2);
    }

    return 0;
}