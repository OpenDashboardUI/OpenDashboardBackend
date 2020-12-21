// Copyright (C) 2020 twyleg
#pragma once

#include <google/protobuf/message_lite.h>

#include <boost/asio.hpp>

#include <cstdint>
#include <array>

namespace OpenDashboard::Common {

class Packet
{

public:

	static constexpr std::size_t PACKET_BUFFER_SIZE = 0xFFFF;

#pragma pack(push)
#pragma pack(8)

	class PacketHeader
	{
	public:
		PacketHeader(const uint32_t numberOfMessages = 0)
		:	mMagicNumber(htonl(0x42)),
			mNumberOfMessages(htonl(numberOfMessages))
		{}

		uint32_t GetNumberOfMessages() const { return ntohl(mNumberOfMessages); }
		void SetNumberOfMessages(const uint32_t numberOfMessages) { mNumberOfMessages = htonl(numberOfMessages); }

	private:
		uint32_t mMagicNumber;
		uint32_t mNumberOfMessages;
	};

	class MessageHeader
	{
	public:
		MessageHeader(const uint32_t messageType, const uint32_t messageLength)
		:	mMessageType(htonl(messageType)),
			mMessageLength(htonl(messageLength))
		{}

		uint32_t GetMessageType() const { return ntohl(mMessageType); }
		uint32_t GetMessageLength() const { return ntohl(mMessageLength); }

		void SetMessageType(const uint32_t messageType) { mMessageType = htonl(messageType); }
		void SetMessageLength(const uint32_t messageLength) { mMessageLength = htonl(messageLength); }

	private:
		uint32_t mMessageType;
		uint32_t mMessageLength;
	};

#pragma pack(pop)

	void AddMessage(const uint32_t mesageType, const void* data, const size_t len);

	char* GetData() { return mPacketBuffer.data(); }
	const char* GetData() const { return mPacketBuffer.data(); }
	size_t GetSize() const { return mBufferUsedLen; }

	size_t GetBufferSize() { return PACKET_BUFFER_SIZE; }

	int GetNumberOfMessages() const { return mPacketHeader->GetNumberOfMessages(); }

protected:

	explicit Packet();

	size_t mBufferUsedLen = sizeof (PacketHeader);
	std::array<char, PACKET_BUFFER_SIZE> mPacketBuffer;

	PacketHeader* mPacketHeader;

};

class OutboundPacket : public Packet
{
public:

	void AddMessage(const uint32_t mesageType, const void* data, const size_t len);
	void AddMessage(const uint32_t mesageType, const google::protobuf::MessageLite& message);

private:

};

class InboundPacket : public Packet
{
public:

	InboundPacket();

	int GetNextMessageType();

	template <class Message>
	Message GetNextMessage();

private:

	MessageHeader* mNextMessageHeader;

};

template <class Message>
Message InboundPacket::GetNextMessage()
{
	char* nextMessage = reinterpret_cast<char*>(mNextMessageHeader) + sizeof(MessageHeader);

	Message message;
	size_t len = mNextMessageHeader->GetMessageLength();
	bool result = message.ParseFromArray(nextMessage, len);

	mNextMessageHeader = reinterpret_cast<MessageHeader*>(nextMessage + len);

	return std::move(message);
}

}
