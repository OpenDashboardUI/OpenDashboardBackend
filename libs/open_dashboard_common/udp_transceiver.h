// Copyright (C) 2020 twyleg
#pragma once

#include "helper.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <string>

namespace OpenDashboard::Common {

class UdpSocket
{

public:

	class UdpSocketException : std::exception
	{
	public:

		UdpSocketException(const std::string& hostname, int port, const std::string& message)
		:	mMessage(FORMAT("UdpSocketException: hostname={}, port={}, message={}", hostname, port, message))
		{}

		const char* what() const noexcept override
		{
			return mMessage.c_str();
		}

	private:

		const std::string mMessage;
	};

	virtual ~UdpSocket();

	void EnableReuseAddr();
	void EnableMulticast();
	void SetTtlLimit(const char ttlLimit);

	void SetNonBlocking();
	void EnableLoopback(bool loopbackEnabled);

protected:

	UdpSocket(const std::string& hostname, int port);

	const std::string mHostname;
	const int mPort;
	const int mSockfd;

	struct sockaddr_in mSockAddr;
};

class UdpTransmitter : public UdpSocket
{
public:

	UdpTransmitter(const std::string& hostname, int port);
	~UdpTransmitter();

	ssize_t Transmit(const char* data, const size_t size);
};


class UdpReceiver : public UdpSocket
{
public:

	UdpReceiver(const std::string& hostname, int port);
	~UdpReceiver();

	ssize_t Receive(char* data, const size_t size);
};

}
