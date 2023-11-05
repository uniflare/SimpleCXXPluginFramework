// Copyright 2023 Jeremy Wade
#include "MessageManager.h"

#include <common/ICore.h>
#include <common/IPluginManager.h>
#include <common/Messages/OnPluginUnloading.h>

CMessageManager::CMessageManager()
{
	RegisterSystem(this);
}

CMessageManager::~CMessageManager()
{
	m_systems.clear();
}

void CMessageManager::OnMessage(const Common::IMessage& message)
{
	if (message.GetTypeId() == Common::SOnPluginUnloading_Message::GUID())
	{
		const auto& payload = static_cast<const Common::SOnPluginUnloading_Message::SPayload*>(message.GetPayload());
		Common::IPlugin* pPlugin = Core::Get()->GetPluginManager()->GetPlugin(payload->unloadedPlugin);
		Common::ISystem* pSystem = pPlugin->GetSystem();

		auto it = m_systems.find(pSystem->GetGUID());
		if (it != m_systems.end())
		{
			m_systems.erase(it);
		}
	}
}

void CMessageManager::RegisterSystem(Common::ISystem* system)
{
	auto it = m_systems.find(system->GetGUID());
	if (it == m_systems.end())
	{
		m_systems[system->GetGUID()] = system;
	}
#ifdef _DEBUG
	else
	{
		std::cout << "System with GUID has already been registered (" << system->GetGUID() << ")" << std::endl;
	}
#endif
}

void CMessageManager::Send(const Common::IMessage& message)
{
	if (message.GetRecipientId() == EmptyGuid)
	{
		// Copy in-case a system is erased during broadcast
		const std::map<SGuid, Common::ISystem*> systems = m_systems;
		for (auto& system : systems)
		{
			system.second->OnMessage(message);
		}
	}
	else
	{
		auto it = m_systems.find(message.GetRecipientId());
		if (it != m_systems.end())
		{
			it->second->OnMessage(message);
		}
#ifdef _DEBUG
		else
		{
			std::cout << "Tried sending a message to '" << message.GetRecipientId() << "' but the recipient was not registered." << std::endl;
		}
#endif
	}
}