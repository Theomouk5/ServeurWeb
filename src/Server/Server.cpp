#include "../../include/Server/Server.hpp"
#include "../../include/Util/string.hpp"
#include <iostream>
#include <string>
#include <cstdint>
#include <thread>
#include <queue>
#include <mutex>

#if __linux__
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
#elif _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <WinSock2.h>
    #include <WS2tcpip.h>
    #pragma comment(lib, "Ws2_32.lib")
#endif

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
    inet_pton(AF_INET, ip.getStrIpAddress().c_str(), &this->m_serverAddress.sin_addr); // Str -> Binaire (IP)
    this->m_serverAddress.sin_port = htons(port);
    this->m_serverAddress.sin_family = AF_INET;
    this->m_threadPool.push_back(std::thread(&Server::HandleRequest, this));
    this->m_threadPool.push_back(std::thread(&Server::HandleRequest, this));
    this->m_threadPool.push_back(std::thread(&Server::HandleRequest, this));
    this->m_queue.running = true;
}

void Server::OnStart()
{
    #ifdef _WIN32 // Initialisation WinSock
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "WSAStartup failed\n";
            return;
        }
    #endif

    SOCKET_TYPE serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    bind(serverSocket, (sockaddr*)&this->m_serverAddress, sizeof(this->m_serverAddress));

    listen(serverSocket, BACKLOG);
    std::cout << "Server listenning on " << this->m_ipAddress << ":" << this->m_port << std::endl;

    while(true)
    {
        SOCKET_TYPE clientSocket = accept(serverSocket, nullptr, nullptr);
        std::unique_lock<std::mutex> lock(this->m_queue.mtx);
        this->m_queue.clientQueue.push(clientSocket);
        lock.unlock();
        this->m_queue.condVar.notify_one();
    }

    CLOSE(serverSocket);

    #ifdef _WIN32
        WSACleanup();
    #endif
}

void Server::HandleRequest()
{
    while(!this->m_queue.clientQueue.empty() || this->m_queue.running)
    {
        std::unique_lock<std::mutex> lock(this->m_queue.mtx);
        this->m_queue.condVar.wait(lock, [&]() { return !this->m_queue.clientQueue.empty(); });

        SOCKET_TYPE client = this->m_queue.clientQueue.front();
        this->m_queue.clientQueue.pop();
        lock.unlock();

        char buffer[1024];
        recv(client, buffer, sizeof(buffer), 0);
        std::vector<std::string> lines = Util::splitLines(buffer);

        for(int i = 0; i < std::size(lines); i++)
        {
            std::cout << "lines " << i << " : " << lines[i] << std::endl;
        }

        CLOSE(client);
    }
}
