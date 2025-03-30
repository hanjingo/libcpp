#include <gtest/gtest.h>
#include <libcpp/net/tcp.hpp>

// TEST(tcp_socket, sig_catch)
// {
//     static int sigill_lambda_entryed_times = 0;
//     static int sigint_lambda_entryed_times = 0;
//     static int last_sig = 0;

//     libcpp::tcp_socket::io_t io;
//     libcpp::tcp_socket sock{io};

//     sock.signal_catch({SIGILL}, [](const libcpp::tcp_socket::err_t& err, libcpp::tcp_socket::signal_t sig)->bool{
//         std::cout << "sigill_lambda_entryed_times++" << std::endl;
//         sigill_lambda_entryed_times++;
//         last_sig = sig;
//         return true;
//     }, true);

//     raise(SIGILL);
//     io.run();
//     ASSERT_EQ(sigill_lambda_entryed_times == 1, true);
//     ASSERT_EQ(sigint_lambda_entryed_times == 0, true);
//     ASSERT_EQ(last_sig == SIGILL, true);

//     raise(SIGILL);
//     io.run();
//     ASSERT_EQ(sigill_lambda_entryed_times == 1, true);
//     ASSERT_EQ(sigint_lambda_entryed_times == 0, true);
//     ASSERT_EQ(last_sig == SIGILL, true);

//     sock.signal_catch({SIGINT, SIGILL}, [](const libcpp::tcp_socket::err_t& err, libcpp::tcp_socket::signal_t sig)->bool{
//         std::cout << "sigint_lambda_entryed_times++" << std::endl;
//         sigint_lambda_entryed_times++;
//         last_sig = sig;
//         return true;
//     }, true);
//     raise(SIGINT);
//     io.run();
//     ASSERT_EQ(sigill_lambda_entryed_times == 1, true);
//     ASSERT_EQ(sigint_lambda_entryed_times == 1, true);
//     ASSERT_EQ(last_sig == SIGINT, true);

//     raise(SIGILL);
//     io.run();
//     ASSERT_EQ(sigill_lambda_entryed_times == 1, true);
//     ASSERT_EQ(sigint_lambda_entryed_times == 1, true);
//     ASSERT_EQ(last_sig == SIGINT, true);

//     sock.signal_catch({SIGINT, SIGILL}, [](const libcpp::tcp_socket::err_t& err, libcpp::tcp_socket::signal_t sig)->bool{
//         std::cout << "sigint_lambda_entryed_times++" << std::endl;
//         sigint_lambda_entryed_times++;
//         last_sig = sig;
//         return true;
//     }, false);
//     raise(SIGINT);
//     io.run();
//     ASSERT_EQ(sigill_lambda_entryed_times == 1, true);
//     ASSERT_EQ(sigint_lambda_entryed_times == 2, true);
//     ASSERT_EQ(last_sig == SIGINT, true);

//     raise(SIGILL);
//     io.run();
//     ASSERT_EQ(sigill_lambda_entryed_times == 1, true);
//     ASSERT_EQ(sigint_lambda_entryed_times == 3, true);
//     ASSERT_EQ(last_sig == SIGILL, true);

//     raise(SIGTERM);
//     io.run();
//     ASSERT_EQ(sigill_lambda_entryed_times == 1, true);
//     ASSERT_EQ(sigint_lambda_entryed_times == 3, true);
//     ASSERT_EQ(last_sig == SIGILL, true);
// }

