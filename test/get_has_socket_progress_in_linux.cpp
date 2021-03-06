#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <stdio.h>
#include <cstdio>
#include <map>

#include <cerrno>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <cctype>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

bool bughuntmode;
const int MAX_PID_LENGTH = 20;
const int MAX_FDLINK = 10;

struct prg_node {
  long inode;
  pid_t pid;
  std::string name;
};

std::map<unsigned long, prg_node *> inodeproc;

static std::string read_file(int fd) {
  char buf[255];
  std::string content;

  for (int length; (length = read(fd, buf, sizeof(buf))) > 0;) {
    if (length < 0) {
      std::fprintf(stderr, "Error reading file: %s\n", std::strerror(errno));
      exit(34);
    }
    content.append(buf, length);
  }

  return content;
}

static std::string read_file(const char *filepath) {
  int fd = open(filepath, O_RDONLY);

  if (fd < 0) {
    std::fprintf(stderr, "Error opening %s: %s\n", filepath,
      std::strerror(errno));
    exit(3);
    return NULL;
  }

  std::string contents = read_file(fd);

  if (close(fd)) {
    std::fprintf(stderr, "Error opening %s: %s\n", filepath,
      std::strerror(errno));
    exit(34);
  }

  return contents;
}

std::string getprogname(pid_t pid) {
  const int maxfilenamelen = 14 + MAX_PID_LENGTH + 1;
  char filename[maxfilenamelen];

  std::snprintf(filename, maxfilenamelen, "/proc/%d/cmdline", pid);
  return read_file(filename);
}

unsigned long str2ulong(const char *ptr) {
  unsigned long retval = 0;

  while ((*ptr >= '0') && (*ptr <= '9')) {
    retval *= 10;
    retval += *ptr - '0';
    ptr++;
  }
  return retval;
}

void setnode(unsigned long inode, pid_t pid) {
  prg_node *current_value = inodeproc[inode];

  if (current_value == NULL || current_value->pid != pid) {
    prg_node *newnode = new prg_node;
    newnode->inode = inode;
    newnode->pid = pid;
    newnode->name = getprogname(pid);

    inodeproc[inode] = newnode;
    delete current_value;
  }
}

int str2int(const char *ptr) {
  int retval = 0;

  while ((*ptr >= '0') && (*ptr <= '9')) {
    retval *= 10;
    retval += *ptr - '0';
    ptr++;
  }
  return retval;
}

void get_info_by_linkname(const char *pid, const char *linkname) {
  if (strncmp(linkname, "socket:[", 8) == 0) {
    setnode(str2ulong(linkname + 8), str2int(pid));
  }
}

bool is_number(const char *string) {
  while (*string) {
    if (!isdigit(*string))
      return false;
    string++;
  }
  return true;
}

void get_info_for_pid(const char *pid) {
  char dirname[10 + MAX_PID_LENGTH];

  size_t dirlen = 10 + strlen(pid);
  snprintf(dirname, dirlen, "/proc/%s/fd", pid);

  DIR *dir = opendir(dirname);

  if (!dir) {
    if (bughuntmode) {
      std::cout << "Couldn't open dir " << dirname << ": " << strerror(errno)
        << "\n";
    }
    return;
  }

  /* walk through /proc/%s/fd/... */
  dirent *entry;
  while ((entry = readdir(dir))) {
    if (entry->d_type != DT_LNK)
      continue;
    // std::cout << "Looking at: " << entry->d_name << std::endl;

    size_t fromlen = dirlen + strlen(entry->d_name) + 1;
    char fromname[10 + MAX_PID_LENGTH + 1 + MAX_FDLINK];
    snprintf(fromname, fromlen, "%s/%s", dirname, entry->d_name);

    // std::cout << "Linking from: " << fromname << std::endl;

    int linklen = 80;
    char linkname[linklen];
    int usedlen = readlink(fromname, linkname, linklen - 1);
    if (usedlen == -1) {
      continue;
    }
    //assert(usedlen < linklen);
    linkname[usedlen] = '\0';
    get_info_by_linkname(pid, linkname);
  }
  closedir(dir);
}

void reread_mapping() {
  DIR *proc = opendir("/proc");

  if (proc == 0) {
    std::cerr << "Error reading /proc, needed to get inode-to-pid-maping\n";
    exit(1);
  }

  dirent *entry;

  while ((entry = readdir(proc))) {
    if (entry->d_type != DT_DIR)
      continue;

    if (!is_number(entry->d_name))
      continue;

    get_info_for_pid(entry->d_name);
  }

  closedir(proc);
}

int main(int argc, char* argv[])
{
  reread_mapping();

  return 0;
}
