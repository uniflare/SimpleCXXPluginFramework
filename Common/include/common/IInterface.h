// Copyright 2023 Jeremy Wade
#pragma once

#include "Guid.h"

#define DEFINE_INTERFACE(guid, name)									  \
	public:																  \
		static constexpr const char*  Name() { return _NAME; }            \
		static constexpr const SGuid& GUID() { return _GUID; }            \
		virtual const char*  GetName() const { return Name(); }			  \
		virtual const SGuid& GetGUID() const { return GUID(); }			  \
	private:															  \
		static constexpr SGuid _GUID{guid};								  \
		static constexpr const char* _NAME = name;  

namespace Common
{

	template<class TConcrete>
	class IInterface
	{

	public:

		virtual const char* GetName() const = 0;
		virtual const SGuid& GetGUID() const = 0;


	};

}