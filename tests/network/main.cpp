#ifdef _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _SILENCE_CXX17_C_HEADER_DEPRECATION_WARNING
#endif
#include <azh/sdk/network/http_message.hpp>
#include <azh/sdk/network/client_socket.hpp>
#include <azh/sdk/type/string.hpp>
#include <azh/sdk/utils/logger.hpp>

#include <iostream>
// #include <netdb.h>
// #include <arpa/inet.h>
#include <cstring>

std::string getIp(const char *domain)
{
    char ip[15];
    struct hostent *host = gethostbyname(domain);
    if (host == NULL) {
        std::cerr << "Failed to resolve domain" << std::endl;
        return "";
    }
    strcpy(ip, inet_ntoa(*(struct in_addr *) host->h_addr_list[0]));

    return ip;
}

int main()
{
    // std::string domain;
    // int port;
    // std::string url;
    // std::vector<std::string> keys;
    // std::vector<std::string> values;

    std::string domain = "cn.apihz.cn";
    int port = 80;
    std::string method = "GET";
    std::string url = "/api/tianqi/tqyb.php";
    std::vector<std::string> keys = {"id", "key", "sheng", "place"};
    std::vector<std::string> values = {"88888888", "88888888", "四川", "绵阳"};

    try {
        azh::sdk::network::client_socket socket;
        std::string ip = getIp(domain.c_str());
        azh::sdk::network::http_message message(method, url);
        for (int i = 0; i < keys.size(); i++) {
            message.addParam(keys[i], values[i]);
        }
        message.setHost(ip);
        message.setPort(port);
        std::string str = message.createMessage();
        aDebug() << "Request Message :\n" << str;

        azh::sdk::network::socket_address sererAddr(AF_INET, ip.c_str(), port);
        socket.setServerAddr(sererAddr);
        socket.setTimeout(3);
        socket.connectServer();
        socket.sendTo(str);
        aDebug() << "Reponse Message :\n" << socket.receive();
        socket.closeSocket();
    } catch (std::string error) {
        std::cout << error;
    }
    return 0;
}