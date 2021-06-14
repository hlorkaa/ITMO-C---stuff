#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <signal.h>

#include "util.h"

void write_dir_contents(int socket_fd, const char* dir_name) {

    DIR* d = opendir(dir_name);
    if (!d)
        return;

    struct dirent* dir;
    while ((dir = readdir(d)) != NULL) {
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
            continue;
        if (write_all(socket_fd, dir->d_name, strlen(dir->d_name)) == -1)
            return;
        if (write_all(socket_fd, "\n", 1) == -1)
            return;
    }
    closedir(d);
}

void worker(int socket_fd) {

    char buffer[4098];
    size_t len = 0;
    int bytes_read;
    while (1) {
        if (len == sizeof(buffer)) {
            fprintf(stderr, "insufficient buffer\n");
            break;
        }
        bytes_read = read(socket_fd, buffer + len, sizeof(buffer) - len);
        if (bytes_read == 0)
            break;
        if (bytes_read == -1) {
            perror("error reading from socket");
            break;
        }
        char* cur = buffer;
        len += bytes_read;
        while (cur < buffer + len) {
            char* next = memchr(cur, '\r', buffer + len - cur);
            if (next == 0 || next + 1 == buffer + len) {
                memmove(buffer, cur, buffer + len - cur);
                break;
            }
            if (next == cur) {
                return;
            }

            *next = '\0';
            write_dir_contents(socket_fd, cur);
            cur = next + 2;
        }
        len = buffer + len - cur;
    }
}

int main() {

    struct sockaddr_in6 address;
    memset(&address, 0, sizeof(address));
    address.sin6_family = AF_INET6;
    address.sin6_port = htons(PORT_NUM);
    address.sin6_addr = in6addr_any;

    int listen_socket_fd = socket(AF_INET6, SOCK_STREAM, 0);
    if (listen_socket_fd == -1) {
        perror("failed to create socket");
        return 1;
    }
    if (bind(listen_socket_fd, (const struct sockaddr*)&address, sizeof(address)) == -1) {
        perror("failed to bind socket");
        return 1;
    }
    if (listen(listen_socket_fd, 10) == -1) {
        perror("failed to start listening");
        return 1;
    }

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_flags |= SA_NOCLDWAIT;
    sa.sa_handler = SIG_DFL;
    sigaction(SIGCHLD, &sa, NULL);

    while (1) {
        int socket_fd = accept(listen_socket_fd, NULL, NULL);
        if (socket_fd == -1) {
            perror("failed to accept connection");
            return 1;
        }
        int pid = fork();
        if (pid == -1)
            perror("failed to fork");
        else if (pid == 0) {
            worker(socket_fd);
            close(socket_fd);
            return 0;
        }
        else
            close(socket_fd);
    }
}
