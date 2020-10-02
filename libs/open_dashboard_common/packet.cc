// Copyright (C) 2020 twyleg
#include "packet.h"
#include "helper.h"

#include <iostream>

namespace OpenDashboard::Common {

Packet::Packet()
:	mPacketHeader(new(reinterpret_cast<PacketHeader*>(mPacketBuffer.data()))PacketHeader)
{}

void OutboundPacket::AddMessage(const uint32_t messageType, const void* data, const size_t len)
{
	const size_t requiredSpace = len + sizeof(MessageHeader);
	THROW_IF ((mBufferUsedLen + requiredSpace) > PACKET_BUFFER_SIZE, "Buffer overflow: {} + {} > {}", mBufferUsedLen,
			requiredSpace, PACKET_BUFFER_SIZE);

	MessageHeader* messageHeader = new (mPacketBuffer.data() + mBufferUsedLen) MessageHeader(messageType, len);

	void* payload = mPacketBuffer.data() + mBufferUsedLen + sizeof(MessageHeader);
	std::memcpy(payload, data, len);

	mBufferUsedLen += requiredSpace;
	mPacketHeader->SetNumberOfMessages(mPacketHeader->GetNumberOfMessages() + 1);
}

void OutboundPacket::AddMessage(const uint32_t messageType, const google::protobuf::MessageLite& message)
{
	const size_t len = message.ByteSizeLong();
	const size_t requiredSpace = len + sizeof(MessageHeader);
	THROW_IF ((mBufferUsedLen + requiredSpace) > PACKET_BUFFER_SIZE, "Buffer overflow: {} + {} > {}", mBufferUsedLen,
			requiredSpace, PACKET_BUFFER_SIZE);

	MessageHeader* messageHeader = new (mPacketBuffer.data() + mBufferUsedLen) MessageHeader(messageType, len);

	void* payload = mPacketBuffer.data() + mBufferUsedLen + sizeof(MessageHeader);
	message.SerializeToArray(payload, len);

	mBufferUsedLen += requiredSpace;
	mPacketHeader->SetNumberOfMessages(mPacketHeader->GetNumberOfMessages() + 1);
}

InboundPacket::InboundPacket()
:	Packet(),
	mNextMessageHeader(reinterpret_cast<MessageHeader*>(mPacketBuffer.data() + sizeof(PacketHeader)))
{}

int InboundPacket::GetNextMessageType()
{
	return mNextMessageHeader->GetMessageType();
}

}
