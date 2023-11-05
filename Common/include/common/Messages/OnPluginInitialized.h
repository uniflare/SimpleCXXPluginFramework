// Copyright 2023 Jeremy Wade
#pragma once

#include "../IMessage.h"
#include "../IInterface.h"

namespace Common
{

	using namespace guid_parse::literals;

	class SOnPluginInitialized_Message : public IMessage
	{
		DEFINE_INTERFACE("{189EC068-81D3-4918-8FC3-A4955B423DD4}"_guid, "Message::OnPluginInitialized")

	public:

		struct SPayload
		{
			const SGuid initializedPlugin;
		};

		SOnPluginInitialized_Message(const SGuid& sender, const SGuid& recipient, const SGuid& initializedPlugin)
			: Common::IMessage(GenerateGuid(), sender, recipient)
			, m_payload(SPayload{ initializedPlugin })
		{}

		virtual ~SOnPluginInitialized_Message() override = default;

		// Common::IMessage
		virtual const SGuid& GetTypeId() const { return GetGUID(); }
		virtual const void* const GetPayload() const { return &m_payload; }
		// ~Common::IMessage

	private:

		const SPayload m_payload;

	};

}