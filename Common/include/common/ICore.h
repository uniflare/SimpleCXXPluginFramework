// Copyright 2023 Jeremy Wade
#pragma once

namespace Common
{
	class IPluginManager;
	class IMessageManager;

	class ICore
	{

	public:

		virtual IPluginManager* GetPluginManager() = 0;
		virtual IMessageManager* GetMessageManager() = 0;

	};
}

namespace Core
{
	namespace detail {
		extern Common::ICore* g_pCore;
	}

	static Common::ICore* Get()
	{
		return detail::g_pCore;
	}

}