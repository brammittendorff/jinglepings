/*
 * Copyright (C) 2019 Studenten Net Twente
 *
 * Licensed under the EUPL. See the file LICENSE.md in the top level
 * directory for more details.
 */

#include <iostream>
#include <cstring>

#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/ipv6.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include "Socket.h"

//! Constructor for socket.
//! \param interface_name network interface name to listen on.
Socket::Socket(std::string interface_name) {
    // Create socket
    sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_IPV6));
    if (sockfd == -1) {
        throw SocketException("Error opening socket");
    }

    // Resolve interface
    auto interface_number = getInterfaceNumber(interface_name);
    if (interface_number < 0) {
        throw SocketException("Unable to resolve interface");
    }

    // Bind to IPv6
    struct sockaddr_ll saddr = {
            .sll_family = AF_PACKET,
            .sll_protocol = htons(ETH_P_IPV6),
            .sll_ifindex = interface_number,
    };
    if (bind(sockfd, (struct sockaddr *) &saddr, sizeof(saddr)) == -1) {
        throw SocketException("Unable to bind on socket");
    }
}

//! Run the socket for a single controller.
//! This retrieves pixels from the socket and passes them to the given controller.
//! \param controller controller to send pixels to.
void Socket::run(JingleController &controller) {
    // Initialize buffers
    struct mmsghdr msgs[msg_count];
    struct iovec iovecs[msg_count];
    char bufs[msg_count][buf_size + 1];

    memset(msgs, 0, sizeof(msgs));
    for (int i = 0; i < msg_count; i++) {
        iovecs[i].iov_base = bufs[i];
        iovecs[i].iov_len = buf_size;
        msgs[i].msg_hdr.msg_iov = &iovecs[i];
        msgs[i].msg_hdr.msg_iovlen = 1;
    }

    // Receive packets
    while (true) {
        receivePixels(msgs, controller);
    }
}

//! Retrieve pixels (in the form of ICMPv6 messages) and send them to the controller.
//! \param msgs buffer to store messages in.
//! \param controller controller to send pixel information to.
void Socket::receivePixels(mmsghdr *msgs, JingleController &controller) {
    auto res = recvmmsg(sockfd, msgs, msg_count, 0, &timeout);
    if (res == -1) {
        throw SocketException("Error receiving from socket: " + std::to_string(res));
    }

    for (int i = 0; i < res; i++) {
        // Ignore messages with insufficient size
        if (msgs[i].msg_hdr.msg_iovlen == 0 ||
            msgs[i].msg_hdr.msg_iov->iov_len < sizeof(struct ethhdr) + sizeof(struct ipv6hdr)) {
            continue;
        }

        // Get ethernet frame and IPv6 packet pointers
        auto frame = (struct ethhdr *) msgs[i].msg_hdr.msg_iov->iov_base;
        auto packet = (struct ipv6hdr *) (frame + 1);

        // Ignore everything that isn't ICMPv6
        if (packet->nexthdr != IPPROTO_ICMPV6) {
            continue;
        }

        auto src = packet->saddr.in6_u;
        auto dst = packet->daddr.in6_u;
        auto x = ntohs(dst.u6_addr16[4]);
        auto y = ntohs(dst.u6_addr16[5]);
        auto color = dst.u6_addr32[3];
        auto sid = (uint64_t) ntohl(src.u6_addr32[0]) << 32u | ntohl(src.u6_addr32[1]);

        controller.drawPixel(sid, y, x, color);
    }
}

//! Get the interface number corresponding to an interface name.
//! \param interface_name interface name.
//! \return interface number.
int Socket::getInterfaceNumber(std::string &interface_name) {
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));

    if (interface_name.size() > IFNAMSIZ) {
        return -1;
    }

    strncpy(ifr.ifr_name, interface_name.c_str(), sizeof(ifr.ifr_name));

    if (ioctl(sockfd, SIOCGIFINDEX, &ifr) == -1) {
        return -1;
    }

    return ifr.ifr_ifindex;
}

