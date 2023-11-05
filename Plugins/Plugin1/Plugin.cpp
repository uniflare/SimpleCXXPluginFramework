// Copyright 2023 Jeremy Wade
#include <common/Platform.h>

#include "IPlugin.h"
#include <iostream>

#include <plugin3/IPlugin.h>
#include <plugin3/Messages/TestMessage.h>
#include <common/IPluginManager.h>
#include <common/IMessageManager.h>

namespace Plugin1
{

	class CPlugin1 : public IPlugin
	{

	public:

		virtual bool Initialize() override
		{
			std::cout << "Initializing Plugin 1" << std::endl;

			Core::Get()->GetMessageManager()->RegisterSystem(this);

			Plugin3::IPlugin* pPlugin3 = Core::Get()->GetPluginManager()->GetPlugin<Plugin3::IPlugin>();
			pPlugin3->TestMessage();

			Core::Get()->GetMessageManager()->Send<IPlugin, Plugin3::IPlugin, Plugin3::CTest_Message>("Test Message from Plugin 1 to Plugin 3");

			return true;
		}

		virtual const std::vector<SGuid>& GetDependencies() const override
		{
			static const std::vector<SGuid> dependencies
			{
				Plugin3::IPlugin::GUID()
			};

			return dependencies;
		}

		virtual ISystem* GetSystem() override
		{
			return this;
		}

		virtual void TestMessage() override
		{
			std::cout << "Hello from Plugin 1" << std::endl;
		}

		// Common::ISystem
		virtual void OnMessage(const Common::IMessage& message) override
		{
		}
		// Common::ISystem

	};
}

REGISTER_INTERFACE(Plugin1::CPlugin1);