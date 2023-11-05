// Copyright 2023 Jeremy Wade
#include "PluginManager.h"
#include <common/IMessageManager.h>

#include <common/Messages/OnPluginInitialized.h>
#include <common/Messages/OnPluginLoaded.h>
#include <common/Messages/OnPluginUnloading.h>

#include <iostream>

std::vector<CPluginManager::SPlugin> CPluginManager::s_pluginRegistrar = {};

extern "C" __declspec(dllexport) Common::ICore* RegisterPlugin(Common::IPlugin* pPlugin)
{
	if (pPlugin)
	{
		CPluginManager::s_pluginRegistrar.push_back(CPluginManager::SPlugin{ pPlugin->GetGUID(), pPlugin->GetName(), pPlugin, NULL, false });
		return Core::Get();
	}

	return nullptr;
}

CPluginManager::CPluginManager(const std::filesystem::path& pluginDirectory)
{
	if (!pluginDirectory.is_absolute())
	{
		std::filesystem::path appRoot = std::filesystem::current_path();
		m_pluginDirectory = appRoot / pluginDirectory;
	}
	else
	{
		m_pluginDirectory = pluginDirectory;
	}

}

CPluginManager::~CPluginManager()
{
	UnloadPlugins();
}

bool CPluginManager::InitializePlugins()
{
	// Register guids in dependency resolver
	for (const SPlugin& plugin : s_pluginRegistrar)
#ifdef _DEBUG
		m_dependencies.RegisterItem(plugin.guid, plugin.szName);
#else
		m_dependencies.RegisterItem(plugin.guid);
#endif

	// Assign dependencies
	for (const SPlugin& plugin : s_pluginRegistrar)
	{
		for (const SGuid& dependency : plugin.pInterface->GetDependencies())
		{
			if (!m_dependencies.ItemExists(dependency))
			{
				std::cout << "Plugin dependency '" << dependency << "' missing for '" << plugin.szName << "'" << std::endl;
				return false;
			}
			else
			{
				m_dependencies.AddDependency(dependency, plugin.guid);
			}
		}
	}

	// Initialize in dependent order
	m_dependencies.VisitTopDown([=](const SDependencyItem& item) {
		auto it = std::find_if(s_pluginRegistrar.begin(), s_pluginRegistrar.end(), [&item](const SPlugin& plugin)
		{
			return item.guid == plugin.guid;
		});

		if (it != s_pluginRegistrar.end())
		{
			if (it->pInterface->Initialize())
			{
				Core::Get()->GetMessageManager()->Broadcast<CPluginManager, Common::SOnPluginInitialized_Message>(it->guid);
			}
		}
	});

	return true;
}

void CPluginManager::LoadPlugins()
{
	if (!std::filesystem::exists(m_pluginDirectory))
		return;

	for (auto const& dir_entry : std::filesystem::directory_iterator{ m_pluginDirectory })
	{
		bool is_directory = dir_entry.is_directory();
		bool is_dll = dir_entry.path().extension() == ".dll";
		if (!is_directory && is_dll)
		{
			LoadPlugin(dir_entry.path());
		}
	}
}

void CPluginManager::UnloadPlugins()
{
	// Unload in reverse dependent order
	m_dependencies.VisitBottomUp([=](const SDependencyItem& item) {
		auto it = std::find_if(s_pluginRegistrar.begin(), s_pluginRegistrar.end(), [&item](const SPlugin& plugin) 
		{
			return item.guid == plugin.guid; 
		});

		if (it != s_pluginRegistrar.end())
		{
			UnloadPlugin(*it);
		}
	});

	m_loadedPlugins.clear();
}

void CPluginManager::LoadPlugin(const std::filesystem::path& library)
{
	const size_t numPlugins = s_pluginRegistrar.size();

	std::string path = library.string();
	HMODULE handle = LoadLibraryA(path.c_str());

#ifdef _DEBUG
	if (handle)
	{
		std::cout << "Loaded plugin " << library  << std::endl;
	}
	else
	{
		std::cout << "Failed to load plugin '" << library << "' (" << GetLastError() << ")" << std::endl;
	}
#endif

	if (handle)
	{
		if (s_pluginRegistrar.size() > numPlugins)
		{
			SPlugin& loadedPlugin = s_pluginRegistrar.back();

			// Latest registration is expected to be this plugin
			loadedPlugin.handle = handle;

			Core::Get()->GetMessageManager()->Broadcast<CPluginManager, Common::SOnPluginLoaded_Message>(loadedPlugin.guid);
		}
		else
		{
			// Plugin failed to register itself. Unload it.
			FreeLibrary(handle);

#ifdef _DEBUG
			std::cout << "Plugin '" << path << "' failed to register itself (Missing REGISTER_PLUGIN macro?)" << std::endl;
#endif
		}
	}
}

void CPluginManager::UnloadPlugin(const SPlugin& plugin)
{
#ifdef _DEBUG
	std::cout << "Unloading plugin '" << plugin.szName << "'" << std::endl;
#endif
	HMODULE handle = plugin.handle;

	Core::Get()->GetMessageManager()->Broadcast<CPluginManager, Common::SOnPluginUnloading_Message>(plugin.guid);
	s_pluginRegistrar.erase(std::remove(s_pluginRegistrar.begin(), s_pluginRegistrar.end(), plugin));
	FreeLibrary(handle);
}

Common::IPlugin* CPluginManager::GetPlugin(const SGuid& guid)
{
	Common::IPlugin* pPlugin = nullptr;

	auto it = std::find_if(s_pluginRegistrar.begin(), s_pluginRegistrar.end(), [&guid](SPlugin& pPluginItem)
	{
		return pPluginItem.guid == guid;
	});

	if (it != s_pluginRegistrar.end())
		pPlugin = it->pInterface;

	return pPlugin;
}
