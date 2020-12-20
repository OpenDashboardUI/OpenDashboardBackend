// Copyright (C) 2020 twyleg
#include "package_manager.h"

namespace OpenDashboard::Common {

void PackageManager::InitTransmitter(const std::string& hostname, unsigned int port)
{
	mUdpTransmitter = std::make_unique<UdpTransmitter>(hostname, port);
}

void PackageManager::InitReceiver(const std::string& hostname, unsigned int port)
{
	mUdpReceiver = std::make_unique<UdpReceiver>(hostname, port);
}

void PackageManager::TransmitPacket(const OutboundPacket& outboundPacket)
{
	mUdpTransmitter->Transmit(outboundPacket.GetData(), outboundPacket.GetSize());
}


InboundPacket PackageManager::HandleIncomingPackets()
{
	while (mUdpReceiver->DataAvailable())
	{
		OpenDashboard::Common::InboundPacket packet;

		const size_t len = mUdpReceiver->Receive(packet.GetData(), packet.GetBufferSize());

		for (int i=0; i<packet.GetNumberOfMessages(); ++i)
		{
			Proto::MessageType messageType = static_cast<Proto::MessageType>(packet.GetNextMessageType());

			switch (messageType)
			{
			case Proto::CONTROL_MSG:
			{
				auto msg = packet.GetNextMessage<Proto::Control>();
				mDataModel.GetControlDataDynamic()->SetState(static_cast<DataModels::ControlDataDynamic::State>(msg.state()));
				break;
			}
			case Proto::DYNAMICS_MSG:
			{
				auto msg = packet.GetNextMessage<Proto::Dynamics>();
				mDataModel.GetVehicleData()->GetDynamics()->FromProto(msg);
				break;
			}
			case Proto::GPS_MSG:
			{
				auto msg = packet.GetNextMessage<Proto::Gps>();
				mDataModel.GetVehicleData()->GetGps()->FromProto(msg);
				break;
			}
			case Proto::POWERTRAIN_MSG:
			{
				auto msg = packet.GetNextMessage<Proto::Powertrain>();
				mDataModel.GetVehicleData()->GetPowertrain()->FromProto(msg);
				break;
			}
			case Proto::DRIVER_INPUT_MSG:
			{
				auto msg = packet.GetNextMessage<Proto::DriverInput>();
				mDataModel.GetVehicleData()->GetDriverInput()->FromProto(msg);
				break;
			}
			default:
				break;
			}

			mDataModel.SerializeQuantities();
		}

	}
}

}
