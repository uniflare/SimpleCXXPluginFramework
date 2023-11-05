// Copyright 2023 Jeremy Wade
#pragma once

#include "IInterface.h"
#include "IMessage.h"

namespace Common
{
	class ISystem : public IInterface<ISystem>
	{

	public:

		virtual void OnMessage(const IMessage& message) = 0;

	};
}