#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#include <atomic>

#include <libcpp/net/proto/message.hpp>
#include <libcpp/net/tcp/tcp_listener.hpp>
#include <libcpp/net/tcp/tcp_conn.hpp>

namespace libcpp
{

template<typename Key = std::uint64_t>
class tcp_server
{
public:
    using listen_handler_t     = std::function<void(tcp_server*, tcp_conn*)>;

public:
    tcp_server()
        : _listener{}
    {
    }
    
    ~tcp_server()
    {
        close();
    }

    inline std::size_t conn_size() { return _conns.size(); }
    inline std::size_t add(const Key id, tcp_conn* conn) { _add(id, conn); return size(); }
    inline tcp_conn* get(const Key id) { return _get(id); }

    tcp_conn* listen(const char* ip, const uint16_t port)
    {
        auto sock_ptr = _listener.accept(ip, port);
        if (sock_ptr == nullptr)
            return nullptr;

        auto ret = new tcp_socket(sock_ptr);
        return ret;
    }

    void async_listen(const char* ip, const uint16_t port, listen_handler_t&& fn)
    {
        _listener.async_accept(ip, port, [this, std::move(fn)](const err_t& err, tcp_socket* sock) {
            if (err.fail())
                fn(this, nullptr);
            else
                fn(this, new tcp_conn(sock));
        });
    }

    void close()
    {
        _listener.close();

        auto keys = _conns.keys();
        for (Key id : keys)
            _del(id);
    }

    bool send(message* msg, const Key id, bool block = false)
    {
        auto conn = _get(id);
        if (conn == nullptr)
            return false;

        conn->send(msg, block);
        return true;
    }

    bool recv(message* msg, const Key id, const bool block = false)
    {
        auto conn = _get(id);
        if (conn == nullptr)
            return false;

        conn->recv(msg, block);
        return true;
    }

    void broad_cast(message* msg, const bool block = false)
    {
        auto ids = _conns.keys();
        for (Key id : ids)
            send(msg, id, block);
    }

    void group_cast(message* msg, const std::vector<Key>& ids, const bool block = false)
    {
        for (auto id : ids)
            send(msg, id, block);
    }

    void kick_off(const std::vector<Key>& ids)
    {
        for (Key id : ids)
            _del(id);
    }

private:
    bool _add(const Key id, tcp_conn* conn)
    {
        if (conn == nullptr || _conns.find(id) != _conns.end())
            return false;

        _conns.emplace(id, conn);
        return true;
    }

    void _del(const Key id)
    {
        auto itr = _conns.remove(id);
        if (itr == _conns.end())
            return;

        itr->second->disconnect();
        delete itr->second;
    }

    tcp_conn* _get(const Key id)
    {
        auto itr = _conns.find(id);
        if (itr == _conns.end())
            return nullptr;

        return itr->second;
    }

private:
    libcpp::tcp_listener _listener;
    std::unordered_map<Key, tcp_conn*> _conns;
};

}

#endif