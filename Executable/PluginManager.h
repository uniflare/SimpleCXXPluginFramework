// Copyright 2023 Jeremy Wade
#pragma once

#include <common/IInterface.h>
#include <common/IPluginManager.h>
#include <common/DependencyManager.h>

#include <string>
#include <filesystem>
#include <map>

#ifdef _DEBUG
#include <iostream>
#endif

extern "C" __declspec(dllexport) Common::ICore * RegisterPlugin(Common::IPlugin * pPlugin);

class CPluginManager : public Common::IPluginManager
{
	DEFINE_INTERFACE("{CCDF7873-467A-4D7B-9E29-7C6090F3DD4B}"_guid, "Plugin Manager");

	struct SPlugin
	{
		SGuid guid;
		const char* szName;
		Common::IPlugin* pInterface;
		HMODULE handle;
		bool isInitialized;

		friend bool operator==(const SPlugin& lhs, const SPlugin& rhs)
		{
			return lhs.guid == rhs.guid;
		}
	};

public:

	friend Common::ICore* RegisterPlugin(Common::IPlugin*);
	static std::vector<SPlugin> s_pluginRegistrar;

public:

	CPluginManager(const std::filesystem::path& pluginDirectory);
	~CPluginManager();

	bool InitializePlugins();
	void LoadPlugins();
	void UnloadPlugins();
	void LoadPlugin(const std::filesystem::path& library);
	void UnloadPlugin(const SPlugin& plugin);

	// IPluginManager
	virtual Common::IPlugin* GetPlugin(const SGuid& guid) override;
	// ~IPluginManager

private:

	CDependencyResolver m_dependencies;
	std::vector<HMODULE> m_loadedPlugins;
	std::filesystem::path m_pluginDirectory;

};