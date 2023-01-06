#pragma once

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

class Server {
public:
    Server(uint16_t port) : port_(port) {
        memset(&server_addr_, 0, sizeof server_addr_);
        server_addr_.sin_family = AF_INET;
        server_addr_.sin_addr.s_addr = htonl(INADDR_ANY);

        if ((fd_ = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
            perror("socket error");
        }
        int opt = 1;
        setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, (const void *) &opt, sizeof(opt));
    }

    uint16_t bind() {
        for (server_addr_.sin_port = htons(port_); port_ < 60000; ++port_) {
            if (::bind(fd_, (struct sockaddr *) &server_addr_, sizeof server_addr_) >= 0) {
                printf("bind success %d", port_);
                return port_;
            } else {
                printf("bind error port: %d", port_);
            }
        }
        return -1;
    }

    void listen() {
        if (::listen(fd_, SOMAXCONN) < 0) {
            perror("listen error");
        }
    }

    int accept() {
        struct sockaddr_in peeraddr;
        socklen_t peerlen = sizeof peeraddr;
        int connfd;
        if ((connfd = ::accept(fd_, (struct sockaddr *) &peeraddr, &peerlen)) < 0) {
            perror("accept error");
        }
        return connfd;
    }

    ssize_t read(int fd, void *buf, size_t count) {
        return ::read(fd, buf, count);
    }

    ssize_t write(int fd, const void *buf, size_t count) {
        return ::write(fd, buf, count);
    }

    ~Server() {
        close(fd_);
    }

    int get_fd() const {
        return fd_;
    }

private:
    int fd_;
    uint16_t port_;
    struct sockaddr_in server_addr_;
};
