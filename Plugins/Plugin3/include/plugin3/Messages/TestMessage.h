// Copyright 2023 Jeremy Wade
#pragma once

#include <common/IInterface.h>
#include <common/IMessage.h>

namespace Plugin3
{

	using namespace guid_parse::literals;

	class CTest_Message : public Common::IMessage
	{
		DEFINE_INTERFACE("{93C0BE19-4C23-48DC-BD70-01A639A1C246}"_guid, "Test Message")

	public:

		struct SPayload
		{
			const std::string text;
		};

		CTest_Message(const SGuid& sender, const SGuid& recipient, std::string&& text)
			: Common::IMessage(GenerateGuid(), sender, recipient)
			, m_payload(SPayload{ std::move(text) })
		{}

		virtual ~CTest_Message() override = default;

		// Common::IMessage
		virtual const SGuid& GetTypeId() const { return GetGUID(); }
		virtual const void* const GetPayload() const { return &m_payload; }
		// ~Common::IMessage

	private:

		const SPayload m_payload;

	};

}