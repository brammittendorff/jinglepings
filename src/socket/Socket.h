#ifndef JINGLEPINGS_SERVER_SOCKET_H
#define JINGLEPINGS_SERVER_SOCKET_H

#include <string>
#include <sys/socket.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/ipv6.h>

#include "../controller/JingleController.h"

class SocketException : public std::runtime_error {
public:
    SocketException(std::string const &msg) :
            std::runtime_error(msg) {}
};

class Socket {
private:
    // Buffer and timing parameters
    static const size_t msg_count = 1024;
    static const size_t buf_size = sizeof(struct ethhdr) + sizeof(struct ipv6hdr);
    timespec timeout = {.tv_sec = 0, .tv_nsec = 1000000};

    // Socket file descriptor
    int sockfd;

public:
    Socket() {}

    Socket(std::string interface_name);

    void run(JingleController &controller);

    void receivePixels(mmsghdr *msgs, JingleController &controller);

private:
    int getInterfaceNumber(std::string &interface_name);
};

#endif //JINGLEPINGS_SERVER_SOCKET_H
