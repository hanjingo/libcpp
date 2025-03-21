#ifndef TCP_SESSION_HPP
#define TCP_SESSION_HPP

#include <chrono>
#include <iostream>
#include <functional>

#include <boost/asio.hpp>
#include <boost/asio/deadline_timer.hpp>

namespace libcpp
{

class tcp_session
{
public:
    using signal_t          = int;
    using signal_set_t      = boost::asio::signal_set;

    using io_t              = boost::asio::io_context;
    using io_work_t         = boost::asio::io_service::work;
    using err_t             = boost::system::error_code;
    using address_t         = boost::asio::ip::address;

    using const_buffer_t    = boost::asio::const_buffer;
    using multi_buffer_t    = boost::asio::mutable_buffer;
    using streambuf_t       = boost::asio::streambuf;
    using iostreambuf_t     = boost::asio::streambuf::mutable_buffers_type;
    using ostreambuf_t      = boost::asio::streambuf::const_buffers_type;

    using sock_t            = boost::asio::ip::tcp::socket;
    using endpoint_t        = boost::asio::ip::tcp::endpoint;
    using timer_t           = boost::asio::deadline_timer;

    using opt_no_delay      = boost::asio::ip::tcp::no_delay;
    using opt_send_buf_sz   = boost::asio::ip::tcp::socket::send_buffer_size;
    using opt_recv_buf_sz   = boost::asio::ip::tcp::socket::receive_buffer_size;
    using opt_reuse_addr    = boost::asio::ip::tcp::socket::reuse_address;

    using conn_handler_t    = std::function<void(const err_t&, tcp_session*)>;
    using send_handler_t    = std::function<void(const err_t&, std::size_t)>;
    using recv_handler_t    = std::function<void(const err_t&, std::size_t)>;
    using signal_handler_t  = std::function<void(const err_t&, signal_t)>;

public:
    tcp_session()
        : tm_{io_},
          sigs_{io_}
    {
    }
    explicit tcp_session(sock_t* sock)
        : tm_{io_},
          sigs_{io_},
          sock_{sock}
    {
    }
    virtual ~tcp_session()
    {
        close();
    }

    inline void poll()
    {
        io_.run();
    }

    inline void loop_start()
    {
        io_work_t work{io_};
        io_.run();
    }

    inline void loop_end()
    {
        io_.stop();
    }

    template<typename T>
    inline void set_option(T opt)
    {
        if (sock_ != nullptr && sock_->is_open()) {
            sock_->set_option(opt);
        }
    }

    bool connect(const char* ip,
                 uint16_t port,
                 std::chrono::milliseconds timeout = std::chrono::milliseconds(0),
                 int retry_times = 1)
    {
        endpoint_t ep{address_t::from_string(ip), port};
        return connect(ep, timeout, retry_times);
    }

    // WARNNING: boost.asio connect fail with default timeout duration = 2s
    // WARNNING: the feature of timeout is not recommended for use in a multi-connection context.
    // See Also: https://www.boost.org/doc/libs/1_80_0/doc/html/boost_asio/example/cpp03/timeouts/blocking_tcp_client.cpp
    bool connect(endpoint_t ep,
                 std::chrono::milliseconds timeout = std::chrono::milliseconds(0),
                 int retry_times = 1)
    {
        if (is_connected()) {
            return false;
        }
        if (retry_times < 1) {
            return false;
        }

        sock_ = new sock_t(io_);
        if (timeout.count() > 0) {
            sock_->async_connect(ep, [ = ](const err_t & err) {
                if (!err.failed()) {
                    is_connected_.store(true);
                }
            });

            io_.restart();
            io_.run_for(timeout);
            if (!io_.stopped() || !is_connected()) {
                retry_times--;
                disconnect();
                return connect(ep, timeout, retry_times);
            }

            return true;
        }

        err_t err;
        sock_->connect(ep, err);
        if (err.failed()) {
            retry_times--;
            disconnect();
            return connect(ep, timeout, retry_times);
        }

        is_connected_.store(true);
        return true;
    }

    void async_connect(const char* ip, uint16_t port, conn_handler_t&& fn)
    {
        endpoint_t ep{address_t::from_string(ip), port};
        async_connect(ep, std::move(fn));
    }

    void async_connect(endpoint_t ep, conn_handler_t&& fn)
    {
        if (is_connected()) {
            fn(boost::system::errc::make_error_code(boost::system::errc::already_connected), this);
            return;
        }

        sock_ = new sock_t(io_);
        sock_->async_connect(ep, [ = ](const err_t & err) {
            if (!err.failed()) {
                is_connected_.store(true);
            }

            fn(err, this);
        });
    }

