#ifndef __SERVER__
#define __SERVER__
#define BACKLOG 5

#include <cstdint>
#include <netinet/in.h>
#include <string>
#include <ostream>


class IpAddress
{
    public:
        IpAddress() = delete;
        IpAddress(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4);
        std::string getStrIpAddress();

        friend std::ostream& operator<<(std::ostream&, const IpAddress& ip);

    private:
        uint8_t m_byte1;
        uint8_t m_byte2;
        uint8_t m_byte3;
        uint8_t m_byte4;
};

class Server
{
    public:
        Server() = delete;
        Server(IpAddress& ip, const uint16_t port);
        void OnStart();

    private:
        sockaddr_in    m_serverAddress;
        IpAddress      m_ipAddress;
        const uint16_t m_port;
};

#endif