TEST(tcp_socket, set_option)
{
    libcpp::tcp_socket::io_t io;
    libcpp::tcp_socket sock{io};
    ASSERT_EQ(sock.set_option(libcpp::tcp_socket::opt_no_delay(true)), false);
    ASSERT_EQ(sock.set_option(libcpp::tcp_socket::opt_send_buf_sz(1024)), false);
    ASSERT_EQ(sock.set_option(libcpp::tcp_socket::opt_recv_buf_sz(1024)), false);
    ASSERT_EQ(sock.set_option(libcpp::tcp_socket::opt_reuse_addr(true)), false);
    ASSERT_EQ(sock.set_option(libcpp::tcp_socket::opt_keep_alive(false)), false);
    ASSERT_EQ(sock.set_option(libcpp::tcp_socket::opt_broadcast(false)), false);

    libcpp::tcp_socket::sock_t* base1 = new libcpp::tcp_socket::sock_t(io);
    libcpp::tcp_socket sock1{io, base1};
    ASSERT_EQ(sock1.set_option(libcpp::tcp_socket::opt_no_delay(true)), false);
    ASSERT_EQ(sock1.set_option(libcpp::tcp_socket::opt_send_buf_sz(1024)), false);
    ASSERT_EQ(sock1.set_option(libcpp::tcp_socket::opt_recv_buf_sz(1024)), false);
    ASSERT_EQ(sock1.set_option(libcpp::tcp_socket::opt_reuse_addr(true)), false);
    ASSERT_EQ(sock1.set_option(libcpp::tcp_socket::opt_keep_alive(false)), false);
    ASSERT_EQ(sock1.set_option(libcpp::tcp_socket::opt_broadcast(false)), false);

    libcpp::tcp_socket::sock_t* base2 = new libcpp::tcp_socket::sock_t(io);
    base2->open(boost::asio::ip::tcp::v4());
    libcpp::tcp_socket sock2{io, base2};
    ASSERT_EQ(sock2.set_option(libcpp::tcp_socket::opt_no_delay(true)), true);
    ASSERT_EQ(sock2.set_option(libcpp::tcp_socket::opt_send_buf_sz(1024)), true);
    ASSERT_EQ(sock2.set_option(libcpp::tcp_socket::opt_recv_buf_sz(1024)), true);
    ASSERT_EQ(sock2.set_option(libcpp::tcp_socket::opt_reuse_addr(true)), true);
    ASSERT_EQ(sock2.set_option(libcpp::tcp_socket::opt_keep_alive(false)), true);
    ASSERT_EQ(sock2.set_option(libcpp::tcp_socket::opt_broadcast(false)), true);
}