    void disconnect()
    {
        if (!is_connected()) {
            return;
        }

        sock_->close();
        delete sock_;
        sock_ = nullptr;
        is_connected_.store(false);
    }

    size_t send(const const_buffer_t& buf)
    {
        if (!is_connected()) {
            return 0;
        }

        return sock_->send(buf);
    }

    size_t send(const char* data, size_t len)
    {
        auto buf = boost::asio::buffer(data, len);
        return send(buf);
    }

    size_t send(const unsigned char* data, size_t len)
    {
        auto buf = boost::asio::buffer(data, len);
        return send(buf);
    }

    void async_send(const const_buffer_t& buf, send_handler_t&& fn)
    {
        if (!is_connected()) {
            fn(boost::system::errc::make_error_code(
                   boost::system::errc::not_connected),
               0);
            return;
        }

        sock_->async_send(buf, std::move(fn));
    }

    void async_send(const char* data, size_t len, send_handler_t&& fn)
    {
        auto buf = boost::asio::buffer(data, len);
        return async_send(buf, std::move(fn));
    }

    void async_send(const unsigned char* data, size_t len, send_handler_t&& fn)
    {
        auto buf = boost::asio::buffer(data, len);
        return async_send(buf, std::move(fn));
    }

    size_t recv(multi_buffer_t& buf)
    {
        if (!is_connected()) {
            return 0;
        }

        return sock_->read_some(buf);
    }

    size_t recv(char* data, size_t len)
    {
        multi_buffer_t buf{data, len};
        return recv(buf);
    }

    size_t recv(unsigned char* data, size_t len)
    {
        multi_buffer_t buf{data, len};
        return recv(buf);
    }

    size_t recv_until(streambuf_t& buf, size_t least)
    {
        if (!is_connected()) {
            return 0;
        }

        return boost::asio::read(*sock_, buf, boost::asio::transfer_at_least(least));
    }

    void async_recv(multi_buffer_t& buf, recv_handler_t&& fn)
    {
        if (!is_connected()) {
            fn(boost::system::errc::make_error_code(
                   boost::system::errc::not_connected),
               0);
            return;
        }

        sock_->async_read_some(buf, std::move(fn));
    }

    void async_recv(char* data, size_t len, recv_handler_t&& fn)
    {
        multi_buffer_t buf{data, len};
        async_recv(buf, std::move(fn));
    }

    void async_recv(unsigned char* data, size_t len, recv_handler_t&& fn)
    {
        multi_buffer_t buf{data, len};
        async_recv(buf, std::move(fn));
    }

    inline void add_signal(signal_t sig, signal_handler_t&& fn)
    {
        sigs_.add(sig);
        sigs_.async_wait(fn);
    }

    void remove_signal(signal_t sig)
    {
        sigs_.remove(sig);
    }

    bool set_conn_status(bool is_connected)
    {
        bool old = is_connected_.load();
        return is_connected_.compare_exchange_strong(old, is_connected);
    }

    bool is_connected()
    {
        return is_connected_.load();
    }

    void close()
    {
        disconnect();
        loop_end();
    }

    // WARNNING: this function will be blocked while timeout = 0ms
    // WARNNING: the feature of timeout is not recommended for use in a multi-thread context.
    // See Also: https://www.boost.org/doc/libs/1_80_0/doc/html/boost_asio/example/cpp03/timeouts/blocking_tcp_client.cpp
    size_t recv(multi_buffer_t& buf, std::chrono::milliseconds timeout)
    {
        if (!is_connected()) {
            return 0;
        }

        std::size_t nrecvd = 0;
        std::future<void> f = std::async(std::launch::async, [&]() {
            nrecvd = sock_->read_some(buf);
            return;
        });
        f.wait_for(timeout);
        return nrecvd;
    }

    size_t recv(char* data, size_t len, std::chrono::milliseconds timeout)
    {
        multi_buffer_t buf{data, len};
        return recv(buf, timeout);
    }

    size_t recv(unsigned char* data, size_t len, std::chrono::milliseconds timeout)
    {
        multi_buffer_t buf{data, len};
        return recv(buf, timeout);
    }

private:
    io_t                 io_;
    timer_t              tm_;
    signal_set_t         sigs_;
    sock_t*              sock_     = nullptr;
    std::atomic_bool     is_connected_{false};

};

}

#endif