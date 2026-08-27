#pragma once

#include <string>
#include <vector>

namespace azh::sdk::network
{
struct http_message_field
{
    std::string key;
    std::string value;
    http_message_field(const std::string &key, const std::string &value)
    {
        this->key = key;
        this->value = value;
    }
};

class http_message
{
private:
    std::string m_request_method;
    std::string m_url;
    std::string m_http_version;
    std::vector<http_message_field> m_fields;
    std::vector<http_message_field> m_params;
    std::string m_content;
    std::string m_hostname;
    int m_port;

public:
    http_message(
        const std::string &method = "GET",
        const std::string &url = "",
        const std::string &version = "HTTP/1.1")
        : m_request_method(method)
        , m_url(url)
        , m_http_version(version)
    {}

    ~http_message() {}

    inline bool setMethod(const std::string &method)
    {
        if (isInvaildForMethod(method)) {
            m_request_method = method;
            return true;
        }
        return false;
    }

    inline void setUrl(const std::string &url) { m_url = url; }
    inline std::string &url() { return m_url; }
    inline void setContent(const std::string &content) { m_content = content; }
    inline std::string &content() { return m_content; }
    inline void setHost(const std::string &host) { m_hostname = host; }
    inline std::string &host() { return m_hostname; }

    inline void setPort(int port)
    {
        if (port > 0 && port < 65535)
            m_port = port;
    }

    inline int port() { return m_port; }

    inline void addParam(const std::string &key, const std::string &value)
    {
        m_params.push_back(http_message_field(key, value));
    }

    inline void addField(const std::string &key, const std::string &value)
    {
        m_fields.push_back(http_message_field(key, value));
    }

    std::string removeField(const std::string &key)
    {
        std::vector<http_message_field>::iterator i;
        for (i = m_fields.begin(); i != m_fields.end(); i++)
            if (i->key == key)
                return i->value;
        return std::string();
    }

    std::string createMessage()
    {
        if (m_hostname.empty() || m_port == 0)
            return std::string();
        std::string message;
        std::string params;
        for (int i = 0; i < m_params.size(); i++) {
            params.append(m_params[i].key + "=" + m_params[i].value);
            if (i != m_params.size() - 1)
                params.append("&");
        }
        if (m_request_method == "GET" && !params.empty())
            m_url += "?" + params;
        message.append(m_request_method + " " + m_url + " " + m_http_version + "\r\n");
        for (int i = 0; i < m_fields.size(); i++)
            message.append(m_fields[i].key + ":" + m_fields[i].value + "\r\n");
        if (m_request_method == "POST") {
            message.append("Content-Length:");
            message.append(std::to_string(params.length()) + "\r\n");
            message.append("\r\n" + params + "\r\n");
        }
        char port[5];
#ifdef _WIN32
        _itoa(m_port, port, 10);
#else
        itoa(m_port, port, 10);
#endif
        message.append("Host:" + m_hostname + ":" + port + "\r\n");
        message.append("\r\n");
        return message;
    }

private:
    bool isInvaildForMethod(const std::string &method)
    {
        static std::string methods[8]
            = {"GET", "HEAD", "POST", "PUT", "DELETE", "CONNECT", "OPTIONS", "TRACE"};
        for (int i = 0; i < 8; i++)
            if (methods[i] == method)
                return true;
        return false;
    }
};

}; // namespace azh::sdk::network