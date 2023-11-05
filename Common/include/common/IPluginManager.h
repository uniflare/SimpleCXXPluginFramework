// Copyright 2023 Jeremy Wade
#pragma once

#include "IPlugin.h"
#include "IMessage.h"

namespace Common
{

	class IPluginManager
	{

	public:

	public:

		template <class T>
		T* GetPlugin()
		{
			return static_cast<T*>(GetPlugin(T::GUID()));
		}

		virtual IPlugin* GetPlugin(const SGuid& guid) = 0;

	};
}