#pragma once

#include "udp_transceiver.h"
#include "packet.h"

#include <open_dashboard.pb.h>

#include <string>
#include <memory>


namespace OpenDashboard::Common {


class PackageManager
{

public:

	void InitTransmitter(const std::string& hostname, unsigned int port);
	void InitReceiver(const std::string& hostname, unsigned int port);

	void TransmitPacket(const OutboundPacket&);

	InboundPacket HandleIncomingPackets();

private:

	std::unique_ptr<UdpTransmitter> mUdpTransmitter;
	std::unique_ptr<UdpReceiver> mUdpReceiver;


};

}
