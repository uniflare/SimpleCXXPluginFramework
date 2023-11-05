// Copyright 2023 Jeremy Wade
#pragma once

#include "common/IPlugin.h"
#include "common/ISystem.h"

namespace Plugin1
{
	using namespace guid_parse::literals;

	class IPlugin : public Common::IPlugin, public Common::ISystem
	{
		DEFINE_INTERFACE("{B0F83F20-0868-4D7E-B2DA-A1F0FBD5C26D}"_guid, "Plugin 1");

	public:

		virtual void TestMessage() = 0;

	};
}