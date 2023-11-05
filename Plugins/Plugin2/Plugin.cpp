// Copyright 2023 Jeremy Wade
#include <common/Platform.h>

#include "IPlugin.h"
#include <iostream>

#include <plugin1/IPlugin.h>
#include <plugin3/IPlugin.h>
#include <common/IPluginManager.h>
#include <common/IMessageManager.h>

namespace Plugin2
{

	class CPlugin2 : public IPlugin
	{

	public:

		virtual bool Initialize() override
		{
			std::cout << "Initializing Plugin 2" << std::endl;

			Core::Get()->GetMessageManager()->RegisterSystem(this);

			Plugin1::IPlugin* pPlugin1 = Core::Get()->GetPluginManager()->GetPlugin<Plugin1::IPlugin>();
			pPlugin1->TestMessage();

			Plugin3::IPlugin* pPlugin3 = Core::Get()->GetPluginManager()->GetPlugin<Plugin3::IPlugin>();
			pPlugin3->TestMessage();

			return true;
		}

		virtual const std::vector<SGuid>& GetDependencies() const override
		{
			static const std::vector<SGuid> dependencies
			{
				Plugin1::IPlugin::GUID(),
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
			std::cout << "Hello from Plugin 2" << std::endl;
		}

		// Common::ISystem
		virtual void OnMessage(const Common::IMessage& message) override
		{
		}
		// Common::ISystem

	};

}

REGISTER_INTERFACE(Plugin2::CPlugin2);