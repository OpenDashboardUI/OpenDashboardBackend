// Copyright (C) 2020 twyleg
#include "udp_transceiver.h"
#include "helper.h"

#include <cstdlib>

namespace OpenDashboard::Common {

using namespace boost::asio;

UdpSocket::UdpSocket(const std::string& hostname, int port)
:	mHostname(hostname),
	mPort(port),
	mUdpEndpoint(ip::udp::endpoint(ip::address::from_string(mHostname), mPort)),
	mSocket(mService)
{
	mSocket.open(boost::asio::ip::udp::v4());
}

size_t UdpSocket::DataAvailable()
{
	return mSocket.available();
}

void UdpSocket::EnableReuseAddr()
{
	mSocket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
}

void UdpSocket::SetNonBlocking()
{
	mSocket.non_blocking(true);
}

UdpReceiver::UdpReceiver(const std::string& hostname, int port)
:	UdpSocket(hostname, port)
{
	mSocket.bind(mUdpEndpoint);
}

size_t UdpReceiver::Receive(char *data, const size_t size)
{
	return mSocket.receive_from(boost::asio::buffer(data, size), mUdpEndpoint);
}

UdpTransmitter::UdpTransmitter(const std::string& hostname, int port)
:	UdpSocket(hostname, port)
{}

size_t UdpTransmitter::Transmit(const char *data, const size_t size)
{
	return mSocket.send_to(boost::asio::buffer(data, size), mUdpEndpoint);
}

}
