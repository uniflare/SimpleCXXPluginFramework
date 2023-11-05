// Copyright 2023 Jeremy Wade
#pragma once

#include "common/IPlugin.h"
#include "common/ISystem.h"

namespace Plugin3
{
	using namespace guid_parse::literals;

	class IPlugin : public Common::IPlugin, public Common::ISystem
	{
		DEFINE_INTERFACE("{2038299A-CFFF-46F5-AF44-D48CBCED3898}"_guid, "Plugin 3");

	public:

		virtual void TestMessage() = 0;
	};
}