// Copyright 2023 Jeremy Wade
#pragma once

#include "../IMessage.h"
#include "../IInterface.h"

namespace Common
{

	using namespace guid_parse::literals;

	class SOnPluginLoaded_Message : public IMessage
	{
		DEFINE_INTERFACE("{FD1397CB-1D30-4581-856C-59C86FF0EFA0}"_guid, "Message::OnPluginLoaded")

	public:

		struct SPayload
		{
			const SGuid loadedPlugin;
		};

		SOnPluginLoaded_Message(const SGuid& sender, const SGuid& recipient, const SGuid& loadedPlugin)
			: Common::IMessage(GenerateGuid(), sender, recipient)
			, m_payload(SPayload{ loadedPlugin })
		{}

		virtual ~SOnPluginLoaded_Message() override = default;

		// Common::IMessage
		virtual const SGuid& GetTypeId() const { return GetGUID(); }
		virtual const void* const GetPayload() const { return &m_payload; }
		// ~Common::IMessage

	private:

		const SPayload m_payload;

	};

}