TEST(tcp_socket, connect)
{
    static int accept_times = 0;
    std::thread t([]() {
        libcpp::tcp_socket::io_t io;
        libcpp::tcp_listener li{io};
        for (int i = 0; i < 3; i++)
        {
            auto sock = li.accept(10091);
            ASSERT_EQ(sock != nullptr, true);
            sock->close();
            delete sock;
            accept_times++;
        }
        li.close();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    libcpp::tcp_socket::io_t io;
    libcpp::tcp_socket sock{io};
    ASSERT_EQ(sock.connect("127.0.0.1", 10091), true);
    
    libcpp::tcp_socket sock1{io};
    ASSERT_EQ(sock1.connect("127.0.0.1", 10091), true);

    libcpp::tcp_socket sock2{io};
    ASSERT_EQ(sock2.connect("127.0.0.1", 10091), true);

    t.join();
    ASSERT_EQ(accept_times == 3, true);
}

TEST(tcp_socket, async_connect)
{
    libcpp::tcp_socket::io_t io;
    static libcpp::tcp_listener li{io};
    li.async_accept(10092, [](const libcpp::tcp_listener::err_t& err, libcpp::tcp_socket* sock){
        ASSERT_EQ(err.failed(), false);
        ASSERT_EQ(sock != nullptr, true);
        li.async_accept(10092, [](const libcpp::tcp_listener::err_t& err, libcpp::tcp_socket* sock){
            ASSERT_EQ(err.failed(), false);
            ASSERT_EQ(sock != nullptr, true);
        });
    });

    libcpp::tcp_socket sock{io};
    static bool lambda1_entryed = false;
    sock.async_connect("127.0.0.1", 10092, 
        [](const libcpp::tcp_socket::err_t& err, libcpp::tcp_socket* sock) {
            ASSERT_EQ(!err.failed(), true);
            ASSERT_EQ(sock != nullptr, true);
            lambda1_entryed = true;
    });

    static bool lambda2_entryed = false;
    libcpp::tcp_socket sock1{io};
    sock1.async_connect("127.0.0.1", 10092, 
        [](const libcpp::tcp_socket::err_t& err, libcpp::tcp_socket* sock) {
            ASSERT_EQ(!err.failed(), true);
            ASSERT_EQ(sock != nullptr, true);
            lambda2_entryed = true;
    });
    io.run();
    li.close();

    ASSERT_EQ(lambda1_entryed, true);
    ASSERT_EQ(lambda2_entryed, true);
}

TEST(tcp_socket, disconnect)
{
    std::thread t([]() {
        libcpp::tcp_socket::io_t io;
        libcpp::tcp_listener li{io};
        for (int i = 0; i < 1; i++)
        {
            auto sock = li.accept(10091);
            ASSERT_EQ(sock != nullptr, true);
        }
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    libcpp::tcp_socket::io_t io;
    libcpp::tcp_socket sock{io};
    
    ASSERT_EQ(sock.is_connected(), false);
    sock.disconnect();
    ASSERT_EQ(sock.is_connected(), false);
    ASSERT_EQ(sock.connect("127.0.0.1", 10091), true);
    ASSERT_EQ(sock.is_connected(), true);
    sock.disconnect();
    ASSERT_EQ(sock.is_connected(), false);
    sock.disconnect();
    ASSERT_EQ(sock.is_connected(), false);

    t.join();
}

TEST(tcp_socket, send)
{
    std::thread t([]() {
        libcpp::tcp_socket::io_t io;
        libcpp::tcp_listener li{io};
        for (int i = 0; i < 2; i++)
        {
            auto sock = li.accept(10091);
            ASSERT_EQ(sock != nullptr, true);
            char buf[1024];
            ASSERT_EQ(sock->recv(buf, 1024) == 6, true);
            std::string str(buf, 5);

            if (i == 0)
                ASSERT_EQ(str == std::string("hello"), true);
            else if (i == 1)
                ASSERT_EQ(str == std::string("harry"), true);
            else
                ASSERT_EQ(true, false);

            sock->close();
        }
    });

    libcpp::tcp_socket::io_t io;
    libcpp::tcp_socket sock{io};
    ASSERT_EQ(sock.connect("127.0.0.1", 10091), true);
    ASSERT_EQ(sock.send(std::string("hello").c_str(), 6) == 6, true);

    libcpp::tcp_socket sock1{io};
    ASSERT_EQ(sock1.connect("127.0.0.1", 10091), true);
    ASSERT_EQ(sock1.send(std::string("harry").c_str(), 6) == 6, true);
    t.join();
}

TEST(tcp_socket, async_send)
{
    // std::thread t([](){
    //     libcpp::tcp_socket::io_t io;
    //     libcpp::tcp_listener li{io};
    //     for (int i = 0; i < 1; i++)
    //     {
    //         li.async_accept(10091, [](const libcpp::tcp_listener::err_t& err, libcpp::tcp_socket* sock) {
    //             ASSERT_EQ(err.failed(), false);
    //             ASSERT_EQ(sock->is_connected(), true);
    //             char buf[1024] = {0};
    //             sock->async_recv(buf, 1024, [buf, sock](const libcpp::tcp_listener::err_t & err, std::size_t sz) {
    //                 std::cout << "err.what()=" << err.what() << std::endl;
    //                 ASSERT_EQ(err.failed(), false);
    //                 ASSERT_EQ(sz == 6, true);
    //                 std::string str(buf, sz - 1);
    //                 ASSERT_EQ(str == std::string("hello"), true);

    //                 char buf1[1024] = {0};
    //                 sock->async_recv(buf1, 1024, [buf1](const libcpp::tcp_listener::err_t & err, std::size_t sz) {
    //                     ASSERT_EQ(err.failed(), false);
    //                     ASSERT_EQ(sz == 6, true);
    //                     std::string str(buf1, sz - 1);
    //                     ASSERT_EQ(str == std::string("harry"), true);
    //                 });
    //             });
    //         });
    //     }

    //     io.run();
    // });

    // libcpp::tcp_socket::io_t io;
    // libcpp::tcp_socket sock{io};
    // sock.async_connect("127.0.0.1", 10091, [](const libcpp::tcp_socket::err_t& err, libcpp::tcp_socket* sock){
    //     ASSERT_EQ(err.failed(), false);
    //     sock->async_send(std::string("hello").c_str(), 6, [sock](const libcpp::tcp_socket::err_t& err, std::size_t sz){
    //         ASSERT_EQ(err.failed(), false);
    //         ASSERT_EQ(sz == 6, true);

    //         sock->async_send(std::string("harry").c_str(), 6, [](const libcpp::tcp_socket::err_t& err, std::size_t sz){
    //             ASSERT_EQ(err.failed(), false);
    //             ASSERT_EQ(sz == 6, true);
    //         });
    //     });
    // });

    // io.run();
    // t.join();
}

TEST(tcp_socket, recv)
{

}

TEST(tcp_socket, recv_until)
{

}

TEST(tcp_socket, async_recv)
{

}

TEST(tcp_socket, set_conn_status)
{

}

TEST(tcp_socket, is_connected)
{

}

TEST(tcp_socket, close)
{

}