
boost::thread th(boost::bind(&boost::asio::io_service::run, &io_service));
th.detach();