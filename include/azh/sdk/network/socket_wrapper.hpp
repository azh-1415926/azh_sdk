#pragma once

#include <cstdbool>
#include <iostream>
#include <string.h>

#if _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#elif __linux__
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

namespace azh::sdk::network
{
class socket_address
{
    struct sockaddr_in m_addr;

public:
#if _WIN32
    typedef SOCKET SOCKET_FD;
#elif __linux__
    typedef int SOCKET_FD;
#endif

    socket_address(int af, const char *ip, unsigned int port)
    {
        memset(&m_addr, 0, sizeof(m_addr));
        m_addr.sin_family = af;
        m_addr.sin_port = htons(port);
#if _WIN32
        m_addr.sin_addr.S_un.S_addr = inet_addr(ip);
#elif __linux__
        inet_pton(af, ip, &m_addr.sin_addr.s_addr);
#endif
    }

    socket_address(int af, unsigned int port, unsigned int addr)
    {
        memset(&m_addr, 0, sizeof(m_addr));
        m_addr.sin_family = af;
        m_addr.sin_port = htons(port);
#if _WIN32
        m_addr.sin_addr.S_un.S_addr = htonl(addr);
#elif __linux__
        m_addr.sin_addr.s_addr = htonl(addr);
#endif
    }

    socket_address(const socket_address &addr)
    {
        memcpy(&m_addr, &addr.m_addr, sizeof(addr.m_addr));
    }

    inline struct sockaddr_in *data() { return &m_addr; }
    inline size_t size() { return sizeof(m_addr); }
    virtual ~socket_address() {}
};

class socket_wrapper
{
    bool m_is_closed;

protected:
#if _WIN32
    typedef SOCKET SOCKET_FD;
#elif __linux__
    typedef int SOCKET_FD;
#endif

    SOCKET_FD m_socket_fd;

public:
    socket_wrapper(int af = AF_INET, int type = SOCK_STREAM, int protocol = 0)
        : m_is_closed(false)
    {
/* init socket */
#if _WIN32
        WSADATA wsadata;
        if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
            throw std::string("WSAStart error!");
#endif
        m_socket_fd = socket(af, type, protocol);
#if _WIN32
        if (m_socket_fd == INVALID_SOCKET) {
            WSACleanup();
            throw std::string("socket error!");
        }
#elif __linux__
        if (m_socket_fd <= 0)
            throw std::string("socket error!");
#endif
        m_is_closed = true;
    }

    socket_wrapper(const socket_wrapper &socket) = delete;
    socket_wrapper &operator=(const socket_wrapper &socket) = delete;

    virtual ~socket_wrapper()
    {
        if (!isClosed())
            closeSocket();
    }

    inline bool isClosed() { return m_is_closed; }

    inline void closeSocket()
    {
#if _WIN32
        closesocket(m_socket_fd);
        WSACleanup();
#elif __linux_
        close(m_socket_fd);
#endif
    }

    void setTimeout(time_t time)
    {
        struct timeval timeout;
        timeout.tv_sec = time;
        timeout.tv_usec = 0;
        setsockopt(m_socket_fd, SOL_SOCKET, SO_SNDTIMEO, (char *) &timeout, sizeof(struct timeval));
#if __linux__
        setsockopt(m_socket_fd, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(struct timeval));
#endif
    }

    void sendTo(const std::string &str) { send(m_socket_fd, str.c_str(), str.length(), 0); }

    std::string receive()
    {
        char buf[4096];
        memset(buf, 0, sizeof(buf));
        recv(m_socket_fd, buf, sizeof(buf), 0);
        return buf;
    }
};
}; // namespace azh::sdk::network