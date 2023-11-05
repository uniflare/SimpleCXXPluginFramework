// Copyright 2023 Jeremy Wade
#pragma once

#include "IMessage.h"
#include "ISystem.h"

#include <map>

#ifdef _DEBUG
#include <iostream>
#endif

using TMessageFunc = void(*)(const Common::IMessage&);

namespace Common
{
	class IMessageManager
	{

	public:

		template<class TSender, class TRecipient, class TMessageType, typename ... TArgs>
		void Send(TArgs&&... args)
		{
			auto pMessage = std::make_unique<TMessageType>(TSender::GUID(), TRecipient::GUID(), std::forward<TArgs>(args)...);
			Send(*pMessage.get());
		}

		template<class TSender, class TMessageType, typename ... TArgs>
		void Broadcast(TArgs&&... args)
		{
			auto pMessage = std::make_unique<TMessageType>(TSender::GUID(), EmptyGuid, std::forward<TArgs>(args)...);
			Send(*pMessage.get());
		}

		virtual void Send(const IMessage& message) = 0;
		virtual void RegisterSystem(ISystem* system) = 0;

	};
}