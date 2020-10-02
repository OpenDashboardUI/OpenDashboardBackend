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

	VehicleDynamic outboundVehicleDynamic;
	outboundVehicleDynamic.set_velocity(1.0);
	outboundVehicleDynamic.set_ax(2.0);
	outboundVehicleDynamic.set_ay(3.0);
	outboundVehicleDynamic.set_az(4.0);
	outboundPacket.AddMessage(VEHICLE_DYNAMIC_MSG, outboundVehicleDynamic);

	Gps outboundGps;
	outboundGps.set_lon(50.1234);
	outboundGps.set_lat(7.1234);
	outboundGps.set_elevation(100.123);
	outboundPacket.AddMessage(GPS_MSG, outboundGps);

	std::memcpy(inboundPacket.GetData(), outboundPacket.GetData(), outboundPacket.GetBufferSize());

	EXPECT_EQ(inboundPacket.GetNextMessageType(), VEHICLE_DYNAMIC_MSG);
	VehicleDynamic inboundVehicleDynamic = inboundPacket.GetNextMessage<VehicleDynamic>();
	EXPECT_TRUE(outboundVehicleDynamic == inboundVehicleDynamic);

	EXPECT_EQ(inboundPacket.GetNextMessageType(), GPS_MSG);
	Gps inboundGps = inboundPacket.GetNextMessage<Gps>();
	EXPECT_TRUE(outboundGps == inboundGps);
}

}

