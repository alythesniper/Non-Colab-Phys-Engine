#pragma once
#include <boost/asio.hpp>
#include <vector>
#include <string>

class ArrayReceiver
{
public:
    ArrayReceiver(boost::asio::io_service& io_service, short port);
    void start();
    void handle_read(const boost::system::error_code& error, std::size_t bytes_transferred);
    
    int recv_count = 0;
    std::vector<std::vector<float>> big_array_;

    bool collideState = false;

private:
    boost::asio::ip::tcp::socket socket_;
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::streambuf recv_buffer_;
    boost::asio::io_service::work work_;
};
