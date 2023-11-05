// Copyright 2023 Jeremy Wade
#pragma once

#include <common/IInterface.h>
#include <common/ISystem.h>
#include <common/IMessageManager.h>

class CMessageManager : public Common::IMessageManager, public Common::ISystem
{

	DEFINE_INTERFACE("{FCF4290B-EFEF-4979-A0BE-768E597E7FF9}"_guid, "Message Manager");

public:

	CMessageManager();
	~CMessageManager();

	// Common::ISystem
	virtual void OnMessage(const Common::IMessage& message) override;
	// Common::ISystem

	// Common::IMessageManager
	virtual void RegisterSystem(Common::ISystem* system) override;
	virtual void Send(const Common::IMessage& message) override;
	// ~Common::IMessageManager

private:

	std::map<SGuid, Common::ISystem*> m_systems;

};