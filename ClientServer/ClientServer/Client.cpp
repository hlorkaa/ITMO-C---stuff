#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <string.h>
#include <netinet/ip.h>
#include <netdb.h>

#include "util.h"

int main(int argc, char** argv) {

    if (argc < 3) {
        printf("insufficient arguments\n");
        return 1;
    }

    struct addrinfo* host_info, hint;
    hint.ai_family = AF_UNSPEC;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_flags = AI_PASSIVE;
    hint.ai_protocol = 0;
    hint.ai_canonname = NULL;
    hint.ai_addr = NULL;
    hint.ai_next = NULL;

    char port_str[10];
    sprintf(port_str, "%d", PORT_NUM);
    int res = getaddrinfo(argv[1], port_str, &hint, &host_info);
    if (res != 0) {
        printf("failed to find host: %s\n", gai_strerror(res));
        return 1;
    }

    int socket_fd = socket(host_info->ai_family, SOCK_STREAM, host_info->ai_protocol);
    if (socket_fd == -1) {
        perror("failed to create socket");
        return 1;
    }
    if (connect(socket_fd, host_info->ai_addr, host_info->ai_addrlen) == -1) {
        perror("failed to connect to server");
        return 1;
    }
    freeaddrinfo(host_info);

    for (int i = 2; i < argc; ++i) {
        if (write_all(socket_fd, argv[i], strlen(argv[i])) == -1)
            return 1;
        if (write_all(socket_fd, "\r\n", 2) == -1)
            return 1;
    }
    if (write_all(socket_fd, "\r\n", 2) == -1)
        return 1;

    char buffer[4096];
    int bytes_read;
    while (1) {
        bytes_read = read(socket_fd, buffer, sizeof(buffer));
        if (bytes_read == -1) {
            perror("error reading from socket");
            return 1;
        }
        if (bytes_read == 0) {
            break;
        }
        printf("%.*s", bytes_read, buffer);
    }
    return 0;
}
