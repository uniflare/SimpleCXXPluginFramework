// Copyright 2023 Jeremy Wade
#pragma once

#include "common/IPlugin.h"
#include "common/ISystem.h"

namespace Plugin2
{
	using namespace guid_parse::literals;

	class IPlugin : public Common::IPlugin, public Common::ISystem
	{
		DEFINE_INTERFACE("{CAE9E879-1F49-45E4-9EB8-4F51C7938DDA}"_guid, "Plugin 2");

	public:

		virtual void TestMessage() = 0;
	};
}