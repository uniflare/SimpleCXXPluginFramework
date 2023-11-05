// Copyright 2023 Jeremy Wade
#pragma once

#include "Guid.h"

namespace Common
{

	class IMessage
	{

	public:

		IMessage(SGuid&& id, const SGuid& senderId, const SGuid& recipientId)
			: m_messageId(std::move(id))
			, m_senderId(senderId)
			, m_recipientId(recipientId)
		{}

		virtual ~IMessage() = default;

		virtual const SGuid& GetId() const { return m_messageId; }
		virtual const SGuid& GetSenderId() const { return m_senderId; }
		virtual const SGuid& GetRecipientId() const { return m_recipientId; }

		virtual const SGuid& GetTypeId() const = 0;
		virtual const void* const GetPayload() const = 0;

	protected:

		SGuid m_messageId;
		SGuid m_senderId;
		SGuid m_recipientId;

	};

}