//
// chat_client.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2021 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
// Added Functionality 
// by Syed Razwanul Haque
//

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "chat_message.hpp"

#include "windows.h"

#include "vfw.h"
#include <ctime>
#include <cstdio>
#include <string.h>

#pragma comment(lib, "Vfw32.lib")

using boost::asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;
using namespace std;

string imageName = "intialImageName";


class chat_client
{
public:
    chat_client(boost::asio::io_context& io_context,
        const tcp::resolver::results_type& endpoints)
        : io_context_(io_context),
        socket_(io_context)
    {
        do_connect(endpoints);
    }

    void write(const chat_message& msg)
    {
        boost::asio::post(io_context_,
            [this, msg]()
            {
                bool write_in_progress = !write_msgs_.empty();
                write_msgs_.push_back(msg);
                if (!write_in_progress)
                {
                    do_write();
                }
            });
    }

    void do_read_body()
    {
        
        boost::asio::async_read(socket_,
            boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
            [this](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec)
                {
                    std::cout.write(read_msg_.body(), read_msg_.body_length());
                    std::cout << "\n";
                    takeImage(read_msg_.body());
                    
                    do_read_header();
                }
                else
                {
                    socket_.close();
                }
            });
    }
    void close()
    {
        boost::asio::post(io_context_, [this]() { socket_.close(); });
    }

    void takeImage(string image) {

        imageName = image;
        char inputFromApp[] = "#";
        //string readMsgBody = read_msg_.body();
        string readMsgBody = image;
        string subString = readMsgBody.substr(0, 1);

        srand((unsigned)time(0));
        int randomNumber = rand();
        imageName = to_string(randomNumber);

        if (subString == inputFromApp) {

            cout << "Capturing Image " << '\n';

            // create the preview window 
            HWND hCam = capCreateCaptureWindow(
                L"hoven",
                WS_CHILD,
                0, 0, 0, 0,
                ::GetDesktopWindow(), 0);

            // connect to the first camera 
            // for other cameras try index 
            // 1, 2, in place of the 0 below 
            string imageFormat = ".jpg";

            string outputImageName = imageName + imageFormat;
            wstring formatedImageName;
            for (int i = 0; i < outputImageName.length(); ++i)
                formatedImageName += wchar_t(outputImageName[i]);

            const wchar_t* imageNameOutput = formatedImageName.c_str();
            cout << "Image Name: " << outputImageName << '\n';
            //CameraCapture cam;

            if (capDriverConnect(hCam, 0))
            {
                capFileSaveDIB(hCam, imageNameOutput);
                //printf("Saved");

                try
                {
                    string ip = "192.168.0.108";
                    string port = "5001";

                    boost::asio::io_context io_context1;

                    tcp::resolver resolver(io_context1);
                    auto endpoints = resolver.resolve(ip, port);
                    chat_client c1(io_context1, endpoints);

                    std::thread t1([&io_context1]() { io_context1.run(); });

                    //std::cout << "Init Image Tx" << '\n';
                    string url = "http://192.168.0.108:8000/";
                    string json1 = "{\"imageUrl\" : \"";
                    string json3 = "\"}";
                    string finalJsonImageUrl = json1 + url + imageName + imageFormat + json3;
                    const char* imageNameToSend = finalJsonImageUrl.c_str(); //converson in order to send


                    chat_message msgImage;
                    msgImage.body_length(std::strlen(imageNameToSend));
                    std::memcpy(msgImage.body(), imageNameToSend, msgImage.body_length());
                    msgImage.encode_header();
                    c1.write(msgImage);
                    std::cout << "Sent" << '\n';

                    c1.close();
                    t1.join();
                }
                catch (std::exception& e)
                {
                    std::cerr << "Exception: " << e.what() << "\n";
                }

            }

            else {
                printf("Check camera?");

            }

            DestroyWindow(hCam);
            std::cout << "\n";
        }
        else {
            //std::cout << "Not Matched: " << read_msg_.body() << " Length: " << read_msg_.body_length()  <<"\n";
            std::cout << "\n";
        }
    }

private:
   
    void do_connect(const tcp::resolver::results_type& endpoints)
    {   
        
        boost::asio::async_connect(socket_, endpoints,
            [this](boost::system::error_code ec, tcp::endpoint)
            {
                if (!ec)
                {
                    do_read_header();
                }
            });
    }

    void do_read_header()
    {
        
        boost::asio::async_read(socket_,
            boost::asio::buffer(read_msg_.data(), chat_message::header_length),
            [this](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec && read_msg_.decode_header())
                {
                    do_read_body();
                }
                else
                {
                    socket_.close();
                }
            });
    }

   

    void do_write()
    {
        boost::asio::async_write(socket_,
            boost::asio::buffer(write_msgs_.front().data(),
                write_msgs_.front().length()),
            [this](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec)
                {
                    write_msgs_.pop_front();
                    if (!write_msgs_.empty())
                    {
                        do_write();
                    }
                }
                else
                {
                    socket_.close();
                }
            });
    }

private:
    boost::asio::io_context& io_context_;
    tcp::socket socket_;
    chat_message read_msg_;
    chat_message_queue write_msgs_;
};

class CameraCapture {
public:
    string imageName;
    

};


int main(int argc, char* argv[])
{
    CameraCapture cameraCapture;
    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage: chat_client <host> <port>\n";
            return 1;
        }

        boost::asio::io_context io_context;

        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(argv[1], argv[2]);
        chat_client c(io_context, endpoints);

        std::thread t([&io_context]() { io_context.run(); });

        char line[chat_message::max_body_length + 1];
        while (std::cin.getline(line, chat_message::max_body_length + 1) || !cin.eof())
        {
            chat_message msg;
            msg.body_length(std::strlen(line));
            std::memcpy(msg.body(),line, msg.body_length());
            
            msg.encode_header();
            c.write(msg);
        }
       
        c.close();
        t.join();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}