/*
 程序环境：linux
 程序功能：运行第三方程序，并接收执行后的结果（ping）
*/

#include <iostream>
#include <stdio.h>

#include <string>
#include <sstream>
#include <fstream>

#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>

class IO {
public:
  IO() { readFileContent("test.log"); }
  ~IO() {}

  void wrapSave(const std::string &fileName, const std::string &data) {
    saveContentToFile(fileName, data);
  }

  void readFileContent(const std::string fileName) {
    return;
  }

  void saveContentToFile(const std::string &fileName,
    const std::string &inputData) {
      std::fstream file(fileName.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
      file << inputData;
      file.close();
  }
};

class pinger {
public:
  pinger() : pinger_fp_(NULL) {}

  ~pinger() {
    if (pinger_fp_) {
      pclose(pinger_fp_);
      pinger_fp_ = NULL;
    }
  }

  bool ping(const char *p_ip, const char *p_param) {

    if (pinger_fp_) {
      pclose(pinger_fp_);
      pinger_fp_ = NULL;
      ping_result_ = ping_cmd_ = "";
    }

    //BOOST_ASSERT(p_ip && p_ip[0]);
    std::stringstream ss_cmd;

    ss_cmd << "ping " << p_param << " " << p_ip;
    ping_cmd_ = ss_cmd.str();
    pinger_fp_ = popen(ping_cmd_.c_str(), "r");
    if (pinger_fp_) {
      int fd = fileno(pinger_fp_);
      if (0 > fcntl(fd, F_SETFD, FD_CLOEXEC) ||
        0 > fcntl(fd, F_SETFL, O_NONBLOCK)) {
          pclose(pinger_fp_);
          pinger_fp_ = NULL;
      }
    }

    return NULL != pinger_fp_;
  }

  bool get_result(int &send_packets, int &received_packets,
    int &packet_losss_percent, double &rtt_min, double &rtt_avg,
    double &rtt_max, double &rtt_mdev) {

      send_packets = received_packets = packet_losss_percent = -1;
      rtt_min = rtt_max = rtt_avg = rtt_mdev = -1.0f;

      if (NULL == pinger_fp_) {
        return false;
      }

      // 读取可用数据
      read_result();

      // 分析结果
      if (!feof(pinger_fp_)) {
        return false;
      }

      std::size_t pos = ping_result_.find("ping statistics");
      if (pos != std::string::npos) {
        pos = ping_result_.find('\n', pos);
      }
      if (pos == std::string::npos) {
        return false;
      }

      sscanf(ping_result_.c_str() + pos, "\n%d packets transmitted, %d received",
        &send_packets, &received_packets);

      pos = ping_result_.find("% packet loss", pos);
      if (pos != std::string::npos) {
        while (pos > 0 && std::isdigit(ping_result_[pos - 1])) {
          --pos;
        }
        packet_losss_percent = atoi(ping_result_.c_str() + pos);

        pos = ping_result_.find("min/avg/max/mdev =", pos);
        if (pos != std::string::npos)
          sscanf(ping_result_.c_str() + pos, "min/avg/max/mdev = %lf/%lf/%lf/%lf",
          &rtt_min, &rtt_avg, &rtt_max, &rtt_mdev);
      }

      return true;
  }

  std::string get_ping_result() {
    try {
      if (pinger_fp_) {
        if (!feof(pinger_fp_)) {
          read_result();
        }

        if (feof(pinger_fp_)) {
          return ping_result_;
        }
      }

    } catch (...) {
    }
    return empty_result_;
  }

  std::string const &get_ping_cmd() { return ping_cmd_; }

protected:
  FILE *pinger_fp_;
  std::string ping_result_;
  std::string empty_result_;
  std::string ping_cmd_;

  void read_result() {
    if (pinger_fp_ == NULL) {
      return;
    }

    int fd = fileno(pinger_fp_);

    fd_set readfd;
    FD_ZERO(&readfd);
    FD_SET(fd, &readfd);

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    do {
      int res = select(fd + 1, &readfd, NULL, NULL, &tv);
      if (res < 0) {
        // LOG(WARN, "select() failed: %s", strerror(errno));
        break;
      } else if (res == 0) {
        // LOG(WARN, "select() timeout");
        break;
      } else {
        if (FD_ISSET(fd, &readfd)) {
          char read_buf[2048];
          std::size_t read_len =
            fread(read_buf, 1, sizeof(read_buf) - 1, pinger_fp_);
          if (read_len >= 0) {
            read_buf[read_len] = '\0';
            ping_result_ += read_buf;
          }
          if (feof(pinger_fp_) || ferror(pinger_fp_)) {
            break;
          }
        } else {
          break;
        }
      }
    } while (true);
  }
};

int main() 
{
  bool ping_ret = false;
  pinger pp;
  IO io;
  do {
    ping_ret = pp.ping("127.0.0.1", "-c 10 -w 9.5 -i 0.9");
    if(false == ping_ret) {
      std::cout<<"ping failed."<<std::endl;
      io.wrapSave("err.log", "ping error.");
    } else {
      io.wrapSave("ping_history.log", pp.get_ping_result());
    }
    usleep(10000000);
  } while(true);

  return 0;
}
