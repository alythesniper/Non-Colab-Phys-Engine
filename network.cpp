#include "network.h"
#include <sstream>
#include <boost/bind.hpp>
#include <iostream>

ArrayReceiver::ArrayReceiver(boost::asio::io_service& io_service, short port)
    : socket_(io_service), acceptor_(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), work_(io_service)
{
}

void ArrayReceiver::start()
{
    std::cout << "Waiting for connection..." << std::endl;
    acceptor_.async_accept(socket_,
        [this](boost::system::error_code ec)
        {
            if (!ec)
            {
                boost::asio::async_read_until(socket_, recv_buffer_, "\n",
                    boost::bind(&ArrayReceiver::handle_read, this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
            }
        });
}

void ArrayReceiver::handle_read(const boost::system::error_code& error, std::size_t bytes_transferred)
{
    if (!error)
    {
        /*
        std::vector<float> array; 
        for (std::size_t i = 0; i < bytes_transferred; i += sizeof(float))  
        {
            float number;
            std::memcpy(&number, static_cast<const char*>(boost::asio::buffer_cast<const void*>(recv_buffer_.data())) + i, sizeof(float));
            array.push_back(number);
        }
        recv_buffer_.consume(bytes_transferred * sizeof(float));
        big_array_.push_back(array);
        recv_count++;
        */

        std::vector<float> array1;  
        std::vector<float> array2; 
        for (std::size_t i = 0; i < bytes_transferred; i += sizeof(float)) 
        {
            float number;
            std::memcpy(&number, static_cast<const char*>(boost::asio::buffer_cast<const void*>(recv_buffer_.data())) + i, sizeof(float));
            if (i < 6 * sizeof(float)) {
                array1.push_back(number);
            }
            else {
                array2.push_back(number);
            }
        }
        recv_buffer_.consume(bytes_transferred * sizeof(float));
        big_array_.push_back(array1);
        big_array_.push_back(array2);
        recv_count += 2;

        /*print out big_array
		std::cout << "Received array: ";
		for (auto& array : big_array_)
		{
			for (auto& number : array)
			{
			    std::cout << number << " ";
			}
			std::cout << std::endl;
		}
        */

        boost::asio::write(socket_, boost::asio::buffer("Successfull\n"));
        socket_.close();
        start(); // Start waiting for the next connection
    }
    else
    {
		std::cerr << "Error: " << error.message() << std::endl;
    }
}