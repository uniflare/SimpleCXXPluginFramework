// Copyright 2023 Jeremy Wade
#include <iostream>

#include "PluginManager.h"
#include "MessageManager.h"
#include <common/ISystem.h>

class CCore : public Common::ICore
{

public:

	CCore()
		: m_pPluginManager(nullptr)
	{
		m_pPluginManager = std::make_unique<CPluginManager>("plugins");
		m_pMessageManager = std::make_unique<CMessageManager>();
	}

	~CCore()
	{
		m_pMessageManager.reset();
		m_pPluginManager.reset();
	}

	// ICore
	virtual Common::IPluginManager* GetPluginManager() override { return m_pPluginManager.get(); }
	virtual Common::IMessageManager* GetMessageManager() override { return m_pMessageManager.get(); }
	// ~ICore

private:

	std::unique_ptr<CPluginManager> m_pPluginManager;
	std::unique_ptr<CMessageManager> m_pMessageManager;

};

int main()
{
	std::cout << "Main Entry\n";

	CCore core;
	Core::detail::g_pCore = &core;

	CPluginManager* pPluginManager = static_cast<CPluginManager*>(Core::Get()->GetPluginManager());
	pPluginManager->LoadPlugins();
	pPluginManager->InitializePlugins();
	pPluginManager->UnloadPlugins();

	std::cout << "Main Exit\n";
}