#ifndef LAB3_UTIL_H
#define LAB3_UTIL_H

#include <unistd.h>


#define PORT_NUM 2221

int write_all(int fd, const void* buf, size_t n) {

    const char* cur = buf;
    size_t left = n;
    while (left != 0) {
        int bytes_written = write(fd, cur, left);
        if (bytes_written == -1) {
            perror("error writing to socket");
            return -1;
        }
        left -= bytes_written;
        cur += bytes_written;
    }
    return 0;
}

#endif //LAB3_UTIL_H

