#include "../../include/Server/Server.hpp"
#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <cstdint>
#include <netinet/in.h>
#include <sys/socket.h>

/*
 *
 * Ip Address Type Implementation
 *
 */

IpAddress::IpAddress(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4)
: m_byte1(b1), m_byte2(b2), m_byte3(b3), m_byte4(b4)
{
}

std::string IpAddress::getStrIpAddress()
{
    std::string ip = std::to_string(this->m_byte1)
        + "." + std::to_string(this->m_byte2)
        + "." + std::to_string(this->m_byte3)
        + "." + std::to_string(this->m_byte4);
    return ip;
}

std::ostream& operator<<(std::ostream& os, const IpAddress& ip)
{
    os << static_cast<int>(ip.m_byte1) << "."
        << static_cast<int>(ip.m_byte2) << "."
        << static_cast<int>(ip.m_byte3) << "."
        << static_cast<int>(ip.m_byte4);
    return os;
}

/*
*
* Server Type Implementation
*
*/

Server::Server(IpAddress& ip, const uint16_t port)
: m_port(port), m_ipAddress(ip)
{
    this->m_serverAddress.sin_port = htons(port);
    this->m_serverAddress.sin_family = AF_INET;
    inet_pton(AF_INET, ip.getStrIpAddress().c_str(), &this->m_serverAddress.sin_addr); // Str -> Binaire (IP)
}

void Server::OnStart()
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    bind(serverSocket, (struct sockaddr*)&this->m_serverAddress, sizeof(this->m_serverAddress));
    listen(serverSocket, BACKLOG);
    std::cout << "Server listenning on " << this->m_ipAddress << ":" << this->m_port << std::endl;
}
