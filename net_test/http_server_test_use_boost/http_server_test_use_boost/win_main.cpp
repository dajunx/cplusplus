//
// win_main.cpp
// ~~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/asio.hpp>
#include "server.hpp"

#if defined(_WIN32)

boost::function0<void> console_ctrl_function;

BOOL WINAPI console_ctrl_handler(DWORD ctrl_type)
{
  switch (ctrl_type)
  {
  case CTRL_C_EVENT:
  case CTRL_BREAK_EVENT:
  case CTRL_CLOSE_EVENT:
  case CTRL_SHUTDOWN_EVENT:
    console_ctrl_function();
    return TRUE;
  default:
    return FALSE;
  }
}

int main(int argc, char* argv[])
{
  try
  {
    // Check command line arguments.
//     if (argc != 4)
//     {
//       std::cerr << "Usage: http_server <address> <port> <doc_root>\n";
//       std::cerr << "  For IPv4, try:\n";
//       std::cerr << "    http_server 0.0.0.0 80 .\n";
//       std::cerr << "  For IPv6, try:\n";
//       std::cerr << "    http_server 0::0 80 .\n";
//       return 1;
//     }

    boost::filesystem::path p(__FILE__);
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::socket socket(io_service);
    boost::asio::ip::tcp::endpoint remote_ep = socket.remote_endpoint();
    boost::asio::ip::address remote_ad = remote_ep.address();
    std::string str_ip = remote_ad.to_string();

    // Initialise server.
    //http::server::server s(argv[1], argv[2], argv[3]);
    // ·ÃÎÊ http://172.20.24.105:9999/ReadMe.txt
    http::server::server s(str_ip, "9999", p.parent_path().string()); //²âÊÔ£¬ÌîÐ´¹Ì¶¨ip + port + file_path

    // Set console control handler to allow server to be stopped.
    console_ctrl_function = boost::bind(&http::server::server::stop, &s);
    SetConsoleCtrlHandler(console_ctrl_handler, TRUE);

    // Run the server until stopped.
    s.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "exception: " << e.what() << "\n";
  }

  return 0;
}

#endif // defined(_WIN32)
