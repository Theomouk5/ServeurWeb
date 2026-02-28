#include "../include/Server/Server.hpp"

int main()
{
    IpAddress ip = {192, 168, 1, 68};
    Server server = Server(ip, 8080);
    {
        server.OnStart();
    }
    return 0;
}
