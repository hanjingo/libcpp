#ifndef ZMQ_CONSUMER_HPP
#define ZMQ_CONSUMER_HPP

#include <zmq.h>
#include <iostream>
#include <libcpp/net/zmq/zmq_chan.hpp>

namespace libcpp
{

class zmq_consumer
{
public:
    zmq_consumer(void* ctx)
        : _ctx{ctx}
        , _sock{zmq_socket(ctx, ZMQ_PULL)}
    {
        zmq_msg_init(&_buf);
    }
    ~zmq_consumer()
    {
        zmq_close(_sock);
		_sock = nullptr;

        zmq_msg_close(&_buf);
    }

    inline int set_opt(const int opt, const int value)
    {
        return zmq_setsockopt(_sock, opt, &value, sizeof(value));
    }

    inline int connect(const std::string& addr)
    {
        return zmq_connect(_sock, addr.c_str());
    }

    inline int disconnect(const std::string& addr)
    {
        return zmq_disconnect(_sock, addr.c_str());
    }

    int pull(std::string& dst, int flags = 0)
    {
        zmq_msg_init(&_buf);
        int nbytes = zmq_msg_recv(&_buf, _sock, flags);
        if (nbytes < 0)
            return nbytes;

        dst.reserve(nbytes);
        dst.assign(static_cast<char*>(zmq_msg_data(&_buf)), nbytes);
        return nbytes;
    }

    inline int pull(zmq_msg_t& data, int flags = 0)
    {
        return zmq_msg_recv(&data, _sock, flags);
    }

private:
    void*            _ctx;
    void*            _sock;
    zmq_msg_t        _buf;
};

}

#endif