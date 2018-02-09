//打印线程id 代码片段
//打印线程id、当前时间
void writer() {
  while (--write_max > -1) {
    boost::mutex::scoped_lock sl(m);
    if (number == 5) {
      not_full.wait(m);
    }
    ++number;
    log_array << "after writer: " << number << ", time:"
              << boost::posix_time::to_simple_string(
                     boost::posix_time::microsec_clock::local_time())
              << ", this_thread:" << boost::this_thread::get_id() << std::endl;

    not_empty.notify_one();
  }
}