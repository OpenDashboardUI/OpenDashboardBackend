// Copyright (C) 2020 twyleg
#pragma once

#include "helper.h"

#include <boost/asio.hpp>
#include <string>

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

	virtual ~UdpSocket() = default;

	size_t DataAvailable();

	void EnableReuseAddr();
	void SetNonBlocking();

protected:

	UdpSocket(const std::string& hostname, int port);

	const std::string mHostname;
	const int mPort;

	boost::asio::ip::basic_endpoint<boost::asio::ip::udp> mUdpEndpoint;

	boost::asio::io_service mService;
	boost::asio::ip::udp::socket mSocket;
};

class UdpTransmitter : public UdpSocket
{
public:

	UdpTransmitter(const std::string& hostname, int port);

	size_t Transmit(const char* data, const size_t);
};


class UdpReceiver : public UdpSocket
{
public:

	UdpReceiver(const std::string& hostname, int port);

	size_t Receive(char* data, const size_t);
};

}
