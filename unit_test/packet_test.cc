// Copyright (C) 2020 twyleg
#include <gtest/gtest.h>

#include <open_dashboard_common/packet.h>

#include <open_dashboard.pb.h>

#include <iostream>
#include <sstream>
#include <cassert>
#include <cstring>

namespace OpenDashboard::testing {

bool operator==(const google::protobuf::MessageLite& msg_a,
		const google::protobuf::MessageLite& msg_b) {
	return (msg_a.GetTypeName() == msg_b.GetTypeName()) &&
		(msg_a.SerializeAsString() == msg_b.SerializeAsString());
}

TEST(PacketTest, SingleMessagePacket_PackUnpackRawData_DataEqual)
{
	Common::OutboundPacket outboundPacket;
	Common::InboundPacket inboundPacket;

	Proto::Dynamics outboundDynamics;
	outboundDynamics.set_velocity(1.0);
	outboundDynamics.set_ax(2.0);
	outboundDynamics.set_ay(3.0);
	outboundDynamics.set_az(4.0);
	outboundPacket.AddMessage(Proto::DYNAMICS_MSG, outboundDynamics);

	Proto::Gps outboundGps;
	outboundGps.set_lon(50.1234);
	outboundGps.set_lat(7.1234);
	outboundGps.set_elevation(100.123);
	outboundPacket.AddMessage(Proto::GPS_MSG, outboundGps);

	std::memcpy(inboundPacket.GetData(), outboundPacket.GetData(), outboundPacket.GetBufferSize());

	EXPECT_EQ(inboundPacket.GetNextMessageType(), Proto::DYNAMICS_MSG);
	Proto::Dynamics inboundVehicleDynamic = inboundPacket.GetNextMessage<Proto::Dynamics>();
	EXPECT_TRUE(outboundDynamics == inboundVehicleDynamic);

	EXPECT_EQ(inboundPacket.GetNextMessageType(), Proto::GPS_MSG);
	Proto::Gps inboundGps = inboundPacket.GetNextMessage<Proto::Gps>();
	EXPECT_TRUE(outboundGps == inboundGps);
}

}

