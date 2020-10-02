// Copyright (C) 2020 twyleg
#include "udp_transceiver.h"
#include "helper.h"

#include <cstdlib>

namespace OpenDashboard::Common {

UdpSocket::UdpSocket(const std::string& hostname, int port)
:	mHostname(hostname),
	mPort(port),
	mSockfd(socket(AF_INET, SOCK_DGRAM, 0))
{
	if (mSockfd == -1)
		throw UdpSocketException(mHostname, mPort, "Unable to create socket");
}

UdpSocket::~UdpSocket()
{
	close(mSockfd);
}

void UdpSocket::EnableReuseAddr()
{
	const char value = 1;
	if (setsockopt(mSockfd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(int)) < 0)
		throw UdpSocketException(mHostname, mPort, "Unable to make address reusable");
}

void UdpSocket::EnableMulticast()
{
	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr(mHostname.c_str());
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);

	if (setsockopt(mSockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
		throw UdpSocketException(mHostname, mPort, "Unable to enable multicast");
}

void UdpSocket::SetTtlLimit(const char ttlLimit)
{
	if (setsockopt(mSockfd, IPPROTO_IP, IP_MULTICAST_TTL, &ttlLimit, sizeof(int)) < 0)
		throw UdpSocketException(mHostname, mPort, FORMAT("Unable to set TTL limit to {}", ttlLimit));
}

void UdpSocket::SetNonBlocking()
{
	int flags = fcntl(this->mSockfd, F_GETFL);
	flags |= O_NONBLOCK;
	fcntl(this->mSockfd, F_SETFL, flags);
}


void UdpSocket::EnableLoopback(bool loopbackEnabled)
{
	const char state = loopbackEnabled ? 1 : 0;
	setsockopt(mSockfd, IPPROTO_IP, IP_MULTICAST_LOOP, &state, sizeof(state));
}


UdpReceiver::UdpReceiver(const std::string& hostname, int port)
:	UdpSocket(hostname, port)
{
	memset(&mSockAddr, 0, sizeof(mSockAddr));
	mSockAddr.sin_family = AF_INET;
	mSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	mSockAddr.sin_port = htons(port);

	if (bind(mSockfd, reinterpret_cast<struct sockaddr*>(&mSockAddr), sizeof(mSockAddr)) < 0)
		throw UdpSocketException(mHostname, mPort, "Unable to bind socket");
}

UdpReceiver::~UdpReceiver()
{
	shutdown(mSockfd, SHUT_RD);
}

ssize_t UdpReceiver::Receive(char *data, const size_t size)
{
	socklen_t addrlen = sizeof(struct sockaddr_in);
	const ssize_t len = recvfrom(mSockfd, data, size, 0, reinterpret_cast<struct sockaddr*>(&mSockAddr), &addrlen);
	return len;
}


UdpTransmitter::UdpTransmitter(const std::string& hostname, int port)
:	UdpSocket(hostname, port)
{
	mSockAddr.sin_family = AF_INET;
	mSockAddr.sin_port = htons(port);
	mSockAddr.sin_addr.s_addr = inet_addr(hostname.c_str());
}

UdpTransmitter::~UdpTransmitter()
{
	shutdown(mSockfd, SHUT_WR);
}

ssize_t UdpTransmitter::Transmit(const char *data, const size_t t_size)
{
	const ssize_t len = sendto(mSockfd, data, t_size, 0, reinterpret_cast<struct sockaddr*>(&mSockAddr), sizeof(struct sockaddr_in));
	return len;
}


}
