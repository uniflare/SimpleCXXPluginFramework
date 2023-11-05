// Copyright 2023 Jeremy Wade
#pragma once

#include "Guid.h"
#include "ICore.h"
#include "IInterface.h"
#include "ISystem.h"

#include <memory>
#include <vector>

namespace Common
{
	class IPlugin;
	using TRegistrarFunc = Common::ICore* (*)(IPlugin*);

	template<class TInterface>
	class _PluginRegistrator
	{
	public:
		_PluginRegistrator()
			: m_pThis(nullptr)
		{
			HINSTANCE _module = GetModuleHandleA(NULL);
			Common::TRegistrarFunc _registrarFunc = (Common::TRegistrarFunc)GetProcAddress(GetModuleHandleA(0), "RegisterPlugin");
			m_pThis = std::make_unique<TInterface>();
			Core::detail::g_pCore = _registrarFunc(m_pThis.get());
		}
	private:
		std::unique_ptr<TInterface> m_pThis;
	};

#define REGISTER_INTERFACE(Interface)								        \
		static auto _registrator = Common::_PluginRegistrator<Interface>();

	class IPlugin : public IInterface<IPlugin>
	{

	public:

		virtual bool Initialize() = 0;
		virtual const std::vector<SGuid>& GetDependencies() const = 0;
		virtual ISystem* GetSystem() = 0;

	};
}