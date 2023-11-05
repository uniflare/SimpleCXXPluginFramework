// Copyright 2023 Jeremy Wade
#include <common/Platform.h>

#include "IPlugin.h"
#include <Messages/TestMessage.h>

#include <common/IMessageManager.h>

#include <iostream>

namespace Plugin3
{

	class CPlugin3 : public IPlugin
	{

	public:

		// Plugin3::IPlugin
		virtual bool Initialize() override
		{
			std::cout << "Initializing Plugin 3" << std::endl;
			Core::Get()->GetMessageManager()->RegisterSystem(this);
			return true;
		}

		virtual const std::vector<SGuid>& GetDependencies() const override
		{
			static const std::vector<SGuid> dependencies {};
			return dependencies;
		}

		virtual ISystem* GetSystem() override
		{
			return this;
		}

		virtual void TestMessage() override
		{
			std::cout << "Hello from Plugin 3" << std::endl;
		}
		// ~Plugin3::IPlugin

		// Common::ISystem
		virtual void OnMessage(const Common::IMessage& message) override
		{
			if (message.GetTypeId() == CTest_Message::GUID())
			{
				std::cout << "Message Received: " << static_cast<const CTest_Message::SPayload*>(message.GetPayload())->text << std::endl;
			}
		}
		// Common::ISystem

	};

}

REGISTER_INTERFACE(Plugin3::CPlugin3);