// Copyright 2023 Jeremy Wade
#pragma once

#include "../IMessage.h"
#include "../IInterface.h"

namespace Common
{

	using namespace guid_parse::literals;

	class SOnPluginUnloading_Message : public IMessage
	{
		DEFINE_INTERFACE("{38E3E547-28AB-46D0-BEFC-079D319DE6A4}"_guid, "Message::OnPluginUnloading")

	public:

		struct SPayload
		{
			const SGuid unloadedPlugin;
		};

		SOnPluginUnloading_Message(const SGuid& sender, const SGuid& recipient, const SGuid& unloadedPlugin)
			: Common::IMessage(GenerateGuid(), sender, recipient)
			, m_payload(SPayload{ unloadedPlugin })
		{}

		virtual ~SOnPluginUnloading_Message() override = default;

		// Common::IMessage
		virtual const SGuid& GetTypeId() const { return GetGUID(); }
		virtual const void* const GetPayload() const { return &m_payload; }
		// ~Common::IMessage

	private:

		const SPayload m_payload;

	};

}