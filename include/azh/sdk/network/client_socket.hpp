#pragma once

#include "socket_wrapper.hpp"

namespace azh::sdk::network
{
class client_socket : public socket_wrapper
{
    socket_address *m_server_address;

public:
    client_socket(int af = AF_INET, int type = SOCK_STREAM, int protocol = 0)
        : socket_wrapper(af, type, protocol)
        , m_server_address(nullptr)
    {}

    client_socket(const client_socket &client) = delete;
    client_socket &operator=(const client_socket &client) = delete;

    virtual ~client_socket()
    {
        if (m_server_address != nullptr)
            delete m_server_address;
    }

    void setServerAddr(const socket_address &addr)
    {
        if (m_server_address != nullptr)
            delete m_server_address;
        m_server_address = new socket_address(addr);
    }

    bool connectServer()
    {
        int status;
#if __linux__
        int flags;
        flags = fcntl(m_socket_fd, F_GETFL);
        fcntl(m_socket_fd, F_SETFL, flags | O_NONBLOCK);
#endif
        status = connect(
            m_socket_fd, (struct sockaddr *) (m_server_address->data()), m_server_address->size());
#if _WIN32
        if (status == SOCKET_ERROR) {
            closeSocket();
            throw std::string("connect error!");
        }
#elif __linux__
        if (status < 0) {
            if (errno != EINPROGRESS) {
                close(m_socket_fd);
                throw std::string("connect error!");
            }
            printf("connect nonblock.\n");
        }
        fcntl(m_socket_fd, F_SETFL, flags);
#endif
        return true;
    }
};
} // namespace azh::sdk::network