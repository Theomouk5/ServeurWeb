#ifndef __SERVER__
#define __SERVER__
#define BACKLOG 5

#ifdef _WIN32
    #include <winsock2.h>
    #define CLOSE closesocket
    #define SOCKET_TYPE SOCKET
#elif __linux__
    #include <unistd.h>
    #include <netinet/in.h>
    #define CLOSE close
    #define SOCKET_TYPE int
#endif

#include <thread>
#include <vector>
#include <cstdint>
#include <string>
#include <ostream>
#include <mutex>
#include <condition_variable>
#include <queue>

struct ThreadSafeQueue
{
    std::mutex mtx;
    std::condition_variable condVar;
    std::queue<SOCKET_TYPE> clientQueue;
    bool running;
};

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
        void HandleRequest();

    private:
        ThreadSafeQueue          m_queue;
        std::vector<std::thread> m_threadPool;
        sockaddr_in              m_serverAddress;
        IpAddress                m_ipAddress;
        const uint16_t           m_port;
};

#endif